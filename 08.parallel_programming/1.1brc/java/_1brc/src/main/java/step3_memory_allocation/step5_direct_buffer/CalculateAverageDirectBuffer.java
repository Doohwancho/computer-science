package step3_memory_allocation.step5_direct_buffer;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;
import java.util.Map;
import java.util.TreeMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.ArrayList;
import java.util.List;

/**
 * ---
 * what
 *
 * 다른 memory allocation 테크닉인 direct buffer (memory mapping 말고)
 *
 * step3-3이랑 다른 부분 동일한데, memory mapping하는 부분만 direct buffer로 바꿈.
 * before)
 * `MappedByteBuffer buffer = channel.map(FileChannel.MapMode.READ_ONLY, position, size);`
 *
 * after)
 * ```
 * ByteBuffer directBuffer = ByteBuffer.allocateDirect(bufferSize);
 * channel.position(position);
 * int bytesRead = channel.read(directBuffer);
 * directBuffer.flip();
 * ```
 *
 * ---
 * 실험결과
 *
 * baseline) 195.28s user 5.10s system 98% cpu 3:22.42 total
 * step3-1_buffered reader) 198.69s user 6.25s system 94% cpu 3:37.38 total
 * step3-4_memory_map + chunking + streaming) 278.95s user 15.91s system 459% cpu 1:04.14 total
 * step3-5_direct_buffer) 327.66s user 5.19s system 616% cpu 53.973 total
 *
 *
 * ---
 * Q. memory mapping이 direct buffer보다 더 빨라야 하는거 아냐?
 *
 * 왜냐면 일단 일반적인 i/o가 어떻게 이루어지는지 보면
 * 1. 커널이 디스크에서 데이터 읽어서
 * 2. 커널 버퍼에 복사하고,
 * 3. 다시 그걸 JVM 힙으로 복사하는 식으로 2번 복사가 일어남.
 * 엄청 비효율적.
 *
 * 그런데 memory mapping은 아예 파일을 가상 메모리 공간에 그냥 "매핑"해버림.
 * 대충 disk에 데이터 주소를 매핑시켜서 데이터 복사해서 가져오는게 아니라 바로 virtual memory 타고 가져오는것.
 *
 * direct buffer는 disk -> jvm native memory에 데이터 복사가 1번 이루어지고, 저기 올라와있는 데이터 읽는 것.
 *
 * 그럼 상식적으로, 1 billion lines, 13GiB의 데이터를 direct buffer는 1번 복사하는데,
 * memory mapping이 복사하는 과정 자체가 없으니까,
 * memory mapping이 direct buffer보다 더 빨라야 하는거 아냐?
 * 근데 왜 실험 결과는 direct buffer가 더 빠르게 나왔지?
 *
 * A.
 * 1. 일단 memory mapping은 4k 페이지 단위로 읽고, direct buffer는 8mb 단위로 크게크게 읽기 때문에, 한번에 많이 가져오니까 더 유리
 *      - Direct Buffer (8MB): 전체 파일을 약 1,625번의 읽기 작업으로 처리 (13GB / 8MB)
 *      - Memory Mapping: 전체 파일을 약 3,328,000번의 페이지 폴트로 처리할 수 있음 (13GB / 4KB)
 * 2. memory mapping은 매번 page fault 발생하고 처리하는 과정이 있는데 그게 overhead 될 수 있음
 * 3. 여러 작은 페이지로 나눠서 접근 -> 캐시 효율성 떨어짐. 반면 direct buffer는 연속적인 큰 블록을 os가 pre-fetch 해서 캐싱함
 *
 */
class CalculateAverageDirectBuffer {
    
    private static final String FILE = "measurements.txt";
    private static final int BUFFER_SIZE = 8 * 1024 * 1024; // 8MB direct buffer
    private static final int LINE_BUFFER_SIZE = 1024; // 1KB for a single line
    
    private static record ResultRow(double min, double mean, double max) {
        public String toString() {
            return round(min) + "/" + round(mean) + "/" + round(max);
        }
        
        private double round(double value) {
            return Math.round(value * 10.0) / 10.0;
        }
    }
    
    private static class MeasurementAggregator {
        private double min = Double.POSITIVE_INFINITY;
        private double max = Double.NEGATIVE_INFINITY;
        private double sum;
        private long count;
        
        void update(double value) {
            min = Math.min(min, value);
            max = Math.max(max, value);
            sum += value;
            count++;
        }
    }
    
    public static void main(String[] args) throws Exception {
        Path path = Path.of(FILE);
        long fileSize = path.toFile().length();
        
        // Calculate number of chunks based on file size
        int numChunks = (int) Math.ceil((double) fileSize / BUFFER_SIZE);
        int numThreads = Runtime.getRuntime().availableProcessors();
        ExecutorService executor = Executors.newFixedThreadPool(Math.min(numThreads, numChunks));
        
        // Process file in parallel and collect results
        Map<String, MeasurementAggregator> results = new ConcurrentHashMap<>();
        List<Future<?>> futures = new ArrayList<>();
        
        try (FileChannel channel = FileChannel.open(path, StandardOpenOption.READ)) {
            // Create tasks for each chunk
            for (int i = 0; i < numChunks; i++) {
                final long position = (long) i * BUFFER_SIZE;
                final int chunkNum = i;
                
                futures.add(executor.submit(() -> {
                    try {
                        processChunkWithDirectBuffer(channel, position, BUFFER_SIZE, results, chunkNum == 0);
                    } catch (Exception e) {
                        System.err.println("Error processing chunk " + chunkNum + ": " + e);
                        e.printStackTrace();
                    }
                    return null;
                }));
            }
            
            // Wait for all chunks to be processed
            for (Future<?> future : futures) {
                future.get();
            }
        } finally {
            executor.shutdown();
        }
        
        // Convert results to final format
        Map<String, ResultRow> measurements = new TreeMap<>();
        results.forEach((station, agg) -> {
            measurements.put(station, new ResultRow(agg.min, agg.sum / agg.count, agg.max));
        });
        
        System.out.println(measurements);
    }
    
    /**
     * Process a chunk using Direct Buffer
     */
    private static void processChunkWithDirectBuffer(FileChannel channel, long position, int bufferSize,
        Map<String, MeasurementAggregator> results,
        boolean isFirstChunk) throws IOException {
        // Allocate a direct buffer for reading from the file
        ByteBuffer directBuffer = ByteBuffer.allocateDirect(bufferSize);
        
        // Position the channel and read into the direct buffer
        channel.position(position);
        int bytesRead = channel.read(directBuffer);
        if (bytesRead <= 0) return;
        
        // Prepare the buffer for reading
        directBuffer.flip();
        
        // Skip to the first complete line if not the first chunk
        if (!isFirstChunk) {
            skipToNextLine(directBuffer);
        }
        
        // Process lines from the direct buffer
        byte[] lineBuffer = new byte[LINE_BUFFER_SIZE];
        int linePos = 0;
        
        while (directBuffer.hasRemaining()) {
            byte b = directBuffer.get();
            
            if (b == '\n') {
                // Process the complete line
                if (linePos > 0) {
                    processLine(lineBuffer, linePos, results);
                    linePos = 0; // Reset line buffer
                }
            } else {
                // Add character to the current line
                if (linePos < lineBuffer.length) {
                    lineBuffer[linePos++] = b;
                } else {
                    // Line buffer overflow - allocate larger buffer
                    byte[] newBuffer = new byte[lineBuffer.length * 2];
                    System.arraycopy(lineBuffer, 0, newBuffer, 0, lineBuffer.length);
                    lineBuffer = newBuffer;
                    lineBuffer[linePos++] = b;
                }
            }
        }
        
        // Process the last line if it doesn't end with a newline
        if (linePos > 0) {
            processLine(lineBuffer, linePos, results);
        }
    }
    
    /**
     * Skip to the next complete line in the buffer
     */
    private static void skipToNextLine(ByteBuffer buffer) {
        while (buffer.hasRemaining()) {
            if (buffer.get() == '\n') {
                break;
            }
        }
    }
    
    /**
     * Process a single line
     */
    private static void processLine(byte[] lineBuffer, int length, Map<String, MeasurementAggregator> results) {
        // Find semicolon position
        int semicolonPos = -1;
        for (int i = 0; i < length; i++) {
            if (lineBuffer[i] == ';') {
                semicolonPos = i;
                break;
            }
        }
        
        if (semicolonPos > 0 && semicolonPos < length - 1) {
            // Extract station name
            String station = new String(lineBuffer, 0, semicolonPos, StandardCharsets.UTF_8);
            
            // Parse temperature value
            boolean negative = false;
            double value = 0;
            boolean decimal = false;
            int decimalPlaces = 0;
            
            for (int i = semicolonPos + 1; i < length; i++) {
                byte b = lineBuffer[i];
                if (b == '-') {
                    negative = true;
                } else if (b == '.') {
                    decimal = true;
                } else if (b >= '0' && b <= '9') {
                    if (!decimal) {
                        value = value * 10 + (b - '0');
                    } else {
                        value = value + (b - '0') * Math.pow(10, -(++decimalPlaces));
                    }
                }
            }
            
            if (negative) {
                value = -value;
            }
            
            // Update results
            double finalValue = value;
            results.compute(station, (k, oldAgg) -> {
                MeasurementAggregator newAgg = (oldAgg == null) ? new MeasurementAggregator() : oldAgg;
                newAgg.update(finalValue);
                return newAgg;
            });
        }
    }
}
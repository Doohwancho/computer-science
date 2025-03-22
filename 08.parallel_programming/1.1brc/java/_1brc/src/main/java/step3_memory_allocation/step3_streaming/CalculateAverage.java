package step3_memory_allocation.step3_streaming;
import java.io.IOException;
import java.nio.MappedByteBuffer;
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
 * parallel() + memory mapping + OOM 방지 메모리 아끼는 테크닉들
 *
 * step1_baseline) 195.28s user 5.10s system 98% cpu 3:22.42 total
 * step2_parallel) 318.59s user 48.54s system 314% cpu 1:56.70 total
 * step3_memory_map + chunking + streaming) 278.95s user 15.91s system 459% cpu 1:04.14 total
 *
 * ---
 * Q. 어디서 메모리 아끼는 테크닉들이 들어감? 왜 이 코드는 OOM이 안뜸?
 * A.
 * 1. step3-2의 방식은 10억 lines을 모두 line by line 읽어서 라인별로 `List<String>` 쌓음. 그래서 new String() 객체가 10억개 생겨서 OOM 뜨는 것.
 *    step3-3.streaming 방식은 쓰레드 풀에 코어 갯수만큼 쓰레드를 만들어서 병렬로 돌리는데, 각 쓰레드마다 new bytes[1024] 사이즈의 버퍼를 할당받아, 여기에 각 line을 올려서 처리하는데,
 *            일단 이 버퍼를 계속 재사용하기 때문에 메모리 상 이점도 있고,
 *            세미콜론; 위치 이전 값을 new String() 해서 "station"이라는 이름의 키값으로 잠시 만든다고 해도,
 *            결국 라인별로  `results.compute(station, (k, agg) -> { ... }` 이 연산을 하고 난 후에 저 station 값을 안쓰기 때문에,
 *            jvm이 안쓰는 new String 객체 메모리 해제 해줌 -> out of memory 에러 안뜸.
 *            (여담으로 각 쓰레드마다 parallel()하게 계산되는 곳은 concurrentHashMap에 저장됨. 일종의 parallel() 연산 최적화를 위한 적절한 자료구조를 쓴 것도 플러스가 됨)
 *            (또한 각 라인별로 파싱할 때, 약간의 최적화도 있었지만 그건 추후 파싱 최적화 챕터를 위해 넘기자.)
 *
 * ---
 * Q. 이 방식이 왜 streaming이라고 불리움?
 * A. 유튜브나 영화 볼 때 전체 다 다운받고 보는게 아니라, 현재 시점부터 조금씩 다운받고 보잖아?
 *    그런 것 처럼 여러 쓰레드가 new bytes[1024]; 마다 읽으면서 각 line별로 바로 계산해서 concurrentHashMap에 올리기 때문에,
 *    모든 lines들을 메모리에 한번에 다 올리고 처리하는게 아니라,
 *    마치 streaming처럼 line별로 조금씩 처리함.
 *
 */
class CalculateAverage {
    
    private static final String FILE = "measurements.txt";
    private static final int CHUNK_SIZE = 256 * 1024 * 1024; // 256MB chunks
    
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
    }
    
    public static void main(String[] args) throws Exception {
        Path path = Path.of(FILE);
        long fileSize = path.toFile().length();
        
        // Calculate number of chunks
        int numChunks = (int) Math.ceil((double) fileSize / CHUNK_SIZE);
        int numThreads = Runtime.getRuntime().availableProcessors();
        ExecutorService executor = Executors.newFixedThreadPool(Math.min(numThreads, numChunks));
        
        // Process chunks in parallel and collect results
        Map<String, MeasurementAggregator> results = new ConcurrentHashMap<>();
        List<Future<?>> futures = new ArrayList<>();
        
        try (FileChannel channel = FileChannel.open(path, StandardOpenOption.READ)) {
            for (int i = 0; i < numChunks; i++) {
                long start = (long) i * CHUNK_SIZE;
                long size = Math.min(CHUNK_SIZE, fileSize - start);
                
                // Submit chunk processing task
                futures.add(executor.submit(() -> {
                    try {
                        processChunk(channel, start, size, results);
                    } catch (Exception e) {
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
        
        // Convert combined results to final format
        Map<String, ResultRow> measurements = new TreeMap<>();
        results.forEach((station, agg) -> {
            measurements.put(station, new ResultRow(agg.min, agg.sum / agg.count, agg.max));
        });
        
        System.out.println(measurements);
    }
    
    /**
     * Process a chunk without loading all lines into memory at once
     */
    private static void processChunk(FileChannel channel, long position, long size,
        Map<String, MeasurementAggregator> results) throws IOException {
        // Map the chunk
        MappedByteBuffer buffer = channel.map(FileChannel.MapMode.READ_ONLY, position, size);
        
        // Skip to first newline if not the first chunk
        if (position > 0) {
            while (buffer.hasRemaining()) {
                byte b = buffer.get();
                if (b == '\n') break;
            }
        }
        
        // Process lines one at a time
        byte[] lineBuffer = new byte[1024]; // Buffer for a single line
        int linePos = 0;
        
        while (buffer.hasRemaining()) {
            byte b = buffer.get();
            
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
        
        // Process the last line if needed
        if (linePos > 0) {
            processLine(lineBuffer, linePos, results);
        }
    }
    
    /**
     * Process a single line without creating unnecessary String objects
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
            
            // Parse temperature value directly from bytes
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
            
            // Update aggregator for this station
            double finalValue = value;
            results.compute(station, (k, agg) -> {
                if (agg == null) {
                    agg = new MeasurementAggregator();
                }
                agg.min = Math.min(agg.min, finalValue);
                agg.max = Math.max(agg.max, finalValue);
                agg.sum += finalValue;
                agg.count++;
                return agg;
            });
        }
    }
}
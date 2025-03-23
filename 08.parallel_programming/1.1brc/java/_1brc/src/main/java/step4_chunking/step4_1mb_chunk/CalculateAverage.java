package step4_chunking.step4_1mb_chunk;

import java.io.IOException;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

/**
 * ---
 * what
 *
 * parallel() + memory mapping + chunking(1mb) + streaming
 * parsing 부분은 최대한 심플하게 유지함
 *
 *
 * ---
 * 실험결과
 *
 *
 * step1_baseline) 195.28s user 5.10s system 98% cpu 3:22.42 total
 * step2_parallel) 318.59s user 48.54s system 314% cpu 1:56.70 total
 * step4_1_memory_map + chunking(256mb) + streaming)  296.57s user 16.67s system 463% cpu 1:07.58 total
 * step4_2_memory_map + chunking(100mb) + streaming)  301.61s user 17.55s system 445% cpu 1:11.70 total
 * step4_3_memory_map + chunking(10mb) + streaming) 281.75s user 16.52s system 506% cpu 58.909 total
 * step4_4_memory_map + chunking(1mb) + streaming) 300.93s user 17.10s system 474% cpu 1:06.96 total
 *
 *
 * ---
 * 10mb chunk로 자를 때 제일 성능이 좋네?
 *
 * Q. 왜 chunk size가 달라지면 결과값이 달라지는거지?
 *
 * 너무 큰 청크(256MB)는 일부 쓰레드가 너무 오래 작업하는 동안 다른 쓰레드는 일찍 완료되어 CPU 코어가 유휴 상태가 될 수 있음.
 * 어떤 쓰레드가 일이 먼저 끝났는데 다른 쓰레드가 아직 일중이면, 기다려야 함.
 * 만약 10mb였다면 기다리는 일이 있어도. 일이 금방 끝났을텐데.
 *
 * 반면 chunk size가 너무 작아도(ex. 1mb) context switching을 너무 많이해서 context switching cost가 듬.
 * 또한 parsing 부분에서 보면 라인 경계 예외 처리 부분이 있는데, 더 잘게 자른 경우에 여기서 오버헤드가 더 발생함 -> 더 느려짐
 *
 *
 */
class CalculateAverage {
    
    private static final String FILE = "measurements.txt";
    // Change this value for your experiments
    private static final int CHUNK_SIZE = 1 * 1024 * 1024; // 1MB chunks
    
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
        
        System.out.println("File size: " + fileSize + " bytes");
        System.out.println("Chunk size: " + CHUNK_SIZE + " bytes (" + (CHUNK_SIZE / (1024 * 1024)) + " MB)");
        System.out.println("Number of chunks: " + numChunks);
        System.out.println("Number of threads: " + Math.min(numThreads, numChunks));
        
        long startTime = System.currentTimeMillis();
        
        // Process chunks in parallel and collect results
        Map<String, MeasurementAggregator> results = new ConcurrentHashMap<>();
        List<Future<?>> futures = new ArrayList<>();
        
        try (FileChannel channel = FileChannel.open(path, StandardOpenOption.READ)) {
            for (int i = 0; i < numChunks; i++) {
                long start = (long) i * CHUNK_SIZE;
                long size = Math.min(CHUNK_SIZE, fileSize - start);
                
                // Submit chunk processing task
                final int chunkNum = i;
                futures.add(executor.submit(() -> {
                    try {
                        processChunk(channel, start, size, results, chunkNum);
                    } catch (Exception e) {
                        System.err.println("Error processing chunk " + chunkNum + ": " + e.getMessage());
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
        
        long processingTime = System.currentTimeMillis() - startTime;
        
        // Convert combined results to final format
        Map<String, ResultRow> measurements = new TreeMap<>();
        results.forEach((station, agg) -> {
            measurements.put(station, new ResultRow(agg.min, agg.sum / agg.count, agg.max));
        });
        
        System.out.println("Processing time: " + processingTime + " ms");
        System.out.println("Number of stations: " + measurements.size());
        System.out.println(measurements);
    }
    
    /**
     * Process a chunk without loading all lines into memory at once
     */
    private static void processChunk(FileChannel channel, long position, long size,
        Map<String, MeasurementAggregator> results, int chunkNum) throws IOException {
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
        StringBuilder lineBuilder = new StringBuilder(100);
        
        while (buffer.hasRemaining()) {
            byte b = buffer.get();
            
            if (b == '\n') {
                // Process the complete line
                String line = lineBuilder.toString();
                if (!line.isEmpty()) {
                    processLine(line, results);
                }
                lineBuilder.setLength(0); // Clear the buffer
            } else {
                // Add character to the current line
                lineBuilder.append((char) b);
            }
        }
        
        // Process the last line if needed
        String lastLine = lineBuilder.toString();
        if (!lastLine.isEmpty()) {
            processLine(lastLine, results);
        }
    }
    
    /**
     * Process a single line using simple parsing (similar to baseline)
     * with improved error handling for malformed data
     */
    private static void processLine(String line, Map<String, MeasurementAggregator> results) {
        int semicolonPos = line.indexOf(';');
        
        if (semicolonPos > 0 && semicolonPos < line.length() - 1) {
            String station = line.substring(0, semicolonPos);
            String valueStr = line.substring(semicolonPos + 1).trim();
            
            // Enhanced error handling for parsing
            double value;
            try {
                // Handle the case where only a minus sign is present
                if (valueStr.equals("-")) {
                    return; // Skip this line - it's likely a corrupted chunk boundary
                }
                value = Double.parseDouble(valueStr);
            } catch (NumberFormatException e) {
                // Log and skip problematic lines
                System.err.println("Failed to parse value: '" + valueStr + "' in line: '" + line + "'");
                return;
            }
            
            // Update aggregator for this station
            results.compute(station, (k, agg) -> {
                if (agg == null) {
                    agg = new MeasurementAggregator();
                }
                agg.min = Math.min(agg.min, value);
                agg.max = Math.max(agg.max, value);
                agg.sum += value;
                agg.count++;
                return agg;
            });
        }
    }
}
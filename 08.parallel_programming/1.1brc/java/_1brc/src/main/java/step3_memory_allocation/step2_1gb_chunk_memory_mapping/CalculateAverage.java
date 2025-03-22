package step3_memory_allocation.step2_1gb_chunk_memory_mapping;

import static java.util.stream.Collectors.groupingBy;

import java.io.IOException;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.stream.Collector;
import java.util.stream.Stream;


/**
 * ---
 * what
 *
 * baseline + parallel() + memory mapping + 1gb chunking
 *
 * step1에서 memory mapping할 때, 최대 2gb까지 가져올 수 있는데, 1billion lines txt 파일은 13GiB라 1기가씩 쪼개서 처리하기로 함.
 *
 *
 * ---
 * results
 *
 * Exception in thread "main" java.lang.OutOfMemoryError: Java heap space
 * 	at java.base/java.util.Arrays.copyOf(Arrays.java:3512)
 * 	at java.base/java.util.Arrays.copyOf(Arrays.java:3481)
 * 	at java.base/java.util.ArrayList.grow(ArrayList.java:237)
 * 	at java.base/java.util.ArrayList.grow(ArrayList.java:244)
 * 	at java.base/java.util.ArrayList.add(ArrayList.java:454)
 * 	at java.base/java.util.ArrayList.add(ArrayList.java:467)
 * 	at step4_chunking.step1_256Mb_chunk.CalculateAverage.createStreamFromMappedChunk(CalculateAverage.java:135)
 * 	at step4_chunking.step1_256Mb_chunk.CalculateAverage.main(CalculateAverage.java:91)
 *  java step4_chunking.step1_256Mb_chunk.CalculateAverage  34.83s user 15.47s system 326% cpu 15.428 total
 *
 * chunking으로 최적화 하는거가 추후 나올거라 일단 1GB씩 읽는식으로 짰는데, 메모리 부족 뜸
 *
 * 왜냐면 13GiB 파일 한줄한줄 읽어서 한줄마다 List<String>에 담고 그걸 합쳐서 처리하는데,
 * String 객체 생성에 너무 많은 메모리를 썼기 때문
 *
 */
/**
 * Baseline implementation + parallel() + Memory Mapping only
 * No other optimizations
 */
class CalculateAverage {
    
    private static final String FILE = "measurements.txt";
    private static final int CHUNK_SIZE = 1 << 30; // 1GB chunks
    
    private static record Measurement(String station, double value) {
        private Measurement(String[] parts) {
            this(parts[0], Double.parseDouble(parts[1]));
        }
    }
    
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
    
    public static void main(String[] args) throws IOException {
        Collector<Measurement, MeasurementAggregator, ResultRow> collector = Collector.of(
            MeasurementAggregator::new,
            (a, m) -> {
                a.min = Math.min(a.min, m.value);
                a.max = Math.max(a.max, m.value);
                a.sum += m.value;
                a.count++;
            },
            (agg1, agg2) -> {
                var res = new MeasurementAggregator();
                res.min = Math.min(agg1.min, agg2.min);
                res.max = Math.max(agg1.max, agg2.max);
                res.sum = agg1.sum + agg2.sum;
                res.count = agg1.count + agg2.count;
                
                return res;
            },
            agg -> {
                return new ResultRow(agg.min, agg.sum / agg.count, agg.max);
            });
        
        // Use memory mapping + parallel processing
        Path path = Path.of(FILE);
        long fileSize = path.toFile().length();
        
        // Create streams for each chunk and concatenate them
        List<Stream<String>> streams = new ArrayList<>();
        
        try (FileChannel channel = FileChannel.open(path, StandardOpenOption.READ)) {
            // Process the file in chunks to handle files larger than 2GB
            for (long position = 0; position < fileSize; position += CHUNK_SIZE) {
                long start = position;
                long size = Math.min(CHUNK_SIZE, fileSize - position);
                
                // Create a stream for this chunk
                Stream<String> chunkStream = createStreamFromMappedChunk(channel, start, size);
                streams.add(chunkStream);
            }
            
            // Concatenate all chunk streams
            Stream<String> combinedStream = streams.stream().flatMap(s -> s);
            
            // Process the combined stream in parallel (same as baseline + parallel)
            Map<String, ResultRow> measurements = new TreeMap<>(
                combinedStream
                    .parallel()
                    .map(l -> {
                        String[] parts = l.split(";");
                        return new Measurement(parts);
                    })
                    .collect(groupingBy(m -> m.station(), collector))
            );
            
            System.out.println(measurements);
        }
    }
    
    /**
     * Creates a Stream<String> from a memory-mapped chunk of the file
     */
    private static Stream<String> createStreamFromMappedChunk(FileChannel channel, long position, long size) throws IOException {
        MappedByteBuffer buffer = channel.map(FileChannel.MapMode.READ_ONLY, position, size);
        
        // Convert MappedByteBuffer to a Stream<String> of lines
        List<String> lines = new ArrayList<>();
        StringBuilder currentLine = new StringBuilder();
        
        // Skip to first newline if not the first chunk
        if (position > 0) {
            while (buffer.hasRemaining()) {
                byte b = buffer.get();
                if (b == '\n') break;
            }
        }
        
        // Read lines from the buffer
        while (buffer.hasRemaining()) {
            byte b = buffer.get();
            if (b == '\n') {
                lines.add(currentLine.toString());
                currentLine.setLength(0);
            } else {
                currentLine.append((char) b);
            }
        }
        
        // Add the last line if not empty
        if (currentLine.length() > 0) {
            lines.add(currentLine.toString());
        }
        
        return lines.stream();
    }
}
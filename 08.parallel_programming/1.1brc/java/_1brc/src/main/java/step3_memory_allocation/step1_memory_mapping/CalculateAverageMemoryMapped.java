package step3_memory_allocation.step1_memory_mapping;

import java.io.IOException;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;
import java.util.Map;
import java.util.TreeMap;
import java.util.stream.Collectors;

/**
 * baseline + Memory Mapping optimization only
 *
 * 문제점) memory allocation 하려면 최대 파일 크기가 2GB인데(Integer.MAX_VALUE = 2,147,483,647 바이트),
 *       measurements.txt의 크기가 13GB라 에러남.
 *       따라서 chunking과 같이 써야함
 */
public class CalculateAverageMemoryMapped {
    
    private static final String FILE = "measurements.txt";
    
    private static record Measurement(double min, double max, double sum, long count) {
        
        Measurement(double initialMeasurement) {
            this(initialMeasurement, initialMeasurement, initialMeasurement, 1);
        }
        
        public static Measurement combineWith(Measurement m1, Measurement m2) {
            return new Measurement(
                Math.min(m1.min, m2.min),
                Math.max(m1.max, m2.max),
                m1.sum + m2.sum,
                m1.count + m2.count
            );
        }
        
        public String toString() {
            return round(min) + "/" + round(sum / count) + "/" + round(max);
        }
        
        private double round(double value) {
            return Math.round(value * 10.0) / 10.0;
        }
    }
    
    public static void main(String[] args) throws IOException {
        Path path = Path.of(FILE);
        long fileSize = path.toFile().length();
        
        Map<String, Measurement> resultMap = new TreeMap<>();
        
        // Open file channel for memory mapping
        try (FileChannel channel = FileChannel.open(path, StandardOpenOption.READ)) {
            // Map the entire file into memory
            // Note: For files larger than 2GB, you would need to map multiple regions
            MappedByteBuffer buffer = channel.map(FileChannel.MapMode.READ_ONLY, 0, fileSize);
            
            // Process the file line by line
            int lineStart = 0;
            String currentLine;
            
            while (buffer.hasRemaining()) {
                byte b = buffer.get();
                if (b == '\n' || !buffer.hasRemaining()) {
                    // Found end of line, process it
                    int lineEnd = buffer.position() - (b == '\n' ? 1 : 0);
                    
                    // Extract the line as a string
                    buffer.position(lineStart);
                    byte[] lineBytes = new byte[lineEnd - lineStart];
                    buffer.get(lineBytes);
                    currentLine = new String(lineBytes, StandardCharsets.UTF_8);
                    
                    // Process the line just like in the original code
                    int pivot = currentLine.indexOf(';');
                    String key = currentLine.substring(0, pivot);
                    double measured = Double.parseDouble(currentLine.substring(pivot + 1));
                    
                    // Update measurements
                    Measurement newMeasurement = new Measurement(measured);
                    resultMap.merge(key, newMeasurement, Measurement::combineWith);
                    
                    // Move to next line
                    lineStart = buffer.position();
                }
            }
        }
        
        // Print results
        System.out.print("{");
        System.out.print(
            resultMap.entrySet().stream()
                .map(Object::toString)
                .collect(Collectors.joining(", ")));
        System.out.println("}");
    }
}
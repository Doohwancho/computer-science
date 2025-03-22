package step4_chunking.step3_parallel_in_10MB_chunk;

import java.io.*;
import java.nio.*;
import java.nio.channels.*;
import java.nio.file.*;
import java.util.*;
import java.util.stream.*;


class CalculateAverage_bjhara {
    private static final String FILE = "measurements.txt";
    
    private static class Measurement {
        private double min = Double.POSITIVE_INFINITY;
        private double max = Double.NEGATIVE_INFINITY;
        private double sum;
        private long count;
        
        public String toString() {
            return round(min) + "/" + round(sum / count) + "/" + round(max);
        }
        
        private double round(double value) {
            return Math.round(value * 10.0) / 10.0;
        }
        
        public static Measurement combine(Measurement m1, Measurement m2) {
            var mres = new Measurement();
            mres.min = m1.min < m2.min ? m1.min : m2.min;
            mres.max = m1.max > m2.max ? m1.max : m2.max;
            mres.sum = m1.sum + m2.sum;
            mres.count = m1.count + m2.count;
            return mres;
        }
    }
    
    public static void main(String[] args) throws IOException {
        //step1. FileChannel library를 써서, read only로 파일을 읽는다.
        try (FileChannel fileChannel = (FileChannel) Files.newByteChannel(Path.of(FILE),
            EnumSet.of(StandardOpenOption.READ))) {
            
            var cities = splitFileChannel(fileChannel)
                .parallel()
                .map(CalculateAverage_bjhara::parseBuffer)
                .collect(Collectors.reducing(CalculateAverage_bjhara::combineMaps));
            
            //step15. Map<city, Measurements>를 TreeMap으로 변환
            var sortedCities = new TreeMap<>(cities.orElseThrow());
            
            //step16. print
            System.out.println(sortedCities);
        }
    }
    
    //step14. reduce로 맵의 두 elements를 합친다. (둘중에 min을 계산해서 넣거나, max 비교 계산해서 넣거나, sum을 더해서 넣거나, count를 늘리거나.. combine() 참조)
    private static Map<String, Measurement> combineMaps(Map<String, Measurement> map1,
        Map<String, Measurement> map2) {
        for (var entry : map2.entrySet()) {
            map1.merge(entry.getKey(), entry.getValue(), Measurement::combine);
        }
        
        return map1;
    }
    
    private static Stream<ByteBuffer> splitFileChannel(final FileChannel fileChannel) throws IOException {
        return StreamSupport.stream(Spliterators.spliteratorUnknownSize(new Iterator<ByteBuffer>() {
            private static final long CHUNK_SIZE = 1024 * 1024 * 10L; //10MB
            
            private final long size = fileChannel.size();
            private long start = 0;
            
            @Override
            public boolean hasNext() {
                return start < size;
            }
            
            @Override
            public ByteBuffer next() {
                try {
                    //step2. FileChannel 라이브러리를 써서 readonly로 10MB chunk size 단위로 읽는다.
                    MappedByteBuffer mappedByteBuffer = fileChannel.map(FileChannel.MapMode.READ_ONLY, start,
                        Math.min(CHUNK_SIZE, size - start));
    
                    //step3. 10MB단위 chunk로 자른게, \n인 엔터로 깔금하게 잘려야 파싱하기 편하다.
                    // don't split the data in the middle of lines
                    // find the closest previous newline
                    int realEnd = mappedByteBuffer.limit() - 1;
                    while (mappedByteBuffer.get(realEnd) != '\n')
                        realEnd--;
                    
                    realEnd++;
                    
                    mappedByteBuffer.limit(realEnd);
                    start += realEnd;
                    
                    return mappedByteBuffer;
                }
                catch (IOException ex) {
                    throw new UncheckedIOException(ex);
                }
            }
        }, Spliterator.IMMUTABLE), false); //Q. 결국 .parallel()을 붙이던데, 얘를 true로 한 것과 차이가 뭘까?
    }
    
    //step4. 10MB chunk단위로 ByteBuffer를 받는다.
    private static Map<String, Measurement> parseBuffer(ByteBuffer bb) {
        Map<String, Measurement> cities = new HashMap<>();
        
        final int limit = bb.limit();
        //step5. 128 byte 고정된 크기의 byte array를 생성. 나중에 city를 new String()으로 옮겨 넣을 때 필요하고, Double형 값을 옮겨 넣을 때 필요하다.
        final byte[] buffer = new byte[128];
        
        while (bb.position() < limit) {
            final int currentPosition = bb.position();
            
        /*
            example) head measurements.txt
            
            Boise;2.5
            Barcelona;18.3
            Ouahigouya;47.1
            Parakou;29.7
            Vilnius;-0.2
            Monaco;15.4
            Dikson;-17.2
            Cape Town;12.4
            Jerusalem;18.4
            Palm Springs;20.0
         */
            //step6. find the ; separator
            int separator = currentPosition;
            while (separator != limit && bb.get(separator) != ';')
                separator++;
            
            //step7. find the end of the line
            int end = separator + 1;
            while (end != limit && !Character.isWhitespace((char) bb.get(end)))
                end++;
            
            //step8. get the name as a string
            int nameLength = separator - currentPosition;
            bb.get(buffer, 0, nameLength);
            String city = new String(buffer, 0, nameLength);
            
            //step9. get rid of the separator
            bb.get();
            
            //step10. get the double value
            int valueLength = end - separator - 1;
            bb.get(buffer, 0, valueLength);
            String valueStr = new String(buffer, 0, valueLength);
            double value = Double.parseDouble(valueStr);
            
            //step11. and get rid of the new line (handle both kinds)
            byte newline = bb.get();
            if (newline == '\r')
                bb.get();
            
            //step12. update the map with the new measurement
            Measurement agg = cities.get(city);
            if (agg == null) {
                agg = new Measurement();
                cities.put(city, agg);
            }
            
            agg.min = agg.min < value ? agg.min : value;
            agg.max = agg.max > value ? agg.max : value;
            agg.sum += value;
            agg.count++;
        }
        
        //step13. return Map<String, Measurement>
        return cities;
    }
}


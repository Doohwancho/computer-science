package step5_parse.a_string_optimization.step3_string_intern;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.AbstractMap;
import java.util.Map;
import java.util.TreeMap;
import java.util.concurrent.ConcurrentHashMap;

/**
 * ---
 * 파싱 최적화 단계 3: 문자열 인터닝(String.intern()) 적용
 *
 * Q. what is String.intern()?
 * 중복 String 처리할 일이 많으면, 중복일거 같은 애를 .intern() 해놓으면 string pool이라는 특별한 곳에서 관리해놓고, 매번 .intern() 할 때마다 중복이면 거기서 반환함.
 * 이론적으로는 중복 String 객체를 안만들어도 되니까, less 객체 -> less memory requirements -> less gc -> faster 인데,
 *
 *
 * ---
 * 실험결과
 * step1_split) 318.59s user 48.54s system 314% cpu 1:56.70 total
 * step2_substring) 272.93s user 29.88s system 284% cpu 1:46.27 total
 * step3_string_intern) 542.33s user 44.22s system 452% cpu 2:09.77 total
 *
 *
 * ---
 * Q. 왜 오히려 느려짐?
 *
 * 1. string pool 객체 내부적으로 관리해줘야 하지,
 * 2. 10억라인 처리하면 .intern() 호출 10억번 해서 매번 중복 string이 거기 있는지 찾는 step이 10억번 추가됨.
 * 3. 또한 string pool은 모든 쓰레드가 공유하는 전역 자원이라, .parallel()로 멀티 쓰레드가 돌다가 동시에 .intern() 호출하면 lock 경합 발생 -> 쓰레드가 기다려야 하니까 느려짐
 * 4. string pool은 특별한 메모리 영역에서 관리되는데, jvm이 gc하는 방식이 g1gc였나? 그 방식이 아니고 다른 방식으로 gc하는데, 이게 기존 방식 대비 별론가봄.
 *
 *
 */
class CalculateAverage {
    
    private static final String FILE = "measurements.txt";
    
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
        long startTime = System.currentTimeMillis();
        
        // 측정소 수를 예상하여 초기 용량 지정 (성능 향상)
        int expectedStations = 150; // 예상 측정소 수
        Map<String, MeasurementAggregator> results = new ConcurrentHashMap<>(expectedStations);
        
        Files.lines(Paths.get(FILE)).parallel()
            .map(record -> {
                int pivot = record.indexOf(";");
                // 문자열 인터닝 적용: 자주 반복되는 측정소 이름을 공유
                String key = record.substring(0, pivot).intern();
                double measured = Double.parseDouble(record.substring(pivot + 1));
                return new AbstractMap.SimpleEntry<>(key, measured);
            })
            .forEach(entry -> {
                String station = entry.getKey();
                double value = entry.getValue();
                
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
            });
        
        // 최종 결과 변환
        Map<String, ResultRow> measurements = new TreeMap<>();
        results.forEach((station, agg) -> {
            measurements.put(station, new ResultRow(agg.min, agg.sum / agg.count, agg.max));
        });
        
        long processingTime = System.currentTimeMillis() - startTime;
        
        System.out.println("Processing time: " + processingTime + " ms");
        System.out.println("Number of stations: " + measurements.size());
        System.out.println(measurements);
    }
}
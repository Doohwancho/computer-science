package step5_parse.a_string_optimization.step2_중간단계_객체생성_최소화;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.AbstractMap;
import java.util.Map;
import java.util.TreeMap;
import java.util.concurrent.ConcurrentHashMap;

/**
 * 파싱 최적화 단계 2: 중간 객체 생성 최소화
 *
 * .split()이 내부적으로 매 라인마다 regex처리하는게 overhead 들고,
 * ArrayList<>만들고 그걸 다시 String[]으로 형변환하는게 비용이 많아서
 * 대신 substring() 방식으로 바꿈.
 *
 * 일단 만들어지는 객체 수가 10억개만큼 줄었음
 * 1. 1단계 (40억개)
 *  - split(";")으로 인한 String[] 배열 (10억개)
 *  - String[] 배열 내부의 String 객체들 (20억개)
 *  - Measurement 객체 (10억개)
 * 2. 2단계 (30억개)
 *  - substring()으로 인한 String 객체 (20억개)
 *  - SimpleEntry 객체 (10억개)
 *
 * 만드는 객체수가 10억개가 줄었으니 그만큼 gc에 들어가는 비용이나 시간도 줄음.
 *
 * 또한 기존엔 그냥 Map<>에 각 라인의 결과값을 담았다면, 이젠 ConcurrentHashMap에 담기 때문에 아마 lock contention cost비용이 더 낮을 듯
 *
 *
 * ---
 * 실험결과
 * step1_split) 318.59s user 48.54s system 314% cpu 1:56.70 total
 * step2_substring) 272.93s user 29.88s system 284% cpu 1:46.27 total
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
        
        // ConcurrentHashMap을 사용하여 병렬 처리 효율 향상
        Map<String, MeasurementAggregator> results = new ConcurrentHashMap<>();
        
        Files.lines(Paths.get(FILE)).parallel()
            .map(record -> {
                // Map.Entry<String, Double>으로 파싱 결과 반환
                int pivot = record.indexOf(";");
                String key = record.substring(0, pivot);
                double measured = Double.parseDouble(record.substring(pivot + 1));
                return new AbstractMap.SimpleEntry<>(key, measured);
            })
            .forEach(entry -> {
                // 각 측정값을 직접 집계기에 추가
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
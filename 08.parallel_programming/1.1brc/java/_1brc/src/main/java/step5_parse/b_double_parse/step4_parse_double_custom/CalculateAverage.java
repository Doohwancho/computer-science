package step5_parse.b_double_parse.step4_parse_double_custom;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.AbstractMap;
import java.util.Map;
import java.util.TreeMap;
import java.util.concurrent.ConcurrentHashMap;

/**
 * double 파싱 최적화 기법 1: custom double parser
 *
 * double 계산하는것중에서 제일 병목이,
 * 소숫점 0.00000xx 이런거 계산하는건데,
 *
 *
 * - Double.parseDouble() 대신 직접 구현한 빠른 파싱 메소드 사용
 * - 자주 사용되는 소수점 자릿수에 대해 미리 계산된 값 활용
 *
 * example)
 * city_name1;-12.3
 * city_name2;8.9
 * ...
 *
 * ---
 * 실험결과
 *
 * step1_split) 318.59s user 48.54s system 314% cpu 1:56.70 total
 * step2_substring) 272.93s user 29.88s system 284% cpu 1:46.27 total
 * step3_string_intern) 542.33s user 44.22s system 452% cpu 2:09.77 total
 * step4_parse_double_custom) 515.43s user 40.75s system 422% cpu 2:11.79 total
 *
 *
 * ---
 * Q. 왜 baseline에 Double.parseDouble() 보다 느릴까?
 *
 *
 * 여기서 double을 parse하는 방법을 최적화 하는건데,
 * city_name1;-12.3
 * city_name2;8.9
 *
 *
 *
 */
class CalculateAverage {
    
    private static final String FILE = "measurements.txt";
    
    // 미리 계산된 10의 거듭제곱 값 (최대 9자리 소수점까지)
    private static final double[] POWERS_OF_10 = {
        1.0, 0.1, 0.01, 0.001, 0.0001, 0.00001, 0.000001, 0.0000001, 0.00000001, 0.000000001
    };
    
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
    
    // 직접 구현한 빠른 Double 파싱 메소드
    private static double parseDouble(String s) {
        if (s == null || s.isEmpty()) {
            throw new NumberFormatException("Empty string");
        }
        
        int len = s.length();
        int i = 0;
        boolean negative = false;
        
        // 부호 처리
        if (s.charAt(0) == '-') {
            negative = true;
            i = 1;
        } else if (s.charAt(0) == '+') {
            i = 1;
        }
        
        // 숫자 파싱
        double result = 0;
        int decimalPosition = -1;
        
        for (; i < len; i++) {
            char c = s.charAt(i);
            if (c == '.') {
                decimalPosition = i;
            } else if (c >= '0' && c <= '9') {
                if (decimalPosition < 0) {
                    // 정수 부분
                    result = result * 10 + (c - '0');
                } else {
                    // 소수 부분
                    int decimalDigits = i - decimalPosition;
                    if (decimalDigits <= POWERS_OF_10.length) {
                        result += (c - '0') * POWERS_OF_10[decimalDigits - 1];
                    } else {
                        // 미리 계산된 값이 없는 경우 Math.pow 사용
                        result += (c - '0') * Math.pow(10, -decimalDigits);
                    }
                }
            } else {
                throw new NumberFormatException("Invalid character: " + c);
            }
        }
        
        return negative ? -result : result;
    }
    
    public static void main(String[] args) throws IOException {
        long startTime = System.currentTimeMillis();
        
        int expectedStations = 10000;
        Map<String, MeasurementAggregator> results = new ConcurrentHashMap<>(expectedStations);
        
        Files.lines(Paths.get(FILE)).parallel()
            .map(record -> {
                int pivot = record.indexOf(";");
                String key = record.substring(0, pivot).intern();
                double measured = parseDouble(record.substring(pivot + 1)); // 직접 구현한 파싱 메소드 사용
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
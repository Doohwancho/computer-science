package step5_parse.b_double_parse.step6_single_multiplication_parsing;


import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Map;
import java.util.TreeMap;
import java.util.concurrent.ConcurrentHashMap;

/**
 * double 파싱 최적화 기법 3: Single Multiplication Parsing
 *
 * - 온도 값의 각 자릿수를 특정 위치에 맞춰 단일 곱셈 연산으로 파싱
 * - 비트 연산을 활용하여 분기문을 줄이고 CPU 파이프라인 효율성 향상
 *
 *
 * ---
 * 실험결과
 *
 * step1_split) 318.59s user 48.54s system 314% cpu 1:56.70 total
 * step2_substring) 272.93s user 29.88s system 284% cpu 1:46.27 total
 * step3_string_intern) 542.33s user 44.22s system 452% cpu 2:09.77 total
 * step4_parse_double_custom) 515.43s user 40.75s system 422% cpu 2:11.79 total
 * step5_parse_as_int) 208.93s user 26.50s system 248% cpu 1:34.61 total
 * step6_single_multiplication_parsing) 230.13s user 26.36s system 265% cpu 1:36.59 total
 *
 *
 */
class CalculateAverage {
    
    private static final String FILE = "measurements.txt";
    
    // 자릿수 위치별 곱셈 상수
    private static final long MAGIC_MULTIPLIER = (100L * 0x1000000 + 10L * 0x10000 + 1L);
    
    private static record ResultRow(double min, double mean, double max) {
        public String toString() {
            return round(min) + "/" + round(mean) + "/" + round(max);
        }
        
        private double round(double value) {
            return Math.round(value * 10.0) / 10.0;
        }
    }
    
    private static class MeasurementAggregator {
        private int min = Integer.MAX_VALUE;
        private int max = Integer.MIN_VALUE;
        private long sum;  // 정수 합계 (실제 값 * 10)
        private long count;
    }
    
    public static void main(String[] args) throws IOException {
        long startTime = System.currentTimeMillis();
        
        Map<String, MeasurementAggregator> results = new ConcurrentHashMap<>();
        
        Files.lines(Paths.get(FILE)).parallel()
            .forEach(line -> {
                // 세미콜론 위치 찾기
                int semicolonPos = line.indexOf(';');
                if (semicolonPos > 0 && semicolonPos < line.length() - 1) {
                    // 측정소 이름 추출
                    String station = line.substring(0, semicolonPos);
                    
                    // 온도 부분 추출
                    String temperatureStr = line.substring(semicolonPos + 1);
                    
                    // 단일 곱셈 파싱으로 온도 값 계산
                    int temperature = parseTemperatureWithMultiplication(temperatureStr);
                    
                    // 결과 업데이트
                    results.compute(station, (k, agg) -> {
                        if (agg == null) {
                            agg = new MeasurementAggregator();
                        }
                        agg.min = Math.min(agg.min, temperature);
                        agg.max = Math.max(agg.max, temperature);
                        agg.sum += temperature;
                        agg.count++;
                        return agg;
                    });
                }
            });
        
        // 최종 결과 변환 (정수 * 10 값을 실제 온도로 변환)
        Map<String, ResultRow> measurements = new TreeMap<>();
        results.forEach((station, agg) -> {
            measurements.put(station, new ResultRow(
                agg.min / 10.0,
                (double) agg.sum / (10.0 * agg.count),
                agg.max / 10.0
            ));
        });
        
        long processingTime = System.currentTimeMillis() - startTime;
        
        System.out.println("Processing time: " + processingTime + " ms");
        System.out.println("Number of stations: " + measurements.size());
        System.out.println(measurements);
    }
    
    /**
     * 단일 곱셈 연산을 사용한 최적화된 온도 파싱
     *
     * 지원하는 형식:
     * - 한 자리 정수 + 소수점 한 자리: "N.D"
     * - 두 자리 정수 + 소수점 한 자리: "NN.D"
     * - 부호가 있는 경우 모두 지원: "-N.D", "-NN.D"
     */
    private static int parseTemperatureWithMultiplication(String temperatureStr) {
        // 특수 경우 처리 (길이가 짧은 경우)
        int len = temperatureStr.length();
        if (len <= 2) {
            // 한 자리 숫자만 있는 경우 (예: "5", "-3")
            // 5라면 10을 곱해서 50으로 만들고 (1자리수 double 이니까) 나중에 /10함. step5)에서 했었던 테크닉임
            char c = temperatureStr.charAt(0);
            return (c == '-') ? 0 : (c - '0') * 10;
        }
        
        // 대부분의 온도 값은 "NN.D" 또는 "-NN.D" 형식
        long numberBytes = 0L;
        int dotPos = 0;
        boolean hasSign = temperatureStr.charAt(0) == '-';
        int startIdx = hasSign ? 1 : 0;
        
        // 문자열을 8바이트 롱 값으로 변환 (비트 연산용)
        for (int i = 0; i < Math.min(len - startIdx, 4); i++) {
            char c = temperatureStr.charAt(startIdx + i);
            if (c == '.') {
                dotPos = i;
            } else {
                //문자열을 8byte long으로 변환함
                numberBytes = (numberBytes << 8) | c;
            }
        }
        
        // 단일 곱셈 파싱 수행
        return parseTemperatureWithBitOperations(numberBytes, dotPos, hasSign);
    }
    
    /**
     * 비트 연산과 단일 곱셈을 사용한 온도 파싱 최적화 구현
     */
    private static int parseTemperatureWithBitOperations(long numberBytes, int dotPos, boolean hasSign) {
        // 1. 소수점 위치에 따른 시프트 계산
        int shift = (dotPos == 1) ? 16 : 8;
        
        // 2. 숫자 자릿수 추출을 bit연산으로 빠르게 함
        long digits = 0;
        if (dotPos > 0) {
            // 두 자리 숫자 ("NN.D")
            digits = ((numberBytes >> shift) & 0xFF) * 100 +
                ((numberBytes >> 8) & 0xFF) * 10 +
                (numberBytes & 0xFF);
        } else {
            // 한 자리 숫자 ("N.D")
            digits = ((numberBytes >> 8) & 0xFF) * 10 +
                (numberBytes & 0xFF);
        }
        
        // 3. 실제 자릿수 값이 아닌 경우 조정 ('0'의 ASCII 값 48 제거)
        digits -= (dotPos > 0 ? 0x30303030L : 0x3030L);
        
        // 4. 정수 값으로 변환
        int result = (int) digits;
        
        // 5. 부호 적용
        return hasSign ? -result : result;
    }
}
package step5_parse.c_cpu_optimization.step8_branchless_programming;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Map;
import java.util.TreeMap;
import java.util.concurrent.ConcurrentHashMap;

/**
 * CPU 최적화 기법 2: 분기 명령어 회피 (Branchless Programming)
 *
 * - 조건문 대신 비트 연산과 산술 연산만 사용하여 분기 예측 실패 방지
 * - CPU 파이프라인 효율성 극대화
 * - 특히 온도 값 파싱과 부호 처리에 적용
 *
 * 기본적으로 10배씩 곱해서 int처리하는 방식은 같은데,
 * 그걸 if-else문을 최대한 없애고 (. 찾는 부분만 제외)
 * 나머지는 branchless로 처리함
 *
 * cpu입장에서 if-else문이 있을 때마다 예측하는데, 틀리면 pipeline stall 걸리는게 없어짐.
 *
 *
 * ---
 * 실험결과
 *
 *
 * step1_split) 318.59s user 48.54s system 314% cpu 1:56.70 total
 * step2_substring) 272.93s user 29.88s system 284% cpu 1:46.27 total
 * step3_string_intern) 542.33s user 44.22s system 452% cpu 2:09.77 total
 * step4_parse_double_custom) 515.43s user 40.75s system 422% cpu 2:11.79 total
 * step5_parse_as_int) 208.93s user 26.50s system 248% cpu 1:34.61 total
 * step6_single_multiplication_parsing) 230.13s user 26.36s system 265% cpu 1:36.59 total
 * step7_SWAR) 244.75s user 28.38s system 269% cpu 1:41.35 total
 * step8_branchless_programming) 198.99s user 23.75s system 233% cpu 1:35.28 total
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
                    
                    // 온도 값을 분기 없이 파싱
                    int temperature = parseBranchlessTemperature(line, semicolonPos + 1);
                    
                    // 결과 업데이트 (이 부분도 분기 최소화)
                    updateResultBranchless(results, station, temperature);
                }
            });
        
        // 최종 결과 변환
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
     * 분기 명령어를 사용하지 않는 온도 파싱
     */
    private static int parseBranchlessTemperature(String line, int start) {
        // 부호 판별
        boolean hasSign = line.charAt(start) == '-';
        
        // 시작 위치 계산 (부호가 있으면 한 칸 뒤에서 시작)
        int pos = start + (hasSign ? 1 : 0);
        
        // 문자를 숫자로 변환 (ASCII 코드에서 '0'을 빼기)
        int firstDigit = line.charAt(pos) - '0';
        
        // 다음 문자 가져오기
        char nextChar = pos + 1 < line.length() ? line.charAt(pos + 1) : '\0';
        
        // 다음 문자가 숫자인지 판별
        boolean secondIsDigit = nextChar >= '0' && nextChar <= '9';
        
        // 두 번째 숫자 추출 (숫자가 아니면 0)
        int secondDigit = secondIsDigit ? (nextChar - '0') : 0;
        
        // 숫자 위치 계산 (두 번째 문자가 숫자인 경우 위치 조정)
        int dotPos = pos + 1 + (secondIsDigit ? 1 : 0);
        
        // 소수점 이하 숫자 추출 (없으면 0)
        int fractionalDigit = 0;
        if (dotPos < line.length() && line.charAt(dotPos) == '.') {
            fractionalDigit = (dotPos + 1 < line.length()) ? (line.charAt(dotPos + 1) - '0') : 0;
        }
        
        // 최종 값 계산 (십진수 조합)
        int value = 0;
        
        // 첫 번째 경우: 한 자리 숫자 (예: "5.7")
        int case1 = firstDigit * 10 + fractionalDigit;
        
        // 두 번째 경우: 두 자리 숫자 (예: "12.3")
        int case2 = firstDigit * 100 + secondDigit * 10 + fractionalDigit;
        
        // 어떤 경우인지 선택 (분기 없이 계산)
        value = secondIsDigit ? case2 : case1;
        
        // 부호 적용 (분기 없이 계산)
        int sign = hasSign ? -1 : 1;
        return sign * value;
    }
    
    /**
     * 분기 명령어를 최소화한 결과 업데이트
     */
    private static void updateResultBranchless(Map<String, MeasurementAggregator> results, String station, int temperature) {
        MeasurementAggregator agg = results.get(station);
        
        // 집계기가 존재하지 않으면 새로 생성
        if (agg == null) {
            agg = new MeasurementAggregator();
            results.put(station, agg);
            
            // 첫 번째 값은 그대로 사용
            agg.min = temperature;
            agg.max = temperature;
            agg.sum = temperature;
            agg.count = 1;
            return;
        }
        
        // 최소값 업데이트 (분기 없이 계산)
        agg.min = Math.min(agg.min, temperature);
        
        // 최대값 업데이트 (분기 없이 계산)
        agg.max = Math.max(agg.max, temperature);
        
        // 합계 및 개수 업데이트
        agg.sum += temperature;
        agg.count++;
    }
}
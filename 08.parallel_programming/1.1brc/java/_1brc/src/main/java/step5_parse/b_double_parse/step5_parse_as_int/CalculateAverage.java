package step5_parse.b_double_parse.step5_parse_as_int;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Map;
import java.util.TreeMap;
import java.util.concurrent.ConcurrentHashMap;

/**
 * double 파싱 최적화 기법 2: 온도를 직접 정수로 파싱
 *
 *  city_name1;-12.3
 *  city_name2;8.9
 *
 *  얘를 파싱을 해야하는데, 어떻게 파싱할거냐?
 *
 *  double을 파싱하는게 아니라, int로 파싱한 다음, 맨 마지막에 aggregate 통계처리할 떄 double로 바꿈.
 *  예를들어 12.3 -> 123으로 바꾼 후 다 더해서 맨 마지막에 딱 한번 /10 하는 것.
 *  cpu입장에서 int연산하는게 소숫점 여러자리 있는 double 연산하는것보다 훨씬 더 빠를껄?
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
 *
 *
 * ---
 * Q. 왜 얘가 Double.parse() 보다 double을 더 빨리 파싱함?
 *
 * A. Double.parse()가 느린 이유.
 * 1. 객체 생성 오버헤드: 문자열을 파싱할 때마다 임시 String 객체 만들어서 GC 부담 개많음
 * 2. 범용성 때문에 느림: 모든 형태의 부동소수점 처리해야 되니까 "1.23e-4" 같은 과학적 표기법도 처리함
 * 3. 예외 처리: 이상한 입력 들어오면 예외 던지는 코드도 있어서 느림
 * 4. 정규식 사용: 내부적으로 정규식 써서 파싱하는데 이게 개느림
 *
 * 근데 이 방식은
 * 1. 걍 한글자씩 읽어서 처리하기 때문에 new String() 객체 별도로 안만듬
 * 2. 문자 -> 숫자 전환시 -'0'이런식으로 ASCII 값 차리로 계산해서 빠름
 * 3. if else문 분기가 custom double parser에 비해 적은데, 분기가 적을 수록 cpu 계산에 더 유리함
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
                    
                    // 온도 직접 파싱 (정수로)
                    int temperature = parseTemperatureAsInt(line, semicolonPos + 1);
                    
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
     * 문자열에서 온도를 직접 정수로 파싱 (소수점 한 자리를 포함해 10을 곱한 값)
     * 예: "12.3" -> 123, "-5.7" -> -57
     */
    private static int parseTemperatureAsInt(String line, int startPos) {
        int pos = startPos;
        int sign = 1;
        
        // 부호 처리
        if (line.charAt(pos) == '-') {
            sign = -1;
            pos++;
        }
        
        int value = 0;
        char c = line.charAt(pos++);
        
        // 첫 번째 숫자
        value = c - '0';
        
        // 두 번째 문자 처리 (정수 부분 두 번째 숫자 또는 소수점)
        if (pos < line.length()) {
            c = line.charAt(pos++);
            if (c != '.') {
                // 두 번째 숫자인 경우
                value = value * 10 + (c - '0');
                
                // 소수점 건너뛰기
                if (pos < line.length() && line.charAt(pos) == '.') {
                    pos++;
                }
            }
        }
        
        // 소수점 이하 자리 처리 (있는 경우)
        if (pos < line.length()) {
            c = line.charAt(pos);
            value = value * 10 + (c - '0');
        } else {
            // 소수점 이하 숫자가 없으면 0 추가
            value *= 10;
        }
        
        return sign * value;
    }
}
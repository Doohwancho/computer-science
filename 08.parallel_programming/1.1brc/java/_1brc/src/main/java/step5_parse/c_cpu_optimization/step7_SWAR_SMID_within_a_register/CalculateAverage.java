package step5_parse.c_cpu_optimization.step7_SWAR_SMID_within_a_register;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
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
 * 파싱 최적화 기법 3: SWAR (SIMD Within A Register) 기법
 *
 *
 * Q.SIMD in a register라는데, SIMD가 뭐지?
 * Single Instruction Multiple Data
 * = 하나의 명령어로 여러 데이터를 한 번에 처리하는 기법
 * 원래 이런거 CPU에 특수 명령어로 있는데(AVX, SSE 같은거), 일반 레지스터로도 비슷한 효과를 낼 수 있음
 *
 * 예를들어 이런 코드는, 8byte를 하나에 long 값에 우겨넣을 수 있음.
 * ```java
 * long word = 0;
 * for (int i = 0; i < 8; i++) {
 *     word |= ((long) (bytes[position + i] & 0xFF)) << (i * 8);
 * }
 * ```
 * 그러면 char8개를 한번에 처리하는거임
 *
 * 1. 루프 반복 횟수가 1/8로 줄어듦 (8개씩 처리하니까)
 * 2. CPU 명령어 수가 확 줄어듦
 *
 * = profit!
 *
 *
 * ';' 찾는것도, 매 char 읽으면서 compare 연산 하는게 아니라,
 * ```java
 * private static long semicolonMatchBits(long word) {
 *     long diff = word ^ BROADCAST_SEMICOLON;
 *     return (diff - BROADCAST_0x01) & (~diff & BROADCAST_0x80);
 * }
 * ```
 * 이런식으로 비트연산으로 찾으면 더 빠름
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
 * step7_SWAR) 244.75s user 28.38s system 269% cpu 1:41.35 total
 *
 *
 */
class CalculateAverage {
    
    private static final String FILE = "measurements.txt";
    
    // SWAR 상수
    private static final long BROADCAST_SEMICOLON = 0x3B3B3B3B3B3B3B3BL; // ';' 반복
    private static final long BROADCAST_0x01 = 0x0101010101010101L;
    private static final long BROADCAST_0x80 = 0x8080808080808080L;
    
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
        
        // baseline + parallel() 방식으로 파일 읽기
        Files.lines(Paths.get(FILE)).parallel()
            .forEach(line -> {
                processLineWithSWAR(line, results);
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
     * SWAR 기법으로 라인 처리
     */
    private static void processLineWithSWAR(String line, Map<String, MeasurementAggregator> results) {
        byte[] bytes = line.getBytes();
        int length = bytes.length;
        
        if (length <= 1) return; // 빈 라인이거나 너무 짧은 라인
        
        // 세미콜론 위치 SWAR로 찾기
        int position = 0;
        int semicolonPos = -1;
        
        while (position <= length - 8) {
            // 8바이트씩 읽어오기
            long word = 0;
            for (int i = 0; i < 8; i++) {
                word |= ((long) (bytes[position + i] & 0xFF)) << (i * 8);
            }
            
            // 세미콜론 찾기
            long matchBits = semicolonMatchBits(word);
            
            if (matchBits != 0) {
                // 세미콜론을 찾으면 정확한 위치 계산
                semicolonPos = position + Long.numberOfTrailingZeros(matchBits) / 8;
                break;
            }
            
            position += 8;
        }
        
        // 남은 바이트 처리 (8바이트 미만)
        if (semicolonPos == -1) {
            for (int i = position; i < length; i++) {
                if (bytes[i] == ';') {
                    semicolonPos = i;
                    break;
                }
            }
        }
        
        // 세미콜론이 없거나 너무 앞이나 뒤에 있는 경우
        if (semicolonPos <= 0 || semicolonPos >= length - 1) {
            return;
        }
        
        // 측정소 이름 추출
        String station = new String(bytes, 0, semicolonPos);
        
        // 온도 값 파싱
        int temperature = parseTemperatureFromBytes(bytes, semicolonPos + 1, length);
        
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
    
    /**
     * SWAR 기법으로 세미콜론 찾기
     */
    private static long semicolonMatchBits(long word) {
        long diff = word ^ BROADCAST_SEMICOLON;
        return (diff - BROADCAST_0x01) & (~diff & BROADCAST_0x80);
    }
    
    /**
     * 바이트 배열에서 온도 값 파싱 (정수 * 10 값으로 반환)
     */
    private static int parseTemperatureFromBytes(byte[] bytes, int start, int end) {
        int sign = 1;
        int result = 0;
        boolean decimalSeen = false;
        
        for (int i = start; i < end; i++) {
            byte b = bytes[i];
            if (b == '\n') break;
            
            if (b == '-') {
                sign = -1;
            } else if (b == '.') {
                decimalSeen = true;
            } else if (b >= '0' && b <= '9') {
                if (!decimalSeen) {
                    result = result * 10 + (b - '0');
                } else {
                    result = result * 10 + (b - '0');
                    break; // 소수점 한 자리만 처리
                }
            }
        }
        
        if (!decimalSeen) {
            result *= 10; // 소수점이 없는 경우 10을 곱함
        }
        
        return sign * result;
    }
}
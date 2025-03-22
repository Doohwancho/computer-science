package step2_parallel;

import static java.util.stream.Collectors.groupingBy;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Map;
import java.util.TreeMap;
import java.util.stream.Collector;

/*
---
Idea

Q. baseline에 .porallel() 만 붙이면 성능차이 얼마나 날까?

step1_baseline) 195.28s user 5.10s system 98% cpu 3:22.42 total
step2_parallel) 318.59s user 48.54s system 314% cpu 1:56.70 total

26초 빨라졌다!
cpu 사용량도 1코어 98%씩 쓰다가 여러 코어쓰니까 사용량이 314%로 늘어났다.

 */

class CalculateAverage {
    
    private static final String FILE = "measurements.txt";
    
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
    };
    
    private static class MeasurementAggregator {
        private double min = Double.POSITIVE_INFINITY;
        private double max = Double.NEGATIVE_INFINITY;
        private double sum;
        private long count;
    }
    
    public static void main(String[] args) throws IOException {
        Collector<Measurement, MeasurementAggregator, ResultRow> collector = Collector.of(
            // supplier: 새 accumulator 생성
            MeasurementAggregator::new,
    
            // accumulator: 데이터 값을 누적기에 추가
            (a, m) -> {
                a.min = Math.min(a.min, m.value);
                a.max = Math.max(a.max, m.value);
                a.sum += m.value;
                a.count++;
            },
    
            // combiner: 두 accumulator 합치기
            // combiner는 여러 스레드에서 만들어진 부분 결과(이미 각각 셔플링+리듀스 된)를 합치는 역할이라 셔플링이라기보다는 "병렬 처리의 결과 병합" 단계임
            (agg1, agg2) -> {
                var res = new MeasurementAggregator();
                res.min = Math.min(agg1.min, agg2.min);
                res.max = Math.max(agg1.max, agg2.max);
                res.sum = agg1.sum + agg2.sum;
                res.count = agg1.count + agg2.count;
                
                return res;
            },
            
            // finisher: 최종 결과 생성
            agg -> {
                return new ResultRow(agg.min, agg.sum / agg.count, agg.max);
            });
        
        Map<String, ResultRow> measurements = new TreeMap<>(Files.lines(Paths.get(FILE))
            .parallel() // <------------ 딱 이 코드만 추가됨! from baseline
            .map(l -> new Measurement(l.split(";")))
            .collect(groupingBy(m -> m.station(), collector)));
        
        System.out.println(measurements);
    }
}

package step6_parse.step1_substring_not_split;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.AbstractMap;
import java.util.Map;
import java.util.stream.Collectors;

/*
---
Q. what's differemt from step2_parallel?


1. 자료구조 최적화
TreeMap -> ConcurrentMap

2. 문자열 파싱 최적화
원래는 .split() 썼음. `.map(l -> new Measurement(l.split(";")))`
이제는 indexOf()와 substring() 사용함
```java
.map(record -> {
    int pivot = record.indexOf(";");
    String key = record.substring(0, pivot);
    double measured = Double.parseDouble(record.substring(pivot + 1));
    return new AbstractMap.SimpleEntry<>(key, measured);
})
```

3. 객체 생성 최소화
첫 번째 코드는 중간 단계로 Measurement 객체를 생성하는데, 이게 10억 개 생성되니까 GC 부담이 엄청남.
두 번째 코드는 AbstractMap.SimpleEntry를 사용해 중간 객체를 줄이고, 필요한 데이터만 추출해서 바로 처리함.


step1_baseline) 195.28s user 5.10s system 98% cpu 3:22.42 total
step2_parallel) 318.59s user 48.54s system 314% cpu 1:56.70 total
current) 300.36s user 29.69s system 316% cpu 1:44.34 total

 */

class CalculateAverage_royvanrijn {
    
    private static final String FILE = "measurements.txt";
    
    private record Measurement(double min, double max, double sum, long count) {
        
        Measurement(double initialMeasurement) {
            this(initialMeasurement, initialMeasurement, initialMeasurement, 1);
        }
        
        public static Measurement combineWith(Measurement m1, Measurement m2) {
            return new Measurement(
                m1.min < m2.min ? m1.min : m2.min,
                m1.max > m2.max ? m1.max : m2.max,
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

//        long before = System.currentTimeMillis();
        
        Map<String, Measurement> resultMap = Files.lines(Path.of(FILE)).parallel() //병렬로 읽기 <-------------- 이게 추가된 것
            .map(record -> {
                // Map to <String,double>
                int pivot = record.indexOf(";");
                String key = record.substring(0, pivot);
                double measured = Double.parseDouble(record.substring(pivot + 1));
                return new AbstractMap.SimpleEntry<>(key, measured);
            })
            .collect(Collectors.toConcurrentMap(
                // Combine/reduce:
                AbstractMap.SimpleEntry::getKey,
                entry -> new Measurement(entry.getValue()),
                Measurement::combineWith));
        
        System.out.print("{");
        System.out.print(
            resultMap.entrySet().stream().sorted(Map.Entry.comparingByKey()).map(Object::toString).collect(Collectors.joining(", ")));
        System.out.println("}");

//        System.out.println("Took: " + (System.currentTimeMillis() - before));
    
    }
}

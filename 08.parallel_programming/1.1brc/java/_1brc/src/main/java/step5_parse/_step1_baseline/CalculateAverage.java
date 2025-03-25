package step5_parse._step1_baseline;

import static java.util.stream.Collectors.groupingBy;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Map;
import java.util.TreeMap;
import java.util.stream.Collector;

/*

baseline + parallel()

파싱 방식: split(";") 사용

`.map(l -> new Measurement(l.split(";")))` 이 코드가 문제임

문제점:
1. .split()을 내부적으로 regex를 써서 찾는데, 이 작업이 오버헤드가 크다고 함.
2. .split()은 내부적으로 ArrayList<String>을 만들어서 String 추가한 후에 String[]으로 형변환 하나봄.

이게 split()의 내부 코드인데, 중간에 ArrayList<>만들고 값 추가해 준 다음, 다시 String[]으로 바꿔주는 형변환 또 해줌

```java
public String[] split(String regex, int limit) {
    char ch = 0;
    if (((regex.length() == 1 &&
         ".$|()[{^?*+\\".indexOf(ch = regex.charAt(0)) == -1) ||
         (regex.length() == 2 &&
          regex.charAt(0) == '\\' &&
          (((ch = regex.charAt(1))-'0')|('9'-ch)) < 0 &&
          ((ch-'a')|('z'-ch)) < 0 &&
          ((ch-'A')|('Z'-ch)) < 0)) &&
        (ch < Character.MIN_HIGH_SURROGATE ||
         ch > Character.MAX_LOW_SURROGATE))
    {
        int off = 0;
        int next = 0;
        boolean limited = limit > 0;
        ArrayList<String> list = new ArrayList<>(); <------------------- 이 부분 !!!!!!!!!!!!!!!!!!!!!!!!!!
        while ((next = indexOf(ch, off)) != -1) {
            if (!limited || list.size() < limit - 1) {
                list.add(substring(off, next));
                off = next + 1;
            } else {    // last one
                //assert (list.size() == limit - 1);
                int last = length();
                list.add(substring(off, last));
                off = last;
                break;
            }
        }
        // If no match was found, return this
        if (off == 0)
            return new String[]{this};

        // Add remaining segment
        if (!limited || list.size() < limit)
            list.add(substring(off, length()));

        // Construct result
        int resultSize = list.size();
        if (limit == 0) {
            while (resultSize > 0 && list.get(resultSize - 1).isEmpty()) {
                resultSize--;
            }
        }
        String[] result = new String[resultSize];
        return list.subList(0, resultSize).toArray(result); <------------------ ArrayList<> -> String[] 형변환 또 해줌
    }
    return Pattern.compile(regex).split(this, limit);
}
```


---
실험결과

step1_baseline) 318.59s user 48.54s system 314% cpu 1:56.70 total

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
        
        Map<String, ResultRow> measurements = new TreeMap<>(Files.lines(Paths.get(FILE)).parallel()
            .map(l -> new Measurement(l.split(";")))
            .collect(groupingBy(m -> m.station(), collector))); //groupBy()가 map-reduce에 셔플링 역할을 함. 키값을 기준으로 데이터를 모음.
        
        // Q. why this is map reduce?
        // A. 이건 map reduce 패턴을 구현한것이지, 분산컴퓨팅 프레임워크에서 사용하는 완전한 map reduce는 아님.
        //    1. map: .map(l -> new Measurement(l.split(";")))
        //              ex) "서울;20.5" -> (서울, 20.5)
        //    2. shuffling: 같은 키를 가진 데이터를 모음
        //              ex) (서울, 20.5), (부산, 15.2), (서울, 22.1) ->
        //                  {서울: [20.5, 22.1]}, {부산: [15.2]}
        //    2. reduce: .collect(groupingBy(m -> m.station(), collector))
        //               (collector 내부의 combiner 함수가 클래식한 reduce 연산의 역할을 함)
        //              ex) {서울: [20.5, 22.1]} -> {서울: (min=20.5, max=22.1, avg=21.3)}
        
        System.out.println(measurements);
    }
}

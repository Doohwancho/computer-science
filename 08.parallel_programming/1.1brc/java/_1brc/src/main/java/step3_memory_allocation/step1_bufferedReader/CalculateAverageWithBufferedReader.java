package step3_memory_allocation.step1_bufferedReader;

import static java.util.stream.Collectors.groupingBy;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Map;
import java.util.TreeMap;
import java.util.stream.Collector;

/*
baseline + buffered reader


disk i/o 한 후에, 메모리 복사는 총 2번 일어남. 이건 일반적인 disk i/o와 같음.
1. 디스크에서 커널 버퍼(페이지 캐시)로 복사 (첫 번째 복사)
2. 커널 버퍼에서 JVM 힙의 버퍼로 복사 (두 번째 복사)

다른점은, baseline은 한번에 한줄 씩 읽어오는데,
얘는 `try (BufferedReader reader = new BufferedReader(new FileReader(FILE), 8192)) {`
로 8k 버퍼 만큼 한번에 읽어옴.

그래서 baseline에서 매 라인마다 10억 rows .txt이니까 10억번 read() syscall 해서 context switching cost 있던게,
한번에 8kb씩 읽어오니까, 그만큼 context switching cost가 줄어듬.

가지만 여전히 disk -> kernel buffer -> jvm native memory(8kb buffer) -> program reads data
로 데이터 복사가 2번씩 일어나기 때문에,

memory mapping이나 direct buffer 방식으로
데이터 복사를 2->1번으로 줄여보자.


---
실험결과

baseline) 195.28s user 5.10s system 98% cpu 3:22.42 total
step3-1_buffered reader) 198.69s user 6.25s system 94% cpu 3:37.38 total


---
Q. 왜 BufferedReader 최적화 썼는데, 왜 더 느려진거지?

1. 이미 Files.lines() 내부에서 BufferedReader를 쓰고있음.
이미 최적화 된걸 한번 더 최적화 하는 꼴임.
이미 8kb씩 버퍼 할당받아서 넣었는데,
거기에 또 8kb 버퍼 만들어서 할당하니까 더 느려짐.

```java
public static Stream<String> lines(Path path, Charset cs) throws IOException {
    // Use the good splitting spliterator if:
    // 1) the path is associated with the default file system;
    // 2) the character set is supported; and
    // 3) the file size is such that all bytes can be indexed by int values
    //    (this limitation is imposed by ByteBuffer)
    if (path.getFileSystem() == FileSystems.getDefault() &&
        FileChannelLinesSpliterator.SUPPORTED_CHARSET_NAMES.contains(cs.name())) {
        FileChannel fc = FileChannel.open(path, StandardOpenOption.READ);

        Stream<String> fcls = createFileChannelLinesStream(fc, cs);
        if (fcls != null) {
            return fcls;
        }
        fc.close();
    }

    return createBufferedReaderLinesStream(Files.newBufferedReader(path, cs)); <----- 이미 BufferedReader 쓰는걸로 최적화 되어있다!
}
```


2. baseline은 java.nio 패키지 쓰는데 이 코드는 java.io 패키지 씀. nio가 비동기기도 하고 channel기반 작동이라 os레벨 최적화가 더 잘되있는 라이브러리임
3. Files.lines()는 내부적으로 스플리터레이터(Spliterator) 최적화 되어 있어서 병렬 처리 가능성도 높음. BufferedReader는 그런 거 없음


결론) 최적화 할꺼면 잘 알고 하자...


*/


class CalculateAverageWithBufferedReader {
    
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
    
    public static void main(String[] args) throws Exception {
        Collector<Measurement, MeasurementAggregator, ResultRow> collector = Collector.of(
            MeasurementAggregator::new,
            (a, m) -> {
                a.min = Math.min(a.min, m.value);
                a.max = Math.max(a.max, m.value);
                a.sum += m.value;
                a.count++;
            },
            (agg1, agg2) -> {
                var res = new MeasurementAggregator();
                res.min = Math.min(agg1.min, agg2.min);
                res.max = Math.max(agg1.max, agg2.max);
                res.sum = agg1.sum + agg2.sum;
                res.count = agg1.count + agg2.count;
                
                return res;
            },
            agg -> {
                return new ResultRow(agg.min, agg.sum / agg.count, agg.max);
            });
        
        // 유일한 변경점: Files.lines() 대신 BufferedReader 사용
        Map<String, ResultRow> measurements = new TreeMap<>();
        try (BufferedReader reader = new BufferedReader(new FileReader(FILE), 8192)) {
            measurements = reader.lines()
                .map(l -> new Measurement(l.split(";")))
                .collect(groupingBy(m -> m.station(), collector));
        }
        
        System.out.println(measurements);
    }
}

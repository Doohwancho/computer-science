
# A. What

![](_1brc/src/main/java/_1brc/1brc.png)

measurements.txt 파일이 10억개의 rows가 있다.

1. 최대한 빠른 시간 안에 전부 읽어서
2. 각 city당 min/mean/max 를 정리하고
3. 출력하자


```
head measurements.txt

Boise;2.5
Barcelona;18.3
Ouahigouya;47.1
Parakou;29.7
Vilnius;-0.2
Monaco;15.4
Dikson;-17.2
Cape Town;12.4
Jerusalem;18.4
Palm Springs;20.0
```

# B. Index
- step1. 3분 22초
  - jdk17
- step2. 1분 35초
   - openjdk21
   - parallel
- step3. 35초
  - jdk21 GraalVM
  - parallel, but read files in 10MB chunks using FileChannel library
  - `final byte[] buffer = new byte[128];` 로 값 옮길 때 재활용(skip _1_syntax._2_syntax_details.memory de-allocation)
- step4. 23초
   - jdk21 GraalVM
   - parallel, but read files in 1MB chunks using FileChannel library
  - `final byte[] buffer = new byte[128];` 로 값 옮길 때 재활용(skip _1_syntax._2_syntax_details.memory de-allocation)
  - int로 처리했다가, 맨 마지막에 /10 해서 Double로 변환
- step5. 17초
  - jdk17, openjdk21
    - 신기한 점은, GraalVM이 오히려 느렸다!
    - 가상 쓰레드를 쓰면, 1400MB -> 8등분 해서 175MB로 나뉜 chunk를 읽는 것 보다, 10MB, 1MB로 나눠서 가상 쓰레드로 처리하는게 더 빠른 듯 하다.
    - 가상 쓰레드는 기존에 쓰레드 식으로 접근하지 말고, task 단위로 일을 나눠서 할당하라는 팁이 유효한 것 같다.
  - parallel, 근데 코어 갯수만큼 쪼개서 처리함
  - `final byte[] buffer = new byte[64];` 로 값 옮길 때 재활용(skip _1_syntax._2_syntax_details.memory de-allocation)
  - SIMD Within A Register (SWAR) for finding ‘;’.
      - Iterates over a long, instead of a byte.
  - branchless parse(skipping if-else by using bitmap 연산)
      - if-else 를 쓰면, CPU에서 성능 최적화 하기 위해 예측하고, 틀리면 수정하는데, if-else 자체를 없애면 이 구간 스킵 가능.
      - branch mis-prediction으로 인한 성능저하 예방.
      - 1 branch mis-predict cost 5ns인데, 1 billion의 20%만 miss나도 10^9 * (1/5) * 5ns = 200ms 의 시간을 아낄 수 있다.
      - royvanrijn이 측정한 branchless method으로 11000 ms를 성능 최적화 했다고 하는데, 11초이면 유의미한 차이인 듯 하다.
  - String type의 Double 숫자를 int로 파싱한 후, 맨 마지막 연산 때에만 Double로 변환
      - Integers are simpler to handle than floating-point numbers (doubles) for the CPU
      - Integers consume less _1_syntax._2_syntax_details.memory compared to doubles (typically 4 bytes for an int vs. 8 bytes for a double on most modern architectures).
      - This reduced _1_syntax._2_syntax_details.memory footprint can lead to better cache utilization, allowing more data to fit in the CPU cache and reducing the need to fetch data from the main _1_syntax._2_syntax_details.memory
  - custom hashmap that skips safety checks + String 객체 안받고 byte[]로 받아서 객체 생성 시간 & 메모리 아낌
- step6. 15.301초
  - jdk21 GraalVM
  - step5과 비교했을 때, 개념적으로 새로운 개념이 들어간 것 같지는 않다. 좀 더 세밀해진 branch prediction 정도?
  - 2MB chunk로 쪼갠걸, 다시 1/3등분 해서 멀티 쓰레드가 parallel로 처리한다.
  - hash collision을 방지하기 위해 hash size를 max_city_size인 1000보다 훨씬 큰 13만으로 설정했다.


# C. How

## c-1. jdk 여러 버전 어떻게 세팅하지?
이 실험은 step1은 jdk8부터 시작해서,\
jdk11에서도 성능테스트 해보고,\
17에서도 해보고, 21에서도 해보고,\
oracle-graalvm-21 에서도 해본다.

```agsl
 cho-cho  ~/.asdf/installs/java  tree -L 1
.
├── oracle-graalvm-21.0.2
├── zulu-11.60.19
├── zulu-17.38.21
├── zulu-21.28.85
└── zulu-8.66.0.15
```

jdk 버전 바꿀 때마다 귀찮으니까,\
build.gradle에 8,11,17,21 버전 detect하게 해놓고(되있음),\
gradle도 빌드할 때 현재 jdk 버전에 맞춰서 sdk 세팅하게 둔다(jdk뿐만 아니라 language version이라고 해야하나? 도 맞춰줌)

---
Q. asdf로 jdk 여러 버전 어떻게 받고 적용하지?\
A. chatgpt한테 물어봐잉~

Q. gradle의 sdk 세팅 어떻게 하지?
1. asdf 을 이용해서 jdk zulu 1.8, 11, 17, 21, oracle-graalvm-21.0.2 다운
2. command + , (설정) -> build, executable, deployment -> build tool -> gradle
3. gradle jvm : 'Project SDK' 로 바꾼 후 저장.

## c-2. 실험 환경
1. _1brc's 실험환경
   - eight cores of a Hetzner AX161 dedicated server (32 core AMD EPYC™ 7502P (Zen2), 128 GB RAM).
2. my pc
   - arm64, 1 CPU, 8 core, 16GiB RAM

## c-3. how to change jdk using asdf?

```
asdf current
asdf list java
asdf local java ${java-version}
or
asdf global java ${java-version}
```

## c-4. how to download 1 billion rows in .txt?

```
cd ~/dev/tree/computer-science/08.parallel_programming/1.1brc/java/_1brc/src/main/java

java _1brc.step0_create_1_billion_rows_txt.CreateMeasurements 1000000000

measurements.txt를 src/main/java/_1brc에 두기 (원래 만들면 거기에 생성됨)
```

## c-5. how to run?
```
cd ~/dev/tree/computer-science/08.parallel_programming/1.1brc/java/_1brc/src/main/java

javac _1brc/step02_parallel/CalculateAverage_royvanrijn.java

time java _1brc.step2_process_lines_in_parallel.CalculateAverage_royvanrijn
```


## c-6. how to interpret 'time' command's outcome?

```
ex) java CalculateAverage  191.55s user 5.38s system 99% cpu 3:17.40 total
```
1. CalculateAverage:
    - 실행한 자바 클래스 이름
2. 191.55s user:
    - amount of CPU time spent in user-mode.
    - This does not include time spent doing system-level tasks or waiting for I/O operations
    - A higher value here means your program spent a lot of time performing calculations or processing data.
3. 5.38s system:
    - This is the amount of CPU time spent in the kernel (system-mode) within the context of this process
    - It indicates the time the operating system kernel spent performing tasks on behalf of the program, such as handling file I/O, network communications, or other system calls
    - A lower value here typically means your program did not require the system to do a lot of work on its behalf
4. 99% cpu:
    - CPU usage
5. 3:17.40 total
    - elapsed real time from start to finish of the command you ran


# D. step01 - Baseline

## d-1. idea
Map-reduce(Collections.collector) 로 1 billion rows의 min/mean/max 계산을 빨리 해보자.

jdk17쓰면 빨라지나? (별 영향 없는듯)


### d-1-1. Q. what is map reduce?

쉽게 말해 멀티 코어 멀티 쓰레드 환경에서 일을 여러 쓰레드한테 나눠줘서 동시에 처리해야 빠를거 아냐?

근데 다른 단위의 사이즈로 나눠주거나, 각 쓰레드마다 다른 종류의 일 시키면 어려워지잖아?

그러니까 같은 단위의 일을 잘게 쪼개서 쓰레드한테 건내준 다음, 여러 쓰레드가 멀티코어들 돌면서(concurrency control 때문에 좀 막히겠지) 일처리 다 끝나면,

맨 끝에 결과를 aggregate하는거지.

쪼갠 일의 단위도 같고 성질도 같아서, 합칠때도 수월함.

example)
```java
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

Map<String, ResultRow> measurements = new TreeMap<>(Files.lines(Paths.get(FILE))
    .map(l -> new Measurement(l.split(";")))
    .collect(groupingBy(m -> m.station(), collector)));
```

### d-1-2. 내부적으로 map reduce 쓰니까 병렬처리 하겠네?

ㄴㄴ 
```java
Map<String, ResultRow> measurements = new TreeMap<>(Files.lines(Paths.get(FILE))
    .parallel() // 이 부분 추가해야 병렬연산함. 원본코드엔 없으니까 싱글코어로 돔 
    .map(l -> new Measurement(l.split(";")))
    .collect(groupingBy(m -> m.station(), collector)));
```

병렬처리하려면 명시적으로 `.parallel()`호출해야 함\
step1_baseline에는 `.parallel()`이 없음.\
이 코드는 싱글코어로 돔. 다른 코어들은 놈.\
아무리 map reduce 방식을 써서 멀티 쓰레드를 쓴다고 해도,\
싱글코어 쓰니까 제일 느림.

**따라서 baseline 코드는 map reduce 패턴을 쓴거지 실제 100% map reduce가 아님**


## d-2. experiment

### jdk17: 3분 22초

```
195.28s user
5.10s system
98% cpu
3:22.42 total
```
- 혹시나 해서 jdk8 버전도 테스트 해봤는데, jdk17버전과 성능상 비슷했음


# E. step02 - parallel

## e-1. idea

map reduce에서 파일 읽는걸 병렬처리 해보자

step1 code는 single core에서만 도는데,\
이 방식은 여러 코어에서 병렬로 돔

```java
Map<String, Measurement> resultMap = Files.lines(Path.of(FILE)).parallel()
```
...을 사용한 방법

## e-2. .parallel().groupBy(groupingByConcurrent());
```java
Map<String, Measurement> resultMap = Files.lines(Path.of(FILE)).parallel() //병렬로 읽기
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
```


## e-3. jdk17: 1분 44초
```
300.36s user
29.69s system
316% cpu
1:44.34 total
```

- 병렬 프로그래밍 썼더니, user space에서 소요시간이 150% 늘었고,
- 커널모드에서 file i/o에 쓰는 시간도 600% 늘었는데,
- 총 소요시간은 1분 38초나 줄었다.

단, 316% CPU usage까지 올라가는걸 보면, 오래하다간 쓰로틀링 엄청 걸릴 듯 하다.


## e-4. openjdk21: 1분 35초
```
412.60s user
29.51s system
464% cpu
1:35.21 total
```
jdk21의 .parallel()이 jdk17보다 더 최적화가 잘되있거나,
jdk21에 experimental 기능중에 하나인 경량 쓰레드인 fibers가 .parallel() 수행시 생기는 concurrency overhead 비용을 줄여줘서 성능개선이 된 듯 하다.

CPU 사용량만 봐도, 150%나 높은걸 보면, 수 많은 경량 쓰레드인 fibers가 CPU를 동시에 점유하려고 하기 때문에 저렇게 높히 올라간 것으로 보인다.


---
# F. step03 - parallel, but read files in 10MB chunks

## f-1. idea
map reduce에서 파일 읽는걸 병렬처리로 하는데,

chunk단위를 10MB로 쪼개서 읽어보자

## f-2. parallel()
파일을 여러 코어에서 병렬로 읽는다

### 0-2. 10MB chunk씩 끊어 읽기
NIO의 FileChannel을 파일 전체를 한번에 읽는게 아니라, 10MB단위로 끊어서 parallel()로 여러 코어들이 읽으면 빨라진다.

### 0-3. MappedByteBuffer로 _1_syntax._2_syntax_details.memory de-allocation 없이 덮어쓰면서 재사용하기
파일을 10MB chunk 단위로 끊어 읽을 때,
MappedByteBuffer 단위로 읽고,

```
final byte[] buffer = new byte[128];
```
여기에 필요한 값을 담아서 쓴다.

그것이 city's name이던, Double 값이던.


### 1. jdk17: 2분 17초
```
130.87s user
6.61s system
99% cpu
2:17.50 total
```

- 여태까지는 java.nio.file.Files 라이브러리를 썼는데, 그와 더불어 java.nio.channel 라이브러리를 썼더니 성능향상이 있었다.
- 그런데 fibers 전용인지 jdk21과 압도적인 성능차이가 난다.


### 2. openjdk21: 44초
```
159.59s user
16.36s system
397% cpu
44.293 total
```

경량 쓰레드를 쓸 때마다 CPU 점유율이 미친듯이 오르는 단점과는 별개로, 성능 차이 또한 미친듯이 많이 난다.



-----------------------------------------------
## step04 - parallel, but read files in 1MB chunks

### 0. idea
map reduce에서 파일 쪼개서 읽는걸 병렬로 하는데,

파일을 쪼개는 단위를 1MB로 해보자.

step3과 차이점은, 10MB chunk -> 1MB chunk로 바뀐 것과, Double을 int처럼 처리한 것 두가지다.


1. parallel
   - [160.5 -> 18]
2. 10MB로 파일을 끊어서 병렬처리하다가, 1MB 단위로 끊어 읽으니 성능이 유의미하게 개선되었다!
    - 전체 파일을 한번에 parallel로 읽는게 아니라, manageable chunks로 나눠서 읽으면, _1_syntax._2_syntax_details.memory overload를 피할 수 있다.
    - parallel processing techniques 중 하나라고 한다.
    - local pc 기준, 아래 두 테크닉 포함해서 12초 시간개선이 되었다.
3. _1_syntax._2_syntax_details.memory de-allocation 피하기
    - [18 -> 6.5]
   - city이름을 옮길 때, `byte[] nameBuffer`를 쓰는데, 딱 필요한 길이의 bytecode size만 length로 빼고 new String(byte[], 0, length);로 써서, _1_syntax._2_syntax_details.memory de-allocation step을 스킵할 수 있었다.
4. 12.3에서 Double 값이 소숫점 한자리인데, 메모리 저장할 땐 int type에 123로 저장하고, 맨 마지막에 min,max,mean 계산할 때만 /10 해서 Double type으로 변환하는 방식
    - Double type은 소숫점 처리하는 계산 때문에 CPU단에서 int type 숫자 처리보다 느리다.
    - Integers are simpler to handle than floating-point numbers (doubles) for the CPU
    - Integers consume less _1_syntax._2_syntax_details.memory compared to doubles (typically 4 bytes for an int vs. 8 bytes for a double on most modern architectures).
    - This reduced _1_syntax._2_syntax_details.memory footprint can lead to better cache utilization, allowing more data to fit in the CPU cache and reducing the need to fetch data from the main _1_syntax._2_syntax_details.memory



### 1. jdk17: 30초
```
80.37s user
13.37s system
308% cpu
30.356 total
```

### 2. openjdk21: 27초
```
73.73s user
14.26s system
319% cpu
27.560 total
```

### 3. jdk21 GraalVM: 23초
```
53.33s user
11.77s system
272% cpu
23.853 total
```


---
## step05 - SWAR

### 0. idea

1. map-reduce 하기 이전 전초작업으로 1.4GiB 파일을 코어 갯수만큼(8 core) 8등분 segment로 쪼갬
2. java는 big endian인데, small endian으로 바꿔서 파일을 읽도록 수정함
3. 각 코어(8개)에서 병렬로 map reduce를 수행함
4. map 단계에서 chunk로 쪼개는데, 여러 optimization technique들이 적용된
   1. 이 때, key:value에서 SWAR로 'Barcelona;18.3' 에서 ';'의 위치를 찾음
   2. ';'의 이전 문자열을 String으로 변환하지 않고 byte[]로 바로 custom hashmap에 validation check 없이 다이렉트로 저장함 (skipping new String Object creation)
   3. ';'이후 Double type의 18.3을 int로 변형해서 custom hashmap에 저장함
   4. branchless (skipping if-else using bitmask)로 1 branch mis-prediction 당 5ns cost를 save (1 billion rows의 20%가 branch mis-prediction만 일어나도 200ms)
   5. 이 key:value pair chunk들을 옮길 때 64 byte sites buffer array를 _1_syntax._2_syntax_details.memory de-allocation 없이 덮어쓰기로 재사용하면서 저장
5. calculation 단계에서 나눴던 여러 같은 단위의 key:pair chunk들(Measurement)을 처리(min, max, mean, count calculation)를 virtual thread로 엄청 많은 가상 쓰레드가 생성되어 처리
6. step5에서 처리한 chunk들의 결과값을 reduce 단계에서 합침
   1. java Collections' collector로 TreeMap 자료구조로 합침
7. 결과를 콘솔에 출력함


#### 0-1. what is SWAR?

- stands for SIMD(Single Instruction, Multiple Data) Within A Register
- a technique for performing multiple, parallel operations on a single data word within a processor's register.
- This approach utilizes the full width of a register to execute the same operation on multiple smaller data elements simultaneously
- SWAR enables a form of Single Instruction, Multiple Data (SIMD) operations without the need for specialized SIMD hardware or instructions.
- The key to SWAR is the ability to apply bitwise operations, addition, subtraction, and other arithmetic operations across these smaller data units in parallel
- particularly good at
    1. fast searching and counting
        - Quickly searching for occurrences of a particular byte within a word or counting the number of set bits
    2. Parallel Comparisons
        - Performing parallel comparisons or masking operations on multiple data elements at once.
    3. Text Processing
        - Accelerating text processing tasks, such as finding delimiters or parsing numbers from strings


### 1. jdk17: 17초
```
30.79s user
9.23s system
236% cpu
16.918 total
```

- jdk21 GraalVM의 가상 쓰레드를 사용하지 않고 jdk17에서도 이정도의 성능을 낸게 새삼 대단하다 느껴진다.

### 2. openjdk21: 17초
```
33.05s user
9.18s system
243% cpu
17.327 total
```

### 3. jdk21 GraalVM: 19초
```
31.70s user
8.53s system
212% cpu
18.966 total
```

---
## step06 -

### 0. idea
1. direct _1_syntax._2_syntax_details.memory access (reducing overhead of read/write operation)
   1. _1_syntax._2_syntax_details.memory mapping
      1. `FileChannel.map()` allows direct _1_syntax._2_syntax_details.memory access
      2. efficient for large file i/o than traditional i/o stream
   2. [Arena](https://cr.openjdk.org/~pminborg/panama/21/v1/javadoc/java.base/java/lang/foreign/Arena.html)
      1. jdk21의 project panama에서 나온 기능
      2. arena controls the lifecycle of native _1_syntax._2_syntax_details.memory segments, providing 1. flexible allocations and 2. timely disallocation
      3. `fileChannel.map(FileChannel.MapMode.READ_ONLY, 0, fileSize, java.lang.foreign.Arena.global()).address();`
   3. sun.misc.Unsafe
      1. directly reading _1_syntax._2_syntax_details.memory, bypassing safety checks of JVM
      2. but be careful of _1_syntax._2_syntax_details.memory corruption
2. parallel
   1. create a worker thread per available core
   2. first divide files into segments, 2MB chunks
      - 2MB size가 너무 작지도, 크지도 않은 사이즈라, 엄청 많은 virtual thread들 중에서 먼저 끝낸애가 있으면 바로바로 2MB씩 쪼개서 처리할 수 있는 사이즈 같다.
      - 사이즈가 너무 작아지면, 엄청 잘게 쪼갠다는 건데, 그러면 load balancing 측면에서는 유리해지지만, thread context switching 측면에서는 불리해지는데, 적절한 size가 2MB인 듯 하다.
   3. and multiple threads takes part in those segment to process parallely
   4. subprocess for resource cleanup
       1. main process가 값을 return하는 동안, sub-process가 _1_syntax._2_syntax_details.memory mapping cleanup을 해준다.
3. branchless programming
   1. branchless number parsing
   2. goal: reduce cpu branch mis-prediction
4. lookup tables and masking for parsing
   1. when parsing data, instead of arithmetic or branch operations, utilize lookup tables & bitmask operation
   2. especially efficient for data with delimiter in between
5. hash 충돌 최소화 하기위한 hash size를 크게 하기
    - max_city 갯수가 1000개인데, hash table size를 13만으로 정한건, 넉넉한 hash table size + avoid hash-collision 만큼 충분히 큼 (city 이름이 1000개니까, 13만개면 hash-collision 최대한 피할 수 있지 않을까? -> _1_syntax._2_syntax_details.memory 엄청 써서 cpu optimize 한 방법
    - 다만, Result 객체 13만개가 들어갈 공간만큼 RAM 메모리 확보해야 하니까, RAM 사이즈, heap area가 커야한다.

### 1. how to run?
```
javac --enable-preview --release 21 CalculateAverage_thomaswue.java
java --enable-preview CalculateAverage_thomaswue
```

### 2. jdk21 GraalVM: 15.3초
- 8.82s user 8.35s system 112% cpu 15.301 total



-----------------------------------------------
# F. Resources

1. https://github.com/gunnarmorling/1brc?tab=readme-ov-file
2. https://tivrfoa.github.io/java/benchmark/performance/2024/02/05/1BRC-Timeline.html

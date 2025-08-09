
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

# summary 
## layer별 최적화 기법 
1. CPU / 하드웨어 레이어
    - 벡터 API (SIMD): 하나의 CPU 명령으로 여러 데이터를 동시에 처리하여 파싱 속도를 극대화.
    - sun.misc.Unsafe: JVM의 안전장치를 우회하여 C++처럼 메모리에 직접 접근하고 값을 읽고 씀.
    - CPU 캐시 최적화: 데이터 구조를 설계할 때 CPU 캐시 라인(보통 64바이트)을 고려하여 캐시 미스(Cache Miss)를 최소화.
    - 분기 예측 최적화: if문 같은 조건 분기를 최소화하여 CPU가 다음에 실행할 명령을 더 쉽게 예측하도록 함.
2. OS / 커널 인터랙션 레이어
    - 메모리 맵 파일 (mmap): 파일을 메모리의 일부처럼 다루게 하여 read() 시스템 콜 반복을 없애고 I/O 오버헤드를 제거.
    - 스레드 어피니티 (Thread Affinity): 특정 스레드를 특정 CPU 코어에 고정시켜, 코어 간 이동으로 인한 캐시 무효화를 방지.
3. JVM / 애플리케이션 레이어
    - 커스텀 해시맵: java.util.HashMap의 오버헤드(객체 생성, 동기화, 충돌 처리)를 없앤 직접 만든 자료구조 사용.

## step1) 파일 읽기(i/o)
1. Naive Version (일반적인 방식)
    - 방법: Files.lines()나 BufferedReader를 사용해 한 줄씩 읽습니다.
    - 과정:
        1. read() 시스템 콜을 호출하여 디스크에서 데이터를 커널 버퍼로 가져옵니다.
        2. 커널 버퍼의 데이터를 다시 애플리케이션 버퍼로 복사합니다. (비용 발생)
        3. 버퍼의 바이트 데이터를 UTF-8 문자열로 디코딩합니다. (CPU 연산 비용 발생)
        4. 줄바꿈 문자(\n)를 찾아 한 줄을 String 객체로 만듭니다.
        5. 1~4 과정을 10억 번 반복합니다.
2. Advanced Version (최상위권 방식)
    - 방법: **메모리 맵 파일 (mmap)** 을 사용합니다.
    - 최적화/생략된 과정:
        1. 프로그램 시작 시 단 한 번의 mmap 시스템 콜로 파일 전체를 가상 메모리 주소 공간에 매핑합니다.
        2. 이제 파일은 거대한 byte 배열처럼 취급됩니다.
        3. (생략) read() 시스템 콜 반복, 커널-애플리케이션 버퍼 간 데이터 복사, 문자열 디코딩 과정이 모두 생략됩니다.
        4. 여러 스레드가 이 거대한 바이트 배열의 각기 다른 구역을 맡아 동시에 병렬로 처리합니다.

## step2) 한줄 파싱(데이터 처리)
1. Naive Version
    - 방법: String.split(";")을 사용합니다.
    - 과정:
        1. 한 줄(String)을 받아 ; 문자를 기준으로 분리합니다.
        2. 분리된 두 개의 새로운 String 객체(station, temperature)를 생성합니다. (메모리 할당 오버헤드)
        3. 온도 문자열을 Double.parseDouble()로 파싱하여 double 타입으로 변환합니다.
2. Advanced Version
    - 방법: 직접 바이트를 탐색하고, 벡터 API(SIMD)를 활용합니다.
    - 최적화/생략된 과정:
        1. (생략) String 객체를 아예 생성하지 않고, mmap으로 얻은 byte 배열 위에서 직접 작업합니다.
        2. ;의 바이트 값(0x3B)을 찾기 위해 벡터 API를 사용합니다. 한 번의 CPU 명령으로 16바이트 또는 32바이트 내에 ;가 있는지 동시에 확인합니다.
        3. (생략) Double.parseDouble() 대신, 숫자 문자(바이트)에서 '0'의 바이트 값을 빼는 등 산술 연산을 통해 매우 빠르게 숫자로 변환합니다.

## step3) 데이터 집계 
1. Naive Version
    - 방법: `HashMap<String, Stats>`를 사용합니다.
    - 과정:
        1. Station 이름(String)으로 hashCode()를 계산합니다.
        2. HashMap에서 해당 키가 있는지 찾습니다.
        3. 해시 충돌이 발생하면 연결된 노드를 순회하며 equals()로 비교합니다. (성능 저하)
        4. 찾은 Stats 객체의 값을 업데이트합니다.
        5. 여러 스레드가 동시에 접근할 경우 synchronized 블록이나 ConcurrentHashMap을 사용해야 하므로 성능이 저하됩니다.
2. Advanced Version
    - 방법: 직접 만든 커스텀 해시맵을 사용합니다.
    - 최적화/생략된 과정:
        1. (생략) String 객체와 hashCode() 계산을 생략합니다. 파싱된 바이트 배열 자체를 키로 사용하거나, 내용물을 long 타입 등으로 변환하여 키로 사용합니다.
        2. Station 이름이 500개 미만인 점을 이용해, 충돌이 거의 없는 완벽에 가까운 해시 함수를 직접 만들어 사용합니다. 해시 충돌이 없으면 탐색 과정이 O(1)이 됩니다.
        3. Unsafe API를 사용해 JVM의 경고를 무시하고 메모리의 특정 주소에 계산 결과를 직접 씁니다.
        4. 각 스레드가 자신만의 로컬 해시맵을 갖게 하여 동기화(synchronization) 자체를 없애고, 모든 작업이 끝난 후에야 결과를 합칩니다.


# Roy van Rijn's approach 
## step1. .parallel()
- 스레드 1개가 아니라 스레드 병렬로 동시처리 
- naive 버전이 4~5분인데, 이 한줄만으로 71 seconds, 몇배는 빨라짐 

## step2. FileChannel
원래 파일 읽으면, bufferedReader 같은거 쓰고 별거 했는데, 
FileChannel은 그냥 메모리에 올리고 쓰는 방법 

```java
// Main 클래스 또는 스레드를 시작하는 부분
public class CalculateAverage_gunnarmorling {

    private static final String FILE = "./measurements.txt";

    public static void main(String[] args) throws IOException {
        // 1. 파일을 열고 파일 채널을 얻음
        try (FileChannel fileChannel = FileChannel.open(Path.of(FILE), StandardOpenOption.READ)) {
            final long fileSize = fileChannel.size();
            final int numberOfThreads = Runtime.getRuntime().availableProcessors();
            final long chunkSize = fileSize / numberOfThreads;

            // 2. 파일 전체를 메모리에 직접 매핑 (가상 메모리)
            //    이제 파일은 거대한 byte 배열처럼 취급됨
            final MemorySegment mappedSegment = fileChannel.map(FileChannel.MapMode.READ_ONLY, 0, fileSize, Arena.global());

            final Thread[] threads = new Thread[numberOfThreads];
            
            // 3. 각 스레드에 처리할 파일 구역(chunk)을 할당
            for (int i = 0; i < numberOfThreads; i++) {
                final long chunkStart = i * chunkSize;
                final long chunkEnd = (i == numberOfThreads - 1) ? fileSize : (i + 1) * chunkSize;

                threads[i] = new Thread(() -> {
                    // 각 스레드는 자신만의 구역을 처리함
                    processChunk(mappedSegment, chunkStart, chunkEnd);
                });
                threads[i].start();
            }

            // 모든 스레드가 끝날 때까지 대기
            for (Thread thread : threads) {
                thread.join();
            }
            // ... 결과 취합 및 출력 ...
        }
    }
}
```
- fileChannel.map(...): 이 한 줄이 핵심입니다. 파일 전체를 운영체제의 가상 메모리에 매핑하여, 자바 힙을 거치지 않고 파일 데이터에 직접 접근할 수 있습니다. read()를 반복 호출할 필요가 없어집니다.
- chunkStart, chunkEnd: 10GB 파일을 10개의 스레드로 처리한다면, 각 스레드는 1GB씩의 구역을 할당받습니다. 스레드들은 서로의 작업에 전혀 영향을 주지 않고 완전히 병렬로 동작합니다.



## step3. break files into multiple pieces 

```java
final File file = new File(MEASUREMENTS_TXT);
final long length = file.length();
final int chunkCount = Runtime.getRuntime().availableProcessors();
final var results = new StationStats[chunkCount][];
final var chunkStartOffsets = new long[chunkCount];
try (var raf = new RandomAccessFile(file, "r")) {
    final var inputBase = raf.getChannel().map(MapMode.READ_ONLY, 0, length, Arena.global()).address();
    for (int i = 1; i < chunkStartOffsets.length; i++) {
        var start = length * i / chunkStartOffsets.length;
        raf.seek(start);
        while (raf.read() != (byte) '\n') {
        }
        start = raf.getFilePointer();
        chunkStartOffsets[i] = start;
    }
    var threads = new Thread[chunkCount];
    for (int i = 0; i < chunkCount; i++) {
        final long chunkStart = chunkStartOffsets[i];
        final long chunkLimit = (i + 1 < chunkCount) ? chunkStartOffsets[i + 1] : length;
        threads[i] = new Thread(new ChunkProcessor(inputBase + chunkStart, inputBase + chunkLimit, results, i));
    }
    for (var thread : threads) {
        thread.start();
    }
    for (var thread : threads) {
        thread.join();
    }
}
```
- `.parallel()`은 스레드 병렬처리하는데, 파일은 큰 한덩이니까, 파일을 여러 쓰레드가 병렬처리할 수 있게 쪼갠 것. 
- 파일을 8조각으로 나눠서 8개 스레드가(8core) 병렬로 처리 
- 처음엔 사람들이 8조각으로 나눴다가, 나중엔 더 세밀하게 나눴더라 (2mb 단위로 나눔)
    1. 왜냐면 8조각으로 나누고 병렬로 처리해도, 맨 마지막에 젤 오래걸리는애 기다려야 하니까, 그 시간마저 아끼려고 2mb단위로 나눈 것 
        - 아마 여기서 virtual thread써서 native thread가 코어 1개씩 물려있고, virtual thread에 task가 끝나면 다음꺼 밀어넣는 식으로 한 듯? 
    2. cpu cache사이즈가 around 2mb 사이즈라, cpu-L2-cache -> cpu-L1-cache -> done -> replace entire cpu-L2-cache 

## step4. parsing 
### 4-1. parse by ';', 근데 byte단위로 읽어옴 string이 아니라 
```java
while(buffer.hasRemaining()) {
    byte b = buffer.get();
    char c = (char) b;

    if(c == ';') {
        state = State.NEXT_READ_MEASUREMENT;
        continue;
    }
}
```
1. while buffer has remaining bytes, read by byte, (char) c, 
2. if that byte is ';', change state 

```java
if (c == '\n') {
  String city = new String(citySink.getBytes(), UTF_8);
  int measurement = bytesToInt(measurementSink.getBytes());
  var entry = results.get(city);
  if (entry == null) {
    entry = new MeasurementAggregator();
    results.put(city, entry);
  }
  entry.count++;
  entry.sum += measurement;
  entry.min = (short) Math.min(entry.min, measurement);
  entry.max = (short) Math.max(entry.max, measurement);
  citySink.clear();
  measurementSink.clear();
  state = State.NEXT_READ_CITY;
  continue;
}
while (buffer.hasRemaining()) {
  byte b = buffer.get();
  char c = (char) b;
  if (c == ';') {
    state = State.NEXT_READ_MEASUREMENT;
    continue;
  }
}

switch (state) {
  case NEXT_READ_CITY -> citySink.append(b);
  case NEXT_READ_MEASUREMENT -> measurementSink.append(b);
}
```
1. ';' 앞뒤로 처리하고 
2. state에 따라 처리 



### 4-2. float -> int로 취급해서 데이터 처리 후, 1/10 해줌 
1. 왜냐면 float 연산보다 int연산이 더 빠르고 
2. 어짜피 float가 전부 1의자리 숫자밖에 없었어서 1/10 연산하면 일괄적용되었음 

```java
static int bytesToInt(byte[] bytes) {
  short index = (short) (bytes.length - 1);

  boolean isNegative = (bytes[0] == '-');
  int number = (bytes[index] - '0');
  index--;

  number += (10 * (bytes[index--] - '0'));

  if (index == 1 || (!isNegative && index == 0)) {
    number += (100 * (bytes[index] - '0'));
  }

  if (isNegative) {
    return -number;
  } else {
    return number;
  }
}
```

- 근데 Integer.parseInt() 같은 고수준 API는 절대 사용하지 않고, 오직 비트 연산만으로 숫자를 파싱함. 
- 목표: parseDouble, parseInt의 오버헤드를 완전히 제거.

```java
// Awesome idea of merykitty:
private int readTemperature() {
    // 1. 온도 부분을 8바이트 long으로 읽어옴 (예: "-12.3\n" -> 바이트 덩어리)
    final long numberBytes = UNSAFE.getLong(ptr);
    
    // 2. '.'의 위치를 비트 연산으로 찾아냄
    final int dotPosition = Long.numberOfTrailingZeros(~numberBytes & 0x10101000);

    // 3. 부호(-) 비트를 비트 연산으로 알아냄
    final long signed = (~numberBytes << 59) >> 63; // 음수면 -1, 양수면 0

    // 4. 소수점 위치에 따라 각 숫자 바이트를 분리하고 정수로 변환
    final long digits = ((numberBytes & ~(signed & 0xFF)) << (28 - dotPosition)) & 0x0F000F0F00L;
    
    // 5. '마법의 곱셈'으로 각 자릿수를 100, 10, 1의 가중치로 합산
    final long absValue = ((digits * 0x640a01L) >>> 32) & 0x3FF;
    
    // 6. 부호를 적용하여 최종 int 값을 얻음
    return (int) ((absValue + signed) ^ signed);
}
```
- 이 코드는 -, ., 숫자의 ASCII 바이트 값 패턴을 이용해, 단 몇 번의 비트 연산과 한 번의 곱셈으로 문자열을 정수로 변환합니다. if문 하나 없이 모든 경우의 수(예: X.X, -X.X, XX.X)를 처리합니다.





### 4-3. SIMD (vector api, still incuvabor ver)
single instruction, multiple data 

저 byte memory chunk를 벡터화 시켜서 가져옴. 

```java
// processChunk 메소드 내부의 반복문 (thomaswue님의 솔루션 참고)
private void processChunk(MemorySegment segment, long start, long end) {
    // SIMD 연산을 위한 벡터 선언 (여기서는 256비트, 즉 32바이트)
    final VectorSpecies<Byte> SPECIES = ByteVector.SPECIES_256;
    final ByteVector SEMICOLON_VECTOR = ByteVector.broadcast(SPECIES, ';'); // 세미콜론으로 채워진 벡터

    long currentOffset = start;
    while (currentOffset < end) {
        // 1. 현재 위치에서 32바이트를 하나의 벡터로 로드
        ByteVector chunkVector = ByteVector.fromMemorySegment(SPECIES, segment, currentOffset, ByteOrder.nativeOrder());
        
        // 2. 32바이트 안에 ';'가 있는지 단 한 번의 CPU 명령으로 비교
        VectorMask<Byte> mask = chunkVector.compare(VectorOperators.EQ, SEMICOLON_VECTOR);
        
        // 3. ';'가 있다면, 그 첫 번째 위치를 찾음
        int semicolonPos = mask.firstTrue();

        // 4. station 이름과 온도를 바이트 단위로 직접 파싱 (String 객체 생성 안 함)
        //    ... (semicolonPos를 이용해 station 이름과 온도 부분의 offset 계산) ...
        
        // 5. 숫자를 매우 빠른 방식으로 파싱 (예시)
        //    - (c - '0') 같은 산술 연산으로 각 자릿수 계산
        //    - 비트 연산으로 소수점 처리
        // ...
        
        // 다음 라인으로 오프셋 이동
        currentOffset += ...; 
    }
}
```
- ByteVector.fromMemorySegment(...): 메모리에서 32바이트(문자 32개)를 한 번에 읽어와 하나의 Vector 객체로 만듭니다.
- chunkVector.compare(...): 이 부분이 SIMD의 마법입니다. 32개의 문자를 하나씩 ;와 비교하는 대신, 단 한 번의 CPU 명령으로 32개 문자를 동시에 비교하여 결과(mask)를 얻습니다.
- mask.firstTrue(): 비교 결과에서 ;가 있는 첫 번째 위치를 즉시 찾아냅니다.
- 직접 숫자 파싱: Double.parseDouble() 대신 바이트 값에 직접 산술 연산을 적용합니다. 예를 들어 음수 부호(-), 소수점(.)을 직접 확인하고, 숫자 바이트 값에서 '0'의 바이트 값을 빼서 실제 숫자 값을 얻는 방식을 사용합니다.



------
다른 설명 

```java
private static int lineSize(long lineStart, MemorySegment buffer) {
  ByteVector line = ByteVector.fromMemorySegment(BYTE_SPECIES, buffer, lineStart,
      ByteOrder.nativeOrder());

  int lineSize = line.compare(VectorOperators.EQ, '\n').firstTrue();
  int index = lineSize;
  while (index == BYTE_SPECIES.vectorByteSize()) {
    index = ByteVector.fromMemorySegment(BYTE_SPECIES, buffer, lineStart + lineSize,
        ByteOrder.nativeOrder()).compare(VectorOperators.EQ, '\n').firstTrue();
    lineSize += index;
  }
  return lineSize;
}
```
- Q. bottleneck: 여태껏은 per-byte-bases. 이걸 multiple bytes를 동시에 읽게할 수 있을까?
- SIMD는 create vector with bytes, and do an operation on that you can fine ';' separator in multiple bytes 
- 원래는 이런거 하려면 c,c++ 밖에 못했는데 이런 java api 같은거 나오면서 가능해짐  

4등한 사람이 oracle jdk 안쓰고 openjdk 썼는데, 이 SIMD기능이 openjdk에는 있는데 oracle jdk(GraalVM)에는 없는 기능이라서. 

그래서 GraalVM쓰는 사람들이 고안한게, bit shift를 해서 graalVM에서 지원 안하는 SIMD 기능을 구현함. 
```java
long lastRead = UNSAFE.getLong(ptr);

entryLength += 16;

// Find delimiter and create mask for long
long comparisonResult1 = (lastRead ^ DELIMITER_MASK);
long highBitMask1 = (comparisonResult1 - 0x0101010101010101L) & (~comparisonResult1 & 0x8080808080808080L);

boolean noContent1 = highBitMask1 == 0;
long mask1 = noContent1 ? 0 : ~((highBitMask1 >>> 7) - 1);
int position1 = noContent1 ? 0 : 1 + (Long.numberOfTrailingZeros(highBitMask1) >>> 3);

readBuffer1 = lastRead & ~mask1;
hash ^= readBuffer1;

// More bit-shifting magic ...
```
1. string bytes -> long 
2. find a separator ';' 
3. ... without making branch (cpu 최적화)

there is no conditional / branch
they work sequentially -> cpu love sequentially executed code, not branches 


### 4-4. SWAR
- SIMD Within A Register 기법 
- 하나의 64비트 long 변수를 8개의 byte로 취급하여, 비트 연산만으로 분기(if문) 없이 원하는 바이트를 찾아내는 신기에 가까운 기술 
- 목표: 벡터 API(SIMD)보다 더 저수준에서, if문 없이 ; 문자의 위치를 찾기.

```java
private boolean readNext() {
    // 1. 메모리에서 8바이트를 하나의 long으로 읽어옵니다.
    long lastRead = UNSAFE.getLong(ptr);

    // 2. 이 8바이트 안에 ';'가 있는지 마법의 비트 연산으로 확인합니다.
    //    DELIMITER_MASK는 8개의 ';' 바이트(0x3B)로 채워진 long입니다.
    //    XOR 연산으로 ';'와 같은 바이트는 0x00이 됩니다.
    long comparisonResult = (lastRead ^ DELIMITER_MASK);

    // 3. 0x00이 된 바이트를 찾아내는 또 다른 마법의 비트 연산
    long highBitMask = (comparisonResult - 0x0101010101010101L) & (~comparisonResult & 0x8080808080808080L);

    // highBitMask가 0이 아니라면, ';'를 찾았다는 의미입니다.
    if (highBitMask != 0) {
        // 4. ';'의 정확한 위치(0~7)를 비트 연산으로 계산합니다.
        int position = (Long.numberOfTrailingZeros(highBitMask) >>> 3);
        // ...
        return false; // 파싱 중단
    }
    // ...
    return true; // ';' 못 찾음, 다음 8바이트 계속 읽기
}
```
- long을 8개의 byte로: 자바는 long을 숫자 하나로 보지만, 이 코드는 long을 8개의 독립된 byte 칸으로 보고 비트 연산을 수행합니다.
- 분기 없는(Branchless) 탐색: for 루프를 돌며 if (c == ';')를 확인하는 대신, XOR와 AND, 쉬프트 같은 몇 번의 비트 연산만으로 8바이트 안에 ;가 있는지, 있다면 몇 번째에 있는지를 알아냅니다. CPU는 if문 같은 분기 예측을 할 필요 없이 순차적으로 명령어만 실행하면 되므로 엄청나게 빠릅니다.



### 4-5. branchless programming 
1. cpu는 여러개의 instruction들을 parallel로 동시에 실행하는데, 
2. 이게 잘 작동하려면, next instruction이 뭔질 알아야 하는데, if-else, switch같은 branch가 있거나, array가 아니라 list같은 애면 다음에 뭐가 올지 모름 
3. if-else, switch문 같은 branch가 있으면, compiler가 어떤게 다음에 올지 predict 해서 맞추면 좋은데, 틀리는 경우가 있음 -> 다시 fetch해야 되서 느려짐 

`if (condition) { a = b; }` 와 같은 코드는 분기 예측 실패 시 큰 비용을 유발함. 
따라서 if문을 없애야 하는데, 
`if (s.len() >= 4)` 와 같은 조건문 대신, `(s.length() >= 4 ? 1 : 0)` (Java) 와 같은 표현식을 사용하여, 결과를 값에 곱해줌. 거짓이면 값이 0되는 것. 

```java
// 10의 자리 숫자인 b는 길이가 4 이상일 때만 유효함
int b_value = (bytes[len - 4] - '0'); 

// if (len >= 4) { value += b_value * 100; } else { value += 0; } 를 아래와 같이 바꿈
int lenCheck = (len >= 4) ? 1 : 0;
int value = b_value * 100 * lenCheck  // 조건이 거짓이면 이 항은 0이 됨
          + c_value * 10
          + d_value;
```
핵심: CPU는 if문을 만났을 때 다음에 실행할 코드를 '예측'해야 하지만, 곱셈은 예측할 필요 없이 그냥 실행하면 됩니다. 

분기 예측 실패로 인한 파이프라인 스톨(pipeline stall)을 원천적으로 차단하는 기법 





## step5. custom data structure 
### 5-1. custom hashmap 
그냥 hashmap은 hash() 후 같은자리에 충돌일어나면 hash collision 보정 + 같은자리에 규모 작으면 linkedlist, 많아지면 tree 만드는데,
이 부분이 병목지점. 

이걸 어떻게 해결했냐면, 

같은 자리에서 충돌일어나면, 걍 옆자리에 넣자는 것. (linear probing)

```java
private static class ByteArrayKeyedMap {
  private final int BUCKET_COUNT = 0xFFFF;
  private final MeasurementAggregator[] buckets = new MeasurementAggregator[BUCKET_COUNT + 1];

  public MeasurementAggregator computeIfAbsent(final byte[] key, final int keyLength, final int hashCode) {
    //step1) run hash() and get hash-key
    var index = hashCode & BUCKET_COUNT;

    while (true) {
      MeasurementAggregator maybe = buckets[index];

      //step2) if a value is already there, move to next one and update it 
      if (maybe != null) {
        if (Arrays.equals(key, 0, keyLength, maybe.stationNameBytes, 0, maybe.stationNameBytes.length)) {
          return maybe;
        }
        index++;
        index &= BUCKET_COUNT;
      } else {
        //step3) if nothing is found, put it in there 
        final var copiedKey = Arrays.copyOf(key, keyLength);
        MeasurementAggregator measurementAggregator = new MeasurementAggregator(copiedKey, hashCode);
        buckets[index] = measurementAggregator;
        return measurementAggregator;
      }
    }
  }
}
```

이게 단순한데 왜 빠르냐면, 

저 step2에서 충돌났을 때, 원래같았으면 string comparison했을텐데 이게 병목.
그러니까 원래 자리에 누가 이미 있으면 simply move aside 



### 5-2. hashmap 안에 객체 안쓰고 byte쓰는데, cpu cache size만큼 씀 
- 모든 데이터를 거대한 byte 배열이나 네이티브 메모리에 C언어의 struct처럼 연속적으로 배치합니다.
- goal: 단순 객체 회피를 넘어, 데이터의 물리적 인접성을 제어하여 캐시 효율을 극대화.
- how 
    1. min, max, sum, count와 station 이름을 저장할 Stats 객체를 만들지 않습니다.
    2. 대신, (min, max, sum, count, name...) (min, max, sum, count, name...) ... 순서로 데이터가 연속적으로 이어지는 거대한 메모리 블록(네이티브 메모리 또는 byte[])을 만듭니다.
    3. 한 station의 모든 데이터는 CPU 캐시 라인 크기(보통 64바이트) 안에 들어오도록 정렬됩니다.

```java
// 각 station의 통계 데이터는 아래 오프셋을 이용해 접근
final int ENTRY_MIN_OFFSET = 8;
final int ENTRY_MAX_OFFSET = 12;
final int ENTRY_SUM_OFFSET = 0;
final int ENTRY_COUNT_OFFSET = 16;
final int ENTRY_NAME_OFFSET = 24;

// station 하나의 데이터가 차지하는 총 바이트 수 (예: 128바이트)
final int SIZEOF_ENTRY = 128; 

// 모든 데이터를 저장할 거대한 네이티브 메모리
long allStatsMemoryAddress = UNSAFE.allocateMemory(STATION_COUNT * SIZEOF_ENTRY);

// --- 메인 루프 내부 ---
int index = phf.index(key); // PHF로 고유 인덱스 획득

// 해당 station 데이터가 저장된 메모리 블록의 시작 주소 계산
long stationBaseAddress = allStatsMemoryAddress + index * SIZEOF_ENTRY;

// 객체 필드 접근 대신, 메모리 주소에 직접 값을 읽고 씀
long currentSum = UNSAFE.getLong(stationBaseAddress + ENTRY_SUM_OFFSET);
UNSAFE.putLong(stationBaseAddress + ENTRY_SUM_OFFSET, currentSum + temperature);
```
1. 핵심: HashMap에서 Stats 객체 참조를 가져오면, 그 객체는 메모리의 다른 곳에 흩어져 있을 수 있습니다(캐시 미스 유발). 
2. 이 방식은 한 station의 모든 데이터(min, max, sum, count)가 물리적으로 바로 옆에 붙어있도록 보장합니다. 
3. CPU가 sum을 읽기 위해 메모리를 가져올 때, 옆에 있는 min, max도 함께 CPU 캐시에 올라오게 되어 다음 접근이 매우 빨라집니다.

### 5-3. perfect custom hashmap 

출돌났을 때, while문으로 옆자리 +1 하는 것 까지 최적화 한 방법
왜냐면 while문도 branch(분기)문 이거든... 

- 방법)
    1. 먼저 파일의 앞부분을 스캔하여 모든 도시 이름(Station)의 목록을 미리 알아냅니다. (어차피 413개로 고정되어 있음)
    2. 이 413개의 이름 목록만을 입력으로 하는 완벽 해시 함수를 미리 생성합니다. 이 함수는 413개의 입력에 대해 절대 충돌하지 않고 항상 0~412 사이의 고유한 인덱스를 반환하도록 설계됩니다. (PtrHash 같은 라이브러리 사용)
    3. 이제 해시맵(HashMap)이 필요 없어지고, Stats[] 형태의 단순 배열만 있으면 됩니다. 해시 충돌을 확인할 필요 없이, PHF가 알려준 인덱스로 배열에 바로 접근하면 됩니다.

이 데이터셋에 statuion이 413개의 유니크한 도시가 나온다라는걸 알기 때문에 가능한 커스텀 자료구조. 

그러니 항상 O(1)남. 무조건. 


### 5-4. 도시이름이 413개인 dataset이 아니라, 10k 다른 도시이름의 dataset인 경우의 custom hashmap 
- hash collision났을 때, 자리+1 하는 로직으로 수정 
- 그리고 도시이름 비교할 때, 문자열 비교를 string으로 비교하는게 아니라, long으로 바꾸고 bitmap && 비교함.
- 왜냐면 대부분의 도시 이름은 16byte 보다 짧거든. 
- UNSAFE.getLong을 이용해 8바이트 덩어리로 한 번에 비교합니다. 100글자짜리 이름도 13번 정도의 비교면 끝나므로 훨씬 효율적입니다

```java
// 빠른 경로: 이름이 16바이트 미만일 경우
public long find(long word1, long word2, long hash) {
    long address = pointer + offset(hash);
    long w1 = word(address + 24); // 저장된 이름의 첫 8바이트
    long w2 = word(address + 32); // 저장된 이름의 다음 8바이트

    // 1. 대부분의 이름은 16바이트 안에 끝나므로, long 두 개를 직접 비교
    // 2. 이 비교가 성공하면 full byte-by-byte 비교를 건너뛸 수 있음
    return (word1 == w1) && (word2 == w2) ? address : 0;
}

// 느린 경로: 이름이 길거나 빠른 경로에서 실패했을 경우
private static boolean equal(long leftAddress, long rightAddress, long length) {
    // 8바이트(long) 단위로 덩어리로 비교하다가, 남은 부분만 처리
    do {
        long left = UNSAFE.getLong(leftAddress);
        long right = UNSAFE.getLong(rightAddress);
        if (left != right) {
            return false;
        }
        leftAddress += 8;
        rightAddress += 8;
        length -= 8;
    } while (length > 0);
    return true;
}
```




## step6. GraalVM's JIT and AOT compiler 
써서 빨라졌다는데 왜진 모르겠네 
안에 어떻게 오라클이 최적화 했는지는 

일반적으로는 c2 compiler라는걸 쓰는데, 그랄 컴파일러 쓰면 이 1brc 예제 한정 더 빠르다네


## step7. further tricks 

### 7-1. don't use garbage collector 

```bash
NATIVE_IMAGE_OPTS="$NATIVE_IMAGE_OPT: --gc=epsilon -H:-GenLoopSafepoints"
```
저 `epsilon` garbage collector는 

...DOES NOTHING!

모든 제약사항 알고, 파일 메모리에 올린게 13gib 밖에 안하고 뭐 custom hashmap에 올린거가 oom 안뜰걸 아니까, 

garbage collecting도 안하게 함. 

just let it run. 


### 7-2. spawn tricks
- unmap 비용 숨기기: 유령 프로세스 트릭 
- mmap으로 메모리에 매핑된 파일은 프로그램이 종료될 때 OS가 munmap 시스템 콜을 통해 정리(매핑 해제)해야 합니다. 10GB가 넘는 거대한 파일을 정리하는 이 과정은 생각보다 오래 걸리며(수십~수백 밀리초), 1BRC 챌린지에서는 이 정리 시간까지 전체 실행 시간에 포함됩니다. 즉, 계산을 아무리 빨리 끝내도 마지막에 벌어지는 '뒷정리' 때문에 기록이 나빠지는 것이죠.
- 목표: 실제 계산과 결과 출력은 부모 프로세스가 하고, 시간이 오래 걸리는 munmap 뒷정리는 자식 프로세스에게 떠넘긴다.

```java
// main 메소드의 시작 부분
public static void main(String[] args) throws Exception {
    // 1. 프로그램이 처음 실행되면, --worker 인자가 없는 상태입니다.
    if (args.length == 0 || !("--worker".equals(args[0]))) {
        // 2. 자기 자신을 '--worker' 인자와 함께 자식 프로세스로 다시 실행시킵니다.
        spawnWorker(); 
        // 3. 부모 프로세스는 자식이 일을 끝내고 결과를 출력할 때까지 기다린 후,
        //    그 결과를 받아서 그대로 출력하고 즉시 종료합니다.
        return; 
    }

    // --- 이 아래 코드는 '--worker' 인자를 가진 자식 프로세스만 실행합니다 ---
    // 4. 자식 프로세스는 mmap으로 파일을 열고 모든 계산을 수행합니다.
    final FileChannel fileChannel = FileChannel.open(Path.of(FILE), ...);
    final long mapAddress = fileChannel.map(...);

    // ... (모든 1BRC 계산 로직 수행) ...

    // 5. 계산이 끝나면, 자식 프로세스는 결과를 표준 출력으로 인쇄합니다.
    //    (이 출력은 부모 프로세스로 리다이렉션됩니다.)
    System.out.print("{" + ... + "}"); 
    
    // 6. 자식 프로세스는 여기서 종료됩니다. 이제 OS는 이 자식 프로세스가 사용한
    //    mmap을 정리(unmap)하기 시작합니다. 하지만 이미 부모는 결과를 받고 종료했죠.
}

private static void spawnWorker() throws IOException {
    ProcessBuilder pb = new ProcessBuilder();
    // 자기 자신(java)을 --worker 인자와 함께 실행할 명령을 준비
    pb.command("java", ... , "--worker"); 
    // 자식 프로세스의 출력을 부모가 읽을 수 있도록 파이프로 연결
    pb.redirectOutput(ProcessBuilder.Redirect.PIPE);
    Process worker = pb.start();
    
    // 자식 프로세스의 출력(결과)을 그대로 현재 콘솔로 복사
    worker.getInputStream().transferTo(System.out);
}
```

1. 부모 프로세스는 실행되자마자 아무 계산도 하지 않고, 자신과 똑같은 자식 프로세스를 하나 만듭니다.
2. 자식 프로세스가 mmap과 모든 무거운 계산을 수행합니다.
3. 계산이 끝난 자식은 결과를 System.out.print로 출력합니다.
4. 부모는 파이프를 통해 그 결과를 받아서 그대로 화면에 보여주고 즉시 System.exit(0)으로 종료합니다.
5. 벤치마크 시간 측정은 부모 프로세스가 종료된 시점에 끝납니다.
6. 그 후, 버려진 자식 프로세스가 OS에 의해 정리되면서 느린 unmap이 일어나지만, 이는 측정 시간에 포함되지 않습니다.




### 7-3. Unsafe 
- mmap같은거나 byte buffer 읽을 때, 계속 safety check 함. 
- b/c you don't want to read/write outside byte buffer, mmap, etc 
- 주로 custom hashmap 과 같이 사용되었음 
- Stats 같은 집계 객체를 전혀 만들지 않고, Unsafe를 이용해 거대한 byte[] 배열을 마치 C언어의 struct 배열처럼 사용합니다
- 목표: 객체 생성 및 GC(Garbage Collection) 오버헤드를 '0'으로 만들기.

```java
// C언어의 struct 정의와 유사하게, 메모리 오프셋을 상수로 정의
private static final int ENTRY_SUM_OFFSET = 8;
private static final int ENTRY_MIN_OFFSET = 16;
private static final int ENTRY_MAX_OFFSET = 20;
private static final int ENTRY_COUNT_OFFSET = 24;
private static final int ENTRY_NAME_OFFSET = 28;

public static void updateEntry(final byte[] entry, final int temp) {
    // entry는 station 정보가 담긴 거대한 byte[] 배열
    long sum = UNSAFE.getLong(entry, ENTRY_SUM_OFFSET);
    int min = UNSAFE.getInt(entry, ENTRY_MIN_OFFSET);
    
    // 객체의 필드에 접근하는 대신, 메모리 주소(오프셋)에 직접 값을 읽고 씀
    UNSAFE.putLong(entry, ENTRY_SUM_OFFSET, sum + temp);
    if (temp < min) {
        UNSAFE.putInt(entry, ENTRY_MIN_OFFSET, temp);
    }
    // ...
}
```

- new MeasurementAggregator()를 호출하는 대신, 그냥 거대한 byte 배열 하나를 만들어 둡니다.
- 각 Station의 통계 정보(sum, min, max, count, 이름)는 이 byte 배열의 특정 위치에 저장됩니다
- UNSAFE.getLong/putInt를 사용하면 JVM의 모든 안전 검사를 건너뛰고 해당 메모리 주소에 값을 직접 읽고 쓸 수 있어, 객체를 생성하고 필드에 접근하는 것보다 훨씬 빠릅니다.


### 7-4. super scalar execution 
```java
while (scanner1.hasNext()) {
  long word = scanner1.getLong();
  long pos = findDelimiter(word);
  long wordB = scanner1.getLongAt(scanner1.pos() + 8);
  long posB = findDelimiter(wordB);
  record(findResult(word, pos, wordB, posB, scanner1, results, collectedResults));
}

while (scanner2.hasNext()) {
  long word = scanner2.getLong();
  long pos = findDelimiter(word);
  long wordB = scanner2.getLongAt(scanner2.pos() + 8);
  long posB = findDelimiter(wordB);
  record(findResult(word, pos, wordB, posB, scanner2, results, collectedResults));
}

while (scanner3.hasNext()) {
  long word = scanner3.getLong();
  long pos = findDelimiter(word);
  long wordB = scanner3.getLongAt(scanner3.pos() + 8);
  long posB = findDelimiter(wordB);
  record(findResult(word, pos, wordB, posB, scanner3, results, collectedResults));
}
```
same code 3 times? 
why?

현대의 CPU는 슈퍼스칼라(Superscalar) 아키텍처를 가집니다. 
즉, 내부에 여러 개의 실행 유닛(정수 연산 장치, 부동소수점 연산 장치 등)이 있어서 서로 의존성이 없는 명령어 여러 개를 한 사이클에 동시에 처리할 수 있습니다.

하지만 `for (int i=0; i<N; i++) { process(i); }` 같은 일반적인 루프는, process(i)가 끝나야 process(i+1)을 시작할 수 있는 데이터 의존성을 가질 수 있어 CPU의 슈퍼스칼라 능력을 100% 활용하기 어렵습니다.

해결책: "다음에 할 일들을 미리 알려주기"

`루프를 수동으로 풀어서(unroll) 한 번의 반복에 여러 작업을 명시적으로 나열하면, 컴파일러와 CPU에게 "이 작업들은 서로 독립적이니까 동시에 처리해도 돼!"라는 강력한 힌트를 줄 수 있습니다.`

목표: CPU의 여러 실행 유닛을 동시에 가동시켜 명령어 수준의 병렬성을 극대화.


### 7-5. '플라이웨이트(Flyweight)' 패턴과 메모리 풀링
목표: 자주 사용될 데이터 구조를 미리 만들어두어, 런타임 중의 메모리 할당을 최소화하고 CPU 캐시 지역성(locality)을 높이기. 

```java
private static byte[][] processMemoryArea(...) {
    // 해시 테이블 자체 (슬롯은 비어있음)
    final byte[][] table = new byte[TABLE_SIZE][];
    
    // 1. 처음 발견되는 512개(PREMADE_ENTRIES)의 station을 위해
    //    미리 byte[] 배열 512개를 만들어 둠 (메모리 풀)
    final byte[][] preConstructedEntries = new byte[PREMADE_ENTRIES][...];

    int entryCount = 0;
    
    while (reader.hasNext()) {
        // ... 파싱 ...
        int index = ...; // 해시값으로 인덱스 계산
        while (true) {
            entry = table[index];
            if (entry == null) {
                // 2. 새로운 station을 처음 발견했을 때
                //    entryCount가 512보다 작으면, new byte[] 대신 미리 만들어 둔 배열을 사용
                byte[] entryBytes = (entryCount < PREMADE_ENTRIES) 
                                    ? preConstructedEntries[entryCount++]
                                    : new byte[...]; // 512개가 넘어가면 그때 새로 할당
                table[index] = fillEntry(...);
                break;
            }
            // ...
        }
    }
    return table;
}
```

- preConstructedEntries: 이 코드는 512개의 byte[] 객체를 미리 힙에 할당해 둡니다. 이 객체들은 메모리 상에 서로 가깝게 위치할 확률이 높습니다.
- entryCount < PREMADE_ENTRIES: 루프를 돌다가 새로운 station을 발견하면, new byte[]를 호출하는 대신 이 미리 만들어 둔 배열 풀에서 하나를 가져다 씁니다.
- 효과:
    1. 메모리 할당 오버헤드 감소: 핫 루프(hot loop) 안에서 new 키워드 호출을 줄입니다.
    2. 캐시 효율 증가: 미리 할당된 객체들은 메모리에 인접해 있을 가능성이 높아, 하나의 객체에 접근할 때 다른 객체들도 함께 CPU 캐시에 로드될 수 있습니다.


### 7-6. 함수 호출 횟수도 줄이기 
calculateHash() 같은 함수를 호출하는 대신, 해시를 계산하는 코드를 메인 루프 안에 직접 펼쳐놓음 

- 왜 하는가? (함수 호출의 숨은 비용):
    1. 스택 프레임 생성: 함수를 호출할 때마다 스택에 파라미터, 복귀 주소 등을 저장하기 위한 공간(스택 프레임)을 만들어야 합니다.
    2. 점프: 프로그램의 실행 흐름이 현재 위치에서 함수가 있는 다른 메모리 주소로 '점프'했다가 다시 돌아와야 합니다.
    3. JIT 컴파일러 최적화 방해: 컴파일러는 코드가 여러 함수로 쪼개져 있으면 전체적인 맥락을 파악하기 어려워 최적화에 제약을 받습니다.

10억 개의 라인을 처리하는 루프 안에서 이 작은 비용들이 10억 번 쌓이면 무시 못 할 시간이 됩니다.


before) calling `calculateHash()`
```java
// 메인 루프 내부
while (reader.hasNext()) {
    // ... 라인 파싱 ...
    long hash = calculateHash(stationNameBytes); // 매번 함수 호출!
    updateTable(hash, temperature);
}

private long calculateHash(byte[] name) {
    // 해시 계산 로직
    return ...;
}
```

after) inlining
```java
// 메인 루프 내부
while (reader.hasNext()) {
    // ... 라인 파싱 ...
    
    // --- 해시 계산 로직이 직접 여기에 펼쳐짐 ---
    long hash = 0;
    // (예: Roy van Rijn의 SWAR 비트 연산 로직)
    long word1 = UNSAFE.getLong(nameAddress);
    hash ^= word1; 
    // ...
    // --- 함수 호출 없이 계산 끝 ---
    
    updateTable(hash, temperature);
}
```

### 7-7. 맨 마지막에 여러 쓰레드들의 결과를 취합하는곳 최적화 
1. 지연된 문자열 생성 (Delayed String Creation)
    - 문제점: new String(bytes)는 생각보다 비싼 작업입니다. 내부적으로 바이트를 UTF-8로 디코딩하고, 새로운 객체를 힙 메모리에 할당해야 합니다.
    - 최적화: 
        1. 각 스레드는 결과를 취합할 때 String 키를 사용하지 않고, 원본 byte[]나 메모리 주소(long)를 키로 사용하는 커스텀 해시맵에 결과를 저장합니다. 
        2. String 객체는 모든 스레드의 작업이 끝나고, 최종 결과를 정렬하여 출력하기 직전, 딱 한 번만 생성합니다. 
        3. 즉, 10억 번이 아닌 413번만 new String()을 호출하는 것이죠.
2. 커스텀 병합 로직 (Custom Merge Logic)
    - 문제점: ConcurrentHashMap.merge()에 람다(lambda) 함수를 사용하면 편리하지만, 람다 객체 생성 및 메소드 호출에 미세한 오버헤드가 있습니다.
    - 최적화: Roy van Rijn님의 코드는 ConcurrentHashMap.merge를 사용하긴 하지만, 병합 로직으로 람다 대신 **정적 메소드 참조(CalculateAverage_royvanrijn::mergeEntry)** 를 사용합니다.
    - mergeEntry 메소드는 Unsafe를 사용하여 byte[] 배열 두 개를 직접 비교하고 값을 합칩니다 
```java
public static byte[] mergeEntry(final byte[] entry, final byte[] merge) {
    long sum = UNSAFE.getLong(merge, ENTRY_SUM);
    // ...
    sum += UNSAFE.getLong(entry, ENTRY_SUM);
    // ...
    UNSAFE.putLong(entry, ENTRY_SUM, sum);
    return entry;
}
```

### 7-8. memory alignment(padding 작업)
```java
// 7바이트를 더해서 다음 데이터가 8바이트 경계에 오도록 함
private static final int ENTRY_BASESIZE_WHITESPACE = ENTRY_NAME + 7;
```
ENTRY_NAME 뒤에 7바이트를 더하는 것은 단순히 공간을 띄우는 게 아닙니다. Station 이름의 최대 길이가 100바이트이고, 통계 데이터(sum, min, max, count 등)의 크기를 고려했을 때, 각 entry의 전체 크기가 **8의 배수(64비트 경계)** 에 맞춰지도록 하는 패딩(padding) 작업입니다.

왜 하는가? CPU는 메모리에서 데이터를 읽을 때 8바이트, 16바이트 등 특정 단위로 읽는 것이 가장 효율적입니다. 데이터 구조가 이 경계에 딱 맞춰져 있으면, 한 번의 메모리 접근으로 필요한 데이터를 모두 가져올 수 있어 성능이 향상됩니다. 이를 **메모리 정렬(Memory Alignment)** 이라고 합니다.

### 7-9. 쓰레드 갯수 최적화  
```java
// PROCESSORS - 1 개의 스레드만 생성
final Thread[] parallelThreads = new Thread[PROCESSORS - 1];

// ... 루프가 끝난 후 ...

// 메인 스레드도 직접 일함
final byte[][] table = processMemoryArea(lastAddress, mapAddress + fileSize, false);
```
1. 코어 개수만큼 스레드를 만드는 게 아니라, 
2. 코어 개수 - 1 만큼만 만들고, 
3. 마지막 작업 덩어리(chunk)는 main 스레드가 직접 처리합니다. 

이렇게 하면 main 스레드가 다른 스레드들이 끝나기를 그냥 기다리며 노는 일 없이, 시스템의 모든 코어가 100% 일하게 됩니다.


## step8. 좀 더 안정적인 방법 
step7까지는 안정성 개나주고 무조건 빠르게만 집중했는데,
industrial level에서 안정성도 어느정도 챙기면서 최적화 하는 기법은?

answer by [shipilev](https://github.com/gunnarmorling/1brc/blob/main/src/main/java/dev/morling/onebrc/CalculateAverage_shipilev.java)

### 8-1. Fork-Join & Work-Stealing 
- 보통은 new Thread()로 직접 스레드를 만들고 join()으로 기다리는 단순한 방식을 쓴 반면,
- 목표: 특정 스레드가 자신의 일을 먼저 끝냈을 때 놀지 않고, 아직 일이 남은 다른 스레드의 작업을 '훔쳐와서' 처리하게 하여 전체 시스템의 처리량을 극대화.

```java
public static final class ParsingTask extends CountedCompleter<Void> {
    // ...
    @Override
    public void compute() {
        // ...
        if (len > UNIT_SLICE_SIZE) {
            // 아직 처리할 덩어리(chunk)가 너무 크면,
            // 1. 작업을 반으로 쪼갬
            int mid = ...;

            // 2. FJP API: 처리해야 할 하위 작업이 하나 있다고 등록
            setPendingCount(1);
            
            // 3. 쪼갠 작업 중 하나를 다른 스레드가 가져가서 처리하도록 던져줌 (fork)
            new ParsingTask(this, buf.slice(0, mid)).fork();

            // 4. 나머지 절반은 현재 스레드가 바로 이어서 처리 (재귀 호출과 유사)
            new ParsingTask(this, buf.slice(mid, len - mid)).compute();
        } else {
            // 5. 덩어리가 충분히 작아지면, 직접 파싱을 시작
            seqCompute(map, buf, len);
            
            // 6. FJP API: 이 작업이 끝났음을 알림
            tryComplete();
        }
    }
}
```

- Work-Stealing: fork()를 통해 작업을 던져두면, 포크-조인 풀의 '놀고 있는' 스레드가 그 작업을 가져가서(steal) 처리합니다. 어떤 스레드는 쉬운(짧은) 데이터 덩어리를 맡고, 어떤 스레드는 어려운(긴) 덩어리를 맡아도, 일이 먼저 끝난 스레드가 다른 스레드를 도와주게 됩니다. 이는 각 스레드가 자기 할당량만 처리하고 끝내는 방식보다 훨씬 유연하고 효율적입니다.
- CountedCompleter: 모든 하위 작업이 끝났을 때를 감지하는 영리한 동기화 도구입니다. 모든 작업 조각들이 완료되어야만 최종적으로 결과 취합 단계로 넘어갈 수 있도록 보장합니다.

### 8-2. 점진적 언매핑(Unmapping)을 통한 메모리 관리
- spawnWorker 트릭이 unmap 비용을 '숨기는' 것이라면, 이 코드는 작업이 끝난 메모리 구역을 실시간으로 정리하여 시스템 부하를 줄이는, 더 안정적인 방법을 사용합니다.
- 목표: 10GB 파일 전체에 대한 unmap이 프로그램 종료 시 한 번에 몰리는 것을 방지.

```java
// 작업이 끝난 버퍼를 담아두는 큐
private static final LinkedBlockingQueue<ByteBuffer> RELEASABLE_BUFFERS = new LinkedBlockingQueue<>();

// ParsingTask 클래스 내부
@Override
public void onCompletion(CountedCompleter<?> caller) {
    if (DIRECT_UNMMAPS && (mappedBuf != null)) {
        // 1. 내 작업이 끝나면, 내가 사용한 메모리 맵 버퍼를 '반납 가능한' 큐에 넣음
        RELEASABLE_BUFFERS.offer(mappedBuf);
    }
}

// main 메소드 내부
public static void main(String[] args) throws Exception {
    // ... 포크-조인 풀에 작업 제출 ...
    
    // 2. 메인 스레드는 파싱 작업이 진행되는 동안 놀지 않고,
    //    '반납 가능한' 큐를 감시하며 정리 작업을 수행함
    ByteBuffer buf;
    while ((buf = RELEASABLE_BUFFERS.take()) != RELEASABLE_BUFFER_POISON_PILL) {
        DirectUnmaps.invokeCleaner(buf); // 리플렉션으로 버퍼를 직접 unmap
    }
    
    // ... 모든 작업이 끝나면 결과 취합 ...
}
```

- spawnWorker 트릭: 손님들이 식사를 다 끝내고 나갈 때까지 기다렸다가, 가게 문을 닫은 뒤 밤새도록 한 번에 모든 접시를 치우는 방식. (뒷정리가 한 번에 몰림)
- 이 코드의 방식: 손님이 식사를 마친 테이블이 생길 때마다, 직원이 실시간으로 돌아다니며 빈 접시를 계속 치우는 방식. 일이 한 번에 몰리지 않아 부하가 분산됩니다.
- DirectUnmaps: MappedByteBuffer는 원래 GC가 처리해야만 unmap이 되는데, 이는 언제 실행될지 알 수 없습니다. DirectUnmaps 클래스는 **리플렉션(reflection)** 이라는 기술을 사용해 JVM 내부의 비공개 cleaner() 메소드를 강제로 호출하여, 원하는 시점에 즉시 unmap을 실행하는 '해킹'에 가까운 고급 기법입니다.

### 8-3. Unsafe 안쓰고 표준 자바 API를 최대한 쓰기 
- int 단위 읽기: slice.getInt(idx)를 사용해 한 번에 4바이트씩 읽습니다. long(8바이트)만큼 빠르진 않지만, 바이트 단위로 읽는 것보다 훨씬 효율적이며, SIMD나 SWAR처럼 복잡하고 이해하기 어려운 코드 작성을 피합니다.
- 객체 헤더를 활용한 빠른 이름 비교: Bucket 클래스를 보면, station 이름의 앞 4바이트와 그다음 4바이트를 prefix1, prefix2라는 int 필드에 저장합니다.

```java
if (prefix1 != cand.getInt(begin)) return false;
if (prefix2 != cand.getInt(begin + 4)) return false;
```

대부분의 이름은 앞 8바이트에서 차이가 나므로, 이 간단한 정수 비교만으로 비싼 전체 바이트 배열 비교를 대부분 피할 수 있습니다. 
이는 SWAR 기법의 아이디어를 안전하고 이해하기 쉬운 방식으로 구현한 것입니다.

- ThreadLocal: 각 스레드가 자신만의 결과 맵(MeasurementsMap)을 갖도록 ThreadLocal을 사용합니다. 이는 스레드 간의 데이터 경합을 원천적으로 차단하여 락(lock) 없이 병렬 작업을 가능하게 하는 표준적이고 안정적인 방법입니다.






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


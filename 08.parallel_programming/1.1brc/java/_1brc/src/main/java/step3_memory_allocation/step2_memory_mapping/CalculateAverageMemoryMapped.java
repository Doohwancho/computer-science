package step3_memory_allocation.step2_memory_mapping;

import java.io.IOException;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.StandardCharsets;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;
import java.util.Map;
import java.util.TreeMap;
import java.util.stream.Collectors;

/**
 * baseline + Memory Mapping optimization only
 *
 * ---
 * Q. 왜 memory mapping이 일반적인 file i/o 보다 빠름?
 *
 * 두가지 이유
 * 1. 일반적인 file i/o는 disk -> kernel buffer -> jvm heap -> read = 즉, 데이터 복사가 2번 일어남
 * 2. memory mapping은 virtual memory에 page mapping table에 disk에 있던 파일 주소 매핑해놓는 거라, 10억라인 한줄한줄 syscall-read() 해서 context switching cost 비용 지불 안해도 됨.
 *
 *
 * ---
 * 1. 데이터 복사 2번 안해도 됨
 * 이게 일반적인 i/o 과정
 * 1. 니가 `read()` 같은 시스템 콜 호출할 때마다
 * 2. 커널이 디스크에서 데이터 읽어서
 * 3. 커널 버퍼(페이지 캐시)에 복사하고,
 * 4. 다시 그걸 JVM 힙으로 복사하는 식으로 2번 복사가 일어남.
 * 5. jvm heap에서 데이터 가져와서 씀
 *
 * 데이터 복사 2번함.
 * 비효율
 *
 * 반면 memory mapping은 데이터 불러오는 과정에서 복사 한번만 해서 효율적임.
 * 1. 파일이 가상 메모리에 직접 매핑됨
 * 2. 페이지 폴트가 발생할 때만 디스크에서 물리 메모리로 데이터가 로드됨 (1번만 데이터 복사)
 *
 * Java에서 메모리 매핑 파일은 MappedByteBuffer를 통해 접근하며, 이는 JVM의 다이렉트 메모리(Direct Memory) 영역에 할당됨.
 * 다이렉트 메모리는 힙(Heap) 외부에 있으며, 가비지 컬렉션의 영향을 받지 않음.
 *
 *
 * ---
 * 2. 매 라인마다 syscall - read()해서 context switching 비용 발생을 줄여줌
 *
 * baseline 코드 보면, 파일 읽는 코드가 이거임.
 * `Map<String, ResultRow> measurements = new TreeMap<>(Files.lines(Paths.get(FILE))`
 * Files.lines()가 매 라인마다 syscall - read()를 보냄.
 * 10억 rows .txt 파일이니까 10억번 read() syscall 보내는건데,
 * syscall시 user mode -> kernel mode 모드 바꿀 떄 마다 context switching 발생함.
 *
 * ---
 * Q. context switching 비용이 비싼 이유?
 *
 * 1. **보안 체크**: User mode에서 kernel mode로 갈 때마다 CPU가 "야 얘 권한 있냐?" 검사함. 이 과정에서 CPU 파이프라인이 싹 비워지고 캐시도 날아감
 * 	1. "얘" == 유저 프로세스
 * 	2. 이 프로세스가 해당 파일 읽을 권한 있는지 (ls하면 나오는 rwxrwxrwx 같은거) 체크. 보안.
 * 2. **레지스터 저장/복원**: Context switching 할 때마다 현재 프로세스 레지스터 상태 다 저장하고, 커널 모드 레지스터 상태 로드해야 됨. 개귀찮음 ㅇㅇ
 * 	1. 프로세스 안 여러 쓰레드가 각각 해당 스레드의 [[Register]] 상태, [[program counter]]를 저장하고, [[Context Switching]] 할 때마다 [[Register]] & [[program counter]] 값 업데이트 해줘야 함
 * 3. **TLB 플러시**: 가상 주소를 물리 주소로 바꿔주는 TLB 캐시가 모드 바뀔 때마다 싹 비워짐. 메모리 접근 속도 개느려짐 ㅋㅋ
 * 	- TLB 초기화하는 이유는 **주소 공간이 바뀌니까** ㅋㅋㅋ. 유저 모드랑 커널 모드는 주소 공간이 완전 다름. TLB는 가상 주소 -> 물리 주소 변환 캐시인데, 모드 바뀌면 그 매핑도 다 바뀜. 그래서 TLB 플러시해야됨. 안그러면 잘못된 주소 접근해서 시스템 개박살남
 * 	1. User mode랑 kernel mode는 완전 다른 주소 공간을 씀 ㅇㅇ
 * 	    - User mode: 프로그램이 쓰는 가상 주소 공간
 * 	    - Kernel mode: OS 커널이 쓰는 물리 주소 공간
 * 	2. 모드 전환할 때 주소 변환 방식이 바뀜
 * 	    - User mode: 가상 주소 -> 물리 주소 변환 필요
 * 	    - Kernel mode: 직접 물리 주소 접근 가능
 * 	3. TLB가 캐싱하는 정보도 달라짐
 * 	    - User mode TLB: 가상 주소 -> 물리 주소 매핑
 * 	    - Kernel mode TLB: 커널 전용 주소 매핑
 * 4. **CPU 캐시 오염**: 커널 코드 실행하면 CPU 캐시가 사용자 모드 데이터랑 다른 걸로 채워짐. 사용자 모드로 돌아가면 캐시 미스 개많이 남
 * 	1. 유저 모드에서 캐시에 A, B, C 데이터 있었는데 커널 모드 가면 X, Y, Z 데이터로 캐시 채워짐. 다시 유저 모드로 돌아오면 A, B, C 데이터 다시 로드해야 되니까 캐시 미스 개많이 남
 * 5. **인터럽트 처리**: 시스템 콜은 소프트웨어 인터럽트로 처리됨. 인터럽트 벡터 테이블 찾고 핸들러로 제어 넘기는 과정 개느림
 * 	1. 유저 프로그램이 시스템 콜 호출하면 소프트웨어 인터럽트 발생시킴 (int 0x80 같은거)
 * 	2. CPU가 현재 작업 중단하고 인터럽트 벡터 테이블 참조함
 * 	3. 해당 인터럽트 번호에 맞는 핸들러 찾아서 점프함
 * 	4. 커널 모드로 전환되고 핸들러 실행
 * 	5. 핸들러가 시스템 콜 처리하고 결과 반환
 * 	6. 다시 유저 모드로 전환
 *
 *
 * ---
 * Q. memory mapping은 context switching 비용이 왜 적지?
 *
 * 맨 처음 파일 open()할 떄 1회 시스템 콜 있고, 그 다음부터는 system call이 없다고 함.
 * 전체 프로세스는 이럼.
 *
 *
 1. 초기 설정 단계 (최초 1회 시스템 콜)
     1. **파일 열기**:`open()`시스템 콜로 파일을 엽니다.
     2. **메모리 매핑 요청**:`mmap()`시스템 콜을 통해 파일을 가상 메모리에 매핑합니다.
     3. `void* addr = mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0);`
     4. **페이지 테이블 설정**: 커널이 파일의 페이지들을 프로세스의 가상 주소 공간에 매핑합니다.
         1. 이때 실제 데이터는 아직 메모리에 로드되지 않습니다.
         2. 단지 매핑 정보만 페이지 테이블에 설정됩니다.
 2. 데이터 접근 시 발생하는 일 (시스템 콜 없음)
     1. **첫 접근 시 페이지 폴트**: 프로그램이 매핑된 메모리 영역에 처음 접근하면 페이지 폴트가 발생합니다.
         -`char firstChar = mappedBuffer.get(0); // 첫 접근 시 페이지 폴트 발생`
     2. **페이지 폴트 핸들러 동작**:
         - 페이지 폴트는 커널 모드로 전환을 유발하지만, 이는 시스템 콜이 아닌 하드웨어 예외 메커니즘입니다.
         - 커널의 페이지 폴트 핸들러가 디스크에서 해당 페이지를 물리 메모리로 로드합니다.
         - 페이지 테이블이 업데이트되어 가상 주소와 물리 주소 간의 매핑이 설정됩니다.
     3. **이후 접근 시 직접 메모리 접근**:
         - 동일한 페이지에 다시 접근할 때는 페이지 폴트가 발생하지 않습니다.
         - MMU(Memory Management Unit)가 가상 주소를 물리 주소로 자동 변환합니다.
         - 프로그램은 일반 메모리 접근 명령어로 데이터를 읽을 수 있습니다.
         - `char nextChar = mappedBuffer.get(1); // 직접 메모리 접근, 시스템 콜 없음`
 *
 *
 * ---
 * 문제점
 *
 * memory allocation 하려면 최대 파일 크기가 2GB인데(Integer.MAX_VALUE = 2,147,483,647 바이트),
 * measurements.txt의 크기가 13GB라 에러남.
 * 따라서 chunking과 같이 써야함.
 * 이 코드는 정상작동 안함. 정상작동하려면 2GB 파일 사이즈 안쪽이어야 함.
 *
 */
public class CalculateAverageMemoryMapped {
    
    private static final String FILE = "measurements.txt";
    
    private static record Measurement(double min, double max, double sum, long count) {
        
        Measurement(double initialMeasurement) {
            this(initialMeasurement, initialMeasurement, initialMeasurement, 1);
        }
        
        public static Measurement combineWith(Measurement m1, Measurement m2) {
            return new Measurement(
                Math.min(m1.min, m2.min),
                Math.max(m1.max, m2.max),
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
        Path path = Path.of(FILE);
        long fileSize = path.toFile().length();
        
        Map<String, Measurement> resultMap = new TreeMap<>();
        
        // Open file channel for memory mapping
        try (FileChannel channel = FileChannel.open(path, StandardOpenOption.READ)) {
            // Map the entire file into memory
            // Note: For files larger than 2GB, you would need to map multiple regions
            MappedByteBuffer buffer = channel.map(FileChannel.MapMode.READ_ONLY, 0, fileSize);
            
            // Process the file line by line
            int lineStart = 0;
            String currentLine;
            
            while (buffer.hasRemaining()) {
                byte b = buffer.get();
                if (b == '\n' || !buffer.hasRemaining()) {
                    // Found end of line, process it
                    int lineEnd = buffer.position() - (b == '\n' ? 1 : 0);
                    
                    // Extract the line as a string
                    buffer.position(lineStart);
                    byte[] lineBytes = new byte[lineEnd - lineStart];
                    buffer.get(lineBytes);
                    currentLine = new String(lineBytes, StandardCharsets.UTF_8);
                    
                    // Process the line just like in the original code
                    int pivot = currentLine.indexOf(';');
                    String key = currentLine.substring(0, pivot);
                    double measured = Double.parseDouble(currentLine.substring(pivot + 1));
                    
                    // Update measurements
                    Measurement newMeasurement = new Measurement(measured);
                    resultMap.merge(key, newMeasurement, Measurement::combineWith);
                    
                    // Move to next line
                    lineStart = buffer.position();
                }
            }
        }
        
        // Print results
        System.out.print("{");
        System.out.print(
            resultMap.entrySet().stream()
                .map(Object::toString)
                .collect(Collectors.joining(", ")));
        System.out.println("}");
    }
}
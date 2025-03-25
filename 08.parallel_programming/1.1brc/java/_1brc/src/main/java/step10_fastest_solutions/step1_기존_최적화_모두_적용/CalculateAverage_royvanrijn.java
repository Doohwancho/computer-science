package step10_fastest_solutions.step1_기존_최적화_모두_적용;

/*
 *  Copyright 2023 The original authors
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.channels.FileChannel;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;
import java.util.ArrayList;
import java.util.List;
import java.util.TreeMap;
import java.util.stream.Collectors;

/**
 * Changelog:
 *
 * Initial submission:          62000 ms
 * Chunked reader:              16000 ms
 * Optimized parser:            13000 ms
 * Branchless methods:          11000 ms
 * Adding _3_syntax_details.memory mapped files:  6500 ms (based on bjhara's submission)
 * Skipping string creation:    4700 ms
 * Custom hashmap...            4200 ms
 * Added SWAR token checks:     3900 ms
 * Skipped String creation:     3500 ms (idea from kgonia)
 * Improved String skip:        3250 ms
 * Segmenting files:            3150 ms (based on spullara's code)
 * Not using SWAR for EOL:      2850 ms
 *
 * Best performing JVM on MacBook M2 Pro: 21.0.1-graal
 * `sdk use java 21.0.1-graal`
 *
 */
class CalculateAverage_royvanrijn {
    
    private static final String FILE = "measurements.txt";
    
    // mutable state now instead of records, ugh, less instantiation.
    static final class Measurement {
        int min, max, count;
        long sum;
        
        public Measurement() {
            this.min = 10000;
            this.max = -10000;
        }
        
        public Measurement updateWith(int measurement) {
            min = min(min, measurement);
            max = max(max, measurement);
            sum += measurement;
            count++;
            return this;
        }
        
        public Measurement updateWith(Measurement measurement) {
            min = min(min, measurement.min);
            max = max(max, measurement.max);
            sum += measurement.sum;
            count += measurement.count;
            return this;
        }
        
        public String toString() {
            return round(min) + "/" + round((1.0 * sum) / count) + "/" + round(max);
        }
        
        private double round(double value) {
            return Math.round(value) / 10.0;
        }
    }
    
    public static final void main(String[] args) throws Exception {
        new CalculateAverage_royvanrijn().run();
    }
    
    private void run() throws Exception {
        
        //step1. File을 segments 단위로 나누기
        var results = getFileSegments(new File(FILE)).stream().map(segment -> {
                
                long segmentEnd = segment.end();
                try (var fileChannel = (FileChannel) Files.newByteChannel(Path.of(FILE), StandardOpenOption.READ)) {
                    //Q. 이 코드는 기존 memory mapping할 떄 썼던 코드랑 조금 다른데?
                    //A. `FileChannel channel = FileChannel.open(path, StandardOpenOption.READ);` 얘랑 같은 코드라고 봐도 된다고 함.
                    
                    //step7. core 갯수(8개)만큼 나눈 파일을 읽는다.
                    var bb = fileChannel.map(FileChannel.MapMode.READ_ONLY, segment.start(), segmentEnd - segment.start());
                    
                    //step8. 값을 옮길 때 여러번 재사용할 버퍼를 생성한다.
                    var buffer = new byte[64];
                    
                    //step9. little endian으로 읽도록 강제(java는 big endian)
                    bb.order(ByteOrder.LITTLE_ENDIAN);
                    //Q. 왜 little endian으로 강제하지?
                    //A. 대부분의 현대 CPU가 Little Endian임ㅋㅋ 인텔, AMD, ARM 다 리틀엔디안임
                    //   근데 Java는 기본으로 Big Endian을 씀. 그래서 CPU가 바이트 순서 바꾸는데 시간 씀.
                    //   ByteBuffer에서 Little Endian으로 강제하면: CPU가 네이티브 순서 그대로 읽을 수 있어서 빨라짐
                    //   특히 getLong() 같은 다중 바이트 연산에서 차이 더 커짐
                    
                    //step10. create custom hashmap without safety checks + 객체를 String으로 안받고 byte[]로 받음 for performance
                    BitTwiddledMap measurements = new BitTwiddledMap();
                    
                    int startPointer;
                    int limit = bb.limit();
                    while ((startPointer = bb.position()) < limit) {
                        
                        //step11. find pointer of ';' using SWAR technique
                        // SWAR is faster for ';'
                        int separatorPointer = findNextSWAR(bb, SEPARATOR_PATTERN, startPointer + 3, limit);
                        
                        //step16. find pointer of '\n' (case가 3개 뿐이라 간단한 if-else로 처리. ex. 1.0 / 12.3 / -14.2)
                        // Simple is faster for '\n' (just three options)
                        int endPointer;
                        if (bb.get(separatorPointer + 4) == '\n') {
                            endPointer = separatorPointer + 4;
                        }
                        else if (bb.get(separatorPointer + 5) == '\n') {
                            endPointer = separatorPointer + 5;
                        }
                        else {
                            endPointer = separatorPointer + 6;
                        }
                        
                        //step17. buffer를 _3_syntax_details.memory de-allocation 스킵하고 재사용해서 하나의 라인을 읽음
                        // Read the entry in a single get():
                        bb.get(buffer, 0, endPointer - startPointer);
                        bb.position(endPointer + 1); // skip to next line.
                        
                        //step18. Extract the measurement value (10x):
                        final int nameLength = separatorPointer - startPointer;
                        final int valueLength = endPointer - separatorPointer - 1;
                        
                        //step19. branchless parse int로 원래 Double형식의 숫자가 String으로 되어있는 것을 int로 읽음
                        final int measured = branchlessParseInt(buffer, nameLength + 1, valueLength);
                        
                        //step20. custom hashmap에 city와 int값을 String 형태로 넘기는게 아니라 byte로 넘겨서 byte로 저장함 (skip creating String 객체)
                        measurements.getOrCreate(buffer, nameLength).updateWith(measured);
                    }
                    return measurements;
                }
                catch (IOException e) {
                    throw new RuntimeException(e);
                }
            }).parallel().flatMap(v -> v.values.stream()) //step21. 위 과정을 parallel()로 처리하고 collector로 합산
            .collect(Collectors.toMap(e -> new String(e.key), BitTwiddledMap.Entry::measurement, (m1, m2) -> m1.updateWith(m2), TreeMap::new));
        
        //step22. print results
        System.out.println(results);
    }
    
    /**
     * -------- This section contains SWAR code (SIMD Within A Register) which processes a bytebuffer as longs to find values:
     */
    private static final long SEPARATOR_PATTERN = compilePattern((byte) ';');
    
    private int findNextSWAR(ByteBuffer bb, long pattern, int start, int limit) {
        int i;
        for (i = start; i <= limit - 8; i += 8) {
            long word = bb.getLong(i);
            //step12. find ';' within word
            int index = firstAnyPattern(word, pattern);
            if (index < Long.BYTES) {
                //step15. return index of ';'
                return i + index;
            }
        }
        // Handle remaining bytes
        for (; i < limit; i++) {
            if (bb.get(i) == (byte) pattern) {
                return i;
            }
        }
        return limit; // delimiter not found
    }
    
    //step. ';'을 빠르게 찾기 위해 특정 패턴의 bytecode로 변환
    //create a long integer where all 8 bytes contain the same value, which is the byte pattern you're looking for.
    //This is useful for SWAR operations where you want to compare multiple bytes in a single operation.
    //ex. If the input byte is 0x3B, the output long would be 0x3B3B3B3B3B3B3B3B
    private static long compilePattern(byte value) {
        return ((long) value << 56) | ((long) value << 48) | ((long) value << 40) | ((long) value << 32) |
            ((long) value << 24) | ((long) value << 16) | ((long) value << 8) | (long) value;
    }
    
    //find the position of any byte in word that matches any byte in pattern.
    //how to find bytes in arrays effectively? (ref: https://richardstartin.github.io/posts/finding-bytes)
    private static int firstAnyPattern(long word, long pattern) {
        //long word = read from the ByteBuffer, representing a block of 8 bytes to be checked.
        //long pattern = A long value prepared by compilePattern(), representing the pattern to search for in word.
        
        //step13. It starts by XOR-ing word with pattern. If a byte in word matches the corresponding byte in pattern, this operation results in 0 for that byte position; otherwise, it's a non-zero value.
        final long match = word ^ pattern;
        
        //step14. The result of XOR is then manipulated using a series of bitwise operations (match - 0x0101010101010101L, & ~match, & 0x8080808080808080L) to isolate the least significant bit of each byte that was a match (turned into 0 by XOR). This series of operations effectively creates a mask where only the bits corresponding to matching positions are set.
        long mask = match - 0x0101010101010101L;
        mask &= ~match;
        mask &= 0x8080808080808080L;
        
        //step15. find the index of the first set bit in this mask, which corresponds to the position of the first matching byte. This index is then divided by 8 (right-shift by 3) because each byte has 8 bits, and we want to convert the bit position back to a byte index.
        return Long.numberOfTrailingZeros(mask) >>> 3;
    }
    
    record FileSegment(long start, long end) {
    }
    
    /** Using this way to segment the file is much prettier, from spullara */
    private static List<FileSegment> getFileSegments(File file) throws IOException {
        //step2. mac m1은 1 CPU, 8 core이니까, 8개의 프로세서만큼 파일을 segments로 나눠 병렬처리할 것
        final int numberOfSegments = Runtime.getRuntime().availableProcessors();
        final long fileSize = file.length();
        final long segmentSize = fileSize / numberOfSegments;
        final List<FileSegment> segments = new ArrayList<>();
        
        //step3. 1.4GiB file을 코어의 갯수(8개)로 나눈다.
        try (RandomAccessFile randomAccessFile = new RandomAccessFile(file, "r")) {
            for (int i = 0; i < numberOfSegments; i++) {
                long segStart = i * segmentSize;
                long segEnd = (i == numberOfSegments - 1) ? fileSize : segStart + segmentSize;
                
                //step4. 8개로 나눈 각 segment의 시작과 끝 지점을 딴다.
                segStart = findSegment(i, 0, randomAccessFile, segStart, segEnd);
                segEnd = findSegment(i, numberOfSegments - 1, randomAccessFile, segEnd, fileSize);
                
                //step5. segments List에 각 segment의 시작지점, 끝지점을 담는다.
                segments.add(new FileSegment(segStart, segEnd));
            }
        }
        //step6. 파일을 프로세서 갯수(8개)로 나눈 segment들의 시작&끝 지점이 담긴 리스트를 건내준다.
        return segments;
    }
    
    // 파일을 코어 수 만큼 8등분 할 떄,
    // 대충 등분하면 라인 중간에 잘릴 수 있으니까
    // raf.seek()랑 raf.read()로 딱 개행문자(\n)까지 찾아서 거기서 자르는거임
    // 그다음에 그 쪼갠 부분들을 메모리 매핑으로 읽는거지
    // 이렇게 하면 각 쓰레드가 딱 라인 단위로 끊어진 청크만 처리하니까 훨씬 깔끔함. 뒤에서 지저분하게 체크할 필요도 없고.
    private static long findSegment(int i, int skipSegment, RandomAccessFile raf, long location, long fileSize) throws IOException {
        if (i != skipSegment) {
            raf.seek(location);
            while (location < fileSize) {
                location++;
                if (raf.read() == '\n') // \n 개행문자 기준으로 자름
                    return location;
            }
        }
        return location;
    }
    
    /**
     * Branchless parser, goes from String to int (10x):
     * "-1.2" to -12
     * "40.1" to 401
     * etc.
     *
     * @param input
     * @return int value x10
     */
    private static int branchlessParseInt(final byte[] input, int start, int length) {
        // 0 if positive, 1 if negative
        final int negative = ~(input[start] >> 4) & 1;
        // 0 if nr length is 3, 1 if length is 4
        final int has4 = ((length - negative) >> 2) & 1;
        
        final int digit1 = input[start + negative] - '0';
        final int digit2 = input[start + negative + has4];
        final int digit3 = input[start + negative + has4 + 2];
        
        return (-negative ^ (has4 * (digit1 * 100) + digit2 * 10 + digit3 - 528) - negative); // 528 == ('0' * 10 + '0')
    }
    
    // branchless max (unprecise for large numbers, but good enough)
    static int max(final int a, final int b) {
        final int diff = a - b;
        final int dsgn = diff >> 31;
        return a - (diff & dsgn);
    }
    
    // branchless min (unprecise for large numbers, but good enough)
    static int min(final int a, final int b) {
        final int diff = a - b;
        final int dsgn = diff >> 31;
        return b + (diff & dsgn);
    }
    
    /**
     * A normal Java HashMap does all these safety things like boundary checks... we don't need that, we need speeeed.
     *
     * So I've written an extremely simple linear probing hashmap that should work well enough.
     */
    class BitTwiddledMap {
        private static final int SIZE = 16384; // A bit larger than the number of keys, needs power of two
        //Q. 왜 숫자를 이렇게 조절한거지?
        //A. hashmap에서 collision이 병목지점이 될 수 있으니 막으려고.
        //   해시 충돌 해결은 선형 탐색(linear probing) 방식으로 함. 충돌나면 다음 칸 보고, 또 충돌나면 또 다음 칸 보고... 이런식임
        //   해시 테이블 크기를 **16384(2^14)**로 잡았는데, 이게 실제 측정소 수(약 10,000개)보다 더 크게 잡아서 충돌 확률 줄임
        //   2의 거듭제곱으로 해서 모듈로 연산을 비트 연산으로 바꿀 수 있게 함 (?)
        private int[] indices = new int[SIZE]; // Hashtable is just an int[]
        
        
        BitTwiddledMap() {
            // Optimized fill with -1, fastest method:
            int len = indices.length;
            if (len > 0) {
                indices[0] = -1;
            }
            // Value of i will be [1, 2, 4, 8, 16, 32, ..., len]
            for (int i = 1; i < len; i += i) {
                System.arraycopy(indices, 0, indices, i, i);
            }
        }
        
        private List<Entry> values = new ArrayList<>(512);
        
        //이 커스텀 해시맵의 키는 String이 아닌 byte[]라 new String()으로 객체 10억개씩 안만들어줘서 되서 gc 없으니까 더 빠름.
        record Entry(int hash, byte[] key, Measurement measurement) {
            @Override
            public String toString() {
                return new String(key) + "=" + measurement;
            }
        }
        
        /**
         * Who needs methods like add(), merge(), compute() etc, we need one, getOrCreate.
         * @param key
         * @return
         */
        public Measurement getOrCreate(byte[] key, int length) {
            int inHash;
            int index = (SIZE - 1) & (inHash = hashCode(key, length));
            int valueIndex;
            Entry retrievedEntry = null;
            while ((valueIndex = indices[index]) != -1 && (retrievedEntry = values.get(valueIndex)).hash != inHash) {
                index = (index + 1) % SIZE;
            }
            if (valueIndex >= 0) {
                return retrievedEntry.measurement;
            }
            // New entry, insert into table and return.
            indices[index] = values.size();
            
            // Only parse this once:
            byte[] actualKey = new byte[length];
            System.arraycopy(key, 0, actualKey, 0, length);
            
            Entry toAdd = new Entry(inHash, actualKey, new Measurement());
            values.add(toAdd);
            return toAdd.measurement;
        }
        
        private static int hashCode(byte[] a, int length) {
            int result = 1;
            for (int i = 0; i < length; i++) {
                result = 31 * result + a[i];
            }
            return result;
        }
    }
    
}

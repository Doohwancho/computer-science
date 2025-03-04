// create-files.js
const fs = require('fs');

console.log('Creating test files...');

// 테스트 파일 생성 함수
function createTestFile(filename, contentPattern, repetitions) {
  console.log(`Creating ${filename}...`);

  // 스트림으로 대용량 파일 작성 (메모리 효율적)
  const writeStream = fs.createWriteStream(`./data/${filename}`);

  // 청크 단위로 쓰기
  const chunkSize = 1000;
  for (let i = 0; i < repetitions; i += chunkSize) {
    const chunk = contentPattern.repeat(Math.min(chunkSize, repetitions - i));
    writeStream.write(chunk);
  }

  writeStream.end();
  console.log(`Created ${filename}`);
}

// data 디렉토리 생성
if (!fs.existsSync('./data')) {
  fs.mkdirSync('./data');
}

// 파일 생성
createTestFile('small-file.txt', 'Hello world! This is a small test file. ', 100);  // 약 5KB
createTestFile('medium-file.txt', 'This is a medium sized test file with some content. ', 5000);  // 약 250KB
createTestFile('large-file.txt', 'This is a large test file with lots of repeated content to process. ', 50000);  // 약 3MB

console.log('All test files created successfully!');

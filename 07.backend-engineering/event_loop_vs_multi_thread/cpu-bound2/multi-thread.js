const express = require('express');
const { Worker } = require('worker_threads');
const app = express();
const PORT = 3003;
const os = require('os');

// 워커 스레드 풀 생성
const numCores = os.cpus().length;
const workers = [];

for (let i = 0; i < numCores; i++) {
  const worker = new Worker(`
    const fs = require('fs');
    const { parentPort } = require('worker_threads');
    
    parentPort.on('message', (filePath) => {
      // 파일을 읽고 단어 수 계산
      fs.readFile(filePath, 'utf8', (err, data) => {
        if (err) {
          parentPort.postMessage({ error: err.message });
          return;
        }
        
        // 단어 수 계산 로직을 싱글스레드 버전과 일치시킴
        const wordCount = data.split(/\\s+/).length;
        parentPort.postMessage({ wordCount });
      });
    });
  `, { eval: true });
  
  workers.push(worker);
}

// 간단한 라운드 로빈 스케줄러
let currentWorker = 0;
function getNextWorker() {
  const worker = workers[currentWorker];
  currentWorker = (currentWorker + 1) % workers.length;
  return worker;
}

app.get('/file/:size', (req, res) => {
  const size = req.params.size; // small, medium, large
  const filePath = `./data/${size}-file.txt`;
  const worker = getNextWorker();
  
  worker.once('message', (result) => {
    if (result.error) {
      return res.status(500).json({ error: result.error });
    }
    res.json({ size, wordCount: result.wordCount });
  });
  
  worker.postMessage(filePath);
});

app.listen(PORT, () => {
  console.log(`IO-bound multi-thread server running on port ${PORT}`);
});

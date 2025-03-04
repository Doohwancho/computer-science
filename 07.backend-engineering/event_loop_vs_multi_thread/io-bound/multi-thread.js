// true-io-bound/multi-thread.js
const express = require('express');
const { Worker } = require('worker_threads');
const os = require('os');
const app = express();
const PORT = 4001;

// 워커 스레드 풀 생성
const numCores = os.cpus().length;
const workers = [];

for (let i = 0; i < numCores; i++) {
  const worker = new Worker(`
    const fs = require('fs');
    const { parentPort } = require('worker_threads');
    
    parentPort.on('message', (task) => {
      if (task.type === 'filestat') {
        // 파일 상태 확인
        fs.stat(task.path, (err, stats) => {
          if (err) {
            parentPort.postMessage({ id: task.id, error: err.message });
          } else {
            parentPort.postMessage({ id: task.id, size: stats.size });
          }
        });
      } else if (task.type === 'network') {
        // 네트워크 지연 시뮬레이션
        setTimeout(() => {
          parentPort.postMessage({ id: task.id, completed: true, delay: task.delay });
        }, task.delay);
      }
    });
  `, { eval: true });
  
  workers.push(worker);
}

// 라운드 로빈 스케줄러
let currentWorker = 0;
function getNextWorker() {
  const worker = workers[currentWorker];
  currentWorker = (currentWorker + 1) % workers.length;
  return worker;
}

// 워커에 작업 전송 및 응답 수신을 위한 래퍼 함수
function runWorkerTask(task) {
  return new Promise((resolve) => {
    const worker = getNextWorker();
    worker.once('message', (result) => {
      resolve(result);
    });
    worker.postMessage(task);
  });
}

// 여러 작은 파일에 대한 IO 작업
app.get('/io-test', async (req, res) => {
  try {
    const files = ['file1.txt', 'file2.txt', 'file3.txt', 'file4.txt', 'file5.txt'];
    const tasks = files.map((file, index) => ({
      type: 'filestat',
      path: `./data/small-files/${file}`,
      id: index
    }));
    
    // 모든 작업을 병렬로 워커에 분배
    const results = await Promise.all(tasks.map(task => runWorkerTask(task)));
    
    res.json({ success: true, results });
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

// 네트워크 IO 시뮬레이션
app.get('/network-io', async (req, res) => {
  try {
    const requests = 5;
    const tasks = [];
    
    for (let i = 0; i < requests; i++) {
      const delay = 10 + Math.random() * 20; // 10-30ms의 랜덤 지연
      tasks.push({
        type: 'network',
        delay,
        id: i
      });
    }
    
    // 모든 네트워크 시뮬레이션 작업을 병렬로 워커에 분배
    const results = await Promise.all(tasks.map(task => runWorkerTask(task)));
    
    res.json({ success: true, results });
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

app.listen(PORT, () => {
  console.log(`True IO-bound multi-thread server running on port ${PORT}`);
});

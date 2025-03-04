// cpu-bound/multi-thread.js
const express = require('express');
const { Worker } = require('worker_threads');
const app = express();
const PORT = 3001;
const os = require('os');

// 워커 스레드 풀 생성
const numCores = os.cpus().length;
const workers = [];

for (let i = 0; i < numCores; i++) {
  const worker = new Worker(`
    const { parentPort } = require('worker_threads');

    function isPrime(num) {
      if (num <= 1) return false;
      if (num <= 3) return true;

      if (num % 2 === 0 || num % 3 === 0) return false;

      let i = 5;
      while (i * i <= num) {
        if (num % i === 0 || num % (i + 2) === 0) return false;
        i += 6;
      }

      return true;
    }

    function findNthPrime(n) {
      let count = 0;
      let num = 2;

      while (count < n) {
        if (isPrime(num)) {
          count++;
        }
        num++;
      }

      return num - 1;
    }

    parentPort.on('message', (n) => {
      const result = findNthPrime(n);
      parentPort.postMessage(result);
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

app.get('/prime/:n', (req, res) => {
  const n = parseInt(req.params.n);
  const worker = getNextWorker();

  worker.once('message', (result) => {
    res.json({ n, prime: result });
  });

  worker.postMessage(n);
});

app.listen(PORT, () => {
  console.log(`CPU-bound multi-thread server running on port ${PORT}`);
});


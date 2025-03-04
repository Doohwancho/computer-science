// true-io-bound/single-thread.js
const express = require('express');
const fs = require('fs');
const app = express();
const PORT = 4000;

// 여러 작은 파일에 대한 빠른 IO 작업 (읽기만 하고 처리는 최소화)
app.get('/io-test', async (req, res) => {
  try {
    // 여러 개의 작은 파일을 비동기적으로 읽기
    // 실제 파일 내용 처리는 최소화 (파일 크기만 확인)
    const files = ['file1.txt', 'file2.txt', 'file3.txt', 'file4.txt', 'file5.txt'];
    const results = [];
    
    // Promise.all을 사용하여 모든 파일 읽기 작업을 병렬로 시작
    await Promise.all(files.map(async (file) => {
      return new Promise((resolve) => {
        fs.stat(`./data/small-files/${file}`, (err, stats) => {
          if (!err) {
            results.push({ file, size: stats.size });
          } else {
            results.push({ file, error: err.message });
          }
          resolve();
        });
      });
    }));
    
    // 최소한의 데이터 처리 후 응답
    res.json({ success: true, results });
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

// 네트워크 IO 시뮬레이션
app.get('/network-io', (req, res) => {
  // 여러 개의 비동기 타이머를 사용하여 네트워크 지연을 시뮬레이션
  const requests = 5;
  let completed = 0;
  const results = [];
  
  for (let i = 0; i < requests; i++) {
    // 각각 다른 지연 시간으로 네트워크 요청을 시뮬레이션
    const delay = 10 + Math.random() * 20; // 10-30ms의 랜덤 지연
    setTimeout(() => {
      results.push({ id: i, delay });
      completed++;
      
      if (completed === requests) {
        res.json({ success: true, results });
      }
    }, delay);
  }
});

app.listen(PORT, () => {
  console.log(`True IO-bound single-thread server running on port ${PORT}`);
});

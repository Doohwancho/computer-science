// cpu-bound/single-thread.js
const express = require('express');
const app = express();
const PORT = 3000;

// CPU 바운드 작업: 큰 수의 소수 여부 확인
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

app.get('/prime/:n', (req, res) => {
  const n = parseInt(req.params.n);
  const result = findNthPrime(n);
  res.json({ n, prime: result });
});

app.listen(PORT, () => {
  console.log(`CPU-bound single-thread server running on port ${PORT}`);
});

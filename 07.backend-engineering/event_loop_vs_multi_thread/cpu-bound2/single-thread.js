// io-bound/single-thread.js
const express = require('express');
const fs = require('fs');
const app = express();
const PORT = 3002;

app.get('/file/:size', (req, res) => {
  const size = req.params.size; // small, medium, large
  const filePath = `./data/${size}-file.txt`;

  // 비동기 파일 읽기 (이벤트 루프 모델)
  fs.readFile(filePath, (err, data) => {
    if (err) {
      return res.status(500).json({ error: err.message });
    }

    // 간단한 텍스트 처리 (단어 수 세기)
    const wordCount = data.toString().split(/\s+/).length;
    res.json({ size, wordCount });
  });
});

app.listen(PORT, () => {
  console.log(`IO-bound single-thread server running on port ${PORT}`);
});

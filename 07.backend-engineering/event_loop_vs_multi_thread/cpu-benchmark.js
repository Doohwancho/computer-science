// benchmark.js
const axios = require('axios');
const { performance } = require('perf_hooks');

async function runBenchmark(url, iterations, concurrency) {
  console.log(`Benchmarking ${url} with ${iterations} iterations and ${concurrency} concurrent requests`);

  const start = performance.now();

  // 병렬 요청 생성
  const requests = [];
  for (let i = 0; i < iterations; i++) {
    const request = axios.get(url)
      .then(response => ({
        success: true,
        time: response.headers['x-response-time'],
        data: response.data
      }))
      .catch(error => ({
        success: false,
        error: error.message
      }));

    requests.push(request);

    // 동시성 제어 (concurrency 단위로만 요청 보내기)
    if (requests.length >= concurrency) {
      await Promise.all(requests);
      requests.length = 0;
    }
  }

  // 남은 요청 처리
  if (requests.length > 0) {
    await Promise.all(requests);
  }

  const end = performance.now();
  const totalTime = end - start;

  console.log(`Total time: ${totalTime.toFixed(2)}ms`);
  console.log(`Average request time: ${(totalTime / iterations).toFixed(2)}ms`);
  console.log(`Requests per second: ${(iterations / (totalTime / 1000)).toFixed(2)}`);
  console.log('-----------------------------------');

  return {
    totalTime,
    averageTime: totalTime / iterations,
    requestsPerSecond: iterations / (totalTime / 1000)
  };
}

async function main() {
  // 테스트 설정
  const iterations = 100;
  const concurrency = 10;

  // CPU 바운드 테스트 (10000번째 소수 찾기)
  console.log('CPU-BOUND TESTS 1');
  console.log('case1) single-thread event loop');
  const cpuSingleResult = await runBenchmark('http://localhost:3000/prime/10000', iterations, concurrency);
  console.log('case2) multi-thread');
  const cpuMultiResult = await runBenchmark('http://localhost:3001/prime/10000', iterations, concurrency);

  // CPU 바운드 테스트2 (대용량 파일 읽기 & word count)
  console.log('CPU-BOUND TESTS 2');
  console.log('case1) single-thread event loop');
  const cpuSingleResult2 = await runBenchmark('http://localhost:3002/file/large', iterations, concurrency);
  console.log('case2) multi-thread');
  const cpuMultiResult2 = await runBenchmark('http://localhost:3003/file/large', iterations, concurrency);

  // 결과 비교
  console.log('COMPARISON RESULTS:');

  console.log('CPU-BOUND1:');
  console.log(`Single-thread RPS: ${cpuSingleResult.requestsPerSecond.toFixed(2)}`);
  console.log(`Multi-thread RPS: ${cpuMultiResult.requestsPerSecond.toFixed(2)}`);
  console.log(`Performance improvement: ${((cpuMultiResult.requestsPerSecond / cpuSingleResult.requestsPerSecond) * 100 - 100).toFixed(2)}%`);

  console.log('CPU-BOUND2:');
  console.log(`Single-thread RPS: ${cpuSingleResult2.requestsPerSecond.toFixed(2)}`);
  console.log(`Multi-thread RPS: ${cpuMultiResult2.requestsPerSecond.toFixed(2)}`);
  console.log(`Performance difference: ${((cpuMultiResult2.requestsPerSecond / cpuSingleResult2.requestsPerSecond) * 100 - 100).toFixed(2)}%`);
}

main().catch(console.error);

// benchmark.js (벤치마크 스크립트)
const axios = require('axios');

// 벤치마크 실행 함수
async function runBenchmark(url, iterations, concurrency) {
  console.log(`Benchmarking ${url} with ${iterations} iterations and ${concurrency} concurrent requests`);
  
  const totalStartTime = Date.now();
  const responseTimes = [];
  
  // 동시 요청 처리를 위한 함수
  async function runBatch(batchSize) {
    const promises = [];
    
    for (let i = 0; i < batchSize; i++) {
      promises.push(
        (async () => {
          const startTime = Date.now();
          await axios.get(url);
          const endTime = Date.now();
          responseTimes.push(endTime - startTime);
        })()
      );
    }
    
    await Promise.all(promises);
  }
  
  // 반복 실행
  const batchCount = Math.ceil(iterations / concurrency);
  for (let i = 0; i < batchCount; i++) {
    const remainingRequests = iterations - i * concurrency;
    const currentBatchSize = Math.min(concurrency, remainingRequests);
    await runBatch(currentBatchSize);
  }
  
  const totalEndTime = Date.now();
  const totalTime = totalEndTime - totalStartTime;
  
  // 결과 계산
  const avgResponseTime = responseTimes.reduce((sum, time) => sum + time, 0) / responseTimes.length;
  const requestsPerSecond = (iterations / totalTime) * 1000;
  
  console.log(`Total time: ${totalTime.toFixed(2)}ms`);
  console.log(`Average request time: ${avgResponseTime.toFixed(2)}ms`);
  console.log(`Requests per second: ${requestsPerSecond.toFixed(2)}`);
  
  return {
    totalTime,
    avgResponseTime,
    requestsPerSecond
  };
}

// 메인 벤치마크 함수
async function main() {
  const iterations = 100;
  const concurrency = 10;
  
  console.log('PURE IO-BOUND TESTS');
  console.log('--------------------------------');
  
  console.log('case1) Single-thread event loop - 파일 IO');
  const singleThreadFileResults = await runBenchmark('http://localhost:4000/io-test', iterations, concurrency);
  console.log('-----------------------------------');
  
  console.log('case2) Multi-thread - 파일 IO');
  const multiThreadFileResults = await runBenchmark('http://localhost:4001/io-test', iterations, concurrency);
  console.log('-----------------------------------');
  
  console.log('case3) Single-thread event loop - 네트워크 IO');
  const singleThreadNetworkResults = await runBenchmark('http://localhost:4000/network-io', iterations, concurrency);
  console.log('-----------------------------------');
  
  console.log('case4) Multi-thread - 네트워크 IO');
  const multiThreadNetworkResults = await runBenchmark('http://localhost:4001/network-io', iterations, concurrency);
  console.log('-----------------------------------');
  
  // 성능 비교 결과 출력
  console.log('COMPARISON RESULTS:');
  
  console.log('파일 IO:');
  console.log(`Single-thread RPS: ${singleThreadFileResults.requestsPerSecond.toFixed(2)}`);
  console.log(`Multi-thread RPS: ${multiThreadFileResults.requestsPerSecond.toFixed(2)}`);
  const fileImprovementPercent = ((multiThreadFileResults.requestsPerSecond / singleThreadFileResults.requestsPerSecond) * 100) - 100;
  console.log(`Performance difference: ${fileImprovementPercent > 0 ? '+' : ''}${fileImprovementPercent.toFixed(2)}%`);
  
  console.log('네트워크 IO:');
  console.log(`Single-thread RPS: ${singleThreadNetworkResults.requestsPerSecond.toFixed(2)}`);
  console.log(`Multi-thread RPS: ${multiThreadNetworkResults.requestsPerSecond.toFixed(2)}`);
  const networkImprovementPercent = ((multiThreadNetworkResults.requestsPerSecond / singleThreadNetworkResults.requestsPerSecond) * 100) - 100;
  console.log(`Performance difference: ${networkImprovementPercent > 0 ? '+' : ''}${networkImprovementPercent.toFixed(2)}%`);
}

// 벤치마크 실행
main().catch(console.error);

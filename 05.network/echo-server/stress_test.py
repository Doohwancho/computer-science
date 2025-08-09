#!/usr/bin/env python3
import asyncio
import aiohttp
import time
import argparse
from concurrent.futures import ThreadPoolExecutor
import statistics

# Default settings
DEFAULT_URL = "http://localhost:8080/"
DEFAULT_DURATION = 10  # seconds
DEFAULT_CONCURRENCY = 100
DEFAULT_TIMEOUT = 5  # seconds

async def make_request(session, url, request_id, results, timeout):
    """Make a single HTTP request and record the result"""
    start_time = time.time()
    try:
        async with session.get(url, timeout=timeout) as response:
            await response.text()
            success = response.status == 200
    except Exception as e:
        success = False
        print(f"Request {request_id} failed: {e}")
    
    end_time = time.time()
    latency = end_time - start_time
    results.append((success, latency))
    return success, latency

async def worker(session, url, request_queue, results, timeout):
    """Worker to process requests from the queue"""
    while True:
        try:
            request_id = await request_queue.get()
            await make_request(session, url, request_id, results, timeout)
            request_queue.task_done()
        except asyncio.CancelledError:
            break
        except Exception as e:
            print(f"Worker error: {e}")
            request_queue.task_done()


async def run_stress_test(url, duration, concurrency, timeout, rate=0):  # rate 매개변수 추가
    """Run the stress test with specified parameters"""
    print(f"Starting stress test on {url}")
    print(f"Duration: {duration} seconds, Concurrency: {concurrency}, Timeout: {timeout} seconds")
    if rate > 0:
        print(f"Rate limited to {rate} requests per second")
    
    # Setup
    results = []
    total_requests = 0
    request_queue = asyncio.Queue()
    
    # Create client session with connection pooling
    connector = aiohttp.TCPConnector(limit=concurrency, limit_per_host=concurrency)
    timeout_obj = aiohttp.ClientTimeout(total=timeout)
    
    async with aiohttp.ClientSession(connector=connector, timeout=timeout_obj) as session:
        # Start workers
        workers = []
        for _ in range(concurrency):
            task = asyncio.create_task(worker(session, url, request_queue, results, timeout))
            workers.append(task)
        
        # Start test
        start_time = time.time()
        end_time = start_time + duration
        
        # 요청 생성 루프 수정
        request_id = 0
        try:
            if rate > 0:
                # 제한된 속도로 요청 생성
                interval = 1.0 / rate  # 요청 간 간격 (초)
                next_request_time = start_time
                
                while time.time() < end_time:
                    current_time = time.time()
                    
                    if current_time >= next_request_time:
                        await request_queue.put(request_id)
                        request_id += 1
                        next_request_time = current_time + interval
                    
                    # 짧은 대기 (CPU 사용량 감소)
                    await asyncio.sleep(0.001)
            else:
                # 기존 코드: 최대 속도로 요청
                while time.time() < end_time:
                    await request_queue.put(request_id)
                    request_id += 1
                    # Yield to allow workers to process
                    await asyncio.sleep(0)
            
            # Wait for all queued requests to complete
            await request_queue.join()
        except KeyboardInterrupt:
            print("Test interrupted by user")
        finally:
            # Cancel all workers
            for task in workers:
                task.cancel()
            await asyncio.gather(*workers, return_exceptions=True)
        
        actual_duration = time.time() - start_time
        total_requests = len(results)
        
        # Calculate statistics
        successful_requests = sum(1 for success, _ in results if success)
        if successful_requests > 0:
            latencies = [latency for success, latency in results if success]
            avg_latency = sum(latencies) / len(latencies)
            min_latency = min(latencies)
            max_latency = max(latencies)
            median_latency = statistics.median(latencies)
            p95_latency = sorted(latencies)[int(len(latencies) * 0.95)]
            p99_latency = sorted(latencies)[int(len(latencies) * 0.99)]
        else:
            avg_latency = min_latency = max_latency = median_latency = p95_latency = p99_latency = 0
        
        # Print results
        print("\n=== Stress Test Results ===")
        print(f"Total requests: {total_requests}")
        print(f"Requests per second: {total_requests / actual_duration:.2f}")
        print(f"Successful requests: {successful_requests} ({successful_requests / total_requests * 100:.2f}%)")
        print(f"Failed requests: {total_requests - successful_requests}")
        print(f"Average latency: {avg_latency*1000:.2f} ms")
        print(f"Min latency: {min_latency*1000:.2f} ms")
        print(f"Max latency: {max_latency*1000:.2f} ms")
        print(f"Median latency: {median_latency*1000:.2f} ms")
        print(f"95th percentile latency: {p95_latency*1000:.2f} ms")
        print(f"99th percentile latency: {p99_latency*1000:.2f} ms")

def main():
    parser = argparse.ArgumentParser(description="Stress test for HTTP echo server")
    parser.add_argument("--url", default=DEFAULT_URL, help=f"Target URL (default: {DEFAULT_URL})")
    parser.add_argument("--duration", type=int, default=DEFAULT_DURATION, 
                        help=f"Test duration in seconds (default: {DEFAULT_DURATION})")
    parser.add_argument("--concurrency", type=int, default=DEFAULT_CONCURRENCY, 
                        help=f"Number of concurrent connections (default: {DEFAULT_CONCURRENCY})")
    parser.add_argument("--timeout", type=float, default=DEFAULT_TIMEOUT, 
                        help=f"Request timeout in seconds (default: {DEFAULT_TIMEOUT})")
    # 요청 속도 제한을 위한 파라미터 추가
    parser.add_argument("--rate", type=int, default=0, 
                        help="Requests per second (0 for unlimited)")

    
    args = parser.parse_args()
    
    asyncio.run(run_stress_test(args.url, args.duration, args.concurrency, args.timeout, args.rate))

if __name__ == "__main__":
    main()

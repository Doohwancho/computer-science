#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

class Counter {
private:
    int value = 0;
    std::mutex mutex;

public:
    void increment(){
        // lock_guard는 스코프를 벗어나면 자동으로 언락함 
        std::lock_guard<std::mutex> lock(mutex);
        value++;
    }

    int getValue() {
        std::lock_guard<std::mutex> lock(mutex);
        return value;
    }
};

void worker(Counter& counter, int numIterators) {
    for(int i = 0; i < numIterators; i++) {
        counter.increment();
    }
}

int main(){
    Counter counter;
    const int numThreads = 5;
    const int numIterationsPerThread = 100000;

    std::vector<std::thread> threads;

    //여러 쓰레드 생성해서 동시에 카운트 증가시킨다 
    for (int i = 0; i < numThreads; i++){
        threads.push_back(std::thread(worker, std::ref(counter), numIterationsPerThread));
    }

    //모든 스레드 종료 기다림 
    for (auto& thread: threads) {
        thread.join();
    }

    std::cout << "예상 카운터 값: " << numThreads * numIterationsPerThread << std::endl;
    std::cout << "실제 카운터 값: " << counter.getValue() << std::endl;

    return 0;
}

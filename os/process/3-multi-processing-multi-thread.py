import concurrent.futures
import time

def process(x) :
    print(f"Sleeping for {x} seconds")
    time.sleep(x)
    return f"Done sleeping {x} seconds"


if __name__ == "__main__" :
    start = time.perf_counter()
    NUMBER_OF_PROCESSES = 3

    # Context manager will automatically join the processes before the context manager finishes
    # ProcessPoolExecutor() uses a pool of processes to execute calls asynchronously.
    # If max_workers are not set, default value is the number of processes in on the computer
    with concurrent.futures.ProcessPoolExecutor(max_workers=4) as executor :
        # submit(function, *args, **kwargs) : schedules the callable, to be executed
        # submit method returns Future object representing the execution of the callable
        secs = [i for i in range(1, NUMBER_OF_PROCESSES + 1)[::-1]]

        # map(function, *iterables) : the iterables are collected immediately rather than lazily
        results = executor.map(process, secs)

        # exceptions will be raised when iterating
        for result in results :
            try :
                print(result)
            except Exception as e :
                print(f"Errors when retrieving results : {e}")

    end = time.perf_counter()

    print(f"Total time {end - start}")

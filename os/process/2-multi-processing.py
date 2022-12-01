import time
import multiprocessing

def process(x) :
    print(f"Sleeping for {x} seconds")
    time.sleep(x)
    print(f"Wake up after {x} seconds")

if __name__ == "__main__" :
    start = time.perf_counter()

    NUMBER_OF_PROCESSES = 3

    processes = [multiprocessing.Process(target=process, args=[i]) for i in range(NUMBER_OF_PROCESSES)]

    for i in range(NUMBER_OF_PROCESSES) :
        processes[i].start()

    for i in range(NUMBER_OF_PROCESSES) :
        processes[i].join()

    end = time.perf_counter()

    print(f"Finished in total {end-start} seconds")



# console.log

#Sleeping for 1 seconds
#Sleeping for 2 seconds
#Sleeping for 0 seconds
#Wake up after 0 seconds
#Wake up after 1 seconds
#Wake up after 2 seconds
#Finished in total 2.6629597089777235 seconds

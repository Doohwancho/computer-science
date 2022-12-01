import time
import multiprocessing

def process(x) :
    print(f"Sleeping for {x} seonds")
    time.sleep(x)
    print(f"Wake up after {x} seconds")

if __name__ == "__main__" :
    start = time.perf_counter()

    NUMBER_OF_PROCESSES = 3

    processes = [process(i) for i in range(NUMBER_OF_PROCESSES)]

    end = time.perf_counter()

    print(f"Finished in total {end-start} seconds")

# console.log

#Sleeping for 0 seonds
#Wake up after 0 seconds
#Sleeping for 1 seonds
#Wake up after 1 seconds
#Sleeping for 2 seonds
#Wake up after 2 seconds
#Finished in total 3.0104972499830183 seconds

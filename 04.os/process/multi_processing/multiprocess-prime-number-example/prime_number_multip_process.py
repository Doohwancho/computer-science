import concurrent.futures
import math
import time

PRIMES = [
    112272535095293,
    112582705942171,
    112272535095293,
    115280095190773,
    115797848077099,
    1099726899285419
]

def is_prime(x) :
    if x < 2 :
        return False
    elif x % 2 == 0 :
        return False

    limit = int(math.floor(math.sqrt(x)))
    for i in range(3, limit + 1, 2) :
        if x % i == 0 :
            return False

    return True

def main() :
    with concurrent.futures.ProcessPoolExecutor() as executor :
        results = executor.map(is_prime, PRIMES)
        for result, prime in zip(results, PRIMES) :
            if result :
                print(f"{prime} is a prime number")
            else :
                print(f"{prime} is not a prime number")

if __name__ == "__main__" :
    start = time.perf_counter()

    main()

    end = time.perf_counter()
    print(f"It took {end - start} seconds")

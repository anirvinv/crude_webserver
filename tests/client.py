import time
import grequests


def main():
    URLs = [
        "http://localhost:3000/",
        "http://localhost:3000/something.html",
        "http://localhost:3000/index.html",
    ] * 50
    start_time = time.perf_counter()
    rs = (grequests.get(u) for u in URLs)
    grequests.map(rs)
    end_time = time.perf_counter()

    print("Elapsed time: ", end_time - start_time, " seconds")


if __name__ == "__main__":
    main()

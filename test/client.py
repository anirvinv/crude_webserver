import time
import requests


def main():
    URLs = [
        "http://localhost:3000/",
        "http://localhost:3000/.gitignore",
        "http://localhost:3000/index.html",
        "http://localhost:3000/client.c",
        "http://localhost:3000/makefile",
        "http://localhost:3000/README.md",
        "http://localhost:3000/something.html",
        "http://localhost:3000/runtests.sh",
        "http://localhost:3000/sthread_server.c",
        "http://localhost:3000/mthread_server.c",
    ] * 50
    start_time = time.perf_counter()
    for idx, URL in enumerate(URLs):
        request_time_start = time.perf_counter()
        try:
            r = requests.get(url=URL)
        except:
            print("Run make and start the server first. Check readme for instructions")
            return

        request_time_end = time.perf_counter()
        print(f"Request: {idx}", request_time_end - request_time_start, " seconds")

    end_time = time.perf_counter()
    print("Elapsed time: ", end_time - start_time, " seconds")


if __name__ == "__main__":
    main()
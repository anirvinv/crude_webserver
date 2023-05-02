all: src/mthread_server.c src/sthread_server.c
	gcc src/mthread_server.c -std=gnu99 -lpthread -fsanitize=address -o builds/mthread_server.out
	gcc src/sthread_server.c -std=gnu99 -lpthread -fsanitize=address -o builds/sthread_server.out
clean:
	rm builds/mthread_server.out
	rm builds/sthread_server.out
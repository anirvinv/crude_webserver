all: src/mthread_server.c src/sthread_server.c
	gcc src/mthread_server.c -std=gnu99 -lpthread -fsanitize=address -o builds/mthread_server
	gcc src/sthread_server.c -std=gnu99 -lpthread -fsanitize=address -o builds/sthread_server
clean:
	rm builds/mthread_server
	rm builds/sthread_server
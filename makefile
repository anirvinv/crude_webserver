all: mthread_server.c sthread_server.c
	gcc mthread_server.c -std=gnu99 -lpthread -fsanitize=address -o mthread_server.out
	gcc sthread_server.c -std=gnu99 -lpthread -fsanitize=address -o sthread_server.out
clean:
	rm mthread_server.out
	rm sthread_server.out
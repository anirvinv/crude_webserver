all: webserver.c client.c
	gcc webserver.c -std=gnu99 -lpthread -fsanitize=address -o webserver.out
clean:
	rm webserver.out
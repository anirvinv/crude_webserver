all: webserver.c client.c
	gcc -std=gnu99 -fsanitize=address -o webserver.out webserver.c
	gcc -std=gnu99 -fsanitize=address -o client.out client.c
clean:
	rm webserver.out
	rm client.out
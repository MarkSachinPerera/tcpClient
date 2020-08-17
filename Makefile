all:
	gcc -Wall -pthread -g -o client client.c
	gcc -Wall -pthread -g -o server server.c
clean:
	rm client server

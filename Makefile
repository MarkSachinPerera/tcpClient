all:
	gcc -Wall -pthread -g -o client client.c -I include
	gcc -Wall -pthread -g -o server server.c -I include
clean:
	rm client server

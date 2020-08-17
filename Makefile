all:
	gcc -Wall -g -o client client.c helper.c -include helper.h
	#gcc -Wall -g -o server server.c helper.c -include helper.h
clean:
	rm client server

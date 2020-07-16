TARGET: exe

exe: server.o clien.o socket.o
	gcc server.o -o server
	gcc client.o -o client

server.o:
	gcc -g -Wall server.c -o server.o

client.o:
	gcc -g -Wall client.c -o client.o

socket.o:
	gcc -g -Wall socket.c -o socket.o

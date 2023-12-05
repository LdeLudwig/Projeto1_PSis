all: lizard-client lizardNroaches-server

lizard-client: lizard-client.c
	gcc lizard-client.c -o lizard-client -lncurses -lzmq

lizardNroaches-server: lizardNroaches-server.c
	gcc lizardNroaches-server.c -o lizardNroaches-server -lncurses -lzmq

clean:
	rm lizard-client lizardNroaches-server *.o
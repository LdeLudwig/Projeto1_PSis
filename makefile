all: lizard-client lizardNroaches-server roaches-client

lizard-client: lizard-client.c
	gcc lizard-client.c -o lizard-client -lncurses -lzmq

lizardNroaches-server: lizardNroaches-server.c
	gcc lizardNroaches-server.c -o lizardNroaches-server -lncurses -lzmq

roaches-client: roaches-client.c
	gcc roaches-client.c -o roaches-client -lncurses -lzmq

test1: roaches-client
	gcc testServer 

clean:
	rm lizard-client lizardNroaches-server roaches-client *.o
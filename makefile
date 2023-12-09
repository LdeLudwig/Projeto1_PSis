all: lizard-client lizardNroaches-server lizardNroaches2 lizardNroaches3 roaches-client

lizard-client: lizard-client.c
	gcc lizard-client.c -o lizard-client -lncurses -lzmq

lizardNroaches-server: lizardNroaches-server.c
	gcc lizardNroaches-server.c -o lizardNroaches-server -lncurses -lzmq

lizardNroaches2: lizardNroaches2.c
	gcc lizardNroaches2.c -o lizardNroaches2 -lncurses -lzmq

lizardNroaches3: lizardNroaches3.c
	gcc lizardNroaches3.c -o lizardNroaches3 -lncurses -lzmq

roaches-client: roaches-client.c
	gcc roaches-client.c -o roaches-client -lncurses -lzmq

clean:
	rm lizard-client lizardNroaches-server lizardNroaches2 lizardNroaches3 roaches-client *.o
all: lizard-client lizardNroaches-server lizardNroaches2 lizardNroaches3 lizardNroaches4 lista Alizard roaches-client

lizard-client: lizard-client.c
	gcc lizard-client.c -o lizard-client -lncurses -lzmq

lizardNroaches-server: lizardNroaches-server.c
	gcc lizardNroaches-server.c -o lizardNroaches-server -lncurses -lzmq

lizardNroaches2: lizardNroaches2.c
	gcc lizardNroaches2.c -o lizardNroaches2 -lncurses -lzmq

lizardNroaches3: lizardNroaches3.c
	gcc lizardNroaches3.c -o lizardNroaches3 -lncurses -lzmq

lizardNroaches4: lizardNroaches4.c
	gcc lizardNroaches4.c -o lizardNroaches4 -lncurses -lzmq

lista: lista.c
	gcc lista.c -o lista -lncurses -lzmq

Alizard: Alizard.c
	gcc Alizard.c -o Alizard -lncurses -lzmq
	
roaches-client: roaches-client.c
	gcc roaches-client.c -o roaches-client -lncurses -lzmq

clean:
	rm lizard-client lizardNroaches-server lizardNroaches2 lizardNroaches3 lizardNroaches4 lista roaches-client *.o
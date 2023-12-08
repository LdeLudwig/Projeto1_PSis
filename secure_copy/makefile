all: lizard server roach

lizard: lizard.c
	gcc lizard.c -o lizard -lncurses -lzmq

server: server.c
	gcc server.c -o server -lncurses -lzmq

roach: roach.c
	gcc roach.c -o roach -lncurses -lzmq

clean:
	rm lizard server roach *.o
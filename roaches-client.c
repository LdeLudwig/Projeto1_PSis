#include "structs.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <ncurses.h>
#include <zmq.h>

int main()
{	 
    char answer[10];
    // Initialize the ZeroMQ context and socket
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REQ);
    zmq_connect(socket, "tcp://localhost:5556");

    //TODO_5
    // read the character from the user
    int num;
    num = random()%5 + 1;

    // TODO_6
    // send connection message
    cockroaches_t roach;
    roach.msg_type = ROACH_CONNECT;
    roach.num = num;
    
    //Depois envio o individuo:
    zmq_send(socket, &roach, sizeof(cockroaches_t), 0);
    zmq_recv(socket, answer, 10, 0);

    int sleep_delay;
    direction direction;
    int n = 0;
    int key;
    while(1)
    {
        sleep_delay = random()%700000;
        n++;
        usleep(sleep_delay);
        direction = random()%4;
        switch (direction)
        {
        case LEFT:
           printf("%d Going Left   \n", n);
            break;
        case RIGHT:
            printf("%d Going Right   \n", n);
           break;
        case DOWN:
            printf("%d Going Down   \n", n);
            break;
        case UP:
            printf("%d Going Up    \n", n);
            break;
        }

        //TODO_9
        // prepare the movement message
        roach.direction = direction;
        roach.msg_type = ROACH_MOVEMENT;

        if(key != 'x'){
            zmq_send(socket, &roach, sizeof(cockroaches_t), 0);
            zmq_recv(socket, answer, 10, 0);        }
    } while (key != 'x');

    endwin();			/* End curses mode		  */
    zmq_close(socket);
    zmq_ctx_destroy(context);
 
	return 0;
}
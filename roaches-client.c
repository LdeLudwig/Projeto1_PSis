#include "structs.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <zmq.h>


int main()
{	 

    char answer[10];
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");

    //TODO_5
    // read the character from the user
    int ch;
    ch = random()%5 + 1;
    

    // TODO_6
    // send connection message
    cockroaches_t m;
    m.msg_type = 0;
    m.ch = ch;
    zmq_send(requester, &m, sizeof(cockroaches_t), 0);
    //zmq_recv(requester, answer, 10, 0);
    
    

    int sleep_delay;
    direction direction;
    int n = 0;
    int key;
    while (1)
    {
        n++;
        sleep_delay = random()%700000;
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
        m.direction = direction;
        m.msg_type = 1;

        //TODO_10
        //send the movement message
        if(key != 'x'){
            zmq_send(requester, &m, sizeof(cockroaches_t), 0);
            zmq_recv(requester, answer, 10, 0);        }
    } while (key != 27);

    endwin();			/* End curses mode		  */
    zmq_close(requester);
    zmq_ctx_destroy(context);
 
	return 0;
}
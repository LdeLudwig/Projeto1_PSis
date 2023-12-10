#include <ncurses.h>
#include "structs.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <ctype.h> 
#include <stdlib.h>
#include <zmq.h>
#include <unistd.h>
#include <string.h>
 

int main()
{
    char answer[50];
    // create and open the zmq socket
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");

    //Before it was like this:
    char ch;
    do{
        printf("what is your character(a..z)?: ");
        ch = getchar();
        ch = tolower(ch);  
    }while(!isalpha(ch));
    

    lizard_t m;
    m.msg_type = LIZARD_CONNECT;
    m.ch = ch;
    m.species_type = LIZARD;
    strcpy(m.tail, ".....");

    //Vou enviar a especie primeiro:
    zmq_send (requester, &m.species_type, sizeof(m.species_type), 0);
    zmq_recv(requester, answer, 50, 0);


	initscr();			/* Start curses mode 		*/
	cbreak();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */

    int n = 0;
    zmq_send (requester, &m, sizeof(m), 0);
    zmq_recv(requester, answer, 50, 0);
    //TODO_9
    // prepare the movement message
    m.msg_type = LIZARD_MOVEMENT;
    
    int key;
    do
    {
    	key = getch();
        if (key == 'q' || key == 'Q') {
            m.msg_type = LIZARD_DISCONNECT;
            zmq_send(requester, &m, sizeof(m), 0);
            zmq_recv(requester, answer, 3, 0);
            printf("\nGood bye...\n");  
            break; // Saia do loop quando 'q' for pressionado
        }		
        n++;
        switch (key)
        {
        case KEY_LEFT:
            mvprintw(0,0,"%d Left arrow is pressed", n);
            //TODO_9
            // prepare the movement message
           m.direction = LEFT;
            break;
        case KEY_RIGHT:
            mvprintw(0,0,"%d Right arrow is pressed", n);
            //TODO_9
            // prepare the movement message
            m.direction = RIGHT;
            break;
        case KEY_DOWN:
            mvprintw(0,0,"%d Down arrow is pressed", n);
            //TODO_9
            // prepare the movement message
           m.direction = DOWN;
            break;
        case KEY_UP:
            mvprintw(0,0,"%d :Up arrow is pressed", n);
            //TODO_9
            // prepare the movement message
            m.direction = UP;
            break;

        default:
            key = 'x'; 
            break;
        }

        //TODO_10
        //send the movement message
        if (key != 'x'){
            zmq_send(requester, &m, sizeof(lizard_t), 0);
            zmq_recv(requester, answer, 3, 0);  
        }
        refresh();			/* Print it on to the real screen */
    }while(key != 27);
    
    
  	endwin();			/* End curses mode		  */
    zmq_close(requester);
    zmq_close(context);
	return 0;
}
#include <zmq.h>
#include <ncurses.h>
#include "structs.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <stdlib.h>
#include <string.h>


#define WINDOW_SIZE 15

// STEP 1
typedef struct ch_info_t
{
    int ch;
    int pos_x, pos_y;
    char tail[5];
} ch_info_t;

direction random_direction(){
    return  random()%4;

}
    void new_position(int* x, int *y, direction direction){
        switch (direction)
        {
        case UP:
            (*x) --;
            if(*x ==0)
                *x = 2;
            break;
        case DOWN:
            (*x) ++;
            if(*x ==WINDOW_SIZE-1)
                *x = WINDOW_SIZE-3;
            break;
        case LEFT:
            (*y) --;
            if(*y ==0)
                *y = 2;
            break;
        case RIGHT:
            (*y) ++;
            if(*y ==WINDOW_SIZE-1)
                *y = WINDOW_SIZE-3;
            break;
        default:
            break;
        }
}

int find_ch_info(ch_info_t char_data[], int n_char, int ch){

    for (int i = 0 ; i < n_char; i++){
        if(ch == char_data[i].ch){
            return i;
        }
    }
    return -1;
}

int main()
{	 
    //STEP 2
    ch_info_t char_data[100];
    int n_chars = 0;

    //Ex 1 - Lab 5:
    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (responder, "tcp://*:5555");
    
    cockroaches_t array_roaches[10];
    lizard_t array_lizards[26];
	
    initscr();		    	
	cbreak();				
    keypad(stdscr, TRUE);   
	noecho();			    

    /* creates a window and draws a border */
    WINDOW * my_win = newwin(WINDOW_SIZE, WINDOW_SIZE, 0, 0);
    box(my_win, 0 , 0);	
	wrefresh(my_win);

    int ch;
    int pos_x;
    int pos_y;
    char tail[5];

    direction  direction;
    while (1)
    {
        for(int i=0; i < (sizeof(array_lizards) / sizeof(array_lizards[0]));i++){
            zmq_recv (responder, &array_lizards[i], sizeof(lizard_t), 0);
            if(array_lizards[i].msg_type == 0){
                ch = array_lizards[i].ch;
                pos_x = WINDOW_SIZE/2;
                pos_y = WINDOW_SIZE/2;

                //STEP 3
                char_data[n_chars].ch = ch;
                char_data[n_chars].pos_x = pos_x;
                char_data[n_chars].pos_y = pos_y;
                //strncpy(char_data[n_chars].tail, array_lizards[i].tail, sizeof(char_data[n_chars].tail));
                n_chars++;
            }
            if(array_lizards[i].msg_type == 1){
                //STEP 4
                int ch_pos = find_ch_info(char_data, n_chars, array_lizards[i].ch);
                if(ch_pos != -1){
                    pos_x = char_data[ch_pos].pos_x;
                    pos_y = char_data[ch_pos].pos_y;
                    ch = char_data[ch_pos].ch;
                    /*deletes old place */
                    wmove(my_win, pos_x, pos_y);
                    waddch(my_win,' ');

                    /* claculates new direction */
                    direction = array_lizards[i].direction;

                    /* claculates new mark position */
                    new_position(&pos_x, &pos_y, direction);
                    char_data[ch_pos].pos_x = pos_x;
                    char_data[ch_pos].pos_y = pos_y;
                }        
            }
            char tail[5];
            strcpy(tail, "....");
            /* draw mark on new position */
            wmove(my_win, pos_x, pos_y);
            waddch(my_win,ch| A_BOLD);
            for(int i = 0; i<4;i++){
                //Mano, por algum motivo tem um bug que as posições estão trocadas!
                //Perguntar ao lucas e explicar logica correta e logica do codigo!
                if(direction == UP){
                    wmove(my_win, pos_x+i+1, pos_y);
                    waddch(my_win,tail[i]| A_BOLD);
                }
                if(direction == DOWN){
                    wmove(my_win, pos_x-i-1, pos_y);
                    waddch(my_win,tail[i]| A_BOLD);
                }
                if(direction == LEFT){
                    wmove(my_win, pos_x, pos_y+i+1);
                    waddch(my_win,tail[i]| A_BOLD);
                }
                if(direction == RIGHT){
                    wmove(my_win, pos_x, pos_y-i-1);
                    waddch(my_win,tail[i]| A_BOLD);
                }
            }
            wrefresh(my_win);	
            zmq_send(responder, "OK", 3, 0); 
        }

        for(int i=0; i < (sizeof(array_roaches)/sizeof(array_roaches[0]));i++){
            zmq_recv (responder, &array_roaches[i], sizeof(cockroaches_t), 0);
            if(array_roaches[i].msg_type == 0){
                ch = array_roaches[i].ch;
                pos_x = WINDOW_SIZE/2;
                pos_y = WINDOW_SIZE/2;

                //STEP 3
                char_data[n_chars].ch = ch;
                char_data[n_chars].pos_x = pos_x;
                char_data[n_chars].pos_y = pos_y;
                n_chars++;
            }
            if(array_roaches[i].msg_type == 1){
                //STEP 4
                int ch_pos = find_ch_info(char_data, n_chars, array_roaches[i].ch);
                if(ch_pos != -1){
                    pos_x = char_data[ch_pos].pos_x;
                    pos_y = char_data[ch_pos].pos_y;
                    ch = char_data[ch_pos].ch;
                    /*deletes old place */
                    wmove(my_win, pos_x, pos_y);
                    waddch(my_win,' ');

                    /* calculates new direction */
                    direction = array_roaches[i].direction;

                    /* calculates new mark position */
                    new_position(&pos_x, &pos_y, direction);
                    char_data[ch_pos].pos_x = pos_x;
                    char_data[ch_pos].pos_y = pos_y;
                }      
                wmove(my_win, pos_x, pos_y);
                waddch(my_win,ch| A_BOLD);
                wrefresh(my_win);	
                zmq_send(responder, "OK", 3, 0);  
            }
        }
    }
  	endwin();			/* End curses mode		  */

    zmq_close(responder);
    zmq_ctx_destroy(context);

	return 0;
}
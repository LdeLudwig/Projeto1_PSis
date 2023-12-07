#include <zmq.h>
#include <ncurses.h>
#include "structs.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <stdlib.h>
#include <string.h>

#define WINDOW_SIZE 30
#define BUFFER_SIZE 1024



// STEP 1
typedef struct ch_info_t
{
    int ch;
    int pos_x, pos_y;
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

    int buffer_roach[BUFFER_SIZE];
    char buffer_lizard[BUFFER_SIZE];

    //Ex 1 - Lab 5:
    //  Socket to talk to clients
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (responder, "tcp://*:5555");


    lizard_t array_lizard[26];
    cockroaches_t array_roaches[10];
    
	initscr();		    	
	cbreak();				
    keypad(stdscr, TRUE);   
	noecho();			    

    // creates a window and draws a border 
    WINDOW * my_win = newwin(WINDOW_SIZE, WINDOW_SIZE, 0, 0);
    box(my_win, 0 , 0);	
	wrefresh(my_win);

    //lizard and roach content
    int lizard_ch;
    int roach_num;

    //lizard and roach position
    int lizard_pos_x;
    int lizard_pos_y;
    
    int roach_pos_x;
    int roach_pos_y;

    direction  direction;
    while (1)
    {
    
    for(int i=0; i<(sizeof(array_lizard)/sizeof(array_lizard[0]));i++){
        zmq_recv (responder, &buffer_lizard, sizeof(BUFFER_SIZE), 0);
        

        if(array_lizard[i].Message == LIZARD_CONNECT){
            lizard_ch = array_lizard[i].ch;
            lizard_pos_x = WINDOW_SIZE/2;
            lizard_pos_y = WINDOW_SIZE/2;

            //STEP 3
            char_data[n_chars].ch = lizard_ch;
            char_data[n_chars].pos_x = lizard_pos_x;
            char_data[n_chars].pos_y = lizard_pos_y;
            n_chars++;
        }
        if(array_lizard[i].Message == LIZARD_MOVEMENT){
            //STEP 4
            int ch_pos = find_ch_info(char_data, n_chars, array_lizard[i].ch);
            if(ch_pos != -1){
                lizard_pos_x = char_data[ch_pos].pos_x;
                lizard_pos_y = char_data[ch_pos].pos_y;
                lizard_ch = char_data[ch_pos].ch;
                //deletes old place 
                wmove(my_win, lizard_pos_x, lizard_pos_y);
                waddch(my_win,' ');

                // claculates new direction 
                direction = array_lizard[i].direction;

                //claculates new mark position 
                new_position(&lizard_pos_x, &lizard_pos_y, direction);
                char_data[ch_pos].pos_x = lizard_pos_x;
                char_data[ch_pos].pos_y = lizard_pos_y;

            }        
        }
        // draw mark on new position 
        wmove(my_win, lizard_pos_x, lizard_pos_y);
        waddch(my_win,lizard_ch| A_BOLD);
        wrefresh(my_win);	
        zmq_send(responder, "OK", 3, 0); 
    }


    for(int i=0; i<(sizeof(array_roaches)/sizeof(array_roaches[0]));i++){
        zmq_recv (responder, &buffer_roach, sizeof(BUFFER_SIZE), 0);
        
        if(array_roaches[i].Message == ROACH_CONNECT){
            roach_num = array_roaches[i].num;
            roach_pos_x = WINDOW_SIZE/3;
            roach_pos_y = WINDOW_SIZE/3;

            //STEP 3
            char_data[n_chars].ch = roach_num;
            char_data[n_chars].pos_x = roach_pos_x;
            char_data[n_chars].pos_y = roach_pos_y;
            n_chars++;
        }
        if(array_roaches[i].Message == ROACH_MOVEMENT){
            //STEP 4
            int ch_pos = find_ch_info(char_data, n_chars, array_roaches[i].num);
            if(ch_pos != -1){
                roach_pos_x = char_data[ch_pos].pos_x;
                roach_pos_y = char_data[ch_pos].pos_y;
                roach_num = char_data[ch_pos].ch;
                //deletes old place 
                wmove(my_win, roach_pos_x, roach_pos_y);
                waddch(my_win,' ');

                //claculates new direction 
                direction = array_roaches[i].direction;

                // claculates new mark position 
                new_position(&roach_pos_x, &roach_pos_y, direction);
                char_data[ch_pos].pos_x = roach_pos_x;
                char_data[ch_pos].pos_y = roach_pos_y;

            }        
        }
        // draw mark on new position 
        wmove(my_win, roach_pos_x, roach_pos_y);
        wprintw(my_win, "%d", roach_num);
        wrefresh(my_win);	
        zmq_send(responder, "OK", 3, 0); 
    }
        
    }
  	endwin();			

    zmq_close(responder);
    zmq_ctx_destroy(context);

	return 0;
}
#include <ncurses.h>
#include "structs.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <stdlib.h>
#include <assert.h>
#include <zmq.h>

#define WINDOW_SIZE 15 

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

    lizard_t m;
	void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REP);
    int rc = zmq_bind(socket, "ipc://tmp/s1");
    assert(rc == 0);


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


    direction  direction;
    while (1)
    {
        zmq_recv(socket, &m, sizeof(lizard_t), 0);
        int ch_pos = find_ch_info(char_data, n_chars, m.ch);
        if(m.msg_type == 0){
            if(ch_pos < 0)
            {
                ch = m.ch;
                pos_x = WINDOW_SIZE/2;
                pos_y = WINDOW_SIZE/2;

                //STEP 3
                char_data[n_chars].ch = ch;
                char_data[n_chars].pos_x = pos_x;
                char_data[n_chars].pos_y = pos_y;
                n_chars++;
            }    
        }
        if(m.msg_type == 1){
            //STEP 4
            for(int i = 0; i < n_chars; i++){
                wmove(my_win, char_data[i].pos_x, char_data[i].pos_y);
                waddch(my_win,char_data[i].ch| A_BOLD);
                wrefresh(my_win);	
                zmq_send(socket, "OK", 2, 0);
            }
            if(ch_pos != -1){
                pos_x = char_data[ch_pos].pos_x;
                pos_y = char_data[ch_pos].pos_y;
                ch = char_data[ch_pos].ch;
                /*deletes old place */
                wmove(my_win, pos_x, pos_y);
                waddch(my_win,' ');

                /* claculates new direction */
                direction = m.direction;

                /* claculates new mark position */
                new_position(&pos_x, &pos_y, direction);
                char_data[ch_pos].pos_x = pos_x;
                char_data[ch_pos].pos_y = pos_y;

            }        
        }
        /* draw mark on new position */
        wmove(my_win, pos_x, pos_y);
        waddch(my_win,ch| A_BOLD);
        wrefresh(my_win);	
        zmq_send(socket, "OK", 3, 0);
    }
  	endwin();			/* End curses mode		  */

	return 0;
}
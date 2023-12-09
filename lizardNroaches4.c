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


typedef struct ch_info_t
{
    int ch;
    int pos_x, pos_y;
} ch_info_t;

/*-------------- find informations about content in each client --------------*/
int find_ch_info(ch_info_t char_data[], int n_chars, int ch){

    for (int i = 0 ; i < n_chars; i++){
        if(ch == char_data[i].ch){
            return i;
        }
    }
    return -1;
}

/*--------------------- new position calculator function ---------------------*/
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

/*--------------------- child function to lizard process ---------------------*/
void client_handler_lizard(void* responder, ch_info_t char_data[], int n_chars, WINDOW* my_win ){
    lizard_t array_lizards[26];
    int ch;
    int lizard_pos_x, lizard_pos_y, tail_x, tail_y;
    direction direction;

    //Drawing the tail:
    char tail[6];
    strcpy(tail, ".....");

    while(1){
        //whole process to lizard creation and movement
        
        for(int i =0; i < (sizeof(array_lizards)/sizeof(array_lizards[0])); i++){
             zmq_recv(responder, &array_lizards[i], sizeof(lizard_t),0);

            if(array_lizards[i].msg_type == LIZARD_CONNECT){
                ch = array_lizards[i].ch;
                lizard_pos_x = WINDOW_SIZE/2;
                lizard_pos_y = WINDOW_SIZE/2;
                //STEP 3
                char_data[n_chars].ch = ch;
                char_data[n_chars].pos_x = lizard_pos_x;
                char_data[n_chars].pos_y = lizard_pos_y;
                n_chars++;
            }


            if(array_lizards[i].msg_type == LIZARD_MOVEMENT){
                //STEP 4
                int ch_pos = find_ch_info(char_data, n_chars, array_lizards[i].ch);


                if(ch_pos != -1){
                    lizard_pos_x = char_data[ch_pos].pos_x;
                    lizard_pos_y = char_data[ch_pos].pos_y;
                    ch = char_data[ch_pos].ch;
                    //deletes old place 
                    wmove(my_win, lizard_pos_x, lizard_pos_y);
                    waddch(my_win,' ');


                    for(int i = 0; i < sizeof(tail)/sizeof(tail[0]); i++){
                        tail_x = lizard_pos_x;
                        tail_y = lizard_pos_y;


                        if(direction == UP){
                            tail_x += i + 1;
                            wmove(my_win, tail_x, tail_y);
                            waddch(my_win,' ');
                        }
                        if (direction == DOWN) {
                            tail_x -= i + 1;
                            wmove(my_win, tail_x, tail_y);
                            waddch(my_win,' ');
                        }
                        if (direction == LEFT) {
                            tail_y += i + 1;
                            wmove(my_win, tail_x, tail_y);
                            waddch(my_win,' ');
                        }
                        if (direction == RIGHT) {
                            tail_y -= i + 1;
                            wmove(my_win, tail_x, tail_y);
                            waddch(my_win,' ');
                        }
                    }

                    // calculates new direction 
                    direction = array_lizards[i].direction;

                    // calculates new mark position 
                    new_position(&lizard_pos_x, &lizard_pos_y, direction);
                    char_data[ch_pos].pos_x = lizard_pos_x;
                    char_data[ch_pos].pos_y = lizard_pos_y;
                }        
            }
            for (int i = 0; i < sizeof(tail)/sizeof(tail[0]) - 1; i++) {
                tail_x = lizard_pos_x;
                tail_y = lizard_pos_y;
                if (direction == UP) {
                    tail_x += i + 1;
                    wmove(my_win, tail_x, lizard_pos_y);
                    waddch(my_win, tail[i] | A_BOLD);
                } else if (direction == DOWN) {
                    tail_x -= i + 1;
                    wmove(my_win, tail_x, lizard_pos_y);
                    waddch(my_win, tail[i] | A_BOLD);
                } else if (direction == LEFT) {
                    tail_y += i + 1;
                    wmove(my_win, lizard_pos_x, tail_y);
                    waddch(my_win, tail[i] | A_BOLD);
                } else if (direction == RIGHT) {
                    tail_y -= i + 1;
                    wmove(my_win, lizard_pos_x, tail_y);
                    waddch(my_win, tail[i] | A_BOLD);
                }
            }

            // draw mark on new position 
            wmove(my_win, lizard_pos_x, lizard_pos_y);
            waddch(my_win,ch| A_BOLD);
            wrefresh(my_win);	
            zmq_send(responder, "OK", 3, 0); 
        } 
    }
    zmq_close(responder);
}

/*------------------- child function to cockroaches process ------------------*/
/* void client_handles_roaches(void* responder){
    cockroaches_t array_roaches[10];
    int roach_pos_x, roach_pos_y;
    direction direction;

    while(1){
        //whole process to cockroaches creation and movement

    }
}
 */
/*--------------------------------------------------- MAIN FUNCTION -------------------------------------------------------*/
int main(){
    //char_data handler
    ch_info_t char_data[100];
    int n_chars = 0;

    //Context creation
    void* context = zmq_ctx_new();
    
    pid_t lizard_pid, cockroaches_pid;

    //Initializing curses mode  
    initscr();		    	
	cbreak();				
    keypad(stdscr, TRUE);   
	noecho();			    

    //creates a window and draws a border
    WINDOW * my_win = newwin(WINDOW_SIZE, WINDOW_SIZE, 0, 0);
    box(my_win, 0 , 0);	
	wrefresh(my_win);

    

    /* if((cockroaches_pid = fork()) == 0){
        void* responder2 = zmq_socket(context, ZMQ_REP);
        zmq_bind(responde2, "tcp://*5556");

    }
 */

    

    while (1)
    {
        if((lizard_pid = fork()) == 0){
        void* responder1 = zmq_socket(context, ZMQ_REP);
        zmq_bind(responder1, "tcp://*5555");
        
        //call lizard_client_handler function
        client_handler_lizard(responder1, char_data, n_chars, my_win);

        }
    
    }
    endwin();			// End curses mode

    
    zmq_ctx_destroy(context);

}
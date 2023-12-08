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
#define MAX_LIZARDS 26
#define MAX_ROACHES 50


// STEP 1
typedef struct ch_info_t
{
    int ch;
    int pos_x, pos_y;
} ch_info_t;

typedef struct {
    lizard_t array[MAX_LIZARDS];
    int size;
} lizard_array_t;

typedef struct {
    cockroaches_t array[MAX_ROACHES];
    int size;
} cockroaches_array_t;


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


void append_lizard(lizard_array_t *lizard_array, lizard_t new_lizard) {
    if (lizard_array->size < MAX_LIZARDS) {
        lizard_array->array[lizard_array->size] = new_lizard;
        lizard_array->size++;
    } else {
        // Lidar com a condição de array cheio
        printf("Erro: Array de lagartos cheio\n");
    }
}

void append_roach(cockroaches_array_t *roach_array, cockroaches_t new_roach) {
    if (roach_array->size < MAX_ROACHES) {
        roach_array->array[roach_array->size] = new_roach;
        roach_array->size++;
    } else {
        // Lidar com a condição de array cheio
        printf("Erro: Array de baratas cheio\n");
    }
}

int main()
{	 
    ch_info_t char_data[100];
    int n_chars = 0;

    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (responder, "tcp://*:5555");
    
    cockroaches_array_t array_roaches = {.size = 0};
    lizard_array_t array_lizards = {.size = 0};

    initscr();		    	
	cbreak();				
    keypad(stdscr, TRUE);   
	noecho();			    

    //Creates a window and draws a border
    WINDOW * my_win = newwin(WINDOW_SIZE, WINDOW_SIZE, 0, 0);
    box(my_win, 0 , 0);	
	wrefresh(my_win);

    int ch;
    int pos_x;
    int pos_y;
    int tail_x;
    int tail_y;
    //Drawing the tail:
    char tail[6];
    strcpy(tail, ".....");
    

    direction  direction;
    species species;
    while (1)
    {
        zmq_recv(responder, &species, sizeof(species), 0);

        if(species == LIZARD){
            zmq_send(responder, "OK", 3, 0);
            lizard_t new_lizard;
            zmq_recv(responder, &new_lizard, sizeof(new_lizard), 0);
            append_lizard(&array_lizards, new_lizard);
        }
        else{
            zmq_send(responder, "OK", 3, 0);
            cockroaches_t new_roach;
            zmq_recv(responder, &new_roach, sizeof(new_roach), 0);
            append_roach(&array_roaches, new_roach);
        }

        for(int i=0; i < array_lizards.size - 1;i++){
            printf("\nDentro do loop msg type: %d\nchar: %d\n", array_lizards.array[i].msg_type, array_lizards.array[i].ch);
            if(array_lizards.array[i].msg_type == LIZARD_CONNECT){
                printf("ENTREI ENTREI ENTREI!\nLIZARD MSG TYPE 0\n");
                ch = array_lizards.array[i].ch;
                pos_x = WINDOW_SIZE/2;
                pos_y = WINDOW_SIZE/2;
                //STEP 3
                char_data[n_chars].ch = ch;
                char_data[n_chars].pos_x = pos_x;
                char_data[n_chars].pos_y = pos_y;
                n_chars++;
            }
            if(array_lizards.array[i].msg_type == LIZARD_MOVEMENT){
                printf("ENTREI ENTREI ENTREI!\nLIZARD MSG TYPE 1\n");
                int ch_pos = find_ch_info(char_data, n_chars, array_lizards.array[i].ch);
                if(ch_pos != -1){
                    pos_x = char_data[ch_pos].pos_x;
                    pos_y = char_data[ch_pos].pos_y;
                    ch = char_data[ch_pos].ch;
                    //deletes old place 
                    wmove(my_win, pos_x, pos_y);
                    waddch(my_win,' ');
                    for(int i = 0; i < sizeof(tail)/sizeof(tail[0]); i++){
                        tail_x = pos_x;
                        tail_y = pos_y;
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
                    direction = array_lizards.array[i].direction;

                    // calculates new mark position 
                    new_position(&pos_x, &pos_y, direction);
                    char_data[ch_pos].pos_x = pos_x;
                    char_data[ch_pos].pos_y = pos_y;
                }   
                for (int i = 0; i < sizeof(tail)/sizeof(tail[0]) - 1; i++) {
                    tail_x = pos_x;
                    tail_y = pos_y;
                    //printf("UEPA!\n");
                    if (direction == UP) {
                        tail_x += i + 1;
                        wmove(my_win, tail_x, pos_y);
                        waddch(my_win, tail[i] | A_BOLD);
                    } else if (direction == DOWN) {
                        tail_x -= i + 1;
                        wmove(my_win, tail_x, pos_y);
                        waddch(my_win, tail[i] | A_BOLD);
                    } else if (direction == LEFT) {
                        tail_y += i + 1;
                        wmove(my_win, pos_x, tail_y);
                        waddch(my_win, tail[i] | A_BOLD);
                    } else if (direction == RIGHT) {
                        tail_y -= i + 1;
                        wmove(my_win, pos_x, tail_y);
                        waddch(my_win, tail[i] | A_BOLD);
                    }
                }
                // draw mark on new position 
                wmove(my_win, pos_x, pos_y);
                waddch(my_win,ch| A_BOLD);
                wrefresh(my_win);	
                zmq_send(responder, "OK", 3, 0);      
            }
        }
    
        for(int i=0; i < array_roaches.size - 1;i++){
            if(array_roaches.array[i].msg_type == ROACH_CONNECT){
                ch = array_roaches.array[i].ch;
                pos_x = WINDOW_SIZE/2;
                pos_y = WINDOW_SIZE/2;

                //STEP 3
                char_data[n_chars].ch = ch;
                char_data[n_chars].pos_x = pos_x;
                char_data[n_chars].pos_y = pos_y;
                n_chars++;
            }
            if(array_roaches.array[i].msg_type == ROACH_MOVEMENT){
                //STEP 4
                printf("\nENTROU ENTROU NA COCKROACHES MSG TYPE 1");
                int ch_pos = find_ch_info(char_data, n_chars, array_roaches.array[i].ch);
                if(ch_pos != -1){
                    pos_x = char_data[ch_pos].pos_x;
                    pos_y = char_data[ch_pos].pos_y;
                    ch = char_data[ch_pos].ch;
                    //deletes old place 
                    wmove(my_win, pos_x, pos_y);
                    waddch(my_win,' ');

                    // calculates new direction
                    direction = array_roaches.array[i].direction;

                    // calculates new mark position 
                    new_position(&pos_x, &pos_y, direction);
                    char_data[ch_pos].pos_x = pos_x;
                    char_data[ch_pos].pos_y = pos_y;
                }       
            }
            wmove(my_win, pos_x, pos_y);
            waddch(my_win,ch| A_BOLD);
            wrefresh(my_win);	
            zmq_send(responder, "OK", 3, 0); 
        }
    }
    endwin();			// End curses mode

    zmq_close(responder);
    zmq_ctx_destroy(context);
    return 0;
}
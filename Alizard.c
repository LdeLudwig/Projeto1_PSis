#include <zmq.h>
#include <ncurses.h>
#include "structs.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define WINDOW_SIZE 30
#define MAX_LIZARDS 26
#define MAX_ROACHES (WINDOW_SIZE * WINDOW_SIZE / 3)


typedef struct ch_info_t
{
    int ch;
    int pos_x, pos_y;
} ch_info_t;

//Lizard structures:
typedef struct LizardNode {
    lizard_t lizard;
    struct LizardNode* next;
} LizardNode;

typedef struct {
    LizardNode* head;
    int size;
} LizardList;

//Cockroach structures:
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


//LIZARD FUNCTIONS:

void append_lizard2(LizardList* lizardList, lizard_t* new_lizard) {
    if (lizardList->size < MAX_LIZARDS) {
        LizardNode* newNode = (LizardNode*)malloc(sizeof(LizardNode));
        if (newNode == NULL) {
            perror("Erro ao alocar memória para o nó");
            exit(EXIT_FAILURE);
        }

        newNode->lizard = *new_lizard;
        newNode->next = NULL;

        if (lizardList->head == NULL) {
            lizardList->head = newNode;
        } else {
            LizardNode* current = lizardList->head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
        }

        lizardList->size++;
    }
    else{
        printf("Erro: max numero de lagartos atingido\n");
    }
}

// Função para liberar a memória alocada para a lista encadeada
void freeLizardList(LizardList* lizardList) {
    LizardNode* current = lizardList->head;
    while (current != NULL) {
        LizardNode* temp = current;
        current = current->next;
        free(temp);
    }

    lizardList->head = NULL;
    lizardList->size = 0;
}

void update_lizard2(LizardList* lizardList, lizard_t* new_lizard) {
    LizardNode* current = lizardList->head;

    while (current != NULL) {
        if (current->lizard.ch == new_lizard->ch) {
            current->lizard = *new_lizard;
            return;  // Se encontrou e atualizou, podemos sair da função
        }

        current = current->next;
    }
}

//ROACH FUNCTIONS:
void append_roach(cockroaches_array_t *roach_array, cockroaches_t *new_roach) {
    if (roach_array->size < MAX_ROACHES) {
        roach_array->array[roach_array->size] = *new_roach;
        roach_array->size++;
    } else {
        // Lidar com a condição de array cheio
        printf("Erro: Array de baratas cheio\n");
    }
}

void update_roach(cockroaches_array_t *roach_array, cockroaches_t *new_roach){
    for(int i =0; i<roach_array->size; i++){
        if(roach_array->array[i].ch == new_roach->ch){
            roach_array->array[i] = *new_roach;
        }
    }
}


int main()
{	 
    ch_info_t char_data[100];
    int n_chars = 0;

    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (responder, "tcp://*:5555");
    assert (rc == 0);

    cockroaches_array_t array_roaches = {.size = 0};
    LizardList lizardList = {.head = NULL, .size = 0};


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
        zmq_send(responder, "OK", 3, 0);
        
        if(species == LIZARD){
            lizard_t new_lizard;
            zmq_recv(responder, &new_lizard, sizeof(new_lizard), 0);
            
            //Check if the lizard is in the array
            int lizard_pos = find_ch_info(char_data, n_chars, new_lizard.ch);
            if (lizard_pos == -1) {
                append_lizard2(&lizardList, &new_lizard);
            }
        }
        else{
            cockroaches_t new_roach;
            zmq_recv(responder, &new_roach, sizeof(new_roach), 0);
            
            //Check if the roach is in the array
            int roach_pos = find_ch_info(char_data, n_chars, new_roach.ch);

            if (roach_pos == -1) {
                //If the roach is not in the array, it will be added
                append_roach(&array_roaches, &new_roach);
            }
            //printf("\nmsg type: %d\nEndereço de memoria: %p", new_roach.msg_type, &new_roach);
            for(int i=0;i < array_roaches.size;i++){
                update_roach(&array_roaches, &new_roach);
            }
        }

        for (LizardNode* current = lizardList.head; current != NULL; current = current->next) {
            if(current->lizard.msg_type == LIZARD_CONNECT){
                //printf("ENTREI ENTREI ENTREI!\nLIZARD MSG TYPE 0\n");
                ch = current->lizard.ch;
                pos_x = WINDOW_SIZE/2;
                pos_y = WINDOW_SIZE/2;
                //STEP 3
                char_data[n_chars].ch = ch;
                char_data[n_chars].pos_x = pos_x;
                char_data[n_chars].pos_y = pos_y;
                n_chars++;
            }
            if(current->lizard.msg_type == LIZARD_MOVEMENT){
                //printf("ENTREI ENTREI ENTREI!\nLIZARD MSG TYPE 1\n%d", species);
                int ch_pos = find_ch_info(char_data, n_chars, current->lizard.ch);
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
                    direction = current->lizard.direction;

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
                //zmq_send(responder, "OK", 3, 0);      
            }
            if(current->lizard.msg_type == LIZARD_DISCONNECT){
                zmq_send(responder, "OK", 3, 0);
            }
            zmq_send(responder, "OK", 3, 0);
            zmq_recv(responder, &current->lizard, sizeof(current->lizard), 0);
        }
    
        for(int i=0; i < array_roaches.size;i++){
            //printf("TESTANDO msg_type %dEndereço de memoria: %p\n", array_roaches.array[i].msg_type, &array_roaches.array[i]);
            //printf("\nUHUUL UPDATE!  %d\nEndereço de memoria: %p\n", array_roaches.array[i].msg_type, &array_roaches.array[i]);
            if(array_roaches.array[i].msg_type == ROACH_CONNECT){
               // printf("\nENTROU ENTROU NA COCKROACHES MSG TYPE: %d", array_roaches.array[i].msg_type);
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
                //printf("\nENTROU ENTROU NA COCKROACHES MSG TYPE 3");
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
            //zmq_send(responder, "OK", 3, 0); 
        }
        zmq_send(responder, "OK", 3, 0);
    }
    endwin();			// End curses mode

    zmq_close(responder);
    zmq_ctx_destroy(context);
    return 0;
}
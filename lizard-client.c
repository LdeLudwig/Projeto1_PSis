#include <stdio.h>
#include <zmq.h>
#include <ncurses.h>
#include "structs.h"

int main(){
    //criando lizard
    lizard_t lizard;

    //ZeroQM context
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REQ);
    zmq_connect (socket, "ipc://tmp/s1");

    //usuário definindo o char.
    do{
        printf("Digite o caractere (a..z): ");
        lizard.ch = getchar();
        lizard.ch = tolower(lizard.ch);  
    } while(!isalpha(lizard.ch));


    //adicionando letra e pontos dentro do array lizard
    lizard.arr[0] = lizard.ch;
    for (int i = 1; i < sizeof(lizard.arr); i++){
        lizard.arr[i] = '.';
    }

    //mensagme de conexão
    lizard.msg_type = 0;
    zmq_send(socket, &lizard, sizeof(lizard_t), 0);


    

    
	initscr();			/* Start curses mode 		*/
	cbreak();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */
   

    //criar direção do ncurses aqui!
    int ch;
    int n = 0;
    do
    {
    	ch = getch();		
        n++;
        switch (ch)
        // definindo direção para o ncurses
        {
            case KEY_LEFT:
                mvprintw(0,0,"%d Left arrow is pressed", n);
                lizard.direction = LEFT;
                break;
            case KEY_RIGHT:
                mvprintw(0,0,"%d Right arrow is pressed", n);
                lizard.direction = RIGHT;
                break;
            case KEY_DOWN:
                mvprintw(0,0,"%d Down arrow is pressed", n);
                lizard.direction = DOWN;
                break;
            case KEY_UP:
                mvprintw(0,0,"%d Up arrow is pressed", n);
                lizard.direction = UP;
                break;
            default:
                ch = 'x';
                    break;
        }
        refresh(); 

        //TODO_9
        // prepare the movement message
        
        

        //TODO_10
        //send the movement message
        


    }while(ch != 81);

    
}
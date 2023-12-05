#include <stdio.h>
#include <zmq.h>
#include <ncurses.h>
#include "structs.h"
#include <ctype.h> 

int main(){

    //defining answer
    char answer[50];

    //criando lizard
    lizard_t lizard;

    //ZeroQM context
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_REQ);
    zmq_connect (socket, "tcp://localhost:5555");

    char ch;
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
    lizard.msg_type = 1;
    zmq_send(socket, &lizard, sizeof(lizard_t), 0);
    zmq_recv(socket, answer, sizeof(answer), 0);
    
	initscr();			/* Start curses mode 		*/
	cbreak();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */
   

    //definindo tipo de mensagem para movimento
    lizard.msg_type = 1;
    
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
        if (ch != 'x'){
            zmq_send(socket, &lizard, sizeof(lizard_t), 0);
            zmq_recv(socket, answer, sizeof(answer), 0);
        }
        
    }while(ch != 81);

     endwin();
    zmq_close(socket);
    zmq_ctx_destroy(context);

}
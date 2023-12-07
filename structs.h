typedef enum direction {UP, DOWN, LEFT, RIGHT} direction;
typedef enum message {LIZARD_CONNECT, LIZARD_MOVEMENT, ROACH_CONNECT, ROACH_MOVEMENT} message;


typedef struct lizard_t
{   
    char ch;
    char arr[5];
    message Message;
    direction direction ;

}lizard_t;

typedef struct cockroaches_t
{   
    int num; 
    direction direction ;
    message Message;
    /* data */
}cockroaches_t;

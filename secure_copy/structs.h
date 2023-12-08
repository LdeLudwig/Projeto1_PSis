typedef enum direction {UP, DOWN, LEFT, RIGHT} direction;
typedef enum message {LIZARD_CONNECT, LIZARD_MOVEMENT, ROACH_CONNECT, ROACH_MOVEMENT} message;

typedef struct lizard_t
{   
    message msg_type; 
    char ch;
    char tail[5];
    direction direction ;

}lizard_t;

typedef struct cockroaches_t
{   
    message msg_type; 
    char ch; 
    direction direction ;
    /* data */
}cockroaches_t;
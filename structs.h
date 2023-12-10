typedef enum direction {UP, DOWN, LEFT, RIGHT} direction;
typedef enum message {LIZARD_CONNECT, LIZARD_MOVEMENT, ROACH_CONNECT, ROACH_MOVEMENT, LIZARD_DISCONNECT} message;
typedef enum species {LIZARD, ROACH} species;

typedef struct lizard_t
{   
    message msg_type; 
    char ch;
    char tail[6];
    direction direction;
    species species_type;
    /* data */
}lizard_t;

typedef struct cockroaches_t
{   
    message msg_type; 
    int num; 
    direction direction ;
    species species_type;
    /* data */
}cockroaches_t;

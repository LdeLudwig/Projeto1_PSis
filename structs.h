typedef enum direction {UP, DOWN, LEFT, RIGHT} direction;
typedef enum message {LIZARD_CONNECT, LIZARD_MOVEMENT, ROACH_CONNECT, ROACH_MOVEMENT, LIZARD_DISCONNECT, FIELD_UPDATE} message;
typedef enum species {LIZARD, ROACH} species;

typedef struct lizard_t
{   
    message msg_type; 
    char ch;
    char tail[6];
    direction direction;
    species species_type;
    int pos_x;
    int pos_y;
    /* data */
}lizard_t;

typedef struct cockroaches_t
{   
    message msg_type; 
    char ch;
    int num; 
    direction direction ;
    species species_type;
    /* data */
}cockroaches_t;

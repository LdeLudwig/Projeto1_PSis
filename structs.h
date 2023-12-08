typedef enum direction {UP, DOWN, LEFT, RIGHT} direction;
typedef enum message {LIZARD_CONNECT, LIZARD_MOVEMENT, ROACH_CONNECT, ROACH_MOVEMENT} message;
typedef enum species {LIZARD, ROACH} species;

typedef struct lizard_t
{   
    message msg_type; 
    char ch;
    char tail[6];
    direction direction;
    species species_type;

}lizard_t;

typedef struct cockroaches_t
{   
    message msg_type; 
    char ch; 
    direction direction ;
    species species_type;
    /* data */
}cockroaches_t;

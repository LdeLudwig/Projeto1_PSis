typedef enum direction {UP, DOWN, LEFT, RIGHT} direction;

typedef struct lizard_t
{   
    int msg_type; /* 0 join   1 - move */
    char ch;
    char tail[5];
    direction direction ;

}lizard_t;

typedef struct cockroaches_t
{   
    int msg_type; /* 0 join   1 - move */
    char ch; 
    direction direction ;
    /* data */
}cockroaches_t;

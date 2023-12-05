typedef enum direction {UP, DOWN, LEFT, RIGHT} direction;

typedef struct lizard_t
{   
    int msg_type; /* 0 join   1 - move */
    char ch;
    char arr[5];
    direction direction ;
    /* data */
}lizard_t;

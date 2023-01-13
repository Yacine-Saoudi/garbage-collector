typedef enum{
    OBJ_INT,
    OBJ_PAIR
} ObjectType;

typedef struct  sObject
{
    ObjectType type;
    struct sObject* next;
    unsigned char marked;
    union{
        int value;

        struct{
            struct sObject* head;
            struct sObject* tail;
        };
    };
} Object;

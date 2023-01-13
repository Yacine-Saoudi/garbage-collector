#include "object.h"
#define STACK_MAX 256
#define GC_THRESHOLD 5

typedef struct {
    Object* stack[STACK_MAX];
    Object* head;
    int currNum; //current amount of allocated objects
    int maxNum; //max amount of allocated objects
    int size;
} VM;
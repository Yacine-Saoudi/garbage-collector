#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// forward declare functions
VM* newVM();
Object* newObject(VM* vm, ObjectType type);
void push(VM* vm, Object* value);
Object* pop(VM* vm);
void pushInt(VM* vm, int val);
Object* pushPair(VM* vm);
void markAll(VM* vm);
void mark(Object* object);
void sweep(VM* vm);
void gcollect(VM* vm);
void noScope(VM* vm);


// construct new VM
VM* newVM(){
    VM* vm = malloc(sizeof(VM));
    vm->head = NULL;
    vm->size = 0;
    vm->currNum = 0;
    vm->maxNum = GC_THRESHOLD;
    return vm;
}

// construct new object
Object* newObject(VM* vm , ObjectType type){
    if(vm->currNum == vm->maxNum) {
        gcollect(vm);
    }

    printf("creating new object\n");

    Object* object = malloc(sizeof(Object));
    object->type = type;
    object->marked = 0;

    object->next = vm->head; //take new object and set it to the head of the VM LL
    vm->head = object;

    vm->currNum++;

    return object;
}

// push object to VM stack
void push(VM* vm, Object* value){
    assert(vm->size < STACK_MAX); //stack overflow
    vm->stack[vm->size++] = value;
}

// pop object from VM stack
Object* pop(VM* vm){
    assert(vm->size > 0); // stack underflow
    Object* res = vm->stack[--vm->size];
    return res;
}

// create new int object then push onto VM stack
void pushInt(VM* vm, int val){
    Object* object = newObject(vm, OBJ_INT);
    object->value = val;
    push(vm, object);
}

void markAll(VM* vm){
    for(int i = 0; i < vm->size; i++){
        mark(vm->stack[i]);
    }
}

void mark(Object* object){
    if(object->marked) return;

    object->marked = 1;

    if(object->type == OBJ_PAIR){
        mark(object->head);
        mark(object->tail);
    }
}

// create new pair object, then push onto VM stack
Object* pushPair(VM* vm){
    Object* object = newObject(vm, OBJ_PAIR); 

    object->tail = pop(vm); //take 2 objects from vm stack and
    object->head = pop(vm); //assign them to object to initialize

    push(vm, object);
    return object;
}  

void sweep(VM* vm){
    Object** object = &vm->head;
    while(*object){
        if(!(*object)->marked){ //if object is not marked free its data;
            Object*  unmarked = *object;
            vm->currNum--; 
            *object = unmarked->next;
            free(unmarked);
        }
        else{ //object is marked so unmark it for next sweep 
            (*object)->marked = 0;
            object = &(*object)->next;
        }
    }
}

void gcollect(VM* vm){
    printf("clearing object stack\n");

    markAll(vm);
    sweep(vm);
    vm->maxNum = vm->currNum*2;
}

void noScope(VM* vm){
    for (int i = 0; i < 5; i++){
        Object* leakObj = newObject(vm, OBJ_INT);
    }
    
}

int main(){
    VM* vm  = newVM(); //create new VM object
    Object* obj1 = newObject(vm, OBJ_INT);
    Object* obj2 = newObject(vm, OBJ_INT);
    noScope(vm); //create out of scope objects to test 

    return 0;
}
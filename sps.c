#include <stdlib.h>
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <getopt.h>

/* Definitions */
typedef enum {
    Ok = 0,
    UnexpectedErr,
    ArgErr,
    AllocErr,
    FileErr
} Status;

typedef struct{
    char **selector;
    Status (*function)();
    char **args;
}command;

typedef struct{
   char **array;
   int size;
} argsArray;


/* Prototypes */
void initArgsArray(argsArray *a);
void initCommand(command *c);
void delCmd(command *c);
void printUsage();
Status processRemainingArgs(int count, char **args, argsArray *argsA);
Status getArgs(int count, char **args, argsArray *array, char *delim);
void cleanUp(argsArray *a);
void delArgsArray();

/* Code */
void
initArgsArray(argsArray *a){
    a->array=NULL;
    a->size=0;
}
void
initCommad(command *c){
    c->selector=NULL;
    c->function=NULL;
    c->args=NULL;
}
void
delCmd(command *c){
    free(c->selector);
    c->selector = NULL;

    free(c->args);
    c->args = NULL;
}
void
printUsage(){
    printf("./sps [-d DELIM] CMD_SEQUENCE FILE\n\r");
}
Status
processRemainingArgs(int count, char **args, argsArray *argsA){
        initArgsArray(argsA);

        argsA->array = malloc(count * sizeof(void*));

        if (argsA->array == NULL)
            return AllocErr;

        for (int i = optind, j = 0; i<count; i++,j++){
            asprintf(&argsA->array[j], "%s", args[i]);

            if(argsA->array[j] == NULL)
                return AllocErr;

            argsA->size++;
        }

        if (argsA->size>2){
            printUsage(); 
            return ArgErr;
        }

        return Ok;
}
Status
getArgs(int count, char **args, argsArray *array, char *delim){
    int opt; Status result = UnexpectedErr;

    while ((opt = getopt(count, args, "d:")) != -1 ){
        switch(opt){
            case 'd':
                *delim = optarg[0];
                break;

            default:
                printUsage();
                result = ArgErr;
                break;
        }
    }
    if(optind < count){
        result = processRemainingArgs(count, args, array);
    } else {
        result = ArgErr;
    }
    return result;
}
void
delArgsArray(argsArray *a){
    for (int i = 0; i<a->size; i++){
        free(a->array[i]);
        a->array[i] = NULL;
    }
    a->size=0;
}
void
cleanUp(argsArray *a){
    if(a->array != NULL){
        delArgsArray(a);
        free(a->array);
        a->array=NULL;
    }
}
int
main(int argc, char **argv){
    char delim = ' '; FILE *filePtr; 
    Status result = UnexpectedErr;
    argsArray argsArr;
    initArgsArray(&argsArr);
    if((result = getArgs(argc, argv, &argsArr, &delim)) != Ok){
        cleanUp(&argsArr);
        return result; 
    }
    
    (void)delim;
    (void)filePtr;
    cleanUp(&argsArr);
    return result;
}

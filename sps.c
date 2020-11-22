#include <stdlib.h>
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <getopt.h>

/* Definitions */
typedef enum {
    UnexpectedErr = -1,
    Ok,
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

char delim = ' ';
FILE *filePtr;

/* Prototypes */
void initCommand(command *c);
void delCmd(command *c);
void printUsage();
Status getArgs(int count, char **args, argsArray *array);
Status processArg(char *arg);
void initArgsArray(argsArray *a);
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
processRemainingArguments(int count, char **args, argsArray *argsA){
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
getArgs(int count, char **args, argsArray *array){
    int opt; Status result = UnexpectedErr;

    while ((opt = getopt(count, args, "d:")) != -1 ){
        switch(opt){
            case 'd':
                delim=optarg[0];
                break;

            default:
                printUsage();
                result = ArgErr;
                break;
        }
    }
    if(optind < count){
        result = processRemainingArguments(count, args, array);
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
    delArgsArray(a);
    free(a->array);
    a->array=NULL;
}
int
main(int argc, char **argv){
    Status result = UnexpectedErr;
    argsArray argsArr;
    if((result = getArgs(argc, argv, &argsArr)) != Ok){
        cleanUp(&argsArr);
        return result; 
    }

    cleanUp(&argsArr);
    return result;
}

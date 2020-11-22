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
argsArray argsArr;
FILE *filePtr;

/* Prototypes */
void initCommand(command *c);
void delCmd(command *c);
void printUsage();
Status getArgs(int count, char **args);
Status processArg(char *arg);

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
    free(c->args);
}

void
printUsage(){
    printf("./sps [-d DELIM] CMD_SEQUENCE FILE\n\r");
}

Status
processRemainingArguments(int count, char **args){
        initArgsArray(&argsArr);
        argsArr.array = malloc(count * sizeof(void*));
        if (argsArr.array == NULL)
            return AllocErr;

        for (int i = optind, j = 0; i<count; i++,j++){
            asprintf(&argsArr.array[j], "%s", args[i]);
            if(argsArr.array[j] == NULL)
                return AllocErr;
            argsArr.size++;
        }

        return Ok;
}

Status
getArgs(int count, char **args){
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
        result = processRemainingArguments(count, args);
    }
    return result;
}

void
delArgsArray(){
    for (int i = 0; i<argsArr.size; i++){
        free(argsArr.array[i]);
        argsArr.size--;
    }
}
void
cleanUp(){
    delArgsArray();
    free(argsArr.array);
}

int
main(int argc, char **argv){
    Status result = UnexpectedErr;
    if((result = getArgs(argc, argv)) != Ok)
        return result; 
    
    for (int i= 0; i<argsArr.size; i++)
        printf("%s\n\r", argsArr.array[i]);

    cleanUp();
    return result;
}

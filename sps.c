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
   char **array;
   int size;
} argsArray;


/* Prototypes */
void initArgsArray(argsArray *a);
void printUsage();
Status processRemainingArgs(int count, char **args, argsArray *argsA);
Status getArgs(int count, char **args, argsArray *array, char *delim);
Status openFile(argsArray *Arr, FILE **fp);
void delArgsArray();
void cleanUp(argsArray *a);

/* Code */
void
initArgsArray(argsArray *a){
    a->array=NULL;
    a->size=0;
}
void
printUsage(){
    printf("./sps [-d DELIM] CMD_SEQUENCE FILE\n\r");
}
Status
processRemainingArgs(int count, char **args, argsArray *argsA){
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
Status
openFile(argsArray *Arr, FILE **fp){

    *fp = fopen(Arr->array[1], "r+");
    if (*fp == NULL)
        return FileErr;
    
    free(Arr->array[1]);
    Arr->array[1] = NULL;
    Arr->size--;

    return Ok;
}
void
delArgsArray(argsArray *a){
    for (int i = 0; i<a->size; i++){
        free(a->array[i]);
        a->array[i] = NULL;
    }
    a->size=0;

    free(a->array);
    a->array=NULL;
}
void
cleanUp(argsArray *a){
    delArgsArray(a);
}
int
main(int argc, char **argv){
    char delim = ' '; FILE *filePtr = NULL; 
    Status result = UnexpectedErr;
    argsArray argsArr;

    initArgsArray(&argsArr);

    if((result = getArgs(argc, argv, &argsArr, &delim)) != Ok){
        cleanUp(&argsArr);
        return result; 
    }
   
    if((result = openFile(&argsArr, &filePtr)) != Ok){
        cleanUp(&argsArr);
        return result;
    }

    //Do stuff


    if (fclose(filePtr) != 0)
        return FileErr;

    cleanUp(&argsArr);
    return result;
}

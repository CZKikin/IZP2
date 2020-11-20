#include <stdlib.h>
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <getopt.h>

//TODO: Naprogramovat find_function(); Zase bude vracet pointer na fci ktera se bude spoustet

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

char delim = ' ';
command **cmds = NULL;
int cmdsCount;
FILE *filePtr;

void
initCommad(command *c){
    c->selector=NULL;
    c->function=NULL;
    c->args=NULL;
}

Status
addToCmd(command *c, char *sel, char* fncWithArg){
    asprintf(c->selector,"%s",sel);
    if (c->selector == NULL)
        return AllocErr;

    char *function = strtok(fncWithArg," "); 

    if (c->function = find_function(function) == NULL)
        return AllocErr;

    c->args = fncWithArg;
    return Ok;
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
addCmdToAray(command *c, int index){
    realloc(cmds, (index+1)*sizeof(c));
    if (cmds == NULL)
        return AllocErr;

    cmds[index] = c;
    cmdsCount++;
    return Ok;
}

Status
getargs(int *count, char **args){
    int opt; Status result = UnexpectedErr;

    while ((opt = getopt(*count, args, "d:")) != -1 ){
        switch(opt){
            case 'd':
                delim=optarg[0];
                break;

            case '?':
                result = processArgs(args[optopt]);
                break;

            default:
                printUsage();
                result = ArgErr;
                break;
        }
    }
    return result;
}

Status
separateCmds(char *arg){
    command c; Status result = UnexpectedErr;
    char *sel = strtok(arg, ";");
    char *cmd = strtok(arg, ";");

    for(int i=0; sel != NULL || cmd != NULL; i++){
        initCmd(&c);
        result = addToCmd(&c,sel,cmd);
        if (result != Ok)
            return ArgErr;

        result = addCmdToArray(&c, i);
        if (result != Ok)
            return ArgErr;

        sel = strtok(arg, ";");
        cmd = strtok(arg, ";");
    }
    return result;
}

Status
processArgs(char *arg){
    if (arg[0] == '['){
        separateCmds(arg);
    } else {
       filePtr = fopen (arg,"r+"); 
       if (filePtr == NULL)
           return FileErr;
    }

    return Ok;
}

void
cleanUp(){
    for (int i=0; i<cmdscount; i++)
        delCmd(cmds[i]);
}

int
main(int argc, char **argv){
    Status result = UnexpectedErr;
    result = getargs(&argc, argv);
    if (result != Ok){
        if (cmds != NULL)
            cleanUp();
        return result; 
    }

    
    return result;
}

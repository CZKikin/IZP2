#include <stdlib.h>
//#define _GNU_SOURCE because school :)
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#ifdef DEBUG
#define dp(fmt, args...) (printf("%s:%s:%d "fmt, __FILE__, __FUNCTION__, __LINE__, args))
#else
#define dp(fmt, args...) 
#endif

#define CMDLEN (1001)
#define CMDCOUNT (1000)
#define NUM_OF_FN (16)

/* Definitions */
typedef enum {
    Ok = 0,
    UnexpectedErr,
    ArgErr,
    AllocErr,
    FileErr,
    CmdErr,
    EndReached
} Status;

typedef enum {
    Command = 0,
    Select
} command;

typedef struct {
   char **array;
   int size;
} argsArray;

typedef struct {
    int row;
    int col;
    char *data;
    int size;
} cell;

typedef struct {
    cell *table;
    int size;
} table;

/* Prototypes */
char isDelim(char ch, char *delims);
void initCell(cell *c);
void delCell(cell *c);
Status initCellData(cell *c);
Status insToCell(cell *c, char ch);
void initTable(table *t);
void delTable(table *t);
void setCellCords(cell *c, int col, int row);
Status insToTable(table *t, cell *c);
void delFromTable(table *t, cell *c);
void initArgsArray(argsArray *a);
void printUsage();
void delArgsArray();
Status processRemainingArgs(int count, char **args, argsArray *argsA);
Status getArgs(int count, char **args, argsArray *array, char **delim);
Status openFile(argsArray *Arr, FILE **fp);
Status readFile(FILE *fp, char *delim, table *t);
void cleanUp(argsArray *a, char *delim, table *t, char *cmd);
void DEBUGprintTable(table *t, char *delims);
Status delCmdFromArgs(int index, argsArray *a);
Status getCmd(char **dst, command *dstT, argsArray *a);
void cleanCords(int *cords);
Status pickCells(char *cmd, int *cords, int *cordsCount);
Status runCommand(table *t, char *cmd, int *cords, int cordsCount);
Status (*getFnPt())(char *cmd);
Status setupCell(cell *c, int col, int row);
int getRows(table *t);
void sortTable(table *t);
void insertRow(table *t, row);

Status irow(table *t, char *args, int *cords, int cordsCount);
Status arow(table *t, char *args, int *cords, int cordsCount);
Status drow(table *t, char *args, int *cords, int cordsCount);
Status icol(table *t, char *args, int *cords, int cordsCount);
Status acol(table *t, char *args, int *cords, int cordsCount);
Status dcol(table *t, char *args, int *cords, int cordsCount);
Status set(table *t, char *args, int *cords, int cordsCount);
Status clear(table *t, char *args, int *cords, int cordsCount);
Status swap(table *t, char *args, int *cords, int cordsCount);
Status sum(table *t, char *args, int *cords, int cordsCount);
Status avg(table *t, char *args, int *cords, int cordsCount);
Status count(table *t, char *args, int *cords, int cordsCount);
Status len(table *t, char *args, int *cords, int cordsCount);
Status def(table *t, char *args, int *cords, int cordsCount);
Status use(table *t, char *args, int *cords, int cordsCount);
Status inc(table *t, char *args, int *cords, int cordsCount);

/* Code */
Status
irow(table *t, char *args, int *cords, int cordsCount){
    (void)args;
    int top = 1; int bot = 1;
    int row = 1; char doneFlag = 0;
    cell c;
    dp("Running: Irow with args=>%s\n", args);
    switch (cordsCount){
        case 1:
            top = bot = cords[0];
            break;
        case 2:
        case 4:
            top = cords[0];
            bot = cords[1];
            break;
    }

    if (top == -1)
        top = 1;
    if (bot == -1)
        bot = getRows(t);

    for (int i=0; i<t->size; i++){
        if (t->table[i].row > row){
            row++;
            doneFlag=1;
        }
        
        if (t->table[i].row>=top && t->table[i].row<=bot)
            if (!doneFlag)
                insertRow(*t, row); //doimplementovat ze dostanu radek, vlozim a automaticky pak posunu vse o jedno dal
        
    }
    sortTable(t);
    return Ok;
}
Status
arow(table *t, char *args, int *cords, int cordsCount){
    (void)t;
    dp("Running: Arow with args=>%s\n", args);
    (void)args;
    (void)cords;
    (void)cordsCount;
    return Ok;
}
Status
drow(table *t, char *args, int *cords, int cordsCount){
    (void)t;
    dp("Running: Drow with args=>%s\n", args);
    (void)args;
    (void)cords;
    (void)cordsCount;
    return Ok;
}
Status
icol(table *t, char *args, int *cords, int cordsCount){
    (void)t;
    dp("Running: Icol with args=>%s\n", args);
    (void)args;
    (void)cords;
    (void)cordsCount;
    return Ok;
}
Status
acol(table *t, char *args, int *cords, int cordsCount){
    (void)t;
    dp("Running: Acol with args=>%s\n", args);
    (void)args;
    (void)cords;
    (void)cordsCount;
    return Ok;
}
Status
dcol(table *t, char *args, int *cords, int cordsCount){
    (void)t;
    dp("Running: Dcol with args=>%s\n", args);
    (void)args;
    (void)cords;
    (void)cordsCount;
    return Ok;
}
Status
set(table *t, char *args, int *cords, int cordsCount){
    (void)t;
    dp("Running: Set with args=>%s\n", args);
    (void)args;
    (void)cords;
    (void)cordsCount;
    return Ok;
}
Status
clear(table *t, char *args, int *cords, int cordsCount){
    (void)t;
    dp("Running: Clear with args=>%s\n", args);
    (void)args;
    (void)cords;
    (void)cordsCount;
    return Ok;
}
Status
swap(table *t, char *args, int *cords, int cordsCount){
    (void)t;
    dp("Running: Swap with args=>%s\n", args);
    (void)args;
    (void)cords;
    (void)cordsCount;
    return Ok;
}
Status
sum(table *t, char *args, int *cords, int cordsCount){
    (void)t;
    dp("Running: Sum with args=>%s\n", args);
    (void)args;
    (void)cords;
    (void)cordsCount;
    return Ok;
}
Status
avg(table *t, char *args, int *cords, int cordsCount){
    (void)t;
    dp("Running: Avg with args=>%s\n", args);
    (void)args;
    (void)cords;
    (void)cordsCount;
    return Ok;
}
Status
count(table *t, char *args, int *cords, int cordsCount){
    (void)t;
    dp("Running: Count with args=>%s\n", args);
    (void)args;
    (void)cords;
    (void)cordsCount;
    return Ok;
}
Status
len(table *t, char *args, int *cords, int cordsCount){
    (void)t;
    dp("Running: Len with args=>%s\n", args);
    (void)args;
    (void)cords;
    (void)cordsCount;
    return Ok;
}
Status
def(table *t, char *args, int *cords, int cordsCount){
    (void)t;
    dp("Running: Def with args=>%s\n", args);
    (void)args;
    (void)cords;
    (void)cordsCount;
    return Ok;
}
Status
use(table *t, char *args, int *cords, int cordsCount){
    (void)t;
    dp("Running: Use with args=>%s\n", args);
    (void)args;
    (void)cords;
    (void)cordsCount;
    return Ok;
}
Status
inc(table *t, char *args, int *cords, int cordsCount){
    (void)t;
    dp("Running: Inc with args=>%s\n", args);
    (void)args;
    (void)cords;
    (void)cordsCount;
    return Ok;
}
char
isDelim(char ch, char *delims){
    for (size_t i=0; i<strlen(delims); i++){
        if (ch == delims[i])
            return 1;
    }

    return 0;
}
void
initCell(cell *c){
    c->data=NULL;
    c->row = 0;
    c->col = 0;
    c->size = 0;
}
void
delCell(cell *c){
    free(c->data);
    initCell(c);
}
Status
initCellData(cell *c){
    // asprintf(&c->data, "%s", ""); because school :)
    c->data = malloc(sizeof(char));
    if (c->data == NULL)
       return AllocErr;

    snprintf(c->data, 1, "%s", "");

    return Ok;
}
Status
insToCell(cell *c, char ch){
    c->size++;
    c->data = realloc(c->data, (c->size+1)*sizeof(char));
    if (c->data == NULL)
        return AllocErr;

    strncat(c->data, &ch, 1);
    return Ok;
}
void
initTable(table *t){
    t->table = NULL;
    t->size = 0;
}
void
delTable(table *t){
    int size = t->size;
    if (t->size > 0){
        for (int i=0; i<size; i++)
            delFromTable(t, &t->table[i]);
    }
    
    free(t->table);
    initTable(t);
}
void
setCellCords(cell *c, int col, int row){
    c->col = col;
    c->row = row;
}
Status
insToTable(table *t, cell *c){
    t->size++;
    t->table = realloc(t->table, t->size*sizeof(cell));
    if (t->table == NULL)
        return AllocErr;

    t->table[t->size - 1] = *c;
    return Ok;
}
void
delFromTable(table *t, cell *c){
    delCell(c);
    t->size--;
}
void
initArgsArray(argsArray *a){
    a->array=NULL;
    a->size=0;
}
void
printUsage(){
    printf("./sps [-d DELIM] CMD_SEQUENCE FILE\n\r");
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
Status
processRemainingArgs(int count, char **args, argsArray *argsA){
        argsA->array = malloc(count * sizeof(void*));

        if (argsA->array == NULL)
            return AllocErr;

        for (int i = optind, j = 0; i<count; i++,j++){
            //asprintf(&argsA->array[j], "%s", args[i]); because school
            dp("Argument: %s\n", args[i]);
            argsA->array[j] = NULL;

            argsA->array[j] = realloc(argsA->array[j], (strlen(args[i])+1)*sizeof(char));
            if(argsA->array[j] == NULL)
                return AllocErr;

            memset(argsA->array[j],0,strlen(args[i]+1));

            snprintf(argsA->array[j], strlen(args[i])+1, "%s", args[i]);

            argsA->size++;
        }

        if (argsA->size>2){
            printUsage(); 
            return ArgErr;
        }

        return Ok;
}
Status
getArgs(int count, char **args, argsArray *array, char **delim){
    int opt; Status result = UnexpectedErr;

    while ((opt = getopt(count, args, "d:")) != -1 ){
        switch(opt){
            case 'd':
                //asprintf(delim, "%s", optarg); because school
                *delim = realloc(*delim, (strlen(optarg)+1)*sizeof(char));
                if (delim == NULL)
                    return AllocErr;   

                snprintf(*delim, strlen(optarg)+1, "%s", optarg);
                break;

            default:
                printUsage();
                result = ArgErr;
                break;
        }
    }

    if (*delim == NULL){
        //asprintf(delim, "%s", " "); beacuse school 
        *delim = realloc(*delim, 2*sizeof(char));
        if (*delim==NULL)
            return AllocErr;

        sprintf(*delim, "%s", " ");
 
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

    dp("Trying to open: %s\n", Arr->array[1]);
    *fp = fopen(Arr->array[1], "r+");
    if (*fp == NULL)
        return FileErr;
    
    free(Arr->array[1]);
    Arr->array[1] = NULL;
    Arr->size--;

    return Ok;
}
Status 
setupCell(cell *c, int col, int row){
    Status result = UnexpectedErr;
    initCell(c);
    result = initCellData(c);
    setCellCords(c, col, row);
    return result;
}
Status
readFile(FILE *fp, char *delim, table *t){
    Status result = UnexpectedErr;
    int cols = 1; int rows = 1;
    char ch;
    cell c;

    setupCell(&c, cols, rows); 

    while (!feof(fp)){
        ch = fgetc(fp);
        if (isDelim(ch, delim)){
            dp("Insering cell: row=> %d, col=>%d, data=%s, size=%d\n", c.row, c.col, c.data, c.size);
            result = insToTable(t, &c);
            if(result != Ok){
                delCell(&c);
                return result;
            }
            cols++;
            setupCell(&c, cols, rows);
            
        } else if (ch == '\n') {
            dp("Insering cell: row=> %d, col=>%d, data=%s, size=%d\n", c.row, c.col, c.data, c.size);
            result = insToTable(t, &c);

            if (result != Ok){
                delCell(&c);
                return result;
            }

            cols = 1;
            rows++;
            setupCell(&c, cols, rows);

        } else {
            result = insToCell(&c, ch);
            if(result != Ok){
                delCell(&c);
                return result;
            }
        }

    }
    
    delCell(&c);
    return result;
}
void
cleanUp(argsArray *a, char *delim, table *t, char *cmd){
    delArgsArray(a);
    delTable(t);
    free(delim);
    delim = NULL;
    free(cmd);
    cmd = NULL;
}
void
DEBUGprintTable(table *t, char *delims){
    int row = 1;
    printf("TABLE START=======\n");
    for(int i=0; i<t->size; i++){
        printf("%s", t->table[i].data);
        if (i == t->size-1)
            break;
        if (t->table[i+1].row>row){
            row++;
            printf("\n");
        } else {
            printf("%c", delims[0]);
        }
    }
    printf("\nTABLE END========\n");
}
Status
delCmdFromArgs(int index, argsArray *a){
    char *copy=NULL;
    copy=malloc((strlen(a->array[0])+1)*sizeof(char));

    if (copy == NULL)
        return AllocErr;

    memset(copy, 0, strlen(a->array[0]+1));
    for (int i=0, j=0; i<(int)strlen(a->array[0])+1; i++){
        if(i>index){
            copy[j] = a->array[0][i];
            j++;
        }
    }
    a->array[0] = realloc(a->array[0], (strlen(copy)+1)*sizeof(char));
    if (a->array==NULL)
        return AllocErr;
    memset(a->array[0], 0, strlen(copy)+1);
    strncpy(a->array[0], copy, strlen(copy)); 

    free(copy);
    copy=NULL;

    return Ok;
}
Status
getCmd(char **dst, command *dstT, argsArray *a){
    Status result = UnexpectedErr;
    char command[CMDLEN];
    int delIndex;

    if (a->array[0][0] == '\0')
        return EndReached;

    memset(command, 0, CMDLEN);
    free(*dst);
    *dst = NULL;

    for (int i = 0; a->array[0][i] != ';' && a->array[0][i] != '\0'; i++){
        if (i == (CMDLEN-1))
            return ArgErr;
        command[i] = a->array[0][i]; 
        delIndex = i;
    }

    delIndex++;
    result = delCmdFromArgs(delIndex, a);

    //asprintf(dst, "%s", command); because school :)
    *dst = realloc(*dst, (strlen(command)+1)*sizeof(char));

    if(*dst == NULL)
        return AllocErr;

    memset(*dst, 0, strlen(command)+1);
    snprintf(*dst, strlen(command)+1, "%s", command);

    if (command[0] == '[')
        *dstT = Select;
    else
        *dstT = Command;
    
    return result;
}
void
cleanCords(int *cords){
    for (int i=0; i<4; i++)
        cords[i] = 0;
}
Status
pickCells(char *cmd, int *cords, int *cordsCount){
    int len = strlen(cmd)+1;
    cleanCords(cords);
    *cordsCount = 0;
    char *buf = NULL;
    buf = realloc(buf, CMDLEN*sizeof(char));
    memset(buf, 0, CMDLEN*sizeof(char));


    //TODO: Check for min, max, find
    for (int i = 0; i<len; i++){
        if (*cordsCount>3)
            break;

        if (cmd[i] == '-'){
            cords[*cordsCount] = -1;
            *cordsCount+=1;
            continue;
        } else if (cmd[i]>47 && cmd[i]<58) {
            strncat(buf, &cmd[i], 1);
            if(cmd[i+1]>47 && cmd[i+1]<58){
                continue;
            }

            cords[*cordsCount] = atoi(buf);
            *cordsCount+=1;
            memset(buf, 0, CMDLEN*sizeof(char));
        } 
    }

    free(buf);
    buf = NULL;

    if (*cordsCount == 3)
        return ArgErr;

    if (*cordsCount ==  4){
        for (int i = 1; i<*cordsCount-1; i++){
            if (cords[i-1] > cords[i+1]){
                if (cords[i+1] == -1)
                    continue;
                dp("Arguments: cord1: %d > cord2: %d, & that's bad\n", cords[i-1], cords[i+1]);
                return ArgErr;
            }
        }
    }
    return Ok;
}
char* 
checkCmdArgs(char *cmd){
    int index = 0;
    char *arg = NULL;
    for (int i = 0; i<(int)strlen(cmd)+1; i++){
        if(isDelim(cmd[i], " ")){
                index = ++i;
                break;
        }
    }
    //hen tu by sa taky hodil asprintf :)

    if (index == 0)
        return NULL;

    arg = malloc(strlen(&cmd[index]+1)); 
    snprintf(arg, strlen(&cmd[index])+1, "%s", &cmd[index]);

    cmd[index-1] = '\0';
    cmd = realloc(cmd, strlen(cmd)+1);

    return arg;
}
Status
runCommand(table *t, char *cmd, int *cords, int cordsCount){
    Status result = UnexpectedErr;
    char *argument = NULL;
    Status (*function)() = NULL;

    argument = checkCmdArgs(cmd);

    function = getFnPt(cmd);
    if(function == NULL){
        dp("Invalid function cmd=> %s", cmd);
        free(argument);
        return ArgErr;
    }

    dp("Command & args: cmd=>%s, cordsCount=>%d, argument=> %s\n", cmd, cordsCount, argument);
    result = function(t, argument, cords, cordsCount);

    free(argument);
    argument = NULL;
    return result;
}
Status
(*getFnPt(char *cmd))(){
    //Globální proměnné jsou špatné, tak to je tady no :)
    char fnStr[NUM_OF_FN][6]={"irow", "arow", "drow", "icol", "acol", "dcol", "set", "clear", "swap", "sum", "avg", "count", "len", "def", "use", "inc"};
    Status (*fns[NUM_OF_FN])()={irow, arow, drow, icol, acol, dcol, set, clear, swap, sum, avg, count, len, def, use, inc};

    for (int i=0; i<NUM_OF_FN; i++){
        if (strcmp(cmd, fnStr[i]) == 0)
            return fns[i];
    }
    return NULL;
}
int
getRows(table *t){
    int rows = 1;
    for (int i = 0; i<t->size; i++)
        if (t->table[i].row>rows)
            rows = t->table[i].row;
    return rows;
}
void
sortTable(table *t){
    cell c;
    initCell(&c);

    for (int i = 0; i<t->size; i++){
        for (int j = 1; j<t->size; j++){
            if(t->table[i].row<t->table[j].row){
                c=t->table[i];
                t->table[i] = t->table[j];
                t->table[j] = c;
            }
        }
    }

    for (int i = 0; i<t->size; i++){
        for (int j = 1; j<t->size; j++){
            if(t->table[i].row == t->table[j].row){
                if(t->table[i].col<t->table[j].col){
                    c=t->table[i];
                    t->table[i] = t->table[j];
                    t->table[j] = c;
                }
            }
        }
    }
    delCell(&c);
}
int
main(int argc, char **argv){
    char *delim = NULL; FILE *filePtr = NULL; 
    char *cmd = NULL; command type; int cords[4];
    int cordsCount;
    Status result = UnexpectedErr;
    argsArray argsArr;
    table table;

    initArgsArray(&argsArr);
    dp("argsArr: array=>0x%p, size=>%d\n", argsArr.array, argsArr.size);
    initTable(&table);
    dp("table: table=>0x%p, size=>%d\n", table.table, table.size);

    if((result = getArgs(argc, argv, &argsArr, &delim)) != Ok){
        cleanUp(&argsArr, delim, &table, cmd);
        return result; 
    }

    dp("delims: \"%s\"\n", delim);
   
    if((result = openFile(&argsArr, &filePtr)) != Ok){
        cleanUp(&argsArr, delim, &table, cmd);
        return result;
    }
    
    if((result = readFile(filePtr, delim, &table)) != Ok){
        cleanUp(&argsArr, delim, &table, cmd);
        return result;
    }

    DEBUGprintTable(&table, delim); 
    dp("Commands: %s\n", argsArr.array[0]);
    while ((result = getCmd(&cmd, &type, &argsArr)) != EndReached && result == Ok){
        dp("Chosen command: %s\n", cmd);

        if(type == Command)
            result = runCommand(&table, cmd, cords, cordsCount);
        else
            result = pickCells(cmd, &*cords, &cordsCount);

        free(cmd);
        cmd = NULL;
        if(result!=Ok)
            break;
    }
    if(result == EndReached)
        result = Ok;

    if (fclose(filePtr) != 0){
        cleanUp(&argsArr, delim, &table, cmd);
        return FileErr;
    }

    DEBUGprintTable(&table, delim); 

    cleanUp(&argsArr, delim, &table, cmd);
    return result;
}

#include <stdlib.h>
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#ifdef DEBUG
#define dp(fmt, args...) (printf("%s:%s:%d "fmt, __FILE__, __FUNCTION__, __LINE__, args))
#else
#define dp(fmt, args...) 
#endif
// TODO: Naprogramovat vlastní asprintf - na merlinovi to nejde buildnout!!

/* Definitions */
typedef enum {
    Ok = 0,
    UnexpectedErr,
    ArgErr,
    AllocErr,
    FileErr
} Status;

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
// void changeCellPos(cell *c, int col, int row); //TODO: -1 as param means do not chage
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
void cleanUp(argsArray *a, char *delim, table *t);

/* Code */
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
   asprintf(&c->data, "%s", ""); 
   if (c->data == NULL)
       return AllocErr;
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
getArgs(int count, char **args, argsArray *array, char **delim){
    int opt; Status result = UnexpectedErr;

    while ((opt = getopt(count, args, "d:")) != -1 ){
        switch(opt){
            case 'd':
                asprintf(delim, "%s", optarg);
                if (delim == NULL)
                    return AllocErr;
                break;

            default:
                printUsage();
                result = ArgErr;
                break;
        }
    }

    if (*delim == NULL){
        asprintf(delim, "%s", " ");
        if (*delim==NULL)
            return AllocErr;
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
    dp("cell: row=> %d, col=>%d, data=\"%s\", size=%d\n", c->row, c->col, c->data, c->size);
    result = initCellData(c);
    dp("init cellData: row=> %d, col=>%d, data=\"%s\", size=%d\n", c->row, c->col, c->data, c->size);
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

    while ((ch = fgetc(fp)) != EOF){
        dp("char = %c\n", ch);
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
        
    return result;
}
void
cleanUp(argsArray *a, char *delim, table *t){
    delArgsArray(a);
    delTable(t);
    free(delim);
    delim = NULL;
}
int
main(int argc, char **argv){
    char *delim = NULL; FILE *filePtr = NULL; 
    Status result = UnexpectedErr;
    argsArray argsArr;
    table table;

    initArgsArray(&argsArr);
    dp("argsArr: array=>0x%p, size=>%d\n", argsArr.array, argsArr.size);
    initTable(&table);
    dp("table: table=>0x%p, size=>%d\n", table.table, table.size);

    if((result = getArgs(argc, argv, &argsArr, &delim)) != Ok){
        cleanUp(&argsArr, delim, &table);
        return result; 
    }

    dp("delims: \"%s\"\n", delim);
   
    if((result = openFile(&argsArr, &filePtr)) != Ok){
        cleanUp(&argsArr, delim, &table);
        return result;
    }
    
    if((result = readFile(filePtr, delim, &table)) != Ok){
        cleanUp(&argsArr, delim, &table);
        return result;
    }

    if (fclose(filePtr) != 0){
        cleanUp(&argsArr, delim, &table);
        return FileErr;
    }

    cleanUp(&argsArr, delim, &table);
    return result;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define CORRECT_LEN 3 
#define INITIAL_SIZE 1009
#define PRIME 31
#define FIRSTPRIME 2
#define BUFFER 50
#define strsame(A,B) (strcmp(A,B) == 0)
#define strdiff(A,B) (strcmp(A,B) != 0)
#define DOUBLE(A) (A*2)
#define SIXTY_PER_CENT(A) ((A/2) * 3)
#define ERROR(PHRASE, A) {fprintf(stderr, "Error: %s\n",\
        PHRASE); exit(A);}
#define ERROR_INPUT(A, B) {fprintf(stderr, \
        "Error: unable to open file %s\n", A); exit(B);}
#define TRUE 1
#define FALSE 0
#define CSEC (double)(CLOCKS_PER_SEC)
#define PRINTROW 10
#define MAPBLOCK 50
#define NEW_MAPBLOCK 2000
#define MAPBLOCK_RESIZE 30000

enum errorCodes{
  erLen,
  erNull,
  erAllocTab,
  erAllocArr,
  erAllocInp,
  erAllocStr,
  erAllocOut,
  erAllocMap,
  erAllocRes,
  erNoFlag,
  erInput
};

struct hashTable{
  struct input **hTable;
  int tableLen;
  int words;
  int flag;
};

struct input{
  char *string;
  struct input *next;
};

struct searchResults{
  int *map;
  double noOfMoves;
  int collisions;
  double time;
  int mapLen;
};

typedef struct hashTable hashTable;
typedef struct input input;
typedef struct searchResults result;

void checkInputCorrect(int argc, FILE *file1, 
     FILE *file2, char **argv);
hashTable* fillTable(FILE *file1, hashTable *table,
     result *res, int flag);
hashTable* initialiseTable(int num, hashTable *table);
hashTable* placeInTable(char *str, hashTable *table, 
     result *res);
int tableNotNull(hashTable *table, int h);
unsigned int hash(char *str, hashTable *table);
void addLink(hashTable *table, unsigned int h, char *str,
     result *res);
input* Allocate(char *str);
void insertStr(char *str, hashTable *table, int h);
hashTable* rehash(hashTable *table, result *res);
int newPrime(hashTable *table);
int prime(int num);
void rehashAllocate(hashTable *newTable, int h, char *str, 
     result *res);
void reallocateList(hashTable *table, hashTable *newTable,
     int i, result *res);
void freeTable(hashTable *table);
result* searchTable(hashTable *table, FILE *file2,
        result *res);
result* findStr(char *str, hashTable *table, 
        result *res);
unsigned int hashDhash(char *str);
unsigned int doubleHash(char *str, hashTable *table);
hashTable* rehashD(hashTable *table, result* res);
result* searchTableDhash(hashTable *table, FILE *file2, 
        result *DResult);
int findStrDhash(char *str, hashTable *table);
void updateOutcomes(hashTable* table, result *res,
     double speed);
hashTable* fillTableDhash(FILE *file, hashTable *table,
           result *res, int flag) ;
hashTable* placeInTableDhash(char *str, hashTable *table,
           result *res);
result* initialiseResult(result* res);
result* initialiseMap(result *res, int k);
int fseek(FILE *file, long offset, int whence);
void rewind(FILE *file);
void DhashActions(FILE* file, FILE* file2, int flag, 
     hashTable *tab, result* res, char **argv);
void ShashActions(FILE* file, FILE* file2, int flag, 
     hashTable *tab, result* res, char** argv);
unsigned int hashSwitch(hashTable *tab, char *str);
unsigned int hashDJB2(char *str, hashTable *table); 
unsigned int myHash(char *str, hashTable* table); 
unsigned int hashFNV1a(char* str, hashTable *table);
void printMap(result* res);



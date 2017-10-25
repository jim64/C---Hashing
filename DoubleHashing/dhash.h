#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>

#define CORRECT_LEN 3 
#define INITIAL_SIZE 1009
#define PRIME 31
#define FIRSTPRIME 2
#define BUFFER 50
#define strsame(A,B) (strcmp(A,B) == 0)
#define strdiff(A,B) (strcmp(A,B) != 0)
#define DOUBLE(A) (A*2)
#define SIXTY_PER_CENT(A) ((A/5) * 3)
#define ERROR(PHRASE, A) {fprintf(stderr, "Error: %s\n",\
        PHRASE); exit(A);}
#define ERROR_INPUT(A, B) {fprintf(stderr,\
        "Error: unable to open file %s\n", A); exit(B);}
#define TRUE 1
#define FALSE 0

enum errorCodes{
  erLen,
  erNull,
  erAllocTab,
  erAllocArr,
  erAllocStr
};

struct hashTable{
  char **hTable;
  int tableLen;
  int words;
};

typedef struct hashTable hashTable;

void checkInputCorrect(int argc, FILE *file,
     FILE *file2, char **argv);
hashTable* fillTable(FILE *file, hashTable *table);
double searchTable(hashTable *table, FILE *file2);
hashTable* initialiseTable(int num, hashTable *table);
hashTable* placeInTable(char *str, hashTable *table);
unsigned int hash(char *str, hashTable *table);
int tableNotNull(hashTable *table, int h);
unsigned int doubleHash(char *str, hashTable *table);
void insertStr(char *str, hashTable *table, int h);
hashTable* rehash(hashTable *table);
int newPrime(hashTable *table);
int prime(int num);
void freeTable(hashTable *table);
int findStr(char *str, hashTable *table);





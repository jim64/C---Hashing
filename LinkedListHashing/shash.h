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
#define SIXTY_PER_CENT(A) ((A/2) * 3)
#define ERROR(PHRASE, A) {fprintf(stderr, "Error: %s\n",\
        PHRASE); exit(A);}
#define ERROR_INPUT(A, B) {fprintf(stderr, \
        "Error: unable to open file %s\n", A); exit(B);}
#define TRUE 1
#define FALSE 0

enum errorCodes{
  erLen,
  erNull,
  erAllocTab,
  erAllocArr,
  erAllocInp,
  erAllocStr
};

struct hashTable{
  struct input **hTable;
  int tableLen;
  int words;
};

struct input{
  char *string;
  struct input *next;
};

typedef struct hashTable hashTable;
typedef struct input input;

void checkInputCorrect(int argc, FILE *file1, 
     FILE *file2, char **argv);
hashTable* fillTable(FILE *file1, hashTable *table);
hashTable* initialiseTable(int num, hashTable *table);
hashTable* placeInTable(char *str, hashTable *table);
int tableNotNull(hashTable *table, int h);
unsigned int hash(char *str, hashTable *table);
void addLink(hashTable *table, unsigned int h, char *str);
input* Allocate(char *str);
void insertStr(char *str, hashTable *table, int h);
hashTable* rehash(hashTable *table);
int newPrime(hashTable *table);
int prime(int num);
void rehashAllocate(hashTable *newTable, int h, char *str);
void reallocateList(hashTable *table,
     hashTable *newTable, int i);
void freeTable(hashTable *table);
double searchTable(hashTable *table, FILE *file2);
int findStr(char *str, hashTable *table);





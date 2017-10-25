#include "dhash.h"

hashTable* initialiseTable(int num, hashTable *table)
{

  hashTable *p = NULL;

  if((p = (hashTable*)calloc(1, sizeof(hashTable)))
    == NULL){ERROR("Cannot allocate array", erAllocTab);
  }

  table = p;

  if((table->hTable = (char**)calloc(num,sizeof(char*)))
    == NULL){ERROR("Cannot allocate array", erAllocArr);
  }

  table->tableLen = num;

  return p;

}

hashTable* placeInTable(char *str, hashTable *table)
{

  unsigned int h = 0;

  h = hash(str, table);

  while(tableNotNull(table, h)){
    h += doubleHash(str, table);
    h = (h % table->tableLen);
  }  

  insertStr(str, table, h);
  
  /*makes new table when current table is 60% full
    - this can be ammended in the header file*/
  if(table->words == (SIXTY_PER_CENT(table->tableLen))){ 
    table = rehash(table); 
  }

  return table;

}

unsigned int hash(char *str, hashTable *table)
{

  short int i;
  short int n = strlen(str);
  unsigned int hash = 0;
  
  for(i = 0; i < n; i++){
    hash = str[i] + PRIME * hash;
  }

  /*ensures hash position will be in table bounds*/
  hash = hash % table->tableLen;

  return(hash == 0) ? 1:hash;

}

int tableNotNull(hashTable *table, int h)
{
  return(table->hTable[h] != NULL) ? TRUE:FALSE;
}

unsigned int doubleHash(char *str, hashTable *table)
{

  short int i;
  short int n = strlen(str);
  unsigned int hash = 0;
  
  for(i = 0; i < n; i++){
    hash = str[i] + PRIME * hash;
  }
  
  /*ensures that hash is positive after modulus is carried
    out and that the step to be taken is at least 1*/
  return(hash % table->tableLen <= 0) ? 1:hash;

}

void insertStr(char *str, hashTable *table, int h)
{

  int len = 0;

  len = strlen(str);
  
  if((table->hTable[h] = (char*)calloc(1, len)) == NULL){
    ERROR("Cannot allocate space for str", erAllocStr);
  } 

  /*strncpy used to avoid potential buffer overflows*/
  strncpy(table->hTable[h], str, len);
  table->words++;

}

hashTable* rehash(hashTable *table)
{

  hashTable *new = NULL;
  unsigned int h = 0;
  int i;
  int next_size = newPrime(table);

  new = initialiseTable(next_size, new);

  for(i = 0; i < table->tableLen; i++){
    if(tableNotNull(table, i)){
      h = hash(table->hTable[i], new);
      /*below checks if position in new table is free or
        not. If not, steps through to find new placement*/
      while(tableNotNull(new, h)){
        h += doubleHash(table->hTable[i], new);
        h = (h % next_size);
      }
    insertStr(table->hTable[i], new, h);
    }
  }
  
  freeTable(table);
  table = new;

  return table;

}

int newPrime(hashTable *table) 
{

  int new = (DOUBLE(table->tableLen)) + 1; 
  
  while(!prime(new)){
    new++;
  }

  return new;

}

int prime(int num)
{

  int i;
  
  for(i = FIRSTPRIME; i < num; i++){
    if((num % i) == 0){
      return FALSE;
    }
  }

  return TRUE;
}

void freeTable(hashTable *table)
{

  int i;

  for(i = 0; i < table->tableLen; i++){
    free(table->hTable[i]);
  }

  free(table);

}

/*check in place in below code to ensure that not
  comparing str with a NULL placement in the table
  - if writing code to return if an item is not in the
  table this could be changed to return the string with 
  a statement that it is not found*/
int findStr(char *str, hashTable *table)
{

  unsigned int h = 0;
  double move = 0;

  h = hash(str, table);

  if((tableNotNull(table, h)) && 
    (strsame(table->hTable[h], str))){
    move++;
  }
  else{
    /*below keeps stepping through table until it finds 
      the string or a NULL place in the table*/
    while((tableNotNull(table, h)) && 
      (strdiff(table->hTable[h], str))){ 
      h += doubleHash(str, table);
      h = (h % table->tableLen);
      move++;   
    }
  }

  return move;

}








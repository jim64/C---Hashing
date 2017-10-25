#include "shash.h"

hashTable* initialiseTable(int num, hashTable *table)
{

  hashTable *tmp = NULL;

  if((tmp = (hashTable*)calloc(1, sizeof(hashTable)))
     == NULL){ERROR("Cannot allocate array", erAllocTab);
  }

  table = tmp;

  if((table->hTable = (input**)calloc(num,
    sizeof(input))) == NULL){ERROR("Cannot allocate array",
    erAllocArr);
  }

  table->tableLen = num;

  return table;

}

hashTable* placeInTable(char *str, hashTable *table)
{

  unsigned int h = 0;

  h = hash(str, table);

  if(tableNotNull(table, h)){ 
    /*this creates / adds to linked list if space already
      full*/ 
    addLink(table, h, str);  
  }  
  else{
    insertStr(str, table, h);
  }

  /*makes new table when current table is 60% full
   - this can be ammended in the header file*/
  if(table->words == (SIXTY_PER_CENT(table->tableLen))){ 
    table = rehash(table); 
  }

  return table;

}

int tableNotNull(hashTable *table, int h)
{

  return(table->hTable[h] != NULL) ? TRUE:FALSE;

}

unsigned int hash(char *str, hashTable *table)
{

  short int i;
  short int n = strlen(str);
  unsigned int hash = 0;

  for(i = 0; i < n; i++){
    hash = str[i] + PRIME *hash;
  }
  
  /*modulus the hash by the table length to ensure that
  the hash is within bounds*/
  hash = hash % table->tableLen;

  /*ensures that the hash is at least 1*/
  return(hash == 0) ? 1:hash;

}

void addLink(hashTable *table, unsigned int h, char *str)
{
  
  input *tmp = NULL;
  
  tmp = table->hTable[h];

  while(tmp->next != NULL){
    tmp = tmp->next;
  }
  
  tmp->next = Allocate(str); 
  table->words++;
   
}

input* Allocate(char *str)
{

  input *tmp = NULL;
  int len = 0;

  len = strlen(str);
  
  if((tmp = (input*)calloc(1, sizeof(input))) == NULL){
    ERROR("Cannot allocate space for input", erAllocInp); 
  } 

  if((tmp->string = (char*)calloc(1, len)) == NULL){
    ERROR("Cannot allocate space for str", erAllocStr);
  } 

  /*strncpy used to avoid buffer overflow*/
  strncpy(tmp->string, str, len);
  tmp->next = NULL;

  return tmp;

}

void insertStr(char *str, hashTable *table, int h)
{

  table->hTable[h] = Allocate(str); 
  table->words++;

}

hashTable* rehash(hashTable *table)
{

  hashTable *new = NULL;
  unsigned int h = 0;
  int i;
  int nextSize = newPrime(table);

  new = initialiseTable(nextSize, new);

  for(i = 0; i < table->tableLen; i++){
    if(tableNotNull(table, i)){
      /*this checks ensures that there is not already a 
        linked list in the original table*/
      if(table->hTable[i]->next == NULL){
        h = hash(table->hTable[i]->string, new);
        rehashAllocate(new, h, table->hTable[i]->string);
      }
      /*ensures that previous linked lists are put into
        new table*/
      else{
        reallocateList(table, new, i);
      }
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

void rehashAllocate(hashTable *newTable, int h, char *str)
{

  if(tableNotNull(newTable, h)){
    addLink(newTable, h, str);
  }
  else{
    insertStr(str, newTable, h);
  }  

}

void reallocateList(hashTable *table, 
     hashTable *newTable, int i)
{

  input *tmp = NULL;
  unsigned int h = 0;

  tmp = table->hTable[i];
  
  while(tmp != NULL){
    h = hash(tmp->string, newTable);
    rehashAllocate(newTable, h, tmp->string);
    tmp = tmp->next;
  }  

} 

/*frees both cell of the table and also any linked list 
  from that cell. Then frees the array*/
void freeTable(hashTable *table) 
{

  int i;
  input *tmp = NULL, *tmp2 = NULL;

  for(i = 0; i < table->tableLen; i++){
    if(tableNotNull(table, i)){
      tmp = table->hTable[i]->next; 
      while(tmp != NULL){
        free(tmp->string);
        tmp2 = tmp;
        tmp = tmp->next;
        free(tmp2);
      }
    } 
    free(table->hTable[i]);
  }

  free(table);

}

/*check in place in below code to ensure that not
  comparing str with a NULL placement in the table
  - if writing code to return if an item is not in the
  table this could be change to return the string with 
  a statement that it is not found*/
int findStr(char *str, hashTable *table)
{

  unsigned int h = 0;
  double move = 0;
  input *tmp = NULL;

  h = hash(str, table);

  if((tableNotNull(table, h)) && 
    (strsame(table->hTable[h]->string, str))){ 
    move++;
  }
  else{
    tmp = table->hTable[h];
    while((tmp != NULL) && (strdiff(tmp->string, str))){
      tmp = tmp->next;
      move++;   
    }
  }
  
  return move;

}











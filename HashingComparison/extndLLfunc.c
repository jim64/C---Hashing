#include "extnd.h"

/*function which drives all other functions*/
void ShashActions(FILE* file, FILE* file2, int flag, 
     hashTable *tab, result* res, char** argv)
{

  clock_t c1, c2;
  double speed = 0.0;
  char view;

  file = fopen(argv[1], "r");
  file2 = fopen(argv[2], "r");

  res = initialiseResult(res);
  tab = fillTable(file, tab, res, flag);
  c1 = clock();
  res = searchTable(tab, file2, res); 
  c2 = clock();
  speed = (double)(c2-c1)/CSEC;
  updateOutcomes(tab, res, speed);
  
  printf("View table hot-spots? (y or n):");
  if(scanf(" %c", &view) != TRUE){
    ERROR("Cannot read response", erInput);
  }
  if(view == 'y'){
    printMap(res);
  } 
  
  free(res);
  freeTable(tab);
  fclose(file);
  fclose(file2);

}

/*switches hash functions dependent on flag input*/
unsigned int hashSwitch(hashTable *tab, char *str)
{

  unsigned int hash = 0;

  switch(tab->flag){
    case 0:
      hash = hashDhash(str);
      break;
    case 1:
      hash = myHash(str, tab);
      break;
    case 2:
      hash = hashDJB2(str, tab);
      break;
    case 3:
      hash = hashFNV1a(str, tab);
      break;
    default:
      ERROR("No flag set\n", erNoFlag);
   }
      
  return hash;
  
}

/*check in place in below code to ensure that not
  comparing str with a NULL placement in the table
  - if writing code to return if an item is not in the
  table this would be change to return the string with 
  a statement that it is not found*/
result* findStr(char *str, hashTable *table,
        result *res)
{

  unsigned int h = 0;
  double move = 0;
  input *tmp = NULL;

  h = hashSwitch(table, str);
  h = h % table->tableLen;

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

  res->noOfMoves += move;

  return res;

}


void rehashAllocate(hashTable *newTable, int h, char *str,
     result *res)
{

  if(tableNotNull(newTable, h)){
    addLink(newTable, h, str, res);
  }
  else{
    insertStr(str, newTable, h);
  }  

}

void reallocateList(hashTable *table, hashTable *newTable,
     int i, result *res)
{

  input *tmp = NULL;
  unsigned int h = 0;

  tmp = table->hTable[i];
  
  while(tmp != NULL){
    h = hashSwitch(table, tmp->string);
    h = h % table->tableLen;
    rehashAllocate(newTable, h, tmp->string, res);
    tmp = tmp->next;
  }  

} 


hashTable* rehash(hashTable *table, result *res)
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
        h = hashSwitch(table, table->hTable[i]->string);
        h = h % table->tableLen;
        rehashAllocate(new,h,table->hTable[i]->string,res);
      }
      /*ensures that previous linked lists are put into
        new table*/
      else{
        reallocateList(table, new, i, res);
      }
      res->collisions++;
    }
  }

  new->flag = table->flag;
  freeTable(table);
  table = new;

  return table;

}


void insertStr(char *str, hashTable *table, int h)
{

  table->hTable[h] = Allocate(str); 
  table->words++;

}


void addLink(hashTable *table, unsigned int h, char *str,
     result* res)
{
  
  input *tmp = NULL;
  
  tmp = table->hTable[h];

  while(tmp->next != NULL){
    res->collisions++;
    tmp = tmp->next;
  }
  
  tmp->next = Allocate(str); 
  table->words++;
   
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

/*assert used below to stop str overflow of buffer*/
hashTable* fillTable(FILE *file, hashTable *table,
           result *res, int flag) 
{

  char str[BUFFER] = {0};
  int num = INITIAL_SIZE;

  table = initialiseTable(num, table);
  table->flag = flag;

  while(fscanf(file, "%s", str) == TRUE){
    assert(strlen(str) < BUFFER);
    table = placeInTable(str, table, res);
  }

  return table;

}

/*assert used below to stop str overflow of buffer*/
result* searchTable(hashTable *table, FILE *file2,
        result *res)
{

  char str[BUFFER] = {0};
  int cnt = 0;
  
  while(fscanf(file2,"%s", str) == TRUE){
    assert(strlen(str) < BUFFER);
    res = findStr(str, table, res);
    cnt++; 
  } 

  res->noOfMoves = res->noOfMoves / cnt;
    
  return res;

}

hashTable* placeInTable(char *str, hashTable *table,
           result *res)
{

  unsigned int h = 0;

  h = hashSwitch(table, str);
  h = h % table->tableLen;

  if(tableNotNull(table, h)){ 
    /*this creates / adds to linked list if space already
      full*/ 
    addLink(table, h, str, res);  
  }  
  else{
    insertStr(str, table, h);
  }

  /*makes new table when current table is 60% full
   - this can be ammended in the header file*/
  if(table->words == (SIXTY_PER_CENT(table->tableLen))){ 
    table = rehash(table, res); 
    res->collisions = 0;
  }

  return table;

}


/*includes all specific double hashing functions*/

#include "extnd.h" 

void DhashActions(FILE* file, FILE* file2, int flag, 
     hashTable *tab, result* res, char** argv)
{
 
  clock_t c1, c2;
  double speed = 0.0; 
  char view;

  file = fopen(argv[1], "r");
  file2 = fopen(argv[2], "r");

  res = initialiseResult(res);
  tab = fillTableDhash(file, tab, res, flag);
  c1 = clock();
  res = searchTableDhash(tab, file2, res);
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

  freeTable(tab); 
  fclose(file);
  fclose(file2);

}

/*cases used to switch between different hashing options*/
unsigned int DhashSwitch(hashTable *tab, char *str)
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
  

/*assert used below to stop str overflow of buffer*/
result* searchTableDhash(hashTable *table, FILE *file2,
        result *DResult)
{

  char str[BUFFER] = {0};
  int cnt = 0;
  double move = 0.0;
  
  while(fscanf(file2,"%s", str) == TRUE){
    assert(strlen(str) < BUFFER);
    move += findStrDhash(str, table);
    cnt++; 
  } 

  move = move/cnt;

  DResult->noOfMoves = move;
    
  return DResult;

}

/*check in place in below code to ensure that not
  comparing str with a NULL placement in the table
  - if writing code to return if an item is not in the
  table this would be change to return the string with 
  a statement that it is not found*/
int findStrDhash(char *str, hashTable *table)
{

  unsigned int h = 0;
  double move = 1.0;

  h = DhashSwitch(table, str); 
  h = h % table->tableLen;

  if((tableNotNull(table, h)) && 
    (strsame(table->hTable[h]->string, str))){
    move++;
  }
  else{
    /*below keeps stepping through table until it finds 
      the string or a NULL place in the table*/
    while((tableNotNull(table, h)) && 
      (strdiff(table->hTable[h]->string, str))){ 
      h += doubleHash(str, table);
      h = (h % table->tableLen);
      move++;   
    }
  }

  return move;

}

hashTable* placeInTableDhash(char *str, hashTable *table,
           result *res)
{

  unsigned int h = 0;
  
  h = DhashSwitch(table, str);
  h = h % table->tableLen;

  while(tableNotNull(table, h)){
    h += doubleHash(str, table);
    h = (h % table->tableLen);
    res->collisions++;
  }  

  insertStr(str, table, h);

  /*the magic numbers below appear as this function will
    not work with the #define macro of SIXTY_PER_CENT
    the macro is not recognised and so this becomes stuck
    in a while loop. The macro does work in the chained
    list version*/
  if((table->words == table->tableLen / 5) * 3){ 
    res->collisions = 0;
    table = rehashD(table, res); 
  }

  return table;

}

/*double hash function remains the same in all examples
  for clarity of results.*/
unsigned int doubleHash(char *str, hashTable *table)
{

  unsigned int c1, c2, cn1, cn2;
  unsigned int hash;
  unsigned int n;

  n = (unsigned int)strlen(str);

  c1 = str[0];
  c2 = str[1];
  cn1 = str[n-1];
  cn2 = str[n-2];
  
  hash = ((c1*c2+cn1*cn2)*n) % table->tableLen;

  return(hash == 0) ? 1:hash;

}

/*original hash used from assignment notes*/
unsigned int hashDhash(char *str)
{

  short int i;
  short int n = strlen(str);
  unsigned int hash = 0;
  
  for(i = 0; i < n; i++){
    hash = str[i] + PRIME * hash;
  }

  return(hash == 0) ? 1:hash;

}

/*rehashing of table when table size has been changed*/
hashTable* rehashD(hashTable *table, result* res)
{

  hashTable *new = NULL;
  unsigned int h = 0;
  int i;
  int next_size = newPrime(table);

  new = initialiseTable(next_size, new);

  for(i = 0; i < table->tableLen; i++){
    if(tableNotNull(table, i)){
      h = DhashSwitch(table, table->hTable[i]->string);
      h = h % table->tableLen;
      while(tableNotNull(new, h)){
        h += doubleHash(table->hTable[i]->string, table);
        h = (h % table->tableLen);
        res->collisions++;        
      }
    insertStr(table->hTable[i]->string, new, h);
    }
  }
  
  new->flag = table->flag;
  freeTable(table);
  table = new;

  return table;

}

hashTable* fillTableDhash(FILE *file, hashTable *table,
           result *res, int flag) 
{

  char str[BUFFER] = {0};
  int num = INITIAL_SIZE;

  table = initialiseTable(num, table);

  table->flag = flag;

  while(fscanf(file, "%s", str) == TRUE){
    assert(strlen(str) < BUFFER);
    table = placeInTableDhash(str, table, res);
  }

  return table;

}



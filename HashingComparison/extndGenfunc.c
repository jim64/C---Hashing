/*file includes functions which are generic - used by both
  linked list and Double hashing*/

#include "extnd.h"

void printMap(result* res)
{
  int i;        

  for(i = 1; i <= res->mapLen; i++){
    printf("%d\t", res->map[i]);
    if(i % PRINTROW == 0){
      printf("\n");
    }
  }

  printf("\n\n\n\n");

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

/*frees both cell of the table and also any linked list 
  from that cell. Then frees the array. This can be used
  with both linked list and double hash as all double hash
  have a null pointer which is not used, therefore, the
  tmp is the while loop is always pointing at NULL*/
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

result* initialiseResult(result* res)
{

  if((res = (result*)calloc(1,sizeof(result))) == NULL){
    ERROR("Cant allocate", erAllocRes); 
  }

  return res;

}

result* initialiseMap(result *res, int k)
{

  if((res->map = (int*)calloc(k, sizeof(int))) == NULL){
    ERROR("Cant allocate", erAllocMap);
  }

  return res;

}

/* puts all relevant results into a result structure,
   checks the size of the tableLen and adjusts the size
   of the result map to ensure readability when printed*/
void updateOutcomes(hashTable* table, result *res,
     double speed)
{

  int i, j = 0, cnt = 0, k = 0;
  input *tmp = NULL;
  int n;
  
  if(table->tableLen > MAPBLOCK_RESIZE){
    n = NEW_MAPBLOCK;
  }
  else{
    n = MAPBLOCK;
  }

  res->time = speed;
  k = table->tableLen / n;
  res = initialiseMap(res, k);
  res->mapLen = k;

  /*divides table into smaller array to give locations of
    collisions. Ignores final part of array as map will be
    generalised. This leads to some lost data but in 
    practice not much is lost as most strings are being
    placed in lower part of hash table with all hashing
    functions*/
  for(i=1;i<(table->tableLen-(table->tableLen % n));i++){
    if(i % n != 0){
      if(table->hTable[i] != NULL){
        cnt++;
        tmp = table->hTable[i];
        while(tmp->next != NULL){
          cnt++;
          tmp = tmp->next;
        }
      }
    }
    else{
      res->map[j] = cnt;
      cnt = 0;
      j++;
    }
  }

  printf("\nlook-ups = %f\n", res->noOfMoves);
  printf("time taken to search = %f\n", res->time);
  printf("collisions = %d\n", res->collisions);

} 


void checkInputCorrect(int argc, FILE *file, 
     FILE *file2, char **argv)
{

  if(argc != CORRECT_LEN){
    ERROR("Two text files required to run program", erLen);
  }

  if(file == NULL){
    ERROR_INPUT(argv[1], erNull);
  }

  if(file2 == NULL){
    ERROR_INPUT(argv[2], erNull);
  }

}

hashTable* initialiseTable(int num, hashTable *table)
{

  hashTable *tmp = NULL;

  if((tmp = (hashTable*)calloc(1, sizeof(hashTable)))
     == NULL){ERROR("Cannot allocate array", erAllocTab);
  }

  table = tmp;

  if((table->hTable = (input**)calloc(num,
    sizeof(input))) == NULL){
    ERROR("Cannot allocate array", erAllocArr);
  }

  table->tableLen = num;

  return tmp;

}

int tableNotNull(hashTable *table, int h)
{

  return(table->hTable[h] != NULL) ? TRUE:FALSE;

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







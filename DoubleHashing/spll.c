#include "dhash.h"

int main(int argc, char **argv)
{

  FILE *file = NULL;
  FILE *file2 = NULL;
  hashTable *table = NULL;
  double lookUp = 0.0;
  
  file = fopen(argv[1], "r");
  file2 = fopen(argv[2], "r");
  checkInputCorrect(argc, file, file2, argv);
   
  table = fillTable(file, table);
  fclose(file);

  lookUp = searchTable(table, file2); 
  printf("Average look-ups required = %f\n", lookUp);
  
  fclose(file2);
  freeTable(table);

  return 0;

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


/*assert used below to stop str overflow of buffer*/
hashTable* fillTable(FILE *file, hashTable *table) 
{

  char str[BUFFER] = {0};
  int num = INITIAL_SIZE;

  table = initialiseTable(num, table);

  while(fscanf(file, "%s", str) == TRUE){
    assert(strlen(str) < BUFFER);
    table = placeInTable(str, table);
  }

  return table;

}

/*assert used below to stop str overflow of buffer*/
double searchTable(hashTable *table, FILE *file2)
{

  char str[BUFFER] = {0};
  int cnt = 0;
  double move = 0;
  
  while(fscanf(file2,"%s", str) == TRUE){
    assert(strlen(str) < BUFFER);
    move += findStr(str, table);
    cnt++; 
  } 

  move = move / cnt;
    
  return move;

}



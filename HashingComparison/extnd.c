#include "extnd.h"

/*this uses different hash functions for both linked list 
  and double hashing to compare the results. It gives the user the option
  to print out the hot-spot map*/
int main(int argc, char **argv)
{

  FILE *file = NULL;
  FILE *file2 = NULL;
  hashTable *tab = NULL;
  result *res = NULL;
  int flag = 0;

  file = fopen(argv[1], "r");
  file2 = fopen(argv[2], "r");
  checkInputCorrect(argc, file, file2, argv);

  printf("\nDouble hashing with hash-all function\n");
  DhashActions(file, file2, flag, tab, res, argv);
  
  flag = 1;  
  printf("\nDouble hashing with myHash function\n");
  DhashActions(file, file2, flag, tab, res, argv);
  
  flag = 2; 
  printf("\nDouble hashing with DJB2 function\n");
  DhashActions(file, file2, flag, tab, res, argv);
  
  flag = 3;
  printf("\nDouble hashing with FNV1a function\n");
  DhashActions(file, file2, flag, tab, res, argv);

  flag = 0;
  printf("\nChained list hashing with hash-all function\n");
  ShashActions(file, file2, flag, tab, res, argv);

  flag = 1;
  printf("\nChained list hashing with myHash function\n");
  ShashActions(file, file2, flag, tab, res, argv);
  
  flag = 2;
  printf("\nChained list hashing with DJB2 hash function\n");
  ShashActions(file, file2, flag, tab, res, argv);
    
  flag = 3;
  printf("\nChained list hashing with FNV1a hash function\n");
  ShashActions(file, file2, flag, tab, res, argv);
  

  return 0;

}





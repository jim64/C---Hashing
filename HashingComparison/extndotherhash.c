/*includes 3 seperate hash functions to test against*/

#include "extnd.h" 


/*this algorithm found at 
http://www.cse.yorku.ca/~oz/hash.html
written by Dan Bernstein. It is widely reported to be
a fast string hash function that creates few collisions
- magic numbers left in to show clear details of the
hash functions*/

unsigned int hashDJB2(char *str, hashTable* table)
{

  unsigned int hash = 5381;
  unsigned int i;

  for(i = 0; i < strlen(str); i++){
    hash = ((hash << 5) + hash) + (*str++);
  }

  hash = (hash * PRIME) % table->tableLen;

  return (hash <= 0) ? 1:hash;

}

unsigned int myHash(char *str, hashTable* table)
{
  unsigned int hash = 0, n, end;
  unsigned int i;

  n = (unsigned int)strlen(str);

  end = str[n-1];

  for(i = 0; i < n ; i++){
    hash += (((str[i] * PRIME) +(str[end] * PRIME)));
    end--;
  }
  
  hash = (hash * PRIME) % table->tableLen;

  return (hash <= 0) ? 1:hash;

}

/*this algorithm found at 
http://www.partow.net/programming/hashfunctions/
It was created by Glenn Fowler, Landon Curt Noll and 
Kiem-Phong Vo. It is designed to be fast with low 
collision rate*/
unsigned int hashFNV1a(char* str, hashTable* table)
{
   const unsigned int fnv_prime = 0x811C9DC5;
   unsigned int hash      = 0;
   unsigned int i         = 0;
   unsigned int n = strlen(str) - 1;

   for(i = 0; i < n; i++)
   {
      hash ^= (*str);
      hash *= fnv_prime;
      str++;
   }
   hash = (hash * PRIME) % table->tableLen;

   return hash;
}

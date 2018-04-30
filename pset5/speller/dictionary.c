// Implements a dictionary's functionality
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <assert.h>
#include <stdint.h>

#include "dictionary.h"
#define DJM             // for debugging


typedef struct _DICT {
    int size;
    char *buf;
} DICT;

typedef struct _NODE {
    char *addr;
    struct _NODE *next;
} NODE;

typedef struct _HEAD {  // Head pointers for hash array
    int cnt;
    NODE *pNode;
} HEAD;

typedef unsigned hkey;

#define LOG printf ("fn:%s() ln:%d in %s\t",\
                      __func__, __LINE__, __FILE__);

#define WAIT {puts("<any key>"); getchar();}

#define HASHSIZE 143477 //105557   // size of hash table. This is a prime for mathy reasons beyond the
                        // scope of this problem and my brain. 7919 is the 1000th prime fwiw.

DICT dict;              // The Dictionary we build

HEAD HASH[HASHSIZE];    // The Hash Table we use. An array of singly linked lists


/*****
 * *                    InitHashTable()
 * * */
void InitHashtable(void)
{
    HEAD *p = &HASH[0];

    for(int x=0; x<HASHSIZE; x++)
    {
        p->pNode = NULL;
        p->cnt = 0;
    }
}

/* stolen from MurmurHash : https://github.com/aappleby/smhasher */
hkey GetHashKey(const void *key)
{
  const unsigned int m = 0xc6a4a793;    // anything
  const int r = 16;
  int len = strlen((char *)key);
  unsigned int seed = 0x43770;          // anything
  const unsigned char * data = (const unsigned char *)key;

  unsigned int h = seed ^ (len * m);

  int align = (uint64_t)data & 3;

  if(align && (len >= 4))
  {
    // Pre-load the temp registers

    unsigned int t = 0, d = 0;

    switch(align)
    {
      case 1: t |= data[2] << 16;
      case 2: t |= data[1] << 8;
      case 3: t |= data[0];
    }

    t <<= (8 * align);

    data += 4-align;
    len -= 4-align;

    int sl = 8 * (4-align);
    int sr = 8 * align;

    // Mix

    while(len >= 4)
    {
      d = *(unsigned int *)data;
      t = (t >> sr) | (d << sl);
      h += t;
      h *= m;
      h ^= h >> r;
      t = d;

      data += 4;
      len -= 4;
    }

    // Handle leftover data in temp registers

    int pack = len < align ? len : align;

    d = 0;

    switch(pack)
    {
    case 3: d |= data[2] << 16;
    case 2: d |= data[1] << 8;
    case 1: d |= data[0];
    case 0: h += (t >> sr) | (d << sl);
        h *= m;
        h ^= h >> r;
    }

    data += pack;
    len -= pack;
  }
  else
  {
    while(len >= 4)
    {
      h += *(unsigned int *)data;
      h *= m;
      h ^= h >> r;

      data += 4;
      len -= 4;
    }
  }

  //----------
  // Handle tail bytes

  switch(len)
  {
  case 3: h += data[2] << 16;
  case 2: h += data[1] << 8;
  case 1: h += data[0];
      h *= m;
      h ^= h >> r;
  };

  h *= m;
  h ^= h >> 10;
  h *= m;
  h ^= h >> 17;

  return (hkey)(h % HASHSIZE);
} // end of hash func


static inline HEAD * GetHeadPtr(hkey key)
{
    return &HASH[key];
}


/***                    CreateNode()
 *  allocate and init a node, set data to str; next = NULL
 *  Returns NODE * or NULL on error
 */
NODE* CreateNode(char *str)
{
    NODE* np = (NODE*)malloc(sizeof(NODE));
    if(np == NULL)
    {
        LOG printf("malloc error in CreateNode()\n");
        return NULL;
    }
    np->addr = str;
    np->next = NULL;

    return np;
}

/***                    LookupNode()
 *find a node
 * return NODE * if found, else NULL
 */
NODE *LookupNode(char *s, HEAD *pHEAD)
{
    if(pHEAD->pNode == NULL)    // this list is empty
        return NULL;

    NODE *np = pHEAD->pNode;

    while(np != NULL)
        {
        if (0 == strcmp(s, np->addr))
          return np;    /* found */
        np = np->next;
        }
    return NULL;        /* not found */
}


/***                    InsertNode()
 * Insert node at head
 *  new->next = Head
 *  replace HEAD with new
 */
bool InsertNode(NODE *pNode, HEAD *pHEAD)
{
    pNode->next = pHEAD->pNode;
    pHEAD->pNode = pNode;
    pHEAD->cnt++;
    return true;
}

/*****
 * *                    check()
 * * */
// Returns true if word found in dictionary else false
bool check(const char *word)
{
static bool RC;
static char* p;
static hkey key;
static HEAD *pHEAD;

char *tmp = malloc(strlen(word) + 1);
// make a copy and lowercase it
strcpy(tmp, word);
p = tmp;

for (; *p; ++p)
    {
        *p = tolower(*p);
    }

key = GetHashKey(tmp);

pHEAD = GetHeadPtr(key);
//assert(pHEAD);

if(LookupNode(tmp, pHEAD) == NULL)     //Not found
    RC =  false;
else
    RC = true;

free(tmp);
return RC;
}

/*****
 * *                    load()
 * * */
// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
dict.size = 0;
InitHashtable();

//LOG printf("->Opening dict %s\n", dictionary);

FILE *fileDict = fopen(dictionary, "r");
if(fileDict == NULL)
    {
    LOG puts("Can't fopen(dictionary file)\n");
    return false;
    }

// Get length of dictionary file
fseek(fileDict, 0L, SEEK_END);    /* Position to end of file */
long lFileLen = ftell(fileDict);  /* Get file length */
rewind(fileDict);                 /* Back to start of file */

/* Read the entire dictionary file into memory */
dict.buf = calloc(lFileLen + 1, sizeof(char));
//DJM printf("Inputfilesize is %ld\n", lFileLen);

if(dict.buf == NULL )
    {
    LOG printf("\nInsufficient memory to read file.\n");
    return false;
    }

fread(dict.buf, lFileLen, 1, fileDict); /* Read the entire file into DICT */

// Now we're working from memory...

char *pMem=dict.buf;    // ptr into to DICT text
char *endMem = dict.buf + lFileLen;  // ptr to end of buffer

char *strStart;
do
{
    strStart = pMem;    // save the begin of str in DICT

    // find end of string so we can null term it
    while(*pMem > 0x0d) // input strings break at newlines
    {
         pMem++;
    }
    *(pMem++) = 0;  // null terminate both

    hkey key = GetHashKey(strStart);
    HEAD *pHEAD = GetHeadPtr(key);

    // if not in list then create it and update count
//DJM printf("Found Dict word %s(keyval %d)\n", strStart, (int)key);
    if(strlen(strStart))
        {
        NODE *pNode = CreateNode(strStart);
        //assert (pNode);

        if (InsertNode(pNode, pHEAD))
            dict.size++;
        }
} while(pMem < endMem);   // while < endBuf

// Cleanup
fclose (fileDict);

//LOG printf("load() read %i lines from %s\n", dict.size, dictionary);

#if 0
DJM // dump the hash table
for(int z = 0; z<HASHSIZE; z++)
{
    NODE *p = HASH[z].pNode;

    printf("\nHASH[%d] %d elements\n", z, HASH[z].cnt);

    if(p != NULL)
        do
        {
            printf("\t[%s]", p->addr);
            p = p->next;
        }
        while (p != NULL);
}
DJM WAIT
#endif

return true;
}

/*****
 * *                    size()
 * * */
// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    /*
    DJM // alt method
    int size = 0;
    for(int x = 0; x< HASHSIZE; x++)
        size += HASH[x].cnt;
    printf ("size() 1:%d, 2:%d\n", size, dict.size);
    DJM
    */
return dict.size;
}

/*
static int FreeNode(NODE *p)
{
    assert(p);

    if(p->next)
        {
        if (p->next->next)
            {
            return FreeNode(p->next);
            }
        else
            {
            free(p->next);
            p->next = NULL;
            }
        }

    else if(NULL != p)
        {
            free(p);    // if list only has 1 element
            p = NULL;
        }
    return 0;
}
*/

/*****
 * *                    unload()
 * * */
// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
NODE *p, *tmp;

    // Walk each list and unload from the end
    for(int x=0; x<HASHSIZE; x++)
    {
//DJM printf("Freeing HASH[%d]\t", x);
        HEAD *pH = &HASH[x];

        p = pH->pNode;

        while (NULL != p)
            {
                tmp = p;
                p = p->next;
                free(tmp);
            }
//        if(NULL != p->pNode)        // could possibly have a list with no entries
//            FreeNode(p->pNode);         // This will recurse through each list
//                                    // At least it works that way in my dreams....
    }

    free(dict.buf);
    dict.buf = NULL;

return true;
}

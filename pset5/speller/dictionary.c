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
    NODE *pNode;
} HEAD;

typedef unsigned hkey;

#define LOG printf ("fn:%s() ln:%d in %s\t",\
                      __func__, __LINE__, __FILE__);

#define WAIT {puts("<any key>"); getchar();}
//2^16	65536
//2^17	131072
//2^18	262144
#define HASHSIZE 262144 // 143477, 105557   // size of hash table. This is a prime for mathy reasons beyond the scope of this problem and my brain
#define HASHMASK 262143                    // turn all bit on 1 less than HASHSIZE (which MUST BE A Power of 2)

DICT dict;              // The Dictionary we build

HEAD HASH[HASHSIZE];    // The Hash Table we use. An array of singly linked lists

// some crappy hash fn found on interwebs. This is secondary though an optimal fn is sorta important
unsigned int GetHashKey(const char* word)
{
register unsigned int hash = 131; //1315423911;

while (*word)
    {
    hash ^= ((hash << 5) + (*word) + (hash >> 3));
    word++;
    }

#if 0
    return (hash % HASHSIZE);
#else
    return (hash & HASHMASK);
#endif
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



/*****
 * *                    check()
 * * */
// Returns true if word found in dictionary else false
bool check(const char *word)
{
register char* p;
static NODE *np;
static char temp[LENGTH];

// make a copy and lowercase it
/*
register char *tmp;
tmp = malloc(strlen(word) + 1);
strcpy(tmp, word);
p = tmp;
for (; *p; ++p)
    //if(*p>='A')
        *p = tolower(*p);
*/
strcpy(temp, word);
p = temp;
for (; *p; ++p)
    //if(*p>='A')
        *p = tolower(*p);


unsigned int key = GetHashKey(temp);
//printf("%u|", key);

if ((&HASH[key])->pNode != NULL)   // something in this list
    {
    np = (&HASH[key])->pNode;
    while(np != NULL)    // this list is not empty
        {
        if (0 == strcmp(temp, np->addr))
            {
            //free(tmp);
            return true;    // found
                        }
        np = np->next;
        }
    }
//free(tmp);
return false;
}

/*****
 * *                    load()
 * * */
// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
dict.size = 0;

memset(HASH, 0, HASHSIZE*sizeof(HEAD));  //InitHashtable();

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
HEAD * pHEAD;

char *strStart;
do
{
    if(!(*pMem))
    {
        pMem++;
        continue;
    }

    strStart = pMem;    // save the begin of str in DICT

    // find end of string so we can null term it
    while(*pMem > 0x0d) // input strings break at newlines
    {
         pMem++;
    }
    *(pMem) = 0;  // null terminate both
    pMem++;

    /* Create it, insert it and update count
    */
    if(*strStart) // not NULL
        {
        unsigned int key = GetHashKey(strStart);
        pHEAD = &HASH[key];

        NODE *pNode = CreateNode(strStart);

        // INSERT node at HEAD position
        pNode->next = pHEAD->pNode;     // new->next to current head
        pHEAD->pNode = pNode;           // then HEAD is the new guy
        dict.size++;
        }
} while(pMem < endMem);   // while < endBuf

// Cleanup
fclose (fileDict);

//LOG printf("load() read %i lines from %s\n", dict.size, dictionary);


return true;
}

/*****
 * *                    size()
 * * */
// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
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

// Declares a dictionary's functionality

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// Maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

#define LOG printf ("fn:%s() ln:%d in %s\t",\
                      __func__, __LINE__, __FILE__);

// Prototypes
bool check(const char *word);
bool load(const char *dictionary);
unsigned int size(void);
bool unload(void);

typedef struct _DICT {
    int size;
    char *buf;
} DICT;

typedef struct _NODE {
    char *addr;
    struct _NODE *next;
} NODE;

typedef struct _HEAD {  // Head pointers for hash array
    int cnt;catls di
    NODE *pNode;
} HEAD;

typedef unsigned hkey;

#endif // DICTIONARY_H

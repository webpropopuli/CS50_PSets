#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define logi(x) printf("%d\n", x);
#define logs(s) printf("%s\n", s);

// Vigenere's. based on Caesar
int main(int argc, char *argv[])
{
    char *strPlain;
    char strCipher[512] = {0};       // spec says nothing but I think malloc is past curr. class level

    // Only 1 arg allowed
    if (argc != 2)
    {
        printf("Wrong number of arguments. Exiting...\n");
        return 1;
    }


	char *keyval = argv[1];
    int iKeys[128] = {0};
    int lenKey = 0;
    int ndx = 0;   //index into key array, i.e. the letter we are currently 'on'

    //convert each key letter first to lower, then to it's offset from 'a'
    //printf("key is %s\n", keyval);
	for (int x = 0; x < strlen(keyval); x++, lenKey++)
	{
	    if (isalpha(keyval[x]))
	        {
	            iKeys[x] = tolower(keyval[x]) - 'a';
	            //printf("%d\n",iKeys[x]);
	        }
	    else
	        {
	            printf("Non-alpha char in %s\n", argv[1]);
	            return 1;
	        }
	}


    // Get plaintext from user
    printf("plaintext:");
    strPlain = get_string();

    int chr;   //current char in plaintest
    int shift;  // how far to shift from 'A' to 'a'
    bool wasLower;

    // Shift each letter to cipher array
    int len = strlen(strPlain);
    for (int x = 0; x < len; x++)
    {
        wasLower = islower(strPlain[x]);
        chr = toupper(strPlain[x]);

        if (!isalpha(chr))
        {
            strCipher[x] = chr;
        }
        else
            {
            shift = iKeys[ndx++ % lenKey];

            strCipher[x] = (((chr - 'A') + shift) % 26) + 'A';

            if (wasLower)
                {
                    strCipher[x] += ('a' - 'A');
                }
            }
    }

    printf("ciphertext:%s\n", strCipher);

    return (0);
}
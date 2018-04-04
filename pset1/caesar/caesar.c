#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define logi(x) printf("%d\n", x);


int main(int argc, char *argv[])
{
char *strPlain;
char strCipher[512] = {0};;       // spec says nothing but I think malloc is past curr. class level

	// Only 1 arg allowed
    if (argc != 2)
    {
        printf("Wrong number of arguments. Exiting...\n");
		return 1;
	}
	int keyval = atoi(argv[1]) % 26;
	//logi(keyval)

    // Get plaintext from user
    printf("Plaintext:");
    strPlain = get_string();


    int chr;   //current char in plaintest

    int shift;  // how far to shift from 'A' to 'a'

    // Shift each letter to cipher array
    int len = strlen(strPlain);
    for (int x = 0; x < len; x++)
    {
        chr = strPlain[x];

        if(!isalpha(chr))
        {
            strCipher[x] = chr;
        }
        else
            {
            if (islower(chr))
                shift = 'a';
            else
                shift = 'A';

            strCipher[x] = ((((chr - shift) + keyval) % 26)) + shift;
            }
    }

    printf("ciphertext: %s\n", strCipher);

return (0);
}
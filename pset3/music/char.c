/* scraps */
char str[40] = {0};

if(x == 5)
	printf("X is %d\n", 5);
else if (x == 56)
	printf("X is %d\n", 56)

------------------

printf("X is %d\n", x)

----------------

if( (x == 5) !! (x == 56) )
	printf("X is %d\n", x);






















#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <cs50.h>

int main(int argc, char *argv[])
{
	char *note = argv[argc - 1];
	int Octave;
	char Note = 0;
	char Mod = 0;

    int nVal = 99;
	// get tones
	/*      Break str into 2  or 3 parts
	XYZ where X = A-G; Y = #/b (optional; Z = 0-8)
	For now, will compare strle=2 or 3. I think there's an optional param option
	to scanf but I'm not learning it first
	*/
	if (2 == strlen(note))
	{   //grab tone and octave
		sscanf(note, "%1c%1i", &Note, &Octave);
	}
	else if (3 == strlen(note))
	{   //grab tone modifier and active
		sscanf(note, "%1c%1c%1i", &Note, &Mod, &Octave);
	}
	else {
		return 1;
		//add error handler
	}

	printf("Octave=%d\n", Octave);
	printf("Note=%c\n", Note);
	printf("Mod = %c\n", Mod);


// Convert char into place in A=based scale
	switch (toupper(Note))
	    {
	        case 'A':
	            nVal = 0;
	            break;
	        case 'B':
	            nVal = 2;
	            break;
	        case 'C':
	            nVal = 3;
	            break;
	        case 'D':
	            nVal = 5;
	            break;
	        case 'E':
	            nVal = 7;
	            break;
	        case 'F':
	            nVal = 8;
	            break;
	        case 'G':
	            nVal = 10;
	            break;
	       default:
	        // Bad note. Handle error
	        return 1;

	    }

// Sharps and flats go + or =
	if (Mod != 0)
	    {
	    if(Mod == '#')
	        nVal++;
	    else if (Mod == 'b')
	        nVal--;
	    else
            {
                // error invalid mod
                return 1;
            }
	    }

// convert for octave
	printf("nVal pre-octave %d\n", nVal);
nVal = (Octave-4)*12 + nVal;

	printf("Final nVal is %d\n", nVal);
return round( pow(2,nVal/12.000) * 440);

	//puts("'Q' to exit");
	//getchar();
	return 0;
}
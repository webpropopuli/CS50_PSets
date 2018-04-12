// Helper functions for music

#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    int top;
    int bot;

    sscanf(fraction, "%1i/%1i", &top, &bot);

    top *= (8 / bot);

    return top;
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
	int Octave;
	char Note = 0;
	char Mod = 0;   // = 'modifier', i.e. sharp or flat

	   int nVal = 99;
	// get tones
	/* Break str into 2  or 3 parts
	XYZ where X = A-G; Y = #/b (optional; Z = 0-8)
	For now, will compare strle=2 or 3. Not sure if I can have an optional
	param in sscanf so this will do for now
	*/
	if (2 == strlen(note))
	{
	    //grab tone and octave
		sscanf(note, "%1c%1i", &Note, &Octave);
	}
	else if (3 == strlen(note))
	{   //grab tone modifier and active
		sscanf(note, "%1c%1c%1i", &Note, &Mod, &Octave);
	}
	else
	{
	       return 0;
	       // add error handler
	}


// Convert char into place in A=based scale
	switch (toupper(Note))
	    {
	        case 'C':
	            nVal = 0;
	            break;
	        case 'D':
	            nVal = 2;
	            break;
	        case 'E':
	            nVal = 4;
	            break;
	        case 'F':
	            nVal = 5;
	            break;
	        case 'G':
	            nVal = 7;
	            break;
	        case 'A':
	            nVal = 9;
	            break;
	        case 'B':
	            nVal = 11;
	            break;
	       default:
	               // Bad note. Handle error
	            return 0;

	    }

// Sharps and flats go + or =
	if (Mod != 0)
	    {
	    if(Mod == '#')
	        {
	        nVal++;

	        // Bump Octave if we sharp a B
	        if (nVal > 11)
	            {
	                nVal = 0;
	                Octave++;
	            }
	        }
	    else if (Mod == 'b')
	        {
	        nVal--;
	        // Bump Octave if we sharp a B
	        if (nVal < 0)
	            {
	                nVal = 11;
	                Octave--;
	            }
	        }
	    else
	           {
	               // error invalid mod
	               return 0;
	           }
	    }

/* convert for octave
	 Part of me wants to hardcode the freqs into an array,
	 which has to be faster than calculating each note, but
	 they seemed so proud of their "formula", I thought I'd
	 use it a lot and give out some Harvard-love, but really,
	 I wouldn't calc each note at run-time for under 100 possible vals.

	 Then again, I type notes like this to myself so what do I know?
*/
	   // -9 is offset from A back to C. This feels hideous but...
	   nVal = (Octave - 4) * 12 + nVal - 9;

	   return round(pow(2, nVal / 12.000) * 440);
}



// Determines whether a string represents a rest
bool is_rest(string s)
{
    return strlen(s) == 0;
}

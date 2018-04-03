#include <stdio.h>
#include <math.h>
#include <cs50.h>

#define IsEven(x) !(x % 2)
#define CARD_INVALID 0
#define CARD_AMEX 1
#define CARD_MC 2
#define CARD_VISA 3

int main(void)
{
char *cardTypes[] = {"INVALID", "AMEX", "MASTERCARD", "VISA" };

long long powX;
int digit = 0;          // the current digit value
int cksumTotal = 0;     // accumulator for checksum
int cardType = 0;       // AMEX, MC, etc
int x = 0;              // exponent for current power of 2
int n=0;                // a counter
int first2 = 0;         // first 2 digits of card
long long testNum = 0;   // user input
//long long testNum =  4062901840;    //test input
printf("Number: ");

if (testNum == 0)
    testNum = get_long_long();  // get user input unless I give test value

// Find first power of 10 larger than number
do {
    x++;
} while (testNum >= pow(10, x));

// if not at least 13 digits then skip all checks
if (x< 12)
    {
    cksumTotal = 9999;  // force fail
    x=0;
    }
    
    
// From L->R, take each digit and either
//  add (evens) or
//  multiply (odd)
while( x-- )
    {
    powX = pow(10, x);          // Largest power of 10 < num, e.g. 1,000,000

    // Get next digit
    digit = testNum / powX;     // get digit value

    // Get first 2digits to determine card type
    if (first2 == 0)
        cardType += digit*10;
    else if (first2 == 1)
        cardType += digit;
    first2++;

    // add Checksums
    // follow Luhrs, adding evens and mult/flatten odds
    //if(x>0) //skip final digit
        {
        if(IsEven(x))
            {
            cksumTotal += digit;
            }
        else
            {
            //printf("oddnum: digit is %d - ck_in=%d...", digit, cksumTotal);
            n = digit * 2;
            if (n > 9)
                n = 1 + (n - 10 );
            cksumTotal += n;
            //printf("ch_out=%d\n", cksumTotal);
            }
        }

    testNum = testNum - (digit * powX);  // decrement num by subtracting digit * powX
    }

//printf("chksum=%d, cardType=%d\n", cksumTotal, cardType);
if (cksumTotal % 10 != 0)
    cardType = CARD_INVALID;        // failed cksum
else
    {
        if (cardType == 34 || cardType == 37)
            cardType = CARD_AMEX;
        else if (cardType >=40 && cardType <=49)
            cardType = CARD_VISA;
        else if (cardType >=50 && cardType <=55)
            cardType = CARD_MC;
        else
            cardType = CARD_INVALID;
    }

printf("%s\n", cardTypes[cardType]);
return 0;   //per spec
}
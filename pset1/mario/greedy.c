#include <stdio.h>
#include <cs50.h>

/* CS50, pset1, greedy */

int main(void)
{
    printf("O hai! ");
    
    float fChg = -1.0;
    int totalCoins = 0; 
    float coins[4] = {25, 10, 5, 1 }; 
    
    // Get the total. force positive vals
    do 
    {
        printf("How much change is owed?\n");
        fChg = get_float();
    }
    while (fChg < 0);
   
    int iChg = (fChg + .005) * 100;
    for (int x = 0; x < (sizeof(coins) / sizeof(int)); x++)
    {
        int largestCoin = coins[x];
        //printf("curr coin val=%i\n", largestCoin);
        while (iChg >= largestCoin)
        {
            iChg -= largestCoin;
            //printf("Change left=%i/n", iChg);
            totalCoins++;
            //printf("coins=%d\n", totalCoins);
        }
    }
    
    
    // final coins total
    printf("%i\n", totalCoins);
}
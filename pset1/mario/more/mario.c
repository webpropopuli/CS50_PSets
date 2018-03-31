#include <stdio.h>
#include <cs50.h>

/* CS50, pset1, mario/less */

void printSpaces(int n)
    {
        for(int x = 0; x < n; x++)
            printf(" ");
    }
    
void printBlocks(int n)
    {
        for(int x = 0; x < n; x++)
            printf("#");
    }
    
#define printGap() printSpaces(2)
int main(void)
{
    int iHeight = 5;  // Force true first pass


    do
    {
        printf("Height: ");
        iHeight = get_int();
    }
    while (iHeight < 0 || iHeight > 23);

    for (int row = 1; row <= iHeight; row++)
    {
        int cntBlocks = row;
        int cntSpaces = iHeight - cntBlocks;

        printSpaces(cntSpaces);
        printBlocks(cntBlocks);
        printGap();
        printBlocks(cntBlocks);
        // printSpaces(cntSpaces);

        printf("\n"); // EOL
    }
}


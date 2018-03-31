#include <stdio.h>
#include <cs50.h>

/* CS50, pset1, mario/less */
int main(void)
{
    int iHeight =5;  // Force true first pass


    do
    {
        printf("Height: ");
        iHeight = get_int();
    }
    while (iHeight < 0 || iHeight > 23);

    for (int row = 0; row < iHeight; row++)
    {
        int cntBlocks, cntSpaces;

        cntBlocks = row + 2;  // top line always has 2 blocks
        cntSpaces = iHeight - cntBlocks + 1;
        for (int x = 0; x < cntSpaces; x++)
        {
            printf(" ");
        }
        for (int x = 0; x < cntBlocks; x++)
        {
            printf("#");
        }

        printf("\n");
    }
}
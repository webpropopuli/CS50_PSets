//looks awesome, few white spaces there.
#include <stdio.h>
#include <cs50.h>

/* CS50, pset1, mario/less */
int main(void)
{
    //I think do while will run the loop first time anyway and does not need to be forced.
    int iHeight = 5;  // Force true first pass

    do
    {
        printf("Height: ");
        iHeight = get_int();
    }
    while (iHeight < 0 || iHeight > 23);

    for (int row = 0; row < iHeight; row++)
    {
        //like how the variables are named and logic is clear
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
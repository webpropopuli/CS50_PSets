#include <stdio.h>
#include <cs50.h>

/* CS50, pset1, 'water' */
int main(void)
{
    int x;

    do
    {
        printf("Minutes: ");
        x = get_int();
    }
    while (x < 0); /* avoid negs. Maybe get_int already does this? */

    printf("Bottles: %i\n", x * 12);

}
/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9
#define BRD_MAX (dim*dim)

typedef struct _square {
  int value;
  struct _square* _UP;
  struct _square* _DN;
  struct _square* _LF;
  struct _square* _RT;

} square;

// board
//int board[DIM_MAX][DIM_MAX];     // this isn't used now but we need some storage
//square board[DIM_MAX*DIM_MAX];    // space for board maximum size
square *TheBoard;   // will use this when dealing with board in 1-d space
square *pBlank;     // ptr to "empty" square

// dimensions
int dim;      // width and height or board

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

// Swap two tiles.
void swap(square *x, square *y)
{
    int tmp = x->value;
    x->value = y->value;
    y->value = tmp;
}

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    dim = atoi(argv[1]);
    if (dim < DIM_MIN || dim > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // allocate memory
    TheBoard = malloc(dim * dim * sizeof(square));
    if (NULL == TheBoard)
        {  // crash...burn
        puts ("malloc failure, exiting\n");
        return -1;
        }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen then draw the current state of the board
        clear();
        draw();

#ifdef THEIR_WAY
        for (int i = 0; i < dim; i++)
        {
            for (int j = 0; j < dim; j++)
            {
                fprintf(file, "%i", board[i][j]); // print tile
                if (j < dim - 1)
                {
                    fprintf(file, "|");  // print divider
                }
            }
            fprintf(file, "\n");
        }
#else
        // log the current state of the board (for testing)
        square *pS = TheBoard;
        for (int i = 0; i < BRD_MAX; i++, pS++)
        {
            fprintf(file, "%i", pS->value ); // print tile
            if ((i + 1) % dim)
                fprintf(file, "|");  // print divider

            if ( i > 0 && ((i + 1) % dim) == 0) // add newline
                fprintf(file, "\n");
        }
#endif
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }
        else
            printf("\nNot Won; make another move \n");

        // prompt for move
        printf("\nTile to move: ");
        int tile = get_int();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(300000);
        }

        // sleep thread for animation's sake
        usleep(200000);
    }  // end main loop

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
    printf("\033[1;33;40m");           // yellow text, black bg
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(500000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void)
{
    int x;  //loop counter
    square * pS = TheBoard;
    // Fill the board with numbers in reverse order
    for(x = 0; x < BRD_MAX - 1; x++, pS++)
    {
        // set value
        pS->value = BRD_MAX - x -1; // squares are from 1 to Max - 1
        //printf("%d: %i [%p]\n", x, pS->value, (void *)(pS));

        // We've defaulted all to NULL so only set
        // the non-edge conditions

        // set _UP
        if(x>=dim)
            pS->_UP = pS - dim;

        // set _DN
        if (x >= BRD_MAX - dim)
            pS->_DN = pS + dim;

        // set _LF
        if(x % dim != 0)
            pS->_LF = pS - 1;

        //set _RT
        if(x % dim != dim)
            pS->_RT = pS + 1;
    }
    pS->value = '\0';      // empty tile
    pBlank = pS;    // save blank pointer

    // Special: on Boards where Dim is even, need to swap last 2 tiles
    if(0 == (BRD_MAX & 1))
        swap(pS-1, pS-2);  // x was the blank. Now use the 2 preceding
return;
}

/**
 * Prints the current board to stdout
 */
void draw(void)
{
    square *p = TheBoard;               // beginning of board
    int x = 0;
    // TODO
    do
    {
        if(x && !(x % dim))       // Add new line end of row. '!=' 0 omits starting with a blank line
            printf("\n");

        printf("%2i ", p->value);
        x++, p++;
    }
    while(x < BRD_MAX );
}

/*
* GetPtrFromValue(int) Given a numeric val, return the square ptr
*/
square *GetPtrFromValue(n)
{
    static int x;
    square *pS = TheBoard;
    for(x = 0; x < BRD_MAX; x++, pS++)
    {
        if(n == pS->value)
            return pS;
    }
    return NULL;
}

/*
* isSwappable - is the selected square next to the blank
*/
bool isSwappable(square *p)
{
    //Using _UP, _DN. etc, is p next to pBlank?
    return (
        pBlank == p->_UP ||
        pBlank == p->_DN ||
        pBlank == p->_LF ||
        pBlank == p->_RT );
}
/**
 * If tile borders empty space [isAdjacent()], moves tile and returns true, else
 * returns false.
 */
bool move(int tilevalue)
{
    // Error: Out of bounds
    if (tilevalue <1 || tilevalue > BRD_MAX - 1)
    {
        printf("TMP: Value out of bounds (%i)\n", tilevalue);
        return false;
    }

    // find tile with matching value
    square *p = GetPtrFromValue(tilevalue);

    // Error: User selected the Empty square
    if (p == pBlank)
    {
        printf("TMP: Don't select the Empty square, (%d)\n", tilevalue);
        return false;
    }

    // is i adjacent to pBlank
    if (isSwappable(p))
    {
        swap(pBlank, p);
        return true;
    }
    else
    {    // Error: not swappable = not a neighbor of Empty square
        printf("TMP: Not adjacent to Empty square, (%d),n", tilevalue);
        return false;
    }
}

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
bool won(void)
{
    /* Traversing the board from position 0, return false if the value at current position
     *  is not 1 less than the next value (not in order), else board is sorted.
     */
    square *p = TheBoard;
    for(int x=0; x < BRD_MAX - 1; x++, p++)
        if(p->value != (p + 1)->value -1)
            return false;

    return true;  // We win! (by default)
}

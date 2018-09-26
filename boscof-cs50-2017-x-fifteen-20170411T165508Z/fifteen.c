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

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
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
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
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
            usleep(100000);
        }

        // sleep thread for animation's sake
        usleep(100000);
    }
    
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
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(400000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    //fill the 2d array with tiles
    int c=1;
    for (int i=0; i<d; i++)
    {
        for (int j=0; j<d; j++)
        {
            board[i][j]=(d*d - c);
            c++;
        }
    }
    //swap tile 2 with tile 1 if the dimensions of the board are even numbers
    if ((d % 2)==0)
    {
        for (int i=0;i<d;i++)
        {
            for (int j=0;j<d-1;j++)
            {
                if (board[i][j]==2 && board[i][j+1]==1)
                {
                    board[i][j]=1;
                    board[i][j+1]=2;
                }
            }
        }
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // prints the board according to the array
    for (int i=0; i<d; i++)
    {
        for (int j=0; j<d; j++)
        {
            //prints ' _' if the numbered tile is '0'
            if (board[i][j]==0)
            {
                printf("  _ ");
            }
            //otherwise prints the number stored within the array
            else
            {
                printf(" %2i ", board[i][j]);
            }
        }
        //leave two spaces between each line for aesthetic purposes
        printf("\n\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // declare two variables that track the position of the '0'
    int h;
    int w;
    for (int i=0; i<d; i++)
    {
        for (int j=0;j<d;j++)
        {
            if (board[i][j]==0)
            {
                h = i;
                w = j;
            }
        }
    }
    
    //switches the position of the '0' tile with the specified tile if the move is valid
    if (board[h+1][w]==tile)
    {
        board[h][w]=tile;
        board[h+1][w]=0;
        return true;
    }
    else if (board[h-1][w]==tile)
    {
        board[h][w]=tile;
        board[h-1][w]=0;
        return true;
    }
    else if (board[h][w+1]==tile)
    {
        board[h][w]=tile;
        board[h][w+1]=0;
        return true;
    }
    else if (board[h][w-1]==tile)
    {
       board[h][w]=tile;
       board[h][w-1]=0;
       return true;
    }
    
    //returns false if move is invalid
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    //checks that the numbered tiles are arranged from smallest to largest
    int order=1;
    for (int i=0; i<d; i++)
    {
        for (int j=0; j<d; j++)
        {
            //returns false if any tile is out of order
            if (order!=d*d &&
            board[i][j]!=order)
            {
                return false;
            }
            //otherwise increase count by 1 and check that the next tile is in order
            else 
            {
                order++;
            }
        }
    }
    //returns true if tiles are arranged from smallest to largest
    return true;
}

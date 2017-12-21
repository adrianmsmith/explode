/* This code was originally written in 1994 when I was at school. */

#include <string.h>
#include <stdio.h>
#include "adrian.h"

#define contemplate_move_macro(x, y) if (order_grid[x][y] = TRUE, contemplate_move(levelnumber, gridptr, recursionDepth, sign, minimum, x, y, &maximum, &max_x, &max_y)) goto end

#define gridArraySize 10

#define PLAYER -1

static int grids[gridArraySize][6][6];

static int level(int, grid_array *, int, int, int, int *, int *);

BOOL won_or_empty(grid_array *gridptr, int *sign)
{
        int x, y;
        BOOL only_neg, only_pos;

        only_neg = TRUE;
        only_pos = TRUE;
        for (y=0; y<6; y++)
                for (x=0; x<6; x++)
                        if ((*gridptr)[x][y] < 0) only_pos = FALSE;
                        else if ((*gridptr)[x][y] > 0) only_neg = FALSE;

        if (only_neg) *sign = -1;
        else if (only_pos) *sign = 1;
        else return (FALSE);

        return (TRUE);
}

static int new_level(int levelnumber, grid_array *gridptr, int recursionDepth, int sign, int minimum, int xn, int yn)
{
        int new_level = levelnumber + 1;
        grid_array *new_gridptr;

        new_gridptr = gridptr + 1;

        memcpy(new_gridptr, gridptr, sizeof(int [6][6]));
        (*new_gridptr)[xn][yn] += sign;
        explode(new_gridptr);
        return (level(new_level, new_gridptr, recursionDepth, -sign, minimum, NULL, NULL));
}

static BOOL contemplate_move(int levelnumber, grid_array *gridptr, int recursionDepth, int sign, int minimum, int x, int y, int *maximum, int *max_x, int *max_y)        /* TRUE = skip to end */
{
        int new_value;

        if (((*gridptr)[x][y] * sign) >= 0)                                /* ie. valid move */
        {
                new_value = new_level(levelnumber, gridptr, recursionDepth, sign, *maximum, x, y);
                if (sign == 1)
                {
                        if (new_value > *maximum)        /* we are maximising: we will return this or higher.  However, this or higher will never be chosen by above */
                        {
                                *maximum = new_value; *max_x = x; *max_y = y;
                                if (new_value > minimum) return (TRUE);
                        }
                } else {
                        if (new_value < *maximum)
                        {
                                *maximum = new_value; *max_x = x; *max_y = y;
                                if (new_value < minimum) return (TRUE);
                        }
                }
        }

        return (FALSE);
}

static int level(int levelnumber, grid_array *gridptr, int recursionDepth, int sign, int minimum, int *result_x, int *result_y)
{
        int x, y, *loop;
        int win_sign;
        int maximum;
        int max_x = 5, max_y = 0;
        int order_grid[6][6];    /* Have we already tried this square? */

        if (won_or_empty(gridptr, &win_sign)) return (win_sign * 10000);

        for (loop = order_grid[0], x = 36; x; x--) *loop++ = FALSE;   /* Weird loop for efficiency */

        if (levelnumber == recursionDepth)
        {
                /* Static board analysis: Just sum our values vs theirs */
                maximum = 0;
                for (x = 0; x < 6; x++)
                        for (y = 0; y < 6; y++)
                                maximum += (*gridptr)[x][y];
        } else {
                /* For alpha-beta pruning, try most likely squares first, to improve performance */
                maximum = -20000 * sign;

                /* Try corners first */
                contemplate_move_macro(0, 0); contemplate_move_macro(0, 5);
                contemplate_move_macro(5, 0); contemplate_move_macro(5, 5);

                /* Then sides */
                for (x = 1; x < 5; x++)
                {
                        contemplate_move_macro(x, 0); contemplate_move_macro(x, 5);
                        contemplate_move_macro(0, x); contemplate_move_macro(5, x);
                }
                
                /* Then those squares that already have pieces */
                for (x = 1; x < 5; x++)
                        for (y = 1; y < 5; y++)
                                if ((*gridptr)[x-1][y] || (*gridptr)[x+1][y] || (*gridptr)[x][y-1] || (*gridptr)[x][y+1] || (*gridptr)[x][y])
                                        contemplate_move_macro(x, y);

                /* Then the rest */
                for (x = 0; x < 6; x++)
                        for (y = 0; y < 6; y++)
                                if (order_grid[x][y] == FALSE)
                                        if (contemplate_move(levelnumber, gridptr, recursionDepth, sign, minimum, x, y, &maximum, &max_x, &max_y)) goto end;

                if (maximum == -20000 * sign)                                                   /* oh dear */
                        ERROR("There is no legal computer move (program may produce erroneous results henceforth)");
        }

end:    if (result_x) { *result_x = max_x; *result_y = max_y; }
        return (maximum);
}

static void first_move(int *result_x, int *result_y)
{
        int x, y;
        BOOL left;

        for (x = 0; x < 6; x++)
                for (y = 0; y < 6; y++)
                        if (grids[0][x][y] != 0)
                                left = x < 3;

        *result_x = left ? 5 : 0;
        *result_y = 0;
}

static void think(int recursionDepth, int *result_x, int *result_y)
{
        int x, y;

        for (x = 0; x < 6; x++)
                for (y = 0; y < 6; y++)
                        grids[0][x][y] = grid[x][y];

        if (won_or_empty((grid_array *) &grids[0][0][0], &x))
                first_move(result_x, result_y);
        else
                level(0, (grid_array *) &grids[0][0][0], recursionDepth, PLAYER, PLAYER * 30000, result_x, result_y);
}

void print_grid() {
    int x, y;
    for (y = 0; y < 6; y++) {
        for (x = 0; x < 6; x++)
            printf(" %2d", grid[y][x]);
        printf("\n");
    }
}

void main() {
    int x, y, won, recursionDepth;

    for (x = 0; x < 6; x++)
        for (y = 0; y < 6; y++)
            grid[x][y] = 0;

    printf("Difficulty (1..%d)? ", gridArraySize-1);
    scanf("%d", &recursionDepth);

    int first = 1;
    while (1) {
        printf("Your move x (0..5)? ");
        scanf("%d", &x);
        printf("Your move y (0..5)? ");
        scanf("%d", &y);

        grid[y][x]++;
        explode(grid);
        print_grid();
        if (!first) {
            if (won_or_empty(&grid, &won)) {
                printf("Sign %d won.", won);
                return;
            }
        }

        think(recursionDepth, &x, &y);
        printf("Computer chooses (%d,%d)\n", x, y);
        grid[y][x]--;
        explode(grid);
        print_grid();
        if (!first) {
            if (won_or_empty(&grid, &won)) {
                printf("Sign %d won.", won);
                return;
            }
        }

        first = 0;
    }
}
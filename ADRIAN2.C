/* Adrian - processing */

#include "adrian.h"

static grid_array *gridptr;

static void do1(grid_array *gridptr, int sign, int x, int y)
{
        if (x != -1 && y != -1)
        {
                if ((*gridptr)[x][y] < 0) (*gridptr)[x][y] = -(*gridptr)[x][y];
                (*gridptr)[x][y] += 1;
                (*gridptr)[x][y] *= sign;
        }
}

static void over(grid_array *gridptr, BOOL *finished, int max, int x, int y, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3)
{
        int sign = 0;

        if ((*gridptr)[x][y] > max) sign = 1;
        if ((*gridptr)[x][y] < -max) sign = -1;

        if (sign != 0)
        {
                do1(gridptr, sign, x0, y0);
                do1(gridptr, sign, x1, y1);
                do1(gridptr, sign, x2, y2);
                do1(gridptr, sign, x3, y3);
                (*gridptr)[x][y] -= (max+1) * sign;
                *finished = FALSE;
        }
}

void adrian_processgrid(grid_array *grid)
{
        int x, y;
        BOOL finished;

        gridptr = grid;

        do
        {
                finished = TRUE;
                over(grid, &finished, 1, 0,0, 1,0, 0,1, -1,-1, -1,-1);
                for (y=1; y<5; y++) over(grid, &finished, 2, 0,y, 0,y-1, 0,y+1, 1,y, -1,-1);
                over(grid, &finished, 1, 0,5, 1,5, 0,4, -1,-1, -1,-1);
                for (x=1; x<5; x++)
                {
                        over(grid, &finished, 2, x,0, x-1,0, x+1,0, x,1, -1,-1);
                        for (y=1; y<5; y++) over(grid, &finished, 3, x,y, x,y-1, x,y+1, x+1,y, x-1,y);
                        over(grid, &finished, 2, x,5, x-1,5, x+1,5, x,4, -1,-1);
                }
                over(grid, &finished, 1, 5,0, 4,0, 5,1, -1,-1, -1,-1);
                for (y=1; y<5; y++) over(grid, &finished, 2, 5,y, 5,y-1, 5,y+1, 4,y, -1,-1);
                over(grid, &finished, 1, 5,5, 4,5, 5,4, -1,-1, -1,-1);

                if (won(grid, &x))
                {
                        DEBUG_PRINT("***WIN IN PROCESSGRID***\n");
                        break;
                }
        } while (!finished);
}

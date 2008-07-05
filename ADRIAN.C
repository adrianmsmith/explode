/* Adrian - this is my bit */

#include <string.h>
#include "adrian.h"

#define contemplate_move_macro(x, y) if (order_grid[x][y] = TRUE, contemplate_move(levelnumber, gridptr, sign, minimum, x, y, &maximum, &max_x, &max_y, result_x != NULL)) goto end

#define max 3

#define SQUARE_VALUES 0

#define PLAYER -1

static int grids[max][6][6];

static int level(int, grid_array *, int, int, int *, int *);

BOOL won(grid_array *gridptr, int *sign)
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

static int new_level(int levelnumber, grid_array *gridptr, int sign, int minimum, int xn, int yn)
{
        int new_level = levelnumber + 1;
        grid_array *new_gridptr;

        new_gridptr = gridptr + 1;

        memcpy(new_gridptr, gridptr, sizeof(int [6][6]));
        (*new_gridptr)[xn][yn] += sign;
        adrian_processgrid(new_gridptr);
        return (level(new_level, new_gridptr, -sign, minimum, NULL, NULL));
}

static BOOL contemplate_move(int levelnumber, grid_array *gridptr, int sign, int minimum, int x, int y, int *maximum, int *max_x, int *max_y, BOOL print)        /* TRUE = skip to end */
{
        int new_value;

        if (((*gridptr)[x][y] * sign) >= 0)                                /* ie. valid move */
        {
                new_value = new_level(levelnumber, gridptr, sign, *maximum, x, y);
                if (print)
                        DEBUG_PRINT3("Testing %d %d: %6d ", x, y, new_value);
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

static int level(int levelnumber, grid_array *gridptr, int sign, int minimum, int *result_x, int *result_y)
{
        int x, y, *loop;
        int win_sign;
        int maximum;
        int max_x = 5, max_y = 0;
        int order_grid[6][6];

        if (won(gridptr, &win_sign)) return (win_sign * 10000);

        for (loop = order_grid[0], x = 36; x; x--) *loop++ = FALSE;

        if (levelnumber == max-1)
        {
                maximum = 0;
                for (x = 0; x < 6; x++)
                        for (y = 0; y < 6; y++)
                                if (SQUARE_VALUES)
                                {
                                        win_sign = (*gridptr)[x][y];
                                        if (win_sign > 0) maximum += win_sign * win_sign;
                                        else if (win_sign < 0) maximum -= win_sign * win_sign;
                                } else
                                        maximum += (*gridptr)[x][y];
        } else {
                maximum = -20000 * sign;

                contemplate_move_macro(0, 0); contemplate_move_macro(0, 5);
                contemplate_move_macro(5, 0); contemplate_move_macro(5, 5);

                for (x = 1; x < 5; x++)
                {
                        contemplate_move_macro(x, 0); contemplate_move_macro(x, 5);
                        contemplate_move_macro(0, x); contemplate_move_macro(5, x);
                }
                
                for (x = 1; x < 5; x++)
                        for (y = 1; y < 5; y++)
                                if ((*gridptr)[x-1][y] || (*gridptr)[x+1][y] || (*gridptr)[x][y-1] || (*gridptr)[x][y+1] || (*gridptr)[x][y])
                                        contemplate_move_macro(x, y);

                for (x = 0; x < 6; x++)
                        for (y = 0; y < 6; y++)
                                if (order_grid[x][y] == FALSE)
                                        if (contemplate_move(levelnumber, gridptr, sign, minimum, x, y, &maximum, &max_x, &max_y, result_x != NULL)) goto end;

                if (maximum == -20000 * sign)                                                   /* oh dear */
                        ERROR("There is no legal computer move (program may produce erroneous results henceforth)");
        }

end:    if (result_x) { *result_x = max_x; *result_y = max_y; }
        return (maximum);
}

static void first_move(grid_array *gridptr, int *result_x, int *result_y)
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

static void think(int *result_x, int *result_y)
{
        int x, y;

        for (x = 0; x < 6; x++)
                for (y = 0; y < 6; y++)
                        grids[0][x][y] = grid[x][y];

        if (won((grid_array *) &grids[0][0][0], &x))
                first_move((grid_array *) &grids[0][0][0], result_x, result_y);
        else
                level(0, (grid_array *) &grids[0][0][0], PLAYER, PLAYER * 30000, result_x, result_y);
}

void compgo(HWND hWnd)
{
        HDC hDC;
        int x, y;
        RECT temp;              /* original salih name */
        LPSTR string;

        hDC = GetDC(hWnd);

        think(&x, &y);

        grid[x][y]--;
        drawbomb(hDC, hWnd, x, y);
        turn = (PLAYER == -1) ? RED : BLUE;
        DEBUG_PRINT("\n\nRedrawing...");
        if (processgrid(hDC, hWnd)) turn = GAMEOVER;

        temp.left = 6; temp.right = 140;
        temp.top = 50; temp.bottom = 80;
        FillRect(hDC, &temp, LtGreyBrush);
        SetBkColor(hDC, RGB(192, 192, 192));
        SetTextColor(hDC, RGB(255, 0, 0));
        string = rturn;
        if (turn == GAMEOVER) {
                SetTextColor(hDC, RGB(0, 0, 0));
                string = gmover;
                }
        SelectObject(hDC, playerfont);
        TextOut(hDC, 10, 58, string, _fstrlen(string));
        ReleaseDC(hWnd, hDC);
}

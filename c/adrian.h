/* This code was originally written in 1994 when I was at school. */

//#include "windows.h"

#define BOOL int
#define TRUE 1
#define FALSE 0

/* types */

typedef int grid_array[6][6];

/* prototypes */

//void compgo(HWND);
void adrian_processgrid(grid_array *);
BOOL won(grid_array *, int *);

/* machine-dependant */

#ifndef ERROR
	#define ERROR(text)
#endif
#define DEBUG_PRINT(text)
#define DEBUG_PRINT3(text, a, b, c)

/* won't work */

#define RED 1 
#define BLUE 2
#define GAMEOVER 3
#define LtGreyBrush 0
int grid[6][6];
int turn;
typedef void *HWND, *HDC, *LPSTR;
typedef struct { int left, right, top, bottom; } RECT;
extern HDC GetDC(HWND);
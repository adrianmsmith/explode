/* AdrExplode.h */

//#include "windows.h"

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


/* QuickCase:W KNB Version 1.00 */
#include <windows.h>
#include <string.h>

#define IDS_ERR_REGISTER_CLASS   1
#define IDS_ERR_CREATE_WINDOW    2

#define IDM_ALWAYSONTOP	9000

#define RED 1
#define BLUE 2
#define GAMEOVER 3

#define IDHELP	100
#define IDNEW	101
#define IDEXIT	102
#define ID1PLAY	103
#define ID2PLAY	104
#define IDLOAD	105
#define IDSAVE	106

char szString[128];   /* variable to load resource strings         */

char szAppName[20];   /* class name for the window               */
HWND hInst;
HWND hWndMain;


LONG FAR PASCAL WndProc(HWND, WORD, WORD, LONG);
int nCwRegisterClasses(void);
void CwUnRegisterClasses(void);
BOOL calldll(char *grid, int *x, int *y, HANDLE hLib, FARPROC lpThink);
VOID FAR PASCAL Think (char *grid, int *x, int *y);

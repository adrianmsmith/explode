]/* QuickCase:W KNB Version 1.00 */
#include "EXPLODE.h"
#include <math.h>
#include "adrian.h"
#include <string.h>

LPSTR cstring = "Bnoxqhfgs\0370883\037Q\037K\037R`khg";
LPSTR winxpos = "Window X Position";
LPSTR winypos = "Window Y Position";
LPSTR explode = "Explode";
LPSTR button = "Button";
LPSTR plystr = "Players";
LPSTR fname = "game.exp";
LPSTR rturn = "Red's turn";
LPSTR bturn = "Blue's turn";
LPSTR gmover = "Game Over";
LPSTR p1pl = "&1 Player";
LPSTR p2pl = "&2 Players";
LPSTR hlp = "&Help";
LPSTR exits = "E&xit";
LPSTR new = "&New Game";

int turn = RED;
int players;
char grid[6][6];

HPEN GreyPen, LtGreyPen, WhitePen, BlackPen, BluePen, RedPen;
HBRUSH GreyBrush, LtGreyBrush, WhiteBrush, RedBrush, BlueBrush;
HANDLE explodefont, playerfont, bombfont;

void compgo(HWND hWnd);
void drawblank(HDC hDC, HWND hWnd, int X, int Y);
void drawbomb(HDC hDC, HWND hWnd, int x, int y);
BOOL processgrid(HDC hDC, HWND hWnd);

int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{

 MSG        msg;           /* MSG structure to store your messages        */
 int        nRc;           /* return value from Register Classes          */
 long       nWndunits;     /* window units for size and location          */
 int        nWndx;         /* the x axis multiplier                       */
 int        nWndy;         /* the y axis multiplier                       */
 int        nX;            /* the resulting starting point (x, y)         */
 int        nY;
 int        nWidth;        /* the resulting width and height for this     */
 int        nHeight;       /* window                                      */

 int i,j;
 HANDLE hAccel;

// char ts[4];

 for (i = 0; i < (int) _fstrlen(cstring); i++)
 	{
 	cstring[i]++;
 	}

 for (i = 0; i < 6; i++)
 	{
 	for (j = 0; j < 6; j++)
 		{
 		grid[i][j] = 0;
 		}
 	}

 players = GetProfileInt(explode, plystr, 1);
 if (players != 2) players = 1;

 strcpy(szAppName, "EXPLODE");
 hInst = hInstance;
 if(!hPrevInstance)
   {
    /* register window classes if first instance of application         */
    if ((nRc = nCwRegisterClasses()) == -1)
      {
       /* registering one of the windows failed                         */
       LoadString(hInst, IDS_ERR_REGISTER_CLASS, szString, sizeof(szString));
       MessageBox(NULL, szString, NULL, MB_ICONEXCLAMATION);
       return nRc;
      }
   }


/* GetProfileString("intl", "sLanguage","eng", ts, 4);
 if (!_fstrcmp(ts, "esn"))
 	{
 	explode = "Explotar";
 	plystr = "Jugadors";
 	fname = "juego.exp";
 	gmover = "Juego Terminado";
 	rturn = "Rojo turno";
 	bturn = "Azul turno";
 	winxpos = "Ventana X Posición";
 	winypos = "Ventana Y Posición";
	p1pl = "&1 Jugador";
	p2pl = "&2 Jugadors";
	exits = "&Dejar";
	new = "&Neuvo Juego";
	hlp = "&Ayuda";
	}

if (!_fstrcmp(ts, "ita"))
 	{
 	explode = "Esplodere";
 	plystr = "Attores";
 	fname = "giuoco.esp";
 	gmover = "Giuoco Attraverso";
 	rturn = "Rosso direzione";
 	bturn = "Azzuro diresione";
 	winxpos = "Finestra X Posizione";
 	winypos = "Finestra Y Posizione";
	p1pl = "&1 Attore";
	p2pl = "&2 Attore";
	exits = "&Uscita";
	new = "&Nuovo Giuoco";
	hlp = "&Aiuto";
	}
*/
 /* Create a device independant size and location                       */
 nWndunits = GetDialogBaseUnits();
 nWndx = LOWORD(nWndunits);
 nWndy = HIWORD(nWndunits);
 nX = ((GetProfileInt(explode, winxpos, 64) * nWndx) / 8);
 nY = ((GetProfileInt(explode, winypos, 64) * nWndy) / 16);
 nWidth = ((405 * nWndx) / 8);
 nHeight = ((276 * nWndy) / 16);


 /* create application's Main window                                    */
 hWndMain = CreateWindow(
                szAppName,               /* Window class name           */
                explode,               /* Window's title              */
                WS_CAPTION      |        /* Title and Min/Max           */
                WS_SYSMENU      |        /* Add system menu box         */
                WS_MINIMIZEBOX  |        /* Add minimize box            */
                WS_BORDER       |        /* thin frame                  */
                WS_CLIPCHILDREN |         /* don't draw in child windows areas */
                WS_OVERLAPPED,
                nX, nY,                  /* X, Y                        */
                nWidth, nHeight,         /* Width, Height of window     */
                NULL,                    /* Parent window's handle      */
                NULL,                    /* Default to Class Menu       */
                hInst,                   /* Instance of window          */
                NULL);                   /* Create struct for WM_CREATE */


 if(hWndMain == NULL)
   {
    LoadString(hInst, IDS_ERR_CREATE_WINDOW, szString, sizeof(szString));
    MessageBox(NULL, szString, NULL, MB_ICONEXCLAMATION);
    return IDS_ERR_CREATE_WINDOW;
   }

 ShowWindow(hWndMain, nCmdShow);            /* display main window      */

 hAccel = LoadAccelerators(hInst, szAppName);

 while(GetMessage(&msg, NULL, 0, 0))        /* Until WM_QUIT message    */
   {
    if(TranslateAccelerator(hWndMain, hAccel, &msg))
      continue;
    TranslateMessage(&msg);
    DispatchMessage(&msg);
   }

 /* Do clean up before exiting from the application                     */
 CwUnRegisterClasses();
 return msg.wParam;
} /*  End of WinMain                                                    */

LONG FAR PASCAL WndProc(HWND hWnd, WORD Message, WORD wParam, LONG lParam)
{
 HANDLE idTimer;
 HMENU      hMenu=0;            /* handle for the menu                 */
 HBITMAP    hBitmap=0;          /* handle for bitmaps                  */
 HDC        hDC;                /* handle for the display device       */
 PAINTSTRUCT ps;                /* holds PAINT information             */
 int        nRc=0;              /* return code                         */

 int i, j;
 LPSTR string;
 //HDC hMemDC;

 //DWORD winpos;

 RECT rect, temprect;
 static HWND buthelp, butexit, butnew, but1, but2;
 char textstring[5];

 if (turn == BLUE && players == 1) compgo(hWnd);

 switch (Message)
   {
    case WM_CREATE:

		 SetWindowText(hWnd, cstring); 
		 idTimer = SetTimer(hWnd, 1, 10000, (FARPROC) NULL);
		 GreyPen = CreatePen(PS_SOLID, 1, RGB(128,128,128));
		 LtGreyPen = CreatePen(PS_SOLID, 1, RGB(192,192,192));
		 BlackPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
		 WhitePen = CreatePen(PS_SOLID, 1, RGB(255,255,255));
		 BluePen = CreatePen(PS_SOLID, 1, RGB(0,0,255));
		 RedPen = CreatePen(PS_SOLID, 1, RGB(255,0,0));
		 GreyBrush = CreateSolidBrush(RGB(128, 128, 128));
         LtGreyBrush = CreateSolidBrush(RGB(192,192,192));
         WhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
         RedBrush = CreateSolidBrush(RGB(255,0,0));
         BlueBrush = CreateSolidBrush(RGB(0,0,255));
         explodefont = CreateFont(44, 14, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "Times New Roman");
         playerfont = CreateFont(23, 9, 0, 0, FW_NORMAL, TRUE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_MODERN, "Arial");
         bombfont = CreateFont(12, 7, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_MODERN, "Arial");
         butnew = CreateWindow(button, new, BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 10, 165, 120, 24, hWnd, IDNEW, hInst, NULL);
         buthelp = CreateWindow(button, hlp, BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 10, 192, 120, 24, hWnd, IDHELP, hInst, NULL);
		 butexit = CreateWindow(button, exits, BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, 10, 219, 120, 24, hWnd, IDEXIT, hInst, NULL);
		 but1 = CreateWindow(button, p1pl, BS_RADIOBUTTON | WS_CHILD | WS_VISIBLE, 24, 96, 100, 24, hWnd, ID1PLAY, hInst, NULL);
		 but2 = CreateWindow(button, p2pl, BS_RADIOBUTTON | WS_CHILD | WS_VISIBLE, 24, 120, 100, 24, hWnd, ID2PLAY, hInst, NULL);
         if (players == 1) SendMessage(but1, BM_SETCHECK, 1, 0L);
    	 else SendMessage(but2, BM_SETCHECK, 1, 0L);
    	 break;       /*  End of WM_CREATE                              */

    case WM_MOVE:     /*  code for moving the window                    */
         break;

    case WM_SIZE:     /*  code for sizing client area                   */
         break;       /* End of WM_SIZE                                 */

    case WM_TIMER :
    	 SetWindowText(hWnd, explode);
    	 KillTimer(hWnd, 1);
    	 break;

    case WM_PAINT:    /* code for the window's client area              */
         /* Obtain a handle to the device context                       */
         /* BeginPaint will sends WM_ERASEBKGND if appropriate          */
         memset(&ps, 0x00, sizeof(PAINTSTRUCT));
         hDC = BeginPaint(hWnd, &ps);

         /* Included in case the background is not a pure color         */
         SetBkMode(hDC, TRANSPARENT);

         if (IsIconic(hWnd)) {EndPaint(hWnd, &ps);break;}

         GetClientRect (hWnd, &rect);
		 FillRect(hDC, &rect, LtGreyBrush);
		 SelectObject(hDC, GreyPen);
		 InflateRect(&rect, -4, -4);
		 MoveTo(hDC, rect.left, rect.top);
		 LineTo(hDC, rect.right-1, rect.top);
		 SelectObject(hDC, WhitePen);
		 LineTo(hDC, rect.right-1, rect.bottom-1);
		 LineTo(hDC, rect.left, rect.bottom-1);
		 MoveTo(hDC, rect.left + 144, rect.bottom-2);
		 LineTo(hDC, rect.left + 144, rect.top);
		 SelectObject(hDC, GreyPen);
		 MoveTo(hDC, rect.left, rect.bottom-1);
		 LineTo(hDC, rect.left, rect.top);
		 MoveTo(hDC, rect.left + 147, rect.bottom-2);
		 LineTo(hDC, rect.left + 147, rect.top);
		 InflateRect(&rect, -2, -2);
		 SetBkColor(hDC, RGB(128,128,128));
		 SetTextColor(hDC, RGB(255,255,0));
		 SelectObject(hDC, explodefont);
		 TextOut(hDC, rect.left + 4, rect.top, explode, 7);
		 SetTextColor(hDC, RGB(255, 0, 0));
		 string = rturn;
		 if (turn == BLUE)
		 	{
		 	SetTextColor(hDC, RGB(0, 0, 255));
		 	string = bturn;
		 	}
		 if (turn == GAMEOVER)
		 	{
		 	SetTextColor(hDC, RGB(0, 0, 0));
		 	string = gmover;
		 	}
		 SelectObject(hDC, playerfont);
		 TextOut(hDC, rect.left + 4, rect.top + 52, string, _fstrlen(string));
         temprect.left = rect.left + 4;
         temprect.top = rect.top + 84;
		 temprect.right = 123 + rect.left;
		 temprect.bottom = 150 + rect.top;
         FillRect(hDC, &temprect, WhiteBrush);
         SelectObject(hDC, BlackPen);
         MoveTo(hDC, temprect.left + 1, temprect.bottom - 2);
         LineTo(hDC, temprect.left + 1, temprect.top + 1);
         LineTo(hDC, temprect.right - 1, temprect.top + 1);
         SelectObject(hDC, GreyPen);
         MoveTo(hDC, temprect.left, temprect.bottom-1);
         LineTo(hDC, temprect.left, temprect.top);
         LineTo(hDC, temprect.right, temprect.top);
         SelectObject(hDC, LtGreyPen);
         MoveTo(hDC, temprect.right -2 , temprect.top + 1);
         LineTo(hDC, temprect.right -2, temprect.bottom -2);
         LineTo(hDC, temprect.left + 1, temprect.bottom -2);
         for (i = 0; i < 6; i++)
         	{
         	for (j = 0; j < 6; j++)
         		{
         		drawblank(hDC, hWnd, i, j);
         		drawbomb(hDC, hWnd, i, j);
         		}
         	}

		 /* Inform Windows painting is complete                         */
         EndPaint(hWnd, &ps);
         break;       /*  End of WM_PAINT                               */

	case WM_QUERYENDSESSION:
    case WM_CLOSE:  /* close the window                                 */
         /* Destroy child windows, modeless dialogs, then, this window  */
         WinHelp(hWnd, "explode.hlp", HELP_QUIT, NULL);
    	 DeleteObject(GreyPen);
         DeleteObject(LtGreyPen);
         DeleteObject(WhitePen);
         DeleteObject(BlackPen);
         DeleteObject(RedPen);
         DeleteObject(BluePen);
         DeleteObject(GreyBrush);
         DeleteObject(LtGreyBrush);
         DeleteObject(RedBrush);
         DeleteObject(BlueBrush);
         DeleteObject(explodefont);
         DeleteObject(bombfont);
         DeleteObject(playerfont);
         DeleteObject(WhiteBrush);
         	{
	     	RECT r;
	        GetWindowRect(hWnd, &r);
	        wsprintf(textstring, "%d", players);
	        WriteProfileString(explode, plystr, textstring);
	        wsprintf(textstring, "%d", r.left);
	        WriteProfileString(explode, winxpos, textstring);
	        wsprintf(textstring, "%d", r.top);
	        WriteProfileString(explode, winypos, textstring);
	        }
	    DestroyWindow(hWnd);
	     if (hWnd == hWndMain)
         PostQuitMessage(0);  /* Quit the application                 */
        break;

    case WM_COMMAND :
    	switch(wParam)

    		{
    		case IDHELP :
    			WinHelp(hWnd, "explode.hlp", HELP_INDEX, NULL);
    			break;

    		case IDEXIT :
    			PostMessage(hWnd, WM_CLOSE, 0, 0);
    			break;

    		case IDNEW :
    			{
    			int k;
    			for (k = 0; k < 6; k++)
    				{
    				for (j = 0; j < 6; j++)
    					{
    					grid[j][k] = 0;
    					}
    				}
    			turn = RED;
    			InvalidateRect(hWnd, NULL, FALSE);
    			}
    			break;

    		case ID1PLAY :
    			players = 1;
    			SendMessage(but1, BM_SETCHECK, 1, 0L);
    			SendMessage(but2, BM_SETCHECK, 0, 0L);
    			break;

    		case ID2PLAY :
    			players = 2;
    			SendMessage(but1, BM_SETCHECK, 0, 0L);
    			SendMessage(but2, BM_SETCHECK, 1, 0L);
    			break;

    		/*case IDLOAD :
    			{
    			OFSTRUCT *fstr;
    			int fh;
    			fh = OpenFile(fname, fstr, OF_READ | OF_SHARE_DENY_NONE);
    			if (fh == -1) break;
    			_lread(fh, (LPSTR) grid, 36);
    			_lread(fh, (LPSTR) &turn, sizeof(int));
    			_lclose(fh);
    			InvalidateRect(hWnd, NULL, FALSE);
    			}
    			break;

    		case IDSAVE :
    			{
    			OFSTRUCT *fstr;
    			int fh;
    			fh = OpenFile(fname, fstr, OF_CREATE | OF_SHARE_DENY_NONE);
    			_lwrite(fh, (LPSTR) grid, 36);
    			_lwrite(fh, (LPSTR) &turn, sizeof(int));
    			_lclose(fh);
    			break;
    			}*/
    		}
    	break;

    case WM_LBUTTONDOWN :
    	{
    	RECT wind;
    	int xpos, ypos, xs, ys;
    	if (turn == GAMEOVER) break;
    	/*(turn == BLUE && players == 1) break;*/
    	if (players == 3) players = 1;
    	xpos = LOWORD(lParam);
    	ypos = HIWORD(lParam);
    	GetClientRect(hWnd, &wind);
    	xpos -= wind.right;
    	ypos -= wind.bottom;
    	xpos += 25;
    	ypos += 25;
    	xpos *= -1;
    	ypos *= -1;
    	if (xpos < 0 || ypos < 0) break;
    	if ((xpos % 35) > 31) break;
    	if ((ypos % 35) > 31) break;
    	xs = xpos / 35;
    	ys = ypos / 35;
		if (xs > 5 || ys > 5) break;
		if (grid[xs][ys] < 0 && turn == RED) break;
		if (grid[xs][ys] > 0 && turn == BLUE) break;
		if (turn == RED) grid[xs][ys]++; else grid[xs][ys]--;
    	if (turn == RED) turn = BLUE; else turn = RED;
    	hDC = GetDC(hWnd);
    	temprect.left = 6; temprect.right = 140;
    	temprect.top = 50; temprect.bottom = 80;
    	FillRect(hDC, &temprect, LtGreyBrush);
    	SetBkColor(hDC, RGB(192, 192, 192));
    	SetTextColor(hDC, RGB(255, 0, 0));
		string = rturn;
		if (turn == BLUE)
			{
			SetTextColor(hDC, RGB(0, 0, 255));
			string = bturn;
			}
		if (turn == GAMEOVER)
			{
			SetTextColor(hDC, RGB(0, 0, 0));
			string = gmover;
			}
		SelectObject(hDC, playerfont);
		TextOut(hDC, 10, 58, string, _fstrlen(string));
		drawbomb(hDC, hWnd, xs, ys);
		if (processgrid(hDC, hWnd))
			{
			turn = GAMEOVER;
			SetBkColor(hDC, RGB(192,192,192));
			SetTextColor(hDC, RGB(0,0,0));
			SelectObject(hDC, playerfont);
			FillRect(hDC, &temprect, LtGreyBrush);
			TextOut(hDC, 10, 58, gmover, 9);
			}
		ReleaseDC(hWnd, hDC);
		}
    	break;

    default:
         /* For any message for which you don't specifically provide a  */
         /* service routine, you should return the message to Windows   */
         /* for default message processing.                             */
         return DefWindowProc(hWnd, Message, wParam, lParam);
   }
 return 0L;
}     /* End of WndProc                                         */

/************************************************************************/
/*                                                                      */
/* nCwRegisterClasses Function                                          */
/*                                                                      */
/* The following function registers all the classes of all the windows  */
/* associated with this application. The function returns an error code */
/* if unsuccessful, otherwise it returns 0.                             */
/*                                                                      */
/************************************************************************/

int nCwRegisterClasses(void)
{
 WNDCLASS   wndclass;    /* struct to define a window class             */
 memset(&wndclass, 0x00, sizeof(WNDCLASS));


 /* load WNDCLASS with window's characteristics                         */
 wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNWINDOW;
 wndclass.lpfnWndProc = WndProc;
 /* Extra storage for Class and Window objects                          */
 wndclass.cbClsExtra = 0;
 wndclass.cbWndExtra = 0;
 wndclass.hInstance = hInst;
 wndclass.hIcon = LoadIcon(hInst, "EXPLODE");
 wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
 /* Create brush for erasing background                                 */
 wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
 wndclass.lpszMenuName = szAppName;   /* Menu Name is App Name */
 wndclass.lpszClassName = szAppName; /* Class Name is App Name */
 if(!RegisterClass(&wndclass))
   return -1;


 return(0);
} /* End of nCwRegisterClasses                                          */


void CwUnRegisterClasses(void)
{
 WNDCLASS   wndclass;    /* struct to define a window class             */
 memset(&wndclass, 0x00, sizeof(WNDCLASS));

 UnregisterClass(szAppName, hInst);
}    /* End of CwUnRegisterClasses                                      */


void drawblank(HDC hDC, HWND hWnd, int x, int y)
	{
	RECT square;
	GetClientRect(hWnd, &square);
	square.left = square.right - 57 - x * 35;
	square.top = square.bottom - 57 - y * 35;
	square.right = square.left + 33;
	square.bottom = square.top + 33;
	FillRect(hDC, &square, WhiteBrush);
	square.bottom--;
	square.right--;
	SelectObject(hDC, GreyPen);
	MoveTo(hDC, square.left, square.bottom);
	LineTo(hDC, square.left, square.top);
	LineTo(hDC, square.right, square.top);
	SelectObject(hDC, BlackPen);
	MoveTo(hDC, square.left + 1, square.bottom - 1);
	LineTo(hDC, square.left + 1, square.top + 1);
	LineTo(hDC, square.right - 1, square.top + 1);
	SelectObject(hDC, LtGreyBrush);
	MoveTo(hDC, square.left + 1, square.bottom - 1);
	LineTo(hDC, square.right - 1, square.bottom - 1);
	LineTo(hDC, square.right - 1, square.top + 1);
	}

void drawbomb(HDC hDC, HWND hWnd, int x, int y)
	{
	char string[8];
	int num;
	RECT square;
	GetClientRect(hWnd, &square);
	num = grid[x][y];
	if (!num) {drawblank(hDC, hWnd, x, y);return;}
	x = square.right - 57 - x * 35;
	y = square.bottom - 57 - y * 35;
	wsprintf(string, "%d", abs(num));
	if (num < 0)
		{
		SelectObject(hDC, BlueBrush);
		SelectObject(hDC, BluePen);
		SetBkColor(hDC, RGB(0, 0, 255));
		}
	else
		{
		SelectObject(hDC, RedBrush);
		SelectObject(hDC, RedPen);
		SetBkColor(hDC, RGB(255, 0, 0));
		}
	SetTextColor(hDC, RGB(255, 255, 255));
	MoveTo(hDC, x + 15, y + 17);
	LineTo(hDC, x + 25, y + 7);
	MoveTo(hDC, x + 14, y + 17);
	LineTo(hDC, x + 24, y + 7);
	MoveTo(hDC, x + 15, y + 18);
	LineTo(hDC, x + 25, y + 8);
	MoveTo(hDC, x + 13, y + 17);
	LineTo(hDC, x + 23, y + 7);
	MoveTo(hDC, x + 15, y + 19);
	LineTo(hDC, x + 25, y + 9);
	SelectObject(hDC, BlackPen);
	Arc(hDC, x + 22, y + 8, x + 29, y + 15, x + 29, y + 15, x + 22, y + 8);
	SelectObject(hDC, bombfont);
	SelectObject(hDC, RedPen);
	if (num < 0) SelectObject(hDC, BluePen);
	Ellipse(hDC, x + 4, y + 8, x + 24, y + 28);
	TextOut(hDC, x + 10, y + 12, string, 1);
 	}

BOOL processgrid(HDC hDC, HWND hWnd)
	{
//	RECT rect;
	register int i, j;
	WORD noexpl = 0;
	char sign;
	BOOL changed;
	do
		{
		changed = FALSE;
		if (noexpl++ > 80) return(TRUE);
		if (abs(grid[0][0]) > 1)
			{
			sign = 1;
			if (grid[0][0] < 0) sign = -1;
			changed = TRUE;
			grid[0][0] = 0;
			grid[1][0] = (char) ((abs(grid[1][0]) + 1) * sign);
			grid[0][1] = (char) ((abs(grid[0][1]) + 1) * sign);
			drawbomb(hDC, hWnd, 0, 0);
			drawbomb(hDC, hWnd, 1, 0);
			drawbomb(hDC, hWnd, 0, 1);
			continue;
			}
		if (abs(grid[5][5]) > 1)
			{
			sign = 1;
			if (grid[5][5] < 0) sign = -1;
			changed = TRUE;
			grid[5][5] = 0;
			grid[4][5] = (char) ((abs(grid[4][5]) + 1) * sign);
			grid[5][4] = (char) ((abs(grid[5][4]) + 1) * sign);
			drawbomb(hDC, hWnd, 5, 5);
			drawbomb(hDC, hWnd, 4, 5);
			drawbomb(hDC, hWnd, 5, 4);
			continue;
			}
		if (abs(grid[0][5]) > 1)
			{
			sign = 1;
			if (grid[0][5] < 0) sign = -1;
			changed = TRUE;
			grid[0][5] = 0;
			grid[1][5] = (char) ((abs(grid[1][5]) + 1) * sign);
			grid[0][4] = (char) ((abs(grid[0][4]) + 1) * sign);
			drawbomb(hDC, hWnd, 0, 5);
			drawbomb(hDC, hWnd, 1, 5);
			drawbomb(hDC, hWnd, 0, 4);
			continue;
			}
		if (abs(grid[5][0]) > 1)
			{
			sign = 1;
			if (grid[5][0] < 0) sign = -1;
			changed = TRUE;
			grid[5][0] = 0;
			grid[4][0] = (char) ((abs(grid[4][0]) + 1) * sign);
			grid[5][1] = (char) ((abs(grid[5][1]) + 1) * sign);
			drawbomb(hDC, hWnd, 5, 0);
			drawbomb(hDC, hWnd, 4, 0);
			drawbomb(hDC, hWnd, 5, 1);
			continue;
			}
		for (i = 1; i < 5; i++)
			{
			if (changed) continue;
			if (abs(grid[i][0]) > 2)
				{
				changed = TRUE;
				sign = (char) (grid[i][0] / abs(grid[i][0]));
				grid[i][0] = 0;
				drawbomb(hDC, hWnd, i, 0);
				grid[i][1] = (char) (sign * (abs(grid[i][1]) + 1));
				grid[i-1][0] = (char) (sign * (abs(grid[i-1][0]) + 1));
				grid[i+1][0] = (char) (sign * (abs(grid[i+1][0]) + 1));
				drawbomb(hDC, hWnd, i, 1);
				drawbomb(hDC, hWnd, i - 1, 0);
				drawbomb(hDC, hWnd, i + 1, 0);
				}
			}
		if (changed) continue;
		for (i = 1; i < 5; i++)
			{
			if (changed) continue;
			if (abs(grid[i][5]) > 2)
				{
				changed = TRUE;
				sign = (char) (grid[i][5] / abs(grid[i][5]));
				grid[i][5] = 0;
				drawbomb(hDC, hWnd, i, 5);
				grid[i][4] = (char) (sign * (abs(grid[i][4]) + 1));
				grid[i-1][5] = (char) (sign * (abs(grid[i-1][5]) + 1));
				grid[i+1][5] = (char) (sign * (abs(grid[i+1][5]) + 1));
				drawbomb(hDC, hWnd, i, 4);
				drawbomb(hDC, hWnd, i - 1, 5);
				drawbomb(hDC, hWnd, i + 1, 5);
				}
			}
		if (changed) continue;
		for (i = 1; i < 5; i++)
			{
			if (changed) continue;
			if (abs(grid[0][i]) > 2)
				{
				changed = TRUE;
				sign = (char) (grid[0][i] / abs(grid[0][i]));
				grid[0][i] = 0;
				drawbomb(hDC, hWnd, 0, i);
				grid[1][i] = (char) (sign * (abs(grid[1][i]) + 1));
				grid[0][i-1] = (char) (sign * (abs(grid[0][i-1]) + 1));
				grid[0][i+1] = (char) (sign * (abs(grid[0][i+1]) + 1));
				drawbomb(hDC, hWnd, 1, i);
				drawbomb(hDC, hWnd, 0, i - 1);
				drawbomb(hDC, hWnd, 0, i + 1);
				}
			}
		if (changed) continue;
		for (i = 1; i < 5; i++)
			{
			if (changed) continue;
			if (abs(grid[5][i]) > 2)
				{
				changed = TRUE;
				sign = (char) (grid[5][i] / abs(grid[5][i]));
				grid[5][i] = 0;
				drawbomb(hDC, hWnd, 5, i);
				grid[4][i] = (char) (sign * (abs(grid[4][i]) + 1));
				grid[5][i-1] = (char) (sign * (abs(grid[5][i-1]) + 1));
				grid[5][i+1] = (char) (sign * (abs(grid[5][i+1]) + 1));
				drawbomb(hDC, hWnd, 4, i);
				drawbomb(hDC, hWnd, 5, i - 1);
				drawbomb(hDC, hWnd, 5, i + 1);
				}
			}
		if (changed) continue;
		for (i = 1; i < 5; i ++)
			{
			if (changed) continue;
			for (j = 1; j < 5; j++)
				{
				if (changed) continue;
				if (abs(grid[i][j]) >= 4)
					{
					changed = TRUE;
					sign = (char) (grid[i][j] / abs(grid[i][j]));
					grid[i][j] = 0;
					drawbomb(hDC, hWnd, i, j);
					grid[i-1][j] = (char) (sign * (abs(grid[i-1][j]) + 1));
					grid[i+1][j] = (char) (sign * (abs(grid[i+1][j]) + 1));
					grid[i][j-1] = (char) (sign * (abs(grid[i][j-1]) + 1));
					grid[i][j+1] = (char) (sign * (abs(grid[i][j+1]) + 1));
					drawbomb(hDC, hWnd, i - 1, j);
					drawbomb(hDC, hWnd, i + 1, j);
					drawbomb(hDC, hWnd, i, j - 1);
					drawbomb(hDC, hWnd, i, j + 1);
					}
				}
			}
		if (changed) continue;
		}
	while (changed);
	sign = 0;
	changed = 0;
	for (i = 0; i < 6; i++)
		{
		for (j = 0; j < 6; j++)
			{
			if (grid[i][j] > 0) sign += grid[i][j]; else changed -= grid[i][j];
			}
		}
	if (sign == 1) return (FALSE);
	if (sign && changed) return (FALSE);
	return (TRUE);
	}

	
#define contemplate_move_macro(x, y) if (order_grid[x][y] = TRUE, contemplate_move(levelnumber, gridptr, sign, minimum, x, y, &maximum, &max_x, &max_y, result_x != NULL)) goto end

#define maxv 3

#define SQUARE_VALUES 0

#define PLAYER -1

static int grids[maxv][6][6];

int level(int, grid_array *, int, int, int *, int *);

BOOL won(grid_array *gridptr, int *sign)
{
        register int x, y;
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

int new_level(int levelnumber, grid_array *gridptr, int sign, int minimum, int xn, int yn)
{
        int new_level = levelnumber + 1;
        grid_array *new_gridptr;

        new_gridptr = gridptr + 1;

        memcpy(new_gridptr, gridptr, sizeof(int [6][6]));
        (*new_gridptr)[xn][yn] += sign;
        adrian_processgrid(new_gridptr);
        return (level(new_level, new_gridptr, -sign, minimum, NULL, NULL));
}

BOOL contemplate_move(int levelnumber, grid_array *gridptr, int sign, int minimum, register int x, register int y, int *maximum, int *max_x, int *max_y, BOOL print)        /* TRUE = skip to end */
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

int level(int levelnumber, grid_array *gridptr, int sign, int minimum, int *result_x, int *result_y)
{
        register int x, y, *loop;
        int win_sign;
        int maximum;
        int max_x = 5, max_y = 0;
        int order_grid[6][6];

        if (won(gridptr, &win_sign)) return (win_sign * 10000);

        for (loop = order_grid[0], x = 36; x; x--) *loop++ = FALSE;

        if (levelnumber == maxv-1)
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

                if (maximum == -20000 * sign);                                                   /* oh dear */
                        //ERROR("There is no legal computer move (program may produce erroneous results henceforth)");
        }

end:    if (result_x) { *result_x = max_x; *result_y = max_y; }
        return (maximum);
}

void first_move(grid_array *gridptr, int *result_x, int *result_y)
{
        register int x, y;
        BOOL left;

        for (x = 0; x < 6; x++)
                for (y = 0; y < 6; y++)
                        if (grids[0][x][y] != 0)
                                left = x < 3;

        *result_x = left ? 5 : 0;
        *result_y = 0;
}

void think(int *result_x, int *result_y)
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
        HCURSOR oldc;
        
        hDC = GetDC(hWnd);

        oldc = SetCursor(LoadCursor(NULL, IDC_WAIT));
        
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
        SetCursor(oldc);
}
  
static grid_array *gridptr;

void do1(grid_array *gridptr, int sign, register int x, register int y)
{
        if (x != -1 && y != -1)
        {
                if ((*gridptr)[x][y] < 0) (*gridptr)[x][y] = -(*gridptr)[x][y];
                (*gridptr)[x][y] += 1;
                (*gridptr)[x][y] *= sign;
        }
}

void over(grid_array *gridptr, BOOL *finished, int max, register int x, register int y, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3)
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

void adrian_processgrid(register grid_array *grid)
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


#include <jni.h>
#include "JavaAssignment.h"
#include "Mazecommon.h"
#include "Maze.h"
#include "IOFile.h"

#include "Algorithm.h"

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>

#define DLGTITLE  L"Maze Size Input Window"
#define DLGFONT   L"MS Sans Serif"
#define DLGAPPLY  L"&Ok"
#define DLGCANCEL L"&Cancel"
#define NUMCHARS(aa) (sizeof(aa)/sizeof((aa)[0]))

#define DLGCTITLE				L"Color Selection for GUI Window"
#define DLGCFONT				L"MS Sans Serif"
#define DLGCAPPLY				L"&Ok"
#define DLGCCANCEL				L"&Cancel"
#define IDCC_BITMAP				8000
#define IDCOK					8001
#define IDCCANCEL				8002
#define ID_WALLBTN				8003
#define ID_PASSBTN				8004
#define IDCCPASSEDIT			8005
#define IDCCWALLEDIT			8006

#define IDC_BITMAP				99
#define ID_FILE_OPEN			9000
#define ID_FILE_EXIT			9001
#define ID_HELP_ABOUT			9002
#define ID_GENERATE_RECURSIVE	9003
#define ID_GENERATE_HK			9004
#define ID_GENERATE_BONUS	    9005
#define IDC_MAIN_BUTTON			9006	
#define IDC_MAIN_EDIT			9007	
#define ID_INSERTCOORD_LOCATION 9008

#define IDC_COK					1013
#define IDC_CCANCEL				1014
#define IDC_BTNWALL				1015
#define IDC_PASSBTN				1016
#define IDC_EDITWALL			1017
#define IDC_EDITPASS			1018
#define ID_SOLVEBUTTON			1019
#define ID_CHANGECOLORBUTTON	1020
#define IDC_OKK					1021
#define IDC_CANCELL				1022
#define ID_SOLVESTEPBUTTON      1023
#define ID_SOLVE				1024

static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("OOP Maze Generator Studio");

HWND hEdit, hEdit2, ghwndEdit;
Maze newMaze;
HINSTANCE hInst;

WNDCLASSEX wcex;
IOFile newFile;

int step = 0;
int actualStep = 0;

COLORREF wColor = RGB(255,0,0);
COLORREF pColor = RGB(0,0,255);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void OpenDialog(HWND);
void LoadFile(LPSTR);

void generateBtnStepSolve(HWND hwnd);
void generateBtnSolve(HWND hwnd);

void generateSolveButton(HWND hwnd);

void generateBtnColor(HWND hwnd);
COLORREF ShowColorDialog(HWND hwnd);

using namespace std;


bool necroBool = false;
bool necroAgent = false;
bool necroTarget = false;

char outCStr[10] = "javaExit";

HWND WINAPI CreateDialogIndirectParam
(
_In_opt_  HINSTANCE hInstance,
_In_      LPCDLGTEMPLATE lpTemplate,
_In_opt_  HWND hWndParent,
_In_opt_  DLGPROC lpDialogFunc,
_In_      LPARAM lParamInit
);

INT_PTR WINAPI DialogBoxIndirectParam
(
_In_opt_  HINSTANCE hInstance,
_In_      LPCDLGTEMPLATE hDialogTemplate,
_In_opt_  HWND hWndParent,
_In_opt_  DLGPROC lpDialogFunc,
_In_      LPARAM dwInitParam
);

static struct
{
	DWORD  style;
	DWORD  dwExtendedStyle;
	WORD   ccontrols;
	short  x;
	short  y;
	short  cx;
	short  cy;
	WORD   menu;																// name or ordinal of a menu resource
	WORD   windowClass;															// name or ordinal of a window class
	WCHAR  wszTitle[NUMCHARS(DLGTITLE)];										// title string of the dialog box
	short  pointsize;															// only if DS_SETFONT flag is set
	WCHAR  wszFont[NUMCHARS(DLGFONT)];											// typeface name, if DS_SETFONT is set

	struct																		// control info
	{
		DWORD  style;
		DWORD  exStyle;
		short  x;
		short  y;
		short  cx;
		short  cy;
		WORD   id;
		WORD   sysClass;														// 0xFFFF identifies a system window class
		WORD   idClass;															// ordinal of a system window class
		WCHAR  wszTitle[NUMCHARS(DLGAPPLY)];
		WORD   cbCreationData;													// bytes of following creation data
	} apply;

	struct
	{
		DWORD  style;
		DWORD  exStyle;
		short  x;
		short  y;
		short  cx;
		short  cy;
		WORD   id;
		WORD   sysClass;														// 0xFFFF identifies a system window class
		WORD   idClass;															// ordinal of a system window class
		WCHAR  wszTitle[NUMCHARS(DLGCANCEL)];
		WORD   cbCreationData;													// bytes of following creation data
	} cancel;

	struct
	{
		DWORD  style;
		DWORD  exStyle;
		short  x;
		short  y;
		short  cx;
		short  cy;
		WORD   id;
		WORD   sysClass;														// 0xFFFF identifies a system window class
		WORD   idClass;															// ordinal of a system window class
		WCHAR  wszTitle[1];														// title string or ordinal of a resource
		WORD   cbCreationData;													// bytes of following creation data
	} bitmap;

} g_DebugDlgTemplate = {
	WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU	| DS_MODALFRAME | DS_3DLOOK | DS_SETFONT,
	0x0, 3,																		
	0, 0, 250, 80,
	0, 0, DLGTITLE,	8, DLGFONT,
	{
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP | BS_DEFPUSHBUTTON, WS_EX_NOPARENTNOTIFY,													// 0x4
		90, 50, 50, 14,
		IDOK, 
		0xFFFF, 0x0080,DLGAPPLY, 0,
	},

	{
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON, WS_EX_NOPARENTNOTIFY,													
		144, 50, 50, 14,
		IDCANCEL,
		0xFFFF, 0x0080,DLGCANCEL, 0,
	},

	{
		WS_CHILD | WS_VISIBLE | WS_GROUP | SS_LEFT,	WS_EX_NOPARENTNOTIFY,													
		6, 6, 288, 8,
		IDC_BITMAP,
		0xFFFF, 0x0082,	L"", 0,
	},
};

#pragma pack(pop)

INT_PTR CALLBACK Debug_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG:
		{
			hEdit = CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",
				WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_NUMBER | ES_AUTOVSCROLL |	ES_AUTOHSCROLL,
				10,10,300,25,		
				hwnd,(HMENU)IDC_MAIN_EDIT,GetModuleHandle(NULL),NULL);
			HGDIOBJ hfDefault = GetStockObject(DEFAULT_GUI_FONT);
			SendMessage(hEdit,WM_SETFONT,(WPARAM)hfDefault,MAKELPARAM(FALSE, 0));
			SendMessage(hEdit,WM_SETTEXT,NULL,(LPARAM)"Insert Width here .....");

			hEdit2 = CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",
				WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_NUMBER | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
				10,50,300,25,																
				hwnd,(HMENU)IDC_MAIN_EDIT,GetModuleHandle(NULL),NULL);
			HGDIOBJ hfDefault2 = GetStockObject(DEFAULT_GUI_FONT);
			SendMessage(hEdit2,WM_SETFONT,(WPARAM)hfDefault2,MAKELPARAM(FALSE, 0));
			SendMessage(hEdit2,WM_SETTEXT,NULL,(LPARAM)"Insert Height here .....");

			break;
		}
		case WM_COMMAND:
		{
			UINT wId = LOWORD(wParam);

			if (wId == IDOK)
			{
				char buffer[256];
				SendMessage(hEdit,WM_GETTEXT,sizeof(buffer) / sizeof(buffer[0]),reinterpret_cast<LPARAM>(buffer));

				char buffer2[256];
				SendMessage(hEdit2,WM_GETTEXT,sizeof(buffer2) / sizeof(buffer2[0]),reinterpret_cast<LPARAM>(buffer2));

				size_t width = atoi(buffer);
				size_t height = atoi(buffer2);

				if (width < 4 || height < 4 || width > 100 || height > 100)
				{
					MessageBox(hwnd, "Please input values more than 4 and less than 100","Error", MB_OK | MB_ICONERROR);
				}
				else
				{
					EndDialog(hwnd, wId);
					newMaze.createMazeSize(width, height);
				}
			}
			else if (wId == IDCANCEL)
			{
				EndDialog(hwnd, wId);
			}
			break;
		}
		case WM_CLOSE:
		{
			EndDialog(hwnd, IDCANCEL);
			break;
		}
	}
	return FALSE;
}

LRESULT DoDebugDialog(HWND hwndApp, LPVOID pvData)
{
	HINSTANCE hinst = hwndApp ? (HINSTANCE)(LONG_PTR)GetWindowLongPtr(hwndApp, GWLP_HINSTANCE): (HINSTANCE)GetModuleHandle(NULL);

	return DialogBoxIndirectParamW(hinst, (LPCDLGTEMPLATEW)&g_DebugDlgTemplate, hwndApp,Debug_DlgProc, (LPARAM)pvData);
}

/*Color Dialog*/
static struct 
{
	DWORD  style;
	DWORD  dwExtendedStyle;
	WORD   ccontrols;
	short  x;
	short  y;
	short  cx;
	short  cy;
	WORD   menu;																// name or ordinal of a menu resource
	WORD   windowClass;															// name or ordinal of a window class
	WCHAR  wszTitle[NUMCHARS(DLGCTITLE)];										// title string of the dialog box
	short  pointsize;															// only if DS_SETFONT flag is set
	WCHAR  wszFont[NUMCHARS(DLGCFONT)];											// typeface name, if DS_SETFONT is set

	
	struct																		// control info
	{
		DWORD  style;
		DWORD  exStyle;
		short  x;
		short  y;
		short  cx;
		short  cy;
		WORD   id;
		WORD   sysClass;														// 0xFFFF identifies a system window class
		WORD   idClass;															// ordinal of a system window class
		WCHAR  wszTitle[NUMCHARS(DLGCAPPLY)];
		WORD   cbCreationData;													// bytes of following creation data
	} apply;

	struct 
	{
		DWORD  style;
		DWORD  exStyle;
		short  x;
		short  y;
		short  cx;
		short  cy;
		WORD   id;
		WORD   sysClass;														// 0xFFFF identifies a system window class
		WORD   idClass;															// ordinal of a system window class
		WCHAR  wszTitle[NUMCHARS(DLGCCANCEL)];
		WORD   cbCreationData;													// bytes of following creation data
	} cancel;

	struct 
	{
		DWORD  style;
		DWORD  exStyle;
		short  x;
		short  y;
		short  cx;
		short  cy;
		WORD   id;
		WORD   sysClass;														// 0xFFFF identifies a system window class
		WORD   idClass;															// ordinal of a system window class
		WCHAR  wszTitle[1];														// title string or ordinal of a resource
		WORD   cbCreationData;													// bytes of following creation data
	} bitmap;

} g_ColorDlgTemplate = {
	WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | DS_MODALFRAME | DS_3DLOOK | DS_SETFONT,
	0x0,3,          
	0, 0, 300, 150,
	0, 0, DLGCTITLE, 8, DLGCFONT,
	{
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP | BS_DEFPUSHBUTTON, 
		WS_EX_NOPARENTNOTIFY,
		190, 130, 50, 14,
		IDCOK,
		0xFFFF, 0x0080, DLGCAPPLY, 0,
	},

	{
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON, WS_EX_NOPARENTNOTIFY,
		244, 130, 50, 14,
		IDCCANCEL,
		0xFFFF, 0x0080, DLGCCANCEL, 0,
	},

	{
		WS_CHILD | WS_VISIBLE | WS_GROUP | SS_LEFT, WS_EX_NOPARENTNOTIFY,
		6, 6, 288, 8,
		IDCC_BITMAP,
		0xFFFF, 0x0082, L"", 0,
	},
};

#pragma pack(pop)
INT_PTR CALLBACK Color_DlgProc(HWND   hwnd, UINT   uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hWndButton;
		HINSTANCE hInstance = NULL;

		hWndButton = CreateWindowEx
			(0,TEXT("BUTTON"),TEXT("Browse Wall Colors"),
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			280,125,150,20,
			hwnd,(HMENU)ID_WALLBTN,hInstance,NULL);

		hWndButton = CreateWindowEx
			(0,TEXT("BUTTON"),TEXT("Browse Path Colors"),
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			280,155,150,20,                                   
			hwnd,(HMENU)ID_PASSBTN,hInstance,NULL);

		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_NUMBER | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
			10, 125, 250, 20, 
			hwnd, (HMENU)IDCCWALLEDIT, GetModuleHandle(NULL), NULL);
		HGDIOBJ hfDefault = GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(hEdit,WM_SETFONT,(WPARAM)hfDefault,MAKELPARAM(FALSE, 0));
		SendMessage(hEdit,WM_SETTEXT,NULL,(LPARAM)"Wall: Color not selected !!");

		hEdit2 = CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_NUMBER | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
			10,155,250,20,															
			hwnd, (HMENU)IDCCPASSEDIT, GetModuleHandle(NULL), NULL);
		HGDIOBJ hfDefault2 = GetStockObject(DEFAULT_GUI_FONT);
		SendMessage(hEdit2,WM_SETFONT,(WPARAM)hfDefault2,MAKELPARAM(FALSE, 0));
		SendMessage(hEdit2,WM_SETTEXT,NULL,(LPARAM)"Path: Color not selected !!");

		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			case ID_WALLBTN:
			{
				wColor = ShowColorDialog(hwnd);
				SetDlgItemText(hwnd, IDCCWALLEDIT, "Wall: Color selected !!");
				return TRUE;
				break;
			}
			case ID_PASSBTN:
			{
				pColor = ShowColorDialog(hwnd);
				SetDlgItemText(hwnd, IDCCPASSEDIT, "Path: Color selected !!");
				return TRUE;
				break;
			}
			case IDCCANCEL:
			{
				EndDialog(hwnd, -1);
			}
			case IDCOK:
			{
				newMaze.changeMazeWallColor(wColor);
				newMaze.changeMazePassColor(pColor);
				EndDialog(hwnd, -1);
			}
		}
	}
	break;
	case WM_CLOSE:
	{
		EndDialog(hwnd, -1);
		break;
	}
	}
	return FALSE;
}


LRESULT ColorDialog(HWND hwndApp, LPVOID pvData)
{
	HINSTANCE hinst = hwndApp ? (HINSTANCE)(LONG_PTR)GetWindowLongPtr(hwndApp, GWLP_HINSTANCE): (HINSTANCE)GetModuleHandle(NULL);

	return DialogBoxIndirectParamW(hinst, (LPCDLGTEMPLATEW)&g_ColorDlgTemplate, hwndApp,Color_DlgProc, (LPARAM)pvData);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	wcex = { 0 };
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	hInst = hInstance;

	HWND hwnd = CreateWindow
		(szWindowClass,szTitle,WS_OVERLAPPEDWINDOW,CW_USEDEFAULT, CW_USEDEFAULT,
		1024, 768,
		NULL,NULL,hInstance,NULL);

	if (!hwnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	ShowWindow(hwnd, SW_MAXIMIZE);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

void ClearWindow(HWND hwnd)
{
	HDC hdc;
	HBRUSH hbrush;
	RECT recClient;
	recClient = { 0 };
	GetClientRect(hwnd, &recClient);

	hbrush = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	hdc = GetDC(hwnd);
	FillRect(hdc, &recClient, hbrush);
//	InvalidateRect(hwnd, &recClient, true);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_PAINT:
		{
			ClearWindow(hwnd);
			InvalidateRect(hwnd, NULL, true);
			newMaze.renderMaze(hwnd);

	//		Sleep(250);
			UpdateWindow(hwnd);

			return DefWindowProc(hwnd, message, wParam, lParam);
			break;
		}
		case WM_ERASEBKGND:
		{
			return 1;
			break;
		}
		case WM_CREATE:
		{
			HMENU hMenu, hSubMenu;
			HANDLE hIcon, hIconSm;

			hMenu = CreateMenu();												// Dynamically Create Menu Bar

			hSubMenu = CreatePopupMenu();										// Create File Menu
			AppendMenu(hSubMenu, MF_STRING, ID_FILE_OPEN, "&Open");
			AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "&Exit");
			AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");

			hSubMenu = CreatePopupMenu();										// Create About Menu
			AppendMenu(hSubMenu, MF_STRING, ID_HELP_ABOUT, "&About");
			AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Help");

			hSubMenu = CreatePopupMenu();										// Create Maze Algo Menu
			AppendMenu(hSubMenu, MF_STRING, ID_GENERATE_RECURSIVE, "&Recursive BackTrack");
			AppendMenu(hSubMenu, MF_STRING, ID_GENERATE_HK, "&Hunt and Kill");
			AppendMenu(hSubMenu, MF_STRING, ID_GENERATE_BONUS, "&Recursive Bonus");
			AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Generate Maze");

			SetMenu(hwnd, hMenu);

			hIcon = LoadImage(NULL, "sit.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
			if (hIcon)
				SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
			else
				MessageBox(hwnd, "Could not load large icon!", "Error", MB_OK | MB_ICONERROR);

			hIconSm = LoadImage(NULL, "sit.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
			if (hIconSm)
				SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);
			else
				MessageBox(hwnd, "Could not load small icon!", "Error", MB_OK | MB_ICONERROR);

			if (newMaze.getHeight() != 0 || newMaze.getWidth() != 0)
			{
				generateSolveButton(hwnd);
				generateBtnStepSolve(hwnd);
				generateBtnSolve(hwnd);												// Generate Solve Button
			}
			generateBtnColor(hwnd);												// Generate Color Button changer

			break;
		}
		case WM_COMMAND:
		{
			if (((HWND)lParam) && (HIWORD(wParam) == BN_CLICKED))
			{
				int iMID = LOWORD(wParam);
				switch (iMID)
				{
					case ID_SOLVEBUTTON:
					{
						if (newMaze.getHeight() != 0 || newMaze.getWidth() != 0)
						{
							newFile.FileRead(hwnd, "javaTest.txt");
							newMaze.createMazeSize(newFile.getWidth(), newFile.getHeight());
							newMaze.generateByFile(hwnd, newFile.getFileContent());

							InvalidateRect(hwnd, NULL, TRUE);
							UpdateWindow(hwnd);
						}
							break;						
					}
					case ID_SOLVE:
					{
							PostMessage(hwnd, WM_CLOSE, 0, 0);
							break;
					}
					case ID_SOLVESTEPBUTTON:
					{
						if (newMaze.getHeight() != 0 || newMaze.getWidth() != 0)
						{
							string line;
							ifstream infile;
							infile.open("necroTest.txt");

							char player = ' ';
							char move = ' ';
							int xPos;
							int yPos;

							while (!infile.eof())		// To get you all the lines
							{
								getline(infile, line, ',');
								istringstream(line) >> player;
								getline(infile, line, ',');
								istringstream(line) >> move;
								getline(infile, line, ',');
								istringstream(line) >> xPos;
								getline(infile, line);
								istringstream(line) >> yPos;

								UpdateWindow(hwnd);
								InvalidateRect(hwnd, NULL, true);

								// For Agent 
								if (player == 'B' && move == 'N')
								{
									newMaze.mazeGrid[xPos][yPos] = 'A';
								}
								else if (player == 'B' && move == 'P')
								{
									newMaze.mazeGrid[xPos][yPos] = ' ';
								}
								else if (player == 'B' && move == 'R')
								{
									newMaze.mazeGrid[xPos][yPos] = '@';
								}
								// For Target
								else if (player == 'C' && move == 'N')
								{
									newMaze.mazeGrid[xPos][yPos] = 'T';
								}
								else if (player == 'C' && move == 'R')
								{
									newMaze.mazeGrid[xPos][yPos] = '1';
								}
							}
							infile.close();
						}
						break;
					}
					case ID_CHANGECOLORBUTTON:
					{
						ColorDialog(hwnd, (LPVOID)lParam);

						InvalidateRect(hwnd, NULL, true);
						UpdateWindow(hwnd);					
						newMaze.renderMaze(hwnd);
	
						break;
					}
					default:
					{
						break;
					}
				}
			}

			switch (LOWORD(wParam))
			{
				case ID_FILE_EXIT:
				{
					string necro = "javaExit2";
					istringstream(necro) >> outCStr;

					PostMessage(hwnd, WM_CLOSE, 0, 0);
					break;
				}
				case ID_HELP_ABOUT:
				{
					MessageBox(NULL,
						_T("OOP Maze Generator Studio 2015\n")
						_T("Version 2.00\n")
						_T("Copyright © OOP"),
						_T("About OOP Maze Generator Studio"),
						NULL);
					return 0;
					break;
				}
				case ID_FILE_OPEN:
				{
					int result;
					result = newFile.FileOpen(hwnd);

					switch (result)
					{
						case IDYES:
						{
							newMaze.createMazeSize(newFile.getWidth(), newFile.getHeight());
							newMaze.generateByFile(hwnd, newFile.getFileContent());
						
							InvalidateRect(hwnd, NULL, TRUE);
							UpdateWindow(hwnd);
	
							break;
						}
						case IDNO:
						{
							break;
						}
					}
						break;
				}
				case ID_GENERATE_RECURSIVE:
				{
					DoDebugDialog(hwnd, MF_STRING);
				
					newMaze.generate((ALGORITHMS_ENUM)0);

					generateSolveButton(hwnd);

					generateBtnStepSolve(hwnd);
					generateBtnSolve(hwnd);										// Generate Solve Button
					generateBtnColor(hwnd);										// Generate Color Button changer

					break;
				}
				case ID_GENERATE_HK:
				{
					DoDebugDialog(hwnd, MF_STRING);
					newMaze.generate((ALGORITHMS_ENUM)1);

					generateSolveButton(hwnd);

					generateBtnStepSolve(hwnd);
					generateBtnSolve(hwnd);										// Generate Solve Button
					generateBtnColor(hwnd);										// Generate Color Button changer
					break;
				}
				case ID_GENERATE_BONUS:
				{
					DoDebugDialog(hwnd, MF_STRING);
					newMaze.generate((ALGORITHMS_ENUM)2);

					generateSolveButton(hwnd);

					generateBtnStepSolve(hwnd);
					generateBtnSolve(hwnd);										// Generate Solve Button
					generateBtnColor(hwnd);										// Generate Color Button changer
					break;
				}
			}
			break;
		}
		case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		case WM_KEYDOWN:
		{
			if (wParam == 'A')
			{
				if (newMaze.getHeight() == 0 || newMaze.getWidth() == 0)
				{
					MessageBox(hwnd, "No Maze Generated. !!! \nGenerate a Maze first before using ", "Agent - Error", MB_OK | MB_ICONERROR);
				}
				else
				{
					POINT p;

					if (GetCursorPos(&p))
					{
					}

					int sizeConstant = 30, s2 = 75;														// Maze wall thickness constant
					int width3 = newMaze.getHeight();
					int height3 = newMaze.getWidth();

					if (width3 > 20 && width3 <= 50 || height3 > 20 && height3 <= 50)
					{
						sizeConstant = sizeConstant / 2;										// Shrink the wall thickness if > 20 and <= 50
						s2 = s2 - 15;
					}
					else if (width3 > 50 || height3 > 50)
					{
						sizeConstant = sizeConstant / 4;										// Shrink the wall thickness if > 50
						s2 = s2 - 24;
					}

					int yToc = ((p.x - sizeConstant) / sizeConstant);
					int xToc = ((p.y - s2) / sizeConstant);

					int msgboxID = MessageBox(hwnd, "Is this the position you want for Agent ?", "Agent - Confirmation", MB_YESNO | MB_ICONINFORMATION);

					switch (msgboxID)
					{
						case IDYES:
						{
							int width2 = newMaze.getHeight();
							int height2 = newMaze.getWidth();

							for (int i = 0; i < width2; ++i)
							{
								for (int j = 0; j < height2; ++j)
								{
									if (newMaze.mazeGrid[i][j] == 'A')
									{
										newMaze.mazeGrid[i][j] = '1';
									}
								}
							}

							if (newMaze.mazeGrid[xToc][yToc] == '1')
							{
								newMaze.mazeGrid[xToc][yToc] = 'A';
								
								UpdateWindow(hwnd);
								InvalidateRect(hwnd, NULL, true);
							}
							else
							{
								MessageBox(hwnd, "Cannot place AGENT on Wall!", "Error", MB_OK | MB_ICONERROR);
							}

							ofstream fout("test.txt");

							if (fout.is_open())
							{
								for (int i = 0; i < width2; ++i)
								{
									for (int j = 0; j < height2; ++j)
									{
										fout << newMaze.mazeGrid[i][j] << "";
									}
									fout << "\n";
								}
//								cout << "Array data successfully saved into the file test.txt" << endl;
							}
							else																		// File could not be opened
							{
								cout << "File could not be opened. [Check C++ One]" << endl;
							}
						}
						case IDNO:
						{
//							cout << "It works - No" << endl;
						}
					}
				}
				break;
			}
			if (wParam == 'T')
			{
				if (newMaze.getHeight() == 0 || newMaze.getWidth() == 0)
				{
					MessageBox(hwnd, "No Maze Generated. !!! \nGenerate a Maze first before using ", "Target - Error", MB_OK | MB_ICONERROR);
				}
				else
				{
					POINT p;

					if (GetCursorPos(&p))
					{
					}

					int sizeConstant = 30, s2 = 75;								// Maze wall thickness constant
					int width3 = newMaze.getHeight();
					int height3 = newMaze.getWidth();

					if (width3 > 20 && width3 <= 50 || height3 > 20 && height3 <= 50)
					{
						sizeConstant = sizeConstant / 2;						// Shrink the wall thickness if > 20 and <= 50
						s2 = s2 - 15;
					}
					else if (width3 > 50 || height3 > 50)
					{
						sizeConstant = sizeConstant / 4;						// Shrink the wall thickness if > 50
						s2 = s2 - 24;
					}

					int yToc = ((p.x - sizeConstant) / sizeConstant);
					int xToc = ((p.y - s2) / sizeConstant);

					int msgboxID = MessageBox(hwnd, "Is this the position you want for Target ?", "Target - Confirmation", MB_YESNO | MB_ICONINFORMATION);

					switch (msgboxID)
					{
						case IDYES:
						{
						
							int width2 = newMaze.getHeight();
							int height2 = newMaze.getWidth();

							for (int i = 0; i < width2; ++i)
							{
								for (int j = 0; j < height2; ++j)
								{
									if (newMaze.mazeGrid[i][j] == 'T')
									{
										newMaze.mazeGrid[i][j] = '1';
									}
								}
							}
						
							if (newMaze.mazeGrid[xToc][yToc] == '1')
							{
								newMaze.mazeGrid[xToc][yToc] = 'T';

								UpdateWindow(hwnd);
								InvalidateRect(hwnd, NULL, true);
							}
							else
							{
								MessageBox(hwnd, "Cannot place Target on Wall!", "Error", MB_OK | MB_ICONERROR);
							}
						
							ofstream fout("test.txt");

							if (fout.is_open())
							{
								for (int i = 0; i < width2; ++i)
								{
									for (int j = 0; j < height2; ++j)
									{
										fout << newMaze.mazeGrid[i][j] << "";
									}
									fout << "\n";
								}
//								cout << "Array data successfully saved into the file test.txt" << endl;
							}
							else																		// File could not be opened
							{
								cout << "File could not be opened. [Check C++ One]" << endl;
							}

						}
						case IDNO:
						{
//							cout << "It works - No" << endl;
						}
						}
				}
				break;
			}
		}
		default:
		{
			return DefWindowProc(hwnd, message, wParam, lParam);
			break;
		}
	}
	return 0;
}

JNIEXPORT void JNICALL Java_JavaAssignment_sayHello(JNIEnv *env, jobject thisObj)
{
	UnregisterClass(szWindowClass, hInst);
	WinMain(0, 0, 0, 1);

	return;
}

JNIEXPORT jstring JNICALL Java_JavaAssignment_sayNecro(JNIEnv *env, jobject thisObj, jstring inJNIStr) 
{
	const char *inCStr = env->GetStringUTFChars(inJNIStr, NULL);
	if (NULL == inCStr) return NULL;

	env->ReleaseStringUTFChars(inJNIStr, inCStr); 

	return env->NewStringUTF(outCStr);
}

JNIEXPORT jintArray JNICALL Java_JavaAssignment_steps(JNIEnv *env, jobject thisObj, jintArray inJNIArray)
{
	jint *inCArray = env->GetIntArrayElements(inJNIArray, NULL);
	if (NULL == inCArray) return NULL;

	jsize length = env->GetArrayLength(inJNIArray);

	env->ReleaseIntArrayElements(inJNIArray, inCArray, 0);

	step = inCArray[0];
	actualStep = inCArray[1];
	cout << inCArray[0] << endl;
	cout << inCArray[1] << endl;

	return 0;
}

JNIEXPORT jboolean JNICALL Java_JavaAssignment_sayBoolean(JNIEnv *env, jobject thisObj, jboolean inJNIBool)
{
//	printf("\nThis is what C gets from Java: ");
	
//	if (inJNIBool) 
//	{
//		printf("True");
//	}
//	else 
//	{
//		printf("False");
//	}



	int boolWidth = newMaze.getHeight();
	int boolHeight = newMaze.getWidth();

	for (int i = 0; i < boolWidth; ++i)
	{
		for (int j = 0; j < boolHeight; ++j)
		{
			if (newMaze.mazeGrid[i][j] == 'A')
			{
				necroAgent = true;
			}
		}
	}

	for (int i = 0; i < boolWidth; ++i)
	{
		for (int j = 0; j < boolHeight; ++j)
		{
			if (newMaze.mazeGrid[i][j] == 'T')
			{
				necroTarget = true;
			}
		}
	}

	if (necroAgent == true && necroTarget == true)
	{
		necroBool = true;
	}

	return necroBool;
}

void generateSolveButton(HWND hwnd)
{
	HWND hWndButton;
	HINSTANCE hInstance = NULL;

	TCHAR actualStepTaken[500];
	wsprintf(actualStepTaken, "%d", actualStep);

	hWndButton = CreateWindowEx(0,
		TEXT("BUTTON"),
		TEXT("Solve [Close GUI]"),
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		1200, 400, 140, 40,
		hwnd, (HMENU)ID_SOLVE, hInstance, NULL);
}


void generateBtnStepSolve(HWND hwnd)
{
	HWND hWndButton;
	HINSTANCE hInstance = NULL;

	hWndButton = CreateWindowEx(0,         
		TEXT("BUTTON"),                       
		TEXT("Solve Step by Step"),                         
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,   
		1200, 450, 140, 40,
		hwnd, (HMENU)ID_SOLVESTEPBUTTON, hInstance, NULL);
}

void generateBtnSolve(HWND hwnd)
{
	HWND hWndButton;
	HINSTANCE hInstance = NULL;
	hWndButton = CreateWindowEx(0,      
		TEXT("BUTTON"),                        
		TEXT("Solve Maze"),              
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,  
		1200, 500, 140, 40,                                     
		hwnd, (HMENU)ID_SOLVEBUTTON, hInstance, NULL);
}

void generateBtnColor(HWND hwnd)
{
	HWND hWndButton;
	HINSTANCE hInstance = NULL;
	hWndButton = CreateWindowEx(0,          
		TEXT("BUTTON"),                        
		TEXT("Change Color"),                         
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,   
		1200, 550, 140, 40, 
		hwnd, (HMENU)ID_CHANGECOLORBUTTON, hInstance, NULL);
}

COLORREF ShowColorDialog(HWND hwnd) 
{
	CHOOSECOLOR cc;
	static COLORREF crCustClr[16];

	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hwnd;
	cc.lpCustColors = (LPDWORD)crCustClr;
	cc.rgbResult = RGB(0, 255, 0);
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	ChooseColor(&cc);

	return cc.rgbResult;
}
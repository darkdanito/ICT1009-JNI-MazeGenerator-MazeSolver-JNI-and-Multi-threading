#include "HelloJNICppImpl.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>

#include "Mazecommon.h"
#include "Maze.h"
#include "IOFile.h"

#define DLGTITLE  L"Maze Size Input Window"
#define DLGFONT   L"MS Sans Serif"
#define DLGAPPLY  L"&Ok"
#define DLGCANCEL L"&Cancel"
#define NUMCHARS(aa) (sizeof(aa)/sizeof((aa)[0]))
#define IDC_BITMAP 99
 
#define ID_FILE_OPEN			9000
#define ID_FILE_EXIT			9001
#define ID_HELP_ABOUT			9002
#define ID_GENERATE_RECURSIVE	9003
#define ID_GENERATE_HK			9004
#define ID_GENERATE_BONUS	    9005
#define IDC_MAIN_BUTTON			9006	
#define IDC_MAIN_EDIT			9007	

static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("OOP Maze Generator Studio");

HWND hEdit, hEdit2, ghwndEdit;
Maze newMaze;
HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void OpenDialog(HWND);
void LoadFile(LPSTR);

using namespace std;

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
//		WORD   wAlign;															// align next control to DWORD boundry.
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

	WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU								// style  0x94c800c4
	| DS_MODALFRAME | DS_3DLOOK
	| DS_SETFONT,
	0x0,																		// exStyle;
	3,																			// ccontrols
	0, 0, 250, 80,
	0,																			// menu: none
	0,																			// window class: none
	DLGTITLE,																	// Window caption
	8,																			// font pointsize
	DLGFONT,

	{
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_GROUP | BS_DEFPUSHBUTTON,		// 0x50030001
		WS_EX_NOPARENTNOTIFY,													// 0x4
		90, 50, 50, 14,
		IDOK,
		0xFFFF, 0x0080,															// button
		DLGAPPLY, 0,
	},

	{
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,						// 0x50010000
		WS_EX_NOPARENTNOTIFY,													// 0x4
		144, 50, 50, 14,
		IDCANCEL,
		0xFFFF, 0x0080,															// button
		DLGCANCEL, 0,
	},

	{
		WS_CHILD | WS_VISIBLE | WS_GROUP | SS_LEFT,								// 0x50020000
		WS_EX_NOPARENTNOTIFY,													// 0x4
		6, 6, 288, 8,
		IDC_BITMAP,
		0xFFFF, 0x0082,															// static
		L"", 0,
	},
};

#pragma pack(pop)

INT_PTR CALLBACK Debug_DlgProc(
	HWND   hwnd,
	UINT   uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (uMsg)
	{
		case WM_INITDIALOG:
		{
			hEdit = CreateWindowEx
				(
				WS_EX_CLIENTEDGE,
				"EDIT",
				"",
				WS_CHILD |
				WS_VISIBLE |
				ES_MULTILINE | 
				ES_NUMBER | 
				ES_AUTOVSCROLL | 
				ES_AUTOHSCROLL,
				10,
				10,
				300,	// Width
				25,		// Height
				hwnd,
				(HMENU)IDC_MAIN_EDIT,
				GetModuleHandle(NULL),
				NULL
				);
			HGDIOBJ hfDefault = GetStockObject(DEFAULT_GUI_FONT);
			SendMessage
				(
				hEdit,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE, 0)
				);
			SendMessage
				(
				hEdit,
				WM_SETTEXT,
				NULL,
				(LPARAM)"Insert Width here ....."
				);

			hEdit2 = CreateWindowEx(WS_EX_CLIENTEDGE,
				"EDIT",
				"",
				WS_CHILD |
				WS_VISIBLE |
				ES_MULTILINE
				| ES_NUMBER
				| ES_AUTOVSCROLL | ES_AUTOHSCROLL,
				10,
				50,
				300,															// Width
				25,																// Height
				hwnd,
				(HMENU)IDC_MAIN_EDIT,
				GetModuleHandle(NULL),
				NULL);
			HGDIOBJ hfDefault2 = GetStockObject(DEFAULT_GUI_FONT);
			SendMessage(hEdit2,
				WM_SETFONT,
				(WPARAM)hfDefault2,
				MAKELPARAM(FALSE, 0));
			SendMessage(hEdit2,
				WM_SETTEXT,
				NULL,
				(LPARAM)"Insert Height here .....");
			break;
		}
		case WM_COMMAND:
		{
			UINT wId = LOWORD(wParam);

			if (wId == IDOK)
			{
				cout << "OK Selected" << endl;

				char buffer[256];
				SendMessage
					(
					hEdit,
					WM_GETTEXT,
					sizeof(buffer) / sizeof(buffer[0]),
					reinterpret_cast<LPARAM>(buffer)
					);

				char buffer2[256];
				SendMessage
					(
					hEdit2,
					WM_GETTEXT,
					sizeof(buffer2) / sizeof(buffer2[0]),
					reinterpret_cast<LPARAM>(buffer2)
					);

				size_t width = atoi(buffer);
				size_t height = atoi(buffer2);

				newMaze.createMazeSize(width, height);

				// BUG 3:
				/*
				As for Algorithms storing.
				We are using 0, 1 , 2 for the respective Algorithm to generate the maze.

				Currently i am using 0 to static call recusrive backtracker.
				Need to switch back the number depending on which algorithm was selected.
				*/
//				newMaze.generate((ALGORITHMS_ENUM)0);

				EndDialog(hwnd, wId);
			}
			else if (wId == IDCANCEL)
			{
				cout << "Cancel Selected" << endl;
				EndDialog(hwnd, wId);
			}
			//else
			//{
			//	cout << "Error Selected" << endl;								// Bug 6
			//}
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
	HINSTANCE hinst = hwndApp ? (HINSTANCE)(LONG_PTR)GetWindowLongPtr(hwndApp, GWLP_HINSTANCE)
		: (HINSTANCE)GetModuleHandle(NULL);

	return DialogBoxIndirectParamW(hinst, (LPCDLGTEMPLATEW)&g_DebugDlgTemplate, hwndApp,
		Debug_DlgProc, (LPARAM)pvData);
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
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
		(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1024, 768,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hwnd)
	{
		MessageBox
			(
			NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL
			);

		return 1;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	InvalidateRect(hwnd, NULL, true);

	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
		case WM_PAINT:
		{
			UpdateWindow(hwnd);
			newMaze.renderMaze(hwnd);
			
			return DefWindowProc(hwnd, message, wParam, lParam);
			break;
		}
		case WM_CREATE:
		{
			HMENU hMenu, hSubMenu;
			HANDLE hIcon, hIconSm;

			hMenu = CreateMenu();

			hSubMenu = CreatePopupMenu();
			AppendMenu(hSubMenu, MF_STRING, ID_FILE_OPEN, "&Open");
			AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "&Exit");
			AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&File");
			
			hSubMenu = CreatePopupMenu();
			AppendMenu(hSubMenu, MF_STRING, ID_HELP_ABOUT, "&About");
			AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Help");
			
			hSubMenu = CreatePopupMenu();
			AppendMenu(hSubMenu, MF_STRING, ID_GENERATE_RECURSIVE, "&Recursive BackTrack");
			
			AppendMenu(hSubMenu, MF_STRING, ID_GENERATE_HK, "&Hunt and Kill");
			AppendMenu(hSubMenu, MF_STRING, ID_GENERATE_BONUS, "&Recursive Bonus");
			AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Generate Maze");

			SetMenu(hwnd, hMenu);

			// BUG 5:
			/*
			Update the code below for the showing of the icon for the Window
			
			//hIcon = LoadImage(NULL, "sit.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
			//if (hIcon)
			//	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
			//else
			//	MessageBox(hwnd, "Could not load large icon!", "Error", MB_OK | MB_ICONERROR);

			//hIconSm = LoadImage(NULL, "sit.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
			//if (hIconSm)
			//	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);
			//else
			//	MessageBox(hwnd, "Could not load small icon!", "Error", MB_OK | MB_ICONERROR);

			*/

			// Open File De
			//ghwndEdit = CreateWindowEx(WS_EX_RIGHTSCROLLBAR, TEXT("edit"), NULL,
			//	WS_VISIBLE | WS_CHILD | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE,
			//	0, 0, 260, 180,
			//	hwnd, (HMENU)1, NULL, NULL);

			break;
		}
		case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
				case ID_FILE_EXIT:
				{
					PostMessage(hwnd, WM_CLOSE, 0, 0);
					break;
				}
				case ID_HELP_ABOUT:
				{
					MessageBox
						(
						NULL,
						_T("OOP Maze Generator Studio 2015\n")
						_T("Version 2.00\n")
						_T("Copyright © OOP"),
						_T("About OOP Maze Generator Studio"),
						NULL
						);
					return 0;
					break;
				}
				case ID_FILE_OPEN:
				{
					OpenDialog(hwnd);
					break;				
				}
				case ID_GENERATE_RECURSIVE:
				{
					DoDebugDialog(hwnd, MF_STRING);
					newMaze.generate((ALGORITHMS_ENUM)0);
					break;
				}
				case ID_GENERATE_HK:
				{
					DoDebugDialog(hwnd, MF_STRING);
					newMaze.generate((ALGORITHMS_ENUM)1);
					break;
				}
				case ID_GENERATE_BONUS:
				{
					DoDebugDialog(hwnd, MF_STRING);
					newMaze.generate((ALGORITHMS_ENUM)2);
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
		default:
		{
			return DefWindowProc(hwnd, message, wParam, lParam);
			break;
		}
	}

	return 0;
}

void sayHello() 
{
	cout << "Hello World from C++!" << endl;
	WinMain(0, 0, 0, 1);
	return;
}

// BUG 4:
/*
Update the OpenDialog and LoadFile code to our D2 code.
So that read from file can work and render it.
*/
void OpenDialog(HWND hwnd)
{
	OPENFILENAME ofn;
	TCHAR szFile[MAX_PATH];


	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.hwndOwner = hwnd;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = TEXT("All files(*.*)\0*.*\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrFileTitle = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	if (GetOpenFileName(&ofn))
		LoadFile(ofn.lpstrFile);
}

void LoadFile(LPSTR file)														// Can remove i think
{
	HANDLE hFile;
	DWORD dwSize;
	DWORD dw;

	LPBYTE lpBuffer = NULL;

	hFile = CreateFile(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	dwSize = GetFileSize(hFile, NULL);
	lpBuffer = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, dwSize + 1);
	ReadFile(hFile, (LPWSTR)lpBuffer, dwSize, &dw, NULL);
	CloseHandle(hFile);
	lpBuffer[dwSize] = 0;
	SetWindowText(ghwndEdit, (LPSTR)lpBuffer);
	HeapFree(GetProcessHeap(), 0, lpBuffer);
}
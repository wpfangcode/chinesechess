// chinesecheck.cpp : 定義應用程式的進入點。
//

#include "stdafx.h"
#include "chinesecheck.h"
#include "chess.h"
#define MAX_LOADSTRING 100

// 全域變數:
HINSTANCE hInst;                                // 目前執行個體
WCHAR szTitle[MAX_LOADSTRING];                  // 標題列文字
WCHAR szWindowClass[MAX_LOADSTRING];            // 主視窗類別名稱

// 這個程式碼模組所包含之函式的向前宣告:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	WndBoardProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
const int ID_STATE_STORE = 1;
const int ID_STATE_REPLAY = 2;

int state;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置程式碼。

    // 將全域字串初始化
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CHINESECHECK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 執行應用程式初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CHINESECHECK));

    MSG msg;

    // 主訊息迴圈:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHINESECHECK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CHINESECHECK);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

ATOM MyBoardRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndBoardProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CHINESECHECK);
	wcex.lpszClassName = L"boardwnd";
	wcex.hIconSm =0;

	return RegisterClassExW(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 將執行個體控制代碼儲存在全域變數中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
HWND hwndData;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{		
	static HWND hwndBoard;
	static HWND hwndData;
	switch (message)
	{
	case WM_CREATE:
	{
		MyBoardRegisterClass(hInst);
		hwndBoard= CreateWindowW(L"boardwnd",L"", WS_CHILD|WS_VISIBLE,
			0, 0, 0, 0,hWnd, (HMENU)1, hInst, nullptr);
		hwndData= CreateWindowW(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL |
			ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
			0, 0, 0, 0, hWnd, (HMENU)2, hInst, nullptr);
		::hwndData = hwndData;
		
	}
	break;
	case WM_SIZE:
	{
		int W = LOWORD(lParam);
		int H = HIWORD(lParam);
		int w = 200;
		MoveWindow(hwndBoard, w, 0, W - w, H, true);
		MoveWindow(hwndData, 10, 10, w-2, H-10, true);
	}
	break;
	case WM_CHAR:
		SendMessage(hwndBoard, WM_CHAR, wParam, lParam);
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 剖析功能表選取項目:
		switch (wmId)
		{
		case ID_LOGFILE:
		{
			std::ofstream file;
			file.open(L"log.txt");
			const int buffersize = 1024;
			TCHAR s[buffersize];
			int l=GetWindowText(hwndData, s, buffersize);
			file.write((char*) s, l);
			file.close();
		}
			break;
		case ID_TOOLS_RESET:
		{
			SendMessage(hwndBoard, WM_COMMAND, wParam, lParam);
			SetWindowText(hwndData, L"");
		}
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
LRESULT CALLBACK WndBoardProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static Board board;
	static VECTOR oldPosition, Position;
	static std::wstring log;
    switch (message)
    {
	case WM_CREATE:
	{
		RECT rc;
		GetClientRect(hWnd, &rc);
		int W = rc.right;
		int H = rc.bottom;
		board.init(W, H);
		board.setConnectFile(L"chessboard.txt");

	}
	break;
	case WM_SIZE:
	{
		RECT rc;
		GetClientRect(hWnd, &rc);
		int W = rc.right;
		int H = rc.bottom;
		board.reSize(W, H);
	}
	break;
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		VECTOR v;
		v = board.where(x, y);
		int id = board.which(v);
		TCHAR s[64];
		wsprintf(s, L"%d %s", id, v.toString().c_str());
		SetWindowText(GetParent(hWnd),s);
		
	}

		break;
	case WM_CHAR:
	{
		//std::wstring player1 = L"play.exe";
		//board.runNPC(player1);
		board.nextStep();
		RECT rc;
		GetClientRect(hWnd, &rc);
		InvalidateRect(hWnd, &rc, 1);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		oldPosition = board.where(x, y);
		int id = board.which(oldPosition);
		TCHAR s[64];
		wsprintf(s, L"%d %d", id/100,id%100);
		log += s;
		log += L" ";
		log += oldPosition.toString();
		log += L" ";
	}
	break;
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Position = board.where(x, y);
		int id = board.which(Position);
		board.move(oldPosition, Position);
		RECT rc;
		GetClientRect(hWnd, &rc);
		InvalidateRect(hWnd, &rc, 1);
		log += Position.toString();
		log += L"\r\n";
		SetWindowText(hwndData, log.c_str());
	}
	break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 剖析功能表選取項目:
            switch (wmId)
            {
			case ID_TOOLS_RESET:
			{
				RECT rc;
				GetClientRect(hWnd, &rc);
				board.init(rc.right,rc.bottom);
				InvalidateRect(hWnd, &rc, 1);
			}
				break;

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			board.show(hdc);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		board.writeBoard(L"table.txt");
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// [關於] 方塊的訊息處理常式。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Game0828.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Game0828.h"

#include "spriteBase.h"

#include <comdef.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")


//
//  zmx
//
//

/*控制台*/
enum msgType                      //消息类型
{
	mtLog,
	mtWarning,
	mtError,
};
HANDLE hConsole;                  //控制台句柄

void initByZMX(void);             //控制台窗口初始化
void printfMsg(TCHAR *, msgType); //输出日志

/*精灵*/
std::list<spriteBase> drawImageList;


/*全局*/
void initZMX(void);
void updateDraw(HWND hWnd, HDC hdc);
void drawImageWithCoorAndAnchor(HWND, HDC, Gdiplus::Image *, Gdiplus::PointF, Gdiplus::PointF);



#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void CALLBACK showImage(HWND, UINT, UINT, DWORD);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。
	initZMX();

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GAME0828, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAME0828));

    MSG msg;

    // 主消息循环: 
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



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAME0828));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GAME0828);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

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

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		{
			SetTimer(hWnd, 1, 20, (TIMERPROC)showImage);
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
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
            // TODO: 在此处添加使用 hdc 的任何绘图代码...

			//
			updateDraw(hWnd, hdc);

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

// “关于”框的消息处理程序。
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

//
// zmx
//
//
//
void initByZMX(void)
{
	AllocConsole();

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//if (hConsole)
	//{
	//	//中文一个字也算1个长度
	//	WriteConsole(hConsole, _T("hello\n"), 6, NULL, NULL);
	//	WriteConsole(hConsole, _T("123\n"), 4, NULL, NULL);
	//	WriteConsole(hConsole, _T("你\n"), 2, NULL, NULL);
	//	
	//	TCHAR msg[256];
	//	memset(msg, 0, sizeof(msg));
	//	_stprintf_s(msg, _countof(msg), _T("%s\n"), _T("好"));
	//	WriteConsole(hConsole, msg, wcslen(msg), NULL, NULL);
	//}
	//FreeConsole();
	return;
}
void printfMsg(TCHAR *msg, msgType mt)
{
	//保存旧属性
	_CONSOLE_SCREEN_BUFFER_INFO lpc;
	GetConsoleScreenBufferInfo(hConsole, &lpc);
	WORD oldTextAttr = lpc.wAttributes;

	//日期
	TCHAR dateTime[256];
	memset(dateTime, 0, sizeof(dateTime));
	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);
	_stprintf_s(dateTime, _countof(dateTime), _T("[%04d-%02d-%02d %02d:%02d:%02d]:"),
		st.wYear,
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond);
	WriteConsole(hConsole, dateTime, wcslen(dateTime), NULL, NULL);

	//设置颜色
	switch (mt)
	{
	case mtLog:
	{
		//白
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	}
	case mtWarning:
	{
		//黄
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);

		break;
	}
	case mtError:
	{
		//红
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);

		break;
	}
	default:
		break;
	}

	//输出
	WriteConsole(hConsole, msg, wcslen(msg), NULL, NULL);
	//换行
	WriteConsole(hConsole, _T("\n"), 1, NULL, NULL);
	//换回之前的属性
	SetConsoleTextAttribute(hConsole, oldTextAttr);

	return;
}

void CALLBACK showImage(HWND hWnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	//modifyValue();

	InvalidateRect(hWnd, NULL, false);
	SendMessage(hWnd, WM_PAINT, MAKELONG(1, 0), 1);

	//MessageBox(hWnd, _T("callback"), _T(""), MB_OK);
}


void initZMX(void)
{
	//控制台
	initByZMX();

	//gdi+初始化
	ULONG_PTR gdipToken;
	Gdiplus::GdiplusStartupInput gdipInput;
	Gdiplus::GdiplusStartup(&gdipToken, &gdipInput, NULL);

	initList();
}

void updateDraw(HWND hWnd, HDC hdc)
{
	//MessageBox(hWnd, _T("draw"), _T(""), MB_OK);


	//双缓冲，创建内存dc
	RECT rect;
	GetClientRect(hWnd, &rect);//获取客户区尺寸
	SIZE ClientSize;
	ClientSize.cx = rect.right - rect.left;
	ClientSize.cy = rect.bottom - rect.top;
	HDC mHdc1 = CreateCompatibleDC(hdc);//创建内存DC，初始只有1x1像素大小
	HBITMAP hBmp = CreateCompatibleBitmap(hdc, ClientSize.cx, ClientSize.cy);//扩大这个区域
	SelectObject(mHdc1, hBmp);



	HDC  mHdc = mHdc1;
	//Gdiplus::Graphics	graph(mHdc);
	//Gdiplus::Image		image(_T("D:\\PSIMAGE\\map.png"));
	//graph.DrawImage(&image, 0, 0);

	//绘制
	drawImageWithList(hWnd, mHdc);

	//双缓冲,将内存dc拷至设备dc
	BitBlt(hdc, 0, 0, ClientSize.cx, ClientSize.cy, mHdc1, 0, 0, SRCCOPY);
	DeleteDC(mHdc1);
}
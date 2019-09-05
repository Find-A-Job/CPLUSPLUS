// Game0826.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Game0826.h"

#include <comdef.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
//using namespace Gdiplus;
//using Gdiplus::Font;
using Gdiplus::GdiplusStartupInput;
using Gdiplus::GdiplusStartup;
using Gdiplus::GdiplusShutdown;
using Gdiplus::Image;
using Gdiplus::Graphics;
using Gdiplus::Pen;
using Gdiplus::Color;
using Gdiplus::SolidBrush;
using Gdiplus::FontFamily;
using Gdiplus::PointF;
using Gdiplus::FontStyleRegular;
using Gdiplus::UnitPixel;
using Gdiplus::Status;


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



/*
	designed by zmx



*/
#define SCREENWIDTH			GetSystemMetrics(SM_CXSCREEN)
#define SCREENHEIGHT		GetSystemMetrics(SM_CYSCREEN)
#define WINDOWWIDTH			640
#define WINDOWHEIGHT		480
#define STARTTIME			(6 * 60)						///单位：秒
#define LIGHTLIMIT			0								///最亮
#define DARKLIMIT			210								///最暗
#define FRAMERATE			50								///每秒帧率

enum msgType						//消息类型
{
	mtLog,
	mtWarning,
	mtError,
};
HANDLE hConsole;					//控制台句柄

ULONG64 timeNow;					//当前时间，单位：分
bool	isOkToDraw;					//

typedef Gdiplus::Image	*pImage;
Gdiplus::Image			*mapImage; //
Gdiplus::Image			*moveDown[3];
Gdiplus::Image			*moveUp[3];
Gdiplus::Image			*moveLeft[3];
Gdiplus::Image			*moveRight[3];
pImage	pImgMoveDown[3];
pImage	pImgMoveUp[3];
pImage	pImgMoveLeft[3];
pImage	pImgMoveRight[3];

Gdiplus::Rect			destRect;

Gdiplus::Point perCoor(100, 100);
//Gdiplus::

void initByZMX(void);				//控制台窗口初始化
void printfMsg(TCHAR *, msgType);	//输出日志

void drawImage(HWND, HDC, int);	//绘图
void drawImagePerson(HWND, HDC);	//绘图
void drawImageWithCoorAndAnchor(HWND, HDC, Gdiplus::Image *, Gdiplus::Point, Gdiplus::PointF);
void kingOfDraw(HWND, HDC, ULONG64);//总绘图


void CALLBACK    timePassProc(HWND, UINT, UINT, DWORD);//计时器回调
void CALLBACK	personMoveProc(HWND, UINT, UINT, DWORD);
void CALLBACK	updateScreen(HWND hWnd, UINT message, UINT iTimerID, DWORD dwTime);

/*
	入口函数
*/
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

	initByZMX();
	printfMsg(_T("颜色测试：白"), mtLog);
	printfMsg(_T("颜色测试：黄"), mtWarning);
	printfMsg(_T("颜色测试：红"), mtError);
	printfMsg(_T("/*-----------------*/"), mtLog);
	printfMsg(_T("/* designed by zmx */"), mtLog);
	printfMsg(_T("/*-----------------*/"), mtLog);


    // 初始化全局字符串
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	_stprintf_s(szTitle, _countof(szTitle), _T("大自然"));
    //LoadStringW(hInstance, IDC_GAME0826, szWindowClass, MAX_LOADSTRING);
	_stprintf_s(szWindowClass, _countof(szWindowClass), _T("大自然"));
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAME0826));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAME0826));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GAME0826);
	wcex.lpszClassName	= szWindowClass;// _T("大自然");
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,(SCREENWIDTH - WINDOWWIDTH) / 2, 
	   (SCREENHEIGHT - WINDOWHEIGHT) / 2, WINDOWWIDTH, WINDOWHEIGHT, nullptr, nullptr, hInstance, nullptr);

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
	//TCHAR msgTop[256];
	//memset(msgTop, 0, sizeof(msgTop));
	//_stprintf_s(msgTop, _countof(msgTop), _T("WM_PAINT:0x%04X"), message);
	//printfMsg(msgTop, mtLog);

    switch (message)
    {
	case WM_TIMER:
		{

		}
		break;
	case WM_CREATE:
		{
			SetTimer(hWnd, 1, 1000, (TIMERPROC)timePassProc);

			SetTimer(hWnd, 3, (UINT)(1000 / FRAMERATE), (TIMERPROC)updateScreen);
		}
		break;
	case WM_ACTIVATE:
		{

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

			////双缓冲
			//RECT rect;
			//GetClientRect(hWnd, &rect);//客户区尺寸
			//SIZE ClientSize;
			//ClientSize.cx = rect.right - rect.left;
			//ClientSize.cy = rect.bottom - rect.top;
			//HDC mHdc		= CreateCompatibleDC(hdc);//创建内存DC，初始只有1x1像素大小
			//HBITMAP hBmp	= CreateCompatibleBitmap(hdc, ClientSize.cx, ClientSize.cy);//扩大这个区域
			//SelectObject(mHdc, hBmp);

			//绘图
			//kingOfDraw(hWnd, hdc, timeNow);

			////双缓冲
			//BitBlt(hdc, 0, 0, ClientSize.cx, ClientSize.cy, mHdc, 0, 0, SRCCOPY);

			//Gdiplus::Graphics graph(hdc);
			//gpSolidBrush pciBrush(gpColor(255, 0, 0, 0));
			//graph.FillRectangle(&pciBrush, 0, 0, 60, 150);

			//gpBitmap image3(_T("D:\\PSIMAGE\\map.png"));
			////bitmap属性
			//gpColorMatrix colorMatrix = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			//	0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			//	0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			//	0.0f, 0.0f, 0.0f, 0.8f, 0.0f,
			//	0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
			//gpImageAttr imageAttr;
			//imageAttr.SetColorMatrix(&colorMatrix, Gdiplus::ColorMatrixFlagsDefault, Gdiplus::ColorAdjustTypeBitmap);

			//graph.DrawImage(&image3, gpMakeRectF(0, 0, 100, 100), 0, 0, 100, 100, Gdiplus::UnitPixel, &imageAttr);

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


/*
  designed by zmx


*/
//各种初始化
void initByZMX(void)
{
	//控制台窗口初始化
	AllocConsole();
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//控制台会一直保持到进程销毁，所以就不free了
	///FreeConsole();

	//gdi+初始化
	ULONG_PTR gdipToken;
	GdiplusStartupInput gdipInput;
	GdiplusStartup(&gdipToken, &gdipInput, NULL);

	//gdi+会一直持续到进程销毁，所以就不free了
	///GdiplusShutdown(gdipToken);

	//起始时间
	timeNow = STARTTIME;

	isOkToDraw = false;


	//全局变量初始化
	Gdiplus::Image image(_T("D:\\PSIMAGE\\map.png"));
	mapImage = image.Clone();

	destRect.X		= 0;
	destRect.Y		= 0;
	destRect.Width	= mapImage->GetWidth();
	destRect.Height	= mapImage->GetHeight();

	Gdiplus::Image imgMoveDown0(_T("D:\\PSIMAGE\\man_walkWithBackgroundcolor\\moveDown0.png"));
	Gdiplus::Image imgMoveDown1(_T("D:\\PSIMAGE\\man_walkWithBackgroundcolor\\moveDown1.png"));
	Gdiplus::Image imgMoveDown2(_T("D:\\PSIMAGE\\man_walkWithBackgroundcolor\\moveDown2.png"));
	Gdiplus::Image imgMoveUp0(_T("D:\\PSIMAGE\\man_walkWithBackgroundcolor\\moveUp0.png"));
	Gdiplus::Image imgMoveUp1(_T("D:\\PSIMAGE\\man_walkWithBackgroundcolor\\moveUp1.png"));
	Gdiplus::Image imgMoveUp2(_T("D:\\PSIMAGE\\man_walkWithBackgroundcolor\\moveUp2.png"));
	Gdiplus::Image imgMoveRight0(_T("D:\\PSIMAGE\\man_walkWithBackgroundcolor\\moveLR0.png"));
	Gdiplus::Image imgMoveRight1(_T("D:\\PSIMAGE\\man_walkWithBackgroundcolor\\moveLR1.png"));
	Gdiplus::Image imgMoveRight2(_T("D:\\PSIMAGE\\man_walkWithBackgroundcolor\\moveLR2.png"));
	Gdiplus::Image imgMoveLeft0(_T("D:\\PSIMAGE\\man_walkWithBackgroundcolor\\moveLR0.png"));
	Gdiplus::Image imgMoveLeft1(_T("D:\\PSIMAGE\\man_walkWithBackgroundcolor\\moveLR1.png"));
	Gdiplus::Image imgMoveLeft2(_T("D:\\PSIMAGE\\man_walkWithBackgroundcolor\\moveLR2.png"));
	pImgMoveDown[0] = imgMoveDown0.Clone();
	pImgMoveDown[1] = imgMoveDown1.Clone();
	pImgMoveDown[2] = imgMoveDown2.Clone();
	pImgMoveUp[0] = imgMoveUp0.Clone();
	pImgMoveUp[1] = imgMoveUp1.Clone();
	pImgMoveUp[2] = imgMoveUp2.Clone();
	pImgMoveLeft[0] = imgMoveLeft0.Clone();
	pImgMoveLeft[1] = imgMoveLeft1.Clone();
	pImgMoveLeft[2] = imgMoveLeft2.Clone();
	pImgMoveRight[0] = imgMoveRight0.Clone();
	pImgMoveRight[1] = imgMoveRight1.Clone();
	pImgMoveRight[2] = imgMoveRight2.Clone();


	
	return;
}
//打印日志
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
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED| FOREGROUND_GREEN);

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
//绘图
void drawImage(HWND hWnd, HDC hdc, int alphaVal)
{
	if ((mapImage)->GetLastStatus() != Status::Ok)
	{
		printfMsg(_T("图片打开失败"), mtError);

		return;
	}
	Gdiplus::Graphics graph(hdc);
	graph.DrawImage(mapImage, destRect, 0, 0, mapImage->GetWidth(), mapImage->GetHeight(), Gdiplus::UnitPixel);

	//绘制路径并填充
	Gdiplus::GraphicsPath	path;
	Gdiplus::SolidBrush		maskBrush(Gdiplus::Color(alphaVal, 0, 0, 0));
	path.AddLine(0, 0, WINDOWWIDTH, 0);
	path.AddLine(WINDOWWIDTH, 0, WINDOWWIDTH, WINDOWHEIGHT);
	path.AddLine(WINDOWWIDTH, WINDOWHEIGHT, 0, WINDOWHEIGHT);
	path.AddLine(0, WINDOWHEIGHT, 0, 0);
	graph.FillPath(&maskBrush, &path);



	////开始绘画
	//Graphics graph(hdc);
	////graph.DrawImage(&img, 0, 0, img.GetWidth(), img.GetHeight());


	//////画线
	////Pen p(Color(255, 0, 0, 255));
	////graph.DrawLine(&p, 0, 0, 200, 100);

	//////文字
	////SolidBrush		brush(Color(255, 0, 0, 255));
	////FontFamily		fontFamily(_T("Times New Roman"));
	////Gdiplus::Font	font(&fontFamily, 24, FontStyleRegular, UnitPixel);
	////PointF			pointF(10.0f, 20.0f);
	////graph.DrawString(_T("中文"), -1, &font, pointF, &brush);


	return;
}
//画人物
void drawImagePerson(HWND hWnd, HDC mHdc)
{
	Gdiplus::Point coor(200, 200);
	Gdiplus::PointF anchor(0.5, 0);
	Gdiplus::Size	size(0, 0);
	

	coor.X = 200;
	coor.Y = 200;
	anchor.X = 0.5;
	anchor.Y = 0;
	size.Width = pImgMoveDown[0]->GetWidth();
	size.Height = pImgMoveDown[0]->GetHeight();

	//根据锚点绘图
	drawImageWithCoorAndAnchor(hWnd, mHdc, pImgMoveDown[0], perCoor, anchor);

	//开始动画
	SetTimer(hWnd, 2, 300, (TIMERPROC)personMoveProc);

	return;
}
void drawImageWithCoorAndAnchor(HWND hWnd, HDC hdc, Gdiplus::Image *img, Gdiplus::Point coor, Gdiplus::PointF anchor)
{
	//锚点，左上角为(0, 0),右下角为(1, 1)
	Gdiplus::Graphics graph(hdc);

	Gdiplus::Rect rect(coor.X - (float)(anchor.X * img->GetWidth()), coor.Y - (float)(anchor.Y * img->GetHeight()), img->GetWidth(), img->GetHeight());
	graph.DrawImage(img, rect, 0, 0, img->GetWidth(), img->GetHeight(), UnitPixel);

	//TCHAR msgTop[256];
	//memset(msgTop, 0, sizeof(msgTop));
	//_stprintf_s(msgTop, _countof(msgTop), _T("rect:%d, %d, %d, %d"), rect.X, rect.Y, rect.Width, rect.Height);
	//printfMsg(msgTop, mtLog);
}

void kingOfDraw(HWND hWnd, HDC devHdc, ULONG64 time)
{
	//双缓冲，创建内存dc
	RECT rect;
	GetClientRect(hWnd, &rect);//客户区尺寸
	SIZE ClientSize;
	ClientSize.cx	= rect.right - rect.left;
	ClientSize.cy	= rect.bottom - rect.top;
	HDC mHdc1		= CreateCompatibleDC(devHdc);//创建内存DC，初始只有1x1像素大小
	HBITMAP hBmp	= CreateCompatibleBitmap(devHdc, ClientSize.cx, ClientSize.cy);//扩大这个区域
	SelectObject(mHdc1, hBmp);

	//计算alpha值
	ULONG64 imageAlpha;		//alpha值
	switch (time / 60)
	{
	case 4:
	case 5:
	case 6:
	case 7:
		{
			//渐亮，四点至八点
			ULONG64 val=(ULONG64)(DARKLIMIT*((time - 60 * 4) / (double)(60 * 4)));

			imageAlpha = DARKLIMIT - val;
		}
		break;
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		{
			//最亮，八点至十六点
			imageAlpha = LIGHTLIMIT;
		}
		break;
	case 16:
	case 17:
	case 18:
	case 19:
		{
			//渐暗，十六点至二十点
			ULONG64 val = (ULONG64)(DARKLIMIT*((time - 60 * 16) / (double)(60 * 4)));

			imageAlpha =  val;
		}
		break;
	case 20:
	case 21:
	case 22:
	case 23:
	case 0:
	case 1:
	case 2:
	case 3:
		{
			//最暗，二十点至四点
			imageAlpha = DARKLIMIT;
		}
		break;

	default:
		imageAlpha = 0;
		break;
	}

	//画背景
	drawImage(hWnd, mHdc1, imageAlpha);

	//画人物
	drawImagePerson(hWnd, mHdc1);

	
	//双缓冲,将内存dc拷至设备dc
	BitBlt(devHdc, 0, 0, ClientSize.cx, ClientSize.cy, mHdc1, 0, 0, SRCCOPY);
	DeleteDC(mHdc1);
}

void CALLBACK    timePassProc(HWND hWnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	//TCHAR msgTop[256];
	//memset(msgTop, 0, sizeof(msgTop));
	//_stprintf_s(msgTop, _countof(msgTop), _T("time + 1"));
	//printfMsg(msgTop, mtLog);

	timeNow += 1;
	if (timeNow >= 60 *24)//24小时轮回

	{
		timeNow = 0;
	}

	TCHAR msgTop[256];
	memset(msgTop, 0, sizeof(msgTop));
	_stprintf_s(msgTop, _countof(msgTop), _T("timeNow:%llu"), timeNow);
	printfMsg(msgTop, mtLog);

	InvalidateRect(hWnd, NULL, false);//每秒更新一次
	UpdateWindow(hWnd);
}

void CALLBACK	personMoveProc(HWND hWnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	TCHAR msgTop[256];
	memset(msgTop, 0, sizeof(msgTop));
	_stprintf_s(msgTop, _countof(msgTop), _T("animate"));
	printfMsg(msgTop, mtLog);

	perCoor.X += 1;

	InvalidateRect(hWnd, NULL, false);
	SendMessage(hWnd, WM_PAINT, MAKELONG(1, 2), NULL);
}

void CALLBACK	updateScreen(HWND hWnd, UINT message, UINT iTimerID, DWORD dwTime)
{

}
// Game0826.cpp : ����Ӧ�ó������ڵ㡣
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

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
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
#define STARTTIME			(6 * 60)						///��λ����
#define LIGHTLIMIT			0								///����
#define DARKLIMIT			210								///�
#define FRAMERATE			50								///ÿ��֡��

enum msgType						//��Ϣ����
{
	mtLog,
	mtWarning,
	mtError,
};
HANDLE hConsole;					//����̨���

ULONG64 timeNow;					//��ǰʱ�䣬��λ����
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

void initByZMX(void);				//����̨���ڳ�ʼ��
void printfMsg(TCHAR *, msgType);	//�����־

void drawImage(HWND, HDC, int);	//��ͼ
void drawImagePerson(HWND, HDC);	//��ͼ
void drawImageWithCoorAndAnchor(HWND, HDC, Gdiplus::Image *, Gdiplus::Point, Gdiplus::PointF);
void kingOfDraw(HWND, HDC, ULONG64);//�ܻ�ͼ


void CALLBACK    timePassProc(HWND, UINT, UINT, DWORD);//��ʱ���ص�
void CALLBACK	personMoveProc(HWND, UINT, UINT, DWORD);
void CALLBACK	updateScreen(HWND hWnd, UINT message, UINT iTimerID, DWORD dwTime);

/*
	��ں���
*/
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˷��ô��롣

	initByZMX();
	printfMsg(_T("��ɫ���ԣ���"), mtLog);
	printfMsg(_T("��ɫ���ԣ���"), mtWarning);
	printfMsg(_T("��ɫ���ԣ���"), mtError);
	printfMsg(_T("/*-----------------*/"), mtLog);
	printfMsg(_T("/* designed by zmx */"), mtLog);
	printfMsg(_T("/*-----------------*/"), mtLog);


    // ��ʼ��ȫ���ַ���
    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	_stprintf_s(szTitle, _countof(szTitle), _T("����Ȼ"));
    //LoadStringW(hInstance, IDC_GAME0826, szWindowClass, MAX_LOADSTRING);
	_stprintf_s(szWindowClass, _countof(szWindowClass), _T("����Ȼ"));
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAME0826));

    MSG msg;

    // ����Ϣѭ��: 
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
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
	wcex.lpszClassName	= szWindowClass;// _T("����Ȼ");
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
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
            // �����˵�ѡ��: 
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
            // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...

			////˫����
			//RECT rect;
			//GetClientRect(hWnd, &rect);//�ͻ����ߴ�
			//SIZE ClientSize;
			//ClientSize.cx = rect.right - rect.left;
			//ClientSize.cy = rect.bottom - rect.top;
			//HDC mHdc		= CreateCompatibleDC(hdc);//�����ڴ�DC����ʼֻ��1x1���ش�С
			//HBITMAP hBmp	= CreateCompatibleBitmap(hdc, ClientSize.cx, ClientSize.cy);//�����������
			//SelectObject(mHdc, hBmp);

			//��ͼ
			//kingOfDraw(hWnd, hdc, timeNow);

			////˫����
			//BitBlt(hdc, 0, 0, ClientSize.cx, ClientSize.cy, mHdc, 0, 0, SRCCOPY);

			//Gdiplus::Graphics graph(hdc);
			//gpSolidBrush pciBrush(gpColor(255, 0, 0, 0));
			//graph.FillRectangle(&pciBrush, 0, 0, 60, 150);

			//gpBitmap image3(_T("D:\\PSIMAGE\\map.png"));
			////bitmap����
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

// �����ڡ������Ϣ�������
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
//���ֳ�ʼ��
void initByZMX(void)
{
	//����̨���ڳ�ʼ��
	AllocConsole();
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//����̨��һֱ���ֵ��������٣����ԾͲ�free��
	///FreeConsole();

	//gdi+��ʼ��
	ULONG_PTR gdipToken;
	GdiplusStartupInput gdipInput;
	GdiplusStartup(&gdipToken, &gdipInput, NULL);

	//gdi+��һֱ�������������٣����ԾͲ�free��
	///GdiplusShutdown(gdipToken);

	//��ʼʱ��
	timeNow = STARTTIME;

	isOkToDraw = false;


	//ȫ�ֱ�����ʼ��
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
//��ӡ��־
void printfMsg(TCHAR *msg, msgType mt)
{
	//���������
	_CONSOLE_SCREEN_BUFFER_INFO lpc;
	GetConsoleScreenBufferInfo(hConsole, &lpc);
	WORD oldTextAttr = lpc.wAttributes;

	//����
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

	//������ɫ
	switch (mt)
	{
	case mtLog:
	{
		//��
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	}
	case mtWarning:
	{
		//��
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED| FOREGROUND_GREEN);

		break;
	}
	case mtError:
	{
		//��
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);

		break;
	}
	default:
		break;
	}

	//���
	WriteConsole(hConsole, msg, wcslen(msg), NULL, NULL);
	//����
	WriteConsole(hConsole, _T("\n"), 1, NULL, NULL);
	//����֮ǰ������
	SetConsoleTextAttribute(hConsole, oldTextAttr);

	return;
}
//��ͼ
void drawImage(HWND hWnd, HDC hdc, int alphaVal)
{
	if ((mapImage)->GetLastStatus() != Status::Ok)
	{
		printfMsg(_T("ͼƬ��ʧ��"), mtError);

		return;
	}
	Gdiplus::Graphics graph(hdc);
	graph.DrawImage(mapImage, destRect, 0, 0, mapImage->GetWidth(), mapImage->GetHeight(), Gdiplus::UnitPixel);

	//����·�������
	Gdiplus::GraphicsPath	path;
	Gdiplus::SolidBrush		maskBrush(Gdiplus::Color(alphaVal, 0, 0, 0));
	path.AddLine(0, 0, WINDOWWIDTH, 0);
	path.AddLine(WINDOWWIDTH, 0, WINDOWWIDTH, WINDOWHEIGHT);
	path.AddLine(WINDOWWIDTH, WINDOWHEIGHT, 0, WINDOWHEIGHT);
	path.AddLine(0, WINDOWHEIGHT, 0, 0);
	graph.FillPath(&maskBrush, &path);



	////��ʼ�滭
	//Graphics graph(hdc);
	////graph.DrawImage(&img, 0, 0, img.GetWidth(), img.GetHeight());


	//////����
	////Pen p(Color(255, 0, 0, 255));
	////graph.DrawLine(&p, 0, 0, 200, 100);

	//////����
	////SolidBrush		brush(Color(255, 0, 0, 255));
	////FontFamily		fontFamily(_T("Times New Roman"));
	////Gdiplus::Font	font(&fontFamily, 24, FontStyleRegular, UnitPixel);
	////PointF			pointF(10.0f, 20.0f);
	////graph.DrawString(_T("����"), -1, &font, pointF, &brush);


	return;
}
//������
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

	//����ê���ͼ
	drawImageWithCoorAndAnchor(hWnd, mHdc, pImgMoveDown[0], perCoor, anchor);

	//��ʼ����
	SetTimer(hWnd, 2, 300, (TIMERPROC)personMoveProc);

	return;
}
void drawImageWithCoorAndAnchor(HWND hWnd, HDC hdc, Gdiplus::Image *img, Gdiplus::Point coor, Gdiplus::PointF anchor)
{
	//ê�㣬���Ͻ�Ϊ(0, 0),���½�Ϊ(1, 1)
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
	//˫���壬�����ڴ�dc
	RECT rect;
	GetClientRect(hWnd, &rect);//�ͻ����ߴ�
	SIZE ClientSize;
	ClientSize.cx	= rect.right - rect.left;
	ClientSize.cy	= rect.bottom - rect.top;
	HDC mHdc1		= CreateCompatibleDC(devHdc);//�����ڴ�DC����ʼֻ��1x1���ش�С
	HBITMAP hBmp	= CreateCompatibleBitmap(devHdc, ClientSize.cx, ClientSize.cy);//�����������
	SelectObject(mHdc1, hBmp);

	//����alphaֵ
	ULONG64 imageAlpha;		//alphaֵ
	switch (time / 60)
	{
	case 4:
	case 5:
	case 6:
	case 7:
		{
			//�������ĵ����˵�
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
			//�������˵���ʮ����
			imageAlpha = LIGHTLIMIT;
		}
		break;
	case 16:
	case 17:
	case 18:
	case 19:
		{
			//������ʮ��������ʮ��
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
			//�����ʮ�����ĵ�
			imageAlpha = DARKLIMIT;
		}
		break;

	default:
		imageAlpha = 0;
		break;
	}

	//������
	drawImage(hWnd, mHdc1, imageAlpha);

	//������
	drawImagePerson(hWnd, mHdc1);

	
	//˫����,���ڴ�dc�����豸dc
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
	if (timeNow >= 60 *24)//24Сʱ�ֻ�

	{
		timeNow = 0;
	}

	TCHAR msgTop[256];
	memset(msgTop, 0, sizeof(msgTop));
	_stprintf_s(msgTop, _countof(msgTop), _T("timeNow:%llu"), timeNow);
	printfMsg(msgTop, mtLog);

	InvalidateRect(hWnd, NULL, false);//ÿ�����һ��
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
// Game0828.cpp : ����Ӧ�ó������ڵ㡣
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

/*����̨*/
enum msgType                      //��Ϣ����
{
	mtLog,
	mtWarning,
	mtError,
};
HANDLE hConsole;                  //����̨���

void initByZMX(void);             //����̨���ڳ�ʼ��
void printfMsg(TCHAR *, msgType); //�����־

/*����*/
std::list<spriteBase> drawImageList;


/*ȫ��*/
void initZMX(void);
void updateDraw(HWND hWnd, HDC hdc);
void drawImageWithCoorAndAnchor(HWND, HDC, Gdiplus::Image *, Gdiplus::PointF, Gdiplus::PointF);



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

void CALLBACK showImage(HWND, UINT, UINT, DWORD);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˷��ô��롣
	initZMX();

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GAME0828, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAME0828));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAME0828));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GAME0828);
    wcex.lpszClassName  = szWindowClass;
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
	//	//����һ����Ҳ��1������
	//	WriteConsole(hConsole, _T("hello\n"), 6, NULL, NULL);
	//	WriteConsole(hConsole, _T("123\n"), 4, NULL, NULL);
	//	WriteConsole(hConsole, _T("��\n"), 2, NULL, NULL);
	//	
	//	TCHAR msg[256];
	//	memset(msg, 0, sizeof(msg));
	//	_stprintf_s(msg, _countof(msg), _T("%s\n"), _T("��"));
	//	WriteConsole(hConsole, msg, wcslen(msg), NULL, NULL);
	//}
	//FreeConsole();
	return;
}
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
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);

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

void CALLBACK showImage(HWND hWnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	//modifyValue();

	InvalidateRect(hWnd, NULL, false);
	SendMessage(hWnd, WM_PAINT, MAKELONG(1, 0), 1);

	//MessageBox(hWnd, _T("callback"), _T(""), MB_OK);
}


void initZMX(void)
{
	//����̨
	initByZMX();

	//gdi+��ʼ��
	ULONG_PTR gdipToken;
	Gdiplus::GdiplusStartupInput gdipInput;
	Gdiplus::GdiplusStartup(&gdipToken, &gdipInput, NULL);

	initList();
}

void updateDraw(HWND hWnd, HDC hdc)
{
	//MessageBox(hWnd, _T("draw"), _T(""), MB_OK);


	//˫���壬�����ڴ�dc
	RECT rect;
	GetClientRect(hWnd, &rect);//��ȡ�ͻ����ߴ�
	SIZE ClientSize;
	ClientSize.cx = rect.right - rect.left;
	ClientSize.cy = rect.bottom - rect.top;
	HDC mHdc1 = CreateCompatibleDC(hdc);//�����ڴ�DC����ʼֻ��1x1���ش�С
	HBITMAP hBmp = CreateCompatibleBitmap(hdc, ClientSize.cx, ClientSize.cy);//�����������
	SelectObject(mHdc1, hBmp);



	HDC  mHdc = mHdc1;
	//Gdiplus::Graphics	graph(mHdc);
	//Gdiplus::Image		image(_T("D:\\PSIMAGE\\map.png"));
	//graph.DrawImage(&image, 0, 0);

	//����
	drawImageWithList(hWnd, mHdc);

	//˫����,���ڴ�dc�����豸dc
	BitBlt(hdc, 0, 0, ClientSize.cx, ClientSize.cy, mHdc1, 0, 0, SRCCOPY);
	DeleteDC(mHdc1);
}
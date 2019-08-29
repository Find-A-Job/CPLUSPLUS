* 需要包含的头文件有
```
// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
```
* 声明
```
enum msgType                      //消息类型
{
	mtLog,
	mtWarning,
	mtError,
};
HANDLE hConsole;                  //控制台句柄

void initByZMX(void);             //控制台窗口初始化
void printfMsg(TCHAR *, msgType); //输出日志
```

* 定义
```
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
```

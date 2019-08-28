# 记录一些经常用到的技术

### 函数
* GetClientRect 
* CreateCompatibleDC
* CreateCompatibleBitmap
* SelectObject
* 

### 宏
* MAKELONG(a, b)，制作32位数值
* \_countof(),数组长度

### 其他
* 双缓冲
```
HDC hdc;//设备dc

//双缓冲，创建内存dc
RECT rect;
GetClientRect(hWnd, &rect);//获取客户区尺寸
SIZE ClientSize;
ClientSize.cx	= rect.right - rect.left;
ClientSize.cy	= rect.bottom - rect.top;
HDC mHdc1       = CreateCompatibleDC(hdc);//创建内存DC，初始只有1x1像素大小
HBITMAP hBmp	= CreateCompatibleBitmap(hdc, ClientSize.cx, ClientSize.cy);//扩大这个区域
SelectObject(mHdc1, hBmp);

//双缓冲,将内存dc拷至设备dc
BitBlt(hdc, 0, 0, ClientSize.cx, ClientSize.cy, mHdc1, 0, 0, SRCCOPY);
DeleteDC(mHdc1);
```

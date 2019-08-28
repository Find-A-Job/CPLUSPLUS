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
* 重绘
```
有三个相关函数
-- Invalidate() 
-- RedrawWindow() 
-- UpdateWindow()
或者是直接SendMessage(hWnd, WM_PAINT, NULL, NULL);

使用Invalidate() + UpdateWindow() 等同于 RedrawWindow()。
如果直接使用sendmessage，客户区并不会刷新，因为没有设置无效区域。
这时候既想要给wm_paint发送带参数的消息，又想刷新客户区怎么办？
	InvalidateRect(hWnd, NULL, false);
	SendMessage(hWnd, WM_PAINT, MAKELONG(1, 2), NULL);
这样，既指定了参数，又能刷新客户区。要注意先后顺序，先指定无效区域，再发送消息，才会使两条消息合并成为一条消息。
参考自：https://www.cnblogs.com/zhoug2020/p/6264668.html
```

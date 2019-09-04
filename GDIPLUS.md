## 常见错误
* 命名空间冲突<br>
xxx不明确，xxx重复定义<br>
使用某个命名空间时，有三种用法<br>
1.using namespace Gdiplus;<br>
2.using Gdiplus::XXX;<br>
3.Gdiplus::XXX xxx;<br>
用第三种方法可以解决此类问题<br>

* 不可修改的值
```
Gdiplus::Image
Gdiplus::Bitmap
初始化完成之后不可修改（OC里面也有个image创建完后不能修改）。
代替的是，用指针对其引用。
但是不能直接用&取址，需要使用其成员函数.clone()来引用其副本(这里表达的可能有问题，不知道怎么表述)。--删掉这行

//直接赋值会报错,也就是private成员变量无法访问
//"Gdiplus::Bitmap &Gdiplus::Bitmap::operator=(const Gdiplus::Bitmap &)" (已声明 所在行数:639，所属文件:"c:\Program Files (x86)\Windows Kits\8.1\Include\um\gdiplusheaders.h") 不可访问
Gdiplus::Bitmap bp(_T("xxx");
Gdiplus::Bitmap bp2=bp;


//这个情况下可以用
Gdiplus::Bitmap bp(_T("xxx");
Gdiplus::Bitmap *pbp=&bp;
Gdiplus::Graphics graph(hdc);//从其他渠道获取的hdc，没问题
graph.DrawImage(pbp, 0, 0);

//这种情况下不行
Gdiplus::Bitmap *pbp;//在文件顶部，设定为全局变量
...
Gdiplus::Bitmap bp(_T("xxx");//某个函数内
pbp=&bp;
...
Gdiplus::Graphics graph(hdc);//从其他渠道获取的hdc，没问题
graph.DrawImage(pbp, 0, 0);

//这种情况下可以
Gdiplus::Bitmap *pbp;//在文件顶部，设定为全局变量
...
Gdiplus::Bitmap bp(_T("xxx");//某个函数内
pbp=bp.Clone(0, 0, bp.GetWidth(), bp.GetHeight(), bp.GetPixelFormat());
...
Gdiplus::Graphics graph(hdc);//从其他渠道获取的hdc，没问题
graph.DrawImage(pbp, 0, 0);

综上，这种类型的xxx,在其构造阶段就需要确定其值，过后就不可修改，只能使用指针方式来引用。
且其构造函数创建的变量是局部变量,而clone出来的是全局变量(估计从栈中分配了内存，可能需要释放).
(以上都是分析出来的，没啥官方依据)
```
* 修改alpha值，碰到的问题
```
//绘制一个黑色背景
Gdiplus::Graphics graph(hdc);//从WM_PAINT获取的hdc
Gdiplus::SolidBrush pciBrush(Gdiplus::Color(255, 0, 0, 0));
graph.FillRectangle(&pciBrush, 0, 0, 60, 150);

Gdiplus::Bitmap image3(_T("xxx"));//图片是png格式
Gdiplus::ColorMatrix colorMatrix = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				                             0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				                             0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				                             0.0f, 0.0f, 0.0f, 0.2f, 0.0f,
				                             0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
Gdiplus::ImageAttributes imageAttr;
imageAttr.SetColorMatrix(&colorMatrix, Gdiplus::ColorMatrixFlagsDefault, Gdiplus::ColorAdjustTypeBitmap);

Gdiplus::RectF rtf(0, 0, image3.GetWidth(), image3.GetHeight());
graph.DrawImage(&image3, rtf, 0, 0, image3.GetWidth(), image3.GetHeight(), Gdiplus::UnitPixel, &imageAttr);//

上面的代码，f5调试运行没问题，但是再修改一次那个0.2f(改为0.8f)，就会失效，呈现的效果还是0.2f的效果
一般情况下，先注释掉graph.DrawImage这段然后f5跑一次，再取消注释f5跑一次,就能正常
总结起来就是这么个情况:代码1跑起来，呈现了效果1，这个时候修改代码1的部分数据(修改数据后的代码称为代码2吧)，
跑起来后还是呈现效果1(理论上应该呈现效果2)，跑几次都一样。注释掉代码2中的部分代码,再跑，再退出再取消注释再跑，
这时候呈现了效果2。完
```



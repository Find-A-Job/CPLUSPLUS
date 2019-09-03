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



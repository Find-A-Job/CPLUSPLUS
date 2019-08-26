## 常见错误
* 命名空间冲突<br>
xxx不明确，xxx重复定义<br>
使用某个命名空间时，有三种用法<br>
1.using namespace Gdiplus;<br>
2.using Gdiplus::XXX;<br>
3.Gdiplus::XXX xxx;<br>
用第三种方法可以解决此类问题<br>

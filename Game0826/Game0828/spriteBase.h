#pragma once

#ifndef SPRITEBASE_H
#define SPRITEBASE_H

#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <iostream>
#include <list>

#include <comdef.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

//typedef Gdiplus::Image			sbImage;
//typedef Gdiplus::Image			*pSbImage;
typedef Gdiplus::Graphics			gpGraphics;
typedef Gdiplus::Bitmap				gpBitmap;
typedef Gdiplus::Bitmap				*pGpBitmap;
typedef Gdiplus::PointF				gpPointF;
typedef Gdiplus::RectF				gpRectF;
typedef Gdiplus::SizeF				gpSizeF;
typedef Gdiplus::Color				gpColor;
typedef Gdiplus::SolidBrush			gpSolidBrush;
typedef Gdiplus::ColorMatrix		gpColorMatrix;
typedef Gdiplus::ImageAttributes	gpImageAttr;
typedef INT32						i32Val;
typedef INT32						i32ReturnStatus;

enum returnStatus
{
	rs_OK		=0,
	rs_ERROR	=1,
};

class spriteBase;

//精灵基类
class spriteBase {
public:
	//构造函数
	spriteBase();
	spriteBase(TCHAR *imageName);
	spriteBase(pGpBitmap image);
	spriteBase(gpColor cpc);

	~spriteBase();

public:
	//成员set，get函数
	pGpBitmap		getImage();
	gpPointF		getPosition();
	gpPointF		getAnchor();
	i32Val			getZposition();
	gpColor			getColor();
	gpSizeF			getSize();
	FLOAT			getImageAlpha();
	BYTE			getColorAlpha();

	void			setImage(pGpBitmap image);
	void			setPosition(gpPointF position);
	void			setAnchor(gpPointF anchor);
	void			setZposition(i32Val zPosition);
	void			setColor(gpColor gpc);
	void			setSize(gpSizeF size);
	void			setImageAlpha(float alpha);
	void			setColorAlpha(BYTE alpha);


public:
	//子节点
	i32ReturnStatus addChild(spriteBase *sb);
	i32ReturnStatus removeChild(spriteBase *sb);
	i32ReturnStatus removeAllChild(void);

public:
	//other
	spriteBase*		clone();
	bool			isPureColorImage();
	bool			shouldItDraw();				//是否需要被绘制
	void			printMsg();

public:
	//操作符重载
	bool			operator () (spriteBase *sb1, spriteBase *sb2);

private:
	//成员变量
	pGpBitmap		m_pSptite;			//
	gpPointF		m_fAnchor;			//锚点
	gpPointF		m_fPosition;		//坐标
	i32Val			m_iZposition;		//z序
	bool			m_isHidden;			//是否隐藏
	float			m_fImageAlpha;		//图片不透明度


	bool			m_isPureColor;		//是否为纯色image
	gpColor			m_color;			//颜色，只有isPureColor为true时才有用
	gpSizeF			m_fSize;
	BYTE			m_byteColorAlpha;	//纯色image的不透明度

protected:

};

//全局函数
void initList(void);
void drawImageWithList(HWND, HDC);		//准确的应该是shouldItBeDrawn
void addChildToRoot(spriteBase *sb);
void modifyValue();

//
gpPointF	gpMakePointF(float a, float b);
gpSizeF		gpMakeSizeF(float a, float b);
gpRectF		gpMakeRectF(float x, float y, float width, float height);



#endif
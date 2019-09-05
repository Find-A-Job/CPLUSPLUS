#pragma once

#ifndef SPRITEBASE_H
#define SPRITEBASE_H

#include <windows.h>

// C ����ʱͷ�ļ�
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
typedef Gdiplus::PointF				gpPoint;
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

//�������
class spriteBase {
public:
	//���캯��
	spriteBase();
	spriteBase(TCHAR *imageName);
	spriteBase(pGpBitmap image);
	spriteBase(gpColor cpc);

	~spriteBase();

public:
	//��Աset��get����
	pGpBitmap		getImage();
	gpPoint			getPosition();
	gpPoint			getAnchor();
	i32Val			getZposition();
	gpColor			getColor();
	gpSizeF			getSize();
	float			getAlpha();

	void			setImage(pGpBitmap image);
	void			setPosition(gpPoint position);
	void			setAnchor(gpPoint anchor);
	void			setZposition(i32Val zPosition);
	void			setColor(gpColor gpc);
	void			setSize(gpSizeF size);
	void			setAlpha(float alpha);


public:
	//�ӽڵ�
	i32ReturnStatus addChild(spriteBase *sb);
	i32ReturnStatus removeChild(spriteBase *sb);
	i32ReturnStatus removeAllChild(void);

public:
	//other
	bool			isPureColorImage();
	bool			shouldItDraw();				//�Ƿ���Ҫ������

public:
	//����������
	bool			operator () (const spriteBase sb1, const spriteBase sb2);

private:
	//��Ա����
	pGpBitmap		m_pSptite;			//
	gpPoint			m_fAnchor;			//ê��
	gpPoint			m_fPosition;		//����
	i32Val			m_iZposition;		//z��
	bool			m_isHidden;			//�Ƿ�����
	float			m_fImageAlpha;		//ͼƬ��͸����


	bool			m_isPureColor;		//�Ƿ�Ϊ��ɫimage
	gpColor			m_color;			//��ɫ��ֻ��isPureColorΪtrueʱ������
	gpSizeF			m_fSize;
	BYTE			m_byteColorAlpha;	//��ɫimage�Ĳ�͸����

protected:

};

//ȫ�ֺ���
void initList(void);
void drawImageWithList(HWND, HDC);		//׼ȷ��Ӧ����shouldItBeDrawn

//
gpPoint		gpMakePointF(float a, float b);
gpSizeF		gpMakeSizeF(float a, float b);
gpRectF		gpMakeRectF(float x, float y, float width, float height);



#endif
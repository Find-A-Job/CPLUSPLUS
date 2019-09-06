
#include "stdafx.h"
#include "spriteBase.h"


//
// 绘图链表
// 按z序排列
//
static std::list<spriteBase> drawSpriteList;


//
// 构造函数
//
//
spriteBase::spriteBase()
{
	m_pSptite		= nullptr;
	m_fAnchor		= gpMakePointF(0, 0);	//锚点
	m_fPosition		= gpMakePointF(0, 0);	//坐标
	m_iZposition	= 1;					//z序
	m_isHidden		= false;				//是否隐藏
	m_fImageAlpha	= 1.0f;					//图片的不透明度


	m_isPureColor	= true;					//是否为纯色图
	m_color			= gpColor(255, 0, 0, 0);//颜色
	m_fSize			= gpMakeSizeF(0, 0);	//纯色image尺寸
	m_byteColorAlpha = m_color.GetAlpha();	//纯色image的不透明度
}
spriteBase::spriteBase(TCHAR *imageName)
{
	gpBitmap gpb(imageName);
	m_pSptite		= gpb.Clone(0, 0, gpb.GetWidth(), gpb.GetHeight(), gpb.GetPixelFormat());
	m_fAnchor		= gpMakePointF(0, 0);	//锚点
	m_fPosition		= gpMakePointF(0, 0);	//坐标
	m_iZposition	= 1;					//z序
	m_isHidden		= false;				//是否隐藏
	m_fImageAlpha	= 1.0f;					//图片的不透明度


	m_isPureColor	= false;				//是否为纯色图
	m_color			= gpColor(0, 0, 0, 0);	//颜色
	m_fSize			= gpMakeSizeF(0, 0);	//纯色image尺寸
	m_byteColorAlpha = m_color.GetAlpha();//纯色image的不透明度
}
spriteBase::spriteBase(pGpBitmap image)
{
	m_pSptite		= image;
	m_fAnchor		= gpMakePointF(0, 0);	//锚点
	m_fPosition		= gpMakePointF(0, 0);	//坐标
	m_iZposition	= 1;					//z序
	m_isHidden		= false;				//是否隐藏
	m_fImageAlpha	= 1.0f;					//图片的不透明度


	m_isPureColor	= false;				//是否为纯色图
	m_color			= gpColor(0, 0, 0, 0);	//颜色
	m_fSize			= gpMakeSizeF(0, 0);	//纯色image尺寸
	m_byteColorAlpha = m_color.GetAlpha();//纯色image的不透明度
}
spriteBase::spriteBase(gpColor cpc)
{
	m_pSptite		= nullptr;
	m_fAnchor		= gpMakePointF(0, 0);	//锚点
	m_fPosition		= gpMakePointF(0, 0);	//坐标
	m_iZposition	= 1;					//z序
	m_isHidden		= false;				//是否隐藏
	m_fImageAlpha	= 0.0f;					//图片的不透明度


	m_isPureColor	= true;					//是否为纯色图
	m_color			= cpc;					//颜色
	m_fSize			= gpMakeSizeF(0, 0);	//纯色image尺寸
	m_byteColorAlpha = m_color.GetAlpha();//纯色image的不透明度
}

spriteBase::~spriteBase()
{
	;
}


//
// 成员变量操作函数
//
//
void	spriteBase::setImage(pGpBitmap image)
{
	this->m_isPureColor = false;
	this->m_pSptite=image;
}
void	spriteBase::setPosition(gpPoint position)
{
	this->m_fPosition = position;
}
void	spriteBase::setAnchor(gpPoint anchor)
{
	this->m_fAnchor = anchor;
}
void	spriteBase::setZposition(i32Val zPosition)
{
	this->m_iZposition = zPosition;
}
void	spriteBase::setColor(gpColor gpc)
{
	this->m_color = gpc;
}
void	spriteBase::setSize(gpSizeF size)
{
	this->m_fSize = size;
}
void	spriteBase::setImageAlpha(float alpha)
{
	this->m_fImageAlpha = alpha;
}
void	spriteBase::setColorAlpha(BYTE alpha)
{
	this->m_byteColorAlpha = alpha;
	this->m_color = gpColor(alpha, m_color.GetRed(), m_color.GetGreen(), m_color.GetBlue());
}



pGpBitmap	spriteBase::getImage()
{
	return this->m_pSptite;
}
gpPoint	spriteBase::getPosition()
{
	return this->m_fPosition;
}
gpPoint	spriteBase::getAnchor()
{
	return this->m_fAnchor;
}
i32Val	spriteBase::getZposition()
{
	return this->m_iZposition;
}
gpColor	spriteBase::getColor()
{
	return this->m_color;
}
gpSizeF spriteBase::getSize()
{
	return this->m_fSize;
}
FLOAT	spriteBase::getImageAlpha()
{
	return this->m_fImageAlpha;
}
BYTE	spriteBase::getColorAlpha()
{
	return this->m_byteColorAlpha;
}

//
// 节点
//
//
i32ReturnStatus spriteBase::addChild(spriteBase *sb)
{
	//子节点的z序以父节点为基础进行偏移
	sb->m_iZposition += this->m_iZposition;

	//子节点的x, y轴坐标以父节点为基础进行偏移
	gpPoint superCoor = gpMakePointF(this->getPosition().X + this->getAnchor().X * this->getImage()->GetWidth(),
									this->getPosition().Y + this->getAnchor().Y * this->getImage()->GetHeight());
	gpPoint childCoor = sb->getPosition();
	sb->setPosition(gpMakePointF(superCoor.X + childCoor.X, superCoor.Y + childCoor.Y));

	//加入链表
	drawSpriteList.push_back(*sb);

	//需要对节点进行排序
	drawSpriteList.sort(spriteBase());



	return rs_OK;
}

//其他
bool	spriteBase::isPureColorImage()
{
	return this->m_isPureColor;
}
bool	spriteBase::shouldItDraw()
{
	//先判断是否隐藏
	if (m_isHidden == true)
	{
		return false;
	}

	//判断是否为纯色image
	if (m_isPureColor != false)
	{
		//判断是否为零长度宽，或者零长度高
		if (m_fSize.Width == 0 || m_fSize.Height == 0)
		{
			return false;
		}

		//判断alpha值是否为0
		if (m_color.GetAlpha() == (BYTE)0)
		{
			return false;
		} 
		
		return true;
	}

	if (m_pSptite != nullptr)
	{
		if (m_isHidden == true)
		{
			return false;
		}
		
		return true;
	}
	else
	{
		return false;
	}


	return true;
}

//
// 操作符重载
//
//
bool spriteBase::operator () (const spriteBase sb1, const spriteBase sb2)
{
	if (sb1.m_iZposition > sb2.m_iZposition)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//
// 全局函数
//
//
void initList(void)
{
	spriteBase sb;
	gpPoint	fpa(0.5, 1);
	gpPoint	fpp(100, 100);
	gpBitmap image(_T("D:\\PSIMAGE\\man_walkWithBackgroundcolor\\moveDown0.png"));
	sb.setAnchor(fpa);
	sb.setPosition(fpp);
	sb.setImage(image.Clone(0, 0, image.GetWidth(), image.GetHeight(), image.GetPixelFormat()));
	sb.setZposition(2);
	drawSpriteList.push_back(sb);

	
	spriteBase sb2;
	gpPoint	fpa2(0.5, 1);
	gpPoint	fpp2(200, 100);
	gpBitmap image2(_T("D:\\PSIMAGE\\man_walkWithBackgroundcolor\\moveDown1.png"));
	sb2.setAnchor(fpa2);
	sb2.setPosition(fpp2);
	sb2.setImage(image2.Clone(0, 0, image2.GetWidth(), image2.GetHeight(), image2.GetPixelFormat()));
	sb2.setZposition(3);
	drawSpriteList.push_back(sb2);
	
	spriteBase sb3;
	gpPoint	fpa3(0, 0);
	gpPoint	fpp3(0, 0);
	gpBitmap image3(_T("D:\\PSIMAGE\\map.png"));
	sb3.setAnchor(fpa3);
	sb3.setPosition(fpp3);
	sb3.setImage(image3.Clone(0, 0, image3.GetWidth(), image3.GetHeight(), image3.GetPixelFormat()));
	sb3.setZposition(1);
	drawSpriteList.push_back(sb3);

	//
	spriteBase sb4(_T("D:\\PSIMAGE\\man_walkWithBackgroundcolor\\moveDown2.png"));
	sb4.setAnchor(gpMakePointF(0.5, 1));
	sb4.setPosition(gpMakePointF(300, 100));
	sb4.setZposition(2);
	drawSpriteList.push_back(sb4);

	//
	spriteBase sb5(gpColor(0, 1, 1, 1));
	sb5.setAnchor(gpMakePointF(0, 0));
	sb5.setPosition(gpMakePointF(0, 0));
	sb5.setSize(gpMakeSizeF(1000, 1000));
	sb5.setZposition(100);
	drawSpriteList.push_back(sb5);


	//TCHAR msgTop[256];
	//memset(msgTop, 0, sizeof(msgTop));
	//_stprintf_s(msgTop, _countof(msgTop), _T("%d"), li.back().getVal1());
	//MessageBox(NULL, msgTop, _T(""), MB_OK);

	//按z序排列
	drawSpriteList.sort(spriteBase());

	//清空链表
	//drawSpriteList.clear();
}
void drawImageWithList(HWND hWnd, HDC hdc)
{
	pGpBitmap image;
	double anchorX		= 0;
	double anchorY		= 0;
	double spriteWidth	= 0;
	double spriteHeight = 0;
	double coorX		= 0;
	double coorY		= 0;
	float	imageAlpha	= 0;

	std::list<spriteBase> lsb;	//需要绘制的凑成一个链表
	lsb.clear();

	//锚点，左上角为(0, 0),右下角为(1, 1)
	Gdiplus::Graphics graph(hdc);

	//筛选出所有可视精灵，并按z序排列
	if (drawSpriteList.empty() != true)
	{
		for (std::list<spriteBase>::iterator bg = drawSpriteList.begin(); bg != drawSpriteList.end(); bg++)
		{
			//判断是否为隐藏,或者alpha值为0,
			if (bg->shouldItDraw() == false)
			{
				//不需要绘制
				continue;
			}
			else
			{
				spriteBase sb(*bg);
				lsb.push_back(sb);
			}
		}
	}
	

	//绘制所有
	while (lsb.empty() != true)
	{
		//取出顺序很重要，z序小的先取出，
		spriteBase sb = lsb.back();

		//纯色image和普通image的绘制方式不同,
		if (sb.isPureColorImage())
		{
			gpSolidBrush pciBrush(sb.getColor());
			graph.FillRectangle(&pciBrush, sb.getPosition().X, sb.getPosition().Y, sb.getSize().Width, sb.getSize().Height);
		}
		else
		{
			//属性
			image			= sb.getImage();
			anchorX			= sb.getAnchor().X;
			anchorY			= sb.getAnchor().Y;
			spriteWidth		= image->GetWidth();
			spriteHeight	= image->GetHeight();
			coorX			= sb.getPosition().X;
			coorY			= sb.getPosition().Y;
			imageAlpha		= sb.getImageAlpha();

			//bitmap属性
			gpColorMatrix colorMatrix = {   1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
											0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
											0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
											0.0f, 0.0f, 0.0f, imageAlpha, 0.0f,
											0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
			gpImageAttr imageAttr;
			imageAttr.SetColorMatrix(&colorMatrix, Gdiplus::ColorMatrixFlagsDefault, Gdiplus::ColorAdjustTypeBitmap);

			//绘制
			gpRectF rect(coorX - (float)(anchorX * spriteWidth), coorY - (float)(anchorY * spriteHeight), spriteWidth, spriteHeight);
			graph.DrawImage(image, rect, 0, 0, spriteWidth, spriteHeight, Gdiplus::UnitPixel, &imageAttr);
		}


		//用完后数据弹出链表
		lsb.pop_back();
	}

}

void modifyValue()
{
	TCHAR msg[256];
	memset(msg, 0, sizeof(msg));
	_stprintf_s(msg, _countof(msg), _T("%lu"), drawSpriteList.size());
	
	MessageBox(NULL, msg, _T(""), MB_OK);

	MessageBox(NULL, _T(""), _T(""), MB_OK);
	if (drawSpriteList.front().isPureColorImage() == true)
	{
		BYTE oldAlpha = drawSpriteList.front().getColorAlpha();
		BYTE newAlpha = oldAlpha - 1;
		if (newAlpha < 0)
		{
			newAlpha = 0;
		}
		else if (newAlpha > 255)
		{
			newAlpha = 255;
		}
		else
		{
			;
		}
		drawSpriteList.front().setColorAlpha(newAlpha);
	}
	else
	{
		FLOAT oldAlpha = drawSpriteList.front().getImageAlpha();
		FLOAT newAlpha = oldAlpha - 0.01f;
		if (newAlpha < 0)
		{
			newAlpha = 0;
		}
		else if (newAlpha > 1)
		{
			newAlpha = 1;
		}
		else
		{
			;
		}
		drawSpriteList.front().setImageAlpha(newAlpha);
	}

}


//
gpPoint gpMakePointF(float a, float b)
{
	gpPoint fp(a, b);
	return fp;
}
gpSizeF	gpMakeSizeF(float a, float b)
{
	gpSizeF fs(a, b);
	return fs;
}
gpRectF gpMakeRectF(float x, float y, float width, float height)
{
	gpRectF fr(x, y, width, height);
	return fr;
}


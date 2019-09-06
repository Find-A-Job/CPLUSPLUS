
#include "stdafx.h"
#include "spriteBase.h"


//
// ��ͼ����
// ��z������
//
static std::list<spriteBase> drawSpriteList;


//
// ���캯��
//
//
spriteBase::spriteBase()
{
	m_pSptite		= nullptr;
	m_fAnchor		= gpMakePointF(0, 0);	//ê��
	m_fPosition		= gpMakePointF(0, 0);	//����
	m_iZposition	= 1;					//z��
	m_isHidden		= false;				//�Ƿ�����
	m_fImageAlpha	= 1.0f;					//ͼƬ�Ĳ�͸����


	m_isPureColor	= true;					//�Ƿ�Ϊ��ɫͼ
	m_color			= gpColor(255, 0, 0, 0);//��ɫ
	m_fSize			= gpMakeSizeF(0, 0);	//��ɫimage�ߴ�
	m_byteColorAlpha = m_color.GetAlpha();	//��ɫimage�Ĳ�͸����
}
spriteBase::spriteBase(TCHAR *imageName)
{
	gpBitmap gpb(imageName);
	m_pSptite		= gpb.Clone(0, 0, gpb.GetWidth(), gpb.GetHeight(), gpb.GetPixelFormat());
	m_fAnchor		= gpMakePointF(0, 0);	//ê��
	m_fPosition		= gpMakePointF(0, 0);	//����
	m_iZposition	= 1;					//z��
	m_isHidden		= false;				//�Ƿ�����
	m_fImageAlpha	= 1.0f;					//ͼƬ�Ĳ�͸����


	m_isPureColor	= false;				//�Ƿ�Ϊ��ɫͼ
	m_color			= gpColor(0, 0, 0, 0);	//��ɫ
	m_fSize			= gpMakeSizeF(0, 0);	//��ɫimage�ߴ�
	m_byteColorAlpha = m_color.GetAlpha();//��ɫimage�Ĳ�͸����
}
spriteBase::spriteBase(pGpBitmap image)
{
	m_pSptite		= image;
	m_fAnchor		= gpMakePointF(0, 0);	//ê��
	m_fPosition		= gpMakePointF(0, 0);	//����
	m_iZposition	= 1;					//z��
	m_isHidden		= false;				//�Ƿ�����
	m_fImageAlpha	= 1.0f;					//ͼƬ�Ĳ�͸����


	m_isPureColor	= false;				//�Ƿ�Ϊ��ɫͼ
	m_color			= gpColor(0, 0, 0, 0);	//��ɫ
	m_fSize			= gpMakeSizeF(0, 0);	//��ɫimage�ߴ�
	m_byteColorAlpha = m_color.GetAlpha();//��ɫimage�Ĳ�͸����
}
spriteBase::spriteBase(gpColor cpc)
{
	m_pSptite		= nullptr;
	m_fAnchor		= gpMakePointF(0, 0);	//ê��
	m_fPosition		= gpMakePointF(0, 0);	//����
	m_iZposition	= 1;					//z��
	m_isHidden		= false;				//�Ƿ�����
	m_fImageAlpha	= 0.0f;					//ͼƬ�Ĳ�͸����


	m_isPureColor	= true;					//�Ƿ�Ϊ��ɫͼ
	m_color			= cpc;					//��ɫ
	m_fSize			= gpMakeSizeF(0, 0);	//��ɫimage�ߴ�
	m_byteColorAlpha = m_color.GetAlpha();//��ɫimage�Ĳ�͸����
}

spriteBase::~spriteBase()
{
	;
}


//
// ��Ա������������
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
// �ڵ�
//
//
i32ReturnStatus spriteBase::addChild(spriteBase *sb)
{
	//�ӽڵ��z���Ը��ڵ�Ϊ��������ƫ��
	sb->m_iZposition += this->m_iZposition;

	//�ӽڵ��x, y�������Ը��ڵ�Ϊ��������ƫ��
	gpPoint superCoor = gpMakePointF(this->getPosition().X + this->getAnchor().X * this->getImage()->GetWidth(),
									this->getPosition().Y + this->getAnchor().Y * this->getImage()->GetHeight());
	gpPoint childCoor = sb->getPosition();
	sb->setPosition(gpMakePointF(superCoor.X + childCoor.X, superCoor.Y + childCoor.Y));

	//��������
	drawSpriteList.push_back(*sb);

	//��Ҫ�Խڵ��������
	drawSpriteList.sort(spriteBase());



	return rs_OK;
}

//����
bool	spriteBase::isPureColorImage()
{
	return this->m_isPureColor;
}
bool	spriteBase::shouldItDraw()
{
	//���ж��Ƿ�����
	if (m_isHidden == true)
	{
		return false;
	}

	//�ж��Ƿ�Ϊ��ɫimage
	if (m_isPureColor != false)
	{
		//�ж��Ƿ�Ϊ�㳤�ȿ������㳤�ȸ�
		if (m_fSize.Width == 0 || m_fSize.Height == 0)
		{
			return false;
		}

		//�ж�alphaֵ�Ƿ�Ϊ0
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
// ����������
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
// ȫ�ֺ���
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

	//��z������
	drawSpriteList.sort(spriteBase());

	//�������
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

	std::list<spriteBase> lsb;	//��Ҫ���ƵĴճ�һ������
	lsb.clear();

	//ê�㣬���Ͻ�Ϊ(0, 0),���½�Ϊ(1, 1)
	Gdiplus::Graphics graph(hdc);

	//ɸѡ�����п��Ӿ��飬����z������
	if (drawSpriteList.empty() != true)
	{
		for (std::list<spriteBase>::iterator bg = drawSpriteList.begin(); bg != drawSpriteList.end(); bg++)
		{
			//�ж��Ƿ�Ϊ����,����alphaֵΪ0,
			if (bg->shouldItDraw() == false)
			{
				//����Ҫ����
				continue;
			}
			else
			{
				spriteBase sb(*bg);
				lsb.push_back(sb);
			}
		}
	}
	

	//��������
	while (lsb.empty() != true)
	{
		//ȡ��˳�����Ҫ��z��С����ȡ����
		spriteBase sb = lsb.back();

		//��ɫimage����ͨimage�Ļ��Ʒ�ʽ��ͬ,
		if (sb.isPureColorImage())
		{
			gpSolidBrush pciBrush(sb.getColor());
			graph.FillRectangle(&pciBrush, sb.getPosition().X, sb.getPosition().Y, sb.getSize().Width, sb.getSize().Height);
		}
		else
		{
			//����
			image			= sb.getImage();
			anchorX			= sb.getAnchor().X;
			anchorY			= sb.getAnchor().Y;
			spriteWidth		= image->GetWidth();
			spriteHeight	= image->GetHeight();
			coorX			= sb.getPosition().X;
			coorY			= sb.getPosition().Y;
			imageAlpha		= sb.getImageAlpha();

			//bitmap����
			gpColorMatrix colorMatrix = {   1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
											0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
											0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
											0.0f, 0.0f, 0.0f, imageAlpha, 0.0f,
											0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
			gpImageAttr imageAttr;
			imageAttr.SetColorMatrix(&colorMatrix, Gdiplus::ColorMatrixFlagsDefault, Gdiplus::ColorAdjustTypeBitmap);

			//����
			gpRectF rect(coorX - (float)(anchorX * spriteWidth), coorY - (float)(anchorY * spriteHeight), spriteWidth, spriteHeight);
			graph.DrawImage(image, rect, 0, 0, spriteWidth, spriteHeight, Gdiplus::UnitPixel, &imageAttr);
		}


		//��������ݵ�������
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


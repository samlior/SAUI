#ifdef SA_USE_QRCODE

#include "SAQRCode.h"
#include "SA/SAUtils.h"
#include <qr/qr.h>

using namespace std;
using namespace cocos2d;


int SAQRCode::iQRCodeVersion = 4;
int SAQRCode::iQRCodeECLevel = 2;



bool SAQRCode::init(const string& strText)
{
	CQRCodeCreator creator(iQRCodeVersion, iQRCodeECLevel);
	string strOutPut;
	int iSize = 0;
	creator.createQRCode(strText, strOutPut, iSize, QRCODE_OUTPOUT_PNG);

	Texture2D* pTexture = new Texture2D;
	Image* pImg = new Image;
	pImg->initWithImageData((const unsigned char*)strOutPut.c_str(), iSize);
	pTexture->initWithImage(pImg);

	if (!Sprite::initWithTexture(pTexture))
		return false;
	return true;
}





bool SAQRCodeNode::init(const Size& size)
{
	if (!SANode::init())
		return false;

	setContentSize(size);
	return true;
}

bool SAQRCodeNode::init(const Size& size, const string& strText)
{
	if (!SANode::init())
		return false;

	setContentSize(size);
	reload(strText);
	return true;
}

void SAQRCodeNode::reload(const string& strText)
{
	if (m_pCode)
	{
		m_pCode->removeFromParentAndCleanup(true);
		m_pCode = nullptr;
	}
	m_pCode = SAQRCode::Create::create(strText);
	SAUtils::setNodeSize(getContentSize(), m_pCode);
	m_pCode->setAnchorPoint(Vec2::ZERO);
	m_pCode->setPosition(Vec2::ZERO);
	addChild(m_pCode);
}



bool CQRCodeCreator::createQRCode(const string& strData, string& strOutPut, int& iSize, QRCodeOutPutType type)
{
	bool bRet = false;
	int iErrorCode = QR_ERR_NONE;
	QRCode* p = qrInit(m_iVersion, QR_EM_8BIT, m_iECLevel, -1, &iErrorCode);
	if (p != NULL)
	{
		if (iErrorCode == QR_ERR_NONE)
		{
			qrAddData(p, (const qr_byte_t*)strData.data(), strData.length());
			if (qrFinalize(p))
			{
				iSize = 0;
				qr_byte_t* pBuffer = nullptr;

				switch (type)
				{
				case QRCODE_OUTPOUT_BMP:
					pBuffer = qrSymbolToBMP(p, 5, 5, &iSize);
					break;
				case QRCODE_OUTPOUT_TIFF:
					pBuffer = qrSymbolToTIFF(p, 5, 5, &iSize);
					break;
				case QRCODE_OUTPOUT_PNG:
					pBuffer = qrSymbolToPNG(p, 5, 5, &iSize);
					break;
				default:
					break;
				}

				if (pBuffer)
				{
					strOutPut = string((const char*)pBuffer, iSize);
					bRet = true;
				}
			}
		}
		qrDestroy(p);
	}
	return bRet;
}
#endif
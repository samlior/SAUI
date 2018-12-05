#ifdef SA_USE_QRCODE

#include "QRCodeCreator.h"
#include <qr/qr.h>

using namespace std;



bool CQRCodeCreator::createQRCode(const string& strData, string& strOutPut, int& iSize, enQRCodeOutPutType type)
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
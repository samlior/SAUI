#pragma once

#ifdef SA_USE_QRCODE

#include <string>


enum enQRCodeOutPutType
{
	QRCODE_OUTPOUT_BMP,
	QRCODE_OUTPOUT_TIFF,
	QRCODE_OUTPOUT_PNG
};


class CQRCodeCreator
{
public:
	CQRCodeCreator(int iVersion = 4, int iECLevel = 2) :m_iVersion(iVersion), m_iECLevel(iECLevel) {}
	~CQRCodeCreator() {}

	bool createQRCode(const std::string& strData, std::string& strOutPut, int& iSize, enQRCodeOutPutType type);

private:
	int m_iVersion;
	int m_iECLevel;
};

#endif
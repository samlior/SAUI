#pragma once

#ifdef SA_USE_QRCODE

#include "cocos2d.h"
#include "SA/SACreator.h"
#include "SAUI/SANode.h"

//二维码精灵
class SAQRCode : public cocos2d::Sprite
{
public:
	using Create = SACreator<true, SAQRCode, const std::string&>;

	//设置版本
	static void setVersion(int i) { iQRCodeVersion = i; }
	//设置纠错等级
	static void setECLevel(int i) { iQRCodeECLevel = i; }

	virtual bool init(const std::string& strText);

protected:
	static int iQRCodeVersion;
	static int iQRCodeECLevel;
};


//二维码节点
class SAQRCodeNode : public SANode
{
public:
	SAQRCodeNode() :m_pCode(nullptr) {}

	using Create = SACreator<true, SAQRCodeNode, const cocos2d::Size&>;
	using CreateWithText = SACreator<true, SAQRCodeNode, const cocos2d::Size&, const std::string&>;

	//重新加载二维码
	void reload(const std::string& strText);

	virtual bool init(const cocos2d::Size& size);
	virtual bool init(const cocos2d::Size& size, const std::string& strText);

protected:
	SAQRCode* m_pCode;
};


enum QRCodeOutPutType
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

	bool createQRCode(const std::string& strData, std::string& strOutPut, int& iSize, QRCodeOutPutType type);

private:
	int m_iVersion;
	int m_iECLevel;
};

#endif


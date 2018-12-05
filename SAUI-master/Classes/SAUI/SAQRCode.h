#pragma once

#ifdef SA_USE_QRCODE

#include "cocos2d.h"
#include "SA/SACreator.h"
#include "SAUI/SANode.h"

//��ά�뾫��
class SAQRCode : public cocos2d::Sprite
{
public:
	using Create = SACreator<true, SAQRCode, const std::string&>;

	//���ð汾
	static void setVersion(int i) { iQRCodeVersion = i; }
	//���þ���ȼ�
	static void setECLevel(int i) { iQRCodeECLevel = i; }

	virtual bool init(const std::string& strText);

protected:
	static int iQRCodeVersion;
	static int iQRCodeECLevel;
};


//��ά��ڵ�
class SAQRCodeNode : public SANode
{
public:
	SAQRCodeNode() :m_pCode(nullptr) {}

	using Create = SACreator<true, SAQRCodeNode, const cocos2d::Size&>;
	using CreateWithText = SACreator<true, SAQRCodeNode, const cocos2d::Size&, const std::string&>;

	//���¼��ض�ά��
	void reload(const std::string& strText);

	virtual bool init(const cocos2d::Size& size);
	virtual bool init(const cocos2d::Size& size, const std::string& strText);

protected:
	SAQRCode* m_pCode;
};

#endif


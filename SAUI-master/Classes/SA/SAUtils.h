#pragma once
#include "cocos2d.h"

class SAUtils
{
public:
	//���ýڵ�ĳߴ粢ֱ�ӷ���
	template<typename T>
	static T* setNodeSize(const cocos2d::Size& standardSize, T* pT)
	{
		const cocos2d::Size& sizeNode = pT->getContentSize();
		pT->setScaleX(standardSize.width / sizeNode.width);
		pT->setScaleY(standardSize.height / sizeNode.height);
		return pT;
	}

	//�ж�������Ƿ���node�ڵ��ڲ�
	static bool isContain(cocos2d::Node* pNode, const cocos2d::Vec2& vec2WorldLocation, const cocos2d::Rect* const pRect = nullptr);

	//�����ļ�����,��������
	static cocos2d::Sprite* createWithFileConfig(const std::string& strFileName, int iConfig);
	static cocos2d::Sprite* createWithFileConfigAndSize(const cocos2d::Size& size, const std::string& strFileName, int iConfig);

	//�Ƴ��ӽڵ�
	static void removeChildByTag(cocos2d::Node* pParent, int iTag);
	static void removeChildByTag(cocos2d::Node* pParent, int iTagBegin, int iTagEnd);

	//��ȡ�ڵ��ܳߴ�
	static cocos2d::Size getTotalContentSize(cocos2d::Node* pNode);
};


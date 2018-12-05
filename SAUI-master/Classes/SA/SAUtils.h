#pragma once
#include "cocos2d.h"

class SAUtils
{
public:
	//设置节点的尺寸并直接返回
	template<typename T>
	static T* setNodeSize(const cocos2d::Size& standardSize, T* pT)
	{
		const cocos2d::Size& sizeNode = pT->getContentSize();
		pT->setScaleX(standardSize.width / sizeNode.width);
		pT->setScaleY(standardSize.height / sizeNode.height);
		return pT;
	}

	//判断坐标点是否在node节点内部
	static bool isContain(cocos2d::Node* pNode, const cocos2d::Vec2& vec2WorldLocation, const cocos2d::Rect* const pRect = nullptr);

	//依据文件配置,创建精灵
	static cocos2d::Sprite* createWithFileConfig(const std::string& strFileName, int iConfig);
	static cocos2d::Sprite* createWithFileConfigAndSize(const cocos2d::Size& size, const std::string& strFileName, int iConfig);

	//移除子节点
	static void removeChildByTag(cocos2d::Node* pParent, int iTag);
	static void removeChildByTag(cocos2d::Node* pParent, int iTagBegin, int iTagEnd);

	//获取节点总尺寸
	static cocos2d::Size getTotalContentSize(cocos2d::Node* pNode);
};


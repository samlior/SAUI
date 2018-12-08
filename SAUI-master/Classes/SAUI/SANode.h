#pragma once
#include "cocos2d.h"
#include "SA/SACreator.h"

class SANode : public cocos2d::Node
{
public:
	SANode() :m_bRespondEvent(true) {}

	using Create = SACreator<true, SANode>;

	//设置是否响应事件(深度递归)
	static void setRespondEventDeeply(cocos2d::Node* pNode, bool bRespond);
	virtual bool init() override;

	//设置是否响应事件
	virtual void setRespondEvent(bool bRespondEvent) { m_bRespondEvent = bRespondEvent; }
	virtual bool isRespondEvent() { return m_bRespondEvent; }

	//获取包括所有子节点的尺寸
	cocos2d::Size getTotalContentSize();

	//设置是否可视的同时设置是否相应事件
	virtual void setVisibleAndRespondEvent(bool bVisible);

protected:
	bool m_bRespondEvent;
};


#pragma once
#include "cocos2d.h"
#include "SA/SACreator.h"

class SANode : public cocos2d::Node
{
public:
	SANode();

	using Create = SACreator<true, SANode>;

	//设置是否响应事件(深度递归)
	static void setRespondEventDeeply(cocos2d::Node* pNode, bool bRespond);
	virtual bool init() override;

	//设置是否响应事件
	void setRespondEvent(bool bRespondEvent) { m_bRespondEvent = bRespondEvent; }
	bool isRespondEvent() { return m_bRespondEvent; }

	//设置是否裁剪边缘
	void setClippingToBounds(bool bClippingToBounds) { m_bClippingToBounds = bClippingToBounds; }
	bool isClippingToBounds() { return m_bClippingToBounds; }

	//获取包括所有子节点的尺寸
	cocos2d::Size getTotalContentSize();

	//设置是否可视的同时设置是否相应事件
	virtual void setVisibleAndRespondEvent(bool bVisible);

	//重写visi函数,实现clippingToBounds
	virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;

protected:
	virtual void beforeVisit();
	virtual void afterVisit();
	virtual cocos2d::Rect getSelfRect();

	bool m_bRespondEvent;
	bool m_bRestoreScissor;
	bool m_bClippingToBounds;

	cocos2d::Rect m_rectOriginal;
	cocos2d::CustomCommand m_cmdBeforeVisit;
	cocos2d::CustomCommand m_cmdAfterVisit;
};
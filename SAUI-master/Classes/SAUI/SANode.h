#pragma once
#include "cocos2d.h"
#include "SA/SACreator.h"

class SANode : public cocos2d::Node
{
public:
	SANode();

	using Create = SACreator<true, SANode>;

	//�����Ƿ���Ӧ�¼�(��ȵݹ�)
	static void setRespondEventDeeply(cocos2d::Node* pNode, bool bRespond);
	virtual bool init() override;

	//�����Ƿ���Ӧ�¼�
	void setRespondEvent(bool bRespondEvent) { m_bRespondEvent = bRespondEvent; }
	bool isRespondEvent() { return m_bRespondEvent; }

	//�����Ƿ�ü���Ե
	void setClippingToBounds(bool bClippingToBounds) { m_bClippingToBounds = bClippingToBounds; }
	bool isClippingToBounds() { return m_bClippingToBounds; }

	//��ȡ���������ӽڵ�ĳߴ�
	cocos2d::Size getTotalContentSize();

	//�����Ƿ���ӵ�ͬʱ�����Ƿ���Ӧ�¼�
	virtual void setVisibleAndRespondEvent(bool bVisible);

	//��дvisi����,ʵ��clippingToBounds
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
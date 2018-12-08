#pragma once
#include "cocos2d.h"
#include "SA/SACreator.h"

class SANode : public cocos2d::Node
{
public:
	SANode() :m_bRespondEvent(true) {}

	using Create = SACreator<true, SANode>;

	//�����Ƿ���Ӧ�¼�(��ȵݹ�)
	static void setRespondEventDeeply(cocos2d::Node* pNode, bool bRespond);
	virtual bool init() override;

	//�����Ƿ���Ӧ�¼�
	virtual void setRespondEvent(bool bRespondEvent) { m_bRespondEvent = bRespondEvent; }
	virtual bool isRespondEvent() { return m_bRespondEvent; }

	//��ȡ���������ӽڵ�ĳߴ�
	cocos2d::Size getTotalContentSize();

	//�����Ƿ���ӵ�ͬʱ�����Ƿ���Ӧ�¼�
	virtual void setVisibleAndRespondEvent(bool bVisible);

protected:
	bool m_bRespondEvent;
};


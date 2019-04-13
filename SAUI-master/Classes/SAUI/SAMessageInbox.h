#pragma once
#include "cocos2d.h"
#include "SANode.h"
#include "SA/SACreator.h"

class SAMessageInboxCell : public SANode
{
public:
	using Create = SACreator<true, SAMessageInboxCell>;

	virtual cocos2d::Action* createEnterAction(const cocos2d::Vec2& pos);
	virtual cocos2d::Action* createReplaceAction(const cocos2d::Vec2& pos);
	virtual cocos2d::Action* createExitAction(const cocos2d::Vec2& pos);
};



class SAMessageInbox : public SANode
{
public:
	enum InboxFillDirect
	{
		FILL_DIRECT_UP,
		FILL_DIRECT_DOWN,
		FILL_DIRECT_LEFT,
		FILL_DIRECT_RIGHT,
		FILL_DIRECT_NULL
	};

	virtual bool init(
		const cocos2d::Size& sizeCell,
		InboxFillDirect direct,
		size_t maxSize = 0,
		float fSpaceLenX = 0,
		float fSpaceLenY = 0
	);

protected:

};


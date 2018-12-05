#include "SANode.h"
#include "SA/SAUtils.h"

using namespace std;
using namespace cocos2d;

bool SANode::init()
{
	if (!Node::init())
		return false;

	return true;
}

void SANode::setRespondEventDeeply(Node* pNode, bool bEnable)
{
	for (auto pChild : pNode->getChildren())
	{
		setRespondEventDeeply(pChild, bEnable);
	}

	SANode* pChildNode = dynamic_cast<SANode*>(pNode);
	if (pChildNode)
	{
		pChildNode->setRespondEvent(bEnable);
	}
}

Size SANode::getTotalContentSize()
{
	return SAUtils::getTotalContentSize(this);
}

void SANode::setVisibleAndRespondEvent(bool bVisible)
{
	setVisible(bVisible);
	SANode::setRespondEventDeeply(this, bVisible);
}
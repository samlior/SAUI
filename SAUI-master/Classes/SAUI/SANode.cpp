#include "SANode.h"
#include "SA/SAUtils.h"

using namespace std;
using namespace cocos2d;

SANode::SANode() :
	m_bRespondEvent(true),
	m_bRestoreScissor(false),
	m_bClippingToBounds(false)
{
}

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

void SANode::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
	// quick return if not visible
	if (!isVisible())
	{
		return;
	}

	uint32_t flags = processParentFlags(parentTransform, parentFlags);

	// IMPORTANT:
	// To ease the migration to v3.0, we still support the Mat4 stack,
	// but it is deprecated and your code should not rely on it
	Director* director = Director::getInstance();
	CCASSERT(nullptr != director, "Director is null when setting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);

	if (m_bClippingToBounds)
	{
		m_cmdBeforeVisit.init(_globalZOrder);
		m_cmdBeforeVisit.func = CC_CALLBACK_0(SANode::beforeVisit, this);
		renderer->addCommand(&m_cmdBeforeVisit);
	}

	bool visibleByCamera = isVisitableByVisitingCamera();

	if (!_children.empty())
	{
		int i = 0;

		// draw children zOrder < 0
		for (; i < _children.size(); i++)
		{
			Node *child = _children.at(i);
			if (child->getLocalZOrder() < 0)
			{
				child->visit(renderer, _modelViewTransform, flags);
			}
			else
			{
				break;
			}
		}

		// this draw
		if (visibleByCamera)
			this->draw(renderer, _modelViewTransform, flags);

		// draw children zOrder >= 0
		for (; i < _children.size(); i++)
		{
			Node *child = _children.at(i);
			child->visit(renderer, _modelViewTransform, flags);
		}
	}
	else if (visibleByCamera)
	{
		this->draw(renderer, _modelViewTransform, flags);
	}
	
	if (m_bClippingToBounds)
	{
		m_cmdAfterVisit.init(_globalZOrder);
		m_cmdAfterVisit.func = CC_CALLBACK_0(SANode::afterVisit, this);
		renderer->addCommand(&m_cmdAfterVisit);
	}

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void SANode::beforeVisit()
{
	m_bRestoreScissor = false;
	Rect frame = getSelfRect();
	auto glview = Director::getInstance()->getOpenGLView();

	if (glview->getVR() == nullptr)
	{
		if (glview->isScissorEnabled())
		{
			m_bRestoreScissor = true;
			m_rectOriginal = glview->getScissorRect();
			if (frame.intersectsRect(m_rectOriginal))
			{
				float x = MAX(frame.origin.x, m_rectOriginal.origin.x);
				float y = MAX(frame.origin.y, m_rectOriginal.origin.y);
				float xx = MIN(frame.origin.x + frame.size.width, m_rectOriginal.origin.x + m_rectOriginal.size.width);
				float yy = MIN(frame.origin.y + frame.size.height, m_rectOriginal.origin.y + m_rectOriginal.size.height);
				glview->setScissorInPoints(x, y, xx - x, yy - y);
			}
		}
		else
		{
			glEnable(GL_SCISSOR_TEST);
			glview->setScissorInPoints(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
		}
	}
}

void SANode::afterVisit()
{
	auto glview = Director::getInstance()->getOpenGLView();
	if (glview->getVR() == nullptr)
	{
		if (m_bRestoreScissor)
		{
			glview->setScissorInPoints(m_rectOriginal.origin.x, m_rectOriginal.origin.y, m_rectOriginal.size.width, m_rectOriginal.size.height);
		}
		else
		{
			glDisable(GL_SCISSOR_TEST);
		}
	}
}

Rect SANode::getSelfRect()
{
	return Rect(convertToWorldSpace(Vec2::ZERO), getContentSize());
}
#include "SAAdaptedBox.h"
#include "SA/SAUtils.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;

SAAdaptedBox::~SAAdaptedBox()
{
	stopMouseMoveListen();
}

bool SAAdaptedBox::init(Scale9Sprite* pS9Spr /*= nullptr*/)
{
	if (!SANode::init())
		return false;

	m_vecDirectionpriority = { RIGHT_TOP, RIGHT_BOTTOM, LEFT_TOP, LEFT_BOTTOM };
	initNode();
	setBackground(pS9Spr);
	return true;
}

bool SAAdaptedBox::init(const vector<AdaptDirection>& vecPriority, Scale9Sprite* pS9Spr /* = nullptr */)
{
	if (!SANode::init())
		return false;

	setDirectionPriority(vecPriority);
	initNode();
	setBackground(pS9Spr);
	return true;
}

void SAAdaptedBox::initNode()
{
	m_pNode = Node::create();
	addChild(m_pNode, 1);

	m_pWrapper = Node::create();
	m_pNode->addChild(m_pWrapper);
}

void SAAdaptedBox::addChild(Node* child)
{
	if (m_pNode != child && m_pBackground != child)
	{
		m_pWrapper->addChild(child);
	}
	else
	{
		Node::addChild(child);
	}
}

void SAAdaptedBox::addChild(Node* child, int localZOrder)
{
	if (m_pNode != child && m_pBackground != child)
	{
		m_pWrapper->addChild(child, localZOrder);
	}
	else
	{
		Node::addChild(child, localZOrder);
	}
}

void SAAdaptedBox::addChild(Node* child, int localZOrder, int tag)
{
	if (m_pNode != child && m_pBackground != child)
	{
		m_pWrapper->addChild(child, localZOrder, tag);
	}
	else
	{
		Node::addChild(child, localZOrder, tag);
	}
}

void SAAdaptedBox::addChild(Node* child, int localZOrder, const std::string &name)
{
	if (m_pNode != child && m_pBackground != child)
	{
		m_pWrapper->addChild(child, localZOrder, name);
	}
	else
	{
		Node::addChild(child, localZOrder, name);
	}
}

void SAAdaptedBox::setBackground(Scale9Sprite* pS9Spr)
{
	if (pS9Spr == m_pBackground)
		return;

	if (m_pBackground)
	{
		m_pBackground->removeFromParentAndCleanup(true);
		m_pBackground = nullptr;
	}

	if (pS9Spr)
	{
		m_pBackground = pS9Spr;
		addChild(pS9Spr, 0);
	}

	adapt();
}

void SAAdaptedBox::adapt()
{
	BoxAdapter adapter(this);
	Vec2 vec2AnPoint, vec2Position, vec2WrapperPosition;
	adapter.adapt(vec2AnPoint, vec2Position, vec2WrapperPosition, m_bAdaptMode);

	if (m_sizeOffset.width || m_sizeOffset.height)
	{
		vec2WrapperPosition.x += m_sizeOffset.width / 2;
		vec2WrapperPosition.y += m_sizeOffset.height / 2;
	}

	if (m_pBackground)
	{
		m_pBackground->setContentSize(adapter.size + m_sizeOffset);
		m_pBackground->setAnchorPoint(vec2AnPoint);
		if (m_bForceMode)
			m_pBackground->setPosition(vec2Position);
	}

	m_pNode->setContentSize(adapter.size + m_sizeOffset);
	m_pNode->setAnchorPoint(vec2AnPoint);
	if (m_bForceMode)
		m_pNode->setPosition(vec2Position);

	m_pWrapper->setPosition(vec2WrapperPosition);
}

void SAAdaptedBox::startMouseMoveListen()
{
	if (!m_pMouseListener)
	{
		m_pMouseListener = EventListenerMouse::create();
		m_pMouseListener->onMouseMove = [this](EventMouse* pEvent)
		{
			if (this->m_bRespondEvent && this->isVisible())
			{
				this->setPosition(pEvent->getLocationInView());
				this->adapt();
			}
		};
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pMouseListener, this);
	}
}

void SAAdaptedBox::stopMouseMoveListen()
{
	if (m_pMouseListener)
	{
		Director::getInstance()->getEventDispatcher()->removeEventListener(m_pMouseListener);
		m_pMouseListener = nullptr;
	}
}

void SAAdaptedBox::setDirectionPriority(AdaptDirection direction, int iOrder)
{
	CCASSERT(iOrder >= 0 && iOrder <= 3, "illegale iOrder value");

	m_vecDirectionpriority.erase(find(m_vecDirectionpriority.begin(), m_vecDirectionpriority.end(), direction));
	auto itr = m_vecDirectionpriority.begin() + iOrder;
	m_vecDirectionpriority.insert(itr, direction);
}

bool SAAdaptedBox::getNextDirectionInfo(AdaptDirection& direction)
{
	auto itr = find(m_vecDirectionpriority.begin(), m_vecDirectionpriority.end(), direction);
	if (itr + 1 == m_vecDirectionpriority.end())
	{
		return false;
	}
	else
	{
		direction = *(itr + 1);
		return true;
	}
}




SAAdaptedBox::BoxAdapter::BoxAdapter(SAAdaptedBox* pBoxOuter)
{
	pBox = pBoxOuter;
	xl = 0, xr = 0, yu = 0, yb = 0;
	SAUtils::getTotalContentOffset(pBox->m_pWrapper, xl, xr, yu, yb);
	size = Size(xr - xl, yu - yb);
}

void SAAdaptedBox::BoxAdapter::adapt(Vec2& vec2AnPoint, Vec2& vec2Position, cocos2d::Vec2& vec2WrapperPosition, bool bAdapt)
{
	vec2WrapperPosition = Vec2(abs(xl), abs(yb));

	Size sizeVisible = Director::getInstance()->getVisibleSize();
	AdaptDirection direction = pBox->m_vecDirectionpriority[0];
	const Vec2& vec2BoxPosition = pBox->getPosition();
	float fOffsetX = 0;
	float fOffsetY = 0;
	bool bNotMatch = !bAdapt;
	while (true)
	{
		if (direction == LEFT_BOTTOM)
		{
			fOffsetX = vec2BoxPosition.x - size.width - pBox->m_sizeOffset.width;
			fOffsetY = vec2BoxPosition.y - size.height - pBox->m_sizeOffset.height;
			if (fOffsetX < 0 || fOffsetY < 0)
			{
				if (bNotMatch)
				{
					if (fOffsetX < 0)
					{
						vec2Position.x = abs(fOffsetX);
					}
					if (fOffsetY < 0)
					{
						vec2Position.y = abs(fOffsetY);
					}
				}
				else if (pBox->getNextDirectionInfo(direction))
				{
					continue;
				}
				else
				{
					bNotMatch = true;
					direction = pBox->m_vecDirectionpriority[0];
					continue;
				}
			}
		}
		else if (direction == LEFT_TOP)
		{
			fOffsetX = vec2BoxPosition.x - size.width - pBox->m_sizeOffset.width;
			fOffsetY = vec2BoxPosition.y + size.height + pBox->m_sizeOffset.height;
			if (fOffsetX < 0 || fOffsetY > sizeVisible.height)
			{
				if (bNotMatch)
				{
					if (fOffsetX < 0)
					{
						vec2Position.x = abs(fOffsetX);
					}
					if (fOffsetY > sizeVisible.height)
					{
						vec2Position.y = -abs(fOffsetY - sizeVisible.height);
					}
				}
				else if (pBox->getNextDirectionInfo(direction))
				{
					continue;
				}
				else
				{
					bNotMatch = true;
					direction = pBox->m_vecDirectionpriority[0];
					continue;
				}
			}
		}
		else if (direction == RIGHT_BOTTOM)
		{
			fOffsetX = vec2BoxPosition.x + size.width + pBox->m_sizeOffset.width;
			fOffsetY = vec2BoxPosition.y - size.height - pBox->m_sizeOffset.height;
			if (fOffsetX > sizeVisible.width || fOffsetY < 0)
			{
				if (bNotMatch)
				{
					if (fOffsetX > sizeVisible.width)
					{
						vec2Position.x = -abs(fOffsetX - sizeVisible.width);
					}
					if (fOffsetY < 0)
					{
						vec2Position.y = abs(fOffsetY);
					}
				}
				else if (pBox->getNextDirectionInfo(direction))
				{
					continue;
				}
				else
				{
					bNotMatch = true;
					direction = pBox->m_vecDirectionpriority[0];
					continue;
				}
			}
		}
		//RIGHT_TOP
		else
		{
			fOffsetX = vec2BoxPosition.x + size.width + pBox->m_sizeOffset.width;
			fOffsetY = vec2BoxPosition.y + size.height + pBox->m_sizeOffset.height;
			if (fOffsetX > sizeVisible.width || fOffsetY > sizeVisible.height)
			{
				if (bNotMatch)
				{
					if (fOffsetX > sizeVisible.width)
					{
						vec2Position.x = -abs(fOffsetX - sizeVisible.width);
					}
					if (fOffsetY > sizeVisible.height)
					{
						vec2Position.y = -abs(fOffsetY - sizeVisible.height);
					}
				}
				else if (pBox->getNextDirectionInfo(direction))
				{
					continue;
				}
				else
				{
					bNotMatch = true;
					direction = pBox->m_vecDirectionpriority[0];
					continue;
				}
			}
		}

		break;
	}

	static const std::map<AdaptDirection, Vec2> mapDirectionAnPoint =
	{
		{ LEFT_BOTTOM , Vec2::ANCHOR_TOP_RIGHT },
		{ LEFT_TOP , Vec2::ANCHOR_BOTTOM_RIGHT },
		{ RIGHT_BOTTOM , Vec2::ANCHOR_TOP_LEFT },
		{ RIGHT_TOP , Vec2::ANCHOR_BOTTOM_LEFT }
	};

	auto itr = mapDirectionAnPoint.find(direction);
	vec2AnPoint = itr->second;
}
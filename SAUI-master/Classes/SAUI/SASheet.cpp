#include "SASheet.h"
#include "SA/SAUtils.h"

using namespace std;
using namespace cocos2d;

const SAPoint SASheet::pointOutOfRange(-65535, -65535);

SASheet::~SASheet()
{
	stopTouchListen();
	stopMouseMoveListen();
}

bool SASheet::init(
	const Size& sizeElement,
	const SASize& size,
	float fSpaceLenX /*= 0*/,
	float fSpaceLenY /*= 0*/
)
{
	if (!SANode::init())
		return false;

	m_sizeElement = sizeElement;
	m_vecVecNode.resize(size.y);
	m_size = size;
	m_fSpaceLenX = fSpaceLenX;
	m_fSpaceLenY = fSpaceLenY;

	setContentSize(Size(
		sizeElement.width * size.x + (size.x > 0 ? (size.x - 1) * fSpaceLenX : 0),
		sizeElement.height * size.y + (size.y > 0 ? (size.y - 1) * fSpaceLenY : 0)
	));
	return true;
}

void SASheet::startTouchListen(bool bSwallowTouches /* = true */)
{
	if (!m_pTouchListener)
	{
		m_pTouchListener = EventListenerTouchOneByOne::create();
		m_pTouchListener->setSwallowTouches(bSwallowTouches);
		m_pTouchListener->onTouchBegan = CC_CALLBACK_2(SASheet::touchBeganCallBack, this);
		m_pTouchListener->onTouchEnded = CC_CALLBACK_2(SASheet::touchEndedCallBack, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pTouchListener, this);
	}
}

void SASheet::stopTouchListen()
{
	if (m_pTouchListener)
	{
		Director::getInstance()->getEventDispatcher()->removeEventListener(m_pTouchListener);
		m_pTouchListener = nullptr;
	}
}

void SASheet::startMouseMoveListen()
{
	if (!m_pMouseListener)
	{
		m_pMouseListener = EventListenerMouse::create();
		m_pMouseListener->onMouseMove = CC_CALLBACK_1(SASheet::mouseMoveCallBack, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pMouseListener, this);
	}
}

void SASheet::stopMouseMoveListen()
{
	if (m_pMouseListener)
	{
		Director::getInstance()->getEventDispatcher()->removeEventListener(m_pMouseListener);
		m_pMouseListener = nullptr;
	}
}

void SASheet::addChildNode(size_t uiX, size_t uiY, Node* pNode)
{
	pNode->setAnchorPoint(Vec2::ZERO);
	pNode->setIgnoreAnchorPointForPosition(false);
	pNode->setPosition(Vec2(uiX * m_sizeElement.width + uiX * m_fSpaceLenX, uiY * m_sizeElement.height + uiY * m_fSpaceLenY));
	addChild(pNode);
}

void SASheet::append(size_t uiRowIndex, Node* pNode)
{
	vector<Node*>& vecNode = m_vecVecNode[uiRowIndex];
	addChildNode(vecNode.size(), uiRowIndex, pNode);
	vecNode.push_back(pNode);
}

void SASheet::append(size_t uiRowIndex, const vector<Node*>& vecNode)
{
	for (auto itr = vecNode.begin(); itr != vecNode.end(); itr++)
		append(uiRowIndex, *itr);
}

void SASheet::append(const vector<Node*>& vecNode)
{
	if (vecNode.empty())
		return;

	unsigned int uiOffset = 0;
	for (auto itr = m_vecVecNode.begin(); itr != m_vecVecNode.end(); ++itr)
	{
		unsigned int uiCount = m_size.x - itr->size();
		if (uiCount > 0)
		{
			vector<Node*> vecAppendNode(
				vecNode.begin() + uiOffset,
				uiCount + uiOffset >= vecNode.size() ?
				vecNode.end() :
				vecNode.begin() + uiOffset + uiCount
			);

			append(itr - m_vecVecNode.begin(), vecAppendNode);

			if (uiCount + uiOffset >= vecNode.size())
				break;

			uiOffset += uiCount;
		}
	}
}

void SASheet::append(Node* pNode)
{
	vector<Node*> vecNode = { pNode };
	append(vecNode);
}

void SASheet::reload(const std::vector<cocos2d::Node*>& vecNode)
{
	clear();
	append(vecNode);
}

void SASheet::clear()
{
	for (auto itr = m_vecVecNode.begin(); itr != m_vecVecNode.end(); ++itr)
	{
		for (auto itrr = itr->begin(); itrr != itr->end(); ++itrr)
		{
			(*itrr)->removeFromParentAndCleanup(true);
		}
	}
	m_vecVecNode.clear();
	m_vecVecNode.resize(m_size.y);
}

size_t SASheet::getElementCount()
{
	size_t szTotal = 0;
	for (auto itr = m_vecVecNode.begin(); itr != m_vecVecNode.end(); ++itr)
	{
		szTotal += itr->size();
	}
	return szTotal;
}

Node* SASheet::at(const SAPoint& point)
{
	return m_vecVecNode[point.y][point.x];
}

static void calculateIndexWithSpace(float fLen, float fElement, float fSpace, int& iIndex)
{
	float f = 0;
	int i = 0;
	while (true)
	{
		if (i % 2 == 0)
		{
			f += fElement;
			iIndex++;
		}
		else
		{
			f += fSpace;
		}
		i++;
		if (f >= fLen)
		{
			iIndex--;
			break;
		}
	}
}

SAPoint SASheet::convertVec2ToSAPoint(const Vec2& vec2)
{
	SAPoint point(0, 0);
	calculateIndexWithSpace(vec2.x, m_sizeElement.width, m_fSpaceLenX, point.x);
	calculateIndexWithSpace(vec2.y, m_sizeElement.height, m_fSpaceLenY, point.y);
	return point;
}

Vec2 SASheet::convertSAPointToVec2(const SAPoint& point)
{
	return Vec2(
		point.x * m_sizeElement.width + (point.x > 0 ? (point.x - 1) * m_fSpaceLenX : 0),
		point.y * m_sizeElement.height + (point.y > 0 ? (point.y - 1) * m_fSpaceLenY : 0)
	);
}


void SASheet::mouseMoveCallBack(EventMouse* pEvent)
{
	if (!m_bRespondEvent || !isVisible())
		return;

	static SAPoint pointLast(pointOutOfRange);
	const Vec2& vec2 = pEvent->getLocationInView();
	if (SAUtils::isContain(this, vec2))
	{
		const SAPoint& point = convertVec2ToSAPoint(convertToNodeSpace(vec2));
		Node* pNode = at(point);
		if (SAUtils::isContain(pNode, vec2))
		{
			if (m_funcMouseMoveCallBack &&
				point != pointLast)
			{
				m_funcMouseMoveCallBack(this, pEvent, pointLast, point);
				pointLast = point;
			}
			return;
		}
	}

	if (m_funcMouseMoveCallBack && 
		pointLast != pointOutOfRange)
	{
		m_funcMouseMoveCallBack(this, pEvent, pointLast, pointOutOfRange);
		pointLast = pointOutOfRange;
	}
}

bool SASheet::touchBeganCallBack(Touch* pTouch, Event* pEvent)
{
	return m_bRespondEvent && isVisible() && SAUtils::isContain(this, pTouch->getLocation());
}

void SASheet::touchEndedCallBack(Touch* pTouch, Event* pEvent)
{
	Vec2& vec2 = pTouch->getLocation();
	if (SAUtils::isContain(this, vec2))
	{
		const SAPoint& point = convertVec2ToSAPoint(convertToNodeSpace(vec2));
		Node* pNode = at(point);
		if (SAUtils::isContain(pNode, vec2))
		{
			if (m_funcElementTouchedCallBack)
				m_funcElementTouchedCallBack(this, point);
		}
	}
}
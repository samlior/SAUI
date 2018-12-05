#include "SASlider.h"
#include "SAButton.h"
#include "SA/SAUtils.h"

#define BACKGROUND_TAG 101

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;



SASlider::SASlider() :
	m_fOffset(0),
	m_fOffsetMax(0),
	m_fOffsetMin(0),
	m_fOffsetEachPushPercent(0.05f),
	m_bEnable(true),
	m_pHeader(nullptr),
	m_pBlocker(nullptr),
	m_pTailer(nullptr),
	m_pHeaderTexture(nullptr),
	m_pTailerTexture(nullptr) ,
	m_pListener(nullptr)
{
}

SASlider::~SASlider()
{
	stopListenMouseScroll();
}

bool SASlider::init(
	const Size& size, 
	SliderDirection direction, 
	SAButton* pBlocker /* = nullptr */, 
	SAButton* pHeader /* = nullptr */,
	SAButton* pTailer /* = nullptr */
)
{
	if (!SANode::init())
		return false;

	setContentSize(size);
	m_direction = direction;

	setBlocker(pBlocker);
	setHeader(pHeader);
	setTailer(pTailer);

	if (!pBlocker && !pHeader && !pTailer)
	{
		updateOffsetRange();
	}
	return true;
}

void SASlider::startListenMouseScroll()
{
	if (!m_pListener)
	{
		m_pListener = EventListenerMouse::create();
		m_pListener->onMouseScroll = [this](EventMouse* pEvent)
		{
			if (this->m_bRespondEvent)
			{
				float fOffset = this->getContentOffsetEachPush() * -pEvent->getScrollY();
				this->setContentOffset(m_direction == HORIZONTAL ? m_fOffset - fOffset : m_fOffset + fOffset);
			}
		};
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pListener, this);
	}
}

void SASlider::stopListenMouseScroll()
{
	if (m_pListener)
	{
		Director::getInstance()->getEventDispatcher()->removeEventListener(m_pListener);
		m_pListener = nullptr;
	}
}

void SASlider::setBlocker(SAButton* pBlocker)
{
	if (pBlocker == m_pBlocker)
	{
		return;
	}

	if (m_pBlocker)
	{
		m_pBlocker->removeFromParentAndCleanup(true);
		m_pBlocker = nullptr;
	}

	if (pBlocker)
	{
		const Size& sizeHeader = m_pHeader ? m_pHeader->getContentSize() : Size(0, 0);
		const Size& sizeTailer = m_pTailer ? m_pTailer->getContentSize() : Size(0, 0);
		const Size& sizeBlocker = pBlocker ? pBlocker->getContentSize() : Size(0, 0);

		pBlocker->setTouchMovedCallBack(CC_CALLBACK_3(SASlider::blockerMovedCallBack, this));
		pBlocker->setAnchorPoint(Vec2(0.5, 0.5));
		addChild(pBlocker, 12);

		m_pBlocker = pBlocker;
	}

	updateOffsetRange();
	updateBlockerPosition(true);
	updateHeaderTextureSize(true);
	updateTailerTextureSize(true);
}

void SASlider::setHeader(SAButton* pHeader)
{
	if (pHeader == m_pHeader)
	{
		return;
	}

	if (m_pHeader)
	{
		m_pHeader->removeFromParentAndCleanup(true);
		m_pHeader = nullptr;
	}

	if (pHeader)
	{
		pHeader->setTouchEndedCallBack(CC_CALLBACK_3(SASlider::headerCallBack, this));
		pHeader->setAnchorPoint(m_direction == HORIZONTAL ? Vec2(0, 0.5) : Vec2(0.5, 1));
		pHeader->setPosition(m_direction == HORIZONTAL ? Vec2(0, _contentSize.height / 2) : Vec2(_contentSize.width / 2, _contentSize.height));
		addChild(pHeader, 11);

		m_pHeader = pHeader;
	}

	updateOffsetRange();
	updateBlockerPosition(true);
	updateHeaderTextureSize(true);
	updateTailerTextureSize(true);
}

void SASlider::setTailer(SAButton* pTailer)
{
	if (pTailer == m_pTailer)
	{
		return;
	}

	if (m_pTailer)
	{
		m_pTailer->removeFromParentAndCleanup(true);
		m_pTailer = nullptr;
	}

	if (pTailer)
	{
		pTailer->setTouchEndedCallBack(CC_CALLBACK_3(SASlider::tailerCallBack, this));
		pTailer->setAnchorPoint(m_direction == HORIZONTAL ? Vec2(1, 0.5) : Vec2(0.5, 0));
		pTailer->setPosition(m_direction == HORIZONTAL ? Vec2(_contentSize.width, _contentSize.height / 2) : Vec2(_contentSize.width / 2, 0));
		addChild(pTailer, 11);

		m_pTailer = pTailer;
	}

	updateOffsetRange();
	updateBlockerPosition(true);
	updateHeaderTextureSize(true);
	updateTailerTextureSize(true);
}

void SASlider::setHeaderTexture(Sprite* pHeaderTexture)
{
	if (pHeaderTexture == m_pHeaderTexture)
	{
		return;
	}

	if (m_pHeaderTexture)
	{
		m_pHeaderTexture->removeFromParentAndCleanup(true);
		m_pHeaderTexture = nullptr;
	}

	if (pHeaderTexture)
	{
		Texture2D::TexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
		pHeaderTexture->getTexture()->setTexParameters(tp);
		pHeaderTexture->setAnchorPoint(m_direction == HORIZONTAL ? Vec2(0, 0.5) : Vec2(0.5, 1));
		addChild(pHeaderTexture, 10);

		m_pHeaderTexture = pHeaderTexture;
	}

	updateHeaderTextureSize(true);
}

void SASlider::setTailerTexture(Sprite* pTailerTexture)
{
	if (pTailerTexture == m_pTailerTexture)
	{
		return;
	}

	if (m_pTailerTexture)
	{
		m_pTailerTexture->removeFromParentAndCleanup(true);
		m_pTailerTexture = nullptr;
	}

	if (pTailerTexture)
	{
		Texture2D::TexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
		pTailerTexture->getTexture()->setTexParameters(tp);
		pTailerTexture->setAnchorPoint(m_direction == HORIZONTAL ? Vec2(1, 0.5) : Vec2(0.5, 0));
		addChild(pTailerTexture, 10);

		m_pTailerTexture = pTailerTexture;
	}

	updateTailerTextureSize(true);
}

void SASlider::setBackground(Scale9Sprite* pS9Sprite)
{
	pS9Sprite->setContentSize(_contentSize);
	setBackground(static_cast<Node*>(pS9Sprite));
}

void SASlider::setBackground(Node* pNode)
{
	SAUtils::removeChildByTag(pNode, BACKGROUND_TAG);

	if (pNode)
	{
		pNode->setIgnoreAnchorPointForPosition(false);
		pNode->setAnchorPoint(Vec2(0.5, 0.5));
		pNode->setPosition(Vec2(_contentSize.width / 2, _contentSize.height / 2));
		pNode->setTag(BACKGROUND_TAG);
		addChild(pNode, 9);
	}
}

void SASlider::headerCallBack(SASelectedButton* pButton, Touch* pTouch, Event* pEvent)
{
	float fOffset = getContentOffsetEachPush();
	setContentOffset(m_direction == HORIZONTAL ? m_fOffset - fOffset : m_fOffset + fOffset);
}

void SASlider::tailerCallBack(SASelectedButton* pButton, Touch* pTouch, Event* pEvent)
{
	float fOffset = getContentOffsetEachPush();
	setContentOffset(m_direction == HORIZONTAL ? m_fOffset + fOffset : m_fOffset - fOffset);
}

void SASlider::blockerMovedCallBack(SASelectedButton* pButton, Touch* pTouch, Event* pEvent)
{
	const Vec2& vec2Offset = pTouch->getDelta();
	setContentOffset(m_fOffset + (m_direction == HORIZONTAL ? vec2Offset.x : vec2Offset.y));
}

float SASlider::calculatePercent(float fOffset)
{
	float fLen = m_fOffsetMax - m_fOffsetMin;
	if (!fLen)
	{
		return 0;
	}

	float fPercent = (fOffset - m_fOffsetMin) / fLen;
	return m_direction == HORIZONTAL ? fPercent : 1.0f - fPercent;
}

void SASlider::setContentOffsetEachPush(float fOffsetEachPush)
{
	setContentOffsetEachPushPercent(fOffsetEachPush / (m_fOffsetMax - m_fOffsetMin));
}

float SASlider::getContentOffsetEachPush()
{
	return m_fOffsetEachPushPercent * (m_fOffsetMax - m_fOffsetMin);
}

float SASlider::getContentOffsetPercent()
{
	return calculatePercent(m_fOffset);
}

void SASlider::setContentOffsetPercent(float fPercent, bool bCheckCallBack /* = true */)
{
	fPercent = m_direction == HORIZONTAL ? fPercent : 1.0f - fPercent;
	setContentOffset((m_fOffsetMax - m_fOffsetMin) * fPercent + m_fOffsetMin, bCheckCallBack);
}

void SASlider::setContentOffset(float fOffset, bool bCheckCallBack /* = true */)
{
	//禁用状态直接返回
	if (!m_bEnable)
	{
		return;
	}

	fOffset = fOffset > m_fOffsetMax ? m_fOffsetMax : fOffset;
	fOffset = fOffset < m_fOffsetMin ? m_fOffsetMin : fOffset;

	//偏移量未变化则不进行逻辑运算
	if (fOffset == m_fOffset)
	{
		return;
	}

	//如果需要判断回调，并且回调存在并返回false，则不进行逻辑运算，忽略本次偏移，直接返回
	if (bCheckCallBack && m_func && !m_func(this, calculatePercent(m_fOffset), calculatePercent(fOffset)))
	{
		return;
	}

	m_fOffset = fOffset;

	updateBlockerPosition();
	updateHeaderTextureSize();
	updateTailerTextureSize();
}

void SASlider::updateOffsetRange()
{
	float fPercent = getContentOffsetPercent();

	const Size& sizeHeader = m_pHeader ? m_pHeader->getContentSize() : Size(0, 0);
	const Size& sizeTailer = m_pTailer ? m_pTailer->getContentSize() : Size(0, 0);
	const Size& sizeBlocker = m_pBlocker ? m_pBlocker->getContentSize() : Size(0, 0);

	m_fOffsetMin = (m_direction == HORIZONTAL ? sizeHeader.width + sizeBlocker.width / 2 : sizeTailer.height + sizeBlocker.height / 2);
	m_fOffsetMax = (m_direction == HORIZONTAL ? _contentSize.width - sizeTailer.width - sizeBlocker.width / 2 : _contentSize.height - sizeHeader.height - sizeBlocker.height / 2);
	m_fOffset = (m_fOffsetMax - m_fOffsetMin) * fPercent + m_fOffsetMin;
}

void SASlider::updateBlockerPosition(bool bDirectlyCall /* = false */)
{
	if (m_pBlocker)
	{
		const Vec2& vec2Original = m_pBlocker->getPosition();
		if (bDirectlyCall || (m_direction == HORIZONTAL ? m_fOffset != vec2Original.x : m_fOffset != vec2Original.y))
		{
			m_pBlocker->setPosition(m_direction == HORIZONTAL ? Vec2(m_fOffset, _contentSize.height / 2) : Vec2(_contentSize.width / 2, m_fOffset));
		}
	}
}

void SASlider::updateHeaderTextureSize(bool bDirectlyCall /*= false*/)
{
	if (m_pHeaderTexture)
	{
		const Size& sizeBlocker = m_pBlocker ? m_pBlocker->getContentSize() : Size(0, 0);
		const Size& sizeHeader = m_pHeader ? m_pHeader->getContentSize() : Size(0, 0);
		const Size& sizeOriginal = m_pHeaderTexture->getTextureRect().size;

		if (bDirectlyCall || (m_direction == HORIZONTAL ? 
				sizeOriginal.width != m_fOffset - sizeHeader.width :
				sizeOriginal.height != m_fOffsetMax - m_fOffset + sizeBlocker.height / 2))
		{
			float fTextureWidth = m_direction == HORIZONTAL ? m_fOffset - sizeHeader.width : _contentSize.width;
			float fTextureHeight = m_direction == HORIZONTAL ? _contentSize.height : m_fOffsetMax - m_fOffset + sizeBlocker.height / 2;
			m_pHeaderTexture->setTextureRect(Rect(0, 0, fTextureWidth, fTextureHeight));
			m_pHeaderTexture->setPosition(m_direction == HORIZONTAL ?
				Vec2(sizeHeader.width, _contentSize.height / 2) :
				Vec2(_contentSize.width / 2, _contentSize.height - sizeHeader.height));
		}
	}
}

void SASlider::updateTailerTextureSize(bool bDirectlyCall /*= false*/)
{
	if (m_pTailerTexture)
	{
		const Size& sizeBlocker = m_pBlocker ? m_pBlocker->getContentSize() : Size(0, 0);
		const Size& sizeTailer = m_pTailer ? m_pTailer->getContentSize() : Size(0, 0);
		const Size& sizeOriginal = m_pTailerTexture->getTextureRect().size;

		if (bDirectlyCall || (m_direction == HORIZONTAL ?
				sizeOriginal.width != m_fOffsetMax - m_fOffset + sizeBlocker.width / 2 :
				sizeOriginal.height != m_fOffset - sizeTailer.height))
		{
			float fTextureWidth = m_direction == HORIZONTAL ? m_fOffsetMax - m_fOffset + sizeBlocker.width / 2 : _contentSize.width;
			float fTextureHeight = m_direction == HORIZONTAL ? _contentSize.height : m_fOffset - sizeTailer.height;
			m_pTailerTexture->setTextureRect(Rect(0, 0, fTextureWidth, fTextureHeight));
			m_pTailerTexture->setPosition(m_direction == HORIZONTAL ?
				Vec2(_contentSize.width - sizeTailer.width, _contentSize.height / 2) :
				Vec2(_contentSize.width / 2, sizeTailer.height));
		}
	}
}
#include "SAButton.h"
#include "cocos-ext.h"
#include "SA/SAUtils.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;





SASelectedButton::SASelectedButton() :
	m_pSelectedNormal(nullptr),
	m_pSelectedHighLight(nullptr),
	m_pUnselectedNormal(nullptr),
	m_pUnselectedHighLight(nullptr),
	m_pDisabled(nullptr),
	m_pListener(nullptr),
	m_funcOnTouchBegan(nullptr),
	m_funcOnTouchMoved(nullptr),
	m_funcOnTouchEnded(nullptr),
	m_bIsSelected(false),
	m_bEnable(true)
{
}

SASelectedButton::~SASelectedButton()
{
	stopTouchListen();
}


void SASelectedButton::startTouchListen(bool bSwallowTouches /* = true */)
{
	if (!m_pListener)
	{
		m_pListener = EventListenerTouchOneByOne::create();
		m_pListener->setSwallowTouches(bSwallowTouches);
		m_pListener->onTouchBegan = CC_CALLBACK_2(SASelectedButton::touchBeganCallBack, this);
		m_pListener->onTouchMoved = CC_CALLBACK_2(SASelectedButton::touchMovedCallBack, this);
		m_pListener->onTouchEnded = CC_CALLBACK_2(SASelectedButton::touchEndedCallBack, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pListener, this);
	}
}

void SASelectedButton::stopTouchListen()
{
	if (m_pListener)
	{
		Director::getInstance()->getEventDispatcher()->removeEventListener(m_pListener);
		m_pListener = nullptr;
	}
}

bool SASelectedButton::init(const Size& size, const string& strFileName, int iConfig)
{
	if (!SANode::init())
		return false;

	initButton(size, SAUtils::createWithFileConfigAndSize(size, strFileName, iConfig));
	return true;
}

bool SASelectedButton::init(const Size& size, Node* pNode)
{
	if (!SANode::init())
		return false;

	initButton(size, pNode);
	return true;
}

bool SASelectedButton::init(const Size& size, Scale9Sprite* pSprite)
{
	if (!SANode::init())
		return false;

	pSprite->setContentSize(size);
	initButton(size, pSprite);
	return true;
}

void SASelectedButton::initButton(const Size& size, Node* pNode)
{
	setContentSize(size);
	addChildNode(m_pUnselectedNormal, pNode);
	m_pUnselectedNormal->setVisible(true);

	startTouchListen();
}

void SASelectedButton::addChildNode(Node*& pNode, Node* pNodeOuter)
{
	if (pNode)
	{
		pNode->removeFromParentAndCleanup(true);
		pNode = nullptr;
	}

	pNode = pNodeOuter;
	pNode->setAnchorPoint(Vec2::ZERO);
	pNode->setPosition(Vec2::ZERO);
	pNode->setVisible(false);
	addChild(pNode);
}

void SASelectedButton::setSelectedNormal(const string& strFileName, int iConfig)
{
	addChildNode(m_pSelectedNormal, SAUtils::createWithFileConfigAndSize(_contentSize, strFileName, iConfig));
}

void SASelectedButton::setSelectedHighLight(const string& strFileName, int iConfig)
{
	addChildNode(m_pSelectedHighLight, SAUtils::createWithFileConfigAndSize(_contentSize, strFileName, iConfig));
}

void SASelectedButton::setUnselectedHighLight(const string& strFileName, int iConfig)
{
	addChildNode(m_pUnselectedHighLight, SAUtils::createWithFileConfigAndSize(_contentSize, strFileName, iConfig));
}

void SASelectedButton::setDisabled(const string& strFileName, int iConfig)
{
	addChildNode(m_pDisabled, SAUtils::createWithFileConfigAndSize(_contentSize, strFileName, iConfig));
	setEnable(m_bEnable);
}



void SASelectedButton::setSelectedNormal(Node* pNode)
{
	addChildNode(m_pSelectedNormal, pNode);
}

void SASelectedButton::setSelectedHighLight(Node* pNode)
{
	addChildNode(m_pSelectedHighLight, pNode);
}

void SASelectedButton::setUnselectedHighLight(Node* pNode)
{
	addChildNode(m_pUnselectedHighLight, pNode);
}

void SASelectedButton::setDisabled(Node* pNode)
{
	addChildNode(m_pDisabled, pNode);
	setEnable(m_bEnable);
}



void SASelectedButton::setSelectedNormal(Scale9Sprite* pSprite)
{
	pSprite->setContentSize(_contentSize);
	addChildNode(m_pSelectedNormal, pSprite);
}

void SASelectedButton::setSelectedHighLight(Scale9Sprite* pSprite)
{
	pSprite->setContentSize(_contentSize);
	addChildNode(m_pSelectedHighLight, pSprite);
}

void SASelectedButton::setUnselectedHighLight(Scale9Sprite* pSprite)
{
	pSprite->setContentSize(_contentSize);
	addChildNode(m_pUnselectedHighLight, pSprite);
}

void SASelectedButton::setDisabled(Scale9Sprite* pSprite)
{
	pSprite->setContentSize(_contentSize);
	addChildNode(m_pDisabled, pSprite);
	setEnable(m_bEnable);
}



void SASelectedButton::setEnable(bool bEnable)
{
	CCASSERT(m_pSelectedNormal != nullptr, "missing selected normal node!");

	m_bEnable = bEnable;
	if (m_bEnable)
	{
		if (m_pDisabled)
			m_pDisabled->setVisible(false);
		if (m_bIsSelected)
			m_pSelectedNormal->setVisible(true);
		else
			m_pUnselectedNormal->setVisible(true);
	}
	else
	{
		if (m_pDisabled)
		{
			m_pDisabled->setVisible(true);
			m_pSelectedNormal->setVisible(false);
			m_pUnselectedNormal->setVisible(false);
		}

		if (m_pUnselectedHighLight && m_pUnselectedHighLight->isVisible())
			m_pUnselectedHighLight->setVisible(false);
		if (m_pSelectedHighLight && m_pSelectedHighLight->isVisible())
			m_pSelectedHighLight->setVisible(false);
	}
}

void SASelectedButton::setSelected(bool bIsSelected)
{
	CCASSERT(m_pSelectedNormal != nullptr, "missing selected normal node!");

	if (bIsSelected != m_bIsSelected)
	{
		m_bIsSelected = bIsSelected;

		if (m_pUnselectedHighLight && m_pUnselectedHighLight->isVisible())
		{
			m_pUnselectedHighLight->setVisible(false);
			m_pSelectedNormal->setVisible(true);
		}
		else if (m_pSelectedHighLight && m_pSelectedHighLight->isVisible())
		{
			m_pSelectedHighLight->setVisible(false);
			m_pUnselectedNormal->setVisible(true);
		}

		if (m_bIsSelected)
		{
			m_pUnselectedNormal->setVisible(false);
			m_pSelectedNormal->setVisible(true);
		}
		else
		{
			m_pUnselectedNormal->setVisible(true);
			m_pSelectedNormal->setVisible(false);
		}
	}
}



bool SASelectedButton::touchBeganCallBack(Touch* pTouch, Event* pEvent)
{
	if (m_bRespondEvent && m_bEnable && isVisible() && SAUtils::isContain(this, pTouch->getLocation()))
	{
		if (!m_funcOnTouchBegan || m_funcOnTouchBegan(this, pTouch, pEvent))
		{
			CCASSERT(m_pSelectedNormal != nullptr, "missing selected normal node!");

			if (m_pUnselectedNormal->isVisible())
			{
				m_pUnselectedNormal->setVisible(false);

				if (m_pUnselectedHighLight)
				{
					m_pUnselectedHighLight->setVisible(true);
				}
				else if (m_pSelectedNormal)
				{
					m_pSelectedNormal->setVisible(true);
				}
			}
			else
			{
				m_pSelectedNormal->setVisible(false);

				if (m_pSelectedHighLight)
				{
					m_pSelectedHighLight->setVisible(true);
				}
				else
				{
					m_pUnselectedNormal->setVisible(true);
				}
			}
			return true;
		}
	}
	return false;
}

void SASelectedButton::touchMovedCallBack(Touch* pTouch, Event* pEvent)
{
	if (m_funcOnTouchMoved)
		m_funcOnTouchMoved(this, pTouch, pEvent);
}

void SASelectedButton::touchEndedCallBack(Touch* pTouch, Event* pEvent)
{
	setSelected(!m_bIsSelected);

	if (m_funcOnTouchEnded && SAUtils::isContain(this, pTouch->getLocation()))
	{
		m_funcOnTouchEnded(this, pTouch, pEvent);
	}
}





void SAButton::setEnable(bool bEnable)
{
	m_bEnable = bEnable;	
	if (bEnable)
	{
		if (m_pDisabled)
			m_pDisabled->setVisible(false);
		m_pUnselectedNormal->setVisible(true);
	}
	else
	{
		if (m_pDisabled)
		{
			m_pDisabled->setVisible(true);
			m_pUnselectedNormal->setVisible(false);
		}

		if (m_pUnselectedHighLight && m_pUnselectedHighLight->isVisible())
			m_pUnselectedHighLight->setVisible(false);
	}
}

void SAButton::setSelected(bool bIsSelected)
{
	if (bIsSelected != m_bIsSelected)
	{
		if (m_pUnselectedHighLight && m_pUnselectedHighLight->isVisible())
		{
			m_pUnselectedHighLight->setVisible(false);
			m_pUnselectedNormal->setVisible(true);
		}
	}
}

bool SAButton::touchBeganCallBack(Touch* pTouch, Event* pEvent)
{
	if (m_bRespondEvent && m_bEnable && isVisible() && SAUtils::isContain(this, pTouch->getLocation()))
	{
		if (!m_funcOnTouchBegan || m_funcOnTouchBegan(this, pTouch, pEvent))
		{
			if (m_pUnselectedNormal->isVisible())
			{
				if (m_pUnselectedHighLight)
				{
					m_pUnselectedNormal->setVisible(false);
					m_pUnselectedHighLight->setVisible(true);
				}
			}
			return true;
		}
	}
	return false;
}
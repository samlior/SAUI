#include "SAMessageBox.h"
#include "SAButton.h"
#include "SA/SAUtils.h"

#define DIALOG_BUTTON_TAG_BEGIN 110

#define DIALOG_TITLE_TAG		101
#define DIALOG_CONTENT_TAG		102
#define DIALOG_BACKGROUND_TAG	103
#define DIALOG_CLOSE_BUTTON_TAG	104
#define BACKGROUND_TAG			105

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;

SAMessageBox::SAMessageBox() :
	m_bTouchDisapear(false),
	m_pDialog(nullptr),
	m_pKeyboard(nullptr),
	m_pTouch(nullptr),
	m_func(nullptr)
{
}

SAMessageBox::~SAMessageBox()
{
	stopTouchListen();
	stopKeyboardListen();
}

bool SAMessageBox::init(const Size& sizeDialog, Scale9Sprite* pS9Spr)
{
	pS9Spr->setContentSize(sizeDialog);
	return init(sizeDialog, static_cast<Node*>(pS9Spr));
}

bool SAMessageBox::init(const Size& sizeDialog, Node* pNode)
{
	if (!SANode::init())
		return false;

	m_pDialog = Node::create();
	m_pDialog->setContentSize(sizeDialog);
	pNode->setAnchorPoint(Vec2::ZERO);
	pNode->setPosition(Vec2::ZERO);
	pNode->setTag(DIALOG_BACKGROUND_TAG);
	m_pDialog->addChild(pNode);

	setContentSize(sizeDialog);
	addChild(m_pDialog, 10);
	return true;
}

void SAMessageBox::setBackground(const Size& sizeBackground, Node* pNode /*= nullptr*/)
{
	SAUtils::removeChildByTag(this, BACKGROUND_TAG);

	setContentSize(sizeBackground);
	m_pDialog->setAnchorPoint(Vec2(0.5, 0.5));
	m_pDialog->setPosition(Vec2(sizeBackground.width / 2, sizeBackground.height / 2));

	if (!pNode)
		pNode = LayerColor::create(Color4B(111, 111, 111, 111), sizeBackground.width, sizeBackground.height);
	pNode->setAnchorPoint(Vec2::ZERO);
	pNode->setPosition(Vec2::ZERO);
	pNode->setTag(BACKGROUND_TAG);
	addChild(pNode, 9);
}

void SAMessageBox::setButton(const vector<SAButton*>& vecButton, float fOffset)
{
	if (vecButton.empty())
		return;

	Node* pOrgNode = m_pDialog->getChildByTag(DIALOG_BUTTON_TAG_BEGIN);
	for (int i = 1; pOrgNode; i++)
	{
		pOrgNode->removeFromParentAndCleanup(true);
		pOrgNode = m_pDialog->getChildByTag(DIALOG_BUTTON_TAG_BEGIN + i);
	}

	const Size& sizeDialog = m_pDialog->getContentSize();
	float fY = fOffset;
	float fX = sizeDialog.width / (vecButton.size() + 1);

	for (auto itr = vecButton.begin(); itr != vecButton.end(); ++itr)
	{
		SAButton* pButton = *itr;
		pButton->setTouchEndedCallBack(CC_CALLBACK_3(SAMessageBox::touchEndedCallBack, this));
		pButton->setAnchorPoint(Vec2(0.5, 0.5));
		pButton->setPosition(Vec2(
			((itr - vecButton.begin()) * fX + fX),
			fY
		));
		pButton->setTag(DIALOG_BUTTON_TAG_BEGIN + (itr - vecButton.begin()));
		m_pDialog->addChild(pButton, 9);
	}
}

void SAMessageBox::touchEndedCallBack(SASelectedButton* pButton, Touch* pTouch, Event* pEvent)
{
	if (!m_func || m_func(this, pButton->getTag() - DIALOG_BUTTON_TAG_BEGIN, pButton))
		setVisibleAndRespondEvent(false);
}

void SAMessageBox::closeTouchEndedCallBack(SASelectedButton* pButton, Touch* pTouch, Event* pEvent)
{
	setVisibleAndRespondEvent(false);
}

void SAMessageBox::setTitle(const string& strTitle, const string& strFontName, float fFontSize, const Color3B& clrFont, const Vec2& vec2Position)
{
	Label* pLabelTitle = Label::createWithTTF(strTitle, strFontName, fFontSize, Size::ZERO, TextHAlignment::CENTER);
	pLabelTitle->setColor(clrFont);
	setTitle(pLabelTitle, vec2Position);
}

void SAMessageBox::setTitle(cocos2d::Node* pTitle, const Vec2& vec2Position)
{
	SAUtils::removeChildByTag(m_pDialog, DIALOG_TITLE_TAG);

	const Size& sizeDialog = m_pDialog->getContentSize();
	pTitle->setPosition(vec2Position);
	pTitle->setTag(DIALOG_TITLE_TAG);
	m_pDialog->addChild(pTitle, 8);
}

void SAMessageBox::setContent(const string& strContent, const string& strFontName, float fFontSize, const Color3B& clrFont)
{
	Label* pLabelContent = Label::createWithTTF(strContent, strFontName, fFontSize, Size::ZERO, TextHAlignment::CENTER);
	pLabelContent->setColor(clrFont);
	pLabelContent->setMaxLineWidth(m_pDialog->getContentSize().width / 8 * 7);
	setContent(pLabelContent);
}

void SAMessageBox::setContent(Node* pContent)
{
	SAUtils::removeChildByTag(m_pDialog, DIALOG_CONTENT_TAG);

	const Size& sizeDialog = m_pDialog->getContentSize();
	float fY = sizeDialog.height / 2;
	float fX = sizeDialog.width / 2;
	pContent->setPosition(Vec2(fX, fY));
	pContent->setTag(DIALOG_CONTENT_TAG);
	m_pDialog->addChild(pContent, 10);
}

void SAMessageBox::setCloseButton(SAButton* pButton, const Vec2& vec2Position)
{
	SAUtils::removeChildByTag(m_pDialog, DIALOG_CLOSE_BUTTON_TAG);

	const Size& sizeDialog = m_pDialog->getContentSize();
	pButton->setPosition(vec2Position);
	pButton->setTag(DIALOG_CLOSE_BUTTON_TAG);
	pButton->setTouchEndedCallBack([this](SASelectedButton*, Touch*, Event*) { this->setVisibleAndRespondEvent(false); });
	m_pDialog->addChild(pButton);
}

void SAMessageBox::setTouchDisappear(bool bTouchDisappear)
{
	m_bTouchDisapear = bTouchDisappear;
}

void SAMessageBox::startKeyboardListen(EventKeyboard::KeyCode code /*= EventKeyboard::KeyCode::KEY_ESCAPE*/)
{
	if (!m_pKeyboard)
	{
		m_pKeyboard = EventListenerKeyboard::create();
		m_pKeyboard->onKeyReleased = [code, this](EventKeyboard::KeyCode k, Event*)
		{
			if (this->m_bRespondEvent && this->isVisible() && k == code)
				this->setVisibleAndRespondEvent(false);
		};
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pKeyboard, this);
	}
}

void SAMessageBox::stopKeyboardListen()
{
	if (m_pKeyboard)
	{
		Director::getInstance()->getEventDispatcher()->removeEventListener(m_pTouch);
		m_pKeyboard = nullptr;
	}
}

void SAMessageBox::startTouchListen(bool bSwallowTouches /* = true */, bool bTouchDisappear /* = fasle */)
{
	setTouchDisappear(bTouchDisappear);
	if (!m_pTouch)
	{
		m_pTouch = EventListenerTouchOneByOne::create();
		m_pTouch->setSwallowTouches(bSwallowTouches);
		m_pTouch->onTouchBegan = [this](Touch*, Event*) { return this->m_bRespondEvent && this->isVisible(); };
		m_pTouch->onTouchEnded = [this](Touch* pTouch, Event*)
		{
			const Vec2& vec2 = pTouch->getLocation();
			if (this->m_bTouchDisapear && SAUtils::isContain(this, vec2))
			{
				if (!SAUtils::isContain(m_pDialog, vec2))
				{
					setVisibleAndRespondEvent(false);
				}
			}
		};
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pTouch, this);
	}
}

void SAMessageBox::stopTouchListen()
{
	if (m_pTouch)
	{
		Director::getInstance()->getEventDispatcher()->removeEventListener(m_pTouch);
		m_pTouch = nullptr;
	}
}
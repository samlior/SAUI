/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "SA/SAUtils.h"

#define DEFAULT_FONT_NAME		"fonts/arial.ttf"

#define TABLEVIEW_LABEL_TAG		101
#define TABLEVIEW_LEFT_TAG		102
#define TABLEVIEW_RIGHT_TAG		103
#define TABLEVIEW_COMBO_TAG		104

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;

static Vec2 g_vec2TableView = Vec2(100, 100);
static Size g_sizeTableView = Size(500, 500);
static Size g_sizeCell = Size(g_sizeTableView.width, g_sizeTableView.height / 10);
static Size g_sizeSlider = Size(g_sizeTableView.width / 10, g_sizeTableView.height);
static Size g_sizeButton = Size(g_sizeTableView.width / 10, g_sizeTableView.height / 10);

static Size g_sizeSelectedButton = Size(250, 100);

static Size g_sizeSheetElement = Size(200, 200);

static Size g_sizeTitleButton = g_sizeSelectedButton / 2;

static SAButton* createButton(const Size& size)
{
	SAButton* pButton = SAButton::CreateWithFile::create(size, "s9b.png", LOAD_TEXTURE_FROM_LOCAL | SPR_TYPE_SCALE9);
	pButton->setUnselectedHighLight("s9.png", LOAD_TEXTURE_FROM_LOCAL | SPR_TYPE_SCALE9);
	return pButton;
}

static SAButton* createButtonWithLabel(const Size& size, const string& strText)
{
	SAButton* pButton = createButton(size);
	Label* pLabel = Label::createWithTTF(strText, DEFAULT_FONT_NAME, 20);
	pLabel->setPosition(size.width / 2, size.height / 2);
	pButton->addChild(pLabel);
	return pButton;
}

static SAButton* createButtonWithLabelAndDefaultCallBack(const Size& size, const string& strText)
{
	SAButton* pButton = createButtonWithLabel(size, strText);
	pButton->setTouchBeganCallBack([](SASelectedButton* p, Touch*, Event*) 
	{
		if (!p->getParent()->isVisible())
		{
			return false;
		}
		return true;
	});
	return pButton;
}

static SASelectedButton* createSelectedButton(const Size& size)
{
	SASelectedButton* pButton = SASelectedButton::CreateWithFile::create(size, "s9b.png", LOAD_TEXTURE_FROM_LOCAL | SPR_TYPE_SCALE9);
	pButton->setUnselectedHighLight("s9hb.png", LOAD_TEXTURE_FROM_LOCAL | SPR_TYPE_SCALE9);
	pButton->setSelectedNormal("s9.png", LOAD_TEXTURE_FROM_LOCAL | SPR_TYPE_SCALE9);
	pButton->setSelectedHighLight("s9h.png", LOAD_TEXTURE_FROM_LOCAL | SPR_TYPE_SCALE9);
	pButton->setAnchorPoint(Vec2(0.5, 0.5));
	return pButton;
}

template<typename T = SASelectedButton>
static T* createSelectedButtonWithLabel(const Size& size, const std::string& strText)
{
	Scale9Sprite* pS90 = Scale9Sprite::create("s9b.png");
	Label* pLabel0 = Label::createWithTTF(strText, DEFAULT_FONT_NAME, 20);
	pLabel0->setPosition(size.width / 2, size.height / 2);
	pS90->addChild(pLabel0);

	Scale9Sprite* pS91 = Scale9Sprite::create("s9hb.png");
	Label* pLabel1 = Label::createWithTTF(strText, DEFAULT_FONT_NAME, 20);
	pLabel1->setColor(Color3B(0x2C, 0x2C, 0x2C));
	pLabel1->setPosition(size.width / 2, size.height / 2);
	pS91->addChild(pLabel1);

	Scale9Sprite* pS92 = Scale9Sprite::create("s9.png");
	Label* pLabel2 = Label::createWithTTF(strText, DEFAULT_FONT_NAME, 20);
	pLabel2->setPosition(size.width / 2, size.height / 2);
	pS92->addChild(pLabel2);

	Scale9Sprite* pS93 = Scale9Sprite::create("s9h.png");
	Label* pLabel3 = Label::createWithTTF(strText, DEFAULT_FONT_NAME, 20);
	pLabel3->setColor(Color3B(0xF1, 0x48, 0x45));
	pLabel3->setPosition(size.width / 2, size.height / 2);
	pS93->addChild(pLabel3);

	T* pButton = T::CreateWithS9Sprite::create(size, pS90);
	pButton->setUnselectedHighLight(pS91);
	pButton->setSelectedNormal(pS92);
	pButton->setSelectedHighLight(pS93);
	return pButton;
}



Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

HelloWorld::~HelloWorld()
{
	if (m_pGroup)
	{
		m_pGroup->release();
	}
}

bool HelloWorld::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto g_sizeVisible = Director::getInstance()->getVisibleSize();

    auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this)
	);
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	LayerColor* pBg = LayerColor::create(Color4B(255, 255, 255, 255), g_sizeVisible.width, g_sizeVisible.height);
	addChild(pBg);

	createGLViewImplTest();
	createComboBoxTest();
	createQRCodeTest();
	createMessageBoxTest();
	createSheetTest();
	createSelectedButtonTest();
	createTableViewTest();

	vector<Node*> vecNode = { 
		m_pTableViewTest,
		m_pSelectedButtonTest,
		m_pSheetTest,
		m_pMessageBoxTest,
		m_pQRCodeTest,
		m_pComboBoxTest,
		m_pGLViewImplTest
	};

	m_pGroup = SAButtonGroup::Create::create();
	for (size_t i = 0; i < vecNode.size(); i++)
	{
		SAControlButton* pButton = createSelectedButtonWithLabel<SAControlButton>(g_sizeTitleButton, to_string(i));
		pButton->setAnchorPoint(Vec2(0, 1));
		pButton->setPosition(Vec2(i * g_sizeTitleButton.width, g_sizeVisible.height));
		pButton->setNode(vecNode[i]);
		addChild(pButton);
		m_pGroup->add(pButton);
	}

    return true;
}

void HelloWorld::createTableViewTest()
{
	auto g_sizeVisible = Director::getInstance()->getVisibleSize();

	m_pTableViewTest = SANode::Create::create();

	{
		SASlider* pSlider = SASlider::CreateWithAllButton::create(
			g_sizeSlider,
			SASlider::VERTICAL,
			createButtonWithLabelAndDefaultCallBack(g_sizeButton, "LB"),
			createButtonWithLabelAndDefaultCallBack(g_sizeButton, "LH"),
			createButtonWithLabelAndDefaultCallBack(g_sizeButton, "LT")
		);
		pSlider->setHeaderTexture(Sprite::create("texturey.png"));
		pSlider->setPosition(g_vec2TableView.x + g_sizeTableView.width, g_vec2TableView.y);
		m_pTableViewTest->addChild(pSlider);

		SASliderTableView* pTableView = SASliderTableView::CreateWithTag::create(this, g_sizeTableView, pSlider, TABLEVIEW_LEFT_TAG);
		pTableView->setPosition(g_vec2TableView);
		m_pTableViewTest->addChild(pTableView);
	}

	{
		SASlider* pSlider = SASlider::CreateWithBlocker::create(
			g_sizeSlider,
			SASlider::VERTICAL,
			createButtonWithLabelAndDefaultCallBack(g_sizeButton, "RB")
		);
		pSlider->setBackground(Scale9Sprite::create("texturey.png"));
		pSlider->setPosition(g_sizeVisible.width - g_vec2TableView.x - g_sizeTableView.width - g_sizeSlider.width, g_vec2TableView.y);
		m_pTableViewTest->addChild(pSlider);

		SASliderTableView* pTableView = SASliderTableView::CreateWithTag::create(this, g_sizeTableView, pSlider, TABLEVIEW_RIGHT_TAG);
		pTableView->setPosition(Vec2(g_sizeVisible.width - g_vec2TableView.x - g_sizeTableView.width, g_vec2TableView.y));
		m_pTableViewTest->addChild(pTableView);
	}

	m_pTableViewTest->setVisibleAndRespondEvent(false);
	addChild(m_pTableViewTest);
}

void HelloWorld::createSelectedButtonTest()
{
	auto g_sizeVisible = Director::getInstance()->getVisibleSize();

	static float g_fSpaceX = (g_sizeVisible.width - g_sizeSelectedButton.width * 4) / 5;

	m_pSelectedButtonTest = SANode::Create::create();

	{
		SASheet* pSheet = SASheet::CreateWithSpace::create(g_sizeSelectedButton, SASize(4, 1), g_fSpaceX, 0);
		Scale9Sprite* pS90 = Scale9Sprite::create("s9b.png");
		pS90->setContentSize(g_sizeSelectedButton);
		Label* pLabel0 = Label::createWithTTF("UnselectedNormal", DEFAULT_FONT_NAME, 20);
		pLabel0->setPosition(g_sizeSelectedButton.width / 2, g_sizeSelectedButton.height / 2);
		pS90->addChild(pLabel0);

		Scale9Sprite* pS91 = Scale9Sprite::create("s9hb.png");
		pS91->setContentSize(g_sizeSelectedButton);
		Label* pLabel1 = Label::createWithTTF("UnselectedHighLight", DEFAULT_FONT_NAME, 20);
		pLabel1->setColor(Color3B(0x2C, 0x2C, 0x2C));
		pLabel1->setPosition(g_sizeSelectedButton.width / 2, g_sizeSelectedButton.height / 2);
		pS91->addChild(pLabel1);

		Scale9Sprite* pS92 = Scale9Sprite::create("s9.png");
		pS92->setContentSize(g_sizeSelectedButton);
		Label* pLabel2 = Label::createWithTTF("SelectedNormal", DEFAULT_FONT_NAME, 20);
		pLabel2->setPosition(g_sizeSelectedButton.width / 2, g_sizeSelectedButton.height / 2);
		pS92->addChild(pLabel2);

		Scale9Sprite* pS93 = Scale9Sprite::create("s9h.png");
		pS93->setContentSize(g_sizeSelectedButton);
		Label* pLabel3 = Label::createWithTTF("SelectedHighLight", DEFAULT_FONT_NAME, 20);
		pLabel3->setColor(Color3B(0xF1, 0x48, 0x45));
		pLabel3->setPosition(g_sizeSelectedButton.width / 2, g_sizeSelectedButton.height / 2);
		pS93->addChild(pLabel3);

		vector<Node*> vecTotalElement = { pS90, pS91, pS92, pS93 };
		pSheet->append(vecTotalElement);
		pSheet->setPosition(g_fSpaceX, g_sizeVisible.height / 4 * 3);
		m_pSelectedButtonTest->addChild(pSheet);
	}

	{
		Size sizeArrow = Size(g_fSpaceX, g_sizeSelectedButton.height);
		SASheet* pSheet = SASheet::CreateWithSpace::create(sizeArrow, SASize(3, 1), g_sizeSelectedButton.width, 0);
		vector<Node*> vecTotalElement;
		for (int i = 0; i < 3; i++)
		{
			Sprite* pSpr = SAUtils::createWithFileConfig("arrowright.png", LOAD_TEXTURE_FROM_LOCAL | SPR_TYPE_NOMAL);
			pSpr->setPosition(sizeArrow.width / 2, sizeArrow.height / 2);
			Node* pWrapper = Node::create();
			pWrapper->addChild(pSpr);
			vecTotalElement.push_back(pWrapper);
		}
		pSheet->append(vecTotalElement);
		pSheet->setPosition(g_fSpaceX + g_sizeSelectedButton.width, g_sizeVisible.height / 4 * 3);
		m_pSelectedButtonTest->addChild(pSheet);
	}


	{
		SASelectedButton* pButton = createSelectedButtonWithLabel(g_sizeSelectedButton, "SelectedButton");
		pButton->setAnchorPoint(Vec2(0.5, 0.5));
		pButton->setPosition(g_sizeVisible.width / 2, g_sizeVisible.height / 4);
		m_pSelectedButtonTest->addChild(pButton);
	}

	m_pSelectedButtonTest->setVisibleAndRespondEvent(false);
	addChild(m_pSelectedButtonTest);
}



class SheetElement : public Node
{
public:
	using CreateWithSizeAndIndex = SACreator<true, SheetElement, const Size&, int>;

	SheetElement() :m_pS9HighLight(nullptr) {}

	bool init(const Size& size, int index)
	{
		if (!Node::init())
			return false;

		setContentSize(size);

		Scale9Sprite* pS9 = Scale9Sprite::create("s9hb.png");
		pS9->setContentSize(size);
		pS9->setAnchorPoint(Vec2::ZERO);
		addChild(pS9);

		Label* pLabel = Label::createWithTTF(to_string(index), DEFAULT_FONT_NAME, 30);
		pLabel->setColor(Color3B(0x2C, 0x2C, 0x2C));
		pLabel->setPosition(size.width / 2, size.height / 2);
		addChild(pLabel);

		m_pS9HighLight = Scale9Sprite::create("s9h.png");
		m_pS9HighLight->setContentSize(size);
		m_pS9HighLight->setAnchorPoint(Vec2::ZERO);
		m_pS9HighLight->setVisible(false);
		addChild(m_pS9HighLight);
		return true;
	}

	void highLight() { m_pS9HighLight->setVisible(true); }
	void normal() { m_pS9HighLight->setVisible(false); }

private:
	Scale9Sprite* m_pS9HighLight;
};

void HelloWorld::createSheetTest()
{
	auto g_sizeVisible = Director::getInstance()->getVisibleSize();

	m_pSheetTest = SANode::Create::create();

	{
		SAAdaptedBox* pAdpBox = SAAdaptedBox::CreateWithBackground::create(Scale9Sprite::create("s9w.png"));
		pAdpBox->setVisible(false);
		pAdpBox->setOffsetSize(Size(10, 10));
		pAdpBox->startMouseMoveListen();
		//pAdpBox->setAdaptMode(false);
		//pAdpBox->setForceMode(false);

		//std::vector<SAAdaptedBox::AdaptDirection> vecPriority = { SAAdaptedBox::AdaptDirection::RIGHT_BOTTOM, SAAdaptedBox::AdaptDirection::RIGHT_TOP };
		//pAdpBox->setDirectionPriority(vecPriority);

		{
			Label* pLable1 = Label::createWithTTF("LEFT_BOTTOM", DEFAULT_FONT_NAME, 20);
			pLable1->setColor(Color3B(0x3C, 0x3C, 0x3C));
			pLable1->setAnchorPoint(Vec2::ZERO);
			pLable1->setPosition(-250, -250);
			pAdpBox->addChild(pLable1);

			Label* pLable2 = Label::createWithTTF("LEFT_TOP", DEFAULT_FONT_NAME, 20);
			pLable2->setColor(Color3B(0x3C, 0x3C, 0x3C));
			pLable2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
			pLable2->setPosition(-250, 250);
			pAdpBox->addChild(pLable2);

			Label* pLable3 = Label::createWithTTF("RIGHT_BOTTOM", DEFAULT_FONT_NAME, 20);
			pLable3->setColor(Color3B(0x3C, 0x3C, 0x3C));
			pLable3->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
			pLable3->setPosition(250, -250);
			pAdpBox->addChild(pLable3);

			Label* pLable4 = Label::createWithTTF("RIGHT_TOP", DEFAULT_FONT_NAME, 20);
			pLable4->setColor(Color3B(0x3C, 0x3C, 0x3C));
			pLable4->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
			pLable4->setPosition(250, 250);
			pAdpBox->addChild(pLable4);
		}

		m_pSheetTest->addChild(pAdpBox, 10);



		SASheet* pSheet = SASheet::Create::create(g_sizeSheetElement, SASize(3, 3));
		vector<Node*> vecTotalElement;
		for (int i = 0; i < 9; i++)
		{
			vecTotalElement.push_back(SheetElement::CreateWithSizeAndIndex::create(g_sizeSheetElement, i));
		}
		pSheet->append(vecTotalElement);
		Size sizeTotalContent = SAUtils::getTotalContentSize(pSheet);
		Vec2 vec2(g_sizeVisible.width / 2 - sizeTotalContent.width / 2, g_sizeVisible.height / 16);
		pSheet->setPosition(vec2);
		pSheet->startMouseMoveListen();
		pSheet->setMouseMoveCallBack([pAdpBox](SASheet* pSheet, EventMouse* pEvent, const SAPoint& pointA, const SAPoint& pointB)
		{
			if (pointA != SASheet::pointOutOfRange)
			{
				SheetElement* pElement = dynamic_cast<SheetElement*>(pSheet->at(pointA));
				if (pElement)
				{
					pElement->normal();
				}
			}

			if (pointB != SASheet::pointOutOfRange)
			{
				SheetElement* pElement = dynamic_cast<SheetElement*>(pSheet->at(pointB));
				if (pElement)
				{
					pElement->highLight();
				}

				pAdpBox->setVisible(true);
			}
			else
			{
				pAdpBox->setVisible(false);
			}
		});
		m_pSheetTest->addChild(pSheet);
	}

	m_pSheetTest->setVisibleAndRespondEvent(false);
	addChild(m_pSheetTest);
}

void HelloWorld::createMessageBoxTest()
{
	auto g_sizeVisible = Director::getInstance()->getVisibleSize();

	m_pMessageBoxTest = SANode::Create::create();

	{
		Size sizeMessageBox = Size(g_sizeVisible.width / 2, g_sizeVisible.height / 3);

		m_pMessageBox = SAMessageBox::CreateWithS9Sprite::create(sizeMessageBox, Scale9Sprite::create("s9w.png"));
		m_pMessageBox->startKeyboardListen();
		m_pMessageBox->startTouchListen(true, true);

		SAButton* pButton = SAButton::CreateWithFile::create(g_sizeButton, "close.png", LOAD_TEXTURE_FROM_LOCAL | SPR_TYPE_NOMAL);
		pButton->setAnchorPoint(Vec2(1, 1));
		m_pMessageBox->setCloseButton(pButton, Vec2(sizeMessageBox.width - 10, sizeMessageBox.height - 10));

		Size sizeButton = Size(sizeMessageBox.width / 6, sizeMessageBox.height / 4);

		vector<SAButton*> vecButton = {
			createButtonWithLabelAndDefaultCallBack(sizeButton, "No"),
			createButtonWithLabelAndDefaultCallBack(sizeButton, "Yes")
		};
		m_pMessageBox->setTitle("Warnning", DEFAULT_FONT_NAME, 25, Color3B(0x2C, 0x2C, 0x2C), Vec2(sizeMessageBox.width / 2, sizeMessageBox.height / 4 * 3));
		m_pMessageBox->setContent("Are you sure you want to close?", DEFAULT_FONT_NAME, 20, Color3B(0x2C, 0x2C, 0x2C));
		m_pMessageBox->setButton(vecButton, sizeMessageBox.height / 4);
		m_pMessageBox->setBackground(g_sizeVisible);
		m_pMessageBox->setVisible(false);
		m_pMessageBox->setCallBack([this](SAMessageBox*, unsigned int uiIndex, SASelectedButton*) 
		{ 
			if (uiIndex)
			{
				Director::getInstance()->end();
			}
			return true;
		});

//		custom message box
// 		Node* pDialogNode = m_pMessageBox->getMessageBoxNode();
// 		pDialogNode->addChild(...);

		addChild(m_pMessageBox, 10);
	}

	{
		SAButton* pButton = createButtonWithLabelAndDefaultCallBack(g_sizeSelectedButton, "show box");
		pButton->setPosition(g_sizeVisible.width / 2, g_sizeVisible.height / 2);
		pButton->setAnchorPoint(Vec2(0.5, 0.5));
		pButton->setTouchEndedCallBack([this](SASelectedButton*, Touch*, Event*) 
		{
			this->m_pMessageBox->setVisibleAndRespondEvent(true);
		});
		m_pMessageBoxTest->addChild(pButton);
	}

	m_pMessageBoxTest->setVisibleAndRespondEvent(false);
	addChild(m_pMessageBoxTest);
}

void HelloWorld::createQRCodeTest()
{
	auto g_sizeVisible = Director::getInstance()->getVisibleSize();

	m_pQRCodeTest = SANode::Create::create();

#ifdef SA_USE_QRCODE
	{
		static int g_iPushCount = 0;

		SAQRCodeNode* pQRCodeNode = SAQRCodeNode::CreateWithText::create(g_sizeSheetElement, to_string(g_iPushCount));
		pQRCodeNode->setPosition(g_sizeVisible.width / 2, g_sizeVisible.height / 2);
		pQRCodeNode->setAnchorPoint(Vec2(0.5, 0.5));
		m_pQRCodeTest->addChild(pQRCodeNode);

		SAButton* pButton = createButtonWithLabelAndDefaultCallBack(g_sizeSelectedButton, "update qrcode");
		pButton->setPosition(g_sizeVisible.width / 2, g_sizeVisible.height / 4);
		pButton->setAnchorPoint(Vec2(0.5, 0.5));
		pButton->setTouchEndedCallBack([pQRCodeNode](SASelectedButton*, Touch*, Event*) 
		{
			g_iPushCount++;
			pQRCodeNode->reload(to_string(g_iPushCount));
		});
		m_pQRCodeTest->addChild(pButton);
	}
#endif

	m_pQRCodeTest->setVisible(false);
	addChild(m_pQRCodeTest);
}

void HelloWorld::createComboBoxTest()
{
	auto g_sizeVisible = Director::getInstance()->getVisibleSize();

	m_pComboBoxTest = SANode::Create::create();

	{
		SASlider* pSlider = SASlider::CreateWithAllButton::create(
			g_sizeSlider,
			SASlider::VERTICAL,
			createButtonWithLabelAndDefaultCallBack(g_sizeButton, "B"),
			createButtonWithLabelAndDefaultCallBack(g_sizeButton, "H"),
			createButtonWithLabelAndDefaultCallBack(g_sizeButton, "T")
		);
		pSlider->setPosition(g_vec2TableView.x + g_sizeTableView.width, g_vec2TableView.y);
		m_pComboBoxTest->addChild(pSlider);

		SASliderTableView* pTableView = SASliderTableView::CreateWithTag::create(this, g_sizeTableView, pSlider, TABLEVIEW_COMBO_TAG);
		pTableView->setPosition(g_vec2TableView);
		m_pComboBoxTest->addChild(pTableView);

		Size sizeCombo = Size(g_sizeButton.width + g_sizeTableView.width, g_sizeCell.height);

		Scale9Sprite* pS9Unselected = Scale9Sprite::create("blackRect.png");
		pS9Unselected->setContentSize(sizeCombo);
		Sprite* pDown = SAUtils::createWithFileConfigAndSize(g_sizeButton, "down.png", LOAD_TEXTURE_FROM_LOCAL | SPR_TYPE_NOMAL);
		pDown->setAnchorPoint(Vec2(1, 0.5));
		pDown->setPosition(sizeCombo.width, sizeCombo.height / 2);
		pS9Unselected->addChild(pDown);

		Scale9Sprite* pS9Selected = Scale9Sprite::create("blackRect.png");
		pS9Selected->setContentSize(sizeCombo);
		Sprite* pUp = SAUtils::createWithFileConfigAndSize(g_sizeButton, "up.png", LOAD_TEXTURE_FROM_LOCAL | SPR_TYPE_NOMAL);
		pUp->setAnchorPoint(Vec2(1, 0.5));
		pUp->setPosition(sizeCombo.width, sizeCombo.height / 2);
		pS9Selected->addChild(pUp);

		Label* pLabel = Label::createWithTTF("", DEFAULT_FONT_NAME, 20);
		pLabel->setColor(Color3B(0x2C, 0x2C, 0x2C));
		pLabel->setPosition(sizeCombo.width / 2, sizeCombo.height / 2);

		SAComboBox* pCombo = SAComboBox::CreateWithNode::create(sizeCombo, pS9Unselected);
		pCombo->addChild(pLabel);
		pCombo->setSelectedNormal(pS9Selected);
		pCombo->setTableView(pTableView);
		pCombo->setPosition(Vec2(g_vec2TableView.x, g_vec2TableView.y + g_sizeTableView.height));
		pCombo->setComboBoxCallBack([pLabel](SAComboBox*, TableViewCell* pCell) 
		{ 
			Label* pCellLabel = dynamic_cast<Label*>(pCell->getChildByTag(TABLEVIEW_LABEL_TAG));
			if (pCellLabel)
			{
				pLabel->setString(pCellLabel->getString());
			}
			return true;
		});

		m_pComboBoxTest->addChild(pCombo);
	}

	m_pComboBoxTest->setVisibleAndRespondEvent(false);
	addChild(m_pComboBoxTest);
}

void HelloWorld::createGLViewImplTest()
{
	auto g_sizeVisible = Director::getInstance()->getVisibleSize();

	m_pGLViewImplTest = SANode::Create::create();

	{
		m_pLabel1 = Label::createWithTTF("Current X : unknow", DEFAULT_FONT_NAME, 30);
		m_pLabel1->setColor(Color3B(0x2C, 0x2C, 0x2C));
		m_pLabel1->setPosition(g_sizeVisible.width / 2, g_sizeVisible.height / 2);
		m_pGLViewImplTest->addChild(m_pLabel1);

		m_pLabel2 = Label::createWithTTF("Current Y : unknow", DEFAULT_FONT_NAME, 30);
		m_pLabel2->setColor(Color3B(0x2C, 0x2C, 0x2C));
		m_pLabel2->setPosition(g_sizeVisible.width / 2, g_sizeVisible.height / 4);
		m_pGLViewImplTest->addChild(m_pLabel2);
	}

	m_pGLViewImplTest->setVisibleAndRespondEvent(false);
	addChild(m_pGLViewImplTest);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}







//tableview datasource
TableViewCell* HelloWorld::tableCellAtIndex(TableView* pTable, ssize_t idx)
{
	string strText;
	switch (pTable->getTag())
	{
	case TABLEVIEW_LEFT_TAG:
		strText = "L : " + to_string(idx);
		break;
	case TABLEVIEW_RIGHT_TAG:
		strText = "R : " + to_string(idx);
		break;
	default:
		strText =  "ComboBox : " + to_string(idx);
		break;
	}

	TableViewCell* pCell = pTable->dequeueCell();
	if (pCell)
	{
		Label* pLabel = dynamic_cast<Label*>(pCell->getChildByTag(TABLEVIEW_LABEL_TAG));
		if (pLabel)
		{
			pLabel->setString(strText);
		}
	}
	else
	{
		pCell = TableViewCell::create();
		Scale9Sprite* pS9Spr = Scale9Sprite::create("blackRect.png");
		pS9Spr->setContentSize(g_sizeCell);
		pS9Spr->setPosition(Vec2(g_sizeCell.width / 2, g_sizeCell.height / 2));
		pCell->addChild(pS9Spr);

		Label* pLabel = Label::createWithTTF(strText, DEFAULT_FONT_NAME, 20);
		pLabel->setColor(Color3B(0x2C, 0x2C, 0x2C));
		pLabel->setTag(TABLEVIEW_LABEL_TAG);
		pLabel->setPosition(Vec2(g_sizeCell.width / 2, g_sizeCell.height / 2));
		pCell->addChild(pLabel);
	}
	return pCell;
}

Size HelloWorld::cellSizeForTable(TableView* table)
{
	return g_sizeCell;
}

ssize_t HelloWorld::numberOfCellsInTableView(TableView *table)
{
	return 50;
}






void HelloWorld::askClose()
{
	m_pMessageBox->setVisibleAndRespondEvent(true);
}

void HelloWorld::updatePosition(int x, int y)
{
	__String* pStr1 = __String::createWithFormat("Current X : %d", x);
	m_pLabel1->setString(pStr1->getCString());
	__String* pStr2 = __String::createWithFormat("Current Y : %d", y);
	m_pLabel2->setString(pStr2->getCString());
}
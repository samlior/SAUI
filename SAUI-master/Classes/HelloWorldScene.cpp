/****************************************************************************
 Copyright (c) DEFAULT_FONT_SIZE17-DEFAULT_FONT_SIZE18 Xiamen Yaji Software Co., Ltd.
 
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
#define DEFAULT_FONT_SIZE		15

#define TABLEVIEW_LABEL_TAG		101
#define TABLEVIEW_LEFT_TAG		102
#define TABLEVIEW_RIGHT_TAG		103
#define TABLEVIEW_COMBO_TAG		104

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;

static Vec2 g_vec2TableView;
static Size g_sizeVisible;
static Size g_sizeTableView;
static Size g_sizeCell;
static Size g_sizeSlider;
static Size g_sizeButton;
static Size g_sizeSelectedButton;
static Size g_sizeSheetElement;
static Size g_sizeTitleButton;

static SAButton* createButton(const Size& size)
{
	SAButton* pButton = SAButton::CreateWithFile::create(size, "s9b.png", LOAD_TEXTURE_FROM_LOCAL | SPR_TYPE_SCALE9);
	pButton->setUnselectedHighLight("s9.png", LOAD_TEXTURE_FROM_LOCAL | SPR_TYPE_SCALE9);
	return pButton;
}

static SAButton* createButtonWithLabel(const Size& size, const string& strText)
{
	SAButton* pButton = createButton(size);
	Label* pLabel = Label::createWithTTF(strText, DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE);
	pLabel->setPosition(size.width / 2, size.height / 2);
	pButton->addChild(pLabel);
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
	Scale9Sprite* pS90 = "s9b.png"_2s9spr;
	Label* pLabel0 = Label::createWithTTF(strText, DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE, Size::ZERO, TextHAlignment::CENTER);
	pLabel0->setPosition(size.width / 2, size.height / 2);
	pS90->addChild(pLabel0);

	Scale9Sprite* pS91 = "s9hb.png"_2s9spr;
	Label* pLabel1 = Label::createWithTTF(strText, DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE, Size::ZERO, TextHAlignment::CENTER);
	pLabel1->setColor(Color3B(0x2C, 0x2C, 0x2C));
	pLabel1->setPosition(size.width / 2, size.height / 2);
	pS91->addChild(pLabel1);

	Scale9Sprite* pS92 = "s9.png"_2s9spr;
	Label* pLabel2 = Label::createWithTTF(strText, DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE, Size::ZERO, TextHAlignment::CENTER);
	pLabel2->setPosition(size.width / 2, size.height / 2);
	pS92->addChild(pLabel2);

	Scale9Sprite* pS93 = "s9h.png"_2s9spr;
	Label* pLabel3 = Label::createWithTTF(strText, DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE, Size::ZERO, TextHAlignment::CENTER);
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

	//计算布局
    g_sizeVisible = Director::getInstance()->getVisibleSize();
	g_vec2TableView = Vec2(g_sizeVisible.width / 13, g_sizeVisible.height / 7);
	g_sizeTableView = Size(g_sizeVisible.width / 13 * 4, g_sizeVisible.height / 7 * 4);
	g_sizeCell = Size(g_sizeTableView.width, g_sizeTableView.height / 10);
	g_sizeSlider = Size(g_sizeTableView.width / 10, g_sizeTableView.height);
	g_sizeButton = Size(g_sizeTableView.width / 10, g_sizeTableView.height / 10);
	g_sizeSelectedButton = Size(g_sizeVisible.width / 13 * 2.5, g_sizeVisible.height / 7);
	g_sizeSheetElement = Size(g_sizeVisible.width / 13 * 2, g_sizeVisible.width / 13 * 2);
	g_sizeTitleButton = Size(g_sizeVisible.width / 13 * 1.55, g_sizeVisible.height / 14);

	//关闭菜单
    auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this)
	);
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	//设置背景
	LayerColor* pBackground = LayerColor::create(Color4B::WHITE, g_sizeVisible.width, g_sizeVisible.height);
	addChild(pBackground);

	//创建节点
	map<string, SANode*> mapTitleNode =
	{
		{ "SATableView",			createTableViewTest() },
		{ "SASelectedButton",		createSelectedButtonTest() },
		{ "SASheet&\nSAAdaptedBox", createSheetTest() },
		{ "SAMessageBox",			createMessageBoxTest() },
		{ "SAQRCode",				createQRCodeTest() },
		{ "SAComboBox",				createComboBoxTest() },
		{ "SAGLViewImpl",			createGLViewImplTest() },
	};

	//创建按钮组
	m_pGroup = SAButtonGroup::Create::create();
	for (auto itr = mapTitleNode.begin(); itr != mapTitleNode.end(); ++itr)
	{
		//创建控制按钮,控制子节点显示
		SAControlButton* pButton = createSelectedButtonWithLabel<SAControlButton>(g_sizeTitleButton, itr->first);
		unsigned int uiIndex = m_pGroup->add(pButton);

		pButton->setAnchorPoint(Vec2(0, 1));
		pButton->setPosition(Vec2(uiIndex * (g_sizeTitleButton.width + DEFAULT_FONT_SIZE), g_sizeVisible.height));
		pButton->setNode(itr->second);
		addChild(pButton);
	}
    return true;
}

SANode* HelloWorld::createTableViewTest()
{
	SANode* pTableViewTest = SANode::Create::create();

	{
		//创建滚动条
		SASlider* pSlider = SASlider::CreateWithAllButton::create(
			//滚动条尺寸
			g_sizeSlider,
			//滚动条方向
			SASlider::VERTICAL,
			//滑动按钮
			createButtonWithLabel(g_sizeButton, "LB"),
			//向上移动按钮
			createButtonWithLabel(g_sizeButton, "LH"),
			//向下移动按钮
			createButtonWithLabel(g_sizeButton, "LT")
		);
		//设置头部纹理
		pSlider->setHeaderTexture("texturey.png"_2spr);
		pSlider->setPosition(g_vec2TableView.x + g_sizeTableView.width, g_vec2TableView.y);
		pTableViewTest->addChild(pSlider);

		//以slider初始化tableview
		SASliderTableView* pTableView = SASliderTableView::CreateWithTag::create(this, g_sizeTableView, pSlider, TABLEVIEW_LEFT_TAG);
		pTableView->setPosition(g_vec2TableView);
		pTableView->setContentOffset(Vec2::ZERO);
		pTableViewTest->addChild(pTableView);
	}

	{
		SASlider* pSlider = SASlider::CreateWithBlocker::create(
			g_sizeSlider,
			SASlider::VERTICAL,
			createButtonWithLabel(g_sizeButton, "RB")
		);
		//设置背景
		pSlider->setBackground("texturey.png"_2s9spr);
		pSlider->setPosition(g_sizeVisible.width - g_vec2TableView.x - g_sizeTableView.width - g_sizeSlider.width, g_vec2TableView.y);
		pTableViewTest->addChild(pSlider);

		SASliderTableView* pTableView = SASliderTableView::CreateWithTag::create(this, g_sizeTableView, pSlider, TABLEVIEW_RIGHT_TAG);
		pTableView->setPosition(Vec2(g_sizeVisible.width - g_vec2TableView.x - g_sizeTableView.width, g_vec2TableView.y));
		pTableView->setContentOffset(Vec2::ZERO);
		pTableViewTest->addChild(pTableView);
	}

	//使子节点隐藏并停止响应事件
	pTableViewTest->setVisibleAndRespondEvent(false);
	addChild(pTableViewTest);
	return pTableViewTest;
}

SANode* HelloWorld::createSelectedButtonTest()
{
	static float g_fSpaceX = (g_sizeVisible.width - g_sizeSelectedButton.width * 4) / 5;

	SANode* pSelectedButtonTest = SANode::Create::create();

	//创建流程表
	{
		//创建列表
		SASheet* pSheet = SASheet::CreateWithSpace::create(
			//单元格的尺寸
			g_sizeSelectedButton, 
			//列表尺寸(4列1行)
			SASize(4, 1), 
			//横向间隔
			g_fSpaceX,
			//纵向间隔
			0
		);
		Scale9Sprite* pS90 = "s9b.png"_2s9spr;
		pS90->setContentSize(g_sizeSelectedButton);
		Label* pLabel0 = Label::createWithTTF("UnselectedNormal", DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE);
		pLabel0->setPosition(g_sizeSelectedButton.width / 2, g_sizeSelectedButton.height / 2);
		pS90->addChild(pLabel0);

		Scale9Sprite* pS91 = "s9hb.png"_2s9spr;
		pS91->setContentSize(g_sizeSelectedButton);
		Label* pLabel1 = Label::createWithTTF("UnselectedHighLight", DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE);
		pLabel1->setColor(Color3B(0x2C, 0x2C, 0x2C));
		pLabel1->setPosition(g_sizeSelectedButton.width / 2, g_sizeSelectedButton.height / 2);
		pS91->addChild(pLabel1);

		Scale9Sprite* pS92 = "s9.png"_2s9spr;
		pS92->setContentSize(g_sizeSelectedButton);
		Label* pLabel2 = Label::createWithTTF("SelectedNormal", DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE);
		pLabel2->setPosition(g_sizeSelectedButton.width / 2, g_sizeSelectedButton.height / 2);
		pS92->addChild(pLabel2);

		Scale9Sprite* pS93 = "s9h.png"_2s9spr;
		pS93->setContentSize(g_sizeSelectedButton);
		Label* pLabel3 = Label::createWithTTF("SelectedHighLight", DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE);
		pLabel3->setColor(Color3B(0xF1, 0x48, 0x45));
		pLabel3->setPosition(g_sizeSelectedButton.width / 2, g_sizeSelectedButton.height / 2);
		pS93->addChild(pLabel3);

		//将单元格依次加入列表
		vector<Node*> vecTotalElement = { pS90, pS91, pS92, pS93 };
		pSheet->append(vecTotalElement);
		pSheet->setPosition(g_fSpaceX, g_sizeVisible.height / 2);
		pSelectedButtonTest->addChild(pSheet);
	}

	//创建箭头
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
		pSheet->setPosition(g_fSpaceX + g_sizeSelectedButton.width, g_sizeVisible.height / 2);
		pSelectedButtonTest->addChild(pSheet);
	}

	//创建按钮
	{
		SASelectedButton* pButton = createSelectedButtonWithLabel(g_sizeSelectedButton, "SelectedButton");
		pButton->setAnchorPoint(Vec2(0.5, 0.5));
		pButton->setPosition(g_sizeVisible.width / 2, g_sizeVisible.height / 4);
		pSelectedButtonTest->addChild(pButton);
	}

	pSelectedButtonTest->setVisibleAndRespondEvent(false);
	addChild(pSelectedButtonTest);
	return pSelectedButtonTest;
}



//单元格类
class SheetElement : public Node
{
public:
	using CreateWithSizeAndIndex = SACreator<true, SheetElement, const Size&, int>;

	SheetElement() :m_pS9HighLight(nullptr) {}

	//根据子类和下标初始化
	bool init(const Size& size, int index)
	{
		if (!Node::init())
			return false;

		setContentSize(size);

		Scale9Sprite* pS9 = "s9hb.png"_2s9spr;
		pS9->setContentSize(size);
		pS9->setAnchorPoint(Vec2::ZERO);
		addChild(pS9);

		Label* pLabel = Label::createWithTTF(to_string(index), DEFAULT_FONT_NAME, 30);
		pLabel->setColor(Color3B(0x2C, 0x2C, 0x2C));
		pLabel->setPosition(size.width / 2, size.height / 2);
		addChild(pLabel);

		m_pS9HighLight = "s9h.png"_2s9spr;
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

SANode* HelloWorld::createSheetTest()
{
	SANode* pSheetTest = SANode::Create::create();

	//创建列表
	{
		//创建自适应框
		SAAdaptedBox* pAdpBox = SAAdaptedBox::CreateWithBackground::create("s9w.png"_2s9spr);
		pAdpBox->setVisible(false);
		//设置偏移尺寸(自适应框的最总尺寸=所有子节点的尺寸之和+偏移尺寸)
		pAdpBox->setOffsetSize(Size(10, 10));
		//开始监听鼠标移动事件
		pAdpBox->startMouseMoveListen();

		//pAdpBox->setAdaptMode(false);
		//pAdpBox->setForceMode(false);

		//std::vector<SAAdaptedBox::AdaptDirection> vecPriority = { SAAdaptedBox::RIGHT_BOTTOM, SAAdaptedBox::RIGHT_TOP };
		//pAdpBox->setDirectionPriority(vecPriority);

		//向自适应框中添加子节点
		{
			float fLabelOffset = g_sizeSheetElement.width;

			Label* pLable1 = Label::createWithTTF("LEFT_BOTTOM", DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE);
			pLable1->setColor(Color3B(0x3C, 0x3C, 0x3C));
			pLable1->setAnchorPoint(Vec2::ZERO);
			pLable1->setPosition(-fLabelOffset, -fLabelOffset);
			pAdpBox->addChild(pLable1);

			Label* pLable2 = Label::createWithTTF("LEFT_TOP", DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE);
			pLable2->setColor(Color3B(0x3C, 0x3C, 0x3C));
			pLable2->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
			pLable2->setPosition(-fLabelOffset, fLabelOffset);
			pAdpBox->addChild(pLable2);

			Label* pLable3 = Label::createWithTTF("RIGHT_BOTTOM", DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE);
			pLable3->setColor(Color3B(0x3C, 0x3C, 0x3C));
			pLable3->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
			pLable3->setPosition(fLabelOffset, -fLabelOffset);
			pAdpBox->addChild(pLable3);

			Label* pLable4 = Label::createWithTTF("RIGHT_TOP", DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE);
			pLable4->setColor(Color3B(0x3C, 0x3C, 0x3C));
			pLable4->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
			pLable4->setPosition(fLabelOffset, fLabelOffset);
			pAdpBox->addChild(pLable4);
		}

		pSheetTest->addChild(pAdpBox, 10);


		//创建表格
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
			//如果上一个鼠标所在的单元格在范围内,则取消高亮状态
			if (pointA != SASheet::pointOutOfRange)
			{
				SheetElement* pElement = dynamic_cast<SheetElement*>(pSheet->at(pointA));
				if (pElement)
				{
					pElement->normal();
				}
			}

			//如果当前鼠标所在的单元格在范围内,则变为高亮状态,并显示自适应框
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
		pSheetTest->addChild(pSheet);
	}

	pSheetTest->setVisibleAndRespondEvent(false);
	addChild(pSheetTest);
	return pSheetTest;
}

SANode* HelloWorld::createMessageBoxTest()
{
	SANode* pMessageBoxTest = SANode::Create::create();

	{
		//计算Box尺寸
		Size sizeMessageBox = Size(g_sizeVisible.width / 2, g_sizeVisible.height / 3);

		//根据对话框背景初始化Box
		m_pMessageBox = SAMessageBox::CreateWithS9Sprite::create(sizeMessageBox, "s9w.png"_2s9spr);
		//开始监听键盘事件(默认按ESC关闭对话框)
		m_pMessageBox->startKeyboardListen();
		//开始监听触摸事件(点击对话框外部分关闭对话框)
		m_pMessageBox->startTouchListen(true, true);

		//创建关闭按钮
		SAButton* pButton = SAButton::CreateWithFile::create(g_sizeButton, "close.png", LOAD_TEXTURE_FROM_LOCAL | SPR_TYPE_NOMAL);
		pButton->setAnchorPoint(Vec2(1, 1));
		m_pMessageBox->setCloseButton(pButton, Vec2(sizeMessageBox.width - 10, sizeMessageBox.height - 10));

		Size sizeButton = Size(sizeMessageBox.width / 6, sizeMessageBox.height / 4);

		vector<SAButton*> vecButton = {
			createButtonWithLabel(sizeButton, "No"),
			createButtonWithLabel(sizeButton, "Yes")
		};
		//设置标题
		m_pMessageBox->setTitle("Warnning", DEFAULT_FONT_NAME, 25, Color3B(0x2C, 0x2C, 0x2C), Vec2(sizeMessageBox.width / 2, sizeMessageBox.height / 4 * 3));
		//设置内容(内容默认在正中间)
		m_pMessageBox->setContent("Are you sure you want to close?", DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE, Color3B(0x2C, 0x2C, 0x2C));
		//设置按钮
		m_pMessageBox->setButton(vecButton, sizeMessageBox.height / 4);
		//设置背景
		m_pMessageBox->setBackground(g_sizeVisible);
		//隐藏并停止响应事件
		m_pMessageBox->setVisibleAndRespondEvent(false);
		//设置回调
		m_pMessageBox->setCallBack([this](SAMessageBox*, unsigned int uiIndex, SASelectedButton*) 
		{ 
			if (uiIndex)
			{
				Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				exit(0);
#endif
			}
			return true;
		});

//		custom message box
// 		Node* pDialogNode = m_pMessageBox->getMessageBoxNode();
// 		pDialogNode->addChild(...);

		addChild(m_pMessageBox, 10);
	}

	//创建show box按钮
	{
		SAButton* pButton = createButtonWithLabel(g_sizeSelectedButton, "show box");
		pButton->setPosition(g_sizeVisible.width / 2, g_sizeVisible.height / 2);
		pButton->setAnchorPoint(Vec2(0.5, 0.5));
		pButton->setTouchEndedCallBack([this](SASelectedButton*, Touch*, Event*) 
		{
			this->m_pMessageBox->setVisibleAndRespondEvent(true);
		});
		pMessageBoxTest->addChild(pButton);
	}

	pMessageBoxTest->setVisibleAndRespondEvent(false);
	addChild(pMessageBoxTest);
	return pMessageBoxTest;
}

SANode* HelloWorld::createQRCodeTest()
{
	SANode* pQRCodeTest = SANode::Create::create();

#ifdef SA_USE_QRCODE
	{
		static int g_iPushCount = 0;

		SAQRCodeNode* pQRCodeNode = SAQRCodeNode::CreateWithText::create(g_sizeSheetElement, to_string(g_iPushCount));
		pQRCodeNode->setPosition(g_sizeVisible.width / 2, g_sizeVisible.height / 2);
		pQRCodeNode->setAnchorPoint(Vec2(0.5, 0.5));
		pQRCodeTest->addChild(pQRCodeNode);

		SAButton* pButton = createButtonWithLabel(g_sizeSelectedButton, "update qrcode");
		pButton->setPosition(g_sizeVisible.width / 2, g_sizeVisible.height / 4);
		pButton->setAnchorPoint(Vec2(0.5, 0.5));
		pButton->setTouchEndedCallBack([pQRCodeNode](SASelectedButton*, Touch*, Event*) 
		{
			g_iPushCount++;
			pQRCodeNode->reload(to_string(g_iPushCount));
		});
		pQRCodeTest->addChild(pButton);
	}
#endif

	pQRCodeTest->setVisible(false);
	addChild(pQRCodeTest);
	return pQRCodeTest;
}

SANode* HelloWorld::createComboBoxTest()
{
	SANode* pComboBoxTest = SANode::Create::create();

	//创建下拉菜单
	{
		SASlider* pSlider = SASlider::CreateWithAllButton::create(
			g_sizeSlider,
			SASlider::VERTICAL,
			createButtonWithLabel(g_sizeButton, "B"),
			createButtonWithLabel(g_sizeButton, "H"),
			createButtonWithLabel(g_sizeButton, "T")
		);
		pSlider->setPosition(g_vec2TableView.x + g_sizeTableView.width, g_vec2TableView.y);
		pComboBoxTest->addChild(pSlider);

		SASliderTableView* pTableView = SASliderTableView::CreateWithTag::create(this, g_sizeTableView, pSlider, TABLEVIEW_COMBO_TAG);
		pTableView->setPosition(g_vec2TableView);
		pComboBoxTest->addChild(pTableView);

		Size sizeCombo = Size(g_sizeButton.width + g_sizeTableView.width, g_sizeCell.height);

		Scale9Sprite* pS9Unselected ="blackRect.png"_2s9spr;
		pS9Unselected->setContentSize(sizeCombo);
		Sprite* pDown = SAUtils::createWithFileConfigAndSize(g_sizeButton, "down.png", LOAD_TEXTURE_FROM_LOCAL | SPR_TYPE_NOMAL);
		pDown->setAnchorPoint(Vec2(1, 0.5));
		pDown->setPosition(sizeCombo.width, sizeCombo.height / 2);
		pS9Unselected->addChild(pDown);

		Scale9Sprite* pS9Selected = "blackRect.png"_2s9spr;
		pS9Selected->setContentSize(sizeCombo);
		Sprite* pUp = SAUtils::createWithFileConfigAndSize(g_sizeButton, "up.png", LOAD_TEXTURE_FROM_LOCAL | SPR_TYPE_NOMAL);
		pUp->setAnchorPoint(Vec2(1, 0.5));
		pUp->setPosition(sizeCombo.width, sizeCombo.height / 2);
		pS9Selected->addChild(pUp);

		Label* pLabel = Label::createWithTTF("", DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE);
		pLabel->setColor(Color3B(0x2C, 0x2C, 0x2C));
		pLabel->setPosition(sizeCombo.width / 2, sizeCombo.height / 2);


		//创建下拉菜单
		SAComboBox* pCombo = SAComboBox::CreateWithNode::create(sizeCombo, pS9Unselected);
		pCombo->addChild(pLabel);
		pCombo->setSelectedNormal(pS9Selected);
		//将tableview与下拉菜单绑定
		pCombo->setTableView(pTableView);
		pCombo->setPosition(Vec2(g_vec2TableView.x, g_vec2TableView.y + g_sizeTableView.height));
		//设置回调
		pCombo->setComboBoxCallBack([pLabel](SAComboBox*, TableViewCell* pCell) 
		{ 
			Label* pCellLabel = dynamic_cast<Label*>(pCell->getChildByTag(TABLEVIEW_LABEL_TAG));
			if (pCellLabel)
			{
				pLabel->setString(pCellLabel->getString());
			}
			return true;
		});

		pComboBoxTest->addChild(pCombo);
	}

	pComboBoxTest->setVisibleAndRespondEvent(false);
	addChild(pComboBoxTest);
	return pComboBoxTest;
}

SANode* HelloWorld::createGLViewImplTest()
{
	SANode* pGLViewImplTest = SANode::Create::create();

	//创建label,事实更新窗口位置
	{
		m_pLabel1 = Label::createWithTTF("Current X : unknow", DEFAULT_FONT_NAME, 30);
		m_pLabel1->setColor(Color3B(0x2C, 0x2C, 0x2C));
		m_pLabel1->setPosition(g_sizeVisible.width / 2, g_sizeVisible.height / 2);
		pGLViewImplTest->addChild(m_pLabel1);

		m_pLabel2 = Label::createWithTTF("Current Y : unknow", DEFAULT_FONT_NAME, 30);
		m_pLabel2->setColor(Color3B(0x2C, 0x2C, 0x2C));
		m_pLabel2->setPosition(g_sizeVisible.width / 2, g_sizeVisible.height / 4);
		pGLViewImplTest->addChild(m_pLabel2);
	}

	pGLViewImplTest->setVisibleAndRespondEvent(false);
	addChild(pGLViewImplTest);
	return pGLViewImplTest;
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
		Scale9Sprite* pS9Spr = "blackRect.png"_2s9spr;
		pS9Spr->setContentSize(g_sizeCell);
		pS9Spr->setPosition(Vec2(g_sizeCell.width / 2, g_sizeCell.height / 2));
		pCell->addChild(pS9Spr);

		Label* pLabel = Label::createWithTTF(strText, DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE);
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
#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SA/SAHeader.h"
#include "SA/SACreator.h"
#include "SAUI/SANode.h"

class SASelectedButton;

using SAButtonBoolCallBack = std::function<bool(SASelectedButton*, cocos2d::Touch*, cocos2d::Event*)>;
using SAButtonCallBack = std::function<void(SASelectedButton*, cocos2d::Touch*, cocos2d::Event*)>;

//有选中、未选中状态的按钮
class SASelectedButton : public SANode
{
public:
	SASelectedButton();
	virtual ~SASelectedButton();

	using CreateWithFile = SACreator<true, SASelectedButton, const cocos2d::Size&, const std::string&, int>;
	using CreateWithNode = SACreator<true, SASelectedButton, const cocos2d::Size&, cocos2d::Node*>;
	using CreateWithS9Sprite = SACreator<true, SASelectedButton, const cocos2d::Size&, cocos2d::extension::Scale9Sprite*>;

	//设置选中状态普通精灵
	void setSelectedNormal(const std::string& strFileName, int iConfig);
	void setSelectedNormal(cocos2d::Node* pNode);
	void setSelectedNormal(cocos2d::extension::Scale9Sprite* pSprite);

	//设置选中状态高亮精灵
	void setSelectedHighLight(const std::string& strFileName, int iConfig);
	void setSelectedHighLight(cocos2d::Node* pNode);
	void setSelectedHighLight(cocos2d::extension::Scale9Sprite* pSprite);

	//设置未选中状态高亮精灵
	void setUnselectedHighLight(const std::string& strFileName, int iConfig);
	void setUnselectedHighLight(cocos2d::Node* pNode);
	void setUnselectedHighLight(cocos2d::extension::Scale9Sprite* pSprite);

	//设置禁用状态精灵
	void setDisabled(const std::string& strFileName, int iConfig);
	void setDisabled(cocos2d::Node* pNode);
	void setDisabled(cocos2d::extension::Scale9Sprite* pSprite);

	//设置是否选中
	virtual void setSelected(bool bIsSelected);
	bool isSelected() { return m_bIsSelected; }

	//设置是否启用
	virtual void setEnable(bool bEnable);
	bool isEnable() { return m_bEnable; }

	//touch listener call back
	virtual bool touchBeganCallBack(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void touchMovedCallBack(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	virtual void touchEndedCallBack(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

	//set button callback
	virtual void setTouchBeganCallBack(SAButtonBoolCallBack func) { m_funcOnTouchBegan = func; }
	virtual void setTouchMovedCallBack(SAButtonCallBack func) { m_funcOnTouchMoved = func; }
	virtual void setTouchEndedCallBack(SAButtonCallBack func) { m_funcOnTouchEnded = func; }

	//开始监听触摸事件,默认开启
	void startTouchListen(bool bSwallowTouches = true);
	//停止监听触摸事件
	void stopTouchListen();

	//根据未选中状态普通精灵初始化
	virtual bool init(
		const cocos2d::Size& size,
		const std::string& strFileName,
		int iConfig
	);

	virtual bool init(
		const cocos2d::Size& size,
		cocos2d::Node* pNode
	);

	virtual bool init(
		const cocos2d::Size& size,
		cocos2d::extension::Scale9Sprite* pSprite
	);

protected:
	void addChildNode(cocos2d::Node*& pNode, cocos2d::Node* pNodeOuter);

	virtual void initButton(const cocos2d::Size& size, cocos2d::Node* pNode);

	bool m_bIsSelected;
	bool m_bEnable;

	cocos2d::EventListenerTouchOneByOne* m_pListener;

	cocos2d::Node* m_pSelectedNormal;
	cocos2d::Node* m_pSelectedHighLight;
	cocos2d::Node* m_pUnselectedNormal;
	cocos2d::Node* m_pUnselectedHighLight;
	cocos2d::Node* m_pDisabled;

	SAButtonBoolCallBack m_funcOnTouchBegan;
	SAButtonCallBack m_funcOnTouchMoved;
	SAButtonCallBack m_funcOnTouchEnded;
};



//只有未选中状态的按钮
class SAButton : public SASelectedButton
{
public:
	using CreateWithFile = SACreator<true, SAButton, const cocos2d::Size&, const std::string&, int>;
	using CreateWithNode = SACreator<true, SAButton, const cocos2d::Size&, cocos2d::Node*>;
	using CreateWithS9Sprite = SACreator<true, SAButton, const cocos2d::Size&, cocos2d::extension::Scale9Sprite*>;

	virtual void setSelected(bool bIsSelected) override;

	virtual void setEnable(bool bEnable) override;

	virtual bool touchBeganCallBack(cocos2d::Touch* pTouch, cocos2d::Event* pEvent) override;
};

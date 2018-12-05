#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SA/SACreator.h"
#include "SAUI/SANode.h"

class SAButton;
class SASelectedButton;
class SAMessageBox;

using SAMessageBoxCallBack = std::function<bool(SAMessageBox*, unsigned int, SASelectedButton*)>;


//对话框
class SAMessageBox : public SANode
{
public:
	SAMessageBox();
	virtual ~SAMessageBox();

	using Create = SACreator<true, SAMessageBox, const cocos2d::Size&, cocos2d::Node*>;
	using CreateWithS9Sprite = SACreator<true, SAMessageBox, const cocos2d::Size&, cocos2d::extension::Scale9Sprite*>;

	virtual bool init(const cocos2d::Size& sizeDialog, cocos2d::extension::Scale9Sprite* pS9Spr);
	virtual bool init(const cocos2d::Size& sizeDialog, cocos2d::Node* pNode);

	//设置对话框背景
	void setBackground(const cocos2d::Size& sizeBackground, cocos2d::Node* pNode = nullptr);

	//增加对话框按钮
	void setButton(const std::vector<SAButton*>& vecButton, float fOffset);

	void setCallBack(SAMessageBoxCallBack func) { m_func = func; }

	
	/**
	* @name startTouchListen
	*
	* @brief							当开启监听点击事件后,在SAMessageBox以下的所有节点将不再接受点击事件
	*
	* @param bSwallowTouches			
	* @param bTouchDisappear			true-当用户点击在背景内但在对话框外的任意一点时,对话框消失 false-反之
	*/
	void startTouchListen(bool bSwallowTouches = true, bool bTouchDisappear = false);

	void setTouchDisappear(bool bTouchDisappear);

	//停止监听点击事件
	void stopTouchListen();


	//开始监听键盘事件,当用户按下指定的按键时,对话框消失
	void startKeyboardListen(cocos2d::EventKeyboard::KeyCode code = cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE);
	
	//停止监听键盘事件
	void stopKeyboardListen();


	//设置对话框标题
	void setTitle(const std::string& strTitle, const std::string& strFontName, float fFontSize, const cocos2d::Color3B& clrFont, const cocos2d::Vec2& vec2Position);
	void setTitle(cocos2d::Node* pTitle, const cocos2d::Vec2& vec2Position);

	//设置对话框内容
	void setContent(const std::string& strContent, const std::string& strFontName, float fFontSize, const cocos2d::Color3B& clrFont);
	void setContent(cocos2d::Node* pContent);

	//设置关闭按钮
	void setCloseButton(SAButton* pButton, const cocos2d::Vec2& vec2Position);


	//关闭按钮回调
	void closeTouchEndedCallBack(SASelectedButton* pButton, cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

	//按钮回调
	void touchEndedCallBack(SASelectedButton* pButton, cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

	//用户可通过此方式自定义对话框类型
	Node* getMessageBoxNode() { return m_pDialog; }

protected:
	bool m_bTouchDisapear;

	cocos2d::Node* m_pDialog;
	cocos2d::EventListenerKeyboard* m_pKeyboard;
	cocos2d::EventListenerTouchOneByOne* m_pTouch;
	SAMessageBoxCallBack m_func;
};


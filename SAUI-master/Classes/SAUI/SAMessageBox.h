#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SA/SACreator.h"
#include "SAUI/SANode.h"

class SAButton;
class SASelectedButton;
class SAMessageBox;

using SAMessageBoxCallBack = std::function<bool(SAMessageBox*, unsigned int, SASelectedButton*)>;


//�Ի���
class SAMessageBox : public SANode
{
public:
	SAMessageBox();
	virtual ~SAMessageBox();

	using Create = SACreator<true, SAMessageBox, const cocos2d::Size&, cocos2d::Node*>;
	using CreateWithS9Sprite = SACreator<true, SAMessageBox, const cocos2d::Size&, cocos2d::extension::Scale9Sprite*>;

	virtual bool init(const cocos2d::Size& sizeDialog, cocos2d::extension::Scale9Sprite* pS9Spr);
	virtual bool init(const cocos2d::Size& sizeDialog, cocos2d::Node* pNode);

	//���öԻ��򱳾�
	void setBackground(const cocos2d::Size& sizeBackground, cocos2d::Node* pNode = nullptr);

	//���ӶԻ���ť
	void setButton(const std::vector<SAButton*>& vecButton, float fOffset);

	void setCallBack(SAMessageBoxCallBack func) { m_func = func; }

	
	/**
	* @name startTouchListen
	*
	* @brief							��������������¼���,��SAMessageBox���µ����нڵ㽫���ٽ��ܵ���¼�
	*
	* @param bSwallowTouches			
	* @param bTouchDisappear			true-���û�����ڱ����ڵ��ڶԻ����������һ��ʱ,�Ի�����ʧ false-��֮
	*/
	void startTouchListen(bool bSwallowTouches = true, bool bTouchDisappear = false);

	void setTouchDisappear(bool bTouchDisappear);

	//ֹͣ��������¼�
	void stopTouchListen();


	//��ʼ���������¼�,���û�����ָ���İ���ʱ,�Ի�����ʧ
	void startKeyboardListen(cocos2d::EventKeyboard::KeyCode code = cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE);
	
	//ֹͣ���������¼�
	void stopKeyboardListen();


	//���öԻ������
	void setTitle(const std::string& strTitle, const std::string& strFontName, float fFontSize, const cocos2d::Color3B& clrFont, const cocos2d::Vec2& vec2Position);
	void setTitle(cocos2d::Node* pTitle, const cocos2d::Vec2& vec2Position);

	//���öԻ�������
	void setContent(const std::string& strContent, const std::string& strFontName, float fFontSize, const cocos2d::Color3B& clrFont);
	void setContent(cocos2d::Node* pContent);

	//���ùرհ�ť
	void setCloseButton(SAButton* pButton, const cocos2d::Vec2& vec2Position);


	//�رհ�ť�ص�
	void closeTouchEndedCallBack(SASelectedButton* pButton, cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

	//��ť�ص�
	void touchEndedCallBack(SASelectedButton* pButton, cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

	//�û���ͨ���˷�ʽ�Զ���Ի�������
	Node* getMessageBoxNode() { return m_pDialog; }

protected:
	bool m_bTouchDisapear;

	cocos2d::Node* m_pDialog;
	cocos2d::EventListenerKeyboard* m_pKeyboard;
	cocos2d::EventListenerTouchOneByOne* m_pTouch;
	SAMessageBoxCallBack m_func;
};


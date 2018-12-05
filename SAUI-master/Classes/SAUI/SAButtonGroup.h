#pragma once
#include "SAButton.h"
#include "SA/SACreator.h"

using SAButtonGroupCallBack = std::function<void(unsigned int, SASelectedButton*)>;


//���棺��Ҫ�ֶ������ڴ�,����release�ͷ�
class SAButtonGroup : public cocos2d::Ref
{
public:
	SAButtonGroup() :m_funcCallBack(nullptr), m_uiIndexSelected(0) {}
	~SAButtonGroup() {}

	using Create = SACreator<false, SAButtonGroup>;

	//���һ����ť����ť����
	unsigned int add(SASelectedButton* pBtn);

	//�����±�ı䰴ť״̬
	void setSelected(unsigned int uiIndex);
	//���õ���ص�
	void setCallBack(SAButtonGroupCallBack func) { m_funcCallBack = func; }

	//����ص�
	void touchEndedCallBack(SASelectedButton* pBtn, cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

	virtual bool init() { return true; }

protected:
	unsigned int m_uiIndexSelected;
	SAButtonGroupCallBack m_funcCallBack;
	std::vector<SASelectedButton*> m_vecAllBtn;
};


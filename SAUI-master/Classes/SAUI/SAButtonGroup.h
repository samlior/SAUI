#pragma once
#include "SAButton.h"
#include "SA/SACreator.h"

using SAButtonGroupCallBack = std::function<void(unsigned int, SASelectedButton*)>;


//警告：需要手动管理内存,调用release释放
class SAButtonGroup : public cocos2d::Ref
{
public:
	SAButtonGroup() :m_funcCallBack(nullptr), m_uiIndexSelected(0) {}
	~SAButtonGroup() {}

	using Create = SACreator<false, SAButtonGroup>;

	virtual bool init() { return true; }

	//添加一个按钮到按钮组中
	unsigned int add(SASelectedButton* pBtn);

	//根据下标改变按钮状态
	void setSelected(unsigned int uiIndex);
	//设置点击回调
	void setCallBack(SAButtonGroupCallBack func) { m_funcCallBack = func; }

	//点击回调
	bool touchBeganCallBack(SASelectedButton* pBtn, cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	void touchEndedCallBack(SASelectedButton* pBtn, cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

protected:
	unsigned int m_uiIndexSelected;
	SAButtonGroupCallBack m_funcCallBack;
	std::vector<SASelectedButton*> m_vecAllBtn;
};


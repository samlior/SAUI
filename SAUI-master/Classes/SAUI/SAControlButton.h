#pragma once
#include "cocos2d.h"
#include "SAButton.h"
#include "SA/SACreator.h"

class SAControlButton : public SASelectedButton
{
public:
	SAControlButton() :m_pNode(nullptr) {}

	using CreateWithFile = SACreator<true, SAControlButton, const cocos2d::Size&, const std::string&, int>;
	using CreateWithNode = SACreator<true, SAControlButton, const cocos2d::Size&, cocos2d::Node*>;
	using CreateWithS9Sprite = SACreator<true, SAControlButton, const cocos2d::Size&, cocos2d::extension::Scale9Sprite*>;

	virtual void setSelected(bool bIsSelected) override;

	void setNode(cocos2d::Node* pNode);

protected:
	cocos2d::Node* m_pNode;
};


class SATableView;
class SAComboBox;
using SAComboBoxCallBack = std::function<bool(SAComboBox*, cocos2d::extension::TableViewCell*)>;

//基于tableview的下拉框
class SAComboBox : public SAControlButton
{
public:
	SAComboBox() :m_pTableView(nullptr) {}

	using CreateWithFile = SACreator<true, SAComboBox, const cocos2d::Size&, const std::string&, int>;
	using CreateWithNode = SACreator<true, SAComboBox, const cocos2d::Size&, cocos2d::Node*>;
	using CreateWithS9Sprite = SACreator<true, SAComboBox, const cocos2d::Size&, cocos2d::extension::Scale9Sprite*>;

	virtual void setSelected(bool bIsSelected) override;

	virtual void cellTouchEndedCallBack(SATableView* pTableView, cocos2d::extension::TableViewCell* pCell);

	void setTableView(SATableView* pTableView);

	void setComboBoxCallBack(SAComboBoxCallBack func) { m_func = func; }

protected:
	SATableView* m_pTableView;

	SAComboBoxCallBack m_func;
};
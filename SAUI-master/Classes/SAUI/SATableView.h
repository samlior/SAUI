#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SA/SACreator.h"

class SAComboBox;
class SASlider;
class SAButton;
class SASelectedButton;
class SAOutSideButtonTableView;

class SATableView : public cocos2d::extension::TableView
{
public:
	SATableView() :m_bScrollable(true), m_bZoomScaleable(false) {}

	using Create = SACreator<true, SATableView, cocos2d::extension::TableViewDataSource*, const cocos2d::Size&>;
	using CreateWithTag = SACreator<true, SATableView, cocos2d::extension::TableViewDataSource*, const cocos2d::Size&, int>;

	//设置是否可以滚动
	void setScrollEnable(bool bScrollable) { m_bScrollable = bScrollable; }

	//设置是否可以缩放
	void setZoomScaleable(bool bZoomScaleable) { m_bZoomScaleable = bZoomScaleable; }

	//缩放事件回调
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view) override;
	//点击事件回调
	virtual void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) override;
	virtual void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) override;

	//获取内容视图尺寸
	const cocos2d::Size& getContainerSize();

	//不改变当前位置的情况下重新加载数据
	void reloadDataWithCurrentOffset();

	virtual bool init(cocos2d::extension::TableViewDataSource* dataSource, const cocos2d::Size& size, int iTag = -1);

protected:
	using SATableViewTouchEndedCallBack = std::function<void(SATableView*, cocos2d::extension::TableViewCell*)>;
	void setCellTouchEndedCallBack(SATableViewTouchEndedCallBack func) { m_func = func; }

	bool m_bScrollable;
	bool m_bZoomScaleable;

	SATableViewTouchEndedCallBack m_func;

	friend class SAComboBox;
};



//带滚动条的tableView
class SASliderTableView : public SATableView
{
public:
	SASliderTableView() :m_pSlider(nullptr) {}
	~SASliderTableView() {}

	using Create = SACreator<true, SASliderTableView, cocos2d::extension::TableViewDataSource*, const cocos2d::Size&, SASlider*>;
	using CreateWithTag = SACreator<true, SASliderTableView, cocos2d::extension::TableViewDataSource*, const cocos2d::Size&, SASlider*, int>;

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) override;

	virtual bool init(
		cocos2d::extension::TableViewDataSource* dataSource,
		const cocos2d::Size& size,
		SASlider* pSlider,
		int iTag = -1
	);

	bool sliderCallBack(SASlider* pSlider, float fOffsetOrg, float fOffsetNew);

	virtual void setVisible(bool b) override;

protected:

	void updateOffset();

	SASlider* m_pSlider;
};
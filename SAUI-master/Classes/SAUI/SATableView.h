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

	//�����Ƿ���Թ���
	void setScrollEnable(bool bScrollable) { m_bScrollable = bScrollable; }

	//�����Ƿ��������
	void setZoomScaleable(bool bZoomScaleable) { m_bZoomScaleable = bZoomScaleable; }

	//�����¼��ص�
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view) override;
	//����¼��ص�
	virtual void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) override;
	virtual void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) override;

	//��ȡ������ͼ�ߴ�
	const cocos2d::Size& getContainerSize();

	//���ı䵱ǰλ�õ���������¼�������
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



//����������tableView
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
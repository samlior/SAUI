#include "SATableView.h"
#include "SAButton.h"
#include "SASlider.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;



bool SATableView::init(TableViewDataSource* dataSource, const Size& size, int iTag /*= -1*/)
{
	if (!TableView::initWithViewSize(size, nullptr))
		return false;

	setTag(iTag);
	setBounceable(false);
	setDataSource(dataSource);
	_updateCellPositions();
	_updateContentSize();
	return true;
}


void SATableView::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	if (!m_bScrollable)
		return;

	TableView::onTouchMoved(pTouch, pEvent);
}

void SATableView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	if (!this->isVisible()) {
		return;
	}

	ScrollView::onTouchEnded(pTouch, pEvent);

	if (_touchedCell) {
		Rect bb = this->getBoundingBox();
		bb.origin = _parent->convertToWorldSpace(bb.origin);

		if (bb.containsPoint(pTouch->getLocation()) && _tableViewDelegate != nullptr)
		{
			_tableViewDelegate->tableCellUnhighlight(this, _touchedCell);
			_tableViewDelegate->tableCellTouched(this, _touchedCell);
		}

		//call combobox
		if (m_func)
		{
			m_func(this, _touchedCell);
		}

		_touchedCell = nullptr;
	}
}

void SATableView::scrollViewDidZoom(ScrollView* view)
{
	TableView::scrollViewDidZoom(view);

	if (!m_bZoomScaleable)
		view->setZoomScale(1.0);
}

const Size& SATableView::getContainerSize()
{
	return _container->getContentSize();
}

void SATableView::reloadDataWithCurrentOffset()
{
	float fh1 = getContainerSize().height;
	float fp1 = getContentOffset().y;
	TableView::reloadData();
	float fh2 = getContainerSize().height;
	setContentOffset(Vec2(0, -fh2 + (fh1 + fp1)));
}





bool SASliderTableView::init(
	TableViewDataSource* dataSource,
	const Size& size,
	SASlider* pSlider,
	int iTag /*= -1*/
)
{
	if (!SATableView::init(dataSource, size, iTag))
		return false;

	m_pSlider = pSlider;
	m_pSlider->setSliderCallBack(CC_CALLBACK_3(SASliderTableView::sliderCallBack, this));
	return true;
}

bool SASliderTableView::sliderCallBack(SASlider* pSlider, float fOffsetPercentOrg, float fOffsetPercentNew)
{
	float fTableHeight = _container->getContentSize().height - TableView::getViewSize().height;
	float fTableOffset = -(1 - fOffsetPercentNew) * fTableHeight;
	TableView::setContentOffset(Vec2(0, fTableOffset));

	const Vec2& minOffset = ScrollView::minContainerOffset();
	const Vec2& maxOffset = ScrollView::maxContainerOffset();
	return fTableOffset >= minOffset.y && fTableOffset <= maxOffset.y;
}

void SASliderTableView::updateOffset()
{
	if (m_pSlider)
	{
		float fTableHeight = _container->getContentSize().height - TableView::getViewSize().height;
		if (fTableHeight > 0)
		{
			float fCurrentOffset = -TableView::getContentOffset().y;
			m_pSlider->setContentOffsetPercent(1 - fCurrentOffset / fTableHeight, false);
		}
	}
}

void SASliderTableView::scrollViewDidScroll(ScrollView* view)
{
	SATableView::scrollViewDidScroll(view);

	//同步更新
	updateOffset();
}

void SASliderTableView::setVisible(bool b)
{
	Node::setVisible(b);
	m_pSlider->setVisible(b);
}
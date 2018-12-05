#pragma once
#include "cocos2d.h"
#include "SA/SACreator.h"
#include "SA/SAExternal.h"
#include "SAUI/SANode.h"

class SASheet;

//元素点击回调
using SASheetElementTouchedCallBack = std::function<void(SASheet*, const SAPoint&)>;

//鼠标移动回调
//如果point等于SASheet::pointOutOfRange 则代表移出表格或移入表格
using SASheetMouseMoveCallBack = std::function<void(SASheet*, const SAPoint&, const SAPoint&)>;



//表格
class SASheet : public SANode
{
public:
	SASheet() :
		m_size(0, 0),
		m_fSpaceLenX(0),
		m_fSpaceLenY(0),
		m_pMouseListener(nullptr),
		m_pTouchListener(nullptr),
		m_funcElementTouchedCallBack(nullptr),
		m_funcMouseMoveCallBack(nullptr) {}
	virtual ~SASheet();

	using Create = SACreator<true, SASheet, const cocos2d::Size&, const SASize&>;
	using CreateWithSpace = SACreator<true, SASheet, const cocos2d::Size&, const SASize&, float, float>;

	//获取元素个数
	size_t getElementCount();
	//为第uiRowIndex行增加一个元素
	void append(unsigned int uiRowIndex, cocos2d::Node* pNode);
	//为第uiRowIndex行增加vecNode里的元素
	void append(unsigned int uiRowIndex, const std::vector<cocos2d::Node*>& vecNode);
	//自适应填充元素直到填充满
	void append(const std::vector<cocos2d::Node*>& vecNode);
	//重新填充
	void reload(const std::vector<cocos2d::Node*>& vecNode);
	//清空
	void clear();

	//根据下标获取元素
	cocos2d::Node* at(const SAPoint& point);

	//坐标与下标转换
	SAPoint convertVec2ToSAPoint(const cocos2d::Vec2& vec2);
	cocos2d::Vec2 convertSAPointToVec2(const SAPoint& point);

	//开始监听触摸事件
	void startTouchListen(bool bSwallowTouches = true);
	//停止监听触摸事件
	void stopTouchListen();

	//开始监听鼠标移动事件
	void startMouseMoveListen();
	//停止监听鼠标移动事件
	void stopMouseMoveListen();

	//设置元素点击回调
	void setElementTouchedCallBack(SASheetElementTouchedCallBack func) { m_funcElementTouchedCallBack = func; }
	//设置鼠标移动回调
	void setMouseMoveCallBack(SASheetMouseMoveCallBack func) { m_funcMouseMoveCallBack = func; }

	//事件监听器回调
	void mouseMoveCallBack(cocos2d::EventMouse* pEvent);
	bool touchBeganCallBack(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	void touchEndedCallBack(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

	//范围外的点
	static const SAPoint pointOutOfRange;


	/**
	* @name init
	*
	* @param sizeElement		单个元素的尺寸
	* @param size				表格的尺寸
	* @param fSpaceLenX			横向间隔
	* @param fSpaceLenY			纵向间隔
	*/
	bool init(
		const cocos2d::Size& sizeElement,
		const SASize& size,
		float fSpaceLenX = 0,
		float fSpaceLenY = 0
	);

protected:
	void addChildNode(unsigned int uiX, unsigned int uiY, cocos2d::Node* pNode);

	SASize m_size;
	float m_fSpaceLenX;
	float m_fSpaceLenY;
	cocos2d::Size m_sizeElement;
	std::vector<std::vector<cocos2d::Node*>> m_vecVecNode;

	cocos2d::EventListenerTouchOneByOne* m_pTouchListener;
	cocos2d::EventListenerMouse* m_pMouseListener;

	SASheetElementTouchedCallBack m_funcElementTouchedCallBack;
	SASheetMouseMoveCallBack m_funcMouseMoveCallBack;
};


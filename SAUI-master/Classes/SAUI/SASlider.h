#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SA/SACreator.h"
#include "SAUI/SANode.h"

class SASlider;
class SAButton;
class SASelectedButton;

//滚动条回调,前一个输入参数为当前百分比,后一个为新的百分比
using SASliderCallBack = std::function<bool(SASlider*, float, float)>;

//滚动条
class SASlider : public SANode
{
public:
	SASlider();
	virtual ~SASlider();

	enum SliderDirection
	{
		HORIZONTAL,
		VERTICAL
	};

	using Create = SACreator<true, SASlider, const cocos2d::Size&, SliderDirection>;
	using CreateWithBlocker = SACreator<true, SASlider, const cocos2d::Size&, SliderDirection, SAButton*>;
	using CreateWithAllButton = SACreator<true, SASlider, const cocos2d::Size&, SliderDirection, SAButton*, SAButton*, SAButton*>;

	virtual bool init(const cocos2d::Size& size, SliderDirection direction, SAButton* pBlocker = nullptr, SAButton* pHeader = nullptr, SAButton* pTailer = nullptr);

	//开始监听鼠标事件
	void startListenMouseScroll();
	//停止监听鼠标事件
	void stopListenMouseScroll();

	void setSliderCallBack(SASliderCallBack func) { m_func = func; }

	//设置是否启用
	void setEnable(bool bEnable) { m_bEnable = bEnable; }
	bool isEnable() { return m_bEnable; }

	//设置滑块按钮
	void setBlocker(SAButton* pBlocker);

	//设置头部按钮
	void setHeader(SAButton* pHeader);

	//设置尾部按钮
	void setTailer(SAButton* pTailer);

	//设置头部纹理
	void setHeaderTexture(cocos2d::Sprite* pHeaderTexture);

	//设置尾部纹理
	void setTailerTexture(cocos2d::Sprite* pTailerTexture);

	//设置背景
	void setBackground(cocos2d::extension::Scale9Sprite* pS9Sprite);
	void setBackground(cocos2d::Node* pNode);

	//按钮回调
	void headerCallBack(SASelectedButton*, cocos2d::Touch*, cocos2d::Event*);
	void blockerMovedCallBack(SASelectedButton*, cocos2d::Touch*, cocos2d::Event*);
	void tailerCallBack(SASelectedButton*, cocos2d::Touch*, cocos2d::Event*);

	//设置单次偏移距离
	void setContentOffsetEachPushPercent(float fOffsetEachPushPercent) { m_fOffsetEachPushPercent = fOffsetEachPushPercent; }
	void setContentOffsetEachPush(float fOffsetEachPush);

	float getContentOffsetEachPushPercent() { return m_fOffsetEachPushPercent; }
	float getContentOffsetEachPush();


	//设置偏移距离
	virtual void setContentOffsetPercent(float fPercent, bool bCheckCallBack = true);
	virtual void setContentOffset(float fOffset, bool bCheckCallBack = true);

	float getContentOffsetPercent();
	float getContentOffset() { return m_fOffset; }

protected:
	//计算百分比
	float calculatePercent(float fOffset);

	//更新偏移范围
	virtual void updateOffsetRange();
	//更新滑块位置
	virtual void updateBlockerPosition(bool bDirectlyCall = false);
	//更新头部纹理尺寸
	virtual void updateHeaderTextureSize(bool bDirectlyCall = false);
	//更新尾部纹理尺寸
	virtual void updateTailerTextureSize(bool bDirectlyCall = false);

	bool m_bEnable;

	float m_fOffsetMax;
	float m_fOffsetMin;
	float m_fOffset;
	float m_fOffsetEachPushPercent;

	SliderDirection m_direction;

	SAButton* m_pHeader;
	SAButton* m_pBlocker;
	SAButton* m_pTailer;

	cocos2d::Sprite* m_pHeaderTexture;
	cocos2d::Sprite* m_pTailerTexture;

	cocos2d::EventListenerMouse* m_pListener;

	SASliderCallBack m_func;
};


#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SA/SACreator.h"
#include "SAUI/SANode.h"

class SASlider;
class SAButton;
class SASelectedButton;

//�������ص�,ǰһ���������Ϊ��ǰ�ٷֱ�,��һ��Ϊ�µİٷֱ�
using SASliderCallBack = std::function<bool(SASlider*, float, float)>;

//������
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

	//��ʼ��������¼�
	void startListenMouseScroll();
	//ֹͣ��������¼�
	void stopListenMouseScroll();

	void setSliderCallBack(SASliderCallBack func) { m_func = func; }

	//�����Ƿ�����
	void setEnable(bool bEnable) { m_bEnable = bEnable; }
	bool isEnable() { return m_bEnable; }

	//���û��鰴ť
	void setBlocker(SAButton* pBlocker);

	//����ͷ����ť
	void setHeader(SAButton* pHeader);

	//����β����ť
	void setTailer(SAButton* pTailer);

	//����ͷ������
	void setHeaderTexture(cocos2d::Sprite* pHeaderTexture);

	//����β������
	void setTailerTexture(cocos2d::Sprite* pTailerTexture);

	//���ñ���
	void setBackground(cocos2d::extension::Scale9Sprite* pS9Sprite);
	void setBackground(cocos2d::Node* pNode);

	//��ť�ص�
	void headerCallBack(SASelectedButton*, cocos2d::Touch*, cocos2d::Event*);
	void blockerMovedCallBack(SASelectedButton*, cocos2d::Touch*, cocos2d::Event*);
	void tailerCallBack(SASelectedButton*, cocos2d::Touch*, cocos2d::Event*);

	//���õ���ƫ�ƾ���
	void setContentOffsetEachPushPercent(float fOffsetEachPushPercent) { m_fOffsetEachPushPercent = fOffsetEachPushPercent; }
	void setContentOffsetEachPush(float fOffsetEachPush);

	float getContentOffsetEachPushPercent() { return m_fOffsetEachPushPercent; }
	float getContentOffsetEachPush();


	//����ƫ�ƾ���
	virtual void setContentOffsetPercent(float fPercent, bool bCheckCallBack = true);
	virtual void setContentOffset(float fOffset, bool bCheckCallBack = true);

	float getContentOffsetPercent();
	float getContentOffset() { return m_fOffset; }

protected:
	//����ٷֱ�
	float calculatePercent(float fOffset);

	//����ƫ�Ʒ�Χ
	virtual void updateOffsetRange();
	//���»���λ��
	virtual void updateBlockerPosition(bool bDirectlyCall = false);
	//����ͷ������ߴ�
	virtual void updateHeaderTextureSize(bool bDirectlyCall = false);
	//����β������ߴ�
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


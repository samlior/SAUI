#pragma once
#include "cocos2d.h"
#include "SA/SACreator.h"
#include "SA/SAExternal.h"
#include "SAUI/SANode.h"

class SASheet;

//Ԫ�ص���ص�
using SASheetElementTouchedCallBack = std::function<void(SASheet*, const SAPoint&)>;

//����ƶ��ص�
//���point����SASheet::pointOutOfRange ������Ƴ�����������
using SASheetMouseMoveCallBack = std::function<void(SASheet*, const SAPoint&, const SAPoint&)>;



//���
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

	//��ȡԪ�ظ���
	size_t getElementCount();
	//Ϊ��uiRowIndex������һ��Ԫ��
	void append(unsigned int uiRowIndex, cocos2d::Node* pNode);
	//Ϊ��uiRowIndex������vecNode���Ԫ��
	void append(unsigned int uiRowIndex, const std::vector<cocos2d::Node*>& vecNode);
	//����Ӧ���Ԫ��ֱ�������
	void append(const std::vector<cocos2d::Node*>& vecNode);
	//�������
	void reload(const std::vector<cocos2d::Node*>& vecNode);
	//���
	void clear();

	//�����±��ȡԪ��
	cocos2d::Node* at(const SAPoint& point);

	//�������±�ת��
	SAPoint convertVec2ToSAPoint(const cocos2d::Vec2& vec2);
	cocos2d::Vec2 convertSAPointToVec2(const SAPoint& point);

	//��ʼ���������¼�
	void startTouchListen(bool bSwallowTouches = true);
	//ֹͣ���������¼�
	void stopTouchListen();

	//��ʼ��������ƶ��¼�
	void startMouseMoveListen();
	//ֹͣ��������ƶ��¼�
	void stopMouseMoveListen();

	//����Ԫ�ص���ص�
	void setElementTouchedCallBack(SASheetElementTouchedCallBack func) { m_funcElementTouchedCallBack = func; }
	//��������ƶ��ص�
	void setMouseMoveCallBack(SASheetMouseMoveCallBack func) { m_funcMouseMoveCallBack = func; }

	//�¼��������ص�
	void mouseMoveCallBack(cocos2d::EventMouse* pEvent);
	bool touchBeganCallBack(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
	void touchEndedCallBack(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

	//��Χ��ĵ�
	static const SAPoint pointOutOfRange;


	/**
	* @name init
	*
	* @param sizeElement		����Ԫ�صĳߴ�
	* @param size				���ĳߴ�
	* @param fSpaceLenX			������
	* @param fSpaceLenY			������
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


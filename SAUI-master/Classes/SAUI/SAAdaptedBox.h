#pragma once
#include "cocos-ext.h"
#include "SAUI/SANode.h"

//����Ӧ��
class SAAdaptedBox : public SANode
{
public:
	enum AdaptDirection
	{
		//����������
		LEFT_BOTTOM,
		//����������
		LEFT_TOP,
		//����������
		RIGHT_BOTTOM,
		//����������
		RIGHT_TOP
	};

	SAAdaptedBox() :
		m_pNode(nullptr), 
		m_pWrapper(nullptr), 
		m_pBackground(nullptr), 
		m_pMouseListener(nullptr),
		m_bAdaptMode(true),
		m_bForceMode(true) {}
	virtual ~SAAdaptedBox();

	using Create = SACreator<true, SAAdaptedBox>;
	using CreateWithDirectionPriority = SACreator<true, SAAdaptedBox, const std::vector<AdaptDirection>&>;
	using CreateWithBackground = SACreator<true, SAAdaptedBox, cocos2d::extension::Scale9Sprite*>;
	using CreateWithBackgroundAndDirectionPriority = SACreator<true, SAAdaptedBox, const std::vector<AdaptDirection>&, cocos2d::extension::Scale9Sprite*>;

	//��ʼ��
	virtual bool init(cocos2d::extension::Scale9Sprite* pS9Spr = nullptr);
	virtual bool init(const std::vector<AdaptDirection>& vecPriority, cocos2d::extension::Scale9Sprite* pS9Spr = nullptr);

	//����Ӧ
	virtual void adapt();

	//�����Ƿ�����Ӧģʽ,Ĭ�Ͽ���
	//�������,AdaptedBox�����������ȼ��ı�Box��λ��,������з����޷�����Box,ʹ�����ȼ���ߵķ���
	//���������,ֻʹ�����ȼ���ߵķ���
	void setAdaptMode(bool bAdaptMode) { m_bAdaptMode = bAdaptMode; }
	bool isAdaptMode() { return m_bAdaptMode; }

	//�����Ƿ���ǿ��ģʽ,Ĭ�Ͽ���
	//�������,AdaptedBox����ǿ�Ƹı�Box��λ��,ʹ����������ʾ��Χ
	//���������,�򲻻�ǿ�Ƹı�Box��λ��
	void setForceMode(bool bForceMode) { m_bForceMode = bForceMode; }
	bool isForceMode() { return m_bForceMode; }

	//���óߴ�ƫ��,AdaptedBox��������ĳߴ�����sizeOffset
	void setOffsetSize(const cocos2d::Size& sizeOffset) { m_sizeOffset = sizeOffset; }
	const cocos2d::Size& getOffsetSize() { return m_sizeOffset; }

	//���÷�������ȼ�,iOrder��ȡֵ��Χ��0~3,ԽС���ȼ�Խ��
	void setDirectionPriority(AdaptDirection direction, int iOrder);
	//���÷������ȼ�(����ͨ���������ȼ��ķ�ʽ����AdaptedBox�ķ���)
	void setDirectionPriority(const std::vector<AdaptDirection>& vecPriority) { m_vecDirectionpriority = vecPriority; }
	//��ȡ���ȼ�
	const std::vector<AdaptDirection>& getDirectionPriority() { return m_vecDirectionpriority; }

	//���ñ���
	void setBackground(cocos2d::extension::Scale9Sprite* pS9Spr);

	virtual void addChild(Node* child) override;
	virtual void addChild(Node* child, int localZOrder) override;
	virtual void addChild(Node* child, int localZOrder, int tag) override;
	virtual void addChild(Node* child, int localZOrder, const std::string &name) override;

	//��ʼ��������ƶ��¼�,������AdaptedBox��ʵʱˢ��,����������ƶ�
	void startMouseMoveListen();
	//ֹͣ��������ƶ��¼�
	void stopMouseMoveListen();

protected:
	virtual void initNode();

	//��ȡ��һ������
	bool getNextDirectionInfo(AdaptDirection& direction);

	struct BoxAdapter
	{
		float xl;
		float xr;
		float yu;
		float yb;

		cocos2d::Size size;
		SAAdaptedBox* pBox;

		BoxAdapter(SAAdaptedBox* pBoxOuter);

		//��������Ӧ����
		void adapt(cocos2d::Vec2& vec2AnPoint, cocos2d::Vec2& vec2Position, cocos2d::Vec2& vec2WrapperPosition, bool bAdapt);
	};

	bool m_bAdaptMode;
	bool m_bForceMode;

	std::vector<AdaptDirection> m_vecDirectionpriority;
	cocos2d::Size m_sizeOffset;
	cocos2d::Node* m_pNode;
	cocos2d::Node* m_pWrapper;
	cocos2d::extension::Scale9Sprite* m_pBackground;
	cocos2d::EventListenerMouse* m_pMouseListener;

	friend struct BoxAdapter;
};


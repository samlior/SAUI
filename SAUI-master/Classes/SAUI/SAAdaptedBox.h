#pragma once
#include "cocos-ext.h"
#include "SAUI/SANode.h"

//自适应框
class SAAdaptedBox : public SANode
{
public:
	enum AdaptDirection
	{
		//出现在左下
		LEFT_BOTTOM,
		//出现在左上
		LEFT_TOP,
		//出现在右下
		RIGHT_BOTTOM,
		//出现在右上
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

	//初始化
	virtual bool init(cocos2d::extension::Scale9Sprite* pS9Spr = nullptr);
	virtual bool init(const std::vector<AdaptDirection>& vecPriority, cocos2d::extension::Scale9Sprite* pS9Spr = nullptr);

	//自适应
	virtual void adapt();

	//设置是否开启适应模式,默认开启
	//如果开启,AdaptedBox将会依据优先级改变Box的位置,如果所有方向都无法放下Box,使用优先级最高的方向
	//如果不开启,只使用优先级最高的方向
	void setAdaptMode(bool bAdaptMode) { m_bAdaptMode = bAdaptMode; }
	bool isAdaptMode() { return m_bAdaptMode; }

	//设置是否开启强制模式,默认开启
	//如果开启,AdaptedBox将会强制改变Box的位置,使他不超出显示范围
	//如果不开启,则不会强制改变Box的位置
	void setForceMode(bool bForceMode) { m_bForceMode = bForceMode; }
	bool isForceMode() { return m_bForceMode; }

	//设置尺寸偏移,AdaptedBox计算出来的尺寸会加上sizeOffset
	void setOffsetSize(const cocos2d::Size& sizeOffset) { m_sizeOffset = sizeOffset; }
	const cocos2d::Size& getOffsetSize() { return m_sizeOffset; }

	//设置方向的优先级,iOrder的取值范围的0~3,越小优先级越高
	void setDirectionPriority(AdaptDirection direction, int iOrder);
	//设置方向优先级(可以通过设置优先级的方式控制AdaptedBox的方向)
	void setDirectionPriority(const std::vector<AdaptDirection>& vecPriority) { m_vecDirectionpriority = vecPriority; }
	//获取优先级
	const std::vector<AdaptDirection>& getDirectionPriority() { return m_vecDirectionpriority; }

	//设置背景
	void setBackground(cocos2d::extension::Scale9Sprite* pS9Spr);

	virtual void addChild(Node* child) override;
	virtual void addChild(Node* child, int localZOrder) override;
	virtual void addChild(Node* child, int localZOrder, int tag) override;
	virtual void addChild(Node* child, int localZOrder, const std::string &name) override;

	//开始监听鼠标移动事件,开启后AdaptedBox会实时刷新,并跟随鼠标移动
	void startMouseMoveListen();
	//停止监听鼠标移动事件
	void stopMouseMoveListen();

protected:
	virtual void initNode();

	//获取下一个方向
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

		//计算自适应参数
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


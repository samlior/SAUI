#pragma once
#include "cocos2d.h"
#include "SA/SACreator.h"

class SALoadingScene;

using SALSBoolCallBack = std::function<bool(SALoadingScene*)>;
using SALSUpdateCallBack = std::function<bool(SALoadingScene*, float, float&, void*&)>;
using SALSUpdateUICallBack = std::function<void(SALoadingScene*, float, void*)>;
using SALSLoadOverCallBack = std::function<void(SALoadingScene*, void*)>;
using SALSVoidCallBack = std::function<void(SALoadingScene*)>;

class SALoadingScene : public cocos2d::Scene
{
public:
	SALoadingScene() :m_fUpdateInterval(0.5) {}

	using Create = SACreator<true, SALoadingScene>;
	using CreateWithCallBack = SACreator<true, SALoadingScene, SALSBoolCallBack>;

	//根据回调初始化
	virtual bool init(SALSBoolCallBack func = nullptr);

	//设置完成百分比
	virtual void setPercent(float fPercent, void* pUserData = nullptr);

	//加载完成
	virtual void loadOver(void* pUserData = nullptr);

	//设置定时器时间间隔
	void setUpdateInterval(float ft) { m_fUpdateInterval = ft; }
	float getUpdateInterval(float ft) { return m_fUpdateInterval; }

	//设置进入场景回调
	void setEnterCallBack(SALSVoidCallBack func) { m_funcOnEnter = func; }

	//设置更新进度回调(用户需要在此回调中返回最新的百分比及是否加载完成)
	//回调中三个参数分别代表时间间隔、百分比、用户自定义参数
	//返回true代表加载未完成,需要继续加载,返回false代表加载结束
	void setUpdateCallBack(SALSUpdateCallBack func) { m_funcUpdate = func; }

	//设置更新ui回调(用户需要在此回调中根据百分比更新ui)
	//回调中两个参数分别代表百分比、用户自定义参数
	void setUpdateUICallBack(SALSUpdateUICallBack func) { m_funcUpdateUI = func; }

	//加载结束回调(用户需要在此回调中加载下一个场景)
	//回调中参数代表用户自定义参数
	void setLoadOverCallBack(SALSLoadOverCallBack func) { m_funcOnLoadOver = func; }

	//结束场景回调
	void setExitCallBack(SALSVoidCallBack func) { m_funcOnExit = func; }


	//进入场景
	virtual void onEnter() override;

	//结束场景
	virtual void onExit() override;

	//更新进度定时器
	virtual void updateProcess(float ft);

protected:
	float m_fUpdateInterval;

	SALSVoidCallBack m_funcOnEnter;
	SALSUpdateCallBack m_funcUpdate;
	SALSUpdateUICallBack m_funcUpdateUI;
	SALSLoadOverCallBack m_funcOnLoadOver;
	SALSVoidCallBack m_funcOnExit;
};
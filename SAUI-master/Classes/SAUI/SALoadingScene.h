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

	//���ݻص���ʼ��
	virtual bool init(SALSBoolCallBack func = nullptr);

	//������ɰٷֱ�
	virtual void setPercent(float fPercent, void* pUserData = nullptr);

	//�������
	virtual void loadOver(void* pUserData = nullptr);

	//���ö�ʱ��ʱ����
	void setUpdateInterval(float ft) { m_fUpdateInterval = ft; }
	float getUpdateInterval(float ft) { return m_fUpdateInterval; }

	//���ý��볡���ص�
	void setEnterCallBack(SALSVoidCallBack func) { m_funcOnEnter = func; }

	//���ø��½��Ȼص�(�û���Ҫ�ڴ˻ص��з������µİٷֱȼ��Ƿ�������)
	//�ص������������ֱ����ʱ�������ٷֱȡ��û��Զ������
	//����true�������δ���,��Ҫ��������,����false������ؽ���
	void setUpdateCallBack(SALSUpdateCallBack func) { m_funcUpdate = func; }

	//���ø���ui�ص�(�û���Ҫ�ڴ˻ص��и��ݰٷֱȸ���ui)
	//�ص������������ֱ����ٷֱȡ��û��Զ������
	void setUpdateUICallBack(SALSUpdateUICallBack func) { m_funcUpdateUI = func; }

	//���ؽ����ص�(�û���Ҫ�ڴ˻ص��м�����һ������)
	//�ص��в��������û��Զ������
	void setLoadOverCallBack(SALSLoadOverCallBack func) { m_funcOnLoadOver = func; }

	//���������ص�
	void setExitCallBack(SALSVoidCallBack func) { m_funcOnExit = func; }


	//���볡��
	virtual void onEnter() override;

	//��������
	virtual void onExit() override;

	//���½��ȶ�ʱ��
	virtual void updateProcess(float ft);

protected:
	float m_fUpdateInterval;

	SALSVoidCallBack m_funcOnEnter;
	SALSUpdateCallBack m_funcUpdate;
	SALSUpdateUICallBack m_funcUpdateUI;
	SALSLoadOverCallBack m_funcOnLoadOver;
	SALSVoidCallBack m_funcOnExit;
};
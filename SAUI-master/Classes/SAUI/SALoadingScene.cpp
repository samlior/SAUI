#include "SALoadingScene.h"

using namespace std;
using namespace cocos2d;

bool SALoadingScene::init(SALSBoolCallBack func /*= nullptr*/)
{
	if (!Scene::init())
		return false;

	return (!func || func(this));
}

void SALoadingScene::onEnter()
{
	Scene::onEnter();
	schedule(SEL_SCHEDULE(&SALoadingScene::updateProcess), m_fUpdateInterval);
	if (m_funcOnEnter)
	{
		m_funcOnEnter(this);
	}
}

void SALoadingScene::onExit()
{
	Scene::onExit();
	unschedule(SEL_SCHEDULE(&SALoadingScene::updateProcess));
	if (m_funcOnExit)
	{
		m_funcOnExit(this);
	}
}

void SALoadingScene::updateProcess(float ft)
{
	if (m_funcUpdate)
	{
		float fNewPercent = 0.0f;
		void* pUserData = nullptr;
		if (m_funcUpdate(this, ft, fNewPercent, pUserData))
		{
			setPercent(fNewPercent, pUserData);
		}
		else
		{
			loadOver(pUserData);
		}
	}
}

void SALoadingScene::setPercent(float fPercent, void* pUserData /*= nullptr*/)
{
	if (m_funcUpdateUI)
	{
		m_funcUpdateUI(this, fPercent, pUserData);
	}
}

void SALoadingScene::loadOver(void* pUserData /*= nullptr*/)
{
	if (m_funcOnLoadOver)
	{
		m_funcOnLoadOver(this, pUserData);
	}
}
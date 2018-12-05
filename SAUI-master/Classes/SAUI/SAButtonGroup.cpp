#include "SAButtonGroup.h"

using namespace std;
using namespace cocos2d;




unsigned int SAButtonGroup::add(SASelectedButton* pBtn)
{
	m_vecAllBtn.push_back(pBtn);
	pBtn->setTouchEndedCallBack(CC_CALLBACK_3(SAButtonGroup::touchEndedCallBack, this));
	return m_vecAllBtn.size() - 1;
}

void SAButtonGroup::setSelected(unsigned int uiIndex)
{
	for (auto itr = m_vecAllBtn.begin(); itr != m_vecAllBtn.end(); ++itr)
	{
		(*itr)->setSelected((itr - m_vecAllBtn.begin() == uiIndex) ? true : false);
	}
	m_uiIndexSelected = uiIndex;
}

void SAButtonGroup::touchEndedCallBack(SASelectedButton* pBtn, Touch* pTouch, Event* pEvent)
{
	for (auto itr = m_vecAllBtn.begin(); itr != m_vecAllBtn.end(); ++itr)
	{
		if (*itr != pBtn)
			(*itr)->setSelected(false);
		else
		{
			(*itr)->setSelected(true);
			unsigned int uiIndex = itr - m_vecAllBtn.begin();
			if (m_uiIndexSelected != uiIndex)
			{
				m_uiIndexSelected = uiIndex;
				if (m_funcCallBack)
					m_funcCallBack(itr - m_vecAllBtn.begin(), pBtn);
			}
		}
	}
}
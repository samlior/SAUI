#include "SAControlButton.h"
#include "SATableView.h"
#include "SA/SAUtils.h"
#include "SAUI/SANode.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;



void SAControlButton::setSelected(bool bIsSelected)
{
	if (bIsSelected != m_bIsSelected)
	{
		if (m_pNode)
		{
			m_pNode->setVisible(bIsSelected);
			SANode::setRespondEventDeeply(m_pNode, bIsSelected);
		}
	}

	SASelectedButton::setSelected(bIsSelected);
}

void SAControlButton::setNode(Node* pNode)
{
	m_pNode = pNode;
	if (m_pNode)
	{
		m_pNode->setVisible(m_bIsSelected);
	}
}




void SAComboBox::cellTouchEndedCallBack(SATableView* table, TableViewCell* cell)
{
	if (!m_func || m_func(this, cell))
	{
		setSelected(!m_bIsSelected);
	}
}

void SAComboBox::setTableView(SATableView* pTableView)
{
	m_pTableView = pTableView;
	if (m_pTableView)
	{
		m_pTableView->setVisible(m_bIsSelected);
		m_pTableView->setCellTouchEndedCallBack(CC_CALLBACK_2(SAComboBox::cellTouchEndedCallBack, this));
	}
}

void SAComboBox::setSelected(bool bIsSelected)
{
	if (bIsSelected != m_bIsSelected)
	{
		if (m_pNode)
		{
			m_pNode->setVisible(bIsSelected);
			SANode::setRespondEventDeeply(m_pNode, bIsSelected);
		}
		if (m_pTableView)
		{
			m_pTableView->setVisible(bIsSelected);
			SANode::setRespondEventDeeply(m_pTableView, bIsSelected);
		}
		if (bIsSelected && m_pTableView)
		{
			m_pTableView->reloadData();
		}
	}

	SASelectedButton::setSelected(bIsSelected);
}
/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SAUI/SAUI.h"

class HelloWorld : public cocos2d::Scene, public cocos2d::extension::TableViewDataSource
{
public:
	HelloWorld() :
		m_pMessageBox(nullptr),
		m_pGroup(nullptr),
		m_pLabel1(nullptr),
		m_pLabel2(nullptr) {}
	~HelloWorld();

    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	SANode* createGLViewImplTest();
	SANode* createComboBoxTest();
	SANode* createQRCodeTest();
	SANode* createMessageBoxTest();
	SANode* createSheetTest();
	SANode* createSelectedButtonTest();
	SANode* createTableViewTest();

	//tableview datasource
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx) override;
	virtual cocos2d::Size cellSizeForTable(cocos2d::extension::TableView* table) override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table) override;


	void askClose();
	void updatePosition(int x, int y);

private:
	SAMessageBox* m_pMessageBox;

	SAButtonGroup* m_pGroup;

	cocos2d::Label* m_pLabel1;
	cocos2d::Label* m_pLabel2;
};

#endif // __HELLOWORLD_SCENE_H__

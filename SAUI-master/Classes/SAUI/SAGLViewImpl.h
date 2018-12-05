#pragma once

#ifdef SA_USE_GLVIEW_IMPL

#include "cocos2d.h"

/////////////////////警告///////////////////
//如果想使用此类,请替换cocos2d/cocos/platform/desktop/CCGLViewImpl-desktop.h及.cpp
//目前只在windows平台上测试通过
///////////////////////////////////////////


class SAGLViewImplDelegate
{
public:
	//当用户试图关闭程序时(alt+f4或点关闭按钮)进入回调,返回true则关闭程序,返回false不关闭程序
	virtual bool applicationWillClose() { return true; }

	//当窗口位置改变时进入回调
	virtual void applicationDidChangedPosition(int x, int y) {}

	//当应用程序即将关闭时进入回调
	virtual void applicationWillTerminate() {}
};




class SAGLViewImpl : public cocos2d::GLViewImpl
{
public:
	SAGLViewImpl() :m_pDelegate(nullptr) {}

	static SAGLViewImpl* create(const std::string& viewName);
	static SAGLViewImpl* create(const std::string& viewName, bool resizable);
	static SAGLViewImpl* createWithRect(const std::string& viewName, const cocos2d::Rect& rect, float frameZoomFactor = 1.0f, bool resizable = false);
	static SAGLViewImpl* createWithFullScreen(const std::string& viewName);
	static SAGLViewImpl* createWithFullScreen(const std::string& viewName, const GLFWvidmode& videoMode, GLFWmonitor* monitor);

	void setDelegate(SAGLViewImplDelegate* pDelegate) { m_pDelegate = pDelegate; }
	SAGLViewImplDelegate* getDelegate() { return m_pDelegate; }

protected:

	virtual void onGLFWClose(GLFWwindow* window) override;
	virtual void onWindowsSetPos(GLFWwindow* window, int x, int y) override;

	SAGLViewImplDelegate* m_pDelegate;
};

#endif
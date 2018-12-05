#pragma once

#ifdef SA_USE_GLVIEW_IMPL

#include "cocos2d.h"

/////////////////////����///////////////////
//�����ʹ�ô���,���滻cocos2d/cocos/platform/desktop/CCGLViewImpl-desktop.h��.cpp
//Ŀǰֻ��windowsƽ̨�ϲ���ͨ��
///////////////////////////////////////////


class SAGLViewImplDelegate
{
public:
	//���û���ͼ�رճ���ʱ(alt+f4���رհ�ť)����ص�,����true��رճ���,����false���رճ���
	virtual bool applicationWillClose() { return true; }

	//������λ�øı�ʱ����ص�
	virtual void applicationDidChangedPosition(int x, int y) {}

	//��Ӧ�ó��򼴽��ر�ʱ����ص�
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
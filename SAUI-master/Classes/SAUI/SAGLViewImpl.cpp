#ifdef SA_USE_GLVIEW_IMPL

#include "SAGLViewImpl.h"

using namespace std;
using namespace cocos2d;

SAGLViewImpl* SAGLViewImpl::create(const string& viewName)
{
	return SAGLViewImpl::create(viewName, false);
}

SAGLViewImpl* SAGLViewImpl::create(const string& viewName, bool resizable)
{
	auto ret = new (std::nothrow) SAGLViewImpl;
	if (ret && ret->initWithRect(viewName, Rect(0, 0, 960, 640), 1.0f, resizable))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

SAGLViewImpl* SAGLViewImpl::createWithRect(const string& viewName, const Rect& rect, float frameZoomFactor, bool resizable)
{
	auto ret = new (std::nothrow) SAGLViewImpl;
	if (ret && ret->initWithRect(viewName, rect, frameZoomFactor, resizable))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

SAGLViewImpl* SAGLViewImpl::createWithFullScreen(const string& viewName)
{
	auto ret = new (std::nothrow) SAGLViewImpl;
	if (ret && ret->initWithFullScreen(viewName))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

SAGLViewImpl* SAGLViewImpl::createWithFullScreen(const string& viewName, const GLFWvidmode &videoMode, GLFWmonitor *monitor)
{
	auto ret = new (std::nothrow) SAGLViewImpl;
	if (ret && ret->initWithFullscreen(viewName, videoMode, monitor))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void SAGLViewImpl::onGLFWClose(GLFWwindow* window)
{
	if (m_pDelegate)
	{
		if (!m_pDelegate->applicationWillClose())
		{
			GLViewImpl::setGLFWCloseFlag(false);
		}
		else
		{
			m_pDelegate->applicationWillTerminate();
		}
	}
}

void SAGLViewImpl::onWindowsSetPos(GLFWwindow* window, int x, int y)
{
	if (m_pDelegate)
		m_pDelegate->applicationDidChangedPosition(x, y);
}

#endif
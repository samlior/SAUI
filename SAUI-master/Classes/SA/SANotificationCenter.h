#pragma once
#include <map>
#include <string>
#include <functional>


using SANotifyCallBack = std::function<void(void*, void*)>;

//简单实现的通知中心,一种通知只能有一个监听者
template<typename T>
class SANotificationCenter
{
public:
	static void registerObserver(const T& t, SANotifyCallBack func) { m_map.insert(std::pair<T, SANotifyCallBack>(t, func)); }
	static void removeAllObservers() { m_map.clear(); }
	static void removeObserver(const T& t)
	{ 
		auto itr = m_map.find(t);
		if (itr != m_map.end())
			m_map.erase(itr);
	}
	static void postNotify(const T& t, void* pSender = nullptr, void* pUserData = nullptr)
	{
		auto itr = m_map.find(t);
		if (itr != m_map.end())
			itr->second(pSender, pUserData);
	}

protected:
	static std::map<T, SANotifyCallBack> m_map;
};

template<typename T>
std::map<T, SANotifyCallBack> SANotificationCenter<T>::m_map;

using SADefaultNotificationCenter = SANotificationCenter<std::string>;
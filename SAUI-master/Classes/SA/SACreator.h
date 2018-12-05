#pragma once

template<bool bAutoRelease, typename T, typename ... Args>
class SACreator
{
public:
	static T* create(Args ... data)
	{
		T* pT = new (std::nothrow) T;
		if (pT &&
			pT->init(data ...))
		{
			bAutoRelease ? pT->autorelease() : pT->retain();
			return pT;
		}
		else
		{
			delete pT;
			pT = nullptr;
			return nullptr;
		}
	}
};
#pragma once

#define CREATE_SACREATOR_HELPER(__CLASS_NAME__, __FUNC_NAME__) \
template<bool bAutoRelease, typename T, typename ... Args>\
class __CLASS_NAME__\
{\
public:\
	static T* create(Args ... data)\
	{\
		T* pT = new (std::nothrow) T;\
		if (pT &&\
			pT->__FUNC_NAME__(data ...))\
		{\
			bAutoRelease ? pT->autorelease() : pT->retain();\
			return pT;\
		}\
		else\
		{\
			delete pT;\
			pT = nullptr;\
			return nullptr;\
		}\
	}\
};

CREATE_SACREATOR_HELPER(SACreator, init);
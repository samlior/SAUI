#pragma once

template<typename T>
class SAInstanceCreator
{
public:
	static T* getInstance()
	{
		static T t;
		return &t;
	}
};
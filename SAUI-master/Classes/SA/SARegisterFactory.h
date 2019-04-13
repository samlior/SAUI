#pragma once
#include <map>
#include <functional>

template<typename TKey, typename TVal>
class SARegisterFactory
{
public:
	using CCreateFunc = std::function<TVal(void*)>;

	virtual TVal create(TKey key, void* pData = nullptr)
	{
		auto itr = m_mapRegisterFactory.find(key);
		if (itr == m_mapRegisterFactory.end())
		{
			throwErrorWhenMissingKey(key, pData);
		}
		return itr->second(pData);
	}

	virtual void reg(TKey key, CCreateFunc func)
	{
		m_mapRegisterFactory.insert(make_pair(key, func));
	}

protected:
	virtual void throwErrorWhenMissingKey(TKey, void*)
	{
		throw std::exception();
	}

	std::map<TKey, CCreateFunc> m_mapRegisterFactory;
};

template<typename TKey, typename TVal>
class SARegisterFactoryWithBit : public SARegisterFactory<TKey, TVal>
{
public:
	virtual TVal create(TKey key, void* pData = nullptr) override
	{
		for (auto itr = m_mapRegisterFactory.begin(); itr != m_mapRegisterFactory.end(); ++itr)
		{
			if ((itr->first & key) > 0)
			{
				return itr->second(pData);
			}
		}

		throwErrorWhenMissingKey(key, pData);
		return nullptr;
	}
};

template <typename TKey, typename TVal, typename TIns, TKey key>
class SARegister_default
{
public:
	SARegister_default()
	{
		TIns::getInstance()->reg(key, [](void*) { return new TVal; });
	}

	static TKey getKey() { return key; }
};

template <typename TKey, typename TVal, typename TIns, TKey key>
class SARegister_create
{
public:
	SARegister_create()
	{
		TIns::getInstance()->reg(key, [](void*) { return TVal::Create::create(); });
	}

	static TKey getKey() { return key; }
};
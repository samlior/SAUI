#pragma once
#include "cocos2d.h"
#include "SACreator.h"
#include "SAInstanceCreator.h"
#include "SARegisterFactory.h"

#define CREATE_DEFAULT_CHILD_CLASS(__PARENT_NAME__, __ENUM_NAME__)\
class __PARENT_NAME__##_##__ENUM_NAME__ : public __PARENT_NAME__\
{\
public:\
	using Register = SARegister_create<int, __PARENT_NAME__##_##__ENUM_NAME__, SAFactory_##__PARENT_NAME__, __ENUM_NAME__>;\
	using Create = SACreator<true, __PARENT_NAME__##_##__ENUM_NAME__>;\
	static Register __PARENT_NAME__##_##__ENUM_NAME__##_register;\
};

#define CREATE_REGISTER_IMPL(__PARENT_NAME__, __ENUM_NAME__)\
__PARENT_NAME__##_##__ENUM_NAME__##::Register __PARENT_NAME__##_##__ENUM_NAME__##::##__PARENT_NAME__##_##__ENUM_NAME__##_register;

#define CREATE_SAFACTORY(__CLASS_NAME__)\
class SAFactory_##__CLASS_NAME__ : public SARegisterFactory<int, __CLASS_NAME__*>, public SAInstanceCreator<SAFactory_##__CLASS_NAME__>\
{\
protected:\
	void throwErrorWhenMissingKey(int, void*) override { CCASSERT(false, "unknow type"); }\
};

#define APPEND_CHILD_CLASS_NAME(__PARENT_NAME__, __ENUM_NAME__) __PARENT_NAME__##_##__ENUM_NAME__

#define APPEND_FACTORY_NAME(__PARENT_NAME__) SAFactory_##__PARENT_NAME__

#define INIT_CHILD_CLASS(__PARENT_NAME__, __ENUM_NAME__)\
using Register = SARegister_create<int, __PARENT_NAME__##_##__ENUM_NAME__, SAFactory_##__PARENT_NAME__, __ENUM_NAME__>; \
using Create = SACreator<true, __PARENT_NAME__##_##__ENUM_NAME__>;\
static Register __PARENT_NAME__##_##__ENUM_NAME__##_register;
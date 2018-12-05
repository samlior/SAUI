#pragma once

enum enSAFileConfig
{
	//从本地加载文件
	LOAD_TEXTURE_FROM_LOCAL = 0b0000,
	//从精灵表加载文件
	LOAD_TEXTURE_FROM_SPR_FRAME = 0b0001,

	//普通精灵
	SPR_TYPE_NOMAL = 0b0000,
	//SCALE9精灵
	SPR_TYPE_SCALE9 = 0b0010
};
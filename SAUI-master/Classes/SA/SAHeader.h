#pragma once

enum enSAFileConfig
{
	//�ӱ��ؼ����ļ�
	LOAD_TEXTURE_FROM_LOCAL = 0b0000,
	//�Ӿ��������ļ�
	LOAD_TEXTURE_FROM_SPR_FRAME = 0b0001,

	//��ͨ����
	SPR_TYPE_NOMAL = 0b0000,
	//SCALE9����
	SPR_TYPE_SCALE9 = 0b0010
};
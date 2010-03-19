/**************************************************************************************
 *  Copyright (c) Artificial Intelligence Infinity.
 *  Filename: ioApiWrap.h 
 *  Description: debug infomation API header 
 *  Author: caoyun
 *  Create:2010-1-25 
 *  Modification history:
 *
 *
 **************************************************************************************/ 

#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef  __cplusplus
extern "C"
{
#endif

#define    DEBUG_LEVEL_1          1    //��ͨ������Ϣ�������汾�ر�
#define    DEBUG_LEVEL_2          2    //��Ҫ������Ϣ��������һֱ����
#define    DEBUG_LEVEL_3          3    //��΢���������Ϣ,����ʱ�����жϳ���
#define    DEBUG_LEVEL_4          4    //�������������Ϣ������ʱ���жϳ���

void debug_info(int debugLevel, const char *fmt, ...);//������Ϣ

#ifdef __cplusplus
}
#endif

#endif




#pragma once

#ifndef AFCFUNCTION_H_2003_7
#define AFCFUNCTION_H_2003_7

/**************************************************************************************************************/

// AFC ȫ�ֺ���

/**************************************************************************************************************/

//��������ά��
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

//��ȫɾ��ָ��
#define  SafeDelete(pData)	{ try { delete pData; } catch (...) { TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);} pData=NULL; } 

//��ȫɾ��ָ��
#define  SafeDeleteArray(pData)	{ try { delete [] pData; } catch (...) { TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);} pData=NULL; } 

/**************************************************************************************************************/

#endif
#pragma once

#ifndef AFCPTRARRAY_H_FILE_2003_07
#define AFCPTRARRAY_H_FILE_2003_07

#include "AFCLock.h"
#include "AFCFunction.h"

///ָ�������ࣨ��ɢ��ʽ��
template <class ClassType> class CAFCPtrArray
{
	///��������
protected:
	UINT						m_uActiveCount;						///��ǰ��Ч��Ŀ
	UINT						m_uPermitCount;						///���������Ŀ
	ClassType					* * m_pActivePtr;					///��Ч����
	ClassType					* * m_pSleepPtr;					///��Ч����
	ClassType					* m_pClassArray;					///��������
	CAFCSignedLock				m_csLock;							///ͬ����
	///
	///Kylin 20090116 Vip��½����
	UINT						m_uVipReseverCount;						///Vip������½��Ŀ
	///
	///��������
public:
	///���캯��
	CAFCPtrArray();
	///��������
	~CAFCPtrArray();

	///���ܺ���
public:
	///��ʼ��
	bool Init(UINT uPermitCount,UINT uVipReseverCount);
	///ж��
	bool UnInit();
	///��ȡ���ɢ��ʽ��
	ClassType * NewItem();
	///�ͷ����ɢ��ʽ��
	bool FreeItem(ClassType * pItem);
	///��ȡ�˳��ʽ��
	ClassType * NewItem_Order(UINT uIndex);
	///�ͷ��˳��ʽ��
	bool FreeItem_Order(UINT uIndex);

	///��������
public:
	///��ȡ��Ч��Ŀ
	UINT GetActiveCount() { return m_uActiveCount; };
	///��ȡ������Ŀ
	UINT GetPermitCount() { return m_uPermitCount; };
	///
	///Kylin 20090116 Vip��½����	
	UINT GetVipReseverCount() { return m_uVipReseverCount; };
	///
	///ͨ���������ȡ������
	ClassType * GetActiveItem(UINT uIndex) { return *(m_pActivePtr+uIndex); };
	///ͨ��������ȡ��ڴ��
	ClassType * GetArrayItem(UINT uIndex) { return (m_pClassArray+uIndex); };
	///ͨ�����ȡ�������ڴ��
	UINT GetItemIndex(ClassType * pFindItem) { return (UINT)(pFindItem-m_pClassArray); };
	///��ȡ�ڴ�����
	ClassType * GetClassArrayPtr() { return m_pClassArray; };
	///��ȡ��
	CAFCSignedLock * GetArrayLock() { return &m_csLock; };
};

///���캯��
template <class ClassType> CAFCPtrArray<ClassType>::CAFCPtrArray()
{
	m_uPermitCount=0;
	m_uActiveCount=0;
	m_pActivePtr=NULL;
	m_pSleepPtr=NULL;
	m_pClassArray=NULL;
	return;
}

///��������
template <class ClassType> CAFCPtrArray<ClassType>::~CAFCPtrArray()
{
	UnInit();
	return;
}

///��ʼ������
///
///Kylin 20090116 Vip��½����
template<class ClassType> bool CAFCPtrArray<ClassType>::Init(UINT uPermitCount,UINT uVipReseverCount)
{
	try
	{
		///�����ڴ�
		m_pClassArray=new ClassType [uPermitCount];
		m_pSleepPtr=new ClassType * [uPermitCount];
		m_pActivePtr=new ClassType * [uPermitCount];

		///Ч�����
		if ((m_pClassArray==NULL)||(m_pSleepPtr==NULL)||(m_pActivePtr==NULL))
		{
			UnInit();
			return false;
		}

		///���ò���
		memset(m_pActivePtr,0,sizeof(ClassType *)*uPermitCount);
		for (UINT i=0;i<uPermitCount;i++) *(m_pSleepPtr+i)=m_pClassArray+i;
		m_uActiveCount=0;
		m_uPermitCount=uPermitCount;
		m_uVipReseverCount=uVipReseverCount;
		
///	
		return true;
	}
	catch (...) { TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);UnInit(); }
	
	return false;
}

///ж�غ���
template<class ClassType> bool CAFCPtrArray<ClassType>::UnInit()
{
	SafeDeleteArray(m_pActivePtr);
	SafeDeleteArray(m_pSleepPtr);
	SafeDeleteArray(m_pClassArray);
	m_uPermitCount=0;
	m_uActiveCount=0;
	m_pActivePtr=NULL;
	m_pSleepPtr=NULL;

	return true;
}

///��ȡ�����ɢ��ʽ��
template<class ClassType> ClassType * CAFCPtrArray<ClassType>::NewItem()
{
	if (m_uActiveCount<m_uPermitCount)
	{
		*(m_pActivePtr+m_uActiveCount)=*m_pSleepPtr;
		MoveMemory(m_pSleepPtr,m_pSleepPtr+1,(m_uPermitCount-m_uActiveCount-1)*sizeof(ClassType *));
		*(m_pSleepPtr+m_uPermitCount-m_uActiveCount-1)=NULL;
		return *(m_pActivePtr+m_uActiveCount++);
	}
	return NULL;
}

///�ͷ����ɢ��ʽ��
template<class ClassType> bool CAFCPtrArray<ClassType>::FreeItem(ClassType * pItem)
{
	if (pItem!=NULL)
	{
		for (UINT i=0;i<m_uActiveCount;i++)
		{
			if (*(m_pActivePtr+i)==pItem)
			{
				*(m_pSleepPtr+m_uPermitCount-m_uActiveCount)=*(m_pActivePtr+i);
				*(m_pActivePtr+i)=NULL;
				*(m_pActivePtr+i)=*(m_pActivePtr+(--m_uActiveCount));
				return true;
			}
		}
	}
	return false;
}

#endif
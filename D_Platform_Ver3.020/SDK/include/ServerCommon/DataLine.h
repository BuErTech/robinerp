#pragma once

#include "PublicDefine.h"
#include "AFCLock.h"

//�����궨��
#define LD_DEF_STEP				4096								//���ݶ��л��岽��
#define LD_MAX_PART				8192								//���ݰ���󳤶�//3096 -> 1024*5 �ټ��� modify by wlr 20090716
#define LD_MAX_DATA				650000								//���ݶ�����󳤶�

 
#ifndef STRUCT_DATALINEHEAD
#define STRUCT_DATALINEHEAD
//���ݶ�����Ϣͷ
struct DataLineHead
{
	UINT						uSize;								//���ݴ�С
	UINT						uDataKind;							//��������
};
#endif //STRUCT_DATALINEHEAD
#ifndef STRUCT_DATABASERESULTLINE
#define STRUCT_DATABASERESULTLINE
///���ݿ�����Ϣ�ṹ����
struct DataBaseResultLine
{
	DataLineHead						LineHead;					///����ͷ
	UINT								uHandleRusult;				///������
	UINT								uHandleKind;				///��������
	UINT								uIndex;						///��������
	DWORD								dwHandleID;					///�����ʶ
};
#endif //STRUCT_DATABASERESULTLINE


/*
Struct		:ListItemData
Memo		:���������ݽṹ
Author		:Fred Huang
Add Data	:2008-3-4
Modify Data	:none
Parameter	:
	stDataHead	:���ݰ�ͷ
	pData		:ÿ�����������ݵ�ָ�룬ʹ����new��ʽ������ڴ棬ע�⣬�ڳ�����ʱ��Ҫ��ʽ��delete ���ڴ�
*/
struct ListItemData
{
	DataLineHead				stDataHead;
	BYTE						* pData;
};
//���ݶ�����
class EXT_CLASS CDataLine
{
	//��������
private:
	CList <ListItemData*,ListItemData*> m_DataList;

	HANDLE						m_hCompletionPort;					//��ɶ˿�
	CAFCSignedLock				m_csLock;							//ͬ����

	//��������
public:
	//���캯��
	CDataLine();
	//��������
	virtual ~CDataLine(void);

	//���ܺ���
public:
	//��ȡ��
	CAFCSignedLock * GetLock() { return &m_csLock; };
	//������������
	bool CleanLineData();
	//������ɶ˿�
	void SetCompletionHandle(HANDLE hCompletionPort) { m_hCompletionPort=hCompletionPort; }
	//������Ϣ����
	virtual UINT AddData(DataLineHead * pDataInfo, UINT uAddSize, UINT uDataKind, void * pAppendData=NULL, UINT uAppendAddSize=0);
	//��ȡ��Ϣ����
	virtual UINT GetData(DataLineHead * pDataBuffer, UINT uBufferSize);
public:
	INT_PTR GetDataCount(void);
};


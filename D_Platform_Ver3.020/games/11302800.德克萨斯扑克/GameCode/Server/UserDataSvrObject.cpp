
#include "StdAfx.h"
#include "UserDataSvrObject.h"

//��ʼ������
CUserDataSvrObject::CUserDataSvrObject(void)
{	
	// ��¼�Ƿ���ȫ��ȷ�ϴ���
	m_bIn = false;
}

//��������
CUserDataSvrObject::~CUserDataSvrObject(void)
{

}

//��������ʼ��
void CUserDataSvrObject::Init()
{
	HalfInit();

	//����Ƿ��Ѷ���
	m_bNetCut = false;

	//�û���ǰ�����Ķ������
	//m_byVerbFlag = 0L;

	// ��¼�Ƿ���ȫ��ȷ�ϴ���
	//m_bIn = false;

}

//���ʼ��
void CUserDataSvrObject::HalfInit()
{
	//�Լ���״̬
	m_nState = UNKNOWN;

	//��ʼ�Լ��˿˱�
	::memset(m_byMyCards,0,sizeof(m_byMyCards));

	//�û������˿���Ŀ
	m_iCardsNum = 0;

	//��ǰ�û�������˿�
	::memset(m_byOutCards,0,sizeof(m_byOutCards));

	//��ǰ�û�������˿�����
	m_iOutCardsNum = 0;
	
	// ����Ƿ�����
	m_bSendCardFinish = false;

}

//���ĳ��������ϵ�������
//@param pCardsȡ�õ���ֵ���ڴ�
void CUserDataSvrObject::GetHandCards(BYTE* pCards)
{
	::memset(pCards,0,MAX_HANDCARDS_NUM);
	::memcpy(pCards,m_byMyCards,m_iCardsNum);
	return;
}

//�����Ҹճ�����
//@param pCardsȡ�õ���ֵ���ڴ�
void CUserDataSvrObject::GetOutCards(BYTE* pCards)
{
	::memset(pCards,0,MAX_HANDCARDS_NUM);
	::memcpy(pCards,m_byOutCards,m_iOutCardsNum);
	return;
}

/////���ƣ�SetVerbs
/////������������ҵĶ���
/////@param pCardsȡ�õ���ֵ���ڴ�
/////@return
//void CUserDataSvrObject::MakeVerbs(BYTE byFlag)
//{
//	m_byVerbFlag = byFlag;
//	return;
//}
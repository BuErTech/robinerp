
#include "StdAfx.h"
#include "UserDataCliObject.h"

///���ƣ�CUserDataObject
///��������ʼ���������ã��������͵�
///@param 
///@return 
CUserDataCliObject::CUserDataCliObject(void)
{	

}

///���ƣ���������
///������~CUserDataObject
///@param 
///@return 
CUserDataCliObject::~CUserDataCliObject(void)
{

}

///���ƣ�Init
///��������ʼ��
///@param 
///@return 
void CUserDataCliObject::Init()
{
	//��ʼ�Լ��˿˱�
	::memset(m_byMyCards,0,sizeof(m_byMyCards));

	//�û������˿���Ŀ
	m_iCardsNum = 0;
}


///���ƣ�GetCards
///���������ĳ��������ϵ�������
///@param pCardsȡ�õ���ֵ���ڴ�
///@return 
void CUserDataCliObject::GetCards(BYTE* pCards)
{
	::memcpy(pCards,m_byMyCards,m_iCardsNum);
	return;
}
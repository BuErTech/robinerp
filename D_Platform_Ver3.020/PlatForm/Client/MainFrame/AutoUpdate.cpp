//////////�Զ�����
#include "StdAfx.h"
#include "AutoUpdate.h"

//���캯��
CAutoUpdate::CAutoUpdate(void)
{
}
//��������
CAutoUpdate::~CAutoUpdate(void)
{
}
//���º���
bool CAutoUpdate::Start(bool bForce)
{
	return true;
}

//ֹͣ�Զ�����
bool CAutoUpdate::Stop()
{	return true;
}

//�����߳�
unsigned __stdcall CAutoUpdate::UpdateThread(LPVOID pThreadData)
{	//��ȡ�����ļ�
	CInternetSession Seccion;
	return 0;
}
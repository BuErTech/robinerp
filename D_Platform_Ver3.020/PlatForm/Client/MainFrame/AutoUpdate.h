#pragma once
#ifndef AUTOUPDATE_HEAD_H
#define AUTOUPDATE_HEAD_H

#include "Afxinet.h"

//�Զ�������
class CAutoUpdate
{
	//��������
protected:
	

	//��������
public:
	//���캯��
	CAutoUpdate(void);
	//��������
	~CAutoUpdate(void);

	//���ܺ���
public:
	//���º���
	bool Start(bool bForce);
	//ֹͣ�Զ�����
	bool Stop();

	//�̺߳���
protected:
	//�����߳�
	static unsigned __stdcall UpdateThread(LPVOID pThreadData);
};
#endif

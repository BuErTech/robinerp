#pragma once

#include "StdAfx.h"
#include "Resource.h"

//�û�ͼƬ
class CUserAvatar : public CDialog
{
	//��������
public:
	//���캯��
	CUserAvatar();
	//��������
	virtual ~CUserAvatar();

// �Ի�������
	enum { IDD = IDD_USER_AVATAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

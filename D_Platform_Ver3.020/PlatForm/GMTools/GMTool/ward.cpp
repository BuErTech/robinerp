// ward.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TCP.h"
#include "ward.h"


// Cward �Ի���

IMPLEMENT_DYNAMIC(Cward, CDialog)

Cward::Cward(CWnd* pParent /*=NULL*/)
	: CDialog(Cward::IDD, pParent)
{

}

Cward::~Cward()
{
}

void Cward::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Cward, CDialog)
END_MESSAGE_MAP()


// Cward ��Ϣ�������

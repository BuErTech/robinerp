#pragma once
#ifndef GAMEPLACE_HEAD_H
#define GAMEPLACE_HEAD_H

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "StdAfx.h"
#include "Afxmt.h"
#include "Resource.h"
#include "SkinMgr.h"

//Ӧ�ó������
class CGamePlaceApp : public CWinApp
{
	//��������
public:
	//���캯�� 
	CGamePlaceApp();
	~CGamePlaceApp();
	//��ʼ������
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()

public:

	static int GetModuleStatus(char* module,char* compoment,int defval = 0);

private:
	CSkinMgr m_skinmgr;
};

//Ӧ�ó������
extern CGamePlaceApp theApp;
//extern char TML_//SN[128];
//extern CString TML_//SN;///////����ȫ�ֱ���
#endif
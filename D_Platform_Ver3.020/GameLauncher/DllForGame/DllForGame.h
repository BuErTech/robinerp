// DllForGame.h : DllForGame DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDllForGameApp
// �йش���ʵ�ֵ���Ϣ������� DllForGame.cpp
//

class CDllForGameApp : public CWinApp
{
public:
	CDllForGameApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

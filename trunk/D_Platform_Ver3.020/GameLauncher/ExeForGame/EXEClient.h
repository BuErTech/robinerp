// EXE�ͻ���.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CEXEClientApp:
// �йش����ʵ�֣������ EXE�ͻ���.cpp
//

class CEXEClientApp : public CWinApp
{
public:
	CEXEClientApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CEXEClientApp theApp;
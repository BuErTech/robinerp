// TCP.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include "../../../SDK/include/socket/TCPClientSocket.h"

// CTCPApp:
// �йش����ʵ�֣������ TCP.cpp
//

class CTCPApp : public CWinApp
{
public:
	CTCPApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTCPApp theApp;
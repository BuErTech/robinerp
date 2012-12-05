// ***************************************************************
//  BzLog   version:  1.0   ��  date: 01/17/2011
//  -------------------------------------------------------------
//    ��Ϸ��־�ļ�������
//  -------------------------------------------------------------
//  Copyright (C) 2011 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#pragma once

class CBzLog
{

public:

	CBzLog(void);

	//�����ļ���
	static bool CreateGameRoomDir(char* szName);

	//������־
	bool CreateGameRoomLog(const ComRoomInfo* pRoomInfo);

	//�ͷ���־
	bool ReleaseBzLog();

	//д��־
	bool Writeline(CString str);

	//��ȡ��־ID
	const char* GetLogID();

public:

	~CBzLog(void);

public:

	static void init();

	//���������Ϣ,���ı���־
	void OutputTraceStr(const char *lpszFormat, ...);

private:

	//��־�ļ�
	CFile* m_pLogFile;

	//��ϷID
	UINT m_uGameID;

	//��־�ļ���
	char m_szName[MAX_PATH];

	//��־ID
	char m_szLogID[32];

	//Ŀ¼��
	static char m_szRootName[64];

};
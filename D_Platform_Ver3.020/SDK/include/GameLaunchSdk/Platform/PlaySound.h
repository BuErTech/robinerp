#pragma once
#ifndef PLAYSOUND_HEAD_H
#define  PLAYSOUND_HEAD_H

#include "PublicDefine.h"
#include<Vfw.h>
// �������ֲ�����
class EXT_CLASS CPlaySound :public CWnd
{
public:
	CPlaySound(void);
public:
	~CPlaySound(void);

private:
	HWND		hmciwnd;		// MCI����
	int			m_count;		// ��Ҫ���ŵĴ���
	int			m_playCount;	// �Ѿ����ŵĴ���
	//////////////////////////
	HWND		m_PlayWin;		// ���Ž�����, �յ�֪ͨ�Ĵ���
	int			m_AllCount;		// �ܵ���������
	int			m_CurPos;		// ��ǰ���ŵڼ�������
	int			m_PlayMode;		// ����ģʽ
	TCHAR		m_filename[MAX_PATH];	// ini�ļ�·��
	TCHAR		m_key[20];				// ini��Ĺؼ���

public:
	// ����filename, count��
	BOOL PlayMusic(HWND hwnd,TCHAR *filename,int count=-1);
	// ����filename ini�ļ����key section, ����������
	BOOL PlayMusic(HWND hwnd,TCHAR *filename,TCHAR *key,bool IsPlay);
	// ��������
	bool StopMusic();
	// ��ʼ����
	bool Play(bool IsFirst);
	// ��������
	bool CreateWnd(CWnd * pParentWnd, UINT uWndID);

public:
	DECLARE_MESSAGE_MAP()

public:	
	// �ص�����
	afx_msg LRESULT OnNotifyMode(WPARAM wParam,LPARAM lParam);	
};

#endif
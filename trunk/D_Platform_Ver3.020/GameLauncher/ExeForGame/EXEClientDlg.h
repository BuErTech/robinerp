// EXEClientDlg.h : ͷ�ļ�
//

#pragma once

#include "stdafx.h"
#include "LoveSendClassInExe.h"
// IPC���
#include "IPC\common.h"
#include "IPC\sink.h"
#include "IPC\ChannelService.h"
#include "SkinMgr.h"

#define TIMER_HEART_BEAT		100	// ����ά��IPC�����Ķ�ʱ��
const int HEART_BEAT_SECOND		= 3;	// �������
const int HEART_BEAT_DELAY		= 10;	// ���������ʱ

#define TIMER_DRAW              101
#define FRAME_INTERVAL          20

class IGameEngine;
// CEXEClientDlg �Ի���
class CEXEClientDlg : public CDialog, public IChannelMessageSink
{
// ����
public:
	CEXEClientDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CEXEClientDlg();

// �Ի�������
	enum { IDD = IDD_MAIN_DLG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


	//========================================================================\\
									IPC֧��
public:
	bool SetupIPC();
	//�ŵ����ݴ���
	virtual bool __cdecl OnChannelMessage(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
	//����֪ͨ
	bool OnIPCControl(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
	//����ָ��
	bool OnIPCSocket(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
	//����ָ��
	bool OnIPCConfig(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
	//�û�����
	bool OnIPCUserAct(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
	//���߳�ʼ��
	bool OnIPCProp(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
	//��������
	bool SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize);
	//Ӳ������
	void SetAcceleration(int iLevel);
public:
	//�������ԭ����Ӳ����������
	int		m_iLevel;
	int     m_hwAcc;
private:
	HANDLE								m_hShareMemory;
	tagShareMemory						* m_pShareMemory;				//�����ڴ�
	CChannelService						*m_ChannelService;
	DWORD								m_dwIPCHeartBeatTick;			// ������¼������Ϣ����ʱ���Tick��
	//========================================================================//


// ʵ��
protected:
	HICON m_hIcon;
	HINSTANCE						m_hGameInstance;					//��Ϸ����
	//bool							m_bNeedRefleshProp;					//��Ҫˢ�µ���
	//int								m_nPropCount;						//��ƽ̨�����ĵ�������
	CArray<_TAG_USERPROP*,_TAG_USERPROP*> m_userPropLibrary;							//�û�ӵ�еĵ���


	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();

private:
	GameInfoStructInExe		m_GameInfo;
	IFrameInterface			* m_IGameFrame;						//��Ϸ���
    IGameEngine             * m_pNewGame;                       //����Ϸ
//	CLoveSendClassInExe		*m_pLoveSendClassInExe;
	LRESULT OnGameQuit(WPARAM wParam, LPARAM lParam);
	LRESULT OnRequestPlayList(WPARAM wParam, LPARAM lParam);
	LRESULT OnSendGameData(WPARAM wParam, LPARAM lParam);
	LRESULT OnSetStation(WPARAM wParam, LPARAM lParam);
	LRESULT OnSendBroadData(WPARAM wParam, LPARAM lParam);
	LRESULT OnSendBuyProp(WPARAM wParam, LPARAM lParam);
    LRESULT OnSendBuyPropNew(WPARAM wParam, LPARAM lParam); // PengJiLin, 2010-10-13, �̵���ߵļ�ʱ������
	LRESULT OnSendGiveProp(WPARAM wParam, LPARAM lParam);
	LRESULT OnSendBuyVIP(WPARAM wParam, LPARAM lParam);      //������崫�صĹ���VIP��Ϣ�� add by wyl   11-5-21

	LRESULT OnSendAddFriend(WPARAM wParam, LPARAM lParam);      //��Ӻ���
	LRESULT OnSendAddFriend_R(WPARAM wParam, LPARAM lParam);      //��Ӻ���
	LRESULT OnSendGetFriendList(WPARAM wParam, LPARAM lParam);      //��Ӻ���

	// duanxiaohui 2011-11-11 ��������Ϣ
	LRESULT OnSendAddEmeny(WPARAM wParam, LPARAM lParam);      // ���������
	LRESULT OnSendAddEmeny_R(WPARAM wParam, LPARAM lParam);    // ���������
	// end duanxiaohui 

    // PengJiLin, 2011-6-24, ������Ϣ
    LRESULT OnActiveToRoom(WPARAM wParam, LPARAM lParam);
	// zht, 2011-11-21, ��Ϸ��������Ϣ
	LRESULT OnGameWinMove(WPARAM wParam, LPARAM lParam);

	//JianGuankun,2016-6-28,��Ϸ�ı�ֱ�����Ϣ
	LRESULT OnScreenSizeChange(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	CSkinMgr m_skinmgr;
};

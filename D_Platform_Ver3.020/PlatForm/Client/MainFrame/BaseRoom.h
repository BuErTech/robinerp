#pragma once
#ifndef BASEROOM_HEAD_H
#define BASEROOM_HEAD_H

#include "GamePlace.h"
#include "NewControl\\TStatic.h"
#include "SkinMgr.h"
/*******************************************************************************************************/

//���ӶԻ�����
class CConnectDlg : public CGameFaceGo
{
	//��������
protected:
	CNormalBitmapButtonEx				m_Quit;								//�˳���ť
	CTransparentStatic				m_Static_1;
	COLORREF						m_bkcolor;

    CGameImage  m_CbackPic;     //����ͼ    PengJiLin, 2010-5-14

	//��������
public:
	//���캯��
	CConnectDlg(); 
	//��������
	virtual ~CConnectDlg();
	//DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange * pDX);    
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ��
	virtual void OnOK() { return; }
	//ȡ��
	virtual void OnCancel() { return; }
	//������Ϣ��ʾ
	bool SetMessageText(const TCHAR * szMessage);

	//��дCenterWindowʹ����Ļ���У����游���ھ���
	//JianGuankun,2012.7.26
	void CenterWindow(CWnd* pAlternateOwner = NULL);

private:
	CSkinMgr m_skinmgr;

private:
	LRESULT OnExchangeSkin(WPARAM wpara,LPARAM lpara);

private:
	void Init();

	void LoadSkin();
public:
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//�˳���ť��Ϣ
	afx_msg void OnQuit();
	afx_msg void OnPaint();
	//�ؼ���ɫ��Ϣ
	LRESULT OnControlColor(WPARAM wparam, LPARAM lparam);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()
};

/*******************************************************************************************************/

//���������ӿ�
interface IBaseGameRoom
{
	//��������
public:
	//ѯ�ʹر�
	virtual bool CanCloseNow()=0;
	//�رմ���
	virtual bool CloseGameRoom()=0;
	//ȡ��������Ϣ
	virtual bool OnCancelConnectEvent()=0;
	//λ�ñ仯
	virtual bool OnSizeEvent(UINT nSizeType, int cx, int cy)=0;
	//�������Ϣ
	virtual bool OnSplitEvent(UINT uSplitID, UINT uXPos, UINT uYPos)=0;
	//����λ��
	virtual void FixControlStation(int iWidth, int iHeight)=0;
};

/*******************************************************************************************************/

//��������� 
class CBaseRoom  : public CGameFaceGo , public IClientSocketService, public ISocketSendInterface, public IBaseGameRoom
{
	//��������
public:
	CString										m_strTitle;						//������Ϣ
	CConnectDlg									m_Connect;						//������Ϣ��
	CTCPClientSocket*							m_TCPSocket;					//���� SOCKET

	//��������
public:
	//���캯��
	CBaseRoom(UINT uDialogTemplata); 
	//��������
	virtual ~CBaseRoom();

	//���غ���
public:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK() { return; }
	//ȡ������
	virtual void OnCancel() { return; }

	//��Ϣ����
public:
	//λ�ñ仯
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//�������Ϣ
	LRESULT OnSplitMessage(WPARAM wparam, LPARAM lparam);

	//�ӿں���
protected:
	//����������Ϣ����
	inline virtual int SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
	//����������Ϣ����
	inline virtual int SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);

	DECLARE_MESSAGE_MAP()

public:
	//���ͺ���
	virtual int SendData(void * pData, UINT uSize, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);
	//������ͺ���
	virtual int SendData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode);

	virtual int CloseSocket(bool bNotify=true);
};

#endif
/*******************************************************************************************************/

// TCPDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TCP.h"
#include "TCPDlg.h"
#include "CenterServerhead.h"
#include "MD5.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTCPDlg �Ի���




CTCPDlg::CTCPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTCPDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTCPDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CTCPDlg ��Ϣ�������

BOOL CTCPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_HIDE);

	m_LongoDlg.Create(IDD_DIALOG1, this);
	m_GameManageDlg.Create(IDD_DIALOG2, this);

	m_GameManageDlg.m_pLogonInfo = &m_zSocket.m_LogonInfo;
	m_GameManageDlg.m_AwardType	= &m_zSocket.m_AwardType;
	m_GameManageDlg.m_PropList = &m_zSocket.m_PropList;

	

	m_bSocket.ConnectToServer();

	
	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTCPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTCPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CTCPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTCPDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	

}

LRESULT CTCPDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch(message)
	{
	case MSG_CMD_GET_GAMEPARA:
		{
			m_zSocket.m_ServerInfo = *((CenterServerMsg*)wParam);
			m_zSocket.ConnectToServer();
			break;
		}
	case MSG_CMD_CONNET_Z_SUCCEED:
		{
			m_LongoDlg.ShowWindow(SW_SHOW);
			break;
		}
	case MSG_CMD_CONNET_Z_FAIL:
		{
			m_LongoDlg.ShowWindow(SW_SHOW);
			break;
		}
	case MSG_CMD_LONGIN:
		{
			m_zSocket.m_UerseName = *((CString*)wParam);
			m_zSocket.m_PassWord = *((CString*)lParam);
			m_zSocket.Longo();
			break;
		}
	case MSG_CMD_GET_GAMEKIND:
		{
			m_GameManageDlg.ShowWindow(SW_SHOW);
			m_GameManageDlg.OnGetGameKindList(m_zSocket.m_KindList);
			break;
		}
	case MSG_CMD_GET_GAMENAME:
		{
			m_GameManageDlg.OnGetGameNameList(m_zSocket.m_NameList);
			break;
		}
	case MSG_CMD_GET_GAMEROOM:
		{
			m_GameManageDlg.OnGetGameRoomList(m_zSocket.m_RoomList);
			break;
		}
	case MSG_CMD_UPDATE_GAMEROOM:
		{
			UINT kindid = *((UINT*)wParam);
			UINT roomid = *((UINT*)lParam);
			m_zSocket.GetGameRoom(kindid, roomid);
			break;
		}

	case MSG_CMD_UPDATE_GAMEPEO:
		{
			m_GameManageDlg.UpdatePeoCount();
			break;
		}
	case MSG_CMD_OPERATE:
		{
			//m_wSocket->set
			m_zSocket.SendOperate((GM_OperateByID_t*)wParam);
			int roomID = *((int*)lParam);
			if (roomID != 0)
			{
				m_GameManageDlg.SendOperaToW((GM_OperateByID_t*)wParam, roomID);
			}
			break;
		}
	case MSG_CMD_GM_TALK:
		{
			bool *issystem = (bool*)lParam;
			m_GameManageDlg.OnGMTalkMsg((GM_Broadcast_t*)wParam, *issystem);
			delete issystem;
			break;
		}
	case MSG_CMD_GM_FINDUSER:
		{
			m_zSocket.SendFindUser((GM_GetOperationByPlayer_t*)wParam);
			break;
		}
	case MSG_CMD_GM_GETUSER:
		{
			m_GameManageDlg.OnGetUser((GM_OperatedPlayerInfo_t*)wParam);
			break;
		}
	case MSG_CMD_BROADCAST:
		{
			m_zSocket.SendTalk((GM_Broadcast_t*)wParam);
			break;
		}
	
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CTCPDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	CDialog::OnCancel();
}

BOOL CTCPDlg::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CDialog::Create(lpszTemplateName, pParentWnd);
	ShowWindow(SW_HIDE);
}

BOOL CTCPDlg::Create(UINT lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CDialog::Create(lpszTemplateName, pParentWnd);
	ShowWindow(SW_HIDE);
}
BOOL CTCPDlg::DestroyWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	return CDialog::DestroyWindow();
}

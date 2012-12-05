// GameManageDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TCP.h"
#include "GameManageDlg.h"

#include "cfgEngine.h"
#include "wCore2.h"


// CGameManageDlg �Ի���

IMPLEMENT_DYNAMIC(CGameManageDlg, CDialog)

CGameManageDlg::CGameManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGameManageDlg::IDD, pParent)
	, m_TalkMsg(_T(""))
{
	m_OperaState = 0;
	m_AwardType = NULL;
	m_PropList = NULL;
	m_pCurrentRoom = NULL;
	m_BackOpera = NULL;
}

CGameManageDlg::~CGameManageDlg()
{
	for(int i = 0; i < m_ChatDlgs.size(); i++) delete m_ChatDlgs[i];
	CGameKindItem* p1;
	CGameNameItem *p2;
	CGameRoomItem *p3;
	for (int i = 0; i < m_GameRootItem.m_GameKindList.size(); i++)
	{
		p1 = m_GameRootItem.m_GameKindList[i];
		for (int j = 0; j < p1->m_GameNameList.size(); j++)
		{
			p2 = p1->m_GameNameList[j];
			for (int k = 0; k < p2->m_GameRoomList.size(); k++)
			{
				p3 = p2->m_GameRoomList[k];
				if (p3->m_uDataType == GLK_GAME_NONE) delete p3->m_RoomInfo;
				delete p3;
			}
			delete p2;
		}
		delete p1;
	}
}

void CGameManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_GameListTreeCtrl);
	DDX_Control(pDX, IDC_LIST1, m_GameDiskListCtrl);
	DDX_Control(pDX, IDC_LIST2, m_GameUeserListCtrl);
	DDX_Text(pDX, IDC_EDIT2, m_TalkMsg);
	DDX_Control(pDX, IDC_EDIT2, m_MsgEdit);
}


BEGIN_MESSAGE_MAP(CGameManageDlg, CDialog)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE1, &CGameManageDlg::OnTvnItemexpandingTree1)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CGameManageDlg::OnNMDblclkTree1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CGameManageDlg::OnNMRclickList4)
	ON_BN_CLICKED(IDOK, &CGameManageDlg::OnBnClickedOk)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CGameManageDlg::OnNMRclickList1)
	ON_BN_CLICKED(IDC_BUTTON2, &CGameManageDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CGameManageDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CGameManageDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDCANCEL, &CGameManageDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON6, &CGameManageDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CGameManageDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON10, &CGameManageDlg::OnBnClickedPrintLog)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CGameManageDlg::OnNMRclickTree1)
	ON_WM_CTLCOLOR()
//	ON_NOTIFY(NM_RDBLCLK, IDC_TREE1, &CGameManageDlg::OnNMRdblclkTree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CGameManageDlg::OnTvnSelchangedTree1)
END_MESSAGE_MAP()


// CGameManageDlg ��Ϣ�������

BOOL CGameManageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//��Ȩ���
	long timeStamp=coreGetTimeStamp();

	long licExpires=coreGetLicenseExpires();

	if(timeStamp>licExpires)
	{
		CString s,code=coreGetCode();
		s.Format("���ķ�����δע����ѹ��ڣ������������ϵ��\n\n�뽫���»����뷢�͸������̣���ȡע�����ļ���\n\n%s\n\n",code);
		MessageBox(s,"��ʾ",MB_ICONERROR);
		s="�������Ѹ��Ƶ����ļ������У�ֱ��Ctrl+Vճ�������룡";
		MessageBox(s,"��ʾ",MB_ICONINFORMATION);
		OpenClipboard();
		EmptyClipboard();
		HANDLE hData=GlobalAlloc(GMEM_MOVEABLE,code.GetLength()+5); 
		if (hData==NULL)  
		{ 
			CloseClipboard(); 
			return TRUE; 
		}
		LPTSTR szMemName=(LPTSTR)GlobalLock(hData); 
		lstrcpy(szMemName,code); 
		SetClipboardData(CF_TEXT,hData); 
		GlobalUnlock(hData);  
		GlobalFree(hData); 
		CloseClipboard(); 
		PostQuitMessage(0);
		DestroyWindow();
		return FALSE;
	}

	
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW); 
	CenterWindow();
	
	DWORD dwstyleEX = m_GameDiskListCtrl.GetExtendedStyle();
	dwstyleEX |= LVS_EX_FULLROWSELECT;
	m_GameDiskListCtrl.SetExtendedStyle(dwstyleEX);

	dwstyleEX = m_GameUeserListCtrl.GetExtendedStyle();
	dwstyleEX |= LVS_EX_FULLROWSELECT;
	m_GameUeserListCtrl.SetExtendedStyle(dwstyleEX);

	LONG lStyle;
	lStyle = GetWindowLong(m_GameDiskListCtrl.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	lStyle |= LVS_REPORT; //����style
	SetWindowLong(m_GameDiskListCtrl.m_hWnd, GWL_STYLE, lStyle);//����styl

	lStyle;
	lStyle = GetWindowLong(m_GameUeserListCtrl.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	//lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	lStyle |= LVS_REPORT; //����style
	SetWindowLong(m_GameUeserListCtrl.m_hWnd, GWL_STYLE, lStyle);//����styl

	m_GameUeserListCtrl.InsertColumn(0, "ID", LVCFMT_CENTER, 50);
	m_GameUeserListCtrl.InsertColumn(1, "�ǳ�", LVCFMT_CENTER, 70);
	m_GameUeserListCtrl.InsertColumn(2, "״̬", LVCFMT_CENTER, 70);
	m_GameUeserListCtrl.InsertColumn(3, "����", LVCFMT_CENTER, 70);

	m_GameDiskListCtrl.InsertColumn(0, "����ID", LVCFMT_CENTER, 100);
	m_GameDiskListCtrl.InsertColumn(1, "�������", LVCFMT_CENTER, 127);
	m_GameDiskListCtrl.InsertColumn(1, "״̬", LVCFMT_CENTER, 50);

	m_GameRootItem.m_hCurrentItem = m_GameListTreeCtrl.InsertItem("��Ϸ�б�", NULL, NULL);
	m_GameListTreeCtrl.SetItemData(m_GameRootItem.m_hCurrentItem, LPARAM(&m_GameRootItem));


	m_wSocket.m_hwnd = m_hWnd;
	m_BwSocket.m_hwnd = m_hWnd;
	m_LockDeskDlg.m_hWnd = m_hWnd;

	m_hBrush = CreateSolidBrush(RGB(255,255,255));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CGameManageDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	PostQuitMessage(0);
	CDialog::OnCancel();
}



bool CGameManageDlg::OnGetGameKindList(vector<ComKindInfo*> list)
{
	//��Ȩ���
	long timeStamp=coreGetTimeStamp();

	long licExpires=coreGetLicenseExpires();

	if(timeStamp>licExpires)
	{
		CString s,code=coreGetCode();
		s.Format("���ķ�����δע����ѹ��ڣ������������ϵ��\n\n�뽫���»����뷢�͸������̣���ȡע�����ļ���\n\n%s\n\n",code);
		MessageBox(s,"��ʾ",MB_ICONERROR);
		s="�������Ѹ��Ƶ����ļ������У�ֱ��Ctrl+Vճ�������룡";
		MessageBox(s,"��ʾ",MB_ICONINFORMATION);
		OpenClipboard();
		EmptyClipboard();
		HANDLE hData=GlobalAlloc(GMEM_MOVEABLE,code.GetLength()+5); 
		if (hData==NULL)  
		{ 
			CloseClipboard(); 
			return true; 
		}
		LPTSTR szMemName=(LPTSTR)GlobalLock(hData); 
		lstrcpy(szMemName,code); 
		SetClipboardData(CF_TEXT,hData); 
		GlobalUnlock(hData);  
		GlobalFree(hData); 
		CloseClipboard(); 
		PostQuitMessage(0);
		DestroyWindow();
		return false;
	}

	for (int i = 0; i < list.size(); i++)
	{
		if (m_GameRootItem.GetGameKindItem(list[i]->uKindID) == NULL)
		{
			
			TV_INSERTSTRUCT InsertInf;
			memset(&InsertInf,0,sizeof(InsertInf));
			InsertInf.item.cchTextMax = 100;
			InsertInf.hInsertAfter = TVI_LAST;
			InsertInf.item.mask = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;
			InsertInf.item.pszText = list[i]->szKindName;
			
			InsertInf.hParent = m_GameRootItem.m_hCurrentItem;

			CGameKindItem* p = new CGameKindItem;
			p->m_GameRoot = &m_GameRootItem;
			p->m_KindInfo = list[i];
			InsertInf.item.lParam = (LPARAM)(p);
			p->m_hCurrentItem = m_GameListTreeCtrl.InsertItem(&InsertInf);
			m_GameRootItem.m_GameKindList.push_back(p);
		}
	}
	UpdatePeoCount();
	m_GameListTreeCtrl.Invalidate();
	return true;
}


bool CGameManageDlg::OnGetGameNameList(vector<ComNameInfo*> list)
{
	for (int i = 0; i < list.size(); i++)
	{
		CGameKindItem *tmp = m_GameRootItem.GetGameKindItem(list[i]->uKindID);
		if (tmp != NULL)
		{
			if (tmp->GetGameNameItem(list[i]->uNameID) == NULL)
			{
					TV_INSERTSTRUCT InsertInf;
					memset(&InsertInf,0,sizeof(InsertInf));
					InsertInf.item.cchTextMax = 100;
					InsertInf.hInsertAfter = TVI_LAST;
					InsertInf.item.mask = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;
					InsertInf.item.pszText = list[i]->szGameName;
					
					InsertInf.hParent = tmp->m_hCurrentItem; 

					CGameNameItem *p = new CGameNameItem;
					p->m_GameKind = tmp;
					p->m_NameInfo = list[i];
					InsertInf.item.lParam = (LPARAM)(p);
					p->m_hCurrentItem = m_GameListTreeCtrl.InsertItem(&InsertInf);
					tmp->m_GameNameList.push_back(p);

					CGameRoomItem *pRoom = new CGameRoomItem;
					pRoom->m_uDataType = GLK_GAME_NONE;
					pRoom->m_GameName = p;
					pRoom->m_RoomInfo = new ComRoomInfo;
					pRoom->m_hCurrentItem = m_GameListTreeCtrl.InsertItem("û�з���(0)", p->m_hCurrentItem);
					p->m_GameRoomList.push_back(pRoom);
					AfxGetMainWnd()->PostMessage(MSG_CMD_UPDATE_GAMEROOM, (WPARAM)(&(p->m_NameInfo->uKindID)), (LPARAM)(&(p->m_NameInfo->uNameID)));
			}
		}
	}
	UpdatePeoCount();
	m_GameListTreeCtrl.Invalidate();
	return true;
}


bool CGameManageDlg::OnGetGameRoomList(vector<ComRoomInfo*> list)
{
	for (int i = 0; i < list.size(); i++)
	{
		CGameKindItem *tmp = m_GameRootItem.GetGameKindItem(list[i]->uKindID);
		if (tmp != NULL)
		{
			CGameNameItem* tmp1 = tmp->GetGameNameItem(list[i]->uNameID);
			if (tmp1 != NULL && tmp1->GetGameRoomItem(list[i]->uRoomID) == NULL)
			{
				TV_INSERTSTRUCT InsertInf;
				memset(&InsertInf,0,sizeof(InsertInf));
				InsertInf.item.cchTextMax = 100;
				InsertInf.hInsertAfter = TVI_LAST;
				InsertInf.item.mask = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;
				
				TCHAR szBuffer[100];
				ZeroMemory(szBuffer,sizeof(szBuffer));
				//wsprintf(szBuffer,TEXT("%s(%d)"),list[i]->szGameRoomName,list[i]->uPeopleCount);
				InsertInf.item.pszText = list[i]->szGameRoomName;
				
				InsertInf.hParent = tmp1->m_hCurrentItem; 

				CGameRoomItem *p = new CGameRoomItem;
				p->m_GameName = tmp1;
				p->m_RoomInfo = list[i];
				InsertInf.item.lParam = (LPARAM)(p);
				p->m_hCurrentItem = m_GameListTreeCtrl.InsertItem(&InsertInf);

				if (tmp1->m_GameRoomList[0]->m_uDataType == GLK_GAME_NONE) 
				{
					m_GameListTreeCtrl.DeleteItem(tmp1->m_GameRoomList[0]->m_hCurrentItem);
					delete tmp1->m_GameRoomList[0]->m_RoomInfo;
					delete tmp1->m_GameRoomList[0];
					tmp1->m_GameRoomList.clear();
				}
				tmp1->m_GameRoomList.push_back(p);
			}
		}
	}
	UpdatePeoCount();
	m_GameListTreeCtrl.Invalidate();
	return true;
}



bool CGameManageDlg::UpdatePeoCount()
{
	CGameKindItem *pKind;
	CGameNameItem *pName;
	CGameRoomItem *pRoom;
	CString s;
	int RootPeoCount(0);
	for (int i = 0; i < m_GameRootItem.m_GameKindList.size(); i++)
	{
		pKind = m_GameRootItem.m_GameKindList[i];
		int KindPeoCount(0);
		for (int j = 0; j < pKind->m_GameNameList.size(); j++)
		{
			pName = pKind->m_GameNameList[j];
			int NamePeoCount(0);
			for (int k = 0; k < pName->m_GameRoomList.size(); k++)
			{
				pRoom = pName->m_GameRoomList[k];
				if (pRoom->m_uDataType != GLK_GAME_NONE)
				{
					s.Format("%s(%d)", pRoom->m_RoomInfo->szGameRoomName, pRoom->m_RoomInfo->uPeopleCount);
					NamePeoCount += pRoom->m_RoomInfo->uPeopleCount;
				}
				else
					s = "û�з���(0)";
				m_GameListTreeCtrl.SetItemText(pRoom->m_hCurrentItem, s);
			}
			
			NamePeoCount = max(NamePeoCount, pName->m_NameInfo->m_uOnLineCount);
			KindPeoCount += NamePeoCount;
			s.Format("%s(%d)", pName->m_NameInfo->szGameName, NamePeoCount);
			m_GameListTreeCtrl.SetItemText(pName->m_hCurrentItem, s);
		}
		s.Format("%s(%d)", pKind->m_KindInfo->szKindName, KindPeoCount);
		m_GameListTreeCtrl.SetItemText(pKind->m_hCurrentItem, s);
		RootPeoCount += KindPeoCount;
	}
	s.Format("%s(%d)", "��Ϸ�б�", RootPeoCount);
	m_GameListTreeCtrl.SetItemText(m_GameRootItem.m_hCurrentItem, s);
	return true;
}

bool CGameManageDlg::OnGMTalkMsg(GM_Broadcast_t *pMsg, bool isystem)
{
	for (int i = 0; i < m_ChatDlgs.size(); i++)
	{
		if (m_ChatDlgs[i]->m_iUserID == pMsg->iUserID)
		{
			
			m_ChatDlgs[i]->AddMsg(pMsg->szMsg, isystem);
			return true;
		}
	}
	CString s;
	if (pMsg->iUserID == 0)
		s.Format("ϵͳ��Ϣ: %s", pMsg->szMsg);
	else
		s.Format("%d: %s", pMsg->iUserID, pMsg->szMsg);
	AddMsg(s);
}

void CGameManageDlg::AddMsg(CString msg)
{
	m_TalkMsg += msg + "\r\n";
	if (m_TalkMsg.GetLength() > 50000)
		m_TalkMsg = m_TalkMsg.Right(10000);
	UpdateData(false);
	m_MsgEdit.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
}

void CGameManageDlg::OnGetUser(GM_OperatedPlayerInfo_t* user)
{
	switch (m_OperaState)
	{
	case 1:
		{
			m_AWardDlg.SetUserInfo(user);
			m_AWardDlg.SetAwardList(*m_PropList);
			m_AWardDlg.SetAwardProjectList(*m_AwardType);
			m_AWardDlg.DoModal();
			break;
		}
	case 2:
	case 3:
	case 4:
		{
			m_OperateDlg.SetUserInfo(user);
			m_OperateDlg.SetOperateType(m_OperaState);
			m_OperateDlg.DoModal();
			break;
		}
	case 5:
		{
			if (user->ZID != 0)
			{
				bool flag = false;
				for (vector<CChatDlg*>::iterator i = m_ChatDlgs.begin(); i != m_ChatDlgs.end();)
				{
					if ((*i)->m_iUserID  != -1 && (*i)->m_iUserID == user->iUserID) 
					{
						flag = true;
						(*i)->ShowWindow(SW_SHOW);
					}
					if ((*i)->m_iUserID  == -1) 
					{
						(*i)->DestroyWindow(); 
						delete (*i);
						i = m_ChatDlgs.erase(i);
					}
					else i++;

				}
				if (!flag)
				{
					CChatDlg* ChatDlg = new CChatDlg;
					ChatDlg->SetUserInfo(user);
					ChatDlg->Create(IDD_DIALOG6, this);
					ChatDlg->ShowWindow(SW_SHOW);
					m_ChatDlgs.push_back(ChatDlg);
				}
			}
			else
			{
				AfxMessageBox("���û������ߣ�");
			}
			break;
		}
	case 6:
		{
			m_BroadDlg.DoModal();
			break;
		}

	}
}


void CGameManageDlg::SendOperaToW(GM_OperateByID_t* opera, int iRoomID)
{
	if (m_pCurrentRoom != NULL && iRoomID == m_pCurrentRoom->m_RoomInfo->uRoomID)
	{
		m_wSocket.SendOpera(opera);
	}
	else
	{
		m_BackOpera = opera;
		m_BackRoomID = iRoomID;
		m_BwSocket.closesocket();

		CGameRoomItem* pItem;
		CGameKindItem* p1;
		CGameNameItem *p2;
		CGameRoomItem *p3;
		for (int i = 0; i < m_GameRootItem.m_GameKindList.size(); i++)
		{
			p1 = m_GameRootItem.m_GameKindList[i];
			for (int j = 0; j < p1->m_GameNameList.size(); j++)
			{
				p2 = p1->m_GameNameList[j];
				for (int k = 0; k < p2->m_GameRoomList.size(); k++)
				{
					p3 = p2->m_GameRoomList[k];
					if (p3->m_RoomInfo->uRoomID == iRoomID)
					{
						pItem = p3;
					}
				}
			}
		}

		m_BwSocket.m_pLogonInfo = m_pLogonInfo;
		m_BwSocket.m_pRoomInfo = pItem->m_RoomInfo;
		m_BwSocket.ConnectToServer();
	}
}

bool CGameManageDlg::ShowGameDesk()
{
	m_GameDiskListCtrl.DeleteAllItems();
	CString s;
	for (int i = 0; i < m_wSocket.m_pRoomInfo->uDeskCount; i++)
	{
		s.Format("%.3d", i + 1);
		m_GameDiskListCtrl.InsertItem(i, s);
	}
	ShowGameDeskStation();
	return true;
}

bool CGameManageDlg::ShowGameUeser()
{
	m_GameUeserListCtrl.DeleteAllItems();
	CString s;
	for (int i = 0; i < m_wSocket.m_UserManage.GetOnLineUserCount(); i++)
	{
		s.Format("%.3d", m_wSocket.m_UserManage.FindOnLineUserByIndex(i)->GameUserInfo.dwUserID);
		int r = m_GameUeserListCtrl.InsertItem(i, s);
		m_GameUeserListCtrl.SetItemText(r, 1, m_wSocket.m_UserManage.FindOnLineUserByIndex(i)->GameUserInfo.nickName);
		m_GameUeserListCtrl.SetItemText(r, 2, StateToString(m_wSocket.m_UserManage.FindOnLineUserByIndex(i)->GameUserInfo.bUserState));
		if (m_wSocket.m_UserManage.FindOnLineUserByIndex(i)->GameUserInfo.bDeskNO != 255) 
			s.Format("%.3d", m_wSocket.m_UserManage.FindOnLineUserByIndex(i)->GameUserInfo.bDeskNO + 1);
		else
			s = "";
		m_GameUeserListCtrl.SetItemText(r, 3, s);
	}
	ShowGameDeskStation();
	return true;
}

bool CGameManageDlg::ShowGameDeskStation()
{
	//ShowGameDesk();
	
	for (int i = 0; i < m_wSocket.m_pRoomInfo->uDeskCount; i++)
	{
		int usercount = m_wSocket.m_UserManage.GetDeskPlayCount(i);
		bool playing(false);
		for (int j = 0; j < m_wSocket.m_DeskStation.vPlayings.size(); j++)
		{
			if (m_wSocket.m_DeskStation.vPlayings[j] == i) playing = true;
		}
		if (playing)//(usercount >= m_wSocket.m_pRoomInfo->uDeskPeople)
			m_GameDiskListCtrl.SetItemText(i, 1, "��Ϸ��");
		else
		{
			int usercount = m_wSocket.m_UserManage.GetDeskUserCount(i);
			if (0 < usercount && usercount <= m_wSocket.m_pRoomInfo->uDeskPeople)
				m_GameDiskListCtrl.SetItemText(i, 1, "δ��ʼ");
			else m_GameDiskListCtrl.SetItemText(i, 1, "");
		}
		CString s;
		s.Format("%d/%d", m_wSocket.m_UserManage.GetDeskUserCount(i), m_wSocket.m_pRoomInfo->uDeskPeople);
		m_GameDiskListCtrl.SetItemText(i, 2, s);
	}
	for (int i = 0; i < m_wSocket.m_DeskStation.vVirtuals.size(); i++)
	{
		CString s;
		s.Format("%d/%d", m_wSocket.m_pRoomInfo->uDeskPeople, m_wSocket.m_pRoomInfo->uDeskPeople);
		m_GameDiskListCtrl.SetItemText(m_wSocket.m_DeskStation.vVirtuals[i], 2, s);
		m_GameDiskListCtrl.SetItemText(m_wSocket.m_DeskStation.vVirtuals[i], 1, "������");
	}
	return true;
}

void CGameManageDlg::OnTvnItemexpandingTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (pNMTreeView->action==TVE_EXPAND)
	{
		CNodeType* p = (CNodeType*)m_GameListTreeCtrl.GetItemData(pNMTreeView->itemNew.hItem);
		if (p->m_uDataType == GLK_GAME_NAME)
		{
			CGameNameItem* pItem = (CGameNameItem*)p;
			AfxGetMainWnd()->PostMessage(MSG_CMD_UPDATE_GAMEROOM, (WPARAM)(&(pItem->m_NameInfo->uKindID)), (LPARAM)(&(pItem->m_NameInfo->uNameID)));
		}
	}
	
	*pResult = 0;
}

void CGameManageDlg::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM pNMTreeView = m_GameListTreeCtrl.GetSelectedItem();

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (pNMTreeView != NULL)
	{
		CNodeType* p = (CNodeType*)m_GameListTreeCtrl.GetItemData(pNMTreeView);
		if (p != NULL)
		{
			if (p->m_uDataType == GLK_GAME_ROOM)
			{
				CGameRoomItem* pItem = (CGameRoomItem*)p;
				if (m_pCurrentRoom != pItem)
				{
					m_pCurrentRoom = pItem;
					m_wSocket.CloseSocket();
					SetTimer(10000, 1000, NULL);
				}
			}
		}
		*pResult = 0;
	}
}


void CGameManageDlg::OnNMRclickList4(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//m_GameUeserListCtrl.Getse

	CMenu PopMenu;
	CString strMenu;
	PopMenu.CreatePopupMenu();
	CPoint Point;
	GetCursorPos(&Point);
	m_GameUeserListCtrl.ScreenToClient(&Point);
	m_ListIndex2 = m_GameUeserListCtrl.HitTest(Point);
	if (0 <= m_ListIndex2 && m_ListIndex2 < m_wSocket.m_UserManage.GetOnLineUserCount())
	{
		 PopMenu.AppendMenu(0, MSG_CMD_GM_ACT_NO_TALK, TEXT("��ֹ����"));
		 PopMenu.AppendMenu(0, MSG_CMD_GM_ACT_KICK_OUT, TEXT("�߳�����"));
		 PopMenu.AppendMenu(0, MSG_CMD_GM_ACT_BANDON_ID, TEXT("��ֹ��¼"));
		 PopMenu.AppendMenu(0, MSG_CMD_CM_START_CHAT, TEXT("��������"));
		 PopMenu.AppendMenu(0,  MSG_CMD_GM_ACT_AWORD, TEXT("����"));
		 //��ʾ�˵�

		 GetCursorPos(&Point);
		 PopMenu.TrackPopupMenu(TPM_RIGHTBUTTON, Point.x, Point.y, this);
		 PopMenu.DestroyMenu();
	}

	*pResult = 0;
}

BOOL CGameManageDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch (LOWORD(wParam))
	{
	case MSG_CMD_GM_ACT_NO_TALK:
	case MSG_CMD_GM_ACT_KICK_OUT:
//	case MSG_CMD_GM_ACT_KICK_OUT:
	case MSG_CMD_GM_ACT_BANDON_ID:
		{
			m_OperateDlg.SetUserInfo(&(m_wSocket.m_UserManage.FindOnLineUserByIndex(m_ListIndex2)->GameUserInfo), m_pCurrentRoom->m_RoomInfo->uRoomID);
			m_OperateDlg.SetOperateType(LOWORD(wParam) - MSG_CMD_GM_ACT_AWORD + 1);
			m_OperateDlg.DoModal();
			break;
		}
	case MSG_CMD_GM_ACT_AWORD:
		{	
			m_AWardDlg.SetUserInfo(&(m_wSocket.m_UserManage.FindOnLineUserByIndex(m_ListIndex2)->GameUserInfo), m_pCurrentRoom->m_RoomInfo->uRoomID);
			m_AWardDlg.SetAwardList(*m_PropList);
			m_AWardDlg.SetAwardProjectList(*m_AwardType);
			m_AWardDlg.DoModal();
			break;
		}
	case MSG_CMD_CM_START_CHAT:
		{
			
			bool flag = false;
			for (vector<CChatDlg*>::iterator i = m_ChatDlgs.begin(); i != m_ChatDlgs.end();)
			{
				if ((*i)->m_iUserID != -1 && (*i)->m_iUserID == m_wSocket.m_UserManage.FindOnLineUserByIndex(m_ListIndex2)->GameUserInfo.dwUserID) 
				{
					flag = true;
					(*i)->ShowWindow(SW_SHOW);
				}
				if ((*i)->m_iUserID == -1) 
				{
					(*i)->DestroyWindow();
					delete (*i);
					i = m_ChatDlgs.erase(i);
				}
				else i++;

			}
			if (!flag)
			{
				CChatDlg* ChatDlg = new CChatDlg;
				ChatDlg->SetUserInfo(&(m_wSocket.m_UserManage.FindOnLineUserByIndex(m_ListIndex2)->GameUserInfo), m_pCurrentRoom->m_RoomInfo->uRoomID);
				ChatDlg->Create(IDD_DIALOG6, this);
				ChatDlg->ShowWindow(SW_SHOW);
				m_ChatDlgs.push_back(ChatDlg);
			}
			break;
		}
	case MSG_CMD_GM_LOCK:
		{
			m_wSocket.LockDesk(m_pLogonInfo->dwUserID, m_ListIndex1, 1);
			break;
		}
	case MSG_CMD_GM_UNLOCK:
		{
			m_wSocket.LockDesk(m_pLogonInfo->dwUserID, m_ListIndex1, 2);
			break;
		}
	case MSG_CMD_GM_ALLLOCK:
		{
			m_LockDeskDlg.SetInfo(1, 1, m_pCurrentRoom->m_RoomInfo->uDeskCount);
			m_LockDeskDlg.DoModal();
			break;
		}
	case MSG_CMD_GM_ALLUNLOCK:
		{
			m_LockDeskDlg.SetInfo(2, 1, m_pCurrentRoom->m_RoomInfo->uDeskCount);
			m_LockDeskDlg.DoModal();
			break;
		}
	case MSG_CMD_GM_PRINTLOG:
		{
			HTREEITEM hSel = m_GameListTreeCtrl.GetSelectedItem();
			CNodeType* pType = (CNodeType*)m_GameListTreeCtrl.GetItemData(hSel);
			if (pType->m_uDataType == GLK_GAME_ROOM)
			{
				CGameRoomItem* pRoom = (CGameRoomItem*)pType;
				GM_MSG_PrintLog_t tagPrintLog;
				tagPrintLog.uRoomID = pRoom->m_RoomInfo->uRoomID;
				m_wSocket.SendPrintLogCmd(&tagPrintLog);
			}
		
		}
		break;;
	}
	return CDialog::OnCommand(wParam, lParam);
}

LRESULT CGameManageDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch (message)
	{
	case MSG_CMD_SHOW_GAMEROOM:
		{
			if (wParam == 0)
			{
				ShowGameDesk();
				ShowGameUeser();
			}
			else if (wParam == 1)
			{
				int _Index = m_wSocket.m_UserManage.GetIndexByUserID(lParam);
				if (_Index != -1)
				{
					m_GameUeserListCtrl.SetItemText(_Index, 2, StateToString(m_wSocket.m_UserManage.FindOnLineUserByIndex(_Index)->GameUserInfo.bUserState));
				}
			}
			else if (wParam == 2)
			{
				int _Index = m_wSocket.m_UserManage.GetIndexByUserID(lParam);
				if (_Index != -1)
				{
					CString s;
					s.Format("%.3d", m_wSocket.m_UserManage.FindOnLineUserByIndex(_Index)->GameUserInfo.dwUserID);
					int r = m_GameUeserListCtrl.InsertItem(_Index, s);
					m_GameUeserListCtrl.SetItemText(r, 1, m_wSocket.m_UserManage.FindOnLineUserByIndex(_Index)->GameUserInfo.nickName);
					m_GameUeserListCtrl.SetItemText(r, 2, StateToString(m_wSocket.m_UserManage.FindOnLineUserByIndex(_Index)->GameUserInfo.bUserState));
					if (m_wSocket.m_UserManage.FindOnLineUserByIndex(_Index)->GameUserInfo.bDeskNO != 255) 
						s.Format("%.3d", m_wSocket.m_UserManage.FindOnLineUserByIndex(_Index)->GameUserInfo.bDeskNO + 1);
					else
						s = "";
					m_GameUeserListCtrl.SetItemText(r, 3, s);
				}
			}
			else if (wParam == 3)
			{
				m_GameUeserListCtrl.DeleteItem(lParam);
			}
			break;
		}
	case MSG_CMD_SHOW_GAMEDESKSTATE:
		{
			ShowGameDeskStation();
			break;
		}
	case MSG_CMD_BACKWLOGIN:
		{
			m_BwSocket.SendOpera(m_BackOpera);
			break;
		}
	case MSG_CMD_GM_ALLLOCKOK:
		{
			m_wSocket.LockDesk(m_pLogonInfo->dwUserID, *((int*)wParam), *((int*)lParam), 1);
			break;
		}
	case MSG_CMD_GM_ALLUNLOCKOK:
		{
			m_wSocket.LockDesk(m_pLogonInfo->dwUserID, *((int*)wParam), *((int*)lParam), 2);
			break;
		} 
	}
	return CDialog::WindowProc(message, wParam, lParam);
}



void CGameManageDlg::OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMenu PopMenu;
	CString strMenu;
	PopMenu.CreatePopupMenu();
	CPoint Point;
	GetCursorPos(&Point);
	m_GameDiskListCtrl.ScreenToClient(&Point);
	m_ListIndex1 = m_GameDiskListCtrl.HitTest(Point);

	if (0 <= m_ListIndex1 && m_ListIndex1 < m_wSocket.m_pRoomInfo->uDeskCount)
	{
		int flag = -1;
		int uesecount = 0;
		for (int j = 0; j < m_wSocket.m_UserManage.GetOnLineUserCount(); j++)
		{
			if (m_wSocket.m_UserManage.FindOnLineUserByIndex(j)->GameUserInfo.bDeskNO == m_ListIndex1)
			{
				uesecount++;
			}
		}
		if (uesecount == 0) flag = 0; 
		for (int i = 0; i < m_wSocket.m_DeskStation.vVirtuals.size(); i++)
		{
			if (m_ListIndex1 == m_wSocket.m_DeskStation.vVirtuals[i])
			{
				flag = 1;
				break;
			}
		}
		if (flag != -1)
		{
			if (flag == 0)
				PopMenu.AppendMenu(0, MSG_CMD_GM_LOCK, TEXT("����"));
			else if (flag == 1)
				PopMenu.AppendMenu(0, MSG_CMD_GM_UNLOCK, TEXT("���"));
			PopMenu.AppendMenu(0, MSG_CMD_GM_ALLLOCK, TEXT("��������"));
			PopMenu.AppendMenu(0, MSG_CMD_GM_ALLUNLOCK, TEXT("�������"));
			//��ʾ�˵�

			GetCursorPos(&Point);
			//m_GameUeserListCtrl.ClientToScreen(&Point);
			PopMenu.TrackPopupMenu(TPM_RIGHTBUTTON, Point.x, Point.y, this);
			PopMenu.DestroyMenu();
		}
	}
	*pResult = 0;
}

void CGameManageDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//OnOK();
	m_OperaState = 6;
	//m_FindUser.DoModal();
	m_BroadDlg.DoModal();
}


void CGameManageDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_OperaState = 3;
	m_FindUser.DoModal();
}

void CGameManageDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_OperaState = 4;
	m_FindUser.DoModal();
}

void CGameManageDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_OperaState = 4;
	m_FindUser.DoModal();
}

void CGameManageDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_OperaState = 5;
	m_FindUser.DoModal();
}

void CGameManageDlg::OnBnClickedCancel() 
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CGameManageDlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_OperaState = 2;
	m_FindUser.DoModal();
}

void CGameManageDlg::OnBnClickedButton7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_OperaState = 1;
	m_FindUser.DoModal();
}

void CGameManageDlg::OnBnClickedPrintLog()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GM_MSG_PrintLog_t tagPrintLog;
	tagPrintLog.uRoomID = 52;
	m_wSocket.SendPrintLogCmd(&tagPrintLog);
}

void CGameManageDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 10000)
	{
		KillTimer(nIDEvent);
		m_wSocket.m_pLogonInfo = m_pLogonInfo;
		m_wSocket.m_pRoomInfo = m_pCurrentRoom->m_RoomInfo;
		m_wSocket.ConnectToServer();
		ShowGameDesk();
		ShowGameUeser();
		ShowGameDeskStation();
	}
	CDialog::OnTimer(nIDEvent);
}

void CGameManageDlg::OnNMRclickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{

	//HTREEITEM pNMTreeView = m_GameListTreeCtrl.GetSelectedItem();
	CMenu PopMenu;
	CString strMenu;
	PopMenu.CreatePopupMenu();
	CPoint Point;
	GetCursorPos(&Point);
	m_GameListTreeCtrl.ScreenToClient(&Point);
	HTREEITEM pNMTreeView = m_GameListTreeCtrl.HitTest(Point);

	RECT rc;
	GetWindowRect(&rc);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (pNMTreeView != NULL)
	{
		CNodeType* p = (CNodeType*)m_GameListTreeCtrl.GetItemData(pNMTreeView);
		if (p != NULL)
		{
			if (p->m_uDataType == GLK_GAME_ROOM)
			{
// 				PopMenu.AppendMenu(0, MSG_CMD_GM_ALLLOCK, TEXT("����"));
// 				PopMenu.AppendMenu(0, MSG_CMD_GM_ALLUNLOCK, TEXT("���"));
				PopMenu.AppendMenu(0, MSG_CMD_GM_PRINTLOG, TEXT("��ӡ�û�״̬logo"));

				PopMenu.TrackPopupMenu(TPM_RIGHTBUTTON, Point.x + rc.left, Point.y + rc.top, this);
				PopMenu.DestroyMenu();
			}
		}
	}
	*pResult = 0;
}
HBRUSH CGameManageDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	int nID = pWnd->GetDlgCtrlID()   ;   
	if(nID == IDC_EDIT2)       //�����IDC_Edit1��CEdit�ؼ���ID
	{   
		pDC->SetBkColor(RGB(255, 255, 255));   
		pDC->SetTextColor(RGB(0,0,0)); 
		return m_hBrush;   
	}  
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

CString CGameManageDlg::StateToString(int state)
{	
	if (state == 20) return USER_STATESTRING[6];
	else if (state == 21) return USER_STATESTRING[7];
	else return USER_STATESTRING[state];
}
BOOL CGameManageDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	TCHAR nchar = pMsg->wParam;
	if(pMsg->message == WM_KEYDOWN && (nchar==VK_ESCAPE))
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}

//void CGameManageDlg::OnNMRdblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: Add your control notification handler code here
//
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CMenu PopMenu;
//	CString strMenu;
//	PopMenu.CreatePopupMenu();
//	CPoint Point;
//	GetCursorPos(&Point);
//	m_GameListTreeCtrl.ScreenToClient(&Point);
//	m_hGameListItem = m_GameListTreeCtrl.HitTest(Point);
//
//	CNodeType* pNode = (CNodeType*)m_GameListTreeCtrl.GetItemData(m_hGameListItem);
//
// 	if (0 <= m_ListIndex1 && m_ListIndex1 < m_wSocket.m_pRoomInfo->uDeskCount)
// 	{
// 		int flag = -1;
// 		int uesecount = 0;
// 		for (int j = 0; j < m_wSocket.m_UserManage.GetOnLineUserCount(); j++)
// 		{
// 			if (m_wSocket.m_UserManage.FindOnLineUserByIndex(j)->GameUserInfo.bDeskNO == m_ListIndex1)
// 			{
// 				uesecount++;
// 			}
// 		}
// 		if (uesecount == 0) flag = 0; 
// 		for (int i = 0; i < m_wSocket.m_DeskStation.vVirtuals.size(); i++)
// 		{
// 			if (m_ListIndex1 == m_wSocket.m_DeskStation.vVirtuals[i])
// 			{
// 				flag = 1;
// 				break;
// 			}
// 		}
// 		if (flag != -1)
// 		{
// 			if (flag == 0)
// 				PopMenu.AppendMenu(0, MSG_CMD_GM_LOCK, TEXT("����"));
// 			else if (flag == 1)
// 				PopMenu.AppendMenu(0, MSG_CMD_GM_UNLOCK, TEXT("���"));
// 			PopMenu.AppendMenu(0, MSG_CMD_GM_ALLLOCK, TEXT("��������"));
// 			PopMenu.AppendMenu(0, MSG_CMD_GM_ALLUNLOCK, TEXT("�������"));
// 			//��ʾ�˵�
// 
// 			GetCursorPos(&Point);
// 			//m_GameUeserListCtrl.ClientToScreen(&Point);
// 			PopMenu.TrackPopupMenu(TPM_RIGHTBUTTON, Point.x, Point.y, this);
// 			PopMenu.DestroyMenu();
// 		}
// 	}
//
//	*pResult = 0;
//}

void CGameManageDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

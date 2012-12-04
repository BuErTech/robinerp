#include "StdAfx.h"
#include "RightFrame.h"
#include "LoveSendClassInExe.h"

using namespace BzDui;

#include "UserListDlg.h"
#include "MenuDu.h"

#include "UseBoardCastWnd.h"

#define IDT_TRANSPARENT				101		//͸���õĶ�ʱ��
#define IDT_SHOW_TALKBUF			102		//��ʾ������Ϣ����

//�û��б�˵���Ϣ
#define UM_MENU_ULIST				(WM_USER + 100)
#define MMSG_ULIST_INFO				1		//��ϸ��Ϣ
#define MMSG_ULIST_COPYNAME			2		//�����û���
#define MMSG_ULIST_ADDFRIEND		3		//��Ϊ����
#define MMSG_ULIST_ADDBLACKLIST		4		//���������
#define MMSG_ULIST_KICK				5		//����
#define MMSG_ULIST_KICK_PROP		6		//����

//Avatarϵͳ���
#define BzGetAvatarIndex(obj) \
	GetAvatarIndex((obj)->GameUserInfo.bLogoID,(obj)->GameUserInfo.i64Bank,(obj)->GameUserInfo.i64Money,\
		(obj)->GameUserInfo.dwPoint,((CLoveSendClassInExe*)m_pRoom)->m_pGameInfo->uNameID, \
		((CLoveSendClassInExe*)m_pRoom)->m_pGameInfo->uComType)

CRightFrame::CRightFrame(CWnd* pParent)
{
	m_pRoom = pParent;

	//�Ƿ������ظô���
	m_bCanHide;

	//��ǰ��������ʾ��Ϣ���û�
	m_pShowUser = NULL;

	//����б�ؼ�
	m_pUsersList = NULL;

	//���߰�ťȺ
	for (int i = 0; i < MAX_PROP_NUMS; ++i)
	{
		m_pPropBtn[i] = NULL;
	}

	//����ÿҳ������ť������
	m_uPropPageSize = 0;

	//��ǰ��ʾ���ߵ�ҳ
	m_uCurrPropPage = 0;

	//����ҳ��
	m_uPropPageNums = 0;

	//�Ƿ�ټ�����Ϸ
	m_bBJLGame = 0;

	//����Ƿ������˵���
	m_bUseKickUserProp = 0;

	//���ڵ������
	m_uMaxWidth = 0;

	//���ڵ���С���
	m_uMinWidth = 0;

	//����͸����
	m_iTransparent = 255;

	//�����
	m_pTalkBoxList = NULL;

	//������򻺳��ı�
	m_pTalkTempText = NULL;

	//�����������Ϣ
	m_dqTalkBuff.clear();

	//���ȴ���
	m_pBoardCastWnd = NULL;

}

CRightFrame::~CRightFrame(void)
{
	if (m_pUsersList)
	{
		delete m_pUsersList;
		m_pUsersList = NULL;
	}
}

LPCTSTR CRightFrame::GetWindowClassName() const 
{ 
	return _T("UIFrame");
};

UINT CRightFrame::GetClassStyle() const 
{ 
	return UI_CLASSSTYLE_DIALOG; 
};

void CRightFrame::OnFinalMessage(HWND hWnd) 
{
	m_dqTalkBuff.clear();
	m_pm.RemovePreMessageFilter(this);
	delete this;
};

LRESULT CRightFrame::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	m_pm.AddPreMessageFilter(this);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("game\\RightFrame.xml"), (UINT)0, NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	Init();

	::SetTimer(m_hWnd,IDT_SHOW_TALKBUF,200,NULL);

	return 0;
}

LRESULT CRightFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CRightFrame::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CRightFrame::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CRightFrame::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CRightFrame::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = m_pm.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) 
	{
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
			if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 )
				return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CRightFrame::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	FixPropUI(LOWORD(lParam),HIWORD(lParam));
	bHandled = FALSE;
	return 0;
}

LRESULT CRightFrame::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = false;

	if (wParam == IDT_TRANSPARENT)
	{
		m_iTransparent -= 50;

		m_pm.SetTransparent(m_iTransparent);

		if (m_iTransparent <= 20)
		{
			CWindowWnd::ShowWindow(false);
			::KillTimer(m_hWnd,IDT_TRANSPARENT);
			return 0;
		}

	}

	else if (wParam == IDT_SHOW_TALKBUF)
	{

		if (m_dqTalkBuff.size() > 0)
		{
			CString str = m_dqTalkBuff.front();
			m_dqTalkBuff.pop_front();

			m_pTalkTempText->SetVisible(true);
			m_pTalkTempText->SetText(str);

		}

		return 0;
	}

	return 0;
}

LRESULT CRightFrame::OnCmdUserListMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CLoveSendClassInExe* pGameFrame = static_cast<CLoveSendClassInExe*>(m_pRoom);

	switch(wParam)
	{
		case MMSG_ULIST_INFO: //��ϸ��Ϣ
			{
				return pGameFrame->OnCmdShowUserInfo();
			}
			break;

		case MMSG_ULIST_COPYNAME: //�����û���
			{
				return pGameFrame->OnCmdCopyUserInfo();
			}
			break;

		case MMSG_ULIST_ADDFRIEND: //��Ϊ����
			{
				if (m_pShowUser)
				{
					return pGameFrame->m_pGameRoom->PostMessage(WM_FRIEND,0,m_pShowUser->GameUserInfo.dwUserID);
				}
			}
			break;

		case MMSG_ULIST_ADDBLACKLIST: //���������
			{
				if (m_pShowUser)
				{
					return pGameFrame->m_pGameRoom->PostMessage(WM_EMENY,0,m_pShowUser->GameUserInfo.dwUserID);
				}
			}
			break;

		case MMSG_ULIST_KICK: //����
			{
				return pGameFrame->OnCmdKickUser();
			}
			break;

		case MMSG_ULIST_KICK_PROP: //����(����)
			{
				return pGameFrame->OnCmdKickUserProp();
			}
			break;
	}

	return 0;
}


LRESULT CRightFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;

	switch(uMsg) 
	{
		case WM_CREATE:
			{
				 lRes = OnCreate(uMsg, wParam, lParam, bHandled); 
			}
			break;

		case WM_DESTROY:       
			{
				 lRes = OnDestroy(uMsg, wParam, lParam, bHandled);
			}
			break;

		case WM_NCACTIVATE:    
			{
				 lRes = OnNcActivate(uMsg, wParam, lParam, bHandled);
			}
			break;

		case WM_NCCALCSIZE:    
			{
				 lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled);
			}
			break;

		case WM_NCPAINT:       
			{
				lRes = OnNcPaint(uMsg, wParam, lParam, bHandled);
			}
			break;

		case WM_NCHITTEST:     
			{
				 lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled);
			}
			break;

		case WM_SIZE:          
			{
				lRes = OnSize(uMsg, wParam, lParam, bHandled);
			}
			break;

		case WM_TIMER:
			{
				lRes = OnTimer(uMsg, wParam, lParam, bHandled);
			}
			break;

		case UM_MENU_ULIST:
			{
				lRes = OnCmdUserListMenu(uMsg, wParam, lParam, bHandled);
			}
			break;

		default:
			bHandled = FALSE;
	}


// 	if(bHandled)
// 	{
// 		if (uMsg != WM_PAINT)
// 		{
// 			return lRes;
// 		}
// 
// 	}
// 
// 	if(m_pm.MessageHandler(uMsg,wParam,lParam,lRes))
// 	{
// 		if (uMsg != WM_PAINT)
// 		{
// 			return lRes;
// 		}
// 	}

	if(bHandled)
	{
		return lRes;
	}

	if(m_pm.MessageHandler(uMsg, wParam, lParam, lRes))
	{
		return lRes;
	}

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LPCTSTR CRightFrame::GetItemText(BzDui::CControlUI* pControl, int iIndex, int iSubItem)
{
	if (m_pUsersList)
	{
		if (pControl->GetParent()->GetParent() == m_pUsersList->m_pListCtrl)
		{
			CLoveSendClassInExe* pGameFrame = (CLoveSendClassInExe*)m_pRoom;

			if (!pGameFrame)
			{
				return 0;
			}

			bool bSecret = false;

			 UserItemStruct* pSelUser = m_pUsersList->GetUser(iIndex);

			 if (!pSelUser)
			 {
				 return 0;
			 }

			if ((pGameFrame->m_pGameInfo->dwRoomRule & GRR_NOTCHEAT && pSelUser->GameUserInfo.dwUserID != pGameFrame->GetMeUserInfo()->dwUserID) || pGameFrame->m_pGameInfo->dwRoomRule & GRR_CONTEST)
			{
				bSecret = true;
			}
			bool bShowName = !(pGameFrame->m_pGameInfo->dwRoomRule & GRR_NOTCHEAT) || (pSelUser->GameUserInfo.dwUserID == pGameFrame->GetMeUserInfo()->dwUserID);

			char szBuffer[128] = {0};
			m_pUsersList->OnGetItemText(pSelUser,iIndex,iSubItem,szBuffer,bSecret, bShowName); 
			CString strRet;

			if (IsVipEffectTime(pSelUser->GameUserInfo.iVipTime))
			{
				strRet.Format("<c FF0000>%s</c>",szBuffer);
			}
			else
			{
				strRet.Format("%s",szBuffer);
			}

			return strRet;
		}
	}

	return _T("");
}

//��������
void CRightFrame::ShowWindow(bool bShow/* = true*/, bool bTakeFocus/* = true*/)
{
	if (!bShow) //������ʧ
	{
		::SetTimer(m_hWnd,IDT_TRANSPARENT,40,NULL);
		return;
	}
	else
	{
		::KillTimer(m_hWnd,IDT_TRANSPARENT);
		m_pm.SetTransparent(257);
		m_iTransparent = 255;
	}

	CWindowWnd::ShowWindow(bShow,bTakeFocus);
}

//������߰�ť
void CRightFrame::OnBnProp(BzDui::CButtonUI* pButton)
{
	if (!pButton)
	{
		return;
	}

	CLoveSendClassInExe* pGameFrame = (CLoveSendClassInExe*)m_pRoom;

	if (!pGameFrame)
	{
		return;
	}

	//���鳡(��ѳ�) ����ʹ�õ���
	if (pGameFrame->m_pGameInfo->dwRoomRule & GRR_EXPERCISE_ROOM)
	{
		AFCMessageBox("�Բ���, ���鳡(��ѳ�)����ʹ����Ϸ����", "��ܰ��ʾ");
		return;
	}

	//���ΰټ�����ع���
	TCHAR szKey[10];
	CBcfFile fsr(CBcfFile::GetAppPath() + "..\\SpecialRule.bcf");
	_stprintf(szKey,"%d",pGameFrame->m_pGameInfo->uNameID);

	if (fsr.GetKeyVal (_T("BJLType"),szKey,0) > 0)
	{
		AFCMessageBox("�Բ���,�޷��ڰ�������Ϸʹ�õ���","��ʾ");
		return;
	}

	//�򿪵��߿�
	if (pButton->GetName() == _T("Prop_Btn_0")) //VIP���߰�ť
	{	
		pGameFrame->OnGetBuyItemData(9999);
	}
	else
	{
		_TAG_USERPROP* userProp = (_TAG_USERPROP*)pButton->GetTag();
		if (userProp)
		{
			pGameFrame->OnGetBuyItemData(userProp->nPropID);;
		}
	}

	return;
}

//���ѡ�����
void CRightFrame::OnBnExpression()
{
	CLoveSendClassInExe* pGameFrame = static_cast<CLoveSendClassInExe*>(m_pRoom);
	CGameTalkText* pExpressWnd = &pGameFrame->m_ExpressWnd;

	//�������鴰��
	if (pExpressWnd->GetSafeHwnd() == NULL)
	{
		pExpressWnd->CreateExpressionWnd(pGameFrame);
	}

	//�ƶ�����

	CButtonUI* pFaceBtn = static_cast<CButtonUI*>(m_pm.FindControl("Talk_Btn_Face"));
	RECT rcFaceBtn = pFaceBtn->GetPos();

	RECT rcRightWnd;
	RECT rcGameWnd;
	::GetWindowRect(m_hWnd,&rcRightWnd);
	::GetWindowRect(pGameFrame->m_hWnd,&rcGameWnd);

	::CSize siExpress;
	pExpressWnd->GetFixSize(siExpress);

	pGameFrame->SafeMoveWindow(pExpressWnd,
		(rcRightWnd.left + rcFaceBtn.right) - siExpress.cx + 176,
		rcRightWnd.top + rcFaceBtn.top - siExpress.cy - 3,
		siExpress.cx,siExpress.cy);

	pExpressWnd->ShowWindow(SW_SHOW);
	pExpressWnd->SetFocus();

}

//���û��б�Ĳ˵�
void CRightFrame::OnPopupUserListMenu(POINT pt)
{
	CLoveSendClassInExe* pGameFrame = (CLoveSendClassInExe*)m_pRoom;

	//����������û���Ҽ��˵�
	if (pGameFrame->m_pGameInfo->dwRoomRule & GRR_NOTCHEAT)
	{
		return;
	}

	CListUI* pUserlist = m_pUsersList->m_pListCtrl;

	CControlUI* p = m_pm.FindControl(pt);

	int nFindPos = m_pUsersList->FindItem(p);
	if (nFindPos == -1)
	{
		return;
	}

	UserItemStruct* pSelUser = m_pUsersList->GetUser(nFindPos);
	UserInfoStruct* pMeUser = pGameFrame->GetMeUserInfo();

	int iForbitSay = 8 & Glb().m_CenterServerPara.m_nFunction;

	POINT ptS = pt;
	::ClientToScreen(*this,&pt);	

	CMenuDu* pMenu = new CMenuDu(m_hWnd,UM_MENU_ULIST);
	if (!pMenu)
	{
		return;
	}

	char szTime[32];
	_stprintf(szTime,"%d",CTime::GetCurrentTime());

	if (/*!m_bBJLGame && */pSelUser->GameUserInfo.dwUserID != pMeUser->dwUserID)
	{
		if(pMeUser->iVipTime > ::atoi(szTime)) // ��Ա���д˹���
		{
			pMenu->AddMenuItem(_T("�����뿪"),MMSG_ULIST_KICK);
			pMenu->AddMenuSeparator();
		}
		else  //�������Ƿ������˿������˹���
		{
			if(m_bUseKickUserProp) // ��ʾ���˲˵�
			{
				pMenu->AddMenuItem(_T("�����뿪"),MMSG_ULIST_KICK_PROP);
				pMenu->AddMenuSeparator();
			}
		}
	}

	pMenu->AddMenuItem(_T("��ϸ��Ϣ"),MMSG_ULIST_INFO);
	pMenu->AddMenuItem(_T("�����û���"),MMSG_ULIST_COPYNAME);
	
	if (!iForbitSay && pSelUser->GameUserInfo.dwUserID != pMeUser->dwUserID
			&& !(pGameFrame->m_pGameInfo->dwRoomRule & GRR_AUTO_SIT_DESK))
	{
		pMenu->AddMenuSeparator();
		pMenu->AddMenuItem(_T("��Ϊ����"),MMSG_ULIST_ADDFRIEND);
		pMenu->AddMenuItem(_T("���������"),MMSG_ULIST_ADDBLACKLIST);
	}

	pMenu->SetShowPoint(pt);
	pMenu->Init();
	pMenu->ShowWindow(true);

	return;
}

//�������Ϣ��׼����
void CRightFrame::OnTalkBufferReady(BzDui::CStdString strBuf,int iTextHeight)
{
	if (strBuf.GetLength() == 0)
	{
		return;
	}

	CTextUI* pText = new CTextUI();
	pText->SetFloat(false);
	pText->SetShowHtml(true);
	pText->SetText(strBuf);

	CListContainerElementUI* pCtn = new CListContainerElementUI();
	pCtn->SetFixedHeight(iTextHeight);
	pCtn->Add(pText);

	m_pTalkBoxList->Add(pCtn);

	return;
}

LRESULT CRightFrame::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if( uMsg == WM_KEYDOWN ) 
	{
		if( wParam == VK_RETURN ) 
		{
			return true;
		}
		else if( wParam == VK_ESCAPE ) 
		{
			return true;
		}
	}
	else if (uMsg == WM_KEYDOWN)
	{
		return true;
	}
	return false;
}

void CRightFrame::Init() 
{
	CLoveSendClassInExe* pGameFrame = static_cast<CLoveSendClassInExe*>(m_pRoom);

	CString sPath = CBcfFile::GetAppPath();

	//��ȡ���ڵ����������С���
	CBcfFile fGSkin(sPath + "GameOption.bcf");
	m_uMaxWidth = fGSkin.GetKeyVal(_T("RightFrame"),"max_width",324);
	m_uMinWidth = fGSkin.GetKeyVal(_T("RightFrame"),"min_width",248);
	//�Ƿ������ظô���
	m_bCanHide  = fGSkin.GetKeyVal(_T("RightFrame"),"can_hide",1);

	//�ж��Ƿ�ټ�����Ϸ
	TCHAR szKey[10];
	
	CBcfFile fsr(sPath + "..\\SpecialRule.bcf");
	wsprintf(szKey, "%d", pGameFrame->m_pGameInfo->uNameID);
	m_bBJLGame = (fsr.GetKeyVal(_T("BJLType"),szKey,0) == 1);

	//����Ƿ������˵���
	CBcfFile fOr(sPath + "..\\Order.bcf");
	m_bUseKickUserProp = (fOr.GetKeyVal(_T("ClassInExe"),_T("UseKickUserProp"),0) == 1);

	//����ǰ�����Ϸ����ʾ�����ش��ڰ�ť��Ч
	CButtonUI* pBtnCtrl = static_cast<CButtonUI*>(m_pm.FindControl("Btn_Show_Wnd"));
	if (pBtnCtrl)
	{
		pBtnCtrl->SetEnabled(m_bCanHide && (!m_bBJLGame));
	}

	//�û��б�ָ��
	CListUI* pListCtrl = static_cast<CListUI*>(m_pm.FindControl("Lst_List_Users"));
	pListCtrl->SetTextCallback(this);
	pListCtrl->SetContextMenuUsed(true);

	m_pUsersList = new CGameUserListCtrlDu(pListCtrl);

	//���س��ÿؼ�����ʽ
	LoadNormalCtrlSkin();

	//��ʼ��������
	InitProp();

	CFont f;
	f.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,						   // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline�»��߱�ǣ���Ҫ�»��߰��������ó�TRUE
		0,                         // cStrikeOut
		ANSI_CHARSET,			   // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("����"));               // lpszFacename

	//�����
	m_pTalkBoxList = static_cast<CListUI*>(m_pm.FindControl("Talk_Lst_Main"));

	//������򻺳��ı�
	m_pTalkTempText = static_cast<CTextUI*>(m_pm.FindControl("Talk_Text_Temp"));

	OnReLoadTalkINI();

}

//����Combol���б���
void CRightFrame::LoadComboItems(CComboUI* pCtrl,LPCTSTR items[],int nums,int nSelect/* = 0*/)
{
	if (!pCtrl || nums <= 0)
	{
		return;
	}

	for (int i = 0; i < nums; ++i)
	{
		CListLabelElementUI* p = new CListLabelElementUI();
		p->SetText(items[i]);
		pCtrl->Add(p);
	}

	return;
}

//�����ı�����ѡ���б���
bool CRightFrame::SelectComboItem(BzDui::CComboUI* pCtrl,LPCTSTR szText)
{
	if (!pCtrl)
	{
		return false;
	}

	int itNums = pCtrl->GetCount();

	if (itNums <= 0)
	{
		return false;
	}

	for (int i = 0; i < itNums; ++i)
	{
		if (strcmp(szText,pCtrl->GetItemAt(i)->GetText().GetData()) == 0)
		{
			pCtrl->SelectItem(i);
			return true;
		}
	}

	return false;
}

//��ʼ��������
void CRightFrame::InitProp()
{
	CContainerUI* pPropMainCtn = static_cast<CContainerUI*>(m_pm.FindControl("Prop_Ctn_Main"));

	if (!pPropMainCtn)
	{
		return;
	}
	RECT rc = {0,20,1,0};
	char szBuf[32] = {0};

	for (int i = 0; i < MAX_PROP_NUMS; ++i)
	{
		m_pPropBtn[i] = new CButtonUI();

		_stprintf(szBuf,"Prop_Btn_%d",i);
		m_pPropBtn[i]->SetName(szBuf);

		m_pPropBtn[i]->SetText("");
		m_pPropBtn[i]->SetTextColor(0xFFFFFFFF);
		m_pPropBtn[i]->SetAttribute("align","right");
		m_pPropBtn[i]->SetTextPadding(rc);

		m_pPropBtn[i]->SetFixedHeight(34);
		m_pPropBtn[i]->SetFixedWidth(34);
		m_pPropBtn[i]->SetFloat(true);
		m_pPropBtn[i]->SetTag(0);

		pPropMainCtn->Add(m_pPropBtn[i]);
	}

	return;
}

//������UIλ��ƥ��
void CRightFrame::FixPropUI(int cx,int cy)
{
	if (cy <= 0)
	{
		return;
	}

	//ƥ���û���Ϣ��λ��////////////////////////////////////////////////////////////////////////////////
	CContainerUI* pInfoLeft = static_cast<CContainerUI*>(m_pm.FindControl("Info_Ctn_Left"));
	if (pInfoLeft)
	{
		pInfoLeft->SetFixedWidth(cx <= 300 ? 5 : 100);
	}

	//ƥ���������λ��///////////////////////////////////////////////////////////////////////////////////

	CContainerUI* pPropMainCtn = static_cast<CContainerUI*>(m_pm.FindControl("Prop_Ctn_Main"));
	if (pPropMainCtn)
	{
		RECT rcWnd;
		::GetWindowRect(m_hWnd,&rcWnd);

		int iWidth = rcWnd.right - rcWnd.left - 50 - 34;

		const int iSpace = 4;
		const int iHorMums = iWidth / (34 + iSpace) + 1;
		m_uPropPageSize = iHorMums * 2;
		int iCount = 0;

		//����ҳ��
		m_uPropPageNums = (MAX_PROP_NUMS % m_uPropPageSize) == 0 ? 
			MAX_PROP_NUMS / m_uPropPageSize : MAX_PROP_NUMS / m_uPropPageSize + 1;

		for (int iPage = 0; iPage < m_uPropPageNums; ++iPage)
		{
			for (int iCol = 0; iCol < 2; ++iCol)
			{
				for (int iRow = 0; iRow < iHorMums; ++iRow)
				{
					if (iCount < MAX_PROP_NUMS)
					{
						SIZE sz = {(34 + 4) * iRow,15 + (34 + 4) * iCol};	
						m_pPropBtn[iCount++]->SetFixedXY(sz);
					}
				}
			}
		}

		//ÿ���ض�λ�󣬶���ʾ��һҳ
		ShowPropPage(0);

	}

	//ƥ��������λ��///////////////////////////////////////////////////////////////////////////////////

	int x = 0,y = 0,width = 0,height = 0;

	CContainerUI* pCtrl = NULL;
	
	pCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Hor_Talk"));
	x += pCtrl ? pCtrl->GetInset().left : 0;

	pCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Ctn_Talk_Main"));
	x += pCtrl ? pCtrl->GetInset().left : 0;


	pCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Hor_Info"));
	y += pCtrl ? pCtrl->GetFixedHeight() : 0;

	pCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Hor_UserList"));
	y += pCtrl ? pCtrl->GetFixedHeight() : 0;

	pCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Hor_Prop"));
	y += pCtrl ? pCtrl->GetFixedHeight() : 0;

	pCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Hor_Talk"));
	y += pCtrl ? pCtrl->GetInset().top : 0;

	pCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Ctn_Talk_Main"));
	y += pCtrl ? pCtrl->GetInset().top : 0;


	width = cx - x;

	pCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Hor_Talk"));
	width -= pCtrl ? pCtrl->GetInset().right : 0;

	pCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Ctn_Talk_Main"));
	width -= pCtrl ? pCtrl->GetInset().right : 0;


	height = cy - y;

	pCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Hor_Talk"));
	height -= pCtrl ? pCtrl->GetInset().bottom : 0;

	pCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Hor_Talk_Control"));
	height -= pCtrl ? pCtrl->GetFixedHeight() : 0;

	pCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Ctn_Talk_Main"));
	height -= pCtrl ? pCtrl->GetInset().bottom : 0; 

	//�������������λ��//////////////////////////////////////////////////////////////////////////////////////////

	width = cx;

	pCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Hor_Talk"));
	width -= pCtrl ? pCtrl->GetInset().left : 0;
	width -= pCtrl ? pCtrl->GetInset().right : 0;

	pCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Hor_Talk_Control"));
	width -= pCtrl ? pCtrl->GetInset().left : 0;
	width -= pCtrl ? pCtrl->GetInset().right : 0;

	pCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Talk_Ctn_01"));
	width -= pCtrl ? pCtrl->GetFixedWidth() : 0;

	pCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Talk_Ctn_02"));
	width -= pCtrl ? pCtrl->GetFixedWidth() : 0;

	pCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Talk_Ctn_04"));
	width -= pCtrl ? pCtrl->GetFixedWidth() : 0;

	pCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Talk_Ctn_05"));
	width -= pCtrl ? pCtrl->GetFixedWidth() : 0;

	pCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Talk_Ctn_03"));
	width -= pCtrl ? pCtrl->GetInset().left : 0;
	width -= pCtrl ? pCtrl->GetInset().right : 0;


	CEditUI* pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Talk_Edt_Inp"));
	if (pEdtCtrl)
	{
		pEdtCtrl->SetFixedWidth(width - 30);
	}


	return;
}

//��ʾ����ҳ
void CRightFrame::ShowPropPage(UINT uPage)
{
	if (uPage < 0)
	{
		return;
	}

	if (uPage >= m_uPropPageNums)
	{
		return;
	}

	//��ǰ��ʾ���ߵ�ҳ
	m_uCurrPropPage = uPage;

	int iMinPos = uPage * m_uPropPageSize;
	int iMaxPos = iMinPos + m_uPropPageSize - 1;

	for (int i = 0; i < MAX_PROP_NUMS; ++i)
	{
		if (m_pPropBtn[i])
		{
			m_pPropBtn[i]->SetVisible(i >= iMinPos && i <= iMaxPos);
		}
	}

	CButtonUI* pBtnPrev = static_cast<CButtonUI*>(m_pm.FindControl("Prop_Btn_Prev"));
	CButtonUI* pBtnNext = static_cast<CButtonUI*>(m_pm.FindControl("Prop_Btn_Next"));

	pBtnPrev->SetEnabled(uPage > 0);
	pBtnNext->SetEnabled(uPage < m_uPropPageNums - 1);

	return;
}

void CRightFrame::Notify(TNotifyUI& msg)
{
	if(msg.sType == _T("click")) 
	{
		if (msg.pSender->GetName() == _T("Btn_Show_Wnd"))
		{
			CLoveSendClassInExe* pGameFrame = static_cast<CLoveSendClassInExe*>(m_pRoom);
			pGameFrame->OnBnClickedHide();
			return;
		}

		if (msg.pSender->GetName() == _T("Prop_Btn_Prev"))
		{
			ShowPropPage(--m_uCurrPropPage);
			return;
		}

		if (msg.pSender->GetName() == _T("Prop_Btn_Next"))
		{
			ShowPropPage(++m_uCurrPropPage);
			return;
		}

		if (msg.pSender->GetName().Left(8).Compare("Prop_Btn") == 0) //���߿��ڰ�ť��Ϣ��Ӧ
		{
			OnBnProp(static_cast<CButtonUI*>(msg.pSender));
			return;
		}

		if (msg.pSender->GetName() == "Talk_Btn_Face")
		{
			OnBnExpression();
			return;
		}

		if (msg.pSender->GetName() == "Chat_Btn_Send")
		{

			CLoveSendClassInExe* pGameFrame = static_cast<CLoveSendClassInExe*>(m_pRoom);
			pGameFrame->SendTalkMessage();
			return;
		}
		if (msg.pSender->GetName() == "Talk_Btn_BoardCast")
		{
			if (!m_pBoardCastWnd)
			{
				m_pBoardCastWnd = new CUseBoardCastWnd();
				m_pBoardCastWnd->Create(m_hWnd,NULL,UI_WNDSTYLE_DIALOG,0,0,0,0,0,NULL);
				m_pBoardCastWnd->SetParentWnd(m_pRoom);
				m_pBoardCastWnd->CenterWindow();
				m_pBoardCastWnd->ShowModal();
				if (m_pBoardCastWnd != NULL)
				{
					delete m_pBoardCastWnd;
					m_pBoardCastWnd = NULL;
				}
			}
			
		}
	}

	else if(msg.sType == _T("return"))
	{
		if (msg.pSender->GetName() == "Talk_Edt_Inp")
		{
			CLoveSendClassInExe* pGameFrame = static_cast<CLoveSendClassInExe*>(m_pRoom);
			pGameFrame->SendTalkMessage();
			return;
		}
	}

	else if (msg.sType == _T("itemselect"))
	{
		if (msg.pSender == m_pUsersList->m_pListCtrl)
		{
			ShowUserInfo(m_pUsersList->GetUser(msg.wParam));
			return;
		}

		if( msg.pSender->GetName() == _T("Talk_Cob_Inp")) 
		{
			CEditUI* pEditCtrl = static_cast<CEditUI*>(m_pm.FindControl("Talk_Edt_Inp"));
			pEditCtrl->SetText(msg.pSender->GetText());

			CLoveSendClassInExe* pGameFrame = static_cast<CLoveSendClassInExe*>(m_pRoom);
			pGameFrame->SendTalkMessage();

			return;
		}
	}

	else if (msg.sType == _T("menu"))
	{
		if (msg.pSender == m_pUsersList->m_pListCtrl)
		{
			OnPopupUserListMenu(msg.ptMouse);
			return;
		}
	}

	else if( msg.sType == _T("settext") ) 
	{
		if (msg.pSender == m_pTalkTempText)
		{
			m_pTalkTempText->SetVisible(false);
			OnTalkBufferReady(m_pTalkTempText->GetText(),msg.wParam);
			return;
		}

	}
	else if (msg.sType == _T("ScrollSetRange"))
	{
		if (m_pTalkBoxList)
		{
			if (msg.pSender == m_pTalkBoxList->GetVerticalScrollBar())
			{
				m_pTalkBoxList->Scroll(0,msg.wParam);
			}
		}

	}

// 	CString str;
// 	str.Format("DClient: Notify msg = %s",msg.sType);
// 	OutputDebugString(str);
	
}

void CRightFrame::CenterWindow()
{
	RECT rc;
	::GetWindowRect(m_hWnd,&rc);
	int iSrcW = GetSystemMetrics(SM_CXSCREEN);
	int iSrcH = GetSystemMetrics(SM_CYSCREEN);
	int iWndW = rc.right - rc.left;
	int iWndH = rc.bottom - rc.top;
	::MoveWindow(m_hWnd,(iSrcW - iWndW)/2,(iSrcH - iWndH)/2,iWndW,iWndH,true);
}


//���س���ؼ�Ƥ��
void CRightFrame::LoadNormalCtrlSkin()
{
	int idx = 0;
	CStdPtrArray* pCtrls = NULL;

	pCtrls = m_pm.FindSubControlsByClass(m_pm.GetRoot(),_TEXT("EditUI"));
	CEditUI* pEditUI = NULL;
	RECT rcText = {4,1,4,0};
	do 
	{
		pEditUI = static_cast<CEditUI*>(pCtrls->GetAt(idx++));

		if (pEditUI)
		{
			if (pEditUI->GetName() == _T("Talk_Edt_Inp"))
			{
				continue;
			}

			if (!pEditUI->IsReadOnly())
			{
				pEditUI->SetNormalImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,0,500,22' corner='3,4,2,2'");
				pEditUI->SetHotImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,22,500,44' corner='3,4,2,2'");
				pEditUI->SetFocusedImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,44,500,66' corner='3,4,2,2'");
				pEditUI->SetDisabledImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,66,500,88' corner='3,4,2,2'");
			}
			else
			{
				pEditUI->SetNormalImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,66,500,88' corner='3,4,2,2'");
				pEditUI->SetHotImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,66,500,88' corner='3,4,2,2'");
				pEditUI->SetFocusedImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,66,500,88' corner='3,4,2,2'");
				pEditUI->SetDisabledImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,66,500,88' corner='3,4,2,2'");
			}

			pEditUI->SetTextPadding(rcText);
		}
	} while (pEditUI);

	idx = 0;
	pCtrls = m_pm.FindSubControlsByClass(m_pm.GetRoot(),_TEXT("ComboUI"));
	CComboUI* pComboUI = NULL;
	RECT rc = {6,0,30,0};
	do 
	{
		pComboUI = static_cast<CComboUI*>(pCtrls->GetAt(idx++));
		if (pComboUI)
		{
			pComboUI->SetNormalImage("file='dialog\\combo.png' dest='0,0,500,22' source='0,0,500,22' corner='5,0,22,0'");
			pComboUI->SetHotImage("file='dialog\\combo.png' dest='0,0,500,22' source='0,22,500,44' corner='5,0,22,0'");
			pComboUI->SetPushedImage("file='dialog\\combo.png' dest='0,0,500,22' source='0,44,500,66' corner='5,0,22,0'");
			pComboUI->SetDisabledImage("file='dialog\\combo.png' dest='0,0,500,22' source='0,66,500,88' corner='5,0,22,0'");
			pComboUI->EnableScrollBar(true,false);
			pComboUI->SetItemTextPadding(rc);
		}
	} while (pComboUI);

	//�������ж�ѡ��/��ѡ���Ƥ��
	idx = 0;
	pCtrls = m_pm.FindSubControlsByClass(m_pm.GetRoot(),_TEXT("OptionUI"));
	COptionUI* pOptionUI = NULL;
	do 
	{
		pOptionUI = static_cast<COptionUI*>(pCtrls->GetAt(idx++));
		if (pOptionUI)
		{
			if (lstrlen(pOptionUI->GetGroup()) == 0) //��ѡ��
			{
				pOptionUI->SetNormalImage("file='dialog\\check.png' dest='0,0,15,15' source='0,0,15,15' corner='0,0,0,0'");
				pOptionUI->SetHotImage("file='dialog\\check.png' dest='0,0,15,15' source='15,0,30,15' corner='0,0,0,0'");
				pOptionUI->SetSelectedImage("file='dialog\\check.png' dest='0,0,15,15' source='30,0,45,15' corner='0,0,0,0'");
				RECT rc = {9,0,0,0};
				pOptionUI->SetTextPadding(rc);
			}
			else  //��ѡ��
			{
				pOptionUI->SetNormalImage("file='dialog\\radio.png' dest='0,0,15,15' source='0,0,15,15' corner='0,0,0,0'");
				pOptionUI->SetHotImage("file='dialog\\radio.png' dest='0,0,15,15' source='15,0,30,15' corner='0,0,0,0'");
				pOptionUI->SetSelectedImage("file='dialog\\radio.png' dest='0,0,15,15' source='30,0,45,15' corner='0,0,0,0'");
				RECT rc = {18,0,0,0};
				pOptionUI->SetTextPadding(rc);
			}

		}
	} while (pOptionUI);
}

//�б�ѡ�е��û���Ϣ����ʾ���Ϸ����
void CRightFrame::ShowUserInfo(UserItemStruct* pUser)
{
	if (!pUser)
	{
		return;
	}

	m_pShowUser = pUser;

	CLoveSendClassInExe* pGameFrame = (CLoveSendClassInExe*)m_pRoom;
	pGameFrame->m_dwCurrentUserID = pUser->GameUserInfo.dwUserID;

	bool bSecret = false;
	if ((pGameFrame->m_pGameInfo->dwRoomRule & GRR_NOTCHEAT && pUser->GameUserInfo.dwUserID != pGameFrame->GetMeUserInfo()->dwUserID) || pGameFrame->m_pGameInfo->dwRoomRule & GRR_CONTEST)
	{
		bSecret = true;
	}

	CLabelUI* pLabCtrl = NULL;
	CControlUI* pCtnCtrl = NULL;
	char szBuffer[128];



	//�û�ID
	if ((pGameFrame->m_pGameInfo->dwRoomRule & GRR_NOTCHEAT && pUser->GameUserInfo.dwUserID != pGameFrame->GetMeUserInfo()->dwUserID))
	{
		_stprintf(szBuffer,"���");
	}
	else
	{
		//�û���
		_stprintf(szBuffer,"%s", pUser->GameUserInfo.nickName);
	}

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Info_Name"));
	pLabCtrl->SetText(szBuffer);

	//�û�ID
	if ((pGameFrame->m_pGameInfo->dwRoomRule & GRR_NOTCHEAT && pUser->GameUserInfo.dwUserID != pGameFrame->GetMeUserInfo()->dwUserID))
	{
		_stprintf(szBuffer,"???");
	}
	else
	{
		_stprintf(szBuffer,"%d",pUser->GameUserInfo.dwUserID);
	}
	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Info_ID"));
	pLabCtrl->SetText(szBuffer);

	//����
	if (bSecret)
	{
		_stprintf(szBuffer,"???");
	}
	else
	{
		int dwPoint = pUser->GameUserInfo.dwPoint;
		_stprintf(szBuffer,"%d",dwPoint - GetNormalOrderBasePoint(dwPoint));
	}
	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Info_Points"));
	pLabCtrl->SetText(szBuffer);

	//���
	if (bSecret)
	{
		_stprintf(szBuffer,"???");
	}
	else
	{
		::GlbGetNumString(szBuffer,pUser->GameUserInfo.i64Money,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);
	}
	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Info_Coints"));
	pLabCtrl->SetText(szBuffer);

	//����
	if (bSecret)
	{
		_stprintf(szBuffer,"???");
	}
	else
	{
		_stprintf(szBuffer,"%d",pUser->GameUserInfo.dwFascination);
	}
	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Info_Glamour"));
	pLabCtrl->SetText(szBuffer);

	//�����ȼ�
	_stprintf(szBuffer,"%s",bSecret?"???":GetCharmGrade(pUser->GameUserInfo.dwFascination));
	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Info_Level"));
	pLabCtrl->SetText(szBuffer);

	//logo
	pCtnCtrl = m_pm.FindControl("Img_Info_logo");
	GetUserLogoFile(pUser,szBuffer);
	pCtnCtrl->SetBkImage(szBuffer);

	return;
}

//��ȡ���ͷ���logo�ļ�·��
void CRightFrame::ShowUserInfo()
{
	ShowUserInfo(m_pShowUser);

	return;
}

//����б�ѡ�е��û�
UserItemStruct* CRightFrame::GetShowUser()
{
	return m_pShowUser;
}

//��ȡ���ͷ���logo�ļ�·��
void CRightFrame::GetUserLogoFile(UserItemStruct* pUser,char* sz_out_file)
{
	if(!pUser)
	{
		return;
	}

	// �滭����ͷ��
	_stprintf(sz_out_file,TEXT("..\\..\\..\\image\\log\\%s\\gamelog%ld.png"),pUser->GameUserInfo.bBoy?"Boy":"Girl",BzGetAvatarIndex(pUser));

	return;
}

//���õ��߰�ťͼƬ
void CRightFrame::OnSetPropImages()
{
	_TAG_USERPROP *userProp = NULL;
	int propCout = (int)Glb().userPropLibrary.GetCount();

	char szPropImage[MAX_PATH];
	char szBuffer[256];

	//��һ����̶�ΪVIP����
	if (m_pPropBtn[0])
	{
		m_pPropBtn[0]->SetEnabled(true);
		m_pPropBtn[0]->SetNormalImage("file='game\\Prop\\vip01.png' dest='0,0,34,34' source='0,0,34,34'");
		m_pPropBtn[0]->SetHotImage("file='game\\Prop\\vip01.png' dest='0,0,34,34' source='34,0,68,34'");
		m_pPropBtn[0]->SetDisabledImage("file='game\\Prop\\vip01.png' dest='0,0,34,34' source='102,0,136,34'");
		m_pPropBtn[0]->SetText(_T(""));
	}

	CArray<_TAG_USERPROP*,_TAG_USERPROP*> userPropLibraryTmp;
	for(int i=0; i<Glb().userPropLibrary.GetCount(); ++i)
	{			
		userProp = Glb().userPropLibrary.GetAt(i);

		//���ڴ�С���������������ʵ�֣��������е����ȵ���û����ʾ
		if (::strcmp(userProp->szPropName,"������") == 0 
			|| ::strcmp(userProp->szPropName,"С����") == 0)
			continue;

		userPropLibraryTmp.Add(userProp);
	}

	for(int i=1; i<MAX_PROP_NUMS; ++i)
	{
		if (NULL == m_pPropBtn[i])
		{
			continue;
		}

		if (i < propCout+1 && NULL != userPropLibraryTmp.GetAt(i-1)) //�е��ߵĸ���
		{
			m_pPropBtn[i]->SetEnabled(true);
			userProp = userPropLibraryTmp.GetAt(i - 1);

			wsprintf(szPropImage,TEXT("game\\Prop\\Prop%d.png"),i);

			_stprintf(szBuffer,"file='%s' dest='0,0,34,34' source='0,0,34,34'",szPropImage);
			m_pPropBtn[i]->SetNormalImage(szBuffer);

			_stprintf(szBuffer,"file='%s' dest='0,0,34,34' source='34,0,68,34'",szPropImage);
			m_pPropBtn[i]->SetHotImage(szBuffer);

			_stprintf(szBuffer,"file='%s' dest='0,0,34,34' source='102,0,136,34'",szPropImage);
			m_pPropBtn[i]->SetDisabledImage(szBuffer);

			m_pPropBtn[i]->SetToolTip(userProp->szPropName);

			_stprintf(szBuffer,"%d",userProp->nHoldCount);
			m_pPropBtn[i]->SetText(szBuffer);

			//�󶨵���ID�ڰ�ť
			m_pPropBtn[i]->SetTag((UINT_PTR)userProp);
		}
		else //û�е��ߵĿո���
		{
			m_pPropBtn[i]->SetEnabled(false);
			m_pPropBtn[i]->SetNormalImage("file='game\\Prop\\PropEmpty.png' dest='0,0,34,34' source='0,0,34,34'");
			m_pPropBtn[i]->SetHotImage("file='game\\Prop\\PropEmpty.png' dest='0,0,34,34' source='34,0,68,34'");
			m_pPropBtn[i]->SetDisabledImage("file='game\\Prop\\PropEmpty.png' dest='0,0,34,34' source='102,0,136,34'");
			m_pPropBtn[i]->SetToolTip(_T("����������"));
			m_pPropBtn[i]->SetText(_T(""));
		}
	}

	return;
}

//�ض�talkIngame.ini
void CRightFrame::OnReLoadTalkINI()
{
	CComboUI* pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl("Talk_Cob_Inp"));
	CEditUI* pEditCtrl = static_cast<CEditUI*>(m_pm.FindControl("Talk_Edt_Inp"));
	if (!pCobCtrl || !pEditCtrl)
	{
		return;
	}

	//���ԭ������
	pCobCtrl->RemoveAll();

	CBcfFile f("talkInGame.bcf");
	CString key="BZW_TALK";
	CString szTalk;
	CString szNumber;
	int nIdx = 0;
	do
	{
		szNumber.Format("%d",++nIdx);
		szTalk = f.GetKeyVal(key,"T"+szNumber,"");
		if (!szTalk.IsEmpty())
		{
			CListLabelElementUI* p = new CListLabelElementUI();
			p->SetText(szTalk.GetBuffer());
			pCobCtrl->Add(p);
		}
	}
	while(!szTalk.IsEmpty());

	SIZE si = {-1,600};
	pCobCtrl->SetDropBoxSize(si);

	pEditCtrl->SetText(_T(""));

	return;
}

//�����������������
int CRightFrame::GetTalkInputMessage(char* szMsg)
{
	CEditUI* pEditCtrl = static_cast<CEditUI*>(m_pm.FindControl("Talk_Edt_Inp"));
	if (!pEditCtrl)
	{
		return 0;
	}

	if (szMsg)
	{
		strcpy(szMsg,pEditCtrl->GetText().GetData());
	}

	return pEditCtrl->GetText().GetLength();

}

//���ý��������������
void CRightFrame::SetFocusToTalkInput()
{
	CEditUI* pEditCtrl = static_cast<CEditUI*>(m_pm.FindControl("Talk_Edt_Inp"));
	if (pEditCtrl)
	{
		pEditCtrl->SetFocus();
	}
}

//������������
void CRightFrame::ClearTalkInput()
{
	CEditUI* pEditCtrl = static_cast<CEditUI*>(m_pm.FindControl("Talk_Edt_Inp"));
	if (pEditCtrl)
	{
		pEditCtrl->SetText(_T(""));
	}
}

//��ӱ��鵽���������
void CRightFrame::AddExpressionToTalkInput(char* szExpression)
{
	CEditUI* pEditCtrl = static_cast<CEditUI*>(m_pm.FindControl("Talk_Edt_Inp"));

	CStdString str = pEditCtrl->GetText();
	str += szExpression;

	pEditCtrl->SetText(str);

	return;
}


bool CRightFrame::OnNetMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
	return true;
}

//������Box������Ϣ����
void CRightFrame::InsertTalkMessage(char* szMsgBuf)
{
	char* p = szMsgBuf;
	std::string strRet = "";
	while(*p != 0)
	{
		if (*p == '/' 
			&& (*(p + 1) != 0 && *(p + 1) == ':')
			&& (*(p + 2) != 0 && ::isdigit(*(p + 2)))
			&& (*(p + 3) != 0 && ::isdigit(*(p + 3)))
			)
		{
			char szBuf[64] = {0};
			_stprintf(szBuf,"<I dialog\\Image_biaoqing.png 10 7 %c %c>",*(p + 3),*(p + 2));
			strRet += szBuf;

			p += 4;
			continue;
		}
		else
		{
			strRet += *p;
		}
		p++;
	}

	CString str;
	str.Format("DGameL:%s",strRet.c_str());
	OutputDebugString(str);

	m_dqTalkBuff.push_back(strRet.c_str());

	return;
}
#include "StdAfx.h"
#include "PBankWndEx.h"
#include "GameListCtrl.h"
#include "MainRoomEx.h"
#include "Bank4ZMessage.h"

using namespace BzDui;

#define SORT_NUMS				3
#define RECORD_PAGE_SIZE		8	//ת��־ÿҳ�ļ�¼����
#define IDT_TIMEOUT_TO_LOGOUT	1

#define NICK_ID_USE_TO_TRANSFER	11 //��������ת��
#define NICK_ID_USE_TO_DEPOSIT	12 //���ڵ㿨��ֵ
#define NICK_ID_BYID			0  //ͨ���û�ID��ѯ�û��ǳ�
#define NICK_ID_BYNICKNAME		1  //ͨ������ǳƲ�ѯ�û�ID


/*const int WM_BANK_CLOSE= WM_USER + 524;*/

// const int DIGITSIZE = 10;	//�������Ĵ�д
// const int UNITSIZE	= 19;	//������λ��
static char* szDigit[DIGITSIZE] = {"��", "Ҽ", "��", "��", "��", "��", "½", "��", "��", "��"}; //���Ĵ�д����
static char* szUnit[UNITSIZE]   = {"", "ʰ", "��", "Ǫ", "��", "ʰ", "��", "Ǫ", "��", "ʰ","��","Ǫ","��", "ʰ","��","Ǫ","��","ʰ","��"};	  //���Ĵ�д��λ

//��ǰʵ�����ھ��
HWND CPBankWndEx::m_hInstanceWnd = NULL;

CPBankWndEx::CPBankWndEx(CWnd* pParent,bool bInMainRoom /*= false*/)
{

	m_pRoom = pParent;

	//��ǰѡ��Ĺ���
	m_pSelFunNode = NULL;

	//������Ĵ��
	m_i64BankMoney = 0;

	if (bInMainRoom)
	{
		//�û�ID
		m_dwOpUserID = ((CMainRoomEx*)pParent)->m_PlaceUserInfo.dwUserID;
		m_emParWndType = IsMainRoom;
		
	}
	else
	{
		//�û�ID
		m_dwOpUserID = ((CGameRoomEx*)pParent)->GetMeUserInfo()->dwUserID;
		m_emParWndType = ISGameRoom;
	}

	//ת�˳ɹ�
	m_bTransferSuccess = true; 

	//��¼��ؼ�ָ��
	m_pRecordList = NULL;

	//�浥��¼��ؼ�ָ��
	m_pDepList = NULL;

	m_mapRecord.clear();

	//ת����־��¼����ʾλ��
	m_iRecDispPos = 0;

	//�����浥�ɹ�
	m_bDepositSuccess = true; 

	//�浥��¼��
	m_mapDeposits.clear();

	//��ʱ��
	m_dwTickCount = ::GetTickCount();

	//�Ƿ�ʱ���
	m_bCheckFrequence = false;
	m_iMinutes = 0;
}

CPBankWndEx::~CPBankWndEx(void)
{
	m_mapRecord.clear();

	//�浥��¼��
	m_mapDeposits.clear();
}

LPCTSTR CPBankWndEx::GetWindowClassName() const 
{ 
	return _T("UIFrame");
};

UINT CPBankWndEx::GetClassStyle() const 
{ 
	return UI_CLASSSTYLE_DIALOG; 
};

void CPBankWndEx::OnFinalMessage(HWND hWnd) 
{ 
	m_pm.RemovePreMessageFilter(this);
};

LRESULT CPBankWndEx::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);

	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("BankWnd2\\BankWndEx.xml"), (UINT)0, NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	::SetWindowPos(m_hWnd,CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);

	Init();

	CPBankWndEx::m_hInstanceWnd = m_hWnd;

	return 0;
}

LRESULT CPBankWndEx::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CPBankWndEx::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CPBankWndEx::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CPBankWndEx::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CPBankWndEx::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CPBankWndEx::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CPBankWndEx::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = false;

	if (wParam == IDT_TIMEOUT_TO_LOGOUT)
	{
		if (m_bCheckFrequence)
		{
			int iMinutesPass = (::GetTickCount() - m_dwTickCount)/60000;

			if (iMinutesPass >= m_iMinutes)
			{
				::KillTimer(m_hWnd,IDT_TIMEOUT_TO_LOGOUT);
				Close();
				m_pRoom->PostMessage(WM_BANK_CLOSE,1,0);
				return 0;
			}			
		}
	}

	return 0;
}

LRESULT CPBankWndEx::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

	case WM_TIMER:
		{
			lRes = OnTimer(uMsg, wParam, lParam, bHandled);
		}
		break;

	case WM_SIZE:          
		{
			lRes = OnSize(uMsg, wParam, lParam, bHandled);
		}
		break;

	default:
		bHandled = FALSE;
	}

	if(bHandled)
	{
		return lRes;
	}

	if(m_pm.MessageHandler(uMsg,wParam,lParam,lRes))
	{
		return lRes;
	}

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CPBankWndEx::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
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
	return false;
}

LPCTSTR CPBankWndEx::GetItemText(CControlUI* pControl, int iIndex, int iSubItem)
{
	CString strBuffer;

	if (pControl->GetParent()->GetParent() == m_pRecordList) //ת�˼�¼��
	{
		std::map<int,MSG_GR_R_TransferRecord_t>::iterator it = m_mapRecord.find(m_iRecDispPos + iIndex);

		if (it == m_mapRecord.end())
		{
			return _T("");
		}

		MSG_GR_R_TransferRecord_t* pTRec = &it->second;

		if (pTRec == NULL)
		{
			return _T("");
		}

		if (pTRec->dwUserID == 0)
		{
			return _T("");
		}

		if (iSubItem == 0) //����ʱ��
		{
			strBuffer.Format(_T("%02d/%02d %02d:%02d"),
				pTRec->oleDateTime.GetMonth(),
				pTRec->oleDateTime.GetDay(),
				pTRec->oleDateTime.GetHour(),
				pTRec->oleDateTime.GetMinute());
			return strBuffer;
		}
		else if (iSubItem == 1)
		{
			strBuffer.Format(_T(" %d"),pTRec->dwUserID);
			return strBuffer;
		}
		else if (iSubItem == 2)
		{
			strBuffer.Format(_T(" %d"),pTRec->destUserID);
			return strBuffer;
		}
		else if (iSubItem == 3)
		{
			TCHAR szBuffer[32];
			GetNumString(szBuffer,pTRec->i64Money,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);
			strBuffer.Format(_T(" %s"),szBuffer);
			return strBuffer;
		}
	}

	return _T("");
}

void CPBankWndEx::Init() 
{
	//��ʼ����ϷǮ������
	InitGameWalletTable();

	TCHAR szCtrlName[32];

	//�ύ��ť
	m_pSubmitBnt = static_cast<CButtonUI*>(m_pm.FindControl("Btn_OK"));

	for (int i = 1; i <= SORT_NUMS; ++i)
	{
		//�õ����Ͱ�ťָ��
		::sprintf_s(szCtrlName,"Btn_Type_%d",i);
		m_pSortBnt[i - 1] = static_cast<CButtonUI*>(m_pm.FindControl(szCtrlName));

		//�õ��б������ָ��
		::sprintf_s(szCtrlName,"Ctn_Type%d",i);
		m_pFunsCtn[i - 1] = static_cast<CContainerUI*>(m_pm.FindControl(szCtrlName));
		m_pFunsCtn[i - 1]->SetVisible(false);

		//�õ������б�ť��ָ��,��Թ����������UI����ָ�룬ͨ��TFunsNode��
		int idx = 0;
		CStdPtrArray* pCtrls = m_pm.FindSubControlsByClass(m_pFunsCtn[i - 1],_TEXT("ButtonUI"));
		CButtonUI* pBntUI = NULL;
		CControlUI* pImgCtrl = NULL;

		UINT uYPos = 2;

		do 
		{
			pBntUI = static_cast<CButtonUI*>(pCtrls->GetAt(idx++));

			if (pBntUI)
			{
				TFunsNode tagFunsNode;
				tagFunsNode.pFunsBnt = pBntUI;
				::sprintf_s(szCtrlName,"Ctn_Type%d_Ctn%d",i,idx);
				tagFunsNode.pFunsCtn = static_cast<CContainerUI*>(m_pm.FindControl(szCtrlName));
				m_lstFuns[i - 1].push_back(tagFunsNode);
				
				SIZE sz = {pBntUI->GetFixedXY().cx,uYPos};
				pBntUI->SetFixedXY(sz);

				::sprintf_s(szCtrlName,"Img_Type%d_Title%d",i,idx);
				pImgCtrl = m_pm.FindControl(szCtrlName);
				sz.cx = pImgCtrl->GetFixedXY().cx;
				sz.cy += 3;
				pImgCtrl->SetFixedXY(sz);

				if(pBntUI->GetName() == _T("Btn_Type1_Title3") || pBntUI->GetName() == _T("Btn_Type1_Title4"))
				{
					if(CGamePlaceApp::GetModuleStatus("Bank","TransferMoney",1) == 0)
					{
						pBntUI->SetVisible(false);
						pImgCtrl->SetVisible(false);
						continue;
					}
				}

				uYPos += pBntUI->GetFixedHeight();
			}

		} while (pBntUI);
	}

	//��¼��ؼ�ָ��
	m_pRecordList = static_cast<CListUI*>(m_pm.FindControl("Lst_S1_Ctn4_Record"));
	m_pRecordList->SetTextCallback(this);

	//�浥��¼��ؼ�ָ��
	m_pDepList = static_cast<CListUI*>(m_pm.FindControl("Lst_S1_Ctn6_Record"));
	m_pDepList->SetTextCallback(this);

	CControlUI* pCtrl = NULL;

	int xPos = m_pSortBnt[0]->GetFixedXY().cx;
	int yPos = m_pSortBnt[0]->GetFixedXY().cy;
	int iW = m_pSortBnt[0]->GetFixedWidth();
	int iH = m_pSortBnt[0]->GetFixedHeight();

	pCtrl = m_pm.FindControl("VerticalLayoutUI1");

	xPos += pCtrl->GetX();
	yPos += pCtrl->GetY();

	for (int i = 0; i < SORT_NUMS; ++i)
	{
		RECT rc = {xPos,yPos,xPos + iW,yPos + iH};
		m_pSortBnt[i]->SetPos(rc);
		yPos += iH + 3;

		RECT rcLstCtn = {xPos - 1,yPos,xPos + m_pFunsCtn[i]->GetFixedWidth(),yPos + m_pFunsCtn[i]->GetFixedHeight()};
		m_pFunsCtn[i]->SetPos(rcLstCtn);
	}

	//����ȡ�����
	Init_Ctn1_1();

	//���ڴ洢���
	Init_Ctn1_2();

	//����ת��
	Init_Ctn1_3();

	//�����޸�����
	Init_Ctn2_1();

	//���ڴ浥����
	Init_Ctn1_5();

	//���ڳ�ֵ
	Init_Ctn1_7();

	//���ڰ�ȫ����
	Init_Ctn3_1();

	//���س��ÿؼ�����ʽ
	LoadNormalCtrlSkin();

	//���ظ�����UI��������Ϣ����
	LoadNotifyFunctions();

	ShowFuns(0);
}

//��ʼ����ϷǮ������
void CPBankWndEx::InitGameWalletTable()
{
	m_vecGmWallets.clear();

	CGameListCtrl* pGameList = &GetMainRoom()->m_pGameListWnd->m_GameList;

 	CAFCGameListItem * pGameKindItem = pGameList->GetKindParentItem(); //��ȡ����
 	for (INT_PTR i = 0; i < pGameKindItem->m_ItemPtrArray.GetCount(); ++i)
 	{
 		CAFCKindItem* pKindItem=(CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
		if (pKindItem!=NULL && pKindItem->m_KindInfo.uKindID != 999)
		{
			for (INT_PTR j = 0; j < pKindItem->m_ItemPtrArray.GetCount(); ++j)
			{
				CAFCNameItem* pNameItem=(CAFCNameItem*)pKindItem->m_ItemPtrArray.GetAt(j);
				if (pNameItem != NULL)
				{
					if (pNameItem->m_uDataType == GLK_GAME_NAME)
					{
						TGmWalletNode tagNode;
						tagNode.uNameID = pNameItem->m_NameInfo.uNameID;
						::strcpy(tagNode.szGameName,pNameItem->m_NameInfo.szGameName);
						tagNode.i64Wallet = 0;
						m_vecGmWallets.push_back(tagNode);
					}
				}
			}
		}
 	}
}

//����ȡ�����
void CPBankWndEx::Init_Ctn1_1()
{
	CLabelUI*  pLabCtrl = NULL;
	CEditUI*   pEdtCtrl = NULL;
	CComboUI*  pCobCtrl = NULL;

	CBcfFile fMsg(CBcfFile::GetAppPath() + "ClientMessage.bcf");

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn1_OutCoint"));
	pEdtCtrl->SetMaxChar(23);

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn1_Pwd"));
	pEdtCtrl->SetMaxChar(20);

	pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl("Cob_S1_Ctn1_Games"));
	pCobCtrl->SetEnabled(false);
	pCobCtrl->RemoveAll();

	vector<TGmWalletNode>::iterator it = m_vecGmWallets.begin();
	while(it != m_vecGmWallets.end())
	{
		CListLabelElementUI* p = new CListLabelElementUI();
		p->SetText(it->szGameName);
		pCobCtrl->Add(p);
		it++;
	}
}

//���ڴ�����
void CPBankWndEx::Init_Ctn1_2()
{
	CLabelUI*  pLabCtrl = NULL;
	CEditUI*   pEdtCtrl = NULL;
	CComboUI*  pCobCtrl = NULL;

	CBcfFile fMsg(CBcfFile::GetAppPath() + "ClientMessage.bcf");

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn2_OutCoint"));
	pEdtCtrl->SetMaxChar(23);

	pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl("Cob_S1_Ctn2_Game"));
	pCobCtrl->SetEnabled(false);
	pCobCtrl->RemoveAll();
}

//����ת��
void CPBankWndEx::Init_Ctn1_3()
{
	COptionUI* pOptCtrl = NULL;
	CEditUI*   pEdtCtrl = NULL;

	CBcfFile fMsg(CBcfFile::GetAppPath() + "ClientMessage.bcf");

	pOptCtrl = static_cast<COptionUI*>(m_pm.FindControl("Opt_S1_Ctn3_ID"));
	pOptCtrl->Selected(true);

	CLabelUI* pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn3_1"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","TransUserID","�Է�ID��"));

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn3_TargetUser"));
	pEdtCtrl->SetMaxChar(32);

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn3_TransCoint"));
	pEdtCtrl->SetMaxChar(23);

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn3_Pwd"));
	pEdtCtrl->SetMaxChar(20);

}

//���ڴ浥����
void CPBankWndEx::Init_Ctn1_5()
{
	COptionUI* pOptCtrl = NULL;
	CEditUI*   pEdtCtrl = NULL;

	CBcfFile fMsg(CBcfFile::GetAppPath() + "ClientMessage.bcf");

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn5_OutCoint"));
	pEdtCtrl->SetMaxChar(23);

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn5_Pwd"));
	pEdtCtrl->SetMaxChar(20);

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn5_Suc_Serial"));
	pEdtCtrl->SetReadOnly(true);

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn5_Suc_Pwd"));
	pEdtCtrl->SetReadOnly(true);
}

//���ڳ�ֵ
void CPBankWndEx::Init_Ctn1_7()
{
	CEditUI*   pEdtCtrl = NULL;

	CBcfFile fMsg(CBcfFile::GetAppPath() + "ClientMessage.bcf");

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn7_TargetUser"));
	pEdtCtrl->SetMaxChar(16);

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn7_Serial"));
	pEdtCtrl->SetMaxChar(16);

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn7_Pwd"));
	pEdtCtrl->SetMaxChar(16);
}

//�����޸�����
void CPBankWndEx::Init_Ctn2_1()
{
	CLabelUI*  pLabCtrl = NULL;
	CEditUI*   pEdtCtrl = NULL;

	CBcfFile fMsg(CBcfFile::GetAppPath() + "ClientMessage.bcf");

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S2_Ctn1_1"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","OldPW","���������룺"));

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S2_Ctn1_2"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","NewPW","���������룺"));

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S2_Ctn1_3"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","ConfirmPW","������ȷ�ϣ�"));

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S2_Ctn1_OldPwd"));
	pEdtCtrl->SetMaxChar(20);

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S2_Ctn1_NewPwd"));
	pEdtCtrl->SetMaxChar(20);

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S2_Ctn1_PwdCfm"));
	pEdtCtrl->SetMaxChar(20);
}

//���ڰ�ȫ����
void CPBankWndEx::Init_Ctn3_1()
{
	COptionUI* pOptCtrl = NULL;
	CComboUI* pCobCtrl = NULL;

	int nSelect = AfxGetApp()->GetProfileInt(TEXT("BankSafe"),TEXT("Option"),1);
	CString strMin = AfxGetApp()->GetProfileString(TEXT("BankSafe"),TEXT("Minutes"),"5");

	pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl(_T("Cob_S3_Ctn1_Minutes")));
	LPCTSTR szItems[6] = {_TEXT("5"),_TEXT("15"),_TEXT("30"),_TEXT("60"),_TEXT("120")};
	LoadComboItems(pCobCtrl,szItems,5);
	pCobCtrl->SetEnabled(false);

	if (nSelect == 1)
	{
		pOptCtrl = static_cast<COptionUI*>(m_pm.FindControl("Opt_S3_Ctn1_1"));
		pOptCtrl->Selected(true);
	}
	else if (nSelect == 2)
	{
		pOptCtrl = static_cast<COptionUI*>(m_pm.FindControl("Opt_S3_Ctn1_2"));
		pOptCtrl->Selected(true);
		pCobCtrl->SetEnabled(true);

		//�Ƿ�ʱ���
		m_bCheckFrequence = true;
		m_iMinutes = _atoi64((const char*)strMin.GetBuffer());
	}
	else if (nSelect == 3)
	{
		pOptCtrl = static_cast<COptionUI*>(m_pm.FindControl("Opt_S3_Ctn1_3"));
		pOptCtrl->Selected(true);
	}

	::SetTimer(m_hWnd,IDT_TIMEOUT_TO_LOGOUT,5000,NULL);

	return;
}

//����Combol���б���
void CPBankWndEx::LoadComboItems(CComboUI* pCtrl,LPCTSTR items[],int nums,int nSelect/* = 0*/)
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
bool CPBankWndEx::SelectComboItem(BzDui::CComboUI* pCtrl,LPCTSTR szText)
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

void CPBankWndEx::Notify(TNotifyUI& msg)
{
	m_dwTickCount = ::GetTickCount();

	if(msg.sType == _T("click")) 
	{
		//�л�����Tabҳ
		for (int i = 0; i < SORT_NUMS; ++i)
		{
			if (msg.pSender == m_pSortBnt[i])
			{
				ShowFuns(i);
				return;
			}
		}

		//���Tabҳ�б���
		for (int i = 0; i < SORT_NUMS; ++i)
		{
			list<TFunsNode>::iterator it = m_lstFuns[i].begin();
			while(it != m_lstFuns[i].end())
			{
				if (msg.pSender == it->pFunsBnt)
				{
					if (m_pSelFunNode)
					{
						m_pSelFunNode->pFunsBnt->SetEnabled(true);
						if (m_pSelFunNode->pFunsCtn)
						{
							m_pSelFunNode->pFunsCtn->SetVisible(false);
						}
					}

					m_pSelFunNode = (TFunsNode*)&(*it);
					m_pSelFunNode->pFunsBnt->SetEnabled(false);
					if (m_pSelFunNode->pFunsCtn)
					{
						//��ʾ����
						m_pSelFunNode->pFunsCtn->SetVisible(true);

						//�����ύ��ť��ʹ��
						m_pSubmitBnt->SetVisible(m_pSelFunNode->bNeedSubmit);
						m_pSubmitBnt->SetText(m_pSelFunNode->sBntName);

						//�򴰿ڷ��ʹ���Ϣ
						if (m_pSelFunNode->pNotifyFunPtr)
						{
							TNotifyUI tagUIMsg;
							tagUIMsg.pSender = m_pSelFunNode->pFunsCtn;
							tagUIMsg.sType = "on_contain_open";
							(this->*(m_pSelFunNode->pNotifyFunPtr))(tagUIMsg);
						}
					}
					return;
				}
				it++;
			}
		}

		//�ύ��ť,����Ӧ����ת����ӦNotify_x_x�����д���
		if(msg.pSender->GetName() == _T("Btn_OK"))
		{
			if (m_pSelFunNode)
			{
				if (m_pSelFunNode->pNotifyFunPtr)
				{
					(this->*(m_pSelFunNode->pNotifyFunPtr))(msg);
				}
			}
			return;
		}

		//�رհ�ť
		if(msg.pSender->GetName() == _T("Btn_Close"))
		{
			::KillTimer(m_hWnd,IDT_TIMEOUT_TO_LOGOUT);
			Close();
			m_pRoom->PostMessage(WM_BANK_CLOSE,0,0);
			return;
		}

	}

	//����UI����������Ϣ��Ӧ���
	CControlUI* pParentCtrl = msg.pSender->GetParent();
	for (int i = 0; i < SORT_NUMS; ++i)
	{
		list<TFunsNode>::iterator it = m_lstFuns[i].begin();
		while(it != m_lstFuns[i].end())
		{
			if (pParentCtrl == it->pFunsCtn) //�ҵ��ؼ��Ĺ�������
			{
				//ת������Ӧ����Ϣ������
				if (it->pNotifyFunPtr)
				{
					(this->*(it->pNotifyFunPtr))(msg);
				}
				return;
			}
			it++;
		}
	}

	return;

}


void CPBankWndEx::CenterWindow()
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
void CPBankWndEx::LoadNormalCtrlSkin()
{
	int idx = 0;
	CStdPtrArray* pCtrls = NULL;

	for (int i = 1; i <= SORT_NUMS; ++i)
	{
		int idx = 0;
		pCtrls = m_pm.FindSubControlsByClass(m_pFunsCtn[i - 1],_TEXT("ButtonUI"));
		CButtonUI* pBntUI = NULL;
		do 
		{
			pBntUI = static_cast<CButtonUI*>(pCtrls->GetAt(idx++));
			if (pBntUI)
			{
				//�޸Ĺ��ܰ�ť����ʽ
				pBntUI->SetHotImage(_TEXT("file='BankWnd2\\tree_bnt.png' dest='0,0,200,100' source='0,0,20,20'"));
				pBntUI->SetPushedImage(_TEXT("file='BankWnd2\\tree_bnt.png' dest='0,0,200,100' source='0,0,20,20'"));
				pBntUI->SetDisabledImage(_TEXT("file='BankWnd2\\tree_bnt.png' dest='0,0,200,100' source='0,20,20,40'"));
				pBntUI->SetDisabledTextColor(RGB(0xff,0xff,0xff));
			}
		} while (pBntUI);
	}

	pCtrls = m_pm.FindSubControlsByClass(m_pm.GetRoot(),_TEXT("EditUI"));
	CEditUI* pEditUI = NULL;
	RECT rcText = {4,1,4,0};
	do 
	{
		pEditUI = static_cast<CEditUI*>(pCtrls->GetAt(idx++));

		if (pEditUI)
		{
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
	RECT rc = {6,0,0,0};
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

//Ϊĳһ���UI��������Ϣ��Ӧ����
//@parm uSortIdx ��������\
        strCtnName ���ܵ��ڴ�������ķ�װ��������\
		lpFunPtr ��Ϣ���յĺ���\
		bNeedSubmit �Ƿ���Ҫ�ύ��ť
void CPBankWndEx::BindNotify(UINT uSortIdx,BzDui::CStdString strCtnName,lpNotifyFun lpFunPtr,bool bNeedSubmit/* = true*/,BzDui::CStdString strBntName/* = "�ύ"*/)
{
	list<TFunsNode>::iterator it = m_lstFuns[uSortIdx].begin();
	while(it != m_lstFuns[uSortIdx].end())
	{
		if (strCtnName == it->pFunsCtn->GetName())
		{
			it->pNotifyFunPtr = lpFunPtr;
			it->bNeedSubmit = bNeedSubmit;
			it->sBntName = strBntName;
			break;
		}
		it++;
	}
}

//չ��ĳ����������б�
void CPBankWndEx::ShowFuns(int nIdx)
{
	CControlUI* pCtrl = NULL;

	int xPos = m_pSortBnt[0]->GetFixedXY().cx;
	int yPos = m_pSortBnt[0]->GetFixedXY().cy;
	int iW = m_pSortBnt[0]->GetFixedWidth();
	int iH = m_pSortBnt[0]->GetFixedHeight();

	//nIdx���ϵ����Ͱ�ť���Ͽ�
	pCtrl = m_pm.FindControl("VerticalLayoutUI1");
	xPos += pCtrl->GetX();
	yPos += pCtrl->GetY();

	for (int i = 0; i <= nIdx; ++i)
	{
		RECT rc = {xPos,yPos,xPos + iW,yPos + iH};
		m_pSortBnt[i]->SetPos(rc);
		yPos += iH + 3;
	}

	//nIdx���ϵ����Ͱ�ť���¿�
	pCtrl = m_pm.FindControl("VerticalLayoutUI1");
	yPos = pCtrl->GetY() + pCtrl->GetFixedHeight() - (iH + 3);

	for (int i = SORT_NUMS - 1; i > nIdx; --i)
	{
		RECT rc = {xPos,yPos,xPos + iW,yPos + iH};
		m_pSortBnt[i]->SetPos(rc);
		yPos -= (iH + 3);
	}

	//�����б���ʾ���м�
	for (int i = 0; i < SORT_NUMS; ++i)
	{
		m_pFunsCtn[i]->SetVisible(i == nIdx);
	}	

	//ÿ�ε�����Ͱ�ť��Ĭ��ѡ����ʾ��һ������
	list<TFunsNode>::iterator it = m_lstFuns[nIdx].begin();

	if (it != m_lstFuns[nIdx].end())
	{
		TNotifyUI tagNotify;
		tagNotify.sType = "click";
		tagNotify.pSender = it->pFunsBnt;
		Notify(tagNotify);
	}
	
}

//���ת���Ĵ�д
void CPBankWndEx::ChangeDigitToChinese(__int64 i64Num, CString &strResult)
{
	int i,size,high;

	strResult = "";
	digitArray.clear();
	i = 0;
	size = 0;

	//����ֵ��������������������У���������
	while(i64Num)
	{
		digitArray.push_back(i64Num%10);
		i64Num /=10;
		size++;
	}

	for(i = size-1; i >= 0; i--)
	{
		//��������
		if(digitArray[i] == 0 )//����Ϊ���������
		{
			if(i-1 == -1) break; 
			if(digitArray[i-1] != 0 && (szUnit[i] != "��" && szUnit[i] != "��" )) //���ڡ���λΪ0ʱ����������ʱ�����㡣
				strResult += szDigit[digitArray[i]];
		}
		else 
			strResult += szDigit[digitArray[i]];		

		//���뵥λ
		if(digitArray[i] == 0 && (szUnit[i] != "��" && szUnit[i] != "��" ) ) //���ڡ���λΪ0ʱ����
			continue; 
		strResult += szUnit[i];
	}
	if(strResult.Find("����") != -1)//�ַ��������������������⴦��
		strResult.Replace("����", "��");
}

//����������������������ͬ��ת��Ϊ���Ĵ�д
void CPBankWndEx::ChangeEtToChinese(BzDui::CEditUI* pEditCtrl,BzDui::CTextUI* pTextCtrl)
{
	CString strOutCoin = pEditCtrl->GetText();

	//ȥ��������еķָ�����
	if(Glb().m_bUseSpace)
		strOutCoin.Replace(Glb().m_strSpaceChar, "");	//ת��ҳ��		

	//��֤�ַ����Ƿ���0-9������
	for(int i = 0; i < strOutCoin.GetLength(); i++)
	{
		if(strOutCoin[i] < '0' || strOutCoin[i] > '9')
		{
			pEditCtrl->SetText("");
			return;
		}
	}

	__int64 i64Num  = _tstoi64(strOutCoin.GetBuffer());
	TCHAR szNum[128];

	if(bFlag == false)	
	{
		//�ַ����м���ָ���
		GetNumString(szNum, i64Num, 0, Glb().m_bUseSpace, Glb().m_strSpaceChar);
		bFlag = true;
		pEditCtrl->SetText(szNum);
		//���ù�굽�����
		pEditCtrl->SetSel(-1);
	}
	else
		bFlag = false;

	ChangeDigitToChinese(i64Num, strOutCoin);	

	pTextCtrl->SetText(strOutCoin);

	return;
}

//ͨ����ϷNameID,�õ���Ϸ������
char* CPBankWndEx::GetGameNameById(UINT uNameID)
{
	vector<TGmWalletNode>::iterator it = m_vecGmWallets.begin();
	while(it != m_vecGmWallets.end())
	{
		if (it->uNameID == uNameID)
		{
			return it->szGameName;
		}
		it++;
	}

	return NULL;
}

//ͨ����ϷName,�õ���Ϸ��NameID
UINT CPBankWndEx::GetGameIDByName(LPCTSTR pGameName)
{
	vector<TGmWalletNode>::iterator it = m_vecGmWallets.begin();
	while(it != m_vecGmWallets.end())
	{
		if (strcmp(it->szGameName,pGameName) == 0)
		{
			return it->uNameID;
		}
		it++;
	}

	return 0;
}

//ͨ����ϷNameID,�õ���Ϸ������
int CPBankWndEx::GetWalletIndexById(UINT uNameID)
{
	for (int i = 0; i < m_vecGmWallets.size(); ++i)
	{
		if (uNameID == m_vecGmWallets[i].uNameID)
		{
			return i;
		}
	}

	return -1;
}

//ͨ����ϷNameID,������Ǯ����Ǯ
bool CPBankWndEx::SetGameWalletMoney(UINT uNameID,__int64 NewMoney)
{
	vector<TGmWalletNode>::iterator it = m_vecGmWallets.begin();
	while(it != m_vecGmWallets.end())
	{
		if (it->uNameID == uNameID)
		{
			it->i64Wallet = NewMoney;
			return true;
		}
		it++;
	}

	return false;
}

//ͨ����ϷNameID,������Ǯ�����Ǯ
bool CPBankWndEx::AddGameWalletMoney(UINT uNameID,__int64 AddMoney)
{
	vector<TGmWalletNode>::iterator it = m_vecGmWallets.begin();
	while(it != m_vecGmWallets.end())
	{
		if (it->uNameID == uNameID)
		{
			it->i64Wallet += AddMoney;
			return true;
		}
		it++;
	}

	return false;
}

//ͨ����ϷName,��ȡǮ����Ǯ
__int64 CPBankWndEx::GetGameWalletMoney(LPCTSTR pGameName)
{
	vector<TGmWalletNode>::iterator it = m_vecGmWallets.begin();
	while(it != m_vecGmWallets.end())
	{
		if (strcmp(it->szGameName,pGameName) == 0)
		{
			return it->i64Wallet;
		}
		it++;
	}

	return 0;
}

//ͨ����ϷNameID,��ȡǮ����Ǯ
__int64 CPBankWndEx::GetGameWalletMoney(UINT uNameID)
{
	vector<TGmWalletNode>::iterator it = m_vecGmWallets.begin();
	while(it != m_vecGmWallets.end())
	{
		if (it->uNameID == uNameID)
		{
			return it->i64Wallet;
		}
		it++;
	}

	return 0;
}

///
/// ���òƸ���Ϣ
/// @param nBank ���д��
/// @param nWallet �����ֽ�
/// @return void
///
void CPBankWndEx::SetWealthInfor(__int64 i64Bank, __int64 i64Wallet)
{
	m_i64BankMoney = i64Bank;
	UpdateWealthInfor();
}

///
/// ��ʾ
/// @param pToCtrl ��ĳһ�ؼ�����ʾ
/// @param lpSecName ClientMessage.bcf�еĶ���
/// @param lpKeyName ClientMessage.bcf�еļ�ֵ
/// @param lpDefault ClientMessage.bcfû���ҵ���ֵʱ��ʹ�õ�Ĭ��ֵ
/// @return void
///
void CPBankWndEx::ShowTipMessage(BzDui::CControlUI* pToCtrl,LPCTSTR lpSecName,LPCTSTR lpKeyName,LPCTSTR lpDefault,...)
{
	char szFormat[BUFSIZ];
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	strcpy(szFormat,fMsg.GetKeyVal(lpSecName,lpKeyName,lpDefault));

	va_list pArg;
	va_start(pArg,lpDefault);
	char szMessage[BUFSIZ];
	
	#ifdef WIN32
		_vsnprintf(szMessage,BUFSIZ - 1,szFormat,pArg);
	#else
		vsnprintf(szMessage,BUFSIZ - 1,szFormat,pArg);
	#endif

	va_end(pArg);

	if (pToCtrl)
	{
		;
	}
	else
	{
		DUIOkeyMsgBox(CPBankWndEx::m_hInstanceWnd,false,"%s",szMessage);
	}

	return;

}

//�����û�ID��ȡ�ǳ�
bool CPBankWndEx::OnIDKillFocus(BzDui::CEditUI* pEdtCtrl,BYTE nType,BYTE nUserful)
{
	CLabelUI* pLabCtrl = NULL;
	CString strTemp = "";

	// �Է�ID
	strTemp = pEdtCtrl->GetText();

	if (strTemp.GetLength() == 0)
	{
		return true;
	}

	GetNickNameID_t* p = new GetNickNameID_t;

	if (!p)
	{
		return true;
	}

	if(nType == NICK_ID_BYID) //ͨ��ID
	{
		// ��Ч������
		if(atol(strTemp) <= 0)
		{
			return false;
		}

		p->_user_id = atol(strTemp);
	}
	else if(nType == NICK_ID_BYNICKNAME) //ͨ���ǳ�
	{
		strcpy(p->_nickname,(const char*)strTemp.GetBuffer());
	}
	else
	{
		return true;
	}

	p->_nType = nType;
	p->_nUseful = nUserful;

	GetMainRoom()->PostMessage(WM_BANK_GET_NICKNAMEID,0,LPARAM(p));

	return true;
	//WM_BANK_GET_NICKNAME_ONID
}

/// ת�ʳɹ��������������
/// @param Opt 0-��ʾת�������ˣ�1-��ʾ�ɱ���ת����Լ�
/// @param nMoney ��ʾ�����������Ľ����
void CPBankWndEx::UpdateUserMoneyByTransfer(int OptType, __int64 i64Money)
{
	if( OptType == 0 ) //ȡ��
	{
		m_i64BankMoney	-= i64Money;
	}
	else //����
	{
		m_i64BankMoney	+= i64Money;
	}

	UpdateWealthInfor();

}

///
/// �յ�һ��ת�˼�¼
/// @param void
/// @return void
///
void CPBankWndEx::OnReceivedTransferRecord(MSG_GR_R_TransferRecord_t *pRecord)
{
	m_mapRecord[pRecord->nSeriNo] = *pRecord;
	m_pRecordList->NeedUpdate();
	UpdateRecordPageState();
}

///ˢ�¼�¼��ҳ��
void CPBankWndEx::UpdateRecordPageState()
{

	int iPageCount = ((m_mapRecord.size() % RECORD_PAGE_SIZE) == 0) ? 
		m_mapRecord.size() / RECORD_PAGE_SIZE : m_mapRecord.size() / RECORD_PAGE_SIZE + 1;
	int iCurrPage = m_iRecDispPos / RECORD_PAGE_SIZE + 1;

	CLabelUI* pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn4_4"));
	TCHAR szBuffer[32];
	_stprintf(szBuffer,_T("��%d/%dҳ"),iCurrPage,iPageCount);
	pLabCtrl->SetText(szBuffer);

	//��ҳ�ؼ���ť
	CButtonUI* pBntCtrl = NULL;
	pBntCtrl = static_cast<CButtonUI*>(m_pm.FindControl("Bnt_S1_Ctn4_First"));
	pBntCtrl->SetEnabled(iCurrPage > 1);
	pBntCtrl = static_cast<CButtonUI*>(m_pm.FindControl("Bnt_S1_Ctn4_Previous"));
	pBntCtrl->SetEnabled(iCurrPage > 1);
	pBntCtrl = static_cast<CButtonUI*>(m_pm.FindControl("Bnt_S1_Ctn4_Next"));
	pBntCtrl->SetEnabled(iCurrPage < iPageCount);
	pBntCtrl = static_cast<CButtonUI*>(m_pm.FindControl("Bnt_S1_Ctn4_Rear"));
	pBntCtrl->SetEnabled(iCurrPage < iPageCount);
}

///�����û�ID�ǳƵĲ�ѯ���
void CPBankWndEx::UpdateNicknameIDResult(GetNickNameID_t* pNID)
{
	CLabelUI* pLabCtrl = NULL;
	CLabelUI* pLabCtrl_temp = NULL;
	TCHAR szText[64];

	if (pNID->_nUseful == NICK_ID_USE_TO_TRANSFER)
	{
		pLabCtrl_temp = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn3_NickNameID_temp"));
		pLabCtrl_temp->SetText("");

		pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn3_NickNameID"));

		if (pNID->_nType == NICK_ID_BYID)
		{
			if (strlen(pNID->_nickname) == 0)
			{
				pLabCtrl->SetText(_T("(��Ҳ�����)"));
			}
			else
			{
				_stprintf(szText,"(�ǳƣ�%s)",pNID->_nickname);
				pLabCtrl->SetText(szText);
				pLabCtrl_temp->SetText(pNID->_nickname);
			}
		}
		else if (pNID->_nType == NICK_ID_BYNICKNAME)
		{
			if (pNID->_user_id == 0)
			{
				pLabCtrl->SetText(_T("(��Ҳ�����)"));
			}
			else
			{
				_stprintf(szText,"(ID��%d)",pNID->_user_id);
				pLabCtrl->SetText(szText);
				_stprintf(szText,"%d",pNID->_user_id);
				pLabCtrl_temp->SetText(szText);
			}
		}
		
	}
	else if (pNID->_nUseful == NICK_ID_USE_TO_DEPOSIT)
	{
		pLabCtrl_temp = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn7_NickNameID_temp"));
		pLabCtrl_temp->SetText("");

		pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn7_NickNameID"));

		if (pNID->_nType == NICK_ID_BYID)
		{
			if (strlen(pNID->_nickname) == 0)
			{
				pLabCtrl->SetText(_T("(��Ҳ�����)"));
			}
			else
			{
				_stprintf(szText,"(�ǳƣ�%s)",pNID->_nickname);
				pLabCtrl->SetText(szText);
				pLabCtrl_temp->SetText(pNID->_nickname);
			}
		}
		else if (pNID->_nType == NICK_ID_BYNICKNAME)
		{
			return;
		}
	}

	return;
}

///
/// ���²Ƹ���Ϣ
/// @return void
///
void CPBankWndEx::UpdateWealthInfor()
{
	TCHAR szNum[128] = {0};
	TCHAR szTemp[128] = {0};
	TCHAR szBank[128] = {0};
	GetNumString(szTemp,m_i64BankMoney,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);
	_stprintf(szBank,_T("<b>%s</b>"),szTemp);

	CLabelUI* pLabCtrl = NULL;
	CEditUI*  pEdtCtrl = NULL;
	CComboUI* pCobCtrl = NULL;

	//���б�����
	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn1_Bank"));
	pLabCtrl->SetText(szBank);

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn3_Bank"));
	pLabCtrl->SetText(szBank);

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn5_Bank"));
	pLabCtrl->SetText(szBank);

	//����ȡ����ʾ����ϷǮ��
	pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl("Cob_S1_Ctn1_Games"));
	if (pCobCtrl->GetCurSel() != -1)
	{
		GetNumString(szNum,m_vecGmWallets[pCobCtrl->GetCurSel()].i64Wallet,
			Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);
		pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Lab_S1_Ctn1_GameCoint"));
		pEdtCtrl->SetText(szNum);
	}

	//�����ܲƸ�
	__int64 i64TotalMoney = m_i64BankMoney;
	vector<TGmWalletNode>::iterator it = m_vecGmWallets.begin();
	while(it != m_vecGmWallets.end())
	{
		if (it->i64Wallet > 0)
		{
			i64TotalMoney += it->i64Wallet;
		}
		it++;
	}

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_All_Money"));
	GetNumString(szTemp,i64TotalMoney,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);
	pLabCtrl->SetText(szTemp);


	//���³�ʼ��ʾȡ���õ���Ϸѡ���б�����m_i64BankMoney�б䶯���п���ʹ��ѡ����б仯
	pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl("Cob_S1_Ctn2_Game"));
	int nLastSel = -1,nIdx = 0;
	BzDui::CStdString sLastSelName = " ";
	if (pCobCtrl->GetCurSel() != -1)
	{
		pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn2_GameCoint"));
		pLabCtrl->SetText("");

		sLastSelName = pCobCtrl->GetText();
	}

	pCobCtrl->SetEnabled(false);
	pCobCtrl->RemoveAll();

	it = m_vecGmWallets.begin();
	while(it != m_vecGmWallets.end())
	{
		if (it->i64Wallet > 0)
		{
			CListLabelElementUI* p = new CListLabelElementUI();
			p->SetText(it->szGameName);
			pCobCtrl->Add(p);

			if (strcmp(sLastSelName.GetData(),it->szGameName) == 0)
			{
				nLastSel = nIdx;
			}
			nIdx++;
		}
		it++;
	}

	pCobCtrl->SetEnabled(true);

	if (nLastSel != -1)
	{
		pCobCtrl->SelectItem(nLastSel);
	}

	return;
}


/*------------------------------------------------------------------------------------
	���������ǹ������UI�����ľ���ʵ�֡�
	�ؼ���Ϣ�Ѱ����������֣���Ϣ�ѷַ���Notify_x_x����
	������Ա��Ҫ������LoadNotifyFunctions�����ƶ���Ϣ�ַ�����

  ----------------------------------------------------------------------------------*/

//���ظ�����UI��������Ϣ����
void CPBankWndEx::LoadNotifyFunctions()
{
	BindNotify(0,_T("Ctn_Type1_Ctn1"),&CPBankWndEx::Notify_1_1,true);
	BindNotify(0,_T("Ctn_Type1_Ctn2"),&CPBankWndEx::Notify_1_2,true);
	BindNotify(0,_T("Ctn_Type1_Ctn3"),&CPBankWndEx::Notify_1_3,true);
	BindNotify(0,_T("Ctn_Type1_Ctn4"),&CPBankWndEx::Notify_1_4,false);
	BindNotify(0,_T("Ctn_Type1_Ctn5"),&CPBankWndEx::Notify_1_5,true);
	BindNotify(0,_T("Ctn_Type1_Ctn6"),&CPBankWndEx::Notify_1_6,false);
	BindNotify(0,_T("Ctn_Type1_Ctn7"),&CPBankWndEx::Notify_1_7,true);
	BindNotify(1,_T("Ctn_Type2_Ctn1"),&CPBankWndEx::Notify_2_1,true);
	BindNotify(2,_T("Ctn_Type3_Ctn1"),&CPBankWndEx::Notify_3_1,true,_T("����"));
}

//������1���еġ�����1����Ϣ������
void CPBankWndEx::Notify_1_1(BzDui::TNotifyUI& msg)
{
	if (msg.sType == "click")
	{
		if (msg.pSender->GetName() == _T("Btn_S1_Ctn1_All")) //ȫ��
		{
			TCHAR szNum[128];
			GetNumString(szNum,m_i64BankMoney,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);
			CEditUI* pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn1_OutCoint"));
			pEdtCtrl->SetFocus();
			pEdtCtrl->SetText(szNum);
			return;
		}
		else if (msg.pSender->GetName() == _T("Btn_OK")) //�ύ
		{
			CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
			CEditUI* pOutCointEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn1_OutCoint"));

			CComboUI* pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl("Cob_S1_Ctn1_Games"));
			if (pCobCtrl->GetCurSel() == -1)
			{
				DUIOkeyMsgBox(m_hWnd,false,"��ѡ�������Ϸ��");
				pCobCtrl->SetFocus();
				return;
			}

			UINT uGameID = m_vecGmWallets[pCobCtrl->GetCurSel()].uNameID;

			//�����Ϸ��ķ����Ѵ򿪣������Բ���
			for (BYTE i = 0; i < MAX_GAME_ROOM; ++i)
			{
				// ��Ϸ����Ϸ�����ұ���Ϣ��
				CGameRoomEx* pGameRoomEx = (CGameRoomEx*)GetMainRoom()->m_RoomInfo[i].pGameRoomWnd;
				if (m_pRoom == pGameRoomEx) //������Ϣ�ķ��������ٸ��½����
				{
					continue;
				}
				if (NULL != pGameRoomEx)
				{
					if (pGameRoomEx->m_GameInfo.uNameID == uGameID)
					{
						ShowTipMessage(NULL,"PBankWnd2","OutSv_Room_NoRule",
							"��Ϸ��%s�������Ѵ򿪣�����ȡ�������Ҫ���뵽��ӦӦ������ʹ�����н���ȡ�",
							pGameRoomEx->m_GameInfo.szGameName);
						return;
					}
				}	
			}

			TCHAR szMsg[256];
			CString strNum = pOutCointEdt->GetText();
			CString str;

			TCHAR szTail[16];
			_tcscpy(szTail, TEXT(""));
			for (int i=0; i<Glb().m_nPowerOfGold; ++i)
			{
				_tcscat( szTail, TEXT("0"));
			}

			//����зָ�������ȥ���ַ�����ķָ���
			if(Glb().m_bUseSpace)
				strNum.Replace(Glb().m_strSpaceChar,"");

			/// ���û������
			if (strNum.GetLength() == 0)
			{	
				ShowTipMessage(NULL,"PBankWnd2","OutSv_Msg_NotInputCoin",
					"��������Ҫ��ȡ������");
				pOutCointEdt->SetFocus();
				return;
			}

			/// �������̫��
			if (strNum.GetLength()<Glb().m_nPowerOfGold)
			{
				ShowTipMessage(NULL,"PBankWnd2","OutSv_Msg_NotGetCoin",
					"������Ľ�������ϵͳ���ܣ�����������%dλ����ĩ%dλΪ %s ����������",
					Glb().m_nPowerOfGold+1,Glb().m_nPowerOfGold,szTail);
				pOutCointEdt->SetFocus();
				return;
			}
			/// �������̫��
			if (strNum.GetLength()>18 + Glb().m_nPowerOfGold)
			{
				ShowTipMessage(NULL,"PBankWnd2","OutSv_Msg_NumberTooLarge",
					"�����������̫�󣬳�������Χ�������벻����%I64d%s����ĩ%dλΪ %s ����������",
					0x3FFFFFFFFFFFFFFF,szTail,Glb().m_nPowerOfGold,szTail);
				pOutCointEdt->SetFocus();
				return;
			}

			/// ���ĩ��λ������
			CString strSubNum = strNum.Right(Glb().m_nPowerOfGold);
			if (strSubNum != szTail)
			{
				ShowTipMessage(NULL,"PBankWnd2","OutSv_Msg_NotGetCoin2",
					"������Ľ�������ϵͳ���ܣ�������ĩ%dλΪ %s ����������",
					Glb().m_nPowerOfGold, szTail);
				pOutCointEdt->SetFocus();
				return;
			}
			/// ��ȡǰNλ
			strSubNum = strNum.Left(strNum.GetLength()-Glb().m_nPowerOfGold);
			__int64 i64Num = _atoi64(strSubNum.GetBuffer());
			if (i64Num>0x3FFFFFFFFFFFFFFF)
			{
				ShowTipMessage(NULL,"PBankWnd2","OutSv_Msg_NumberTooLarge",
					"�����������̫�󣬳�������Χ�������벻����%I64d%s����ĩ%dλΪ %s ����������",
					0x3FFFFFFFFFFFFFFF,szTail,Glb().m_nPowerOfGold,szTail);
				pOutCointEdt->SetFocus();
				return;
			}
			if (i64Num<=0)
			{
				ShowTipMessage(NULL,"PBankWnd2","OutSv_Msg_NotGetCoin",
					"������Ľ�������ϵͳ���ܣ�����������%dλ����ĩ%dλΪ %s ����������",
					Glb().m_nPowerOfGold+1,Glb().m_nPowerOfGold,szTail);
				pOutCointEdt->SetFocus();
				return;
			}
			if (i64Num > m_i64BankMoney)
			{
				ShowTipMessage(NULL,"","","������������Ѵ��ڳ������еĴ��");
				pOutCointEdt->SetFocus();
				return;
			}

			CEditUI* pPwdEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn1_Pwd"));
			if (pPwdEdt->GetText().GetLength() == 0)
			{
				ShowTipMessage(NULL,"","","��������������");
				pPwdEdt->SetFocus();
				return;
			}

			/// ����Ϣ
			if (m_pRoom != NULL)
			{
				CComboUI* pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl("Cob_S1_Ctn1_Games"));

				bank_op_normal* p = new bank_op_normal;
				p->_user_id = m_dwOpUserID;
				p->_operate_type = 1;
				p->_game_id = uGameID;
				p->_money = i64Num;

				
				//��ȡ���������
				CGameRoomEx::GetCryptedPasswd(p->_szMD5Pass,(TCHAR*)pPwdEdt->GetText().GetData());

				m_pRoom->PostMessage(WM_BANK,0,(LPARAM)p);
			}
			
			return;
		}
	}
	else if (msg.sType == _T("textchanged"))
	{
		if (msg.pSender->GetName() == _T("Edt_S1_Ctn1_OutCoint"))
		{
			//ת�˽��Сдת��Ϊ���Ĵ�д
			ChangeEtToChinese(static_cast<CEditUI*>(msg.pSender),
								static_cast<CTextUI*>(m_pm.FindControl("Txt_S1_Ctn1_Caps")));
			return;
		}
	}
	else if (msg.sType == _T("itemselect"))
	{
		if( msg.pSender->GetName() == _T("Cob_S1_Ctn1_Games")) 
		{
			TCHAR szNum[128];
			__int64 i64GameMoney = m_vecGmWallets[msg.wParam].i64Wallet;
//			UINT uGameID = m_vecGmWallets[msg.wParam].uNameID;
			GetNumString(szNum,i64GameMoney,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);
			CEditUI* pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Lab_S1_Ctn1_GameCoint"));
			pEdtCtrl->SetText(szNum);

			//�����Ϸ��ķ����Ѵ򿪣������Բ���
// 			for (BYTE i = 0; i < MAX_GAME_ROOM; ++i)
// 			{
// 				// ��Ϸ����Ϸ�����ұ���Ϣ��
// 				CGameRoomEx* pGameRoomEx = (CGameRoomEx*)GetMainRoom()->m_RoomInfo[i].pGameRoomWnd;
// 				if (m_pRoom == pGameRoomEx) //������Ϣ�ķ��������ٸ��½����
// 				{
// 					continue;
// 				}
// 				if (NULL != pGameRoomEx)
// 				{
// 					if (pGameRoomEx->m_GameInfo.uNameID == uGameID)
// 					{
// 						ShowTipMessage(NULL,"PBankWnd2","OutSv_Room_NoRule",
// 							"��Ϸ��%s�������Ѵ򿪣�����ȡ�������Ҫ���뵽��ӦӦ������ʹ�����н���ȡ�",
// 							pGameRoomEx->m_GameInfo.szGameName);
// 						return;
// 					}
// 				}	
// 			}

			return;
		}
	}
}

//������1���еġ�����2����Ϣ������
void CPBankWndEx::Notify_1_2(BzDui::TNotifyUI& msg)
{
	if (msg.sType == "click")
	{
		if (msg.pSender->GetName() == _T("Btn_S1_Ctn2_All")) //ȫ��
		{
			CComboUI* pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl("Cob_S1_Ctn2_Game"));
			if (pCobCtrl->GetCurSel() == -1)
			{
				return;
			}

			LPCTSTR lpGameName = pCobCtrl->GetText().GetData();
			UINT uGameID = GetGameIDByName(lpGameName);
			__int64 i64GameWallet = GetGameWalletMoney(lpGameName);

			//ѡ�������Ϸ���ǵ�ǰ�������Ϸһ��ʱ�����������
			if (ISGameRoom == m_emParWndType)
			{
				CGameRoomEx* pRoomEx = (CGameRoomEx*)m_pRoom;
				if (pRoomEx->m_GameInfo.uNameID == uGameID)
				{
					if (i64GameWallet >= pRoomEx->m_GameInfo.uLessPoint)
					{
						i64GameWallet -= pRoomEx->m_GameInfo.uLessPoint;
					}
				}
			}

			TCHAR szNum[128];
			GetNumString(szNum,i64GameWallet,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);
			CEditUI* pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn2_OutCoint"));
			pEdtCtrl->SetText(szNum);
			pEdtCtrl->SetFocus();

			return;
		}
		else if (msg.pSender->GetName() == _T("Btn_OK")) //�ύ
		{
			CString strMsg;
			CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

			CEditUI* pOutCointEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn2_OutCoint"));

			CComboUI* pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl("Cob_S1_Ctn2_Game"));
			if (pCobCtrl->GetCurSel() == -1)
			{
				DUIOkeyMsgBox(m_hWnd,false,"��ѡ�������Ϸ��");
				pCobCtrl->SetFocus();
				return;
			}

			LPCTSTR lpGameName = pCobCtrl->GetText().GetData();
			UINT uGameID = GetGameIDByName(lpGameName);

			//�����Ϸ��ķ����Ѵ򿪣������Բ���
			for (BYTE i = 0; i < MAX_GAME_ROOM; ++i)
			{
				// ��Ϸ����Ϸ�����ұ���Ϣ��
				CGameRoomEx* pGameRoomEx = (CGameRoomEx*)GetMainRoom()->m_RoomInfo[i].pGameRoomWnd;
				if (m_pRoom == pGameRoomEx) //������Ϣ�ķ��������ٸ��½����
				{
					continue;
				}
				if (NULL != pGameRoomEx)
				{
					if (pGameRoomEx->m_GameInfo.uNameID == uGameID)
					{
						ShowTipMessage(NULL,"PBankWnd2","SrvSv_Room_NoRule",
							"��Ϸ��%s�������Ѵ򿪣������������Ҫ���뵽��ӦӦ������ʹ�����н��д�",
							pGameRoomEx->m_GameInfo.szGameName);
						return;
					}
				}	
			}

			CString strNum = pOutCointEdt->GetText();
			CString str;

			TCHAR szTail[16];
			_tcscpy(szTail, TEXT(""));
			for (int i=0; i<Glb().m_nPowerOfGold; ++i)
			{
				_tcscat( szTail, TEXT("0"));
			}

			//����зָ�������ȥ���ַ�����ķָ���
			if(Glb().m_bUseSpace)
				strNum.Replace(Glb().m_strSpaceChar,"");

			/// ���û������
			if (strNum.GetLength() == 0)
			{	
				ShowTipMessage(NULL,"PBankWnd2","InSv_Msg_NotInputCoin",
									"��������Ҫ��ȡ������");
				pOutCointEdt->SetFocus();
				return;
			}

			/// �������̫��
			if (strNum.GetLength()<Glb().m_nPowerOfGold)
			{
				ShowTipMessage(NULL,"PBankWnd2","InSv_Msg_NotGetCoin",
								"������Ľ�������ϵͳ���ܣ�����������%dλ����ĩ%dλΪ %s ����������",
								Glb().m_nPowerOfGold+1,Glb().m_nPowerOfGold,szTail);
				pOutCointEdt->SetFocus();
				return;
			}
			/// �������̫��
			if (strNum.GetLength()>18 + Glb().m_nPowerOfGold)
			{
				ShowTipMessage(NULL,"PBankWnd2","InSv_Msg_NumberTooLarge",
								"�����������̫�󣬳�������Χ�������벻����%I64d%s����ĩ%dλΪ %s ����������",
								0x3FFFFFFFFFFFFFFF,szTail,Glb().m_nPowerOfGold,szTail);
				pOutCointEdt->SetFocus();
				return;
			}

			/// ���ĩ��λ������
			CString strSubNum = strNum.Right(Glb().m_nPowerOfGold);
			if (strSubNum != szTail)
			{
				ShowTipMessage(NULL,"PBankWnd2","InSv_Msg_NotGetCoin2",
								"������Ľ�������ϵͳ���ܣ�������ĩ%dλΪ %s ����������",
								Glb().m_nPowerOfGold, szTail);
				pOutCointEdt->SetFocus();
				return;
			}
			/// ��ȡǰNλ
			strSubNum = strNum.Left(strNum.GetLength()-Glb().m_nPowerOfGold);
			__int64 i64Num = _atoi64(strSubNum.GetBuffer());
			if (i64Num>0x3FFFFFFFFFFFFFFF)
			{
				ShowTipMessage(NULL,"PBankWnd2","InSv_Msg_NumberTooLarge",
					"�����������̫�󣬳�������Χ�������벻����%I64d%s����ĩ%dλΪ %s ����������",
					0x3FFFFFFFFFFFFFFF,szTail,Glb().m_nPowerOfGold,szTail);
				pOutCointEdt->SetFocus();
				return;
			}
			if (i64Num<=0)
			{
				ShowTipMessage(NULL,"PBankWnd2","InSv_Msg_NotGetCoin",
								"������Ľ�������ϵͳ���ܣ�����������%dλ����ĩ%dλΪ %s ����������",
								Glb().m_nPowerOfGold+1,Glb().m_nPowerOfGold,szTail);
				pOutCointEdt->SetFocus();
				return;
			}

			__int64 i64GameWallet = GetGameWalletMoney(lpGameName);

			if (i64Num > i64GameWallet)
			{
				ShowTipMessage(NULL,"","","������������Ѵ��ڳ������еĴ��");
				pOutCointEdt->SetFocus();
				return;
			}
			
			if (uGameID == 0)
			{
				ShowTipMessage(NULL,"","","GameID����");
				return;
			}

			//ѡ�������Ϸ���ǵ�ǰ�������Ϸһ��ʱ�����������
			if (ISGameRoom == m_emParWndType)
			{
				CGameRoomEx* pRoomEx = (CGameRoomEx*)m_pRoom;
				if (pRoomEx->m_GameInfo.uNameID == uGameID)
				{
					if (i64GameWallet - i64Num < pRoomEx->m_GameInfo.uLessPoint)
					{
						TCHAR szNum[128] = {0};
						GetNumString(szNum,i64GameWallet - i64Num,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);

						ShowTipMessage(NULL,"PBankWnd2","InSv_Msg_LessCoint",
							"������Ϸ��%s������Ϸ��ʣ����%s,С�ڷ������޽��%d�������Դ��,�����µ����������ݶ",
							lpGameName,szNum,pRoomEx->m_GameInfo.uLessPoint);
						pOutCointEdt->SetFocus();
						return;
					}
				}
			}

			/// ����Ϣ
			if (m_pRoom != NULL)
			{
				bank_op_normal* p = new bank_op_normal;
				::memset(p,0,sizeof(bank_op_normal));
				p->_user_id = m_dwOpUserID;
				p->_operate_type = 2;
				p->_game_id = uGameID;
				p->_money = i64Num;
				m_pRoom->PostMessage(WM_BANK,0,(LPARAM)p);
			}

			return;
		}
	}
	else if (msg.sType == _T("textchanged"))
	{
		if (msg.pSender->GetName() == _T("Edt_S1_Ctn2_OutCoint"))
		{
			//ת�˽��Сдת��Ϊ���Ĵ�д
			ChangeEtToChinese(static_cast<CEditUI*>(msg.pSender),
				static_cast<CTextUI*>(m_pm.FindControl("Txt_S1_Ctn2_Caps")));
			return;
		}
	}
	else if (msg.sType == _T("itemselect"))
	{
		if( msg.pSender->GetName() == _T("Cob_S1_Ctn2_Game")) 
		{
			TCHAR szNum[128];
			__int64 i64GameMoney = GetGameWalletMoney(msg.pSender->GetText().GetData());
//			UINT uGameID = GetGameIDByName(msg.pSender->GetText().GetData());
			GetNumString(szNum,i64GameMoney,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);
			CEditUI* pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Lab_S1_Ctn2_GameCoint"));
			pEdtCtrl->SetText(szNum);

			//�����Ϸ��ķ����Ѵ򿪣������Բ���
// 			for (BYTE i = 0; i < MAX_GAME_ROOM; ++i)
// 			{
// 				// ��Ϸ����Ϸ�����ұ���Ϣ��
// 				CGameRoomEx* pGameRoomEx = (CGameRoomEx*)GetMainRoom()->m_RoomInfo[i].pGameRoomWnd;
// 				if (m_pRoom == pGameRoomEx) //������Ϣ�ķ��������ٸ��½����
// 				{
// 					continue;
// 				}
// 				if (NULL != pGameRoomEx)
// 				{
// 					if (pGameRoomEx->m_GameInfo.uNameID == uGameID)
// 					{
// 						ShowTipMessage(NULL,"PBankWnd2","SrvSv_Room_NoRule",
// 							"��Ϸ��%s�������Ѵ򿪣������������Ҫ���뵽��ӦӦ������ʹ�����н��д�",
// 							pGameRoomEx->m_GameInfo.szGameName);
// 						return;
// 					}
// 				}	
// 			}

			return;
		}
	}
}

//������1���еġ�����3����Ϣ������
void CPBankWndEx::Notify_1_3(BzDui::TNotifyUI& msg)
{
	if (msg.sType == "on_contain_open")
	{
// 		m_strTransTargetID = "0";
// 		m_strTransTargetNickName = " ";
	}
	else if (msg.sType == "click")
	{
		if (msg.pSender->GetName() == _T("Btn_OK")) //�ύ
		{
			CEditUI* pEdtOutCoint = NULL;
			pEdtOutCoint = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn3_TransCoint"));

			CEditUI* pEdtPwd = NULL;
			pEdtPwd = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn3_Pwd"));

			/// ȡ�ñ༭������ַ���
			TCHAR szMsg[256];
			CString strNum, str;
			strNum = pEdtOutCoint->GetText();

			//����зָ�������ȥ���ַ�����ķָ���
			if(Glb().m_bUseSpace)
				strNum.Replace(Glb().m_strSpaceChar,"");

			TCHAR szTail[16];
			_tcscpy(szTail, TEXT(""));
			for (int i=0; i<Glb().m_nPowerOfGold; ++i)
			{
				_tcscat( szTail, TEXT("0"));
			}
			/// ���û������
			if (strNum.GetLength() == 0)
			{
				ShowTipMessage(NULL,"PBankWnd2","Trans_Msg_NotInputCoin","��������Ҫ��ȡ������");
				pEdtOutCoint->SetFocus();
				return;
			}
			/// �������̫��
			if (strNum.GetLength()<Glb().m_nPowerOfGold)
			{
				ShowTipMessage(NULL,"PBankWnd2","Trans_Msg_NotGetCoin",
									"������Ľ�������ϵͳ���ܣ�����������%dλ����ĩ%dλΪ %s ����������",
									Glb().m_nPowerOfGold+1,Glb().m_nPowerOfGold,szTail);
				pEdtOutCoint->SetFocus();
				return;
			}
			/// �������̫��
			if (strNum.GetLength()>18+Glb().m_nPowerOfGold)
			{
				ShowTipMessage(NULL,"PBankWnd2","Trans_Msg_NumberTooLargen",
									"�����������̫�󣬳�������Χ�������벻����%I64d%s����ĩ%dλΪ %s ����������",
									0x3FFFFFFFFFFFFFFF,szTail,Glb().m_nPowerOfGold,szTail);
				pEdtOutCoint->SetFocus();
				return;
			}

			/// ���ĩ��λ������
			CString strSubNum = strNum.Right(Glb().m_nPowerOfGold);
			if (strSubNum != szTail)
			{
				ShowTipMessage(NULL,"PBankWnd2","Trans_Msg_NotGetCoin2",
									"������Ľ�������ϵͳ���ܣ�������ĩ%dλΪ %s ����������",
									 Glb().m_nPowerOfGold,szTail);
				pEdtOutCoint->SetFocus();
				return;
			}
			/// ��ȡǰNλ
			strSubNum = strNum.Left(strNum.GetLength()-Glb().m_nPowerOfGold);
			__int64 i64Num = _atoi64(strSubNum.GetBuffer());
			if (i64Num>0x3FFFFFFFFFFFFFFF)
			{
				ShowTipMessage(NULL,"PBankWnd2","Trans_Msg_NumberTooLargen",
									"�����������̫�󣬳�������Χ�������벻����%I64d%s����ĩ%dλΪ %s ����������",
									0x3FFFFFFFFFFFFFFF,szTail,Glb().m_nPowerOfGold,szTail);
				pEdtOutCoint->SetFocus();
				return;
			}

			//���Ŀ���û�
			CEditUI* pEdtTargetUser = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn3_TargetUser"));
			strNum = pEdtTargetUser->GetText();

			//��������
			str = pEdtPwd->GetText();

			if (str.GetLength() < 1)
			{
				ShowTipMessage(NULL,"","","���������룡");
				pEdtPwd->SetFocus();
				return;
			}
			if (i64Num<=0)
			{
				ShowTipMessage(NULL,"PBankWnd2","Trans_Msg_NotGetCoin",
									"������Ľ�������ϵͳ���ܣ�����������%dλ����ĩ%dλΪ %s ����������",
									Glb().m_nPowerOfGold + 1,Glb().m_nPowerOfGold,szTail);
				pEdtOutCoint->SetFocus();
				return;
			}

			COptionUI* pOptCtrl = static_cast<COptionUI*>(m_pm.FindControl("Opt_S1_Ctn3_ID"));

			if (0 == atol(strNum) && pOptCtrl->IsSelected())
			{
				ShowTipMessage(NULL,"BankDlg","WrongID","��������ȷ�����ID��");

				CEditUI* pEdtTargetUser = NULL;
				pEdtTargetUser = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn3_TargetUser"));
				pEdtTargetUser->SetFocus();
				return;
			}

			CLabelUI* pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn3_NickNameID_temp"));

			TransferMoney_t *pTransferMoney = new TransferMoney_t();
			pTransferMoney->i64Money = i64Num;
			pTransferMoney->uDestUserID = atol(strNum);
			_tcscpy(pTransferMoney->szPWD, str.Left(20).GetBuffer());
			pOptCtrl = static_cast<COptionUI*>(m_pm.FindControl("Opt_S1_Ctn3_ID"));

			if (pOptCtrl->IsSelected())
			{
				_tcscpy(pTransferMoney->szDestNickName, pLabCtrl->GetText().GetData());
				pTransferMoney->bUseDestID = true;
			}
			else
			{
				_tcscpy(pTransferMoney->szDestNickName, strNum.GetBuffer());
				pTransferMoney->bUseDestID = false;
			}

			/// ��ʾ�û�ȷ��ת��
			CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

			TCHAR szNum[128];
			GetNumString(szNum, pTransferMoney->i64Money, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);

			CString strFormat;
			CString test;
			pOptCtrl = static_cast<COptionUI*>(m_pm.FindControl("Opt_S1_Ctn3_ID"));
			if(pOptCtrl->IsSelected())
			{
				strFormat = fMsg.GetKeyVal("PBankWnd2", "Trans_Msg_ConfirmId",
					"<p 5>ת��ǰ��ȷ������ת����Ϣ���Ƿ�ȷ��ת�ʣ�<n><y 30>Ŀ���û���<b>%s</b> (ID��<b>%d</b>)</y><n>ת�����<b>%s</b> ���<n></p>");
				wsprintf(szMsg,strFormat,pLabCtrl->GetText().GetData(),pTransferMoney->uDestUserID,szNum);
			}
			else
			{
				if (::atol(pLabCtrl->GetText().GetData()) == m_dwOpUserID)
				{
					DUIOkeyMsgBox(AfxGetMainWnd()->m_hWnd,false,
						fMsg.GetKeyVal("BankDlg","TransferToSelf","������ת�ʸ��Լ����Բ���ϵͳ�ݲ��ṩ�������"));
					return;
				}

				strFormat = fMsg.GetKeyVal("PBankWnd2", "Trans_Msg_ConfirmNN",
					"<p 5>ת��ǰ��ȷ������ת����Ϣ���Ƿ�ȷ��ת�ʣ�<n><y 30>Ŀ���û���<b>%s</b> (ID��<b>%d</b>)</y><n>ת�����<b>%s</b> ���<n></p>");
				wsprintf(szMsg,strFormat,pTransferMoney->szDestNickName,pLabCtrl->GetText().GetData(),szNum);
			}	

			if (IDYES != DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONQUESTION,"����",true,szMsg))
			{
				delete pTransferMoney;
				pTransferMoney = NULL;
				return;
			}

			m_bTransferSuccess = true; 

			m_pRoom->PostMessage(WM_TRANSFER_MONEY,0,LPARAM(pTransferMoney));
			
			//��������
			pEdtPwd->SetText("");

			return;
		}
	}
	else if(msg.sType == _T("selectchanged"))
	{
		if (msg.pSender->GetName() == _T("Opt_S1_Ctn3_ID"))
		{
			//�л���Ŀ���û�ID���뷽ʽ
			CBcfFile fMsg(CBcfFile::GetAppPath() + "ClientMessage.bcf");
			CLabelUI* pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn3_1"));
			pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","TransUserID","�Է�ID��"));

			pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn3_NickNameID"));
			pLabCtrl->SetText("");

			CEditUI* pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn3_TargetUser"));
			pEdtCtrl->SetText("");
			pEdtCtrl->SetFocus();


			return;
		}
		else if (msg.pSender->GetName() == _T("Opt_S1_Ctn3_NickName"))
		{
			//�л���Ŀ���û��ǳ����뷽ʽ
			CBcfFile fMsg(CBcfFile::GetAppPath() + "ClientMessage.bcf");
			CLabelUI* pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn3_1"));
			pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","TransUserNick","�Է��ǳƣ�"));

			pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn3_NickNameID"));
			pLabCtrl->SetText("");

			CEditUI* pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn3_TargetUser"));
			pEdtCtrl->SetText("");
			pEdtCtrl->SetFocus();
			return;
		}
	}
	else if (msg.sType == _T("textchanged"))
	{
		if (msg.pSender->GetName() == _T("Edt_S1_Ctn3_TransCoint"))
		{
			//ת�˽��Сдת��Ϊ���Ĵ�д
			ChangeEtToChinese(static_cast<CEditUI*>(msg.pSender),
				static_cast<CTextUI*>(m_pm.FindControl("Txt_S1_Ctn3_Caps")));
			return;
		}
	}
	else if(msg.sType == _T("setfocus"))
	{
		if (msg.pSender->GetName() == _T("Edt_S1_Ctn3_TargetUser"))
		{
			//����ת��Ŀ���û�������һЩ���
			m_strFocusTemp = msg.pSender->GetText();
			return;
		}
	}
	else if(msg.sType == _T("killfocus"))
	{
		if (msg.pSender->GetName() == _T("Edt_S1_Ctn3_TargetUser"))
		{
			//Ŀ���û��༭��û���������ݻ�Ϊ�ն�������OnIDKillFocus

			CLabelUI* pLabCtrl = NULL;

			if (msg.pSender->GetText().GetLength() == 0)
			{
				pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn3_NickNameID_temp"));
				pLabCtrl->SetText("");

				pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn3_NickNameID"));
				pLabCtrl->SetText("");
			}
			else
			{
				//����ת��Ŀ���û�������һЩ���
				if (msg.pSender->GetText().Compare(m_strFocusTemp))
				{
					pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn3_NickNameID"));
					pLabCtrl->SetText("���ڲ�ѯ...");

					COptionUI* pOptCtrl = static_cast<COptionUI*>(m_pm.FindControl("Opt_S1_Ctn3_ID"));
					if(!OnIDKillFocus(static_cast<CEditUI*>(msg.pSender)
							,(pOptCtrl->IsSelected()?NICK_ID_BYID:NICK_ID_BYNICKNAME)
							,NICK_ID_USE_TO_TRANSFER))
					{
						pLabCtrl->SetText("(��Ҳ�����)");
					}
				}

				m_strFocusTemp = "";
			}
			
			return;
		}
	}
}

//������1���еġ�����4����Ϣ������
void CPBankWndEx::Notify_1_4(BzDui::TNotifyUI& msg)
{
	if (msg.sType == _T("on_contain_open"))
	{
		if(m_bTransferSuccess)
		{
			m_mapRecord.clear();
			m_iRecDispPos = 0;
			m_pRoom->PostMessage(WM_TRANSFER_RECORD,0,0);

			m_bTransferSuccess = false;

			//��ҳ�ؼ���ť
			CButtonUI* pBntCtrl = NULL;
			pBntCtrl = static_cast<CButtonUI*>(m_pm.FindControl("Bnt_S1_Ctn4_First"));
			pBntCtrl->SetEnabled(false);
			pBntCtrl = static_cast<CButtonUI*>(m_pm.FindControl("Bnt_S1_Ctn4_Previous"));
			pBntCtrl->SetEnabled(false);
			pBntCtrl = static_cast<CButtonUI*>(m_pm.FindControl("Bnt_S1_Ctn4_Next"));
			pBntCtrl->SetEnabled(false);
			pBntCtrl = static_cast<CButtonUI*>(m_pm.FindControl("Bnt_S1_Ctn4_Rear"));
			pBntCtrl->SetEnabled(false);
		}
	}
	else if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("Btn_S1_Ctn4_Refresh"))
		{
			//ˢ��ת�˼�¼
			m_bTransferSuccess = TRUE;

			TNotifyUI tagUIMsg;
			tagUIMsg.sType = _T("on_contain_open");
			Notify_1_4(tagUIMsg);

			return;
		}
		else if (msg.pSender->GetName() == _T("Bnt_S1_Ctn4_First"))
		{
			//��¼������ҳ
			m_iRecDispPos = 0;
			m_pRecordList->NeedUpdate();
			UpdateRecordPageState();
			return;
		}
		else if (msg.pSender->GetName() == _T("Bnt_S1_Ctn4_Previous"))
		{
			//��¼����һҳ
			if (m_iRecDispPos - RECORD_PAGE_SIZE >= 0)
			{
				m_iRecDispPos -= RECORD_PAGE_SIZE;
				m_pRecordList->NeedUpdate();
				UpdateRecordPageState();
			}
			return;
		}
		else if (msg.pSender->GetName() == _T("Bnt_S1_Ctn4_Next"))
		{
			//��¼����һҳ
			if (m_iRecDispPos + RECORD_PAGE_SIZE < m_mapRecord.size())
			{
				m_iRecDispPos += RECORD_PAGE_SIZE;
				m_pRecordList->NeedUpdate();
				UpdateRecordPageState();
			}
			return;
		}
		else if (msg.pSender->GetName() == _T("Bnt_S1_Ctn4_Rear"))
		{
			//��¼����βҳ
			if (m_mapRecord.size() % RECORD_PAGE_SIZE == 0)
			{
				m_iRecDispPos = m_mapRecord.size() - RECORD_PAGE_SIZE;
			}
			else
			{
				m_iRecDispPos = m_mapRecord.size() - m_mapRecord.size() % RECORD_PAGE_SIZE;
			}

			m_pRecordList->NeedUpdate();
			UpdateRecordPageState();
			return;
		}
	}

	return;
}

//������1���еġ�����5����Ϣ������
void CPBankWndEx::Notify_1_5(BzDui::TNotifyUI& msg)
{
	if (msg.sType == "click")
	{
		if (msg.pSender->GetName() == _T("Btn_OK")) //�ύ
		{
			CEditUI* pEdtOutCoint = NULL;
			pEdtOutCoint = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn5_OutCoint"));

			CEditUI* pEdtPwd = NULL;
			pEdtPwd = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn5_Pwd"));

			/// ȡ�ñ༭������ַ���
			TCHAR szMsg[256];
			CString strNum, str;
			strNum = pEdtOutCoint->GetText();

			//����зָ�������ȥ���ַ�����ķָ���
			if(Glb().m_bUseSpace)
				strNum.Replace(Glb().m_strSpaceChar,"");

			TCHAR szTail[16];
			_tcscpy(szTail, TEXT(""));
			for (int i=0; i<Glb().m_nPowerOfGold; ++i)
			{
				_tcscat( szTail, TEXT("0"));
			}
			/// ���û������
			if (strNum.GetLength() == 0)
			{
				ShowTipMessage(NULL,"PBankWnd2","CreateDep_Msg_NotInputCoin","��������Ҫ��ȡ������");
				pEdtOutCoint->SetFocus();
				return;
			}
			/// �������̫��
			if (strNum.GetLength()<Glb().m_nPowerOfGold)
			{
				ShowTipMessage(NULL,"PBankWnd2","CreateDep_Msg_NotGetCoin",
					"������Ľ�������ϵͳ���ܣ�����������%dλ����ĩ%dλΪ %s ����������",
					Glb().m_nPowerOfGold+1,Glb().m_nPowerOfGold,szTail);
				pEdtOutCoint->SetFocus();
				return;
			}
			/// �������̫��
			if (strNum.GetLength()>18+Glb().m_nPowerOfGold)
			{
				ShowTipMessage(NULL,"PBankWnd2","CreateDep_Msg_NumberTooLargen",
					"�����������̫�󣬳�������Χ�������벻����%I64d%s����ĩ%dλΪ %s ����������",
					0x3FFFFFFFFFFFFFFF,szTail,Glb().m_nPowerOfGold,szTail);
				pEdtOutCoint->SetFocus();
				return;
			}

			/// ���ĩ��λ������
			CString strSubNum = strNum.Right(Glb().m_nPowerOfGold);
			if (strSubNum != szTail)
			{
				ShowTipMessage(NULL,"PBankWnd2","CreateDep_Msg_NotGetCoin2",
					"������Ľ�������ϵͳ���ܣ�������ĩ%dλΪ %s ����������",
					Glb().m_nPowerOfGold,szTail);
				pEdtOutCoint->SetFocus();
				return;
			}
			/// ��ȡǰNλ
			strSubNum = strNum.Left(strNum.GetLength()-Glb().m_nPowerOfGold);
			__int64 i64Num = _atoi64(strSubNum.GetBuffer());
			if (i64Num>0x3FFFFFFFFFFFFFFF)
			{
				ShowTipMessage(NULL,"PBankWnd2","CreateDep_Msg_NumberTooLargen",
					"�����������̫�󣬳�������Χ�������벻����%I64d%s����ĩ%dλΪ %s ����������",
					0x3FFFFFFFFFFFFFFF,szTail,Glb().m_nPowerOfGold,szTail);
				pEdtOutCoint->SetFocus();
				return;
			}

			if (i64Num > m_i64BankMoney)
			{
				ShowTipMessage(NULL,"","","������������Ѵ��ڳ������еĴ��");
				pEdtOutCoint->SetFocus();
				return;
			}

			//��������
			str = pEdtPwd->GetText();

			if (str.GetLength() < 1)
			{
				ShowTipMessage(NULL,"","","���������룡");
				pEdtPwd->SetFocus();
				return;
			}
			if (i64Num<=0)
			{
				ShowTipMessage(NULL,"PBankWnd2","CreateDep_Msg_NotGetCoin",
					"������Ľ�������ϵͳ���ܣ�����������%dλ����ĩ%dλΪ %s ����������",
					Glb().m_nPowerOfGold + 1,Glb().m_nPowerOfGold,szTail);
				pEdtOutCoint->SetFocus();
				return;
			}

			/// ����Ϣ
			if (m_pRoom != NULL)
			{
				bank_op_deposit* pDeposit = new bank_op_deposit();
				pDeposit->_user_id = m_dwOpUserID;
				pDeposit->_operate_type = 1;
				pDeposit->_money = i64Num;
				//��ȡ���������
				CGameRoomEx::GetCryptedPasswd(pDeposit->_szMD5Pass,(TCHAR*)pEdtPwd->GetText().GetData());

				m_pRoom->PostMessage(WM_BANK,1,LPARAM(pDeposit));

				m_bDepositSuccess = true;
			}

			return;
		}
		else if (msg.pSender->GetName() == _T("Bnt_S1_Ctn5_Suc_Return"))
		{
			//�л���ʾ����,��ʾ�����浥��UI����
			m_pSelFunNode->pFunsCtn->SetVisible(false);
			CContainerUI* pCtnCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Ctn_Type1_Ctn5"));
			m_pSelFunNode->pFunsCtn = pCtnCtrl;
			m_pSelFunNode->bNeedSubmit = true;
			m_pSelFunNode->pFunsCtn = pCtnCtrl;
			pCtnCtrl->SetVisible(true);

			//�����ύ��ť��ʹ��
			m_pSubmitBnt->SetVisible(true);

			return;
		}
	}
	else if (msg.sType == _T("textchanged"))
	{
		if (msg.pSender->GetName() == _T("Edt_S1_Ctn5_OutCoint"))
		{
			//ת�˽��Сдת��Ϊ���Ĵ�д
			ChangeEtToChinese(static_cast<CEditUI*>(msg.pSender),
				static_cast<CTextUI*>(m_pm.FindControl("Txt_S1_Ctn5_Caps")));
			return;
		}
	}

	return;
}

//������1���еġ�����6����Ϣ������
void CPBankWndEx::Notify_1_6(BzDui::TNotifyUI& msg)
{
	if (msg.sType == _T("on_contain_open"))
	{
		if(m_bDepositSuccess)
		{
			m_mapDeposits.clear();
			m_pDepList->RemoveAll();
			m_bDepositSuccess = false;
			m_pRoom->PostMessage(WM_BANK,3,m_dwOpUserID);
		}
	}
	else if (msg.sType == _T("itemselect"))
	{
		if (msg.pSender->GetName() == _T("Lst_S1_Ctn6_Record"))
		{
			if (msg.wParam < 0)
			{
				return;
			}

			CLabelUI* pLabCtrl = NULL;
			CEditUI* pEditUI = NULL;

			char szBuffer[128];

			bank_op_deposit_node* pDepNode = &m_mapDeposits[msg.wParam];

			pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn6_Title"));
			pLabCtrl->SetText("�浥");

			pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn6_Creater"));
			_stprintf(szBuffer,"%d",pDepNode->_user_id);
			pLabCtrl->SetText(szBuffer);

// 			pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn6_Time"));
// 			_stprintf(szBuffer,_T("%02d-%02d-%02d %02d:%02d:%02d"),
// 				pDepNode->_create_time.GetYear(),
// 				pDepNode->_create_time.GetMonth(),
// 				pDepNode->_create_time.GetDay(),
// 				pDepNode->_create_time.GetHour(),
// 				pDepNode->_create_time.GetMinute(),
// 				pDepNode->_create_time.GetSecond()
// 				);
// 			pLabCtrl->SetText(szBuffer);

			pEditUI = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn6_Serial"));
			pEditUI->SetText(pDepNode->_deposit_id);

			pEditUI = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn6_Pwd"));
			pEditUI->SetText(pDepNode->_deposit_password);

			GetNumString(szBuffer,pDepNode->_money,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);
			pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn6_Money"));
			pLabCtrl->SetText(szBuffer);


			pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn6_State"));
			if (pDepNode->_state == 0)
			{
				pLabCtrl->SetText("δʹ��");
			}
			else if (pDepNode->_state == 1)
			{
				pLabCtrl->SetText("��ʹ��");
			}

			return;
		}
	}

	return;
}


//������1���еġ�����7����Ϣ������
void CPBankWndEx::Notify_1_7(BzDui::TNotifyUI& msg)
{
	if (msg.sType == "on_contain_open")
	{
// 		m_strTransTargetID = "0";
// 		m_strTransTargetNickName = " ";
	}
	else if (msg.sType == "click")
	{
		if (msg.pSender->GetName() == _T("Btn_OK")) //�ύ
		{
			CEditUI* pTargetUserEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn7_TargetUser"));
			CEditUI* pSerialNoEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn7_Serial"));
			CEditUI* pPasswordEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn7_Pwd"));

			CStdString strTargetUser = pTargetUserEdt->GetText();
			CStdString strSerial = pSerialNoEdt->GetText();
			CStdString strPassword = pPasswordEdt->GetText();

			if (strTargetUser.GetLength() == 0)
			{
				ShowTipMessage(NULL,"","","������Ҫ��ֵ���û���ID");
				pTargetUserEdt->SetFocus();
				return;
			}

			if (strSerial.GetLength() == 0)
			{
				ShowTipMessage(NULL,"","","�������ֵ�����к�");
				pSerialNoEdt->SetFocus();
				return;
			}
			
			if (strPassword.GetLength() == 0)
			{
				ShowTipMessage(NULL,"","","�������ֵ������");
				pPasswordEdt->SetFocus();
				return;
			}

			if (0 == atol(strTargetUser))
			{
				ShowTipMessage(NULL,"","","��������ȷ�����ID��");
				pTargetUserEdt->SetFocus();
				return;
			}



			/// ȡ�ñ༭������ַ���
			TCHAR szMsg[256];

			/// ��ʾ�û�ȷ��ת��
			CBcfFile fMsg(CBcfFile::GetAppPath() + "ClientMessage.bcf");

			CLabelUI* pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn7_NickNameID_temp"));

			CString strFormat;
			strFormat = fMsg.GetKeyVal("PBankWnd2", "UseDep_Msg_Confirm","����Ҫ��ID��Ϊ[%d]��ң��ǳ�[%s]���г�ֵ���Ƿ�ȷ��ת�ʣ�");
			wsprintf(szMsg,strFormat,atol(strTargetUser),pLabCtrl->GetText().GetData());

			if (IDYES != DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONQUESTION,"����",true,szMsg))
			{
				return;
			}

			bank_op_deposit* pDeposit = new bank_op_deposit();
			pDeposit->_user_id = (int)atol(strTargetUser);
			pDeposit->_operate_type = 2;
			strcpy_s(pDeposit->_deposit_id,sizeof(pDeposit->_deposit_id),strSerial.GetData());
			strcpy_s(pDeposit->_deposit_password,sizeof(pDeposit->_deposit_password),strPassword.GetData());

			/// ����Ϣ
			if (m_pRoom != NULL)
			{
				m_pRoom->PostMessage(WM_BANK,1,LPARAM(pDeposit));
			}
 
			return;
		}
	}
	else if(msg.sType == _T("setfocus"))
	{
		if (msg.pSender->GetName() == _T("Edt_S1_Ctn7_TargetUser"))
		{
			//����ת��Ŀ���û�������һЩ���
			m_strFocusTemp = msg.pSender->GetText();
			return;
		}
	}
	else if(msg.sType == _T("killfocus"))
	{
		if (msg.pSender->GetName() == _T("Edt_S1_Ctn7_TargetUser"))
		{
			//Ŀ���û��༭��û���������ݻ�Ϊ�ն�������OnIDKillFocus

			CLabelUI* pLabCtrl = NULL;

			if (msg.pSender->GetText().GetLength() == 0)
			{
				pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn7_NickNameID_temp"));
				pLabCtrl->SetText("");

				pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn7_NickNameID"));
				pLabCtrl->SetText("");
			}
			else
			{
				if (msg.pSender->GetText().Compare(m_strFocusTemp))
				{
					pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn7_NickNameID"));
					pLabCtrl->SetText("���ڲ�ѯ...");

					//����ת��Ŀ���û�������һЩ���
					if(!OnIDKillFocus(static_cast<CEditUI*>(msg.pSender),NICK_ID_BYID,NICK_ID_USE_TO_DEPOSIT))
					{
						pLabCtrl->SetText("(��Ҳ�����)");
					}
					m_strFocusTemp = "";
				}
			}
			
			return;
		}
	}

	return;
}

//������2���еġ�����1����Ϣ������
void CPBankWndEx::Notify_2_1(BzDui::TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("Btn_OK")) //�ύ
		{

			CEditUI* pOldPwdEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_S2_Ctn1_OldPwd"));
			CEditUI* pNewPwdEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_S2_Ctn1_NewPwd"));
			CEditUI* pPwdCfmEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_S2_Ctn1_PwdCfm"));

			CString strOld, strNew, strNewCfm, str;
			TCHAR szMsg[MAX_PATH];

			strOld = pOldPwdEdt->GetText();//������

			if ((strOld.GetLength() < 6) ||(strOld.GetLength() > 20))
			{
				ShowTipMessage(NULL,"PBankWnd2","ChPwd_Msg_OldBytes","������6-20�ַ���ɣ���������������룡");
				pOldPwdEdt->SetFocus();
				return;
			}

			/// �ȱȽ��������ȷ�������Ƿ���ͬ�������ͬ����ֱ�ӷ��أ���ͬ�ż���
			strNew = pNewPwdEdt->GetText();//������
			strNewCfm = pPwdCfmEdt->GetText();//ȷ��������
			if (strNew != strNewCfm)
			{
				ShowTipMessage(NULL,"PBankWnd2","ChPwd_Msg_NotSame","�������������벻һ�£�����������!");
				pNewPwdEdt->SetFocus();
				return;
			}
			if ((strNew.GetLength() < 6) ||(strNew.GetLength() > 20))
			{
				ShowTipMessage(NULL,"PBankWnd2","ChPwd_Msg_NewBytes","�������������벻һ�£�����������!");
				pNewPwdEdt->SetFocus();
				return;
			}

			TMSG_GP_BankChPwd* pChPwd = new TMSG_GP_BankChPwd();
			pChPwd->_user_id = m_dwOpUserID;

			CGameRoomEx::GetCryptedPasswd(pChPwd->_MD5Pass_old,(char*)strOld.Left(20).GetBuffer());
			CGameRoomEx::GetCryptedPasswd(pChPwd->_MD5Pass_new,(char*)strNew.Left(20).GetBuffer());

			GetMainRoom()->PostMessage(WM_CHANGE_PASSWD,0,LPARAM(pChPwd));

			pOldPwdEdt->SetText("");//������
			pNewPwdEdt->SetText("");//������
			pPwdCfmEdt->SetText("");//ȷ��������

			return;
		}
	}
	return;
}

//������3���еġ�����1����Ϣ������
void CPBankWndEx::Notify_3_1(BzDui::TNotifyUI& msg)
{
	if (msg.sType == _T("on_contain_open"))
	{
		COptionUI* pOptCtrl = NULL;
		CComboUI* pCobCtrl = NULL;

		int nSelect = AfxGetApp()->GetProfileInt(TEXT("BankSafe"),TEXT("Option"),1);
		CString strMin = AfxGetApp()->GetProfileString(TEXT("BankSafe"),TEXT("Minutes"),_TEXT("5"));

		pOptCtrl = static_cast<COptionUI*>(m_pm.FindControl("Opt_S3_Ctn1_2"));

		if (pOptCtrl->IsSelected())
		{
			pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl(_T("Cob_S3_Ctn1_Minutes")));
			SelectComboItem(pCobCtrl,strMin);
		}

		return;
	}
	else if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("Btn_OK"))
		{
			COptionUI* pOptCtrl = NULL;

			pOptCtrl = static_cast<COptionUI*>(m_pm.FindControl("Opt_S3_Ctn1_1"));
			if (pOptCtrl->IsSelected())
			{
				AfxGetApp()->WriteProfileInt(TEXT("BankSafe"),TEXT("Option"),1);
				m_bCheckFrequence = false;
			}

			pOptCtrl = static_cast<COptionUI*>(m_pm.FindControl("Opt_S3_Ctn1_2"));
			if (pOptCtrl->IsSelected())
			{
				CComboUI* pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl(_T("Cob_S3_Ctn1_Minutes")));
				AfxGetApp()->WriteProfileString(TEXT("BankSafe"),TEXT("Minutes"),pCobCtrl->GetText());
				AfxGetApp()->WriteProfileInt(TEXT("BankSafe"),TEXT("Option"),2);
				m_iMinutes = _atoi64(pCobCtrl->GetText());
				m_bCheckFrequence = true;
			}

			pOptCtrl = static_cast<COptionUI*>(m_pm.FindControl("Opt_S3_Ctn1_3"));
			if (pOptCtrl->IsSelected())
			{
				AfxGetApp()->WriteProfileInt(TEXT("BankSafe"),TEXT("Option"),3);
				m_bCheckFrequence = false;
			}

			ShowTipMessage(NULL,"","","���óɹ���");
			
		}
	}
	else if(msg.sType == _T("selectchanged"))
	{
		if (msg.pSender->GetName() == _T("Opt_S3_Ctn1_1"))
		{
			CComboUI* pCobCtrl = NULL;
			pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl("Cob_S3_Ctn1_Minutes"));
			pCobCtrl->SetEnabled(false);
		}
		else if (msg.pSender->GetName() == _T("Opt_S3_Ctn1_2"))
		{
			CComboUI* pCobCtrl = NULL;
			pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl("Cob_S3_Ctn1_Minutes"));
			pCobCtrl->SetEnabled(true);
			if (pCobCtrl->GetCurSel() == -1)
			{
				pCobCtrl->SelectItem(0);
			}
		}
		else if (msg.pSender->GetName() == _T("Opt_S3_Ctn1_3"))
		{
			CComboUI* pCobCtrl = NULL;
			pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl("Cob_S3_Ctn1_Minutes"));
			pCobCtrl->SetEnabled(false);
		}
	}

	return;
}

bool CPBankWndEx::OnNetMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	if (pNetHead->bMainID != MDM_GP_BANK && pNetHead->bMainID != MDM_BANK) return true;

	switch (pNetHead->bAssistantID)
	{
	case ASS_BANK_NORMAL:
	case ASS_GP_BANK_CHECK:
		{
			bank_op_normal *p = (bank_op_normal *)pNetData;
			if (p != NULL)
			{
				if (p->_operate_type == 1) //ȡǮ
				{
					if (pNetHead->bHandleCode == HC_BANK_OP_SUC)
					{
						TCHAR szNum[128];
						GetNumString(szNum,p->_money,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);

						ShowTipMessage(NULL,"PBankWnd2","OutSv_Msg_Success",
										"<format><p 5><y 25>���ɹ�ȡ�� <b>%s</b> ��ҵ� <b>��%s��</b> ��Ϸ��Ǯ���С�</y></p>",
										szNum,GetGameNameById(p->_game_id));

						CEditUI* pPwdEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn1_Pwd"));
						pPwdEdt->SetText("");

						//�������ݼ���ʾ����
						m_i64BankMoney -= p->_money;
						AddGameWalletMoney(p->_game_id,p->_money);
						UpdateWealthInfor();

						//���´����Ľ����ʾ
// 						CGameRoomEx* pGameRoomEx = (CGameRoomEx*)m_pGameRoom;
// 						if (p->_game_id == pGameRoomEx->m_GameInfo.uNameID)
// 						{
// 							pGameRoomEx->FlushUserMoney(m_dwOpUserID,p->_money);
// 						}

						return true;

					}
					else if (pNetHead->bHandleCode == HC_BANK_OP_ERR_PWD)
					{
						DUIOkeyMsgBox(m_hWnd,false,"�������벻��ȷ");
						CEditUI* pPwdEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn1_Pwd"));
						pPwdEdt->SetFocus();

						return true;
					}
				}
				else //��Ǯ
				{
					if (pNetHead->bHandleCode == HC_BANK_OP_SUC)
					{
						TCHAR szNum[128];
						GetNumString(szNum,p->_money,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);

						ShowTipMessage(NULL,"PBankWnd2","InSv_Msg_Success",
							"<format><p 5><y 25>���ɹ��Ѵ�<b>��%s��</b>��ϷǮ��ȡ��<n><b>%s</b> ��Ǯ���С�</y></p>",
							GetGameNameById(p->_game_id),szNum);

						CEditUI* pPwdEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn2_OutCoint"));
						pPwdEdt->SetFocus();
						pPwdEdt->SetText("");

						//�������ݼ���ʾ����
						m_i64BankMoney += p->_money;
						AddGameWalletMoney(p->_game_id,-(p->_money));
						UpdateWealthInfor();

						//���´����Ľ����ʾ
// 						CGameRoomEx* pGameRoomEx = (CGameRoomEx*)m_pGameRoom;
// 						if (p->_game_id == pGameRoomEx->m_GameInfo.uNameID)
// 						{
// 							pGameRoomEx->FlushUserMoney(m_dwOpUserID,-(p->_money));
// 						}
					}

					return true;
				}
			}
			else
			{
				if (pNetHead->bHandleCode == HC_BANK_OP_CHECKOUT_PLAYING) 
				{
					ShowTipMessage(NULL,"GameRoom","NoDrawForPlaying","��������Ϸ�У�����ȡ�");
					return true;
				}
				else if (pNetHead->bHandleCode == HC_BANK_OP_CHECKIN_PLAYING) 
				{
					ShowTipMessage(NULL,"GameRoom","NoSaveForPlaying","��������Ϸ�У����ܴ�");
					return true;
				}
				else if (pNetHead->bHandleCode == HC_BANK_OP_CHECKIN_ZHUANG) 
				{
					ShowTipMessage(NULL,"GameRoom","NoSaveForZhuangjia","��������ׯ�У����ܴ�");
					return true;
				}
			}

			break;
		}
	case ASS_BANK_DEPOSIT:
	case ASS_GP_BANK_DEPOSIT:
		{
			bank_op_deposit *p = (bank_op_deposit*)pNetData;
			if (p != NULL)
			{
				if (p->_operate_type == 1) //���ɴ浥
				{
					if (pNetHead->bHandleCode == HC_BANK_OP_SUC)
					{
						//�浥�����ɹ����л���ʾ����,��ʾ��ϲ��Ϣ
						m_pSelFunNode->pFunsCtn->SetVisible(false);
						CContainerUI* pCtnCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Ctn_Type1_Ctn5_Suc"));
						m_pSelFunNode->pFunsCtn = pCtnCtrl;
						m_pSelFunNode->bNeedSubmit = false;
						m_pSelFunNode->pFunsCtn = pCtnCtrl;
						//�����ύ��ť��ʹ��
						m_pSubmitBnt->SetVisible(false);

						CEditUI* pEditCtrl = NULL;
						CLabelUI* pLabCtrl = NULL;

						TCHAR szBuffer[128];

						//��ʾ�浥�˺�
						pEditCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn5_Suc_Serial"));
						pEditCtrl->SetText(p->_deposit_id);

						//��ʾ�浥����
						pEditCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn5_Suc_Pwd"));
						pEditCtrl->SetText(p->_deposit_password);

						//��ʾ�浥��ֵ
						GetNumString(szBuffer,p->_money,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);
						pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn5_Suc_Money"));
						pLabCtrl->SetText(szBuffer);

						//��ʾ�����浥��������
						__int64 i64Tax = __int64(p->_money * (1.0 * p->_csds/100));
						GetNumString(szBuffer,i64Tax,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);
						pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_S1_Ctn5_Suc_Tax"));
						pLabCtrl->SetText(szBuffer);

						pCtnCtrl->SetVisible(true);

						//���±��ؽ����ʾ
						m_i64BankMoney -= (p->_money + i64Tax);
						UpdateWealthInfor();

					}
					else if (pNetHead->bHandleCode == HC_BANK_OP_ERR_PWD)
					{
						DUIOkeyMsgBox(m_hWnd,false,"�������벻��ȷ");
						CEditUI* pPwdEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_S1_Ctn5_Pwd"));
						pPwdEdt->SetFocus();
					}
					else if (pNetHead->bHandleCode == HC_BANK_OP_NOT_MONEY)
					{

						DUIOkeyMsgBox(m_hWnd,false,"�������㣬���ܲ����浥");
					}
					else if (pNetHead->bHandleCode == HC_BANK_OP_NO_SERIAL)
					{
						DUIOkeyMsgBox(m_hWnd,false,"�浥������Դ�ľ�������ͷ���ϵ");
					}
					return true;
				}
				else if (p->_operate_type == 2) //ʹ�ô浥������ֵ
				{
					if (pNetHead->bHandleCode == HC_BANK_OP_SUC)
					{
						TCHAR szNum[128];
						GetNumString(szNum,p->_money, Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);
						ShowTipMessage(NULL,"PBankWnd2","UseDep_Msg_Success",
											"��ת��ID[%d]���ǳ�[%s]����ҳ�ֵ %s ���",
											p->_user_id,p->_szNickName,szNum);

						//���½��
						if (m_dwOpUserID == p->_user_id)
						{
							m_i64BankMoney += p->_money;
							UpdateWealthInfor();
						}
					}
					else if (pNetHead->bHandleCode == HC_BANK_OP_NO_USER)
					{
						ShowTipMessage(NULL,"PBankWnd2","UseDep_Msg_NoUser",
											"δ�ܳ�ֵ�����û������ڡ�");
					}
					else if (pNetHead->bHandleCode == HC_BANK_OP_ERR_PWD)
					{
						ShowTipMessage(NULL,"PBankWnd2","UseDep_Msg_ErrPassword",
											"δ�ܳ�ֵ����ֵ�����벻��ȷ��");
					}
					else if (pNetHead->bHandleCode == HC_BANK_OP_INVDEP)
					{
						ShowTipMessage(NULL,"PBankWnd2","UseDep_Msg_Used",
											"δ�ܳ�ֵ���ó�ֵ���ѱ�ʹ�á�");
					}

					return true;
				}
			}
			break;
		}
	case ASS_BANK_WALLET:
	case ASS_GP_BANK_GETGM_WALLET:
		{
			bank_game_wallet* pWallet = (bank_game_wallet*)pNetData;
			if (pWallet != NULL)
			{
				if (pNetHead->bHandleCode == 0)
				{
					vector<TGmWalletNode>::iterator it = m_vecGmWallets.begin();
					while(it != m_vecGmWallets.end())
					{
						if (it->uNameID == pWallet->_game_id)
						{
							it->i64Wallet = pWallet->_money; //������ϷǮ��
							return true;
						}
						it++;
					}

					//����Ϸ�б���û���ҵ������һ���Ҫ������ӵ���Ϸѡ��������б���
					TGmWalletNode tagNode;
					tagNode.uNameID = pWallet->_game_id;
					strcpy(tagNode.szGameName,pWallet->_game_name);
					tagNode.i64Wallet = pWallet->_money;
					m_vecGmWallets.push_back(tagNode);

					CComboUI* pCobCtrl = NULL;
					pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl("Cob_S1_Ctn1_Games"));
					CListLabelElementUI* p = new CListLabelElementUI();
					p->SetText(it->szGameName);
					pCobCtrl->Add(p);

				}
				else //��ϷǮ�����ݸ������
				{
					CComboUI* pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl("Cob_S1_Ctn1_Games"));

					vector<TGmWalletNode>::iterator it = m_vecGmWallets.begin();
					UINT uInNameID = 0;
					int iPos = 0;
					
					if (CPBankWndEx::ISGameRoom == m_emParWndType)
					{
						uInNameID = ((CGameRoomEx*)m_pRoom)->m_GameInfo.uNameID;
						while(it != m_vecGmWallets.end())
						{
							if (uInNameID == it->uNameID)
							{
								break;
							}
							iPos++;
							it++;
						}
					}
					
					pCobCtrl->SelectItem(iPos);
					pCobCtrl->SetEnabled(true);

					pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl("Cob_S1_Ctn2_Game"));
					it = m_vecGmWallets.begin();
					while(it != m_vecGmWallets.end())
					{
						if (it->i64Wallet > 0) //ֻ��ʾ��Ǯ����Ϸ
						{
							CListLabelElementUI* p = new CListLabelElementUI();
							p->SetText(it->szGameName);
							pCobCtrl->Add(p);
						}
						it++;
					}
					pCobCtrl->SetEnabled(true);

					UpdateWealthInfor();

				}
			}
			break;
		}
	case ASS_BANK_QUERY_DEPOITS:
	case ASS_GP_BANK_QUERY_DEPOITS:
		{
			bank_op_deposit_node* pDepNode = (bank_op_deposit_node*)pNetData;
			if (pDepNode != NULL)
			{
				if (pNetHead->bHandleCode == 0)
				{
					m_mapDeposits[pDepNode->_serial_no] = *pDepNode;

					CListContainerElementUI* pCtnCtrl = new CListContainerElementUI();
					pCtnCtrl->SetFixedHeight(55);

					CLabelUI* pLabCtrl = new CLabelUI();
					SIZE s = {5,3};
					pLabCtrl->SetFixedXY(s);
					pLabCtrl->SetFixedWidth(120);
					pLabCtrl->SetFixedHeight(15);
					pLabCtrl->SetMouseEnabled(false);
					pLabCtrl->SetFloat();

					char szBuffer[128];
					_stprintf(szBuffer,_T("%02d-%02d-%02d %02d:%02d:%02d"),
						pDepNode->_create_time.GetYear(),
						pDepNode->_create_time.GetMonth(),
						pDepNode->_create_time.GetDay(),
						pDepNode->_create_time.GetHour(),
						pDepNode->_create_time.GetMinute(),
						pDepNode->_create_time.GetSecond()
						);

					pLabCtrl->SetText(szBuffer);

					pCtnCtrl->Add(pLabCtrl);

					pLabCtrl = new CLabelUI();
					s.cx = 65;
					s.cy = 25;
					pLabCtrl->SetFixedXY(s);
					pLabCtrl->SetFixedWidth(60);
					pLabCtrl->SetFixedHeight(15);
					pLabCtrl->SetShowHtml(true);
					pLabCtrl->SetFloat();
					pLabCtrl->SetText("<c #00008080>�浥��Ϣ</c>");
					pLabCtrl->SetMouseEnabled(false);
					pCtnCtrl->Add(pLabCtrl);


					CControlUI* pCtrl = new CControlUI();
					s.cx = 20;
					s.cy = 18;
					pCtrl->SetBkImage("BankWnd2\\item_ico.png");
					pCtrl->SetFixedXY(s);
					pCtrl->SetFixedWidth(32);
					pCtrl->SetFixedHeight(31);
					pCtrl->SetFloat();
					pCtrl->SetMouseEnabled(false);
					pCtnCtrl->Add(pCtrl);

					m_pDepList->Add(pCtnCtrl);
			
					
				}
				else if (pNetHead->bHandleCode == 1)
				{
					//����Deposits��ɣ�
					m_pDepList->NeedUpdate();

					bool bCtrlVisible = true;

					if (m_mapDeposits.size() == 0)
					{
						bCtrlVisible = false;
					}

					int idx = 0;
					CStdPtrArray* pCtrlArray = NULL;
					CControlUI* pCtrl = NULL;

					CControlUI* pRoot = m_pm.FindControl("Ctn_Type1_Ctn6");

					pCtrlArray = m_pm.FindSubControlsByClass(pRoot,_TEXT("LabelUI"));
					do 
					{
						pCtrl = static_cast<CButtonUI*>(pCtrlArray->GetAt(idx++));
						if (pCtrl)
						{
							pCtrl->SetVisible(bCtrlVisible);
						}
					} while (pCtrl);

					idx = 0;
					pCtrlArray = m_pm.FindSubControlsByClass(pRoot,_TEXT("EditUI"));
					do 
					{
						pCtrl = static_cast<CButtonUI*>(pCtrlArray->GetAt(idx++));
						if (pCtrl)
						{
							pCtrl->SetVisible(bCtrlVisible);
						}
					} while (pCtrl);

					idx = 0;
					pCtrlArray = m_pm.FindSubControlsByClass(pRoot,_TEXT("ListUI"));
					do 
					{
						pCtrl = static_cast<CButtonUI*>(pCtrlArray->GetAt(idx++));
						if (pCtrl)
						{
							pCtrl->SetVisible(bCtrlVisible);
						}
					} while (pCtrl);

					pCtrl = m_pm.FindControl("Lab_S1_Ctn6_9");
					pCtrl->SetVisible(!bCtrlVisible);
				}
			}
			break;
		}

	case ASS_GP_BANK_CHPWD:	//< �޸�����
		{
			if (pNetHead->bHandleCode == HC_GP_BANK_CHPWD_SUC)
			{
				ShowTipMessage(NULL,"GameRoom","ChangePasswdOK","�޸���������ɹ��������Ʊ��������롣");
			}
			else if (pNetHead->bHandleCode == HC_GP_BANK_CHPWD_ERR_PWD)
			{
				ShowTipMessage(NULL,"GameRoom","ChangePasswdFailed","�޸���������ʧ�ܣ������в���ȷ��");
			}
			break;
		}
	}

	return true;
}
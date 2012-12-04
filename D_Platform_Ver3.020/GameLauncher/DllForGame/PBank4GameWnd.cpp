#include "StdAfx.h"
#include "PBank4GameWnd.h"
#include "Bank4ZMessage.h"

using namespace BzDui;

#define IDT_SET_FOCUS	1

CPBank4GameWnd::CPBank4GameWnd(CWnd* pParent,GameInfoStructInExe* pGameInfo)
{
	m_pRoom = pParent;

	//���а汾
	m_nBankVer = 1;

	//������Ĵ��
	m_i64BankMoney = 0;

	//��Ϸ��Ǯ����Ǯ
	m_i64GameWallet = 0;

	//��Ϸ��Ϣ
	m_pGameInfo = pGameInfo;

	//������ñ༭��
	m_pCointEdt = NULL;

}

CPBank4GameWnd::~CPBank4GameWnd(void)
{
}

LPCTSTR CPBank4GameWnd::GetWindowClassName() const 
{ 
	return _T("UIFrame");
};

UINT CPBank4GameWnd::GetClassStyle() const 
{ 
	return UI_CLASSSTYLE_DIALOG; 
};

void CPBank4GameWnd::OnFinalMessage(HWND hWnd) 
{ 
	m_pm.RemovePreMessageFilter(this);
	//delete this;
};

LRESULT CPBank4GameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	m_pm.AddPreMessageFilter(this);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("BankWnd2\\PBank4GameWnd.xml"), (UINT)0, NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	Init();

	::SetWindowPos(m_hWnd,CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);

	return 0;
}

LRESULT CPBank4GameWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CPBank4GameWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CPBank4GameWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CPBank4GameWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CPBank4GameWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CPBank4GameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CPBank4GameWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	return 0;
}


LRESULT CPBank4GameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

		default:
			bHandled = FALSE;
	}


	if(bHandled)
	{
		if (uMsg != WM_PAINT)
		{
			return lRes;
		}

	}

	if(m_pm.MessageHandler(uMsg,wParam,lParam,lRes))
	{
		if (uMsg != WM_PAINT)
		{
			return lRes;
		}
	}

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CPBank4GameWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if( uMsg == WM_KEYDOWN ) 
	{
		if( wParam == VK_RETURN ) 
		{
			return true;
		}
		else if( wParam == VK_ESCAPE ) 
		{
			m_nRetFlag = 0;
			Close();
			return true;
		}
	}
	else if (uMsg == WM_KEYDOWN)
	{
		return true;
	}
	return false;
}


//����OK
void CPBank4GameWnd::OnInOK()
{
	CString strMsg;
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

	CEditUI* pOutCointEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_Money"));

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


	if (i64Num > m_i64GameWallet)
	{
		ShowTipMessage(NULL,"","","������������Ѵ��ڳ������еĴ��");
		pOutCointEdt->SetFocus();
		return;
	}

	//ѡ�������Ϸ���ǵ�ǰ�������Ϸһ��ʱ�����������
	if (m_i64GameWallet - i64Num < m_pGameInfo->uLessPoint)
	{	
		TCHAR szNum[128] = {0};
		GlbGetNumString(szNum,m_i64GameWallet - i64Num,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);

		ShowTipMessage(NULL,"PBankWnd2","InSv_Msg_LessCoint",
			"������Ϸ��%s������Ϸ��ʣ����%s,С�ڷ������޽��%d�������Դ��,�����µ����������ݶ",
			m_pGameInfo->szGameName,szNum,m_pGameInfo->uLessPoint);
		pOutCointEdt->SetFocus();
		return;
	}

	/// ����Ϣ
	if (m_pRoom != NULL)
	{
		if (m_nBankVer == 1)
		{
			CheckMoney_t* pCheckMoney = new CheckMoney_t();
			pCheckMoney->uType = 0;
			pCheckMoney->i64Money = i64Num;
			m_pRoom->PostMessage(WM_CHECK_MONEY, 0, (LPARAM)pCheckMoney);
			//m_pRoom->PostMessage(WM_CHECK_MONEY2,0,(LPARAM)&tagBankOp);
			//����PostMessage��Ч�����Զ�ʱ���޹�!�ĳɵ���PreTranslateMessage
// 			MSG msg;
// 			msg.message = WM_CHECK_MONEY;
// 			msg.lParam = (LPARAM)&tagCheckMoney;
// 			m_pRoom->PreTranslateMessage(&msg);
		}
		else if (m_nBankVer == 2)
		{
			bank_op_normal* pBankOp = new bank_op_normal;
			pBankOp->_user_id = m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID;
			pBankOp->_operate_type = 2;
			pBankOp->_game_id = m_pGameInfo->uNameID;
			pBankOp->_money = i64Num;
			m_pRoom->PostMessage(WM_CHECK_MONEY2,0,(LPARAM)pBankOp);
			//����PostMessage��Ч�����Զ�ʱ���޹�!�ĳɵ���PreTranslateMessage
// 			MSG msg;
// 			msg.message = WM_CHECK_MONEY2;
// 			msg.lParam = (LPARAM)&tagBankOp;
// 			m_pRoom->PreTranslateMessage(&msg);
		}
	}

	return;
}

//ȡ��OK
void CPBank4GameWnd::OnOutOK()
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CEditUI* pOutCointEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_Money"));


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

	CStdString strPassword;
	CEditUI* pEditCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Password"));
	strPassword = pEditCtrl->GetText();
	if (strPassword.GetLength() == 0)
	{
		ShowTipMessage(NULL,"","","����������");
		pEditCtrl->SetFocus();
		return;
	}

	/// ����Ϣ
	if (m_pRoom != NULL)
	{
		if (m_nBankVer == 1)
		{
			CheckMoney_t* pCheckMoney = new CheckMoney_t;
			pCheckMoney->uType = 1;
			pCheckMoney->i64Money = i64Num;
			//��ȡ���������
			//��ȡ���������
			_tcscpy(pCheckMoney->szPWD,(TCHAR*)strPassword.GetData());
			m_pRoom->PostMessage(WM_CHECK_MONEY, 0, (LPARAM)pCheckMoney);
			//m_pRoom->PostMessage(WM_CHECK_MONEY2,0,(LPARAM)&tagBankOp);
			//����PostMessage��Ч�����Զ�ʱ���޹�!�ĳɵ���PreTranslateMessage
// 			MSG msg;
// 			msg.message = WM_CHECK_MONEY;
// 			msg.lParam = (LPARAM)&tagCheckMoney;
// 			m_pRoom->PreTranslateMessage(&msg);

		}
		else if (m_nBankVer == 2)
		{
			bank_op_normal* pBankOp = new bank_op_normal;
			pBankOp->_user_id = m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID;
			pBankOp->_operate_type = 1;
			pBankOp->_game_id = m_pGameInfo->uNameID;
			pBankOp->_money = i64Num;
			//��ȡ���������
			//��ȡ���������
			GetCryptedPasswd(pBankOp->_szMD5Pass,(TCHAR*)strPassword.GetData());
			m_pRoom->PostMessage(WM_CHECK_MONEY2,0,(LPARAM)pBankOp);
			//����PostMessage��Ч�����Զ�ʱ���޹�!�ĳɵ���PreTranslateMessage
// 			MSG msg;
// 			msg.message = WM_CHECK_MONEY2;
// 			msg.lParam = (LPARAM)&tagBankOp;
// 			m_pRoom->PreTranslateMessage(&msg);
		}
	}
}


void CPBank4GameWnd::Init() 
{
	CEditUI* pEdtCtrl = NULL;
	CLabelUI* pLabCtrl = NULL; 

	LoadNormalCtrlSkin();
	
	COptionUI* pOptCtrl = static_cast<COptionUI*>(m_pm.FindControl("Opt_In"));
	pOptCtrl->Selected(true);

	m_pCointEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_Money"));
	m_pCointEdt->SetMaxChar(23);

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Password"));
	pEdtCtrl->SetMaxChar(16);

	//��ʾ��Ϸ��
	TCHAR szText[64];
	pLabCtrl = static_cast<CEditUI*>(m_pm.FindControl("Lab_GameName"));
	_stprintf(szText,"<b>%s</b>",m_pGameInfo->szGameName);
	pLabCtrl->SetText(szText);

}

//����Combol���б���
void CPBank4GameWnd::LoadComboItems(CComboUI* pCtrl,LPCTSTR items[],int nums,int nSelect/* = 0*/)
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
bool CPBank4GameWnd::SelectComboItem(BzDui::CComboUI* pCtrl,LPCTSTR szText)
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

void CPBank4GameWnd::Notify(TNotifyUI& msg)
{
	if(msg.sType == _T("click")) 
	{
		if( msg.pSender->GetName() == _T("Btn_OK")) 
		{
			COptionUI* pOptCtrl = static_cast<COptionUI*>(m_pm.FindControl("Opt_In"));
			if (pOptCtrl->IsSelected()) //����
			{
				OnInOK();
			}
			else
			{
				OnOutOK();
			}
		}
		else if (msg.pSender->GetName() == _T("Btn_Sel_0"))
		{
			TCHAR szNum[128];
			COptionUI* pOptCtrl = static_cast<COptionUI*>(m_pm.FindControl("Opt_In"));
			if (pOptCtrl->IsSelected()) //����
			{
				if (m_i64GameWallet - m_pGameInfo->uLessPoint > 0)
				{
					GlbGetNumString(szNum,m_i64GameWallet - m_pGameInfo->uLessPoint,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);
				}
				else
				{
					ShowTipMessage(NULL,NULL,NULL,_T("�����Դ�������"));
					return;
				}
			}
			else
			{
				GlbGetNumString(szNum,m_i64BankMoney,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);
			}
			
			CEditUI* pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Money"));
			pEdtCtrl->SetText(szNum);
			pEdtCtrl->SetFocus();
		}
		else if (msg.pSender->GetName() == _T("Btn_Sel_1"))
		{
			AddMoneyToEdtCtrl(m_pCointEdt,10000000);
		}
		else if (msg.pSender->GetName() == _T("Btn_Sel_2"))
		{
			AddMoneyToEdtCtrl(m_pCointEdt,1000000000);
		}
		else if (msg.pSender->GetName() == _T("Btn_Sel_3"))
		{
			AddMoneyToEdtCtrl(m_pCointEdt,1000000);
		}
		else if (msg.pSender->GetName() == _T("Btn_Sel_4"))
		{
			AddMoneyToEdtCtrl(m_pCointEdt,100000000);
		}
		else if (msg.pSender->GetName() == _T("Btn_Sel_5"))
		{
			AddMoneyToEdtCtrl(m_pCointEdt,100000);
		}
		else if(msg.pSender->GetName() == _T("Btn_Close") 
					|| msg.pSender->GetName() == _T("Btn_Cancel")) //�رհ�ť
		{
			Close();
			m_pRoom->PostMessage(WM_BANK2_CLOSE,0,0);
			//����PostMessage��Ч�����Զ�ʱ���޹�!�ĳɵ���PreTranslateMessage

// 			MSG msg;
// 			msg.message = WM_BANK2_CLOSE;
// 			m_pRoom->PreTranslateMessage(&msg);

			return;
		}
	}
	else if (msg.sType == _T("textchanged"))
	{
		if (msg.pSender->GetName() == _T("Edt_Money"))
		{
			//ת�˽��Сдת��Ϊ���Ĵ�д
			ChangeEtToChinese(static_cast<CEditUI*>(msg.pSender),NULL);
			return;
		}
	}
	else if(msg.sType == _T("selectchanged"))
	{
		if (msg.pSender->GetName() == _T("Opt_In"))
		{
			//�л�������ģʽ
			OnBnClickedAccessIn();
			return;
		}
		else if (msg.pSender->GetName() == _T("Opt_Out"))
		{
			//�л���ȡ��ģʽ
			OnBnClickedAccessOut();
			return;
		}

	}
}

void CPBank4GameWnd::CenterWindow()
{
	RECT rc;
	::GetWindowRect(m_hWnd,&rc);
	int iSrcW = GetSystemMetrics(SM_CXSCREEN);
	int iSrcH = GetSystemMetrics(SM_CYSCREEN);
	int iWndW = rc.right - rc.left;
	int iWndH = rc.bottom - rc.top;
	::MoveWindow(m_hWnd,(iSrcW - iWndW)/2,(iSrcH - iWndH)/2,iWndW,iWndH,true);
}

void CPBank4GameWnd::ShowWindow(bool bShow /*= true*/, bool bTakeFocus /*= true*/)
{
	CEditUI* pEdtCtrl = NULL;

	__super::ShowWindow(bShow,bTakeFocus);

	if(bShow)
	{
		pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Money"));
		pEdtCtrl->SetFocus();
	}
	else
	{
		pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Money"));
		pEdtCtrl->SetText(_T(""));

		pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Password"));
		pEdtCtrl->SetText(_T(""));
	}
}

//���س���ؼ�Ƥ��
void CPBank4GameWnd::LoadNormalCtrlSkin()
{
	int idx = 0;
	CStdPtrArray* pCtrls = NULL;

	//���������ı��༭���Ƥ��
	pCtrls = m_pm.FindSubControlsByClass(m_pm.GetRoot(),_TEXT("EditUI"));
	CEditUI* pEditUI = NULL;
	do 
	{
		pEditUI = static_cast<CEditUI*>(pCtrls->GetAt(idx++));
		if (pEditUI)
		{
			pEditUI->SetNormalImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,0,500,22' corner='3,4,2,2'");
			pEditUI->SetHotImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,22,500,44' corner='3,4,2,2'");
			pEditUI->SetFocusedImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,44,500,66' corner='3,4,2,2'");
			pEditUI->SetDisabledImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,66,500,88' corner='3,4,2,2'");
		}
	} while (pEditUI);

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

/// �������а汾
void CPBank4GameWnd::SetBankVersion(int nVer)
{
	m_nBankVer = nVer;
}

/// ����ȫ�ֱ����еļ��ܷ�ʽ��ȡ�ü��ܺ�������ַ���������sha������MD5
/// �����ﲻ��ָ��ĺϷ������ж�
/// @param szMD5Pass ���ܺ���ַ���
/// @param szSrcPass Դ�ַ���
/// @return ���ܺ���ַ���ָ��
TCHAR* CPBank4GameWnd::GetCryptedPasswd(TCHAR *szMD5Pass, TCHAR *szSrcPass)
{
	CenterServerMsg& par=Glb().m_CenterServerPara;
	//md5��ʽ
	if( par.m_nEncryptType == 1)
	{
		unsigned char szMDTemp[16];
		MD5_CTX Md5;
		Md5.MD5Update((unsigned char *)szSrcPass,lstrlen(szSrcPass));
		Md5.MD5Final(szMDTemp);
		for (int i = 0; i < 16; i ++) 
			wsprintf(&szMD5Pass[i * 2], "%02x", szMDTemp[i] );
	}
	//sha���ܷ�ʽ
	if(par.m_nEncryptType == 2)
	{
		unsigned szMDTemp[5]={0};
		CString strSha=szSrcPass;
		SHA1 sha;
		sha.Reset();
		sha<<strSha;
		sha.Result(szMDTemp);

		CString strSingle;
		strSha="";
		for (int i=0;i<5;i++) 
		{
			strSingle.Format("%.8x",szMDTemp[i]);
			strSha+=strSingle;
		}
		wsprintf(szMD5Pass,"%s",strSha);
	}
	return szMD5Pass;
}

///
/// ���òƸ���Ϣ
/// @param nBank ���д��
/// @param nWallet �����ֽ�
/// @return void
///
void CPBank4GameWnd::SetWealthInfor(__int64 i64Bank, __int64 i64Wallet)
{
	m_i64BankMoney = i64Bank;
	m_i64GameWallet = i64Wallet;
	UpdateWealthInfor();
}

///
/// ���òƸ���Ϣ
/// @param nWallet �����ֽ�
/// @return void
///
void CPBank4GameWnd::SetWealthInfor(__int64 i64Wallet)
{
	m_i64GameWallet = i64Wallet;
	UpdateWealthInfor();
}

///
/// ���²Ƹ���Ϣ
/// @return void
///
void CPBank4GameWnd::UpdateWealthInfor()
{
	TCHAR szNum[128] = {0};
	TCHAR szTemp[128] = {0};
	CLabelUI* pLabCtrl = NULL;

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_BankCoints"));
	pLabCtrl->SetShowHtml(true);
	GlbGetNumString(szNum,m_i64BankMoney,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);
	_stprintf(szTemp,_T("<b>%s</b>"),szNum);
	pLabCtrl->SetText(szTemp);

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_WalletCoints"));
	pLabCtrl->SetShowHtml(true);
	GlbGetNumString(szNum,m_i64GameWallet,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);
	_stprintf(szTemp,_T("<b>%s</b>"),szNum);
	pLabCtrl->SetText(szTemp);

}

//����������������������ͬ��ת��Ϊ���Ĵ�д
void CPBank4GameWnd::ChangeEtToChinese(BzDui::CEditUI* pEditCtrl,BzDui::CTextUI* pTextCtrl)
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
		GlbGetNumString(szNum,i64Num,0,Glb().m_bUseSpace,Glb().m_strSpaceChar);
		bFlag = true;
		pEditCtrl->SetText(szNum);
		//���ù�굽�����
		pEditCtrl->SetSel(-1);
	}
	else
	{
		bFlag = false;
	}
		

	return;
}


///
/// ��ʾ
/// @param pToCtrl ��ĳһ�ؼ�����ʾ
/// @param lpSecName ClientMessage.bcf�еĶ���
/// @param lpKeyName ClientMessage.bcf�еļ�ֵ
/// @param lpDefault ClientMessage.bcfû���ҵ���ֵʱ��ʹ�õ�Ĭ��ֵ
/// @return void
///
void CPBank4GameWnd::ShowTipMessage(BzDui::CControlUI* pToCtrl,LPCTSTR lpSecName,LPCTSTR lpKeyName,LPCTSTR lpDefault,...)
{
	char szFormat[BUFSIZ];
	CBcfFile fMsg(CBcfFile::GetAppPath()+"..\\ClientMessage.bcf");
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
		DUIOkeyMsgBox(NULL,false,"%s",szMessage);
	}

	return;

}

//�л���ȡ���ͣ����
void CPBank4GameWnd::OnBnClickedAccessIn()
{
	//����ʾ���������
	CLabelUI* pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Password"));
	pLabCtrl->SetVisible(false);

	CEditUI* pEditCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Password"));
	pEditCtrl->SetVisible(false);

	return;
}

//�л���ȡ���ͣ�ȡ��
void CPBank4GameWnd::OnBnClickedAccessOut()
{
	//��ʾ���������
	CLabelUI* pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Password"));
	pLabCtrl->SetVisible(true);

	CEditUI* pEditCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Password"));
	pEditCtrl->SetVisible(true);

	return;
}

//�������İ�ť��ӽ�Ҵ������
void CPBank4GameWnd::AddMoneyToEdtCtrl(CEditUI* pEdtUI,__int64 i64Money)
{
	CString strNum = pEdtUI->GetText();

	//����зָ�������ȥ���ַ�����ķָ���
	if(Glb().m_bUseSpace)
		strNum.Replace(Glb().m_strSpaceChar,"");

	COptionUI* pOptCtrl = static_cast<COptionUI*>(m_pm.FindControl("Opt_In"));

	/// ���ĩ��λ������,��ȡǰNλ
	CString strSubNum = strNum.Right(Glb().m_nPowerOfGold);
	strSubNum = strNum.Left(strNum.GetLength()-Glb().m_nPowerOfGold);
	__int64 i64Num = _atoi64(strSubNum.GetBuffer());

	if(pOptCtrl->IsSelected()) //��Ǯ
	{
		if(i64Num + i64Money > m_i64GameWallet - m_pGameInfo->uLessPoint)
		{
			return;
		}
	}
	else
	{
		if(i64Num + i64Money > m_i64BankMoney)
		{
			return;
		}
	}
	

	i64Num += i64Money;

	TCHAR szNum[128];
	GlbGetNumString(szNum,i64Num,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);
	pEdtUI->SetText(szNum);
	pEdtUI->SetFocus();

}

bool CPBank4GameWnd::OnNetMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
	if (pNetHead->bMainID != MDM_GP_BANK && pNetHead->bMainID != MDM_BANK) return true;

	switch (pNetHead->bAssistantID)
	{
	case ASS_BANK_NORMAL:
	case ASS_GP_BANK_CHECK:
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

			bank_op_normal *p = (bank_op_normal *)pNetData;
			if (p != NULL)
			{
				if (p->_operate_type == 1) //ȡǮ
				{
					if (pNetHead->bHandleCode == HC_BANK_OP_SUC)
					{
						TCHAR szNum[128];
						GlbGetNumString(szNum,p->_money,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);

						ShowTipMessage(NULL,"PBankWnd2","OutSv_Msg_Success",
							"���ɹ���ȡ�� %s ��ҵ���%s����Ϸ��Ǯ���С�",
							szNum,m_pGameInfo->szGameName);

						//�������ݼ���ʾ����
// 						m_i64BankMoney -= p->_money;
// 						m_i64GameWallet += p->_money;
// 						UpdateWealthInfor();

						m_pRoom->PostMessage(WM_BANK2_CHECK_SUC,0,1);

						return true;

					}
					else if (pNetHead->bHandleCode == HC_BANK_OP_ERR_PWD)
					{
						DUIOkeyMsgBox(NULL,false,"�������벻��ȷ");
						CEditUI* pEditCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Password"));
						pEditCtrl->SetText("");
						pEditCtrl->SetFocus();
						return true;
					}
				}
				else //��Ǯ
				{
					if (pNetHead->bHandleCode == HC_BANK_OP_SUC)
					{
						TCHAR szNum[128];
						GlbGetNumString(szNum,p->_money,Glb().m_nPowerOfGold,Glb().m_bUseSpace,Glb().m_strSpaceChar);

						ShowTipMessage(NULL,"PBankWnd2","InSv_Msg_Success",
							"���ɹ��Ѵӡ�%s����ϷǮ��ȡ�� %s ��Ǯ���С�",
							m_pGameInfo->szGameName,szNum);

						m_pRoom->PostMessage(WM_BANK2_CHECK_SUC,0,0);

						//�������ݼ���ʾ����
// 						m_i64BankMoney += p->_money;
// 						m_i64GameWallet -= p->_money;
// 						UpdateWealthInfor();
					}

					return true;
				}
			}
			break;
		}
	}


	return true;
}
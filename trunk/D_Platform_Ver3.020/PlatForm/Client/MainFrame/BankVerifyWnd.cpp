#include "StdAfx.h"
#include "BankVerifyWnd.h"
#include "GameRoomEx.h"

using namespace BzDui;

#define IDT_SET_FOCUS	1

CBankVerifyWnd::CBankVerifyWnd(void)
{
	//���֤�����
	m_pPwdEdt = NULL;

	m_pVerifyInfo = NULL;

	m_nRetFlag = 0;

	//��֤��������
	m_pVCCreater = NULL;

	//��½���е��û���Ϣ
	m_pMyInfo = NULL;
}

CBankVerifyWnd::~CBankVerifyWnd(void)
{
}

LPCTSTR CBankVerifyWnd::GetWindowClassName() const 
{ 
	return _T("UIFrame");
};

UINT CBankVerifyWnd::GetClassStyle() const 
{ 
	return UI_CLASSSTYLE_DIALOG; 
};

void CBankVerifyWnd::OnFinalMessage(HWND hWnd) 
{ 
	m_pm.RemovePreMessageFilter(this);
	//delete this;
};

LRESULT CBankVerifyWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	m_pm.AddPreMessageFilter(this);
	CDialogBuilder builder;

	CControlUI* pRoot = builder.Create(_T("BankWnd2\\BankVerify.xml"), (UINT)0, NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	Init();
	return 0;
}

LRESULT CBankVerifyWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CBankVerifyWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CBankVerifyWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CBankVerifyWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CBankVerifyWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CBankVerifyWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CBankVerifyWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == IDT_SET_FOCUS)
	{
		::KillTimer(m_hWnd,IDT_SET_FOCUS);
		m_pPwdEdt->SetFocus();
	}

	return 0;
}


LRESULT CBankVerifyWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

	if (uMsg == WM_PAINT)
	{
		if (m_pVCCreater)
		{
			m_pVCCreater->Draw(128,150);
		}
	}

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CBankVerifyWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if( uMsg == WM_KEYDOWN ) 
	{
		if( wParam == VK_RETURN ) 
		{
			OnOK();
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

void CBankVerifyWnd::OnOK()
{
	if (m_pPwdEdt->GetText().GetLength() == 0)
	{
		//AFCMessageBox("����д��������");
		DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"����д��������");
		m_pPwdEdt->SetFocus();
		return;
	}

	if (m_pVCodeEdt->GetText().GetLength() == 0)
	{
		//AFCMessageBox("����д��֤��");
		DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"����д��֤��");
		m_pVCodeEdt->SetFocus();
		return;
	}

	if (!m_pVCCreater)
	{
		return;
	}

	if (!m_pVCCreater->IsVerify(m_pVCodeEdt->GetText()))
	{
		//AFCMessageBox("��֤�벻��ȷ");
		DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"��֤�벻��ȷ");
		m_pVCodeEdt->SetFocus();
		return;
	}

	char szMD5Input[64];
	CGameRoomEx::GetCryptedPasswd(szMD5Input,(char*)m_pPwdEdt->GetText().GetData());

	if (strcmp((const char*)szMD5Input,m_pVerifyInfo->szTwoPassword) == 0)
	{
		m_nRetFlag = 1;
		Close();
		return;
	}
	else
	{
		//AFCMessageBox("�������벻��ȷ����������");
		DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"�������벻��ȷ����������");
		m_pPwdEdt->SetFocus();
	}

}

void CBankVerifyWnd::Init() 
{
	LoadNormalCtrlSkin();

	::SetWindowPos(m_hWnd,CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);

	m_pPwdEdt = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Pwd")));
	m_pPwdEdt->SetMaxChar(20);
	m_pVCodeEdt = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_VCode")));
	m_pVCodeEdt->SetMaxChar(4);

	//�����֤����ƶ���
	m_pVCCreater = CVerifyCodeCreater::GetInstance();
	m_pVCCreater->SetParent(m_hWnd);

	//������֤��ı�����ɫ
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f(CBcfFile::GetAppPath() + strSkin);
	if ("skin0.bcf" == strSkin)
		m_crVCodeBk = f.GetKeyVal(_T("skin0"),_T("verify_code_bkcolor"),(__int64)RGB(0xEC,0xF6,0xFF));
	else
		m_crVCodeBk = f.GetKeyVal(_T("skin1"),_T("verify_code_bkcolor"),(__int64)RGB(0xEC,0xF6,0xFF));
	m_pVCCreater->ReCreateBMP(130,37,m_crVCodeBk);

	if (m_pMyInfo)
	{
		CLabelUI* pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Welcome"));
		CStdString sWelCome = pLabCtrl->GetText();
		char szBuffer[128];
		_stprintf(szBuffer,sWelCome.GetData(),m_pMyInfo->dwUserID,m_pMyInfo->nickName);
		pLabCtrl->SetText(szBuffer);
	}

	::SetTimer(m_hWnd,IDT_SET_FOCUS,500,NULL);
	
}

//����Combol���б���
void CBankVerifyWnd::LoadComboItems(CComboUI* pCtrl,LPCTSTR items[],int nums,int nSelect/* = 0*/)
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
bool CBankVerifyWnd::SelectComboItem(BzDui::CComboUI* pCtrl,LPCTSTR szText)
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

void CBankVerifyWnd::Notify(TNotifyUI& msg)
{
	if(msg.sType == _T("click")) 
	{
		if( msg.pSender->GetName() == _T("Bnt_Close")) 
		{
			m_nRetFlag = 0;
			Close();
		}
		if( msg.pSender->GetName() == _T("Btn_OK")) 
		{
			OnOK();
		}
		if( msg.pSender->GetName() == _T("Btn_Cancel")) 
		{
			m_nRetFlag = 0;
			Close();
		}
		if ( msg.pSender->GetName() == _T("Bnt_Change"))
		{
			if (m_pVCCreater)
			{
				m_pVCCreater->ReCreateBMP(130,37,m_crVCodeBk);
				m_pVCCreater->Draw(128,150);
			}
		}
	}
}

void CBankVerifyWnd::CenterWindow()
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
void CBankVerifyWnd::LoadNormalCtrlSkin()
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
			//pEditUI->SetHotImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,22,500,44' corner='3,4,2,2'");
			//pEditUI->SetFocusedImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,44,500,66' corner='3,4,2,2'");
			pEditUI->SetDisabledImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,66,500,88' corner='3,4,2,2'");
		}
	} while (pEditUI);
}
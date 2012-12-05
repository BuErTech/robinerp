#include "StdAfx.h"
#include "PBankWnd.h"
#include "MainRoomEx.h"
#include "MyDiskInfo.h"
#include "Bank4ZMessage.h"

using namespace BzDui;

#define PAGE_NUMS			5	//������Ŀ����
#define RECORD_PAGE_SIZE	8	//ת��־ÿҳ�ļ�¼����

#define NICK_ID_USE_TO_TRANSFER	11 //��������ת��
#define NICK_ID_USE_TO_DEPOSIT	12 //���ڵ㿨��ֵ
#define NICK_ID_BYID			0  //ͨ���û�ID��ѯ�û��ǳ�
#define NICK_ID_BYNICKNAME		1  //ͨ������ǳƲ�ѯ�û�ID

static char* szDigit[DIGITSIZE] = {"��", "Ҽ", "��", "��", "��", "��", "½", "��", "��", "��"}; //���Ĵ�д����
static char* szUnit[UNITSIZE] = {"", "ʰ", "��", "Ǫ", "��", "ʰ", "��", "Ǫ", "��", "ʰ","��","Ǫ","��", "ʰ","��","Ǫ","��","ʰ","��"};	  //���Ĵ�д��λ

//��ǰʵ�����ھ��
HWND CPBankWnd::m_hInstanceWnd = NULL;

CPBankWnd::CPBankWnd(CWnd* pParent,bool bInMainRoom /*= false*/)
{
	//Ĭ����ʾ��ҳ����
	m_nDefaultPage = 0;

	bFlag = false;

	//��ֵ��Ŀ����ң�ʹ��ID��ʽ����ʹ���ǳƷ�ʽ
	m_bUseDestID = true;

	m_bTransferSuccess = true/*false*/;

	m_pGameRoom = pParent;

	m_strSaveID = "";

	m_iUseNickName = 0;

	m_uCheckInOrOut = 0;

	m_mapRecord.clear();

	//ת����־��¼����ʾλ��
	m_iRecDispPos = 0;

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

}

CPBankWnd::~CPBankWnd(void)
{
	m_mapRecord.clear();
}

LPCTSTR CPBankWnd::GetWindowClassName() const 
{ 
	return _T("UIFrame");
};

UINT CPBankWnd::GetClassStyle() const 
{ 
	return UI_CLASSSTYLE_DIALOG; 
};

void CPBankWnd::OnFinalMessage(HWND hWnd) 
{ 
	m_pm.RemovePreMessageFilter(this);
};

LRESULT CPBankWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);

	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("BankWnd\\BankWnd.xml"), (UINT)0, NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	::SetWindowPos(m_hWnd,CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);

	Init();

	CPBankWnd::m_hInstanceWnd = m_hWnd;

	//::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	return 0;
}

LRESULT CPBankWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CPBankWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CPBankWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CPBankWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CPBankWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CPBankWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CPBankWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

LRESULT CPBankWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
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

LPCTSTR CPBankWnd::GetItemText(CControlUI* pControl, int iIndex, int iSubItem)
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
			strBuffer.Format(_T("%02d-%02d %02d:%02d"),
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

bool CPBankWnd::OnNetMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, CTCPClientSocket * pClientSocket)
{
	//Ŀǰֻ���ڴ����е�����ʹ��

	if (pNetHead->bMainID != MDM_GP_BANK && pNetHead->bMainID != MDM_BANK)
	{
		 return true;
	}

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

// 						ShowTipMessage(NULL,"PBankWnd2","OutSv_Msg_Success",
// 							"���ɹ���ȡ�� %s ��ҵ���%s����Ϸ��Ǯ���С�",
// 							szNum,GetGameNameById(p->_game_id));

						CEditUI* pPwdEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn4_Pwd"));
						pPwdEdt->SetText("");

						//�������ݼ���ʾ����
						UpdateUserMoney(0,p->_money);
						
						GetMainRoom()->m_PlaceUserInfo.i64Money += p->_money;

						return true;

					}
					else if (pNetHead->bHandleCode == HC_BANK_OP_ERR_PWD)
					{
						DUIOkeyMsgBox(m_hWnd,false,"�������벻��ȷ");
						CEditUI* pPwdEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn4_Pwd"));
						pPwdEdt->SetFocus();

						return true;
					}
					else if (pNetHead->bHandleCode == HC_BANK_OP_CHECKMONEY_INV)
					{
						DUIOkeyMsgBox(m_hWnd,false,"��������Ϸ�У�����ȡ�");
						CEditUI* pPwdEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn4_Pwd"));
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

// 						ShowTipMessage(NULL,"PBankWnd2","InSv_Msg_Success",
// 							"���ɹ��Ѵӡ�%s����ϷǮ��ȡ�� %s ��Ǯ���С�",
// 							GetGameNameById(p->_game_id),szNum);

						CEditUI* pPwdEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn4_OutCoints"));
						pPwdEdt->SetFocus();
						pPwdEdt->SetText("");

						//�������ݼ���ʾ����
						UpdateUserMoney(1,p->_money);

						GetMainRoom()->m_PlaceUserInfo.i64Money -= p->_money;
					}
					else if (pNetHead->bHandleCode == HC_BANK_OP_CHECKMONEY_INV)
					{
						DUIOkeyMsgBox(m_hWnd,false,"��������Ϸ�У����ܴ�");
						CEditUI* pPwdEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn4_Pwd"));
						pPwdEdt->SetFocus();

						return true;
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

void CPBankWnd::Init() 
{

	char szName[20];
	UINT uYPos = 40;

	for (int i = 0; i < PAGE_NUMS; ++i)
	{
		::sprintf_s(szName,sizeof(szName),_T("Ctn_Page_%d"),i + 1);
		m_pPageContainer[i] = static_cast<CContainerUI*>(m_pm.FindControl(szName));
	}

	for (int i = 0; i < PAGE_NUMS; ++i)
	{
		::sprintf_s(szName,sizeof(szName),_T("Bnt_Page_%d"),i + 1);
		CButtonUI* pBtnCtrl = static_cast<CButtonUI*>(m_pm.FindControl(szName));
		if(pBtnCtrl)
		{
			SIZE sz = {pBtnCtrl->GetFixedXY().cx,uYPos};
			pBtnCtrl->SetFixedXY(sz);
			if(pBtnCtrl->GetName() == _T("Bnt_Page_2") || pBtnCtrl->GetName() == _T("Bnt_Page_3"))
			{
				if(CGamePlaceApp::GetModuleStatus("Bank","TransferMoney",1) == 0)
				{
					pBtnCtrl->SetVisible(false);
					continue;
				}
			}
			uYPos += 34;
		}
	}

	//��¼��ؼ�ָ��
	m_pRecordList = static_cast<CListUI*>(m_pm.FindControl("Lst_Ctn3_Record"));
	m_pRecordList->SetTextCallback(this);

	//��ҳ�ؼ���ť
	m_pFirstPageBnt = static_cast<CButtonUI*>(m_pm.FindControl("Bnt_Ctn3_First"));
	m_pPrePageBnt   = static_cast<CButtonUI*>(m_pm.FindControl("Bnt_Ctn3_Previous"));
	m_pNextPageBnt  = static_cast<CButtonUI*>(m_pm.FindControl("Bnt_Ctn3_Next"));
	m_pRearPageBnt  = static_cast<CButtonUI*>(m_pm.FindControl("Bnt_Ctn3_Rear"));

	CEditUI* pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn2_Bank"));
	pEdtCtrl->SetShowHtml(true);
	pEdtCtrl->SetReadOnly(true);

	LoadNormalCtrlSkin();

	//��ʼ���Ƹ���Ϣ
	InitMoneyInfo();

	//��ʼ���Ͳ���UI
	InitGiftInfo();

	//��ʼ��ȡ�����UI
	InitAccessInfo();

	//��ʼ����־����UI
	InitRecordInfo();

	//��ʼ���޸���������UI
	InitModifyPwdInfo();

	//��ʼĬ����ʾ��һҳ
	ShowPage(m_nDefaultPage);

}

//����Combol���б���
void CPBankWnd::LoadComboItems(CComboUI* pCtrl,LPCTSTR items[],int nums,int nSelect/* = 0*/)
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
bool CPBankWnd::SelectComboItem(BzDui::CComboUI* pCtrl,LPCTSTR szText)
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

//���ת���Ĵ�д
void CPBankWnd::ChangeDigitToChinese(__int64 i64Num, CString &strResult)
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

///
/// ���òƸ���Ϣ
/// @param nBank ���д��
/// @param nWallet �����ֽ�
/// @return void
///
void CPBankWnd::SetWealthInfor(__int64 i64Bank, __int64 i64Wallet)
{
	CBcfFile fMsg(CBcfFile::GetAppPath() + "ClientMessage.bcf");

	m_i64BankMoney = i64Bank;
	m_i64WalletMoney = i64Wallet;

	TCHAR szNum[128] = {0};
	TCHAR szBank[128] = {0};
	GetNumString(szBank, m_i64BankMoney, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
	GetNumString(szNum, m_i64WalletMoney, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);

	CLabelUI* pLabCtrl = NULL;
	CEditUI* pEdtCtrl = NULL;
	TCHAR szOut[128];
	
	//���б�����
	::memset(szOut,NULL,sizeof(szOut));
	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn1_Wallet"));
	::sprintf(szOut,pLabCtrl->GetText().GetData(),szNum);
	pLabCtrl->SetText(szOut);

	//���ڽ��
	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn1_Bank"));
	::sprintf(szOut,pLabCtrl->GetText().GetData(),szBank);
	pLabCtrl->SetText(szOut);

	::memset(szOut,NULL,sizeof(szOut));
	::sprintf(szOut,fMsg.GetKeyVal("PBankDlg","UHaveCoin","��Ǯ������%s��ҡ�"),szNum);
	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn2_Tip1"));
	pLabCtrl->SetText(szOut);

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn2_Bank"));
	pEdtCtrl->SetText(szBank);

	::memset(szOut,NULL,sizeof(szOut));
	::sprintf(szOut,fMsg.GetKeyVal("PBankDlg","AccountCoin","���н�ң�%s"),szNum);
	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn4_Wallet"));
	pLabCtrl->SetText(szOut);

	::memset(szOut,NULL,sizeof(szOut));
	::sprintf(szOut,fMsg.GetKeyVal("PBankDlg","BoxCoin","���д�%s"),szBank);
	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn4_Bank"));
	pLabCtrl->SetText(szOut);
	
}

// PengJiLin, 2010-8-23, �����û�ID��ȡ���ǳ�
// ���û�ȡ���ǳ�
void CPBankWnd::SetNickName(TCHAR* szNickName)
{
	m_strNickName = szNickName;

	//m_Button_Trans_All.EnableWindow(TRUE);
	//m_btnTranOK.EnableWindow(TRUE);
}

///�����û�ID�ǳƵĲ�ѯ���
void CPBankWnd::UpdateNicknameIDResult(GetNickNameID_t* pNID)
{
	CLabelUI* pLabCtrl = NULL;
	CLabelUI* pLabCtrl_temp = NULL;
	TCHAR szText[64];

	if (pNID->_nUseful == NICK_ID_USE_TO_TRANSFER)
	{
		pLabCtrl_temp = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn2_NickNameID_temp"));
		pLabCtrl_temp->SetText("");

		pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn2_NickNameID"));

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

/// ת�ʳɹ��������������
/// @param Opt 0-��ʾת�������ˣ�1-��ʾ�ɱ���ת����Լ�
/// @param nMoney ��ʾ�����������Ľ����
void CPBankWnd::UpdateUserMoneyByTransfer(int OptType, __int64 i64Money)
{
	if( OptType == 0 ) //ȡ��
	{
		m_i64BankMoney	-= i64Money;
	}
	else //����
	{
		m_i64BankMoney	+= i64Money;
	}

	TCHAR szBank[128] = {0};
	GetNumString(szBank, m_i64BankMoney, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);

	CEditUI* pEdtCtrl = NULL;
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn2_Bank"));
	pEdtCtrl->SetText(szBank);

}

/// ��ȡǮ����½���
/// @param Opt 0-��ʾȡ����1-��ʾ����
/// @param nMoney ��ʾ�����������Ľ����
void CPBankWnd::UpdateUserMoney(int OptType, __int64 i64Money)
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	TCHAR szNum[128];
	GetNumString(szNum, i64Money, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
	CString strMsg;
	CString strFormat = fMsg.GetKeyVal("PBankDlg", "CheckInOutTip", "�ɹ�%s %s ���");
	if( OptType == 0 ) //ȡ��
	{
		m_i64BankMoney	-= i64Money;
		m_i64WalletMoney += i64Money;
		strMsg.Format(strFormat, "ȡ��", szNum);
	}
	else //����
	{
		m_i64BankMoney	+= i64Money;
		m_i64WalletMoney -= i64Money;
		strMsg.Format(strFormat, "����", szNum);
	}

	SetWealthInfor(m_i64BankMoney,m_i64WalletMoney);

// 	if (GetSafeHwnd()!=NULL)
// 	{
// 		ShowInterface();
// 		/// ��ʾ��Ҵ�ȡǮ�ɹ���
// 		SetDlgItemText(IDC_ETBREGPW, strMsg.GetBuffer());
// 		GetDlgItem(IDC_ETBREGPW)->ShowWindow(SW_SHOW);
// 	}

	CLabelUI* pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn4_Result"));
	pLabCtrl->SetText(strMsg);

	DUIOkeyMsgBox(m_hWnd,false,strMsg);

}

///ˢ�¼�¼��ҳ��
void CPBankWnd::UpdateRecordPageState()
{
	int iPageCount = ((m_mapRecord.size() % RECORD_PAGE_SIZE) == 0) ? 
		m_mapRecord.size() / RECORD_PAGE_SIZE : m_mapRecord.size() / RECORD_PAGE_SIZE + 1;
	int iCurrPage = m_iRecDispPos / RECORD_PAGE_SIZE + 1;

	CLabelUI* pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn3_4"));
	TCHAR szBuffer[32];
	_stprintf(szBuffer,_T("��%d/%dҳ"),iCurrPage,iPageCount);
	pLabCtrl->SetText(szBuffer);

	m_pFirstPageBnt->SetEnabled(iCurrPage > 1);
	m_pPrePageBnt->SetEnabled(iCurrPage > 1);
	m_pNextPageBnt->SetEnabled(iCurrPage < iPageCount);
	m_pRearPageBnt->SetEnabled(iCurrPage < iPageCount);
}

///
/// ��ʾ
/// @param pToCtrl ��ĳһ�ؼ�����ʾ
/// @param lpSecName ClientMessage.bcf�еĶ���
/// @param lpKeyName ClientMessage.bcf�еļ�ֵ
/// @param lpDefault ClientMessage.bcfû���ҵ���ֵʱ��ʹ�õ�Ĭ��ֵ
/// @return void
///
void CPBankWnd::ShowTipMessage(BzDui::CControlUI* pToCtrl,LPCTSTR lpSecName,LPCTSTR lpKeyName,LPCTSTR lpDefault,...)
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
		DUIOkeyMsgBox(CPBankWndEx::m_hInstanceWnd,false,"<y 25><p 5>%s</p></y>",szMessage);
	}

	return;

}

void CPBankWnd::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("click")) 
	{
		if (msg.pSender->GetName() == _T("Bnt_Page_1"))
		{
			ShowPage(0);
			return;
		}
		else if (msg.pSender->GetName() == _T("Bnt_Page_2"))
		{
			ShowPage(1);
			return;
		}
		else if (msg.pSender->GetName() == _T("Bnt_Page_3"))
		{
			ShowPage(2);
			OnBnClickedRecord();
			return;
		}
		else if (msg.pSender->GetName() == _T("Bnt_Page_4"))
		{
			ShowPage(3);
			return;
		}
		else if (msg.pSender->GetName() == _T("Bnt_Page_5"))
		{
			ShowPage(4);
			return;
		}
		else if (msg.pSender->GetName() == _T("Btn_Ctn2_OK"))
		{
			//ȷ��ת��
			OnBnClickedAccountOK();
			return;
		}
		else if (msg.pSender->GetName() == _T("Btn_Ctn4_All"))
		{
			//����ȡ��ȫ��
			OnBnClickedCoinAll();
			return;
		}
		else if (msg.pSender->GetName() == _T("Btn_Ctn4_OK"))
		{
			//ȷ����ȡ
			OnBnClickedBankOK();
			return;
		}
		else if (msg.pSender->GetName() == _T("Btn_Ctn3_Refresh"))
		{
			//ˢ��ת�˼�¼
			m_bTransferSuccess = TRUE;
			OnBnClickedRecord();
			return;
		}
		else if (msg.pSender->GetName() == _T("Bnt_Ctn3_First"))
		{
			//��¼������ҳ
			OnBnClickedRecordFirstPage();
			return;
		}
		else if (msg.pSender->GetName() == _T("Bnt_Ctn3_Previous"))
		{
			//��¼����һҳ
			OnBnClickedRecordPreviousPage();
			return;
		}
		else if (msg.pSender->GetName() == _T("Bnt_Ctn3_Next"))
		{
			//��¼����һҳ
			OnBnClickedRecordNextPage();
			return;
		}
		else if (msg.pSender->GetName() == _T("Bnt_Ctn3_Rear"))
		{
			//��¼����βҳ
			OnBnClickedRecordRearPage();
			return;
		}
		else if (msg.pSender->GetName() == _T("Btn_Ctn5_OK"))
		{
			//�޸���������
			OnBnClickedChPW();
			return;
		}
		else if(msg.pSender->GetName() == _T("Bnt_Close") ||
			msg.pSender->GetName() == _T("Btn_Ctn5_Close")) //�رհ�ť
		{
			Close();
			m_pGameRoom->PostMessage(WM_BANK_CLOSE,0,0);
			return;
		}
	}
	else if (msg.sType == _T("textchanged"))
	{
		if (msg.pSender->GetName() == _T("Edt_Ctn2_OutCoint"))
		{
			//ת�˽��Сдת��Ϊ���Ĵ�д
			OnEnChangeEtbconnoin();
			return;
		}
		else if (msg.pSender->GetName() == _T("Edt_Ctn4_OutCoints"))
		{
			//��ȡ����Сдת��Ϊ���Ĵ�д
			OnEnChangeMoney();
			return;
		}
	}
	else if(msg.sType == _T("selectchanged"))
	{
		if (msg.pSender->GetName() == _T("Opt_Ctn2_ID"))
		{
			//�л���Ŀ���û�ID���뷽ʽ
			OnBnClickedRTransId();
			return;
		}
		else if (msg.pSender->GetName() == _T("Opt_Ctn2_NickName"))
		{
			//�л���Ŀ���û��ǳ����뷽ʽ
			OnBnClickedRTransNn();
			return;
		}

		if (msg.pSender->GetName() == _T("Opt_Ctn4_In"))
		{
			//�л�������ģʽ
			OnBnClickedAccessIn();
			return;
		}
		else if (msg.pSender->GetName() == _T("Opt_Ctn4_Out"))
		{
			//�л���ȡ��ģʽ
			OnBnClickedAccessOut();
			return;
		}

	}
	else if(msg.sType == _T("setfocus"))
	{
		if (msg.pSender->GetName() == _T("Edt_Ctn2_TargetUser"))
		{
			//����ת��Ŀ���û�������һЩ���
			m_strFocusTemp = msg.pSender->GetText();
			return;
		}
	}
	else if(msg.sType == _T("killfocus"))
	{
// 		if (msg.pSender->GetName() == _T("Edt_Ctn2_TargetUser"))
// 		{
// 			//����ת��Ŀ���û�������һЩ���
// 			OnIDKillFocus();
// 			return;
// 		}

		if (msg.pSender->GetName() == _T("Edt_Ctn2_TargetUser"))
		{
			//Ŀ���û��༭��û���������ݻ�Ϊ�ն�������OnIDKillFocus
			OnKillfocusTargetUser();

			return;
		}

	}
	else if(msg.sType == _T("return"))
	{
		if (msg.pSender->GetName() == _T("Edt_Ctn2_TargetUser"))
		{
			CEditUI* pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn2_OutCoint"));
			pEdtCtrl->SetFocus();
			return;
		}

		if (msg.pSender->GetName() == _T("Edt_Ctn2_OutCoint"))
		{
			CEditUI* pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn2_Pwd"));
			pEdtCtrl->SetFocus();
			return;
		}
		
		if (msg.pSender->GetName() == _T("Edt_Ctn2_Pwd"))
		{
			//ȷ��ת��
			OnBnClickedAccountOK();
			return;
		}

		if (msg.pSender->GetName() == _T("Edt_Ctn4_OutCoints"))
		{
			CEditUI* pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn4_Pwd"));
			if (pEdtCtrl->IsVisible())
			{
				pEdtCtrl->SetFocus();
				return;
			}
			//ȷ����ȡ
			OnBnClickedBankOK();
			return;
		}

		if (msg.pSender->GetName() == _T("Edt_Ctn4_Pwd"))
		{
			//ȷ����ȡ
			OnBnClickedBankOK();
			return;
		}

		//�޸��������
		if (msg.pSender->GetName() == _T("Edt_Ctn5_OldPwd"))
		{
			CEditUI* pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn5_NewPwd"));
			pEdtCtrl->SetFocus();
			return;
		}

		if (msg.pSender->GetName() == _T("Edt_Ctn5_NewPwd"))
		{
			CEditUI* pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn5_PwdCfm"));
			pEdtCtrl->SetFocus();
			return;
		}

		if (msg.pSender->GetName() == _T("Edt_Ctn5_PwdCfm"))
		{
			//�޸���������
			OnBnClickedChPW();
			return;
		}


	}
}

void CPBankWnd::CenterWindow()
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
void CPBankWnd::LoadNormalCtrlSkin()
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
			if (!pEditUI->IsReadOnly())
			{
				pEditUI->SetNormalImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,0,500,22' corner='3,4,2,2'");
				pEditUI->SetHotImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,22,500,44' corner='3,4,2,2'");
				pEditUI->SetFocusedImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,44,500,66' corner='3,4,2,2'");
				pEditUI->SetDisabledImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,66,500,88' corner='3,4,2,2'");
			}
// 			else
// 			{
// 				pEditUI->SetNormalImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,66,500,88' corner='3,4,2,2'");
// 				pEditUI->SetHotImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,66,500,88' corner='3,4,2,2'");
// 				pEditUI->SetFocusedImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,66,500,88' corner='3,4,2,2'");
// 				pEditUI->SetDisabledImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,66,500,88' corner='3,4,2,2'");
// 			}
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

//��ʾ����ҳ��
void CPBankWnd::ShowPage(int nIndex)
{
	if (nIndex < 0 || nIndex >= PAGE_NUMS)
	{
		return;
	}

	CControlUI* pContainer = NULL;
	TCHAR szName[20];

	for (int i = 0; i < PAGE_NUMS; ++i)
	{
		m_pPageContainer[i]->SetVisible(i == nIndex);

		::sprintf_s(szName,sizeof(szName),_T("Bnt_Page_%d"),i + 1);
		pContainer = static_cast<CControlUI*>(m_pm.FindControl(szName));
		pContainer->SetEnabled(i != nIndex);

	}

	::sprintf_s(szName,sizeof(szName),_T("Bnt_Page_%d"),nIndex + 1);
	pContainer = static_cast<CControlUI*>(m_pm.FindControl(szName));
	SIZE sizeBnt = pContainer->GetFixedXY();

	pContainer = static_cast<CControlUI*>(m_pm.FindControl(_T("Img_ListBg")));
	SIZE size = {sizeBnt.cx,sizeBnt.cy - 22};
	pContainer->SetFixedXY(size);

	return;
}

//��ʼ���Ƹ���Ϣ
void CPBankWnd::InitMoneyInfo()
{
	CLabelUI* pLabCtrl = NULL;

	CBcfFile fMsg(CBcfFile::GetAppPath() + "ClientMessage.bcf");

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn1_1"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","UseDirectly","����Ϸ��ʹ�á�"));

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn1_2"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","TransInfor","�����еĽ�ң���������Ϸ��ֱ��ʹ�á�"));

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn1_3"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","NeedPassWord","�����еĴ�������֧��������ߵķ��á�"));

}

//��ʼ���Ͳ���UI
void CPBankWnd::InitGiftInfo()
{
	CLabelUI* pLabCtrl = NULL;
	CEditUI* pEdtCtrl = NULL;
	COptionUI* pOptCtrl = NULL;

	CBcfFile fMsg(CBcfFile::GetAppPath() + "ClientMessage.bcf");

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn2_1"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","UserCoin","���н�ң�"));

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn2_Bank"));
	//pEdtCtrl->SetEnabled(false);

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn2_2"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","TransCoinNum","���ͽ������"));

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn2_OutCoint"));
	pEdtCtrl->SetMaxChar(23);

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn2_3"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","NumToChinese","�����(��д)��"));

	pOptCtrl = static_cast<COptionUI*>(m_pm.FindControl("Opt_Ctn2_ID"));
	pOptCtrl->SetText(fMsg.GetKeyVal("PBankDlg","RTransUserID","�Է�ID"));
	pOptCtrl->Selected(true);
	if (pOptCtrl->IsSelected())
	{
		pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn2_4"));
		pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","TransUserID","�Է�ID��"));
	}

	m_bUseDestID = true;

	pOptCtrl = static_cast<COptionUI*>(m_pm.FindControl("Opt_Ctn2_NickName"));
	pOptCtrl->SetText(fMsg.GetKeyVal("PBankDlg","RTransUserNick","�Է��ǳ�"));
	if (pOptCtrl->IsSelected())
	{
		pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Opt_Ctn2_NickName"));
		pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","TransUserNick","�Է��ǳƣ�"));
	}
	
	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn2_5"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","SecondPassword","�������룺"));

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn2_TargetUser"));
	pEdtCtrl->SetMaxChar(32);

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn2_Pwd"));
	pEdtCtrl->SetMaxChar(32);

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn2_6"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","Tip","��ʾ��"));

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn2_Tip2"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","CoinToGameCoin","ת����۳�һ���������ѡ�"));

	m_iUseNickName = fMsg.GetKeyVal("PBankDlg","UseNickName",0);
}

//��ʼ��ȡ�����UI
void CPBankWnd::InitAccessInfo()
{
	CLabelUI*  pLabCtrl = NULL;
	CEditUI*   pEdtCtrl = NULL;
	COptionUI* pOptCtrl = NULL;
	CButtonUI* pBntCtrl = NULL;

	CBcfFile fMsg(CBcfFile::GetAppPath() + "ClientMessage.bcf");

	pOptCtrl = static_cast<COptionUI*>(m_pm.FindControl("Opt_Ctn4_In"));
	pOptCtrl->SetText(fMsg.GetKeyVal("PBankDlg","SaveGameCoin","������Ϸ��"));
	pOptCtrl->Selected(true);

	pOptCtrl = static_cast<COptionUI*>(m_pm.FindControl("Opt_Ctn4_Out"));
	pOptCtrl->SetText(fMsg.GetKeyVal("PBankDlg","GetGameCoin","ȡ����Ϸ��"));

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn4_5"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","InputGameCoin","�������ȡ��"));

	pBntCtrl = static_cast<CButtonUI*>(m_pm.FindControl("Btn_Ctn4_All"));
	pBntCtrl->SetText(fMsg.GetKeyVal("PBankDlg","CoinAll","ȫ��"));

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn4_6"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","InputPW","�������������룺"));

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn4_7"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","NumToChinese","�����(��д)��"));

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn4_OutCoints"));
	pEdtCtrl->SetMaxChar(23);

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn4_Pwd"));
	pEdtCtrl->SetMaxChar(32);

}

//��ʼ����־����UI
void CPBankWnd::InitRecordInfo()
{
	
}

//��ʼ���޸���������UI
void CPBankWnd::InitModifyPwdInfo()
{
	CLabelUI*  pLabCtrl = NULL;
	CEditUI*   pEdtCtrl = NULL;

	CBcfFile fMsg(CBcfFile::GetAppPath() + "ClientMessage.bcf");

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn5_1"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","OldPW","���������룺"));

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn5_2"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","NewPW","���������룺"));

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn5_3"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","ConfirmPW","ȷ�������룺"));

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn5_4"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","ChangeYourPW","���������ʼ��Ϊ��Ϸ��½���룬�뼰ʱ�޸ģ�"));

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn5_OldPwd"));
	pEdtCtrl->SetMaxChar(20);

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn5_NewPwd"));
	pEdtCtrl->SetMaxChar(20);

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn5_PwdCfm"));
	pEdtCtrl->SetMaxChar(20);
}

// lxl 2010-11-19  ת�˽��Сдת��Ϊ���Ĵ�д
void CPBankWnd::OnEnChangeEtbconnoin()
{
	CEditUI* pEdtCtrl = NULL;

	// ת�˽��ID	
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn2_OutCoint"));
	CString strOutCoin = pEdtCtrl->GetText();

	//ȥ��������еķָ�����
	if(Glb().m_bUseSpace)
		strOutCoin.Replace(Glb().m_strSpaceChar, "");	//ת��ҳ��		

	//��֤�ַ����Ƿ���0-9������
	for(int i = 0; i < strOutCoin.GetLength(); i++)
	{
		if(strOutCoin[i] < '0' || strOutCoin[i] > '9')
		{
			pEdtCtrl->SetText("");
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
		pEdtCtrl->SetText(szNum);
		//���ù�굽�����
		pEdtCtrl->SetSel(-1);
	}
	else
		bFlag = false;

	ChangeDigitToChinese(i64Num, strOutCoin);	

	CTextUI* pTxtCtrl = static_cast<CTextUI*>(m_pm.FindControl("Txt_Ctn2_Caps"));
	pTxtCtrl->SetText(strOutCoin);

	return;

}

//��ȡ����Сдת��Ϊ���Ĵ�д
void CPBankWnd::OnEnChangeMoney()
{
	CEditUI* pEdtCtrl = NULL;

	// ת�˽��ID	
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn4_OutCoints"));
	CString strGetCoin = pEdtCtrl->GetText();

	//ȥ��������еķָ�����
	if(Glb().m_bUseSpace)
		strGetCoin.Replace(Glb().m_strSpaceChar, "");	//����ҳ��

	//��֤�ַ����Ƿ���0-9������
	for(int i = 0; i < strGetCoin.GetLength(); i++)
	{
		if(strGetCoin[i] < '0' || strGetCoin[i] > '9')
		{
			pEdtCtrl->SetText("");
			return;
		}
	}	

	__int64 i64Num  = _tstoi64(strGetCoin.GetBuffer());
	TCHAR szNum[128];

	if(bFlag == false)	
	{		
		//�ַ����м���ָ���
		GetNumString(szNum, i64Num, 0, Glb().m_bUseSpace, Glb().m_strSpaceChar);
		bFlag = true;
		pEdtCtrl->SetText(szNum);
		//���ù�굽�����
		pEdtCtrl->SetSel(-1);
	}
	else 
		bFlag = false;

	ChangeDigitToChinese(i64Num, strGetCoin);

	CTextUI* pTxtCtrl = static_cast<CTextUI*>(m_pm.FindControl("Txt_Ctn4_Caps"));
	pTxtCtrl->SetText(strGetCoin);
}

//�л�Ŀ���û�����������
void CPBankWnd::OnBnClickedRTransId()
{
	CBcfFile fMsg(CBcfFile::GetAppPath() + "ClientMessage.bcf");
	CLabelUI* pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn2_4"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","TransUserID","�Է�ID��"));

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn2_NickNameID"));
	pLabCtrl->SetText("");

	CEditUI* pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn2_TargetUser"));
	pEdtCtrl->SetText("");
	pEdtCtrl->SetFocus();

	m_bUseDestID = true;
}

//�л�Ŀ���û�����������
void CPBankWnd::OnBnClickedRTransNn()
{
	CBcfFile fMsg(CBcfFile::GetAppPath() + "ClientMessage.bcf");
	CLabelUI* pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn2_4"));
	pLabCtrl->SetText(fMsg.GetKeyVal("PBankDlg","TransUserNick","�Է��ǳƣ�"));

	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn2_NickNameID"));
	pLabCtrl->SetText("");

	CEditUI* pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn2_TargetUser"));
	pEdtCtrl->SetText("");
	pEdtCtrl->SetFocus();

	m_bUseDestID = false;
}

//�л���ȡ���ͣ����
void CPBankWnd::OnBnClickedAccessIn()
{
	//����ʾ���������
	CLabelUI* pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn4_6"));
	pLabCtrl->SetVisible(false);

	CEditUI* pEditCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn4_Pwd"));
	pEditCtrl->SetVisible(false);

	m_uCheckInOrOut = 0;
}

//�л���ȡ���ͣ�ȡ��
void CPBankWnd::OnBnClickedAccessOut()
{
	//��ʾ���������
	CLabelUI* pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn4_6"));
	pLabCtrl->SetVisible(true);

	CEditUI* pEditCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn4_Pwd"));
	pEditCtrl->SetVisible(true);

	m_uCheckInOrOut = 1;
}

///
/// �ʺ�ת�� ��ȷ����ť��Ӧ����
/// @param void
/// @return void
void CPBankWnd::OnBnClickedAccountOK()
{
	// PengJiLin, 2010-9-27, ȫת ��ť�����«��������
// 	if (1&Glb().m_CenterServerPara.m_nFunction)
// 	{
// 		if(m_Check_TransAll.GetCheck())
// 		{
// 			return OnBnClickedTransAll();
// 		}
// 	}

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

	CEditUI* pEdtOutCoint = NULL;
	pEdtOutCoint = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn2_OutCoint"));

	CEditUI* pEdtPwd = NULL;
	pEdtPwd = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn2_Pwd"));

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
		//BZSoundPlay(this, "music/������ʾ.mp3", 0);
		ShowTipMessage(NULL,"BankDlg","NotInputCoin","��������Ҫ��ȡ������",
			Glb().m_nPowerOfGold+1, Glb().m_nPowerOfGold, szTail);
		pEdtOutCoint->SetFocus();
		return;
	}
	/// �������̫��
	if (strNum.GetLength()<Glb().m_nPowerOfGold)
	{
		//BZSoundPlay(this, "music/������ʾ.mp3", 0);
		ShowTipMessage(NULL,"BankDlg","NotGetCoin",
			"������Ľ�������ϵͳ���ܣ�����������%dλ����ĩ%dλΪ %s ����������",
			Glb().m_nPowerOfGold+1,Glb().m_nPowerOfGold, szTail);
		pEdtOutCoint->SetFocus();
		return;
	}
	/// �������̫��
	if (strNum.GetLength()>18+Glb().m_nPowerOfGold)
	{
		//BZSoundPlay(this, "music/������ʾ.mp3", 0);
		ShowTipMessage(NULL,"BankDlg","NumberTooLarge",
			"�����������̫�󣬳�������Χ�������벻����%I64d%s����ĩ%dλΪ %s ����������",
			0x3FFFFFFFFFFFFFFF,szTail,Glb().m_nPowerOfGold,szTail);
		pEdtOutCoint->SetFocus();
		return;
	}

	/// ���ĩ��λ������
	CString strSubNum = strNum.Right(Glb().m_nPowerOfGold);
	if (strSubNum != szTail)
	{
		//BZSoundPlay(this, "music/������ʾ.mp3", 0);
		ShowTipMessage(NULL,"BankDlg","NotGetCoin2",
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
		//BZSoundPlay(this, "music/������ʾ.mp3", 0);
		ShowTipMessage(NULL,"BankDlg","NumberTooLarge",
			"�����������̫�󣬳�������Χ�������벻����%I64d%s����ĩ%dλΪ %s ����������",
			0x3FFFFFFFFFFFFFFF,szTail,Glb().m_nPowerOfGold,szTail);
		pEdtOutCoint->SetFocus();
		return;
	}

	//���Ŀ���û�
	pEdtOutCoint = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn2_TargetUser"));
	strNum = pEdtOutCoint->GetText();

	//��������
	pEdtOutCoint = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn2_Pwd"));
	str = pEdtOutCoint->GetText();

	if (str.GetLength()<1)
	{
		//BZSoundPlay(this, "music/������ʾ.mp3", 0);
		ShowTipMessage(NULL,"BankDlg","PasswdBytes","���������룡");
		pEdtPwd->SetFocus();
		return;
	}
	if (i64Num<=0)
	{
		//BZSoundPlay(this, "music/������ʾ.mp3", 0);
		ShowTipMessage(NULL,"BankDlg","NotGetCoin",
			"������Ľ�������ϵͳ���ܣ�����������%dλ����ĩ%dλΪ %s ����������",
			Glb().m_nPowerOfGold+1,Glb().m_nPowerOfGold,szTail);
		pEdtOutCoint->SetFocus();
		return;
	}

	if (0 == atol(strNum) && m_bUseDestID)
	{
		//BZSoundPlay(this, "music/������ʾ.mp3", 0);
		ShowTipMessage(NULL,"BankDlg","WrongID","��������ȷ�����ID��");
		CEditUI* pEdtTargetUser = NULL;
		pEdtTargetUser = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn2_TargetUser"));
		pEdtTargetUser->SetFocus();
		return;
	}

	TransferMoney_t *pTransferMoney = new TransferMoney_t();
	pTransferMoney->i64Money = i64Num;
	pTransferMoney->uDestUserID = atol(strNum);
	_tcscpy(pTransferMoney->szPWD, str.Left(20).GetBuffer());
	if (m_bUseDestID)
	{
		_tcscpy(pTransferMoney->szDestNickName, m_strNickName);
	}
	else
	{
		_tcscpy(pTransferMoney->szDestNickName, strNum.GetBuffer());
	}


	pTransferMoney->bUseDestID = m_bUseDestID;

	CLabelUI* pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn2_NickNameID_temp"));

	/// ��ʾ�û�ȷ��ת��
	TCHAR szNum[128];
	GetNumString(szNum, pTransferMoney->i64Money, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
	//int iUseNickName = fMsg.GetKeyVal("PBankDlg","UseNickName",0);

	CString strFormat;
	CString test;
	test.Format("lxl is use nick name : %d",m_iUseNickName );
	OutputDebugString(test);

	if(m_bUseDestID)
	{
		//BZSoundPlay(this, "music/������ʾ.mp3", 0);
		strFormat = fMsg.GetKeyVal("PBankDlg", "TransferConfirmId", 
			"<p 5>ת��ǰ��ȷ������ת����Ϣ���Ƿ�ȷ��ת�ʣ�<n><y 30>Ŀ���û���<b>%s</b> (ID��<b>%d</b>)</y><n>ת�����<b>%s</b> ���<n></p>");
		wsprintf( szMsg, strFormat,pLabCtrl->GetText().GetData(),pTransferMoney->uDestUserID,szNum);
	}
	else
	{
		//BZSoundPlay(this, "music/������ʾ.mp3", 0);
		// lxl, 2010-11-17, �����ǳ���ʾ

		if (::atol(pLabCtrl->GetText().GetData()) == m_dwOpUserID)
		{
			DUIOkeyMsgBox(AfxGetMainWnd()->m_hWnd,false,
				fMsg.GetKeyVal("BankDlg","TransferToSelf","������ת�ʸ��Լ����Բ���ϵͳ�ݲ��ṩ�������"));
			return;
		}

		strFormat = fMsg.GetKeyVal("PBankDlg", "TransferConfirmNn", 
			"<p 5>ת��ǰ��ȷ������ת����Ϣ���Ƿ�ȷ��ת�ʣ�<n><y 30>Ŀ���û���<b>%s</b></y><n>ת�����<b>%s</b> ���<n></p>");
		wsprintf( szMsg,strFormat,pLabCtrl->GetText().GetData(),szNum );
	}	

	if (IDYES != DUIMessageBox(m_hWnd,MB_YESNO|MB_ICONQUESTION,"����",true,szMsg))
	{
		//BZSoundPlay(this, "music/������ʾ.mp3", 0);
		delete pTransferMoney;
		pTransferMoney = NULL;
		return;
	}

	// PengJiLin, 2010-8-23, �˴���Ϊ�Ѿ�ת�˳ɹ�������ˢ���б�
	m_bTransferSuccess = true;

	//��������
	pEdtPwd->SetText("");
	pEdtPwd->SetFocus();


	m_pGameRoom->PostMessage(WM_TRANSFER_MONEY, 0, LPARAM(pTransferMoney));
}

//�����û�ID��ȡ�ǳ�,������û��ǳƻ�ȡ�û�ID
// @param pEdtCtrl ��ID/�ǳ�����ؼ�
// @param nType �������ͣ���ID��ȡ�ǳ�,�����ǳ��û�ID
// @param nUserful ��;����ָ������������г��ϣ���ת�ˣ���ֵ�ȡ�
// @return bool �Ƿ�����ɹ�
// JianGuankun, 2012-7-30
bool CPBankWnd::OnIDKillFocus(BzDui::CEditUI* pEdtCtrl,BYTE nType,BYTE nUserful)
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

///
/// �յ�һ��ת�˼�¼
/// @param void
/// @return void
///
void CPBankWnd::OnReceivedTransferRecord(MSG_GR_R_TransferRecord_t *pRecord)
{
	m_mapRecord[pRecord->nSeriNo] = *pRecord;
	m_pRecordList->NeedUpdate();

	UpdateRecordPageState();
}

//��¼������ҳ
void CPBankWnd::OnBnClickedRecordFirstPage()
{
	m_iRecDispPos = 0;
	m_pRecordList->NeedUpdate();
	UpdateRecordPageState();
}

//��¼����βҳ
void CPBankWnd::OnBnClickedRecordRearPage()
{
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
}

//��¼����һҳ
void CPBankWnd::OnBnClickedRecordPreviousPage()
{
	if (m_iRecDispPos - RECORD_PAGE_SIZE >= 0)
	{
		m_iRecDispPos -= RECORD_PAGE_SIZE;
		m_pRecordList->NeedUpdate();
		UpdateRecordPageState();
	}
}

//��¼����һҳ
void CPBankWnd::OnBnClickedRecordNextPage()
{
	if (m_iRecDispPos + RECORD_PAGE_SIZE < m_mapRecord.size())
	{
		m_iRecDispPos += RECORD_PAGE_SIZE;
		m_pRecordList->NeedUpdate();
		UpdateRecordPageState();
	}
}

///
/// �޸����� ��ȷ����ť��Ӧ����
/// @param void
/// @return void
///
void CPBankWnd::OnBnClickedChPW()
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

	CEditUI* pOldPwdEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn5_OldPwd"));
	CEditUI* pNewPwdEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn5_NewPwd"));
	CEditUI* pPwdCfmEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn5_PwdCfm"));

	CString strOld, strNew, strNewCfm, str;
	TCHAR szMsg[MAX_PATH];

	strOld = pOldPwdEdt->GetText();//������

	if ((strOld.GetLength()<6) ||(strOld.GetLength()>20))
	{
		//BZSoundPlay(this, "music/������ʾ.mp3", 0);
		ShowTipMessage(NULL,"BankDlg","NewPasswdBytes","������6-20�ַ���ɣ���������������룡");
		pOldPwdEdt->SetFocus();
		return;
	}

	/// �ȱȽ��������ȷ�������Ƿ���ͬ�������ͬ����ֱ�ӷ��أ���ͬ�ż���
	strNew = pNewPwdEdt->GetText();//������
	strNewCfm = pPwdCfmEdt->GetText();//ȷ��������
	if (strNew != strNewCfm)
	{
		//BZSoundPlay(this, "music/������ʾ.mp3", 0);
		ShowTipMessage(NULL,"BankDlg","ConfirmPasswdNotSame","�������������벻һ�£�����������!");
		pNewPwdEdt->SetFocus();
		return;
	}
	if ((strNew.GetLength()<6) ||(strNew.GetLength()>20))
	{
		//BZSoundPlay(this, "music/������ʾ.mp3", 0);
		ShowTipMessage(NULL,"BankDlg","NewPasswdBytes","������6-20�ַ���ɣ����������������룡");
		pNewPwdEdt->SetFocus();
		return;
	}

	if (ISGameRoom == m_emParWndType)
	{
		ChangePasswd_t *pChangePasswd = new ChangePasswd_t();
		_tcscpy( pChangePasswd->szPwdOld, strOld.Left(20).GetBuffer() );
		_tcscpy( pChangePasswd->szPwdNew, strNew.Left(20).GetBuffer() );

		m_pGameRoom->PostMessage(WM_CHANGE_PASSWD, 0, LPARAM(pChangePasswd));
	}
	else if (IsMainRoom == m_emParWndType)
	{
		TMSG_GP_BankChPwd* pChPwd = new TMSG_GP_BankChPwd();
		pChPwd->_user_id = m_dwOpUserID;

		CGameRoomEx::GetCryptedPasswd(pChPwd->_MD5Pass_old,(char*)strOld.Left(20).GetBuffer());
		CGameRoomEx::GetCryptedPasswd(pChPwd->_MD5Pass_new,(char*)strNew.Left(20).GetBuffer());

		GetMainRoom()->PostMessage(WM_CHANGE_PASSWD,0,LPARAM(pChPwd));
	}

	pOldPwdEdt->SetText("");//������
	pNewPwdEdt->SetText("");//������
	pPwdCfmEdt->SetText("");//ȷ��������

	return;
}

///
/// ���� ��ȷ����ť��Ӧ����
/// ���жϣ�������ȡǮ���Ǵ�Ǯ
/// @param void
/// @return void
///
void CPBankWnd::OnBnClickedBankOK()
{
	
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

	CEditUI* pOutCointEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn4_OutCoints"));
	CEditUI* pPwdEdt = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn4_Pwd"));

	/// ȡ�ñ༭������ַ���
	TCHAR szMsg[256];
	CString strNum, str;
	strNum = pOutCointEdt->GetText();

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
		//BZSoundPlay(this, "music/������ʾ.mp3", 0);
		ShowTipMessage(NULL,"BankDlg","NotInputCoin","��������Ҫ��ȡ������",
			Glb().m_nPowerOfGold+1,Glb().m_nPowerOfGold,szTail);
		pOutCointEdt->SetFocus();
		return;
	}
	/// �������̫��
	if (strNum.GetLength()<Glb().m_nPowerOfGold)
	{
		//BZSoundPlay(this, "music/������ʾ.mp3", 0);
		ShowTipMessage(NULL,"BankDlg","NotGetCoin",
			"������Ľ�������ϵͳ���ܣ�����������%dλ����ĩ%dλΪ %s ����������",
			Glb().m_nPowerOfGold+1,Glb().m_nPowerOfGold,szTail);
		pOutCointEdt->SetFocus();
		return;
	}
	/// �������̫��
	if (strNum.GetLength()>18 + Glb().m_nPowerOfGold)
	{
		//BZSoundPlay(this, "music/������ʾ.mp3", 0);
		ShowTipMessage(NULL,"BankDlg","NumberTooLarge",
			"�����������̫�󣬳�������Χ�������벻����%I64d%s����ĩ%dλΪ %s ����������",
			0x3FFFFFFFFFFFFFFF,szTail,Glb().m_nPowerOfGold,szTail);
		pOutCointEdt->SetFocus();
		return;
	}

	/// ���ĩ��λ������
	CString strSubNum = strNum.Right(Glb().m_nPowerOfGold);
	if (strSubNum != szTail)
	{
		//BZSoundPlay(this, "music/������ʾ.mp3", 0);
		ShowTipMessage(NULL,"BankDlg","NotGetCoin2",
			"������Ľ�������ϵͳ���ܣ�������ĩ%dλΪ %s ����������",
			Glb().m_nPowerOfGold,szTail);
		pOutCointEdt->SetFocus();
		return;
	}
	/// ��ȡǰNλ
	strSubNum = strNum.Left(strNum.GetLength()-Glb().m_nPowerOfGold);
	__int64 i64Num = _atoi64(strSubNum.GetBuffer());
	if (i64Num>0x3FFFFFFFFFFFFFFF)
	{
		//BZSoundPlay(this, "music/������ʾ.mp3", 0);
		ShowTipMessage(NULL,"BankDlg","NumberTooLarge",
			"�����������̫�󣬳�������Χ�������벻����%I64d%s����ĩ%dλΪ %s ����������",
			0x3FFFFFFFFFFFFFFF,szTail,Glb().m_nPowerOfGold,szTail);
		pOutCointEdt->SetFocus();
		return;
	}
	if (i64Num<=0)
	{
		//BZSoundPlay(this, "music/������ʾ.mp3", 0);
		ShowTipMessage(NULL,"BankDlg","NotGetCoin",
			"������Ľ�������ϵͳ���ܣ�����������%dλ����ĩ%dλΪ %s ����������",
			Glb().m_nPowerOfGold+1,Glb().m_nPowerOfGold,szTail);
		pOutCointEdt->SetFocus();
		return;
	}

	/// ����Ϣ
	if (m_pGameRoom!=NULL)
	{
		/// ������ȡ���Ǵ棬��ֻ���������ڷ���һ����Ϣ����
		/// Ϊͳһ�ӿڣ���ǮҲ���������뷢�͵�������
		/// ���������룬ȡ���Ǵ�
		str = pPwdEdt->GetText();

		if ((m_uCheckInOrOut==1) && (str.GetLength()<1))
		{
			//BZSoundPlay(this, "music/������ʾ.mp3", 0);
			ShowTipMessage(NULL,"BankDlg","PasswdBytes","���������룡");
			pPwdEdt->SetFocus();
			return;
		}

		//��������Ϸ�������ʱ�Ĵ���
		if (ISGameRoom == m_emParWndType)
		{
			CheckMoney_t *pCheckMoney = new CheckMoney_t;
			pCheckMoney->uType = m_uCheckInOrOut;
			pCheckMoney->i64Money = i64Num;
			_tcscpy(pCheckMoney->szPWD, str.Left(20).GetBuffer());
			m_pGameRoom->PostMessage(WM_CHECK_MONEY, 0, (LPARAM)pCheckMoney);
		}

		//�����ڴ�������ʱ�Ĵ���
		else if (IsMainRoom == m_emParWndType)
		{
			bank_op_normal* p = new bank_op_normal;
			p->_user_id = m_dwOpUserID;
			p->_game_id = 0;
			p->_money = i64Num;

			if (m_uCheckInOrOut == 1) //ȡ
			{
				p->_operate_type = 1;
				//��ȡ���������
				CGameRoomEx::GetCryptedPasswd(p->_szMD5Pass,(TCHAR*)str.GetBuffer());

				m_pGameRoom->PostMessage(WM_BANK,0,(LPARAM)p);
			}
			else //��
			{
				p->_operate_type = 2;
				m_pGameRoom->PostMessage(WM_BANK,0,(LPARAM)p);
			}
		}
		
	}
}

//�����û�ID/�ǳ�
void CPBankWnd::OnKillfocusTargetUser()
{
	CLabelUI* pLabCtrl = NULL;
	CEditUI* pEdtCtrl = NULL;

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn2_TargetUser"));

	if (pEdtCtrl->GetText().GetLength() == 0)
	{
		pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn2_NickNameID_temp"));
		pLabCtrl->SetText("");

		pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn2_NickNameID"));
		pLabCtrl->SetText("");
	}
	else
	{
		//����ת��Ŀ���û�������һЩ���
		if (pEdtCtrl->GetText().Compare(m_strFocusTemp))
		{
			pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Ctn2_NickNameID"));
			pLabCtrl->SetText("���ڲ�ѯ...");

			COptionUI* pOptCtrl = static_cast<COptionUI*>(m_pm.FindControl("Opt_Ctn2_ID"));
			if(!OnIDKillFocus(pEdtCtrl,(pOptCtrl->IsSelected()?NICK_ID_BYID:NICK_ID_BYNICKNAME),NICK_ID_USE_TO_TRANSFER))
			{
				pLabCtrl->SetText("(��Ҳ�����)");
			}
		}

		m_strFocusTemp = "";
	}
}

void CPBankWnd::OnBnClickedCoinAll()
{
	TCHAR szNum[128];
	if(m_uCheckInOrOut == 0)
	{
		int nLess = 0;
		if(m_pGameRoom != NULL)
		{
			nLess = ((CGameRoomEx*)m_pGameRoom)->m_GameInfo.uLessPoint;
		}
		GetNumString(szNum, m_i64WalletMoney-nLess, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
	}
	else
	{
		GetNumString(szNum, m_i64BankMoney, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
	}

	CEditUI* pEdtCtrl = NULL;
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn4_OutCoints"));
	pEdtCtrl->SetFocus();
	pEdtCtrl->SetText(szNum);

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn4_Pwd"));
	pEdtCtrl->SetFocus();

}

void CPBankWnd::OnBnClickedRecord()
{
	// PengJiLin, 2010-8-23, ת�˳ɹ�����ˢ��
	if(m_bTransferSuccess)
	{
		RefreshRecordList();
		m_bTransferSuccess = false;

		m_pFirstPageBnt->SetEnabled(false);
		m_pPrePageBnt->SetEnabled(false);
		m_pNextPageBnt->SetEnabled(false);
		m_pRearPageBnt->SetEnabled(false);
	}
}

///
/// ˢ��ת�˼�¼
/// @return void
///
void CPBankWnd::RefreshRecordList()
{
	m_mapRecord.clear();
	m_iRecDispPos = 0;
	m_pGameRoom->PostMessage(WM_TRANSFER_RECORD, 0, 0);
}
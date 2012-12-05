#include "stdafx.h"
#include "LoadSelection.h"
#include "MiniGameEngine.h"

extern TCHAR *GlbGetFloatString(TCHAR pRet[], int nValue); 

#define IDT_NOTICES		900


BEGIN_MESSAGE_MAP(CLoadSelectionDlg, BModalContainer)
	ON_WM_LBUTTONUP()
	ON_EN_CHANGE(IDC_PLAN_MONEY, &CLoadSelectionDlg::OnEnChangeEdit)
	ON_WM_TIMER()
END_MESSAGE_MAP()


CLoadSelectionDlg::CLoadSelectionDlg(DWORD pId,CWnd* pParent)
	: BModalContainer(pId, pParent)
{
	m_pWndParent = pParent;
	m_bGameLoadSelection = false;
	m_strEdit = "";
}

void CLoadSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	BModalContainer::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PLAN_MONEY, m_strEdit);
}


CLoadSelectionDlg::~CLoadSelectionDlg()
{

}

/**
 * ����ǰ�������еĿؼ���Ϣ
 *
 * @param pId Ҫ����Ŀؼ�
 * @param pParam Ҫ������¼���Ϣ
 */
bool CLoadSelectionDlg::OnPostMessage(UINT pId, BBaseParameter* pParam)
{
	switch(pId)
	{
	case IDB_OK_LOAD:
		{
			CTrueEdit * pEdit = dynamic_cast<CTrueEdit *>(GetControlById(IDC_PLAN_MONEY));
			if (pEdit != NULL)
			{
				// ��ȡ��ǰ������
				CString strMoney;
				pEdit->GetWindowText(strMoney);
				int nCurrentMoney = atoi(strMoney);

				if (m_subMoney.nCurrentMoney <m_subMoney.nMinMoney ||  m_subMoney.nCurrentMoney > m_subMoney.nMaxMoney)
				{
					m_subMoney.nCurrentMoney = m_subMoney.nMinMoney;
					m_subMoney.nSelfMoney = m_nOldSelfMoney - m_subMoney.nCurrentMoney;
				}

				m_subMoney.nCurrentMoney = atoi(strMoney);
				
				CMiniGameEngine* pGameDlg = (CMiniGameEngine*)m_pWndParent;
				EndModalLoop(EVENT_CONTINUE_MAINFRAME);
				pGameDlg->PlayBaseSound(DZPKSound::BT_OK);
			}
		}
		break;
	
	case IDB_EXIT_GAME:
		{
			EndModalLoop(EVENT_CONTINUE_MAINFRAME);
			CMiniGameEngine* pGameDlg = (CMiniGameEngine*)m_pWndParent;
			pGameDlg->OnCancel();
		}
		break;
	case IDB_ADD:
		{
			CTrueEdit * pEdit = dynamic_cast<CTrueEdit *>(GetControlById(IDC_PLAN_MONEY));
			CTransparentStatic * pStatic = dynamic_cast<CTransparentStatic *>(GetControlById(IDC_TOTAL_MONEY));

			if (pEdit == NULL || pStatic == NULL)
			{
				AfxMessageBox("IDC_PLAN_MONEY OR IDC_TOTAL_MONEY �ؼ�������");
				break;
			}

			// ��ȡ��ǰ������
			CString strMoney;
			pEdit->GetWindowText(strMoney);
			m_subMoney.nCurrentMoney = atoi(strMoney);

			if ((m_subMoney.nCurrentMoney + m_subMoney.nMinusMoney) > m_subMoney.nMaxMoney)
			{
				m_subMoney.nCurrentMoney = m_subMoney.nMaxMoney;
				m_subMoney.nSelfMoney = m_nOldSelfMoney - m_subMoney.nCurrentMoney;
			}	
			else
			{
				m_subMoney.nCurrentMoney += m_subMoney.nMinusMoney;
				m_subMoney.nSelfMoney -= m_subMoney.nMinusMoney;
			}
			
			// ��ֹ�����ҳ���������
			if (m_subMoney.nCurrentMoney > m_nOldSelfMoney)
			{
				m_subMoney.nCurrentMoney = m_nOldSelfMoney;
				m_subMoney.nSelfMoney = 0;
			}

			// ���õ�ǰ������
			strMoney.Format("%d",m_subMoney.nCurrentMoney);
			pEdit->SetWindowText(strMoney.GetBuffer());
			pEdit->ShowWindow(SW_SHOW);

			// �����Լ�������
			strMoney.Format("%d", m_subMoney.nSelfMoney);
			pStatic->SetMyText(strMoney.GetBuffer());
			pStatic->ShowWindow(SW_SHOW);
		}
		break;
	case IDB_SUB:
		{
			CTrueEdit * pEdit = dynamic_cast<CTrueEdit *>(GetControlById(IDC_PLAN_MONEY));
			CTransparentStatic * pStatic = dynamic_cast<CTransparentStatic *>(GetControlById(IDC_TOTAL_MONEY));

			if (pEdit == NULL || pStatic == NULL)
			{
				AfxMessageBox("IDC_PLAN_MONEY OR IDC_TOTAL_MONEY �ؼ�������");
				break;
			}

			// ��ȡ��ǰ������
			CString strMoney;
			pEdit->GetWindowText(strMoney);
			m_subMoney.nCurrentMoney = atoi(strMoney);

			if ((m_subMoney.nCurrentMoney - m_subMoney.nMinusMoney) < m_subMoney.nMinMoney)
			{
				m_subMoney.nCurrentMoney = m_subMoney.nMinMoney;
				m_subMoney.nSelfMoney = m_nOldSelfMoney - m_subMoney.nCurrentMoney;
			}	
			else
			{
				m_subMoney.nCurrentMoney -= m_subMoney.nMinusMoney;
				m_subMoney.nSelfMoney += m_subMoney.nMinusMoney;
			}

			// ���õ�ǰ������
			strMoney.Format("%d",m_subMoney.nCurrentMoney);
			pEdit->SetWindowText(strMoney.GetBuffer());
			pEdit->ShowWindow(SW_SHOW);

			// �����Լ�������
			strMoney.Format("%d", m_subMoney.nSelfMoney);
			pStatic->SetMyText(strMoney.GetBuffer());
			pStatic->ShowWindow(SW_SHOW);
		}
		break;
	case IDB_STOP:
		{
			EndModalLoop(EVENT_CLOSE_MAINFRAME);
		}
		break;
	}
	return __super::OnPostMessage(pId,pParam);
}

/// ���ضԻ���
bool CLoadSelectionDlg::LoadConfig(std::string path)
{
	return __super::LoadConfig(path);
}

// д����
void CLoadSelectionDlg::WriteMoney()
{
	
	return;
}

void CLoadSelectionDlg::SetNotice(std::string str,bool bShowNextBnt)
{
	if (bShowNextBnt)
	{
		ShowControl(EVENT_CONTINUE_MAINFRAME,SW_SHOW);
	}
	else
	{
		ShowControl(EVENT_CONTINUE_MAINFRAME,SW_HIDE);
	}

	CTransparentStatic* pStatic = dynamic_cast<CTransparentStatic*>(GetControlById(IDT_NOTICES));

	if (pStatic)
	{
		pStatic->SetMyText(str);
		pStatic->ShowWindow(SW_SHOW);
	}

	return;
}

//���óɷ�������Ϸ��LoadSelection��״̬
void CLoadSelectionDlg::SetGameLoadSelection(bool bLoadSelection)
{
	m_bGameLoadSelection = bLoadSelection;
	return;
}

void CLoadSelectionDlg::SetMessage(SubMoney stData)
{
	CString strTotalMoney;
	CTransparentStatic* pStatic = NULL;
	CTransparentStatic* pStatic1 = NULL;
	CTransparentStatic* pStatic2 = NULL;
	CTransparentStatic* pStatic3 = NULL;


	m_subMoney = stData;
	m_nOldSelfMoney = m_subMoney.nSelfMoney;
	
	//if (m_subMoney.nSelfMoney < m_subMoney.nMaxMoney)
	//{
	//	// ��ֹ���û���������
	//	m_subMoney.nCurrentMoney = m_subMoney.nSelfMoney;
	//}

	//if (m_subMoney.nSelfMoney < m_subMoney.nMinMoney)
	//{
	//	m_subMoney.nCurrentMoney = m_subMoney.nSelfMoney;
	//}
	
	// ��ֹ��ұ����ң����ڴ�����
	if (m_subMoney.nSelfMoney < m_subMoney.nCurrentMoney)
	{
		m_subMoney.nCurrentMoney = m_subMoney.nSelfMoney;
	}

	m_subMoney.nSelfMoney = m_nOldSelfMoney - m_subMoney.nCurrentMoney;
	
	ShowAllControl(SW_SHOW);
	ShowControl(IDT_NOTICES,SW_HIDE);
	pStatic = dynamic_cast<CTransparentStatic *>(GetControlById(IDC_TOTAL_MONEY));
	pStatic1 = dynamic_cast<CTransparentStatic *>(GetControlById(IDC_MAX_MONEY));
	pStatic2 = dynamic_cast<CTransparentStatic *>(GetControlById(IDC_MIN_MONEY));
	CTrueEdit *pEdit = dynamic_cast<CTrueEdit	*>(GetControlById(IDC_PLAN_MONEY));

	// ��ʾ�Լ�������
	if (pStatic != NULL)
	{
		strTotalMoney.Format("%d", m_subMoney.nSelfMoney);
		pStatic->SetMyText(strTotalMoney.GetBuffer());
		pStatic->ShowWindow(SW_SHOW);
	}
	// ��ʾ��ߴ�����
	if (pStatic1 != NULL)
	{
		strTotalMoney.Format("%d", m_subMoney.nMaxMoney);
		pStatic1->SetMyText(strTotalMoney.GetBuffer());
		pStatic1->ShowWindow(SW_SHOW);		
	}
	// ��ʾ��ʹ�����
	if (pStatic2 != NULL)
	{
		strTotalMoney.Format("%d", m_subMoney.nMinMoney);
		pStatic2->SetMyText(strTotalMoney.GetBuffer());
		pStatic2->ShowWindow(SW_SHOW);
	}	

	// ��ʾ׼��������
	if (pEdit != NULL)
	{
		// ���ñ༭��ֻ����������
		DWORD dwStyle = pEdit->GetStyle(); 
		dwStyle |= ES_NUMBER; 
		SetWindowLong(pEdit->m_hWnd, GWL_STYLE, dwStyle);

		strTotalMoney.Format("%d", m_subMoney.nCurrentMoney);
		pEdit->SetWindowText(strTotalMoney.GetBuffer());
		pEdit->ShowWindow(SW_SHOW);
	}
	
	if (m_subMoney.bAuto)
	{
		// ���ö�ʱȷ������
		SetAutoStart();
	}
	return;
}

void CLoadSelectionDlg::OnEnChangeEdit()
{
	CTrueEdit *pEdit = dynamic_cast<CTrueEdit *>(GetControlById(IDC_PLAN_MONEY));
	if (pEdit == NULL)
	{
		return ;
	}
	
	CString strMoney;
	pEdit->GetWindowText(strMoney);
	int nCurrentMoney = atoi(strMoney);

	// ������ݲ���Ⱦ͸���
	m_subMoney.nCurrentMoney = nCurrentMoney;
	if (m_subMoney.nCurrentMoney < m_subMoney.nMinMoney)
	{
		m_subMoney.nCurrentMoney = m_subMoney.nMinMoney;
	}
	else if (m_subMoney.nCurrentMoney > m_subMoney.nMaxMoney)
	{
		m_subMoney.nCurrentMoney = m_subMoney.nMaxMoney;
	}

	// ��ֹ�����ҳ���������
	if (m_subMoney.nCurrentMoney > m_nOldSelfMoney)
	{
		m_subMoney.nCurrentMoney = m_nOldSelfMoney;
		m_subMoney.nSelfMoney = 0;
	}
	else
	{
		m_subMoney.nSelfMoney = m_nOldSelfMoney - m_subMoney.nCurrentMoney;
	}

	// ���ô�����
	strMoney.Format("%d", m_subMoney.nCurrentMoney);

	m_strEdit = strMoney;
	UpdateData(FALSE);  // ʹ�� pEdit->SetWindowText �����, ���Բ���CString m_strEdit�󶨶���ʹ��
	//pEdit->SetWindowText(strMoney);

	// ���ù��λ��
	pEdit->SetSel(m_strEdit.GetLength(), m_strEdit.GetLength());

	CTransparentStatic * pStatic = dynamic_cast<CTransparentStatic *>(GetControlById(IDC_TOTAL_MONEY));
	if (pStatic != NULL)
	{
		// �����Լ�������
		strMoney.Format("%d", m_subMoney.nSelfMoney);
		pStatic->SetMyText(strMoney.GetBuffer());
		pStatic->ShowWindow(SW_SHOW);
	}
}

void CLoadSelectionDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (IDT_MODIFY_MONEY == nIDEvent)
	{
		KillTimer(IDT_MODIFY_MONEY);

		// ��ʱ����ȷ�ϴ��밴ť
		OnPostMessage(IDB_OK_LOAD, NULL);
	}
	BModalContainer::OnTimer(nIDEvent);
}

// �Զ���ʼ
void CLoadSelectionDlg::SetAutoStart()
{
	KillTimer(IDT_MODIFY_MONEY);
	SetTimer(IDT_MODIFY_MONEY, 5000, 0);
}
#include "StdAfx.h"
#include "UserInfoWnd.h"
#include "MainRoomEx.h"
#include "MyDiskInfo.h"

#define PAGE_NUMS				5	//Tabҳ������
#define PAGE_LOCK_MACHINE		2	//"��������" Tabҳ����
#define PAGE_BIND_MOBILE		3	//"�ֻ���" Tabҳ����

#define IDT_RE_GET_MOBILE_VCODE_1	1 //��ȡ���ֻ���֤��1
#define IDT_RE_GET_MOBILE_VCODE_2	2 //��ȡ���ֻ���֤��2
#define IDT_RE_GET_MOBILE_VCODE_3	3 //��ȡ���ֻ���֤��3

#define RE_VCODE_TIME	80	//��ȡ�ֻ���֤���ʱ��

using namespace BzDui;

typedef map<char*,vector<char*>> AREA_MAP;

//ʡ�ݵ�����ϵ
AREA_MAP CUserInfoWnd::m_mapArea;

//��ǰʵ�����ھ��
HWND CUserInfoWnd::m_hInstanceWnd = NULL;

CUserInfoWnd::CUserInfoWnd()
{
	m_pUserInfo = NULL;

	//Ĭ����ʾ��ҳ����
	m_nDefaultPage = 0;

	//Ĭ����ʾ����ҳ����
	m_nDefaultSubPage = 0;

	//��ȡ�ֻ���֤���ʣ��ʱ��
	m_iReVCodeTime = CMobileVerifyWnd::GetVCodeValidTime();

}

CUserInfoWnd::~CUserInfoWnd(void)
{
}

LPCTSTR CUserInfoWnd::GetWindowClassName() const 
{ 
	return _T("UIFrame");
};

UINT CUserInfoWnd::GetClassStyle() const 
{ 
	return UI_CLASSSTYLE_DIALOG; 
};

void CUserInfoWnd::OnFinalMessage(HWND hWnd) 
{ 
	m_pm.RemovePreMessageFilter(this);
};

LRESULT CUserInfoWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);

	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("left\\uinfo\\UserInfoWnd.xml"), (UINT)0, NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	::SetWindowPos(m_hWnd,CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);

	CUserInfoWnd::m_hInstanceWnd = m_hWnd;

	Init();
	return 0;
}

LRESULT CUserInfoWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUserInfoWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CUserInfoWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CUserInfoWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CUserInfoWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CUserInfoWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CUserInfoWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//��ȡ�ֻ���֤�밴ť1����ʱ(�޸��û���ϸ��Ϣtabҳ��)
	if (wParam == IDT_RE_GET_MOBILE_VCODE_1)
	{
		if (m_iReVCodeTime == 0)
		{
			::KillTimer(m_hWnd,IDT_RE_GET_MOBILE_VCODE_1);
			CMainRoomEx::ClearMobileVCode();

			m_pVCodeBtn1->SetEnabled(true);
			m_pVCodeBtn1->SetText(_T("��ȡ��֤��"));
			m_iReVCodeTime = CMobileVerifyWnd::GetVCodeValidTime();

			CEditUI* pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn_2_1_MobileNo")));
			pEdtCtrl->SetEnabled(true);

			return 0;
		}

		char szText[32] = {0};
		_stprintf(szText,"%2d�����ȡ",m_iReVCodeTime--);
		m_pVCodeBtn1->SetText(szText);

		return 0;
	}

	//��ȡ�ֻ���֤�밴ť2����ʱ(�޸��������õ�tabҳ��)
	else if (wParam == IDT_RE_GET_MOBILE_VCODE_2)
	{
		if (m_iReVCodeTime == 0)
		{
			::KillTimer(m_hWnd,IDT_RE_GET_MOBILE_VCODE_2);
			CMainRoomEx::ClearMobileVCode();

			m_pVCodeBtn2->SetEnabled(true);
			m_pVCodeBtn2->SetText(_T("��ȡ��֤��"));
			m_iReVCodeTime = CMobileVerifyWnd::GetVCodeValidTime();
			return 0;
		}

		char szText[32] = {0};
		_stprintf(szText,"%2d�����ȡ",m_iReVCodeTime--);
		m_pVCodeBtn2->SetText(szText);

		return 0;
	}

	//��ȡ�ֻ���֤�밴ť3����ʱ(�޸��������õ�tabҳ��)
	else if (wParam == IDT_RE_GET_MOBILE_VCODE_3)
	{
		if (m_iReVCodeTime == 0)
		{
			::KillTimer(m_hWnd,IDT_RE_GET_MOBILE_VCODE_3);
			CMainRoomEx::ClearMobileVCode();

			m_pVCodeBtn3->SetEnabled(true);
			m_pVCodeBtn3->SetText(_T("��ȡ��֤��"));
			m_iReVCodeTime = CMobileVerifyWnd::GetVCodeValidTime();
			return 0;
		}

		char szText[32] = {0};
		_stprintf(szText,"%2d�����ȡ",m_iReVCodeTime--);
		m_pVCodeBtn3->SetText(szText);

		return 0;
	}

	return 0;
}

LRESULT CUserInfoWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		return lRes;
	}

	if(m_pm.MessageHandler(uMsg,wParam,lParam,lRes))
	{
		return lRes;
	}

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CUserInfoWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
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

void CUserInfoWnd::Init() 
{

	char szName[20];
	for (int i = 0; i < PAGE_NUMS; ++i)
	{
		::sprintf_s(szName,sizeof(szName),_T("Ctn_Page_%d"),i + 1);
		m_pPageContainer[i] = static_cast<CContainerUI*>(m_pm.FindControl(szName));
	}

	UINT uYPos = 40;

	for (int i = 0; i < PAGE_NUMS; ++i)
	{
		::sprintf_s(szName,sizeof(szName),_T("Bnt_Page_%d"),i + 1);
		CButtonUI* pBtnCtrl = static_cast<CButtonUI*>(m_pm.FindControl(szName));
		if(pBtnCtrl)
		{
			SIZE sz = {pBtnCtrl->GetFixedXY().cx,uYPos};
			pBtnCtrl->SetFixedXY(sz);
			if(pBtnCtrl->GetName() == _T("Bnt_Page_4"))
			{
				if(CGamePlaceApp::GetModuleStatus("UserSecurity","MobileVerify",0) == 0)
				{
					pBtnCtrl->SetVisible(false);
					continue;
				}
			}
			uYPos += 35;
		}

		if (i < PAGE_NUMS - 1)
		{
			::sprintf_s(szName,sizeof(szName),_T("Img_Sep_%d"),i + 1);
			CControlUI* pSepImage = static_cast<CControlUI*>(m_pm.FindControl(szName));
			if(pSepImage)
			{
				SIZE sz = {pSepImage->GetFixedXY().cx,uYPos};
				pSepImage->SetFixedXY(sz);
			}
		}
		
	}

	//��������ҳ�洰��
	::sprintf_s(szName,sizeof(szName),_T("Ctn_Page_%d"),PAGE_LOCK_MACHINE + 1);
	m_pLockMachineCtn[0] = static_cast<CContainerUI*>(m_pm.FindControl(szName));
	::sprintf_s(szName,sizeof(szName),_T("Ctn_Page_%d_1"),PAGE_LOCK_MACHINE + 1);
	m_pLockMachineCtn[1] = static_cast<CContainerUI*>(m_pm.FindControl(szName));

	//�ֻ���ҳ��
	::sprintf_s(szName,sizeof(szName),_T("Ctn_Page_%d"),PAGE_BIND_MOBILE + 1);
	m_pBindMobileCtn[0] = static_cast<CContainerUI*>(m_pm.FindControl(szName));
	::sprintf_s(szName,sizeof(szName),_T("Ctn_Page_%d_1"),PAGE_BIND_MOBILE + 1);
	m_pBindMobileCtn[1] = static_cast<CContainerUI*>(m_pm.FindControl(szName));


	//��ȡ�ֻ���֤�밴ť1(�޸��û���ϸ��Ϣtabҳ��)
	m_pVCodeBtn1 = static_cast<CButtonUI*>(m_pm.FindControl("Btn_Ctn_2_1_GetVCode"));

	//��ȡ�ֻ���֤�밴ť2(�޸��������õ�tabҳ��)
	m_pVCodeBtn2 = static_cast<CButtonUI*>(m_pm.FindControl("Btn_Ctn_3_1_GetVCode"));

	//��ȡ�ֻ���֤�밴ť3(�ֻ��󶨵�tabҳ��)
	m_pVCodeBtn3 = static_cast<CButtonUI*>(m_pm.FindControl("Btn_Ctn_4_1_GetVCode"));

	LoadNormalCtrlSkin();

	//��ʼ��������Ϣҳ��
	InitBaseInfo();

	//��ʼ����ϸ��Ϣҳ��
	InitDetail();

	//��ʼ��������Ϣҳ��
	InitLockPCInfo();

	//��ʼ���ֻ���ҳ��
	InitBindMobile();

	//��ʼĬ����ʾ��һҳ
	ShowPage(m_nDefaultPage,m_nDefaultSubPage);

}

//��ʼ��ʡ�ݵ�����ϵ
void CUserInfoWnd::InitAreaMap()
{
	SafeClearAreaMap();

	vector<LPSTR> city;

	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("����"),
		*PutInVector(&city,LPSTR("����|����|����|����|����|��̨|ʯ��ɽ|����|��ͷ��|��ɽ|ͨ��|˳��|��ƽ|����|ƽ��|����|����|����|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("�Ϻ�"),
		*PutInVector(&city,LPSTR("����|¬��|���|����|����|����|բ��|���|����|����|��ɽ|�ζ�|�ֶ�|��ɽ|�ɽ�|����|�ϻ�|����|����|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("���"),
		*PutInVector(&city,LPSTR("��ƽ|����|�Ӷ�|����|����|����|�Ͽ�|����|�ӱ�|����|����|����|����|���|����|����|����|����|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("����"),
		*PutInVector(&city,LPSTR("����|����|����|��ɿ�|����|ɳƺ��|������|�ϰ�|����|��ʢ|˫��|�山|����|ǭ��|����|�뽭|����|ͭ��|����|�ٲ�|��ɽ|��ƽ|�ǿ�|�ᶼ|�潭|��¡|����|����|����|���|��ɽ|��Ϫ|ʯ��|��ɽ|����|��ˮ|����|�ϴ�|����|�ϴ�|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("�ӱ�"),
		*PutInVector(&city,LPSTR("ʯ��ׯ|����|��̨|����|�żҿ�|�е�|�ȷ�|��ɽ|�ػʵ�|����|��ˮ|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("ɽ��"),
		*PutInVector(&city,LPSTR("̫ԭ|��ͬ|��Ȫ|����|����|˷��|����|����|����|�ٷ�|�˳�|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("���ɹ�"),
		*PutInVector(&city,LPSTR("���ͺ���|��ͷ|�ں�|���|���ױ�����|��������|����ľ��|�˰���|�����첼��|���ֹ�����|�����׶���|��������|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("����"),
		*PutInVector(&city,LPSTR("����|����|��ɽ|��˳|��Ϫ|����|����|Ӫ��|����|����|�̽�|����|����|��«��|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("����"),
		*PutInVector(&city,LPSTR("����|����|��ƽ|��Դ|ͨ��|��ɽ|��ԭ|�׳�|�ӱ�|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("������"),
		*PutInVector(&city,LPSTR("������|�������|ĵ����|��ľ˹|����|�绯|�׸�|����|�ں�|˫Ѽɽ|����|��̨��|���˰���|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("����"),
		*PutInVector(&city,LPSTR("�Ͼ�|��|����|��ͨ|����|�γ�|����|���Ƹ�|����|����|��Ǩ|̩��|����|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("�㽭"),
		*PutInVector(&city,LPSTR("����|����|����|����|����|����|��|����|��ɽ|̨��|��ˮ|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("����"),
		*PutInVector(&city,LPSTR("�Ϸ�|�ߺ�|����|��ɽ|����|ͭ��|����|��ɽ|����|����|����|����|����|����|����|����|����|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("����"),
		*PutInVector(&city,LPSTR("����|����|����|����|Ȫ��|����|��ƽ|����|����|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("����"),
		*PutInVector(&city,LPSTR("�ϲ���|������|�Ž�|ӥ̶|Ƽ��|����|����|����|�˴�|����|����|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("ɽ��"),
		*PutInVector(&city,LPSTR("����|�ൺ|�Ͳ�|��ׯ|��Ӫ|��̨|Ϋ��|����|̩��|����|����|����|����|����|�ĳ�|����|����|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("����"),
		*PutInVector(&city,LPSTR("֣��|����|����|ƽ��ɽ|����|�ױ�|����|����|���|���|���|����Ͽ|����|����|����|�ܿ�|פ���|��Դ|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("����"),
		*PutInVector(&city,LPSTR("�人|�˲�|����|�差|��ʯ|����|�Ƹ�|ʮ��|��ʩ|Ǳ��|����|����|����|����|Т��|����"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("����"),
		*PutInVector(&city,LPSTR("��ɳ|����|����|��̶|����|����|����|����|¦��|����|����|����|����|�żҽ�|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("�㶫"),
		*PutInVector(&city,LPSTR("����|����|�麣|��ͷ|��ݸ|��ɽ|��ɽ|�ع�|����|տ��|ï��|����|����|÷��|��β|��Դ|����|��Զ|����|����|�Ƹ�|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("����"),
		*PutInVector(&city,LPSTR("����|����|����|����|����|���Ǹ�|����|���|����|��������|���ݵ���|����|��ɫ|�ӳ�|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("����"),
		*PutInVector(&city,LPSTR("����|����|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("�Ĵ�"),
		*PutInVector(&city,LPSTR("�ɶ�|����|����|�Թ�|��֦��|��Ԫ|�ڽ�|��ɽ|�ϳ�|�˱�|�㰲|�ﴨ|�Ű�|üɽ|����|��ɽ|����|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("����"),
		*PutInVector(&city,LPSTR("����|����ˮ|����|��˳|ͭ��|ǭ����|�Ͻ�|ǭ����|ǭ��|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("����"),
		*PutInVector(&city,LPSTR("����|����|����|��Ϫ|��ͨ|����|���|��ɽ|˼é|��˫����|��ɽ|�º�|����|ŭ��|����|�ٲ�"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("����"),
		*PutInVector(&city,LPSTR("����|�տ���|ɽ��|��֥|����|����|����|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("����"),
		*PutInVector(&city,LPSTR("����|����|����|ͭ��|μ��|�Ӱ�|����|����|����|����|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("����"),
		*PutInVector(&city,LPSTR("����|������|���|����|��ˮ|��Ȫ|��Ҵ|����|����|¤��|ƽ��|����|����|����|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("����"),
		*PutInVector(&city,LPSTR("����|ʯ��ɽ|����|��ԭ|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("�ຣ"),
		*PutInVector(&city,LPSTR("����|����|����|����|����|����|����|����|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("�½�"),
		*PutInVector(&city,LPSTR("��³ľ��|ʯ����|��������|����|��������|����|�������տ¶�����|��������|��³��|����|��ʲ|����|������|"))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("���"),
		*PutInVector(&city,LPSTR(""))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("����"),
		*PutInVector(&city,LPSTR(""))));
	m_mapArea.insert(pair<char*,vector<char*>>(LPSTR("̨��"),
		*PutInVector(&city,LPSTR("̨��|����|̨��|̨��|����|��Ͷ|����|����|�û�|����|����|����|��԰|����|��¡|̨��|����|����|���|"))));

}

//�ѳ������ַ�����ʼ��vector��
vector<LPSTR>* CUserInfoWnd::PutInVector(vector<LPSTR>* vec,LPSTR pszCitys)
{
	vec->clear();

	char* p = pszCitys;
	int iTotLen = strlen(p);

	if (iTotLen == 0)
	{
		return vec;
	}

	int nRead = 0;
	int nTempRead = 0;

	while (nRead < iTotLen)
	{
		nTempRead++;
		if (pszCitys[nRead] == '|')
		{
			char* pszTemp = new char[64];
			::memset(pszTemp,NULL,64);
			memcpy(pszTemp,p,nTempRead - 1);
			vec->push_back(pszTemp);

			p += nTempRead;
			nTempRead = 0;
		}
		nRead++;
	}


	return vec;
}

//��ȫ���������ϵ
void CUserInfoWnd::SafeClearAreaMap()
{
	if (!m_mapArea.empty())
	{
		map<LPSTR,vector<LPSTR>>::iterator itMap = m_mapArea.begin();
		while(itMap != m_mapArea.end())
		{
			vector<LPSTR>* vecCitys = &itMap->second;
			if (!vecCitys->empty())
			{
				vector<LPSTR>::iterator itCity = vecCitys->begin();
				while(itCity != vecCitys->end())
				{
					char* p = *itCity;
					if (p)
					{
						delete[] p;
						p = NULL;
					}
					itCity++;
				}
			}
			itMap++;
		}
	}

	m_mapArea.clear();
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
void CUserInfoWnd::ShowTipMessage(BzDui::CControlUI* pToCtrl,LPCTSTR lpSecName,LPCTSTR lpKeyName,LPCTSTR lpDefault,...)
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
		DUIOkeyMsgBox(CUserInfoWnd::m_hInstanceWnd,false,"%s",szMessage);
	}

	return;

}

//���³����б�
void CUserInfoWnd::UpdateCitys(int nProvIdx)
{
	CComboUI* pCombol = NULL;
	pCombol	= static_cast<CComboUI*>(m_pm.FindControl(_T("Cob_Ctn2_Prov")));

	char* szPriName = (char*)pCombol->GetItemAt(nProvIdx)->GetText().GetData();
	AREA_MAP::iterator it = m_mapArea.begin();

	while(it != m_mapArea.end())
	{
		if (strcmp(it->first,szPriName) == 0)
		{
			break;
		}
		it++;
	}

	pCombol	= static_cast<CComboUI*>(m_pm.FindControl(_T("Cob_Ctn2_City")));
	pCombol->RemoveAll();
	
	if (it != m_mapArea.end())
	{
		vector<LPSTR>::iterator itCity = it->second.begin();

		if (itCity != it->second.end())
		{
			while(itCity != it->second.end())
			{
				CListLabelElementUI* p = new CListLabelElementUI();
				p->SetText(*itCity);

				pCombol->Add(p);
				itCity++;
			}
			pCombol->SetEnabled(true);
			pCombol->SelectItem(0);
		}
		else //ʡ������û�г���
		{
			pCombol->SetEnabled(false);
		}
	}
}

//��ʼ��������Ϣҳ��
void CUserInfoWnd::InitBaseInfo()
{
	if (!m_pUserInfo)
	{
		return;
	}

	CEditUI* pEdtCtrl = NULL;
	CComboUI* pCobCtrl = NULL;
	CRichEditUI* pREdtCtrl = NULL;
	TCHAR szTemp[BUFSIZ];

	m_pPageContainer[0]->SetVisible(true);

	//�û���
	pEdtCtrl= static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn1_Name")));
	pEdtCtrl->SetText(m_pUserInfo->nickName);
	pEdtCtrl->SetMaxChar(sizeof(m_pUserInfo->nickName) - 2);

	//�û�ID
	pEdtCtrl= static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn1_ID")));
	::sprintf_s(szTemp,BUFSIZ,"%d",m_pUserInfo->dwUserID);
	pEdtCtrl->SetText(szTemp);

	//�û��Ա�
	pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl(_T("Cob_Ctn1_Sex")));
	
	LPCTSTR szItems[2] = {_TEXT("Ů"),_TEXT("��")};
	LoadComboItems(pCobCtrl,szItems,2);
	pCobCtrl->SelectItem(m_pUserInfo->bBoy);

	//�û�����ǩ��
	pREdtCtrl= static_cast<CRichEditUI*>(m_pm.FindControl(_T("Edt_Ctn1_Sign")));
	pREdtCtrl->SetText(m_pUserInfo->szSignDescr);

	m_pPageContainer[0]->SetVisible(false);
}

//��ʼ����ϸ��Ϣҳ��
void CUserInfoWnd::InitDetail()
{
	if (!m_pUserInfo)
	{
		return;
	}

	CEditUI* pEdtCtrl = NULL;
	CButtonUI* pBntCtrl = NULL;
	CComboUI* pCobCtrl = NULL;
	CContainerUI* pCtnCtrl = NULL;
	TCHAR szTemp[BUFSIZ];

	m_pPageContainer[1]->SetVisible(true);

	//��ʵ����
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_RealName")));
	pEdtCtrl->SetText(m_pUserInfo->szRealName);
	pEdtCtrl->SetMaxChar(sizeof(m_pUserInfo->szRealName) - 2);

	//֤������
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_IDCard")));
	pEdtCtrl->SetText(m_pUserInfo->szIDCardNo);
	pEdtCtrl->SetEnabled(false);

	//�ֻ�����
	pBntCtrl = static_cast<CButtonUI*>(m_pm.FindControl(_T("Btn_Ctn2_TelNo_Modify")));
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_MobileNo")));
	pEdtCtrl->SetText(m_pUserInfo->szMobileNo);
	pEdtCtrl->SetMaxChar(sizeof(m_pUserInfo->szMobileNo) - 2);

	if (CGamePlaceApp::GetModuleStatus(_T("UserSecurity"),_T("MobileVerify")) == 1)
	{
		pEdtCtrl->SetEnabled(false);
		pBntCtrl->SetVisible(true);
	}
	else
	{
		pEdtCtrl->SetEnabled(true);
		pBntCtrl->SetVisible(false);
	}

	//�ֻ���������
	pCtnCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Ctn_Page_2_1"));
	pCtnCtrl->SetVisible(false);

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn_2_1_MobileNo")));
	pEdtCtrl->SetMaxChar(18);

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn_2_1_VCode")));
	pEdtCtrl->SetMaxChar(6);

	//QQ����
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_QQNo")));
	pEdtCtrl->SetText(m_pUserInfo->szQQNum);
	pEdtCtrl->SetMaxChar(sizeof(m_pUserInfo->szQQNum) - 2);

	//���ù���
	pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl(_T("Cob_Ctn2_Nat")));
	LPCTSTR szNats[1] = {_TEXT("�й�")};
	LoadComboItems(pCobCtrl,szNats,1);
	pCobCtrl->SelectItem(0);

	int iSelect = 0;
	int iCounter = 0;

	//����ʡ��
	pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl(_T("Cob_Ctn2_Prov")));
	map<LPSTR,vector<LPSTR>>::iterator itMap= m_mapArea.begin();
	while(itMap != m_mapArea.end())
	{
		CListLabelElementUI* p = new CListLabelElementUI();
		p->SetText(itMap->first);
		pCobCtrl->Add(p);
		itMap++;
	}
	SelectComboItem(pCobCtrl,m_pUserInfo->szAdrProvince);

	//���ó���
	pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl(_T("Cob_Ctn2_City")));
	SelectComboItem(pCobCtrl,m_pUserInfo->szAdrCity);

	//��������
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_ZipCode")));
	pEdtCtrl->SetText(m_pUserInfo->szZipCode);
	pEdtCtrl->SetMaxChar(sizeof(m_pUserInfo->szZipCode) - 2);

	//��ϵ��ʽ
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_Address")));
	pEdtCtrl->SetText(m_pUserInfo->szAddress);
	pEdtCtrl->SetMaxChar(sizeof(m_pUserInfo->szAddress) - 2);

	//��ʼѧ������ʾ
	pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl(_T("Cob_Ctn2_Schooling")));
	LPCTSTR szSchooling[6] = {_TEXT("��������"),_TEXT("����"),_TEXT("��ר"),_TEXT("��ר"),_TEXT("����"),_TEXT("˶ʿ����")};
	LoadComboItems(pCobCtrl,szSchooling,6);
	SelectComboItem(pCobCtrl,m_pUserInfo->szSchooling - 2);

	//�����ʼ�
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_EMail")));
	pEdtCtrl->SetText(m_pUserInfo->szEmail);
	pEdtCtrl->SetMaxChar(sizeof(m_pUserInfo->szEmail) - 2);

	//������ҳ
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_HomePage")));
	pEdtCtrl->SetText(m_pUserInfo->szHomePage);
	pEdtCtrl->SetMaxChar(sizeof(m_pUserInfo->szHomePage) - 2);

	//�̶��绰
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_TelNo")));
	pEdtCtrl->SetText(m_pUserInfo->szTelNo);
	pEdtCtrl->SetMaxChar(sizeof(m_pUserInfo->szTelNo) - 2);

	//MSN�ʺ�
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_MSN")));
	pEdtCtrl->SetText(m_pUserInfo->szMSNID);
	pEdtCtrl->SetMaxChar(sizeof(m_pUserInfo->szMSNID) - 2);

	m_pPageContainer[1]->SetVisible(false);
}

//��ʼ��������Ϣҳ��
void CUserInfoWnd::InitLockPCInfo()
{
	if (!m_pUserInfo)
	{
		return;
	}

	CLabelUI* pLabCtrl = NULL;
	CButtonUI* pBntCtrl = NULL;
	CEditUI* pEdtCtrl = NULL;
	TCHAR szTemp[BUFSIZ];

	//�󶨲�����ť
	pBntCtrl = static_cast<CButtonUI*>(m_pm.FindControl(_T("Bnt_Ctn3_PageNext")));
	if(m_pUserInfo->iLockMathine == 1)
	{
		pBntCtrl->SetText(_TEXT("�����"));
		pBntCtrl->SetTag(0);
	}
	else
	{
		pBntCtrl->SetText(_TEXT("��Ҫ��"));
		pBntCtrl->SetTag(1);
	}


	//������־
	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl(_T("Lab_Ctn3_State")));
	if(m_pUserInfo->iLockMathine == 1)
	{
		pLabCtrl->SetShowHtml(false);
		pLabCtrl->SetText(_TEXT("��ǰ״̬���Ѱ�"));
	}
	else
	{
		pLabCtrl->SetShowHtml(true);
		pLabCtrl->SetText(_TEXT("<c #FF0000><b>��ǰ״̬��δ��</b><c>"));
	}

	bool bMobileVerify = false;//(CGamePlaceApp::GetModualStatus(_T("UserSecurity"),_T("MobileVerify")) == 1);

	//��������༭��
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn3_1_Password")));
	pEdtCtrl->SetPasswordMode(true);
	pEdtCtrl->SetText(_T(""));
	pEdtCtrl->SetMaxChar(32);

	//�ֻ��ű༭��
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn3_1_PhoneNo")));
	pEdtCtrl->SetMaxChar(16);

	if (!bMobileVerify)
	{
		pEdtCtrl->SetVisible(false);
	}

	//�ֻ���֤��༭��
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn3_1_VCode")));
	pEdtCtrl->SetMaxChar(6);

	if (!bMobileVerify)
	{
		pEdtCtrl->SetVisible(false);
	}

	if (!bMobileVerify)
	{
		pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Page_3_1"));
		pLabCtrl->SetVisible(false);
		pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl("Lab_Page_3_2"));
		pLabCtrl->SetVisible(false);

		pBntCtrl = static_cast<CButtonUI*>(m_pm.FindControl("Btn_Ctn_3_1_GetVCode"));
		pBntCtrl->SetVisible(false);
	}

}

//��ʼ���ֻ���ҳ��
void CUserInfoWnd::InitBindMobile()
{
	if (!m_pUserInfo)
	{
		return;
	}

	CLabelUI* pLabCtrl = NULL;
	CButtonUI* pBntCtrl = NULL;
	CEditUI* pEdtCtrl = NULL;
	TCHAR szTemp[BUFSIZ];

	//�󶨲�����ť
	pBntCtrl = static_cast<CButtonUI*>(m_pm.FindControl(_T("Bnt_Ctn4_PageNext")));
	if(m_pUserInfo->iBindMobile == 1)
	{
		pBntCtrl->SetText(_TEXT("�����"));
		pBntCtrl->SetTag(0);
	}
	else
	{
		pBntCtrl->SetText(_TEXT("��Ҫ��"));
		pBntCtrl->SetTag(1);
	}


	//������־
	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl(_T("Lab_Ctn4_State")));
	if(m_pUserInfo->iBindMobile == 1)
	{
		pLabCtrl->SetShowHtml(false);
		pLabCtrl->SetText(_TEXT("��ǰ״̬���Ѱ�"));
	}
	else
	{
		pLabCtrl->SetShowHtml(true);
		pLabCtrl->SetText(_TEXT("<c #FF0000><b>��ǰ״̬��δ��</b><c>"));
	}

	//�ֻ��ű༭��
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn4_1_PhoneNo")));
	pEdtCtrl->SetMaxChar(16);

	//�ֻ���֤��༭��
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn4_1_VCode")));
	pEdtCtrl->SetMaxChar(6);
}

//����Combol���б���
void CUserInfoWnd::LoadComboItems(CComboUI* pCtrl,LPCTSTR items[],int nums,int nSelect/* = 0*/)
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
bool CUserInfoWnd::SelectComboItem(BzDui::CComboUI* pCtrl,LPCTSTR szText)
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

void CUserInfoWnd::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("click")) 
	{
		if( msg.pSender->GetName() == _T("Bnt_Close")) 
		{
			Close();
		}
		else if (msg.pSender->GetName() == _T("Bnt_Page_1"))
		{
			ShowPage(0);
		}
		else if (msg.pSender->GetName() == _T("Bnt_Page_2"))
		{
			ShowPage(1);
		}
		else if (msg.pSender->GetName() == _T("Bnt_Page_3"))
		{
			ShowPage(2);
		}
		else if (msg.pSender->GetName() == _T("Bnt_Page_4"))
		{
			ShowPage(3);
		}
		else if (msg.pSender->GetName() == _T("Bnt_Page_5"))
		{
			ShowPage(4);
		}
		else if (msg.pSender->GetName() == _T("Bnt_Cnt1_Modify"))
		{
			OnUpdateBaseInfo();
		}
		else if (msg.pSender->GetName() == _T("Btn_Ctn2_Modify"))
		{
			OnUpdateDetailInfo();
		}
		else if (msg.pSender->GetName() == _T("Btn_Ctn_2_1_GetVCode"))
		{
			OnGetMobileVCode(3,m_pVCodeBtn1,
				static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn_2_1_MobileNo")),
				static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn_2_1_VCode")),
				IDT_RE_GET_MOBILE_VCODE_1);
		}
		else if (msg.pSender->GetName() == _T("Btn_Ctn2_TelNo_Modify"))
		{
			OnModifyMobileOpen();
		}
		else if (msg.pSender->GetName() == _T("Btn_Ctn_2_1_Cancel"))
		{
			OnModifyMobileCancel();
		}

		else if (msg.pSender->GetName() == _T("Btn_Ctn_2_1_OK"))
		{
			OnModifyMobileOK();
		}
		else if (msg.pSender->GetName() == _T("Bnt_Ctn3_PageNext"))
		{
			OnLockOperatePage(msg.pSender->GetTag() == 1);
		}
		
		else if (msg.pSender->GetName() == _T("Bnt_Ctn3_1_Band"))
		{
			OnLockPC(msg.pSender->GetTag() == 1);
		}
		else if (msg.pSender->GetName() == _T("Btn_Ctn_3_1_GetVCode"))
		{
			OnGetMobileVCode((m_pUserInfo->iLockMathine == 1 ? 2 : 1),m_pVCodeBtn2,
								static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn3_1_PhoneNo")),
								static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn3_1_VCode")),
								IDT_RE_GET_MOBILE_VCODE_2);
		}

		else if (msg.pSender->GetName() == _T("Bnt_Ctn4_PageNext"))
		{
			OnBindMobileOperatePage(msg.pSender->GetTag() == 1);
		}
		else if (msg.pSender->GetName() == _T("Btn_Ctn_4_1_GetVCode"))
		{
			OnGetMobileVCode((m_pUserInfo->iBindMobile == 1 ? 5 : 4),m_pVCodeBtn3,
				static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn4_1_PhoneNo")),
				static_cast<CEditUI*>(m_pm.FindControl("Edt_Ctn4_1_VCode")),
				IDT_RE_GET_MOBILE_VCODE_3);
		}
		else if (msg.pSender->GetName() == _T("Bnt_Ctn4_1_Band"))
		{
			OnBindMobile(msg.pSender->GetTag() == 1);
		}

		else if (msg.pSender->GetName() == _T("Btn_Ctn5_Modify"))
		{
			OnUpdatePassword();
		}
		
	}
	else if (msg.sType == _T("itemselect"))
	{
		if( msg.pSender->GetName() == _T("Cob_Ctn2_Prov")) 
		{
			UpdateCitys(msg.wParam);
		}
	}
}

//�����û�����
void CUserInfoWnd::SetPlaceUserInfo(MSG_GP_UserInfo* pUserInfo)
{
	m_pUserInfo = pUserInfo;
}

//���س���ؼ�Ƥ��
void CUserInfoWnd::LoadNormalCtrlSkin()
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
			pEditUI->SetNormalImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,0,500,22' corner='3,4,2,2'");
			pEditUI->SetHotImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,22,500,44' corner='3,4,2,2'");
			pEditUI->SetFocusedImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,44,500,66' corner='3,4,2,2'");
			pEditUI->SetDisabledImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,66,500,88' corner='3,4,2,2'");
			pEditUI->SetTextPadding(rcText);
		}
	} while (pEditUI);

	pCtrls = m_pm.FindSubControlsByClass(m_pm.GetRoot(),_TEXT("RichEdit"));
	CRichEditUI* pREditUI = NULL;
	RECT rcRText = {4,1,4,0};
	do 
	{
		pREditUI = static_cast<CRichEditUI*>(pCtrls->GetAt(idx++));

		if (pREditUI)
		{
// 			pREditUI->SetNormalImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,0,500,22' corner='3,4,2,2'");
// 			pREditUI->SetHotImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,22,500,44' corner='3,4,2,2'");
// 			pREditUI->SetFocusedImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,44,500,66' corner='3,4,2,2'");
// 			pREditUI->SetDisabledImage("file='dialog\\edit.png' dest='0,0,500,22' source='0,66,500,88' corner='3,4,2,2'");
// 			pREditUI->SetTextPadding(rcRText);
		}
	} while (pREditUI);

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

}

//��ʾ����ҳ��
void CUserInfoWnd::ShowPage(int nIndex,int nSubIndex/* = 0*/)
{
	if (nIndex < 0 || nIndex >= PAGE_NUMS)
	{
		return;
	}

	CControlUI* pContainer = NULL;
	TCHAR szName[20];
	
	for (int i = 0; i < PAGE_NUMS; ++i)
	{
		if (i == PAGE_LOCK_MACHINE) //����ҳ�����ص�һ����
		{
			m_pPageContainer[i]->SetVisible(false);
			m_pPageContainer[i] = m_pLockMachineCtn[nSubIndex];
			if (nSubIndex == 1)
			{
				OnLockOperatePage((m_pUserInfo->iLockMathine == 1) ? false : true);
			}
		}
		else if (i == PAGE_BIND_MOBILE) //�ֻ���ҳ�����ص�һ����
		{
			m_pPageContainer[i]->SetVisible(false);
			m_pPageContainer[i] = m_pBindMobileCtn[nSubIndex];
			if (nSubIndex == 1)
			{
				OnBindMobileOperatePage((m_pUserInfo->iBindMobile == 1) ? false : true);
			}
		}

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

	//��ҳʱ�������л�ȡ�ֻ���֤��İ�ť��ԭ����֤����ȡ��ʱ���رյ�
	//��֤����ȡ��ʱ�����ڼ�ʱʱ�����������
	ResetReVCodeUI();

	return;
}

//�����û��Ļ�����Ϣ
void CUserInfoWnd::OnUpdateBaseInfo()
{
	MSG_GP_S_UserInfo tagUserInfo;

	CEditUI* pEdtCtrl = NULL;
	CRichEditUI* pREdtCtrl = NULL;
	CComboUI* pCobCtrl = NULL;
	TCHAR szTemp[BUFSIZ];

	//�û���
	pEdtCtrl= static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn1_Name")));
	CStdString str = pEdtCtrl->GetText();
	if (str.Find(" ",0) > 0)
	{
		DUIOkeyMsgBox(m_hWnd,false,"�ǳƲ��ܺ��пո�");
		pEdtCtrl->SetFocus();
		return;
	}

	if (str.GetLength() < 6)
	{
		DUIOkeyMsgBox(m_hWnd,false,"�ǳ�������6���ַ���ɡ�");
		pEdtCtrl->SetFocus();
		return;
	}

	::strcpy_s(m_pUserInfo->nickName,sizeof(m_pUserInfo->nickName),str);

	//�û��Ա�
	pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl(_T("Cob_Ctn1_Sex")));
	m_pUserInfo->bBoy = (pCobCtrl->GetText() == _T("��"));

	//�û�����ǩ��
	pREdtCtrl = static_cast<CRichEditUI*>(m_pm.FindControl(_T("Edt_Ctn1_Sign")));

	if (pREdtCtrl->GetText().GetLength() > 100)
	{
		DUIOkeyMsgBox(m_hWnd,false,"ǩ�����������ܳ���100��Ӣ���ַ�(50�������ַ�)");
		pREdtCtrl->SetFocus();
		return;
	}

	::strcpy_s(m_pUserInfo->szSignDescr,sizeof(m_pUserInfo->szSignDescr),pREdtCtrl->GetText());

	::memcpy(&tagUserInfo,m_pUserInfo,sizeof(MSG_GP_S_UserInfo));
	GetMainRoom()->SendData(&tagUserInfo,sizeof(MSG_GP_S_UserInfo),MDM_GP_USERINFO,ASS_GP_USERINFO_UPDATE_BASE,0);

	return;
}

//�����û�����ϸ��Ϣ
void CUserInfoWnd::OnUpdateDetailInfo()
{
	MSG_GP_S_UserInfo tagUserInfo;

	CEditUI* pEdtCtrl = NULL;
	CComboUI* pCobCtrl = NULL;
	TCHAR szTemp[BUFSIZ];

	//��ʵ����
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_RealName")));
	::strcpy_s(m_pUserInfo->szRealName,sizeof(m_pUserInfo->szRealName),pEdtCtrl->GetText());

	//֤������
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_IDCard")));
	::strcpy_s(m_pUserInfo->szIDCardNo,sizeof(m_pUserInfo->szIDCardNo),pEdtCtrl->GetText());

	//�ֻ�����
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_MobileNo")));
	::strcpy_s(m_pUserInfo->szMobileNo,sizeof(m_pUserInfo->szMobileNo),pEdtCtrl->GetText());

	//QQ����
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_QQNo")));
	::strcpy_s(m_pUserInfo->szQQNum,sizeof(m_pUserInfo->szQQNum),pEdtCtrl->GetText());

	//���ù���
	pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl(_T("Cob_Ctn2_Nat")));
	::strcpy_s(m_pUserInfo->szAdrNation,sizeof(m_pUserInfo->szAdrNation),pCobCtrl->GetText());

	//����ʡ��
	pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl(_T("Cob_Ctn2_Prov")));
	::strcpy_s(m_pUserInfo->szAdrProvince,sizeof(m_pUserInfo->szAdrProvince),pCobCtrl->GetText());

	//���ó���
	pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl(_T("Cob_Ctn2_City")));
	::strcpy_s(m_pUserInfo->szAdrCity,sizeof(m_pUserInfo->szAdrCity),pCobCtrl->GetText());

	//��������
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_ZipCode")));
	::strcpy_s(m_pUserInfo->szZipCode,sizeof(m_pUserInfo->szZipCode),pEdtCtrl->GetText());

	//��ϵ��ʽ
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_Address")));
	::strcpy_s(m_pUserInfo->szAddress,sizeof(m_pUserInfo->szAddress),pEdtCtrl->GetText());

	//��ʼѧ������ʾ
	pCobCtrl = static_cast<CComboUI*>(m_pm.FindControl(_T("Cob_Ctn2_Schooling")));
	::strcpy_s(m_pUserInfo->szSchooling,sizeof(m_pUserInfo->szSchooling),pCobCtrl->GetText());

	//�����ʼ�
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_EMail")));
	::strcpy_s(m_pUserInfo->szEmail,sizeof(m_pUserInfo->szEmail),pEdtCtrl->GetText());

	//������ҳ
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_HomePage")));
	::strcpy_s(m_pUserInfo->szHomePage,sizeof(m_pUserInfo->szHomePage),pEdtCtrl->GetText());

	//�̶��绰
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_TelNo")));
	::strcpy_s(m_pUserInfo->szTelNo,sizeof(m_pUserInfo->szTelNo),pEdtCtrl->GetText());

	//MSN�ʺ�
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_MSN")));
	::strcpy_s(m_pUserInfo->szMSNID,sizeof(m_pUserInfo->szMSNID),pEdtCtrl->GetText());

	::memcpy(&tagUserInfo,m_pUserInfo,sizeof(MSG_GP_S_UserInfo));
	GetMainRoom()->SendData(&tagUserInfo,sizeof(MSG_GP_S_UserInfo),MDM_GP_USERINFO,ASS_GP_USERINFO_UPDATE_DETAIL,0);

	return;
}

//��������ֻ���
void CUserInfoWnd::OnModifyMobileOpen()
{
	ResetReVCodeUI();

	CContainerUI* pCtnCtrl = NULL;
	CEditUI* pEdtCtrl = NULL;

	pCtnCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Ctn_Page_2_1"));
	pCtnCtrl->SetVisible(true);

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn_2_1_MobileNo")));
	pEdtCtrl->SetFocus();
	pEdtCtrl->SetText(_TEXT(""));

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn_2_1_VCode")));
	pEdtCtrl->SetText(_TEXT(""));
}

//ȡ�������ֻ���
void CUserInfoWnd::OnModifyMobileCancel()
{
	ResetReVCodeUI();

	CContainerUI* pCtnCtrl = NULL;

	pCtnCtrl = static_cast<CContainerUI*>(m_pm.FindControl("Ctn_Page_2_1"));
	pCtnCtrl->SetVisible(false);
}

//ȷ�������ֻ���
void CUserInfoWnd::OnModifyMobileOK()
{
	CContainerUI* pCtnCtrl = NULL;
	CEditUI* pEdtCtrl = NULL;

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn_2_1_VCode")));
	CStdString strVCode = pEdtCtrl->GetText();

	if(strVCode.GetLength() == 0)
	{
		ShowTipMessage(NULL,"","","�������ֻ���֤��");
		pEdtCtrl->SetFocus();
		return;
	}

	//�ڴ�����ֻ���֤
	if (!CMainRoomEx::ValidMobileVCode((char*)strVCode.GetData()))
	{
		ShowTipMessage(NULL,"","","�ֻ���֤�벻��ȷ");
		pEdtCtrl->SetFocus();
		return;
	}

	pEdtCtrl->SetText(_T(""));

	//�ֻ���֤ͨ��
	//�����л�ȡ�ֻ���֤��İ�ť��ԭ����֤����ȡ��ʱ���رյ�
	ResetReVCodeUI();

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn_2_1_MobileNo")));
	CStdString strMobileNo = pEdtCtrl->GetText();

	pEdtCtrl->SetText(_T(""));

	//�����ֻ����޸Ŀ�
	pCtnCtrl = static_cast<CContainerUI*>(m_pm.FindControl(_T("Ctn_Page_2_1")));
	pCtnCtrl->SetVisible(false);

	//����֤ͨ�������ֻ��Ÿ��µ��ϲ�༭��
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_MobileNo")));
	pEdtCtrl->SetText(strMobileNo);

	ShowTipMessage(NULL,"","","���ĸ��ֻ������ע�ⰴ�·����޸ġ���ť�ύ���ġ�");

}

//����/������Ť��Ӧ
void CUserInfoWnd::OnLockOperatePage(bool bLock)
{
	//GetMainRoom()->OnSwitchLockButton();
	m_pPageContainer[PAGE_LOCK_MACHINE]->SetVisible(false);
	m_pPageContainer[PAGE_LOCK_MACHINE] = m_pLockMachineCtn[1];
	m_pPageContainer[PAGE_LOCK_MACHINE]->SetVisible(true);

	CEditUI* pEdtCtrl = NULL;
	CButtonUI* pBtnCtrl = NULL;

	//��������༭��
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn3_1_Password")));
	pEdtCtrl->SetText(_T(""));
	pEdtCtrl->SetFocus();

	//�󶨲�����ť
	pBtnCtrl = static_cast<CButtonUI*>(m_pm.FindControl(_T("Bnt_Ctn3_1_Band")));
	if (bLock)
	{
		if (CMainRoomEx::m_bLockQuit)
		{
			pBtnCtrl->SetText(_TEXT("�󶨲��˳�"));	
		}
		else
		{
			pBtnCtrl->SetText(_TEXT("��"));
		}
		pBtnCtrl->SetTag(1);
	}
	else
	{
		pBtnCtrl->SetText(_TEXT("�����"));
		pBtnCtrl->SetTag(0);
	}
	
	//�ֻ�����ʾ����������ֻ��ţ�������д
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn3_1_PhoneNo")));
	if (::strlen(m_pUserInfo->szMobileNo) > 0)
	{
		pEdtCtrl->SetText(m_pUserInfo->szMobileNo);
		pEdtCtrl->SetEnabled(false);
	}
	else
	{
		pEdtCtrl->SetEnabled(true);
		pEdtCtrl->SetText(_T(""));
	}
	
	//�ֻ���֤��༭��
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn3_1_VCode")));
	pEdtCtrl->SetText(_T(""));
}

//�����ֻ����󶨵���
void CUserInfoWnd::OnBindMobileOperatePage(bool bLock)
{
	//GetMainRoom()->OnSwitchLockButton();
	m_pPageContainer[PAGE_BIND_MOBILE]->SetVisible(false);
	m_pPageContainer[PAGE_BIND_MOBILE] = m_pBindMobileCtn[1];
	m_pPageContainer[PAGE_BIND_MOBILE]->SetVisible(true);

	CEditUI* pEdtCtrl = NULL;
	CButtonUI* pBtnCtrl = NULL;

	//�󶨲�����ť
	pBtnCtrl = static_cast<CButtonUI*>(m_pm.FindControl(_T("Bnt_Ctn4_1_Band")));
	if (bLock)
	{
		pBtnCtrl->SetText(_TEXT("��"));
		pBtnCtrl->SetTag(1);
	}
	else
	{
		pBtnCtrl->SetText(_TEXT("�����"));
		pBtnCtrl->SetTag(0);
	}

	//�ֻ�����ʾ����������ֻ��ţ�������д
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn4_1_PhoneNo")));
	if (::strlen(m_pUserInfo->szMobileNo) > 0)
	{
		pEdtCtrl->SetText(m_pUserInfo->szMobileNo);
		pEdtCtrl->SetEnabled(false);
	}
	else
	{
		pEdtCtrl->SetEnabled(true);
		pEdtCtrl->SetText(_T(""));
	}

	//�ֻ���֤��༭��
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn4_1_VCode")));
	pEdtCtrl->SetText(_T(""));
}

//����/������Ť��Ӧ
void CUserInfoWnd::OnLockPC(bool bLock)
{
	CEditUI* pEdtCtrl = NULL;

	//��������༭��
	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn3_1_Password")));
	CStdString strPassword = pEdtCtrl->GetText();

	if(strPassword.GetLength() == 0 || strPassword.GetLength() > 32)
	{
		ShowTipMessage(NULL,"LockAccountDlg","InputBankPW","��������������");
		pEdtCtrl->SetFocus();
		return;
	}

	if(false/*CGamePlaceApp::GetModualStatus(_T("UserSecurity"),_T("MobileVerify")) == 1*/)
	{
		pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn3_1_VCode")));
		CStdString strVCode = pEdtCtrl->GetText();

		if(strVCode.GetLength() == 0)
		{
			ShowTipMessage(NULL,"","","�������ֻ���֤��");
			pEdtCtrl->SetFocus();
			return;
		}

		//�ڴ�����ֻ���֤
		if (!CMainRoomEx::ValidMobileVCode((char*)strVCode.GetData()))
		{
			ShowTipMessage(NULL,"","","�ֻ���֤�벻��ȷ");
			pEdtCtrl->SetFocus();
			return;
		}

		//�ֻ���֤ͨ��
		//�����л�ȡ�ֻ���֤��İ�ť��ԭ����֤����ȡ��ʱ���رյ�
		ResetReVCodeUI();

		pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn3_1_PhoneNo")));
		CStdString strMobileNo = pEdtCtrl->GetText();
		if (::strlen(m_pUserInfo->szMobileNo) == 0) //��һ�������ֻ���
		{
			//���޸Ĺ��ֻ���
			pEdtCtrl->SetEnabled(false);
			::lstrcpy(m_pUserInfo->szMobileNo,strMobileNo.GetData());

			//ͬ���ֻ��ŵ���ϸ����Tab�ġ��ֻ��š�����
			pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_MobileNo")));
			pEdtCtrl->SetText(strMobileNo);
		}
	}

	//�޸�֧��MD5 ��SHA ���ܷ�ʽ
	CenterServerMsg& par=Glb().m_CenterServerPara;

	//md5��ʽ
	char szMD5Pass[50] = {0};
	if( par.m_nEncryptType == 1)
	{
		unsigned char szMDTemp[16];
		MD5_CTX Md5;
		Md5.MD5Update((unsigned char*)strPassword.GetData(),strPassword.GetLength());
		Md5.MD5Final(szMDTemp);
		for (int i = 0; i < 16; i ++) 
			wsprintf(&szMD5Pass[i * 2], "%02x", szMDTemp[i] );
	}

	//sha���ܷ�ʽ
	if(par.m_nEncryptType == 2)
	{
		unsigned szMDTemp[5] = {0};
		CString strSha = strPassword;
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


	MSG_GP_S_LockAccount lockMsg;
	::memset(&lockMsg,NULL,sizeof(MSG_GP_S_LockAccount));

	lockMsg.dwUserID = m_pUserInfo->dwUserID;
	lockMsg.dwCommanType = bLock ? 1 : 0;
	::lstrcpy(lockMsg.szMathineCode,CMainRoomEx::coreGetCode());
	::lstrcpy(lockMsg.szMD5Pass,szMD5Pass);
	::lstrcpy(lockMsg.szMobileNo,m_pUserInfo->szMobileNo);

	//����������������
	GetMainRoom()->SendData(&lockMsg,sizeof(MSG_GP_S_LockAccount),
		MDM_GP_LOCK_ACCOUNT,ASS_LOCK_ACCOUNT,0);

}

//�����û�����
void CUserInfoWnd::OnUpdatePassword()
{
	CEditUI* pEdtOldPwd = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn5_oldpw")));
	CEditUI* pEdtNewPwd = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn5_newpw")));
	CEditUI* pEdtCfmPwd = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn5_cfmpw")));

	if (pEdtOldPwd->GetText().GetLength() == 0)
	{
		DUIOkeyMsgBox(m_hWnd,true,"����д������");
		pEdtOldPwd->SetFocus();
		return;
	}

	if (pEdtNewPwd->GetText().GetLength() == 0)
	{
		DUIOkeyMsgBox(m_hWnd,true,"����д������");
		pEdtNewPwd->SetFocus();
		return;
	}

	if (pEdtCfmPwd->GetText().GetLength() == 0)
	{
		DUIOkeyMsgBox(m_hWnd,true,"����д������ȷ��");
		pEdtCfmPwd->SetFocus();
		return;
	}

	CenterServerMsg& par=Glb().m_CenterServerPara;

	char szMD5Input[64];
	::strcpy_s(szMD5Input,sizeof(szMD5Input),pEdtOldPwd->GetText());

	if( par.m_nEncryptType == 1)
	{

		MD5_CTX Md5;
		Md5.MD5Update((unsigned char *)szMD5Input,lstrlen(szMD5Input));
		unsigned char szMDTemp[16];
		Md5.MD5Final(szMDTemp);
		for (int i=0;i<16;i++)
		{
			wsprintf(&szMD5Input[i*2],"%02x",szMDTemp[i]);
		}
	}
	else if(par.m_nEncryptType == 2)
	{
		//JMod-���ܴ���-20090429
		unsigned szMDTemp[5]={0};
		CString strSha= pEdtOldPwd->GetText();
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
		wsprintf(szMD5Input,"%s",strSha);
	}


	if (strcmp((const char*)szMD5Input,m_pUserInfo->szMD5Pass) != 0)
	{
		DUIOkeyMsgBox(m_hWnd,true,"�����벻��ȷ");
		pEdtOldPwd->SetFocus();
		return;
	}

	if (pEdtNewPwd->GetText().GetLength() < 6 || pEdtNewPwd->GetText().GetLength() > 12)
	{
		DUIOkeyMsgBox(m_hWnd,true,"������ĳ���Ϊ6-12λ�ַ�");
		pEdtNewPwd->SetFocus();
		return;
	}

	if (strcmp(pEdtNewPwd->GetText(),pEdtCfmPwd->GetText()) != 0)
	{
		DUIOkeyMsgBox(m_hWnd,true,"��������ȷ�ϲ�һ��");
		pEdtCfmPwd->SetFocus();
		return;
	}

	::strcpy_s(szMD5Input,sizeof(szMD5Input),pEdtNewPwd->GetText());

	if( par.m_nEncryptType == 1)
	{

		MD5_CTX Md5;
		Md5.MD5Update((unsigned char *)szMD5Input,lstrlen(szMD5Input));
		unsigned char szMDTemp[16];
		Md5.MD5Final(szMDTemp);
		for (int i=0;i<16;i++)
		{
			wsprintf(&szMD5Input[i*2],"%02x",szMDTemp[i]);
		}
	}
	else if(par.m_nEncryptType == 2)
	{
		//JMod-���ܴ���-20090429
		unsigned szMDTemp[5]={0};
		CString strSha= pEdtNewPwd->GetText();
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
		wsprintf(szMD5Input,"%s",strSha);
	}

	CMyDiskInfo diskInfo;
	diskInfo.GetDiskInfo();

	MSG_GP_S_ChPassword tagChPwd;
	tagChPwd.dwUserID = m_pUserInfo->dwUserID;
	lstrcpy(tagChPwd.szHardID,diskInfo.szSerialNumber);

	::memcpy_s(tagChPwd.szMD5OldPass,sizeof(tagChPwd.szMD5OldPass),m_pUserInfo->szMD5Pass,sizeof(tagChPwd.szMD5OldPass));
	::memcpy_s(tagChPwd.szMD5NewPass,sizeof(tagChPwd.szMD5NewPass),szMD5Input,sizeof(szMD5Input));

	GetMainRoom()->SendData(&tagChPwd,sizeof(MSG_GP_S_ChPassword),MDM_GP_USERINFO,ASS_GP_USERINFO_UPDATE_PWD,0);

	::memcpy_s(m_pUserInfo->szMD5Pass,sizeof(m_pUserInfo->szMD5Pass),szMD5Input,sizeof(m_pUserInfo->szMD5Pass));
	
}

//�����ȡ��֤��İ�ť
void CUserInfoWnd::OnGetMobileVCode(UINT uType,BzDui::CButtonUI* pBtn,BzDui::CEditUI* pMobileNoEdt,BzDui::CEditUI* pVCodeEdt,UINT uTimerID)
{
	CStdString strMobileNo = pMobileNoEdt->GetText();
	if (strMobileNo.GetLength() == 0)
	{
		ShowTipMessage(NULL,"","","�������ֻ�����");
		pMobileNoEdt->SetFocus();
		return;
	}

	pMobileNoEdt->SetEnabled(false);

	pVCodeEdt->SetText(_T(""));
	pVCodeEdt->SetFocus();
	CMainRoomEx::ClearMobileVCode();
	GetMainRoom()->OnSendMobileVCodeSms((char*)strMobileNo.GetData(),uType);

	pBtn->SetEnabled(false);
	pBtn->SetText("��ȡ...");

	::SetTimer(m_hWnd,uTimerID,1000,NULL);
}

//����������ȡ��֤��Ķ�ʱ��
void CUserInfoWnd::ResetReVCodeUI()
{
	//if (m_iReVCodeTime < CMobileVerifyWnd::GetVCodeValidTime();)
	{
		::KillTimer(m_hWnd,IDT_RE_GET_MOBILE_VCODE_1);
		::KillTimer(m_hWnd,IDT_RE_GET_MOBILE_VCODE_2);
		::KillTimer(m_hWnd,IDT_RE_GET_MOBILE_VCODE_3);
		CMainRoomEx::ClearMobileVCode();

		m_pVCodeBtn1->SetEnabled(true);
		m_pVCodeBtn1->SetText(_T("��ȡ��֤��"));

		m_pVCodeBtn2->SetEnabled(true);
		m_pVCodeBtn2->SetText(_T("��ȡ��֤��"));

		m_pVCodeBtn3->SetEnabled(true);
		m_pVCodeBtn3->SetText(_T("��ȡ��֤��"));

		CEditUI* pEdtCtrl = NULL;
		pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn_2_1_MobileNo")));
		pEdtCtrl->SetEnabled(true);

		m_iReVCodeTime = CMobileVerifyWnd::GetVCodeValidTime();
	}
}

//����/������Ϣ
void CUserInfoWnd::OnLockResult(bool bLock)
{
	bool bShowing = m_pPageContainer[PAGE_LOCK_MACHINE]->IsVisible();
	m_pPageContainer[PAGE_LOCK_MACHINE]->SetVisible(false);
	m_pPageContainer[PAGE_LOCK_MACHINE] = m_pLockMachineCtn[0];
	if (bShowing)
	{
		m_pPageContainer[PAGE_LOCK_MACHINE]->SetVisible(true);
	}
	

	CLabelUI* pLabCtrl = NULL;
	CButtonUI* pBntCtrl = NULL;
	TCHAR szTemp[BUFSIZ];

	//�󶨲�����ť
	pBntCtrl = static_cast<CButtonUI*>(m_pm.FindControl(_T("Bnt_Ctn3_PageNext")));
	if(bLock)
	{
		pBntCtrl->SetText(_TEXT("�����"));
		pBntCtrl->SetTag(0);
	}
	else
	{
		pBntCtrl->SetText(_TEXT("��Ҫ��"));
		pBntCtrl->SetTag(1);
	}

	//������־
	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl(_T("Lab_Ctn3_State")));
	if(bLock)
	{
		pLabCtrl->SetShowHtml(false);
		pLabCtrl->SetText(_TEXT("��ǰ״̬���Ѱ�"));
	}
	else
	{
		pLabCtrl->SetShowHtml(true);
		pLabCtrl->SetText(_TEXT("<c #FF0000><b>��ǰ״̬��δ��</b><c>"));
	}

}

//���ֻ����밴Ť��Ӧ
void CUserInfoWnd::OnBindMobile(bool bLock)
{
	CEditUI* pEdtCtrl = NULL;

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn4_1_VCode")));
	CStdString strVCode = pEdtCtrl->GetText();

	if(strVCode.GetLength() == 0)
	{
		ShowTipMessage(NULL,"","","�������ֻ���֤��");
		pEdtCtrl->SetFocus();
		return;
	}

	//�ڴ�����ֻ���֤
	if (!CMainRoomEx::ValidMobileVCode((char*)strVCode.GetData()))
	{
		ShowTipMessage(NULL,"","","�ֻ���֤�벻��ȷ");
		pEdtCtrl->SetFocus();
		return;
	}

	//�ֻ���֤ͨ��
	//�����л�ȡ�ֻ���֤��İ�ť��ԭ����֤����ȡ��ʱ���رյ�
	ResetReVCodeUI();

	pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn4_1_PhoneNo")));
	CStdString strMobileNo = pEdtCtrl->GetText();
	if (::strlen(m_pUserInfo->szMobileNo) == 0) //��һ�������ֻ���
	{
		//���޸Ĺ��ֻ���
		pEdtCtrl->SetEnabled(false);
		::lstrcpy(m_pUserInfo->szMobileNo,strMobileNo.GetData());

		//ͬ���ֻ��ŵ���ϸ����Tab�ġ��ֻ��š�����
		pEdtCtrl = static_cast<CEditUI*>(m_pm.FindControl(_T("Edt_Ctn2_MobileNo")));
		pEdtCtrl->SetText(strMobileNo);
	}


	MSG_GP_S_BindMobile BindMsg;
	::memset(&BindMsg,NULL,sizeof(MSG_GP_S_BindMobile));

	BindMsg.dwUserID = m_pUserInfo->dwUserID;
	BindMsg.dwCommanType = bLock ? 1 : 0;
	::lstrcpy(BindMsg.szMobileNo,m_pUserInfo->szMobileNo);

	//����������������
	GetMainRoom()->SendData(&BindMsg,sizeof(MSG_GP_S_BindMobile),
		MDM_GP_BIND_MOBILE,ASS_GP_BIND_MOBILE,0);

}

//���ֻ�������Ϣ
void CUserInfoWnd::OnBindMobileResult(bool bLock)
{
	bool bShowing = m_pPageContainer[PAGE_BIND_MOBILE]->IsVisible();
	m_pPageContainer[PAGE_BIND_MOBILE]->SetVisible(false);
	m_pPageContainer[PAGE_BIND_MOBILE] = m_pBindMobileCtn[0];
	if (bShowing)
	{
		m_pPageContainer[PAGE_BIND_MOBILE]->SetVisible(true);
	}


	CLabelUI* pLabCtrl = NULL;
	CButtonUI* pBntCtrl = NULL;
	TCHAR szTemp[BUFSIZ];

	//�󶨲�����ť
	pBntCtrl = static_cast<CButtonUI*>(m_pm.FindControl(_T("Bnt_Ctn4_PageNext")));
	if(bLock)
	{
		pBntCtrl->SetText(_TEXT("�����"));
		pBntCtrl->SetTag(0);
	}
	else
	{
		pBntCtrl->SetText(_TEXT("��Ҫ��"));
		pBntCtrl->SetTag(1);
	}

	//������־
	pLabCtrl = static_cast<CLabelUI*>(m_pm.FindControl(_T("Lab_Ctn4_State")));
	if(bLock)
	{
		pLabCtrl->SetShowHtml(false);
		pLabCtrl->SetText(_TEXT("��ǰ״̬���Ѱ�"));
	}
	else
	{
		pLabCtrl->SetShowHtml(true);
		pLabCtrl->SetText(_TEXT("<c #FF0000><b>��ǰ״̬��δ��</b><c>"));
	}
}
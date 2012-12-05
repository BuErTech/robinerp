#include "stdafx.h"
#include "UserDetailsDlg.h"
#include "GameRoomEx.h"

// CUserDetailsDlg �Ի���
HHOOK CUserDetailsDlg::m_hMHook  = NULL;
CWnd* CUserDetailsDlg::m_pActWnd = NULL;


//Avatarϵͳ���
#define BzGetAvatarIndex(obj) \
	GetAvatarIndexInRoom((obj)->GameUserInfo.bLogoID,(obj)->GameUserInfo.i64Bank,(obj)->GameUserInfo.i64Money,\
		(obj)->GameUserInfo.dwPoint,((CGameRoomEx*)m_pParentWnd)->m_GameInfo.uNameID,\
		((CGameRoomEx*)m_pParentWnd)->m_GameInfo.uComType)

// ////////////////////////////////////////////////////////////////////
// ��깴��
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) 
{ 
	bool bHide = false;

	if(nCode == HC_ACTION) 
	{ 
		PMOUSEHOOKSTRUCT mhs = (PMOUSEHOOKSTRUCT)lParam; 

		if (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN)
		{ 
			if (CUserDetailsDlg::m_pActWnd)
			{
				if (CUserDetailsDlg::m_pActWnd->IsWindowVisible())
				{
					// �����겻����ڱ�������, �����ظô���
					CRect rect;
					CUserDetailsDlg::m_pActWnd->GetWindowRect(&rect);
					if (!rect.PtInRect(CPoint(mhs->pt.x, mhs->pt.y)))
					{
						CUserDetailsDlg::m_pActWnd->ShowWindow(SW_HIDE);
						bHide = true;
					}
				}
			}
		}
	}

	LRESULT lRet = CallNextHookEx(CUserDetailsDlg::m_hMHook, nCode, wParam, lParam); 

	if (bHide)
	{
		UnhookWindowsHookEx(CUserDetailsDlg::m_hMHook);
	}

	return lRet;
}
///////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CUserDetailsDlg, CDialog)

CUserDetailsDlg::CUserDetailsDlg(CWnd* pParent /*=NULL*/)
	: m_isShow(0),CDialog(CUserDetailsDlg::IDD, pParent)
{
	memset(m_wszBuffer, 0, sizeof(m_wszBuffer));

	m_pParentWnd = pParent;
}

CUserDetailsDlg::~CUserDetailsDlg()
{
	// �ͷŹ���
// 	if (hMHook != NULL)
// 	{	
// 		UnhookWindowsHookEx(hMHook);
// 	}
}

void CUserDetailsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUserDetailsDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_EXCHANGE_SKIN,OnExchangeSkin)
END_MESSAGE_MAP()

// CUserDetailsDlg ��Ϣ�������
int CUserDetailsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	//g_pWnd = this;

	TCHAR szFileName[MAX_PATH];
	wsprintf(szFileName, "%s%sdialog\\UserDetails_BK.bmp", CBcfFile::GetAppPath(true), m_skinmgr.GetSkinPath());

	// ��������
	m_BackBitmap.SetLoadInfo(szFileName, CGameImageLink::m_bAutoLock);
	CGameImageHelper ImageHandle(&m_BackBitmap);
	HRGN hRgn = AFCBmpToRgn(ImageHandle, RGB(255, 0, 255), RGB(1, 1, 1));
	if (hRgn!=NULL)
	{
		SetWindowPos(NULL, 0, 0, ImageHandle.GetWidth(), ImageHandle.GetHeight(), SWP_NOMOVE);
		SetWindowRgn(hRgn, TRUE);
		DeleteObject(hRgn);  // �ͷ��������
	}

	wsprintf(szFileName, "%s%sdialog\\log_exit_bt.png", CBcfFile::GetAppPath(true),m_skinmgr.GetSkinPath());

	// �����رհ�ť
	m_btClose.Create("", WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|BS_OWNERDRAW, CRect(0,0,0,0), this, IDCANCEL);
	m_btClose.LoadButtonBitmap(szFileName, false);
	CGameImage image;
	image.SetLoadInfo(szFileName, CGameImageLink::m_bAutoLock);
	m_btClose.MoveWindow(ImageHandle.GetWidth() - image.GetWidth() / 4, 0, image.GetWidth() / 4, image.GetHeight());
	

	CString s = CBcfFile::GetAppPath (); // ����·��
	CBcfFile f(s + m_skinmgr.GetSkinBcfFileName());

	// ��ǩ���ı��������������ɫֵ
	CString key= TEXT("UserDetailsDlg");
	int r,g,b;
	r = f.GetKeyVal(key, "LabelR", 0);
	g = f.GetKeyVal(key, "LabelG", 0);
	b = f.GetKeyVal(key, "LabelB", 0);
	m_clrLabel = RGB(r, g, b);

	r = f.GetKeyVal(key, "TextR", 0);
	g = f.GetKeyVal(key, "TextG", 0);
	b = f.GetKeyVal(key, "TextB", 0);
	m_clrText = RGB(r, g, b);

	// ���ػ������ͺ�ս�������ʽ
	CBcfFile f1(s + "ClientMessage.bcf");
	m_strMoneyType = f1.GetKeyVal(key, "MoneyType", "��    ��");
	m_strCombatGainsFormat = f1.GetKeyVal(key, "CombatGainsFormat", "Ӯ%d  ��%d  ��%d");
	m_isShow = f1.GetKeyVal(key,"IsShowCombatGains",0);

	// ������깴��
	//m_hMHook = SetWindowsHookEx(WH_MOUSE, (HOOKPROC)MouseHookProc, AfxGetInstanceHandle(), AfxGetThread()->m_nThreadID);
	return 0;
}

BOOL CUserDetailsDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// �滭��ͼ
	if (!m_BackBitmap.IsNull())
	{
		CRect ClientRect;
		GetClientRect(&ClientRect);
		CGameImageHelper ImageHandle(&m_BackBitmap);
		ImageHandle.BitBlt(pDC->GetSafeHdc(), 0, 0);
	}

	return TRUE;
}

void CUserDetailsDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	if (m_pUserItem != NULL)
	{
		dc.SetBkMode(TRANSPARENT);

		TCHAR szFileName[MAX_PATH];
		CString strText;
		int x = 107;
		int y = 58;

		CFont * pFont, *pOldFont = NULL;
		pFont = new CFont();
		pFont->CreateFont(15,         // nHeight 
			0,						  // nWidth 
			0,						  // nEscapement�ı�����ʱ����ת�Ƕ� 
			0,						  // nOrientation����Ƕ�
			400,				      // nWeight�����ϸ�̶�  FW_BOLD
			FALSE,					  // bItalic 
			FALSE,					  // bUnderline 
			0,						  // cStrikeOut ɾ����
			ANSI_CHARSET,			  // nCharSet ANSI_CHARSET
			OUT_DEFAULT_PRECIS,		  // nOutPrecision 
			CLIP_DEFAULT_PRECIS,	  // nClipPrecision 
			DEFAULT_QUALITY,		  // nQuality 
			DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
			_T("����_GB2312"));		  // lpszFac 

		pOldFont = dc.SelectObject(pFont);

		// ����ͷ��
		Graphics graphics(dc);
		Image image(m_wszBuffer);   // Construct an image
		graphics.DrawImage(&image, (REAL)31, (REAL)63, (REAL)60, (REAL)61);
		
		// �����û�����	
		dc.SetTextColor(m_clrLabel);
		strText.Format("��  ��  ��: ");
		dc.DrawText(strText, CRect(x, y, x + 56, y + 20), DT_LEFT);

		dc.SetTextColor(m_clrText);
		strText.Format("%s", m_pUserItem->GameUserInfo.nickName);
		dc.DrawText(strText, CRect(x + 56, y, x + 150, y + 20), DT_LEFT);

		y += 20;
		// �����û�ID
		dc.SetTextColor(m_clrLabel);
		strText.Format("��  ��  ID: ");
		dc.DrawText(strText, CRect(x, y, x + 56, y + 20), DT_LEFT);

		dc.SetTextColor(m_clrText);
		strText.Format("%d", m_pUserItem->GameUserInfo.dwUserID);
		dc.DrawText(strText, CRect(x + 56, y, x + 150, y + 20), DT_LEFT);

		y += 20;
		// ���Ƶ�ǰ״̬
		dc.SetTextColor(m_clrLabel);
		strText = "��ǰ״̬: ";
		dc.DrawText(strText, CRect(x, y, x + 56, y + 20), DT_LEFT);

		switch(m_pUserItem->GameUserInfo.bUserState)
		{	
		case USER_NO_STATE:
			strText = "���ڻ�ȡ���״̬";
			break;
		case USER_LOOK_STATE:
			strText = "δ����";
			break;
		case USER_SITTING:
			strText = "����Ϸ��";
			break;
		case USER_ARGEE:
			strText = "ͬ��״̬";
			break;
		case USER_WATCH_GAME:
			strText = "�Թ���Ϸ";
			break;
		case USER_CUT_GAME:
			strText = "����״̬";
			break;
		case USER_PLAY_GAME:
			strText = "��Ϸ��";
			break;
		default:
			strText = "���ڻ�ȡ���״̬";
			break;
		}

		dc.SetTextColor(m_clrText);
		dc.DrawText(strText, CRect(x + 56, y, x + 150, y + 20), DT_LEFT);

		y += 20;
		// ���Ƹ���ǩ��
		dc.SetTextColor(m_clrLabel);
		strText = "����ǩ��:";
		dc.DrawText(strText, CRect(x, y, x + 56, y + 20), DT_LEFT);

		dc.SetTextColor(m_clrText);
		strText.Format("                                              %s", m_pUserItem->GameUserInfo.szSignDescr);
		dc.DrawText(strText, CRect(26/*x + 56*/, y, x + 56 + 100, y + 80), DT_LEFT | DT_WORDBREAK);

		x = 36;
		y = 210;//203;
		// ���ƻ���
		dc.SetTextColor(m_clrLabel);
		strText.Format("��    ��: ");
		dc.DrawText(strText, CRect(x, y, x + 45, y + 20), DT_LEFT);

		dc.SetTextColor(m_clrText);
		TCHAR szNum[128] = {0}; 
		GetNumString(szNum, m_pUserItem->GameUserInfo.dwPoint, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
		strText.Format("%s", szNum);
		dc.DrawText(strText, CRect(x + 45, y, x + 160, y + 20), DT_LEFT);

		// ���ƲƸ��ȼ�
		dc.SetTextColor(m_clrLabel);
		strText.Format("��    ��: ");
		dc.DrawText(strText, CRect(x + 130, y, x + 130 + 45, y + 20), DT_LEFT);

		dc.SetTextColor(m_clrText);
		strText.Format("%s", GetMoneyOrderName(m_pUserItem->GameUserInfo.i64Money,((CGameRoomEx*)m_pParentWnd)->m_GameInfo.uNameID));
		dc.DrawText(strText, CRect(x + 130 + 45, y, x + 130 + 100, y + 20), DT_LEFT);

		y += 20;
		// ���ƽ��
		dc.SetTextColor(m_clrLabel);
		strText.Format("%s: ", m_strMoneyType);
		dc.DrawText(strText, CRect(x, y, x + 45, y + 20), DT_LEFT);

		dc.SetTextColor(m_clrText);
		GetNumString(szNum, m_pUserItem->GameUserInfo.i64Money, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
		strText.Format("%s", szNum);
		dc.DrawText(strText, CRect(x + 45, y, x + 160, y + 20), DT_LEFT);

		if (1 == m_isShow)
		{
			y += 20;
			// ����ս��
			dc.SetTextColor(m_clrLabel);
			strText.Format("ս    ��: ");
			dc.DrawText(strText, CRect(x, y, x + 45, y + 20), DT_LEFT);

			dc.SetTextColor(m_clrText);
			strText.Format(m_strCombatGainsFormat, m_pUserItem->GameUserInfo.uWinCount, m_pUserItem->GameUserInfo.uLostCount, m_pUserItem->GameUserInfo.uMidCount, m_pUserItem->GameUserInfo.uCutCount);
			dc.DrawText(strText, CRect(x + 45, y, x + 180, y + 20), DT_LEFT);
		}

		int iCountTotal = m_pUserItem->GameUserInfo.uWinCount + m_pUserItem->GameUserInfo.uLostCount + m_pUserItem->GameUserInfo.uMidCount;
		int tempwin = (iCountTotal == 0) ? 0 : m_pUserItem->GameUserInfo.uWinCount * 100 / iCountTotal;
		int temprun = (iCountTotal == 0) ? 0 : m_pUserItem->GameUserInfo.uCutCount * 100 / iCountTotal;
		
		y += 20;
		// ����ʤ��
		dc.SetTextColor(m_clrLabel);
		strText.Format("ʤ    ��: ");
		dc.DrawText(strText, CRect(x, y, x + 45, y + 20), DT_LEFT);

		dc.SetTextColor(m_clrText);
		strText.Format("%d%%", tempwin);
		dc.DrawText(strText, CRect(x + 45, y, x + 150, y + 20), DT_LEFT);

		y += 20;
		// ����������
		dc.SetTextColor(m_clrLabel);
		strText.Format("������: ");
		dc.DrawText(strText, CRect(x, y, x + 45, y + 20), DT_LEFT);

		dc.SetTextColor(m_clrText);
		strText.Format("%d%%", temprun);
		dc.DrawText(strText, CRect(x + 45, y, x + 150, y + 20), DT_LEFT);

		dc.SelectObject(pOldFont);

		pFont->DeleteObject();
		delete pFont;
		pFont = NULL;
	}
}

void CUserDetailsDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// ģ�ⰴ����
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));

	CDialog::OnLButtonDown(nFlags, point);
}

void CUserDetailsDlg::SetUserItem(UserItemStruct * pUserItem)
{
	m_pUserItem = pUserItem;

	// ��������ͷ��
	TCHAR szBuffer[MAX_PATH];
	memset(szBuffer,0,sizeof(szBuffer));
	memset(m_wszBuffer, 0, sizeof(m_wszBuffer));

	// �滭����ͷ��
	if (pUserItem->GameUserInfo.bLogoID >= 0xFF && pUserItem->GameUserInfo.bLogoID < 0x200)
	{
		wsprintf(szBuffer,TEXT("%sCustomFace\\%d.png"),CBcfFile::GetAppPath(),(pUserItem->GameUserInfo.dwUserID)%6);
		CFileFind ff;
		BOOL bFoundFile=ff.FindFile(szBuffer);
		ff.Close();

		if(!bFoundFile)
		{	
			wsprintf(szBuffer,TEXT("%simage\\log\\%s\\gamelog%ld_big.png"),CBcfFile::GetAppPath(), pUserItem->GameUserInfo.bBoy?"Boy":"Girl",BzGetAvatarIndex(pUserItem));
		}
	}
	else
	{
		wsprintf(szBuffer,TEXT("%simage\\log\\%s\\gamelog%ld_big.png"),CBcfFile::GetAppPath(), pUserItem->GameUserInfo.bBoy?"Boy":"Girl",BzGetAvatarIndex(pUserItem));
	}

	if (!CBcfFile::IsFileExist(szBuffer))
	{
		wsprintf(szBuffer,TEXT("%simage\\log\\%s\\gamelog%ld_big.png"),CBcfFile::GetAppPath(), pUserItem->GameUserInfo.bBoy?"Boy":"Girl",0/*pUserItem->GameUserInfo.bLogoID%12*/);
	}

	MultiByteToWideChar(CP_ACP,MB_COMPOSITE, szBuffer,-1, m_wszBuffer, sizeof(m_wszBuffer));

	Invalidate();
}
//����
LRESULT CUserDetailsDlg::OnExchangeSkin(WPARAM wpara,LPARAM lpara)
{

	TCHAR szFileName[MAX_PATH];
	wsprintf(szFileName, "%s%sdialog\\UserDetails_BK.bmp", CBcfFile::GetAppPath(true), m_skinmgr.GetSkinPath());

	// ��������
	m_BackBitmap.SetLoadInfo(szFileName, CGameImageLink::m_bAutoLock);
	//CGameImageHelper ImageHandle(&m_BackBitmap);
	//HRGN hRgn = AFCBmpToRgn(ImageHandle, RGB(255, 0, 255), RGB(1, 1, 1));
	//if (hRgn!=NULL)
	//{
	//	SetWindowPos(NULL, 0, 0, ImageHandle.GetWidth(), ImageHandle.GetHeight(), SWP_NOMOVE);
	//	SetWindowRgn(hRgn, TRUE);
	//	DeleteObject(hRgn);  // �ͷ��������
	//}

	wsprintf(szFileName, "%s%sdialog\\log_exit_bt.png", CBcfFile::GetAppPath(true),m_skinmgr.GetSkinPath());

	// �����رհ�ť
	//m_btClose.Create("", WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|BS_OWNERDRAW, CRect(0,0,0,0), this, IDCANCEL);
	m_btClose.LoadButtonBitmap(szFileName, false);
	CGameImage image;
	image.SetLoadInfo(szFileName, CGameImageLink::m_bAutoLock);
	//m_btClose.MoveWindow(ImageHandle.GetWidth() - image.GetWidth() / 4, 0, image.GetWidth() / 4, image.GetHeight());


	CString s = CBcfFile::GetAppPath (); // ����·��
	CBcfFile f(s + m_skinmgr.GetSkinBcfFileName());

	// ��ǩ���ı��������������ɫֵ
	CString key= TEXT("UserDetailsDlg");
	int r,g,b;
	r = f.GetKeyVal(key, "LabelR", 0);
	g = f.GetKeyVal(key, "LabelG", 0);
	b = f.GetKeyVal(key, "LabelB", 0);
	m_clrLabel = RGB(r, g, b);

	r = f.GetKeyVal(key, "TextR", 0);
	g = f.GetKeyVal(key, "TextG", 0);
	b = f.GetKeyVal(key, "TextB", 0);
	m_clrText = RGB(r, g, b);

	// ���ػ������ͺ�ս�������ʽ
	CBcfFile f1(s + "ClientMessage.bcf");
	m_strMoneyType = f1.GetKeyVal(key, "MoneyType", "��    ��");
	m_strCombatGainsFormat = f1.GetKeyVal(key, "CombatGainsFormat", "Ӯ%d  ��%d  ��%d");

	Invalidate();
	return LRESULT(0);
}

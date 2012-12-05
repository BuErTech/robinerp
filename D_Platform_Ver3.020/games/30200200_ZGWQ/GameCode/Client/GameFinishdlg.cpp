#include "StdAfx.h"
#include "GameFinishDlg.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern __int64 GetPowerofGold();

BEGIN_MESSAGE_MAP(CGameFinishDlg, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//���캯��
CGameFinishDlg::CGameFinishDlg(void)// :CGameFaceGo(CGameFinishDlg::IDD)
{
	AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));


	m_nPowerOfGold=1;
	AfxSetResourceHandle(GetModuleHandle(NULL));
}
//
int CGameFinishDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	TCHAR szName[MAX_PATH];
	wsprintf(szName,TEXT(".\\image\\%s\\result.bmp"),SKIN_FOLDER);
	m_bk.SetLoadInfo(szName,TRUE);
	
	wsprintf(szName,".\\image\\%s\\win.bmp",SKIN_FOLDER);
	m_Flag.SetLoadInfo(szName,false);

	CRect ClientRect;
	GetClientRect(&ClientRect);	
	CGameImageHelper	help(&m_bk);
	MoveWindow(ClientRect.left,ClientRect.top,help.GetWidth(),help.GetHeight());
	HRGN hRgn=AFCBmpToRgn(help,RGB(255,0,255),RGB(1,0,1));
	if (hRgn!=NULL)
	{
		SetWindowPos(NULL,0,0,help.GetWidth(),help.GetHeight(),SWP_NOMOVE);
		SetWindowRgn(hRgn,TRUE);
	}

	CString s = CINIFile::GetAppPath ();/////����·��	
	DWORD cfgHandle;
#ifdef	VIDEO
	cfgHandle=cfgOpenFile(s +"30200205.bcf");
#else
	cfgHandle=cfgOpenFile(s +"30200200.bcf");
#endif
	if(cfgHandle<0x10)	
		return -1;		

#ifdef	VIDEO
	CBcfFile f( "30200205.bcf");
#else
	CBcfFile f( "30200200.bcf");
#endif
	
	CString key = TEXT("config");
	CString key1 = TEXT("uiset");	
	CString sec = _T("");
	
	wsprintf(szName,".\\image\\win.bmp");
	m_Flag.SetLoadInfo(szName,false);

	m_iGameEndX = f.GetKeyVal(key1,TEXT("GameEndX"), 24);
	m_iGameEndY = f.GetKeyVal(key1,TEXT("GameEndY"), 96);	
	m_iGameEndWinLostX = f.GetKeyVal(key1,TEXT("GameEndWinLostX"), 226);
	m_iGameEndWinLostY = f.GetKeyVal(key1,TEXT("GameEndWinLostY"), 108);

	cfgClose(cfgHandle);
	return 0;
}
//��������
CGameFinishDlg::~CGameFinishDlg(void)
{
}

//�ػ�����
void CGameFinishDlg::OnPaint() 
{
	CPaintDC dc(this); 
	
	CGameImageHelper	help(&m_bk);	
	help.BitBlt(dc.GetSafeHdc(),0,0);	
	
	CRect rect;
	CString sz;
	CFont Font;

	Font.CreateFont(14,0,0,0,0,0,0,0,134,3,2,1,2,TEXT("����"));
	CFont *OldFont = dc.SelectObject(&Font);
	dc.SetTextColor(RGB(255,255,255));
	dc.SetBkMode(TRANSPARENT);	

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		if(strlen(m_iFinish.name[i]) <= 0)
			continue;
		if(m_iViewStation==i)
			dc.SetTextColor(RGB(255,0,0));
		else
			dc.SetTextColor(RGB(0,255,0));

		sz.Format("%s",m_iFinish.name[i]);
		rect.left=m_iGameEndX;
		rect.right=rect.left+80;
		rect.top=m_iGameEndY+33*i;
		rect.bottom=rect.top+33;
		dc.DrawText(sz.GetBuffer(),sz.GetLength(),&rect,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	
		sz.Format("%I64d",m_iFinish.iTurePoint[i]);
		rect.left=m_iGameEndX+204;
		rect.right=rect.left+70;
		rect.top=m_iGameEndY+33*i;
		rect.bottom=rect.top+33;
		dc.DrawText(sz.GetBuffer(),sz.GetLength(),&rect,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);	

		sz.Format("%I64d",m_iFinish.iMoney[i]*GetPowerofGold());
		rect.left=m_iGameEndX+278;//JMod-20090109-//250;
		rect.right=rect.left+90;
		rect.top=m_iGameEndY+33*i;
		rect.bottom=rect.top+33;
		dc.DrawText(sz.GetBuffer(),sz.GetLength(),&rect,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
		
		if(m_iFinish.iTurePoint[i] > 0 || m_iFinish.iMoney[i] > 0)
			sz = ".\\image\\win.bmp";
		else if(m_iFinish.iTurePoint[i] == 0 && m_iFinish.iMoney[i] == 0)
			sz = ".\\image\\equal.bmp";
		else
			sz = ".\\image\\lost.bmp";		
		m_Flag.SetLoadInfo(sz.GetBuffer(),false);
		CGameImageHelper flag(&m_Flag);
		CDC srcDC;
		srcDC.CreateCompatibleDC(&dc);
		CBitmap *OldBmp = srcDC.SelectObject(flag);		
		::TransparentBlt(dc.GetSafeHdc(),m_iGameEndWinLostX, m_iGameEndWinLostY + 33 * i,flag.GetWidth(),flag.GetHeight(),
			srcDC.GetSafeHdc(),0,0,m_Flag.GetWidth(),m_Flag.GetHeight(),srcDC.GetPixel(0,0));//JMod-20090109-//StartX,StartY+ 33* i
		srcDC.SelectObject(OldBmp);
		srcDC.DeleteDC();
	}
	Font.DeleteObject();
	return ;
}

//�Զ�������С
void CGameFinishDlg::AutoSize(int xValue,int yValue)
{
	MoveWindow(xValue,yValue,m_bk.GetWidth(),m_bk.GetHeight());
	/*Invalidate();
	UpdateWindow();*/
}


void CGameFinishDlg::SetMessage(GameFinishNotify finishnotify,BYTE iViewStation)
{

	m_iFinish=finishnotify;
	m_iViewStation=iViewStation;
	TCHAR szName[MAX_PATH];

	wsprintf(szName,TEXT(".\\image\\%s\\result.bmp"),SKIN_FOLDER);

	m_bk.SetLoadInfo(szName,TRUE);
	/*Invalidate(TRUE);
	UpdateWindow();*/
	return;
}

void CGameFinishDlg::OnLButtonDown(UINT nFags,CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	return ;
}
/**
* ����ǰ��ұ��ʵõ��ַ���
* @param [in] int nNum Ҫ��ʾ������
* @param [out] CString &Str ���ص��ַ���
* @param [in] int nPower ��ұ���
* @param [in] bool bIsBlank �����Ƿ�ӿո�
* @return void
*/
void CGameFinishDlg::GetNumString(int nNum, CString &Str, int nPower,bool bIsBlank)
{
	if (0 == nNum)
	{
		Str.Format("0");
		return;
	}
	/// �ݴ���
	TCHAR strTmp[250];
	TCHAR str[250];
	/// Ŀǰֻ�������0��������������ַ�������Ҫ������һ��0
	wsprintf( strTmp, TEXT("%d"), nNum);
	for (int i=0; i<nPower; ++i)
	{
		_tcscat( strTmp, TEXT("0"));
	}
	/// ��λ��ÿ��λ��һ����ʾ
	int nLen = _tcslen(strTmp);	// �ַ�������
	int nTimes = nLen % 3;	// ����ַ������ԷֳɵĽ�����������1��

	int nSrc = 0;
	int nDes = 0;

	for (int i=0; i<nLen; ++i)
	{
		if ((i>0)&&(i%3==nTimes)&&bIsBlank)
		{
			str[nDes++] = TEXT(' ');
		}
		str[nDes++] = strTmp[nSrc++];
	}
	str[nDes] = TEXT('\0');
	Str.Format("%s",str);
	return ;
}


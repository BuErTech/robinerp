#include "StdAfx.h"
#include "GameFinishDlg.h"
#include "Resource.h"
#include "ClientGameDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CGameFinishDlg, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//���캯��
CGameFinishDlg::CGameFinishDlg(void) //:CGameFaceGo(CGameFinishDlg::IDD)
{
	//AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));
	m_pParnet=NULL;
	//AfxSetResourceHandle(GetModuleHandle(NULL));
	m_iCx=0;
	m_iCy=0;
	m_nPowerOfGold=0;
}

//��������
CGameFinishDlg::~CGameFinishDlg(void)
{
}

//�ػ�����
void CGameFinishDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CDC srcdc;
	CGameImageHelper help(&m_bk);
	help.BitBlt(dc.GetSafeHdc(),0,0);
	
	CGameImageHelper	win(&m_win);
	CGameImageHelper	lost(&m_lost);
	CGameImageHelper	equal(&m_equal);
	srcdc.CreateCompatibleDC(&dc);
	srcdc.SelectObject(help);

	CFont Font;
	CRect rect;
	TCHAR szMsg[50]="";
	Font.CreateFont(14,0,0,0,600,NULL,NULL,NULL,134,3,2,1,2,TEXT("����"));

	CFont *OldFont=dc.SelectObject(&Font);
	CString str;
	dc.SetTextColor(RGB(0,255,30));
	int x = 15;
	int y = 107;
	int YAdd = 34;
	int winX=240;
	int winY=97;

	dc.SetBkMode(TRANSPARENT);
	
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_iFinish.iCardShape[i] == 100)
			continue;
		if(m_iViewStation == i)
		{
			dc.SetTextColor(RGB(255,0,0));
		}
		else
		{
			dc.SetTextColor(RGB(0,0,0));
		}

		wsprintf(szMsg,"%s",m_iFinish.name[i]);//����
		rect.left=x+7;
		rect.right=rect.left+90;
		rect.top=y+YAdd*i-5;
		rect.bottom=rect.top+18;
		dc.DrawText(szMsg,lstrlen(szMsg),&rect,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		wsprintf(szMsg,m_ShapeName[m_iFinish.iCardShape[i]]);
		rect.left=122;
		rect.right=rect.left+80;
		rect.top=y+YAdd*i-5;
		rect.bottom=rect.top+18;
		dc.DrawText(szMsg,lstrlen(szMsg),&rect,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
		
		wsprintf(szMsg,"%I64d",m_iFinish.iWardPoint[i]);//�÷�
		rect.left=282;
		rect.right=rect.left+110 + 40;
		rect.top=y+YAdd*i-5;
		rect.bottom=rect.top+18;   
		dc.DrawText(szMsg,lstrlen(szMsg),&rect,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		if (m_iFinish.iWardPoint[i] > 0||m_iFinish.iMoney[i]>0)
		{
			srcdc.SelectObject(win);
			::TransparentBlt(dc.GetSafeHdc(),winX,winY+i*34,48,26,srcdc,0,0,win.GetWidth(),win.GetHeight(),srcdc.GetPixel(0,0));
		}
		else if(m_iFinish.iWardPoint[i] ==0&&m_iFinish.iMoney[i] ==0 )
		{
			srcdc.SelectObject(equal);
			::TransparentBlt(dc.GetSafeHdc(),winX,winY+i*34,48,26,srcdc,0,0,equal.GetWidth(),equal.GetHeight(),srcdc.GetPixel(0,0));
		}
		else
		{
			srcdc.SelectObject(lost);
			::TransparentBlt(dc.GetSafeHdc(),winX,winY+i*34,48,26,srcdc,0,0,lost.GetWidth(),lost.GetHeight(),srcdc.GetPixel(0,0));
			
		}

		CString strMoney;
		GetNumString(m_iFinish.iMoney[i],strMoney,m_nPowerOfGold);
		wsprintf(szMsg,"%s",strMoney);//����
		rect.left=380;
		rect.right=rect.left+110 + 40;
		rect.top=y+YAdd*i-5;
		rect.bottom=rect.top+18;
		dc.DrawText(szMsg,lstrlen(szMsg),&rect,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	}

	srcdc.DeleteDC();
	dc.SelectObject(OldFont);
}

void CGameFinishDlg::SetMessage(GameFinishNotify finishnotify,BYTE iViewStation, int nPowerOfGold)
{
	m_iFinish=finishnotify;
	m_iViewStation=iViewStation;
	m_nPowerOfGold = nPowerOfGold;
	return;
}

void CGameFinishDlg::OnLButtonDown(UINT nFags,CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	return ;
}

int CGameFinishDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	TCHAR szName[MAX_PATH];
	wsprintf(szName,".\\image\\result.bmp");
	m_bk.SetLoadInfo(szName,false);


	wsprintf(szName,".\\image\\win.bmp");
	m_win.SetLoadInfo(szName,false);

	wsprintf(szName,".\\image\\lost.bmp");
	m_lost.SetLoadInfo(szName,false);

	wsprintf(szName,".\\image\\equal.bmp");
	m_equal.SetLoadInfo(szName,false);


	m_ShapeName[0] = TEXT("ţ��");
	m_ShapeName[1] = TEXT("ţһ");
	m_ShapeName[2] = TEXT("ţ��");
	m_ShapeName[3] = TEXT("ţ��");
	m_ShapeName[4] = TEXT("ţ��");
	m_ShapeName[5] = TEXT("ţ��");
	m_ShapeName[6] = TEXT("ţ��");
	m_ShapeName[7] = TEXT("ţ��");
	m_ShapeName[8] = TEXT("ţ��");
	m_ShapeName[9] = TEXT("ţ��");
	m_ShapeName[10] = TEXT("ţţ");
	m_ShapeName[11] = TEXT("��ţ");
	m_ShapeName[12] = TEXT("��ţ");
	m_ShapeName[13] = TEXT("��С");
	m_ShapeName[14] = TEXT("ը��");

	// TODO:  �ڴ������ר�õĴ�������
	CGameImageHelper help(&m_bk);
	HRGN hRgn=AFCBmpToRgn(help,RGB(255,0,255),RGB(1,1,1));     //���˵�RGB(255,0,255)����ɫ����
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);        //����Ϊû�й��˵�������
	}
	CRect rect;
	GetClientRect(&rect);
	this->MoveWindow(0,0,help.GetWidth(),help.GetHeight());

	return 0;
}
///
///����ǰ��ұ��ʵõ��ַ���
///@param [int] int nNum Ҫ��ʾ������
///@param [out] CString &Str ���ص��ַ���
///@param [int] int nPower ��ұ���
///@return void
///
void CGameFinishDlg::GetNumString(__int64 nNum, CString &Str, int nPower)
{
	if (0 == nNum)
	{
		Str.Format("0");
		return;
	}
	/// �ݴ���
	TCHAR strTmp[64];
    TCHAR str[50];
	/// Ŀǰֻ�������0��������������ַ�������Ҫ������һ��0
	wsprintf( strTmp, TEXT("%I64d"), nNum);
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
		//if ((i>0)&&(i%3==nTimes))
		//{
			//str[nDes++] = TEXT(' ');///ţţ���ӿո�
		//}
		str[nDes++] = strTmp[nSrc++];
	}
	str[nDes] = TEXT('\0');
	Str.Format("%s",str);
	return ;
}

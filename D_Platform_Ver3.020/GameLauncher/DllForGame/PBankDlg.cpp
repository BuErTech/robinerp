// PBankDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PBankDlg.h"
#include "resource.h"
#include "Platform/bzsound.h"
#include "NewGameFrame\XLogic.h"

// CPBankDlg �Ի���

IMPLEMENT_DYNAMIC(CPBankDlg, CDialog)

CPBankDlg::CPBankDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_BANK, pParent)
{
	m_pbgImg = NULL;
	m_pGameRoom = pParent;
	m_uCheckInOrOut = 0;
	m_bkBrush=NULL;
	m_strSaveID = "";
	m_strNickName = "";
    m_iUseNickName = 0;
	m_pLogic = NULL;
	m_bUseDestID = true;
}

CPBankDlg::~CPBankDlg()
{
	delete m_pbgImg;
	DeleteObject(m_bkBrush);
}

void CPBankDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_BtClose);
	DDX_Control(pDX, IDC_BTNINFOR, m_btInfor);
	DDX_Control(pDX, IDC_BTNTRANSFER, m_bnTrans);
	DDX_Control(pDX, IDC_BTNRECORD, m_bnRecord);
	DDX_Control(pDX, IDC_BTNSAFE, m_bnSafe);
	DDX_Control(pDX, IDC_BTNPW, m_bnPW);
	DDX_Control(pDX, IDC_BTN_TRANOK, m_btnTranOK);
	DDX_Control(pDX, IDC_SKINLIST, m_SkinList);
	DDX_Control(pDX, IDC_BTNREFRESH, m_btnRefresh);
	DDX_Control(pDX, IDC_BTNCLOSE, m_btnRcoClose);
	DDX_Control(pDX, IDC_BTNBOK, m_btnBok);
	DDX_Control(pDX, IDC_BTNBACTIVE, m_btnBactive);
	DDX_Control(pDX, IDC_BTNBCHARGE, m_btnBcharge);
	DDX_Control(pDX, IDC_BTNCHGOK, m_btnChOK);
	DDX_Control(pDX, IDC_BTNCHGCLOSE, m_btnChClose);
	DDX_Control(pDX, IDC_BTN_COIN_ALL, m_btnCoinAll);
}


BEGIN_MESSAGE_MAP(CPBankDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTNINFOR,&CPBankDlg::OnBnClickedInfor)
	ON_BN_CLICKED(IDC_BTNTRANSFER,&CPBankDlg::OnBnClickedTrans)
	ON_BN_CLICKED(IDC_BTNRECORD,&CPBankDlg::OnBnClickedRecord)
	ON_BN_CLICKED(IDC_BTNSAFE,&CPBankDlg::OnBnClickedSafe)
	ON_BN_CLICKED(IDC_BTNPW,&CPBankDlg::OnBnClickedPassWord)

	ON_BN_CLICKED(IDC_BTNCHGCLOSE,&CDialog::OnCancel)
	ON_BN_CLICKED(IDC_BTNCLOSE,&CDialog::OnCancel)

	ON_BN_CLICKED(IDC_RSAVE,OnBnClickedSave)
	ON_BN_CLICKED(IDC_RGET,OnBnClickedGet)

	ON_BN_CLICKED(IDC_RTRANS_ID,OnBnClickedRTransId)
	ON_BN_CLICKED(IDC_RTRANS_NN,OnBnClickedRTransNn)

	ON_BN_CLICKED(IDC_BTNCHGOK,OnBnClickedChPW)
	ON_BN_CLICKED(IDC_BTN_TRANOK,OnBnClickedAccountOK)

	ON_BN_CLICKED(IDC_BTNREFRESH,RefreshRecordList)

	ON_BN_CLICKED(IDC_BTNBOK,OnBnClickedBankOK)
	ON_BN_CLICKED(IDC_BTNBACTIVE,OnBnClickedBankActive)
	ON_BN_CLICKED(IDC_BTNBCHARGE,OnBnClickedBankRecharge)

	ON_BN_CLICKED(IDC_BTN_COIN_ALL, OnBnClickedCoinAll)

	ON_EN_KILLFOCUS(IDC_ETOTHERID, OnIDKillFocus)
	ON_EN_CHANGE(IDC_ETOUTCOIN, &CPBankDlg::OnEnChangeEtbconnoin)
	ON_EN_CHANGE(IDC_ETBCONNOIN, &CPBankDlg::OnEnChangeMoney)
END_MESSAGE_MAP()


// CPBankDlg ��Ϣ�������
void CPBankDlg::OnPaint()
{
	CPaintDC dc(this);
	Graphics pGrap(dc.GetSafeHdc());
	pGrap.DrawImage(m_pbgImg,Rect(0,0,m_pbgImg->GetWidth(),m_pbgImg->GetHeight()),0,0,m_pbgImg->GetWidth(),m_pbgImg->GetHeight(),UnitPixel);
}

BOOL CPBankDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f(CBcfFile::GetAppPath () + "..\\" + strSkin);
	TCHAR path[MAX_PATH];
	CString skinfolder = f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath());
	wsprintf(path,"..\\%sdialog\\BankDlg\\wealth.png",skinfolder);
	WCHAR wpath[MAX_PATH] = {0};
	MultiByteToWideChar(CP_ACP,0,path,strlen(path),wpath,256);
	if (NULL == m_pbgImg)
	{
#ifndef BZ_ZLIB
		m_pbgImg = new Image(wpath);
#else
		CUnZipRes unZipRes;
		IStream * pStream=NULL;
		pStream=unZipRes.LoadFileResFromZip(path);
		m_pbgImg=new Image(pStream);
		if(pStream!=NULL)
		{
			pStream->Release();
			pStream=NULL;
		}
		
#endif
		
	}
	
	if (NULL != m_pbgImg)
	{
		SetWindowPos(NULL,0,0,m_pbgImg->GetWidth(),m_pbgImg->GetHeight(),SWP_NOMOVE|SWP_NOZORDER);
	}
	{
		DWORD color = GetBackGroundColor();
		m_SkinList.SetColor(color);

		LOGFONT lf;
		memset(&lf, 0, sizeof(LOGFONT));
		lf.lfHeight = 19;
		strcpy(lf.lfFaceName, "Sevenet 7");
		CFont font;
		font.CreateFontIndirect(&lf);
		m_SkinList.SetFont(&font, TRUE);

		CBcfFile fMsg(CBcfFile::GetAppPath()+"..\\ClientMessage.bcf");

		m_SkinList.InsertColumn(0, fMsg.GetKeyVal("PBankDlg","TransferTime","����ʱ��"), LVCFMT_LEFT, 82);
		m_SkinList.InsertColumn(1, fMsg.GetKeyVal("PBankDlg","PayerID","֧����ID"), LVCFMT_LEFT, 82);
		m_SkinList.InsertColumn(2, fMsg.GetKeyVal("PBankDlg","PayeeID","�տ���ID"), LVCFMT_LEFT, 82);
		m_SkinList.InsertColumn(3, fMsg.GetKeyVal("PBankDlg","Payment","֧�����"), LVCFMT_LEFT, 65);


		m_SkinList.SetRedraw(FALSE);
	}	



	GetDlgItem(IDC_ET_MONEY)->SetWindowText("");
	GetDlgItem(IDC_ETCHINESE)->SetWindowText("");

	m_bkBrush = CreateSolidBrush(GetBackGroundColor());

	OnBnClickedInfor();
    CBcfFile fMsg(CBcfFile::GetAppPath()+"..\\ClientMessage.bcf");
    m_iUseNickName = fMsg.GetKeyVal("PBankDlg","UseNickName",0);



	wsprintf(path,"..\\%sdialog\\BankDlg\\wealth.png",skinfolder);
	m_bkimage.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	CGameImageHelper ImageHandle(&m_bkimage);
	HRGN hRgn=AFCBmpToRgn(ImageHandle,m_bkimage.GetPixel(0,0),RGB(1,1,1));
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
		DeleteObject(hRgn);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
void CPBankDlg::LoadSkin()
{
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f(CBcfFile::GetAppPath () + "..\\" + strSkin);
	TCHAR path[MAX_PATH];
	CString skinfolder = f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath());
	wsprintf(path,"..\\%sdialog\\BankDlg\\wealth.png",skinfolder);
	WCHAR wpath[MAX_PATH] = {0};
	MultiByteToWideChar(CP_ACP,0,path,strlen(path),wpath,256);
	if (NULL == m_pbgImg)
	{
#ifndef BZ_ZLIB
		m_pbgImg = new Image(wpath);
#else
		CUnZipRes unZipRes;
		IStream * pStream=NULL;
		pStream=unZipRes.LoadFileResFromZip(path);
		m_pbgImg=new Image(pStream);
		if(pStream!=NULL)
		{
			pStream->Release();
			pStream=NULL;
		}
#endif
	}

	if (NULL != m_pbgImg)
	{
		SetWindowPos(NULL,0,0,m_pbgImg->GetWidth(),m_pbgImg->GetHeight(),SWP_NOMOVE|SWP_NOZORDER);
	}
	{
		DWORD color = GetBackGroundColor();
		m_SkinList.SetColor(color);
	}	

	OnBnClickedInfor();

	wsprintf(path,"..\\%sdialog\\BankDlg\\wealth.png",skinfolder);
	m_bkimage.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	CGameImageHelper ImageHandle(&m_bkimage);
	HRGN hRgn=AFCBmpToRgn(ImageHandle,m_bkimage.GetPixel(0,0),RGB(1,1,1));
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
		DeleteObject(hRgn);
	}
}

LRESULT CPBankDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch(message)
	{
	case WM_CTLCOLORSTATIC :
		DWORD color = GetBackGroundColor();
		SetTextColor((HDC)wParam, RGB(0,0,0));
		SetBkColor((HDC)wParam,color);
		return (LRESULT)m_bkBrush;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

DWORD CPBankDlg::GetBackGroundColor()
{
	CBcfFile fBF("..\\ClientMessage.bcf");
	CString str = fBF.GetKeyVal("BankDlg","BackGroundRGB","198, 228, 244");
	int iR,iG,iB;
	CString strTem = str.Left(str.Find(","));
	iR = atoi(strTem);
	str = str.Right(str.GetLength()-str.Find(",")-1);
	strTem = str.Left(str.Find(","));
	iG = atoi(strTem);
	str = str.Right(str.GetLength()-str.Find(",")-1);
	iB = atoi(str);

	return RGB((unsigned char)iR,(unsigned char)iG,(unsigned char)iB);
}
void CPBankDlg::OnOK()
{
	switch(m_nCurPage)
	{
	case 2:
		return OnBnClickedAccountOK();
		break;
	case 4:
		return OnBnClickedBankOK();
		break;
	case 5:
		return OnBnClickedChPW();
		break;
	default:
		break;
	}
	//return __super::OnOK();
}

void CPBankDlg::OnBnClickedInfor()
{
	m_nCurPage = 1;
	ShowInterface();
}

void CPBankDlg::OnBnClickedTrans()
{
	m_nCurPage = 2;
	ShowInterface();
}

void CPBankDlg::OnBnClickedRecord()
{
	m_nCurPage = 3;
	ShowInterface();
}

void CPBankDlg::OnBnClickedSafe()
{
	m_nCurPage = 4;
	ShowInterface();
}

void CPBankDlg::OnBnClickedPassWord()
{
	m_nCurPage = 5;
	ShowInterface();
}

void CPBankDlg::ShowInterface()
{
	CWnd * pWnd;
	WCHAR PathImg[MAX_PATH]={0};
	CGameImage btn;
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CString strAppPath = CBcfFile::GetAppPath();
	CBcfFile fMsg(strAppPath +"..\\ClientMessage.bcf");
	CBcfFile f(strAppPath + "..\\" + strSkin);
	TCHAR path[MAX_PATH];
	CString skinfolder = f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath());

	if (1 == m_nCurPage)
	{
		wsprintf(path,"..\\%sdialog\\BankDlg\\wealth.png",skinfolder);

		Graphics *pG = Graphics::FromImage(m_pbgImg);

		
		wmemset(PathImg,0,MAX_PATH);
		MultiByteToWideChar(CP_ACP,0,path,strlen(path),PathImg,MAX_PATH);
#ifndef BZ_ZLIB
		Image img(PathImg);
#else
		CUnZipRes unZipRes;
		IStream * pStream=NULL;
		pStream=unZipRes.LoadFileResFromZip(path);
		Image img(pStream);
		if(pStream!=NULL)
		{
			pStream->Release();
			pStream=NULL;
		}
		
#endif
		
		pG->DrawImage(&img,Rect(0,0,m_pbgImg->GetWidth(),m_pbgImg->GetHeight()),0,0,m_pbgImg->GetWidth(),m_pbgImg->GetHeight(),UnitPixel);
		delete pG;

		pWnd = GetDlgItem(IDC_EDIT_INFO_GOLD);
		pWnd->MoveWindow(248,84,210,20);
//		pWnd->SetWindowText("0.0");
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_EDIT_GAMECOIN);
		pWnd->MoveWindow(248,156,210,20);
//		pWnd->SetWindowText("204");
		pWnd->ShowWindow(SW_SHOW);
		
		pWnd = GetDlgItem(IDC_EDIT_BOXCOIN);
		pWnd->MoveWindow(248,228,210,20);
//		pWnd->SetWindowText("2000");
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_EDIT_INFO_GOLD2);
		pWnd->MoveWindow(168,120,290,20);
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","UseDirectly","����Ϸ��ʹ�á�"));
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_EDIT_GAMECOIN2);
		pWnd->MoveWindow(168,192,290,20);
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","TransInfor","�����еĽ�ң���������Ϸ��ֱ��ʹ�á�"));
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_EDIT_BOXCOIN2);
		pWnd->MoveWindow(168,262,290,20);
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","NeedPassWord","�����еĴ�������֧��������ߵķ��á�"));
		pWnd->ShowWindow(SW_SHOW);

		ShowMoneyBank();
		m_btInfor.ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_INFO_GOLD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_GAMECOIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_BOXCOIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_INFO_GOLD2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_GAMECOIN2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_BOXCOIN2)->ShowWindow(SW_HIDE);
		m_btInfor.ShowWindow(SW_SHOW);
	}

	if(2 == m_nCurPage)
	{
		wsprintf(path,"..\\%sdialog\\BankDlg\\transfer.png",skinfolder);
		Graphics *pG = Graphics::FromImage(m_pbgImg);
		wmemset(PathImg,0,MAX_PATH);
		MultiByteToWideChar(CP_ACP,0,path,strlen(path),PathImg,MAX_PATH);
#ifndef BZ_ZLIB
		Image img(PathImg);
#else
		CUnZipRes unZipRes;
		IStream * pStream=NULL;
		pStream=unZipRes.LoadFileResFromZip(path);
		Image img(pStream);
		if(pStream!=NULL)
		{
			pStream->Release();
			pStream=NULL;
		}
#endif
		pG->DrawImage(&img,Rect(0,0,m_pbgImg->GetWidth(),m_pbgImg->GetHeight()),0,0,m_pbgImg->GetWidth(),m_pbgImg->GetHeight(),UnitPixel);
		delete pG;

		wsprintf(path,"..\\%slogon\\button.bmp",skinfolder);
		btn.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
		HRGN hRgn = AFCBmpToRgn(btn,btn.GetPixel(0,0),RGB(1,1,1));
		m_btnTranOK.LoadButtonBitmap(path,false);
		if(hRgn)
		{
			m_btnTranOK.SetWindowRgn(hRgn,true);

			DeleteObject(hRgn);
		}
		pWnd = GetDlgItem(IDC_BTN_TRANOK);
		pWnd->MoveWindow(290,269,btn.GetWidth()/4,btn.GetHeight());
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","OK","ȷ��"));
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_ETUSERCOIN);
		pWnd->MoveWindow(169,76,80,22);
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","UserCoin","���н�ң�"));
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_ETCOIN);
		pWnd->MoveWindow(254,76,210,20);
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_ETTRANSCOIN);
		pWnd->MoveWindow(169,106,80,22);
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","TransCoinNum","���ͽ������"));
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_ETOUTCOIN);
		pWnd->MoveWindow(254,106,210,20);
		pWnd->ShowWindow(SW_SHOW);
		
		//�����������λ��
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_ETOUTCOIN);
		if(Glb().m_bUseSpace)
			pEdit->SetLimitText(Glb().m_iMaxBit  + Glb().m_nPowerOfGold);	
		else
			pEdit->SetLimitText(18 + Glb().m_nPowerOfGold);

		pWnd = GetDlgItem(IDC_ETNUMTOCHINESE);
		pWnd->MoveWindow(169,136,80,22);
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","NumToChinese","�����(��д)��"));
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_ETCHINESE);
		pWnd->MoveWindow(254,136,150,40);
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_RTRANS_ID);
		pWnd->MoveWindow(254,180,80,20);
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","RTransUserID","�Է�ID"));
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_RTRANS_NN);
		pWnd->MoveWindow(365,180,80,20);
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","RTransUserNick","�Է��ǳ�"));
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_ETTRANSUSERID);
		pWnd->MoveWindow(170,200,80,20);
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","TransUserID","�Է�ID��"));
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_ETOTHERID);
		pWnd->MoveWindow(254,200,210,20);
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_ETSECONDPW);
		pWnd->MoveWindow(170,238,80,20);
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","SecondPassword","�������룺"));
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_ETPW);
		pWnd->MoveWindow(254,238,210,20);
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_ETTIP);
		pWnd->MoveWindow(164,280,40,20);
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","Tip","��ʾ��"));
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_ETCOINNUM);
		pWnd->MoveWindow(164,300,240,20);
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_ETTRANSNUM);		
		pWnd->MoveWindow(164,318,240,20);
		pWnd->ShowWindow(SW_SHOW);

		SetAccountInfor();

		if(m_bUseDestID)
		{
			((CButton*)GetDlgItem(IDC_RTRANS_ID))->SetCheck(BST_CHECKED);
			((CButton*)GetDlgItem(IDC_RTRANS_NN))->SetCheck(BST_UNCHECKED);
			OnBnClickedRTransId();
		}
		else
		{
			((CButton*)GetDlgItem(IDC_RTRANS_ID))->SetCheck(BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_RTRANS_NN))->SetCheck(BST_CHECKED);
			OnBnClickedRTransNn();
		}

		m_bnTrans.ShowWindow(SW_HIDE);
	}
	else
	{  
		GetDlgItem(IDC_ETCOIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETOUTCOIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETCHINESE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETOTHERID)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETPW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETTRANSNUM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETCOINNUM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETTIP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_TRANOK)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_ETUSERCOIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETTRANSCOIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETNUMTOCHINESE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETTRANSUSERID)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETSECONDPW)->ShowWindow(SW_HIDE);		

		GetDlgItem(IDC_RTRANS_ID)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RTRANS_NN)->ShowWindow(SW_HIDE);

		m_bnTrans.ShowWindow(SW_SHOW);
	}


	if (3 == m_nCurPage)
	{
		wsprintf(path,"..\\%sdialog\\BankDlg\\record.png",skinfolder);
		Graphics *pG = Graphics::FromImage(m_pbgImg);
		wmemset(PathImg,0,MAX_PATH);
		MultiByteToWideChar(CP_ACP,0,path,strlen(path),PathImg,MAX_PATH);
#ifndef BZ_ZLIB
		Image img(PathImg);
#else
		CUnZipRes unZipRes;
		IStream * pStream=NULL;
		pStream=unZipRes.LoadFileResFromZip(path);
		Image img(pStream);
		if(pStream!=NULL)
		{
			pStream->Release();
			pStream=NULL;
		}
#endif
		pG->DrawImage(&img,Rect(0,0,m_pbgImg->GetWidth(),m_pbgImg->GetHeight()),0,0,m_pbgImg->GetWidth(),m_pbgImg->GetHeight(),UnitPixel);
		delete pG;

		pWnd = GetDlgItem(IDC_SKINLIST);
		pWnd->MoveWindow(157,29,328,278);
		pWnd->ShowWindow(SW_SHOW);

		wsprintf(path,"..\\%slogon\\button.bmp",skinfolder);
		btn.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
		HRGN hRgn = AFCBmpToRgn(btn,btn.GetPixel(0,0),RGB(1,1,1));
		m_btnRefresh.LoadButtonBitmap(path,false);
		if(hRgn)
		{
			m_btnRefresh.SetWindowRgn(hRgn,true);
			///{{ Added by zxd 20100709 �ͷ��������
			DeleteObject(hRgn);
			///Added by zxd 20100709 �ͷ��������}}
		}
		m_btnRefresh.MoveWindow(225,314,btn.GetWidth()/4,btn.GetHeight());
		m_btnRefresh.SetWindowText(fMsg.GetKeyVal("PBankDlg","Refresh","ˢ��"));
		m_btnRefresh.ShowWindow(SW_SHOW);

		m_btnRcoClose.LoadButtonBitmap(path,false);
		if(hRgn)
			m_btnRcoClose.SetWindowRgn(hRgn,true);
		m_btnRcoClose.MoveWindow(351,314,btn.GetWidth()/4,btn.GetHeight());
		m_btnRcoClose.SetWindowText(fMsg.GetKeyVal("PBankDlg","Close","�ر�"));
		m_btnRcoClose.ShowWindow(SW_SHOW);

		InsertRecordList();
		m_bnRecord.ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_SKINLIST)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNCLOSE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNREFRESH)->ShowWindow(SW_HIDE);

		m_bnRecord.ShowWindow(SW_SHOW);
	}

	if (4 == m_nCurPage)
	{
		wsprintf(path,"..\\%sdialog\\BankDlg\\boxsafe.png",skinfolder);
		Graphics *pG = Graphics::FromImage(m_pbgImg);
		wmemset(PathImg,0,MAX_PATH);
		MultiByteToWideChar(CP_ACP,0,path,strlen(path),PathImg,MAX_PATH);
#ifndef BZ_ZLIB
		Image img(PathImg);
#else
		CUnZipRes unZipRes;
		IStream * pStream=NULL;
		pStream=unZipRes.LoadFileResFromZip(path);
		Image img(pStream);
		if(pStream!=NULL)
		{
			pStream->Release();
			pStream=NULL;
		}
#endif
		pG->DrawImage(&img,Rect(0,0,m_pbgImg->GetWidth(),m_pbgImg->GetHeight()),0,0,m_pbgImg->GetWidth(),m_pbgImg->GetHeight(),UnitPixel);
		delete pG;

		wsprintf(path,"..\\%slogon\\button.bmp",skinfolder);
		btn.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
		HRGN hRgn = AFCBmpToRgn(btn,btn.GetPixel(0,0),RGB(1,1,1));
		m_btnBok.LoadButtonBitmap(path,false);
		if(hRgn)
		{
			m_btnBok.SetWindowRgn(hRgn,true);
			///{{ Added by zxd 20100709 �ͷ��������
			DeleteObject(hRgn);
			///Added by zxd 20100709 �ͷ��������}}
		}
		m_btnBok.MoveWindow(325,m_pbgImg->GetHeight() - btn.GetHeight() - 30,btn.GetWidth()/4,btn.GetHeight());
		//m_btnBok.MoveWindow(285,m_pbgImg->GetHeight() - 15 - btn.GetHeight(),btn.GetWidth()/4,btn.GetHeight());
		m_btnBok.SetWindowText(fMsg.GetKeyVal("PBankDlg","OK","ȷ��"));
		m_btnBok.ShowWindow(SW_SHOW);

		m_btnBactive.LoadButtonBitmap(path,false);
		if(hRgn)
			m_btnBactive.SetWindowRgn(hRgn,true);
		m_btnBactive.MoveWindow(285,303,btn.GetWidth()/4,btn.GetHeight());
		m_btnBactive.SetWindowText(fMsg.GetKeyVal("PBankDlg","Active","����"));
		m_btnBactive.ShowWindow(SW_HIDE);

		m_btnBcharge.LoadButtonBitmap(path,false);
		if(hRgn)
			m_btnBcharge.SetWindowRgn(hRgn,true);
		m_btnBcharge.MoveWindow(378,303,btn.GetWidth()/4,btn.GetHeight());
		m_btnBcharge.SetWindowText(fMsg.GetKeyVal("PBankDlg","Charge","����"));
		m_btnBcharge.ShowWindow(SW_HIDE);

		pWnd = GetDlgItem(IDC_ETBOXCOIN);
		pWnd->MoveWindow(180,115,200,20);
		//pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","AccountCoin","�ʻ�Я����Ϸ�ң�%s"));
		TCHAR szNum[128];
		CString strMsg;
		CString strTmp;
		GlbGetNumString(szNum, m_i64WalletMoney, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
		strTmp = fMsg.GetKeyVal("PBankDlg","AccountCoin","���н�ң�%s");
		strMsg.Format(strTmp, szNum);
		pWnd->SetWindowText(strMsg);
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_ETACOIN);
		pWnd->MoveWindow(180,135,200,20);
		//pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","BoxCoin","�������е���Ϸ�ң�%s"));
		GlbGetNumString(szNum, m_i64BankMoney, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
		strTmp = fMsg.GetKeyVal("PBankDlg","BoxCoin","���д�%s");
		strMsg.Format(strTmp, szNum);
		pWnd->SetWindowText(strMsg);
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_ETBCONNO);
		pWnd->MoveWindow(180,180,100,20);
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","InputGameCoin","�������ȡ��"));
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_ETBPW);
		pWnd->MoveWindow(180,205,100,20);
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","InputPW","�������������룺"));
		pWnd->ShowWindow(SW_SHOW);
		
		pWnd = GetDlgItem(IDC_ETBCONNOIN);
		pWnd->MoveWindow(280,180,135,20);
	//	pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","BoxCoin","�������е���Ϸ�ң�"));
		pWnd->ShowWindow(SW_SHOW);


		wsprintf(path,"..\\%slogon\\button.bmp",skinfolder);
		btn.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
		hRgn = AFCBmpToRgn(btn,btn.GetPixel(0,0),RGB(1,1,1));
		m_btnCoinAll.LoadButtonBitmap(path,false);
		if(hRgn)
		{
			m_btnCoinAll.SetWindowRgn(hRgn,true);
			DeleteObject(hRgn);
		}
		pWnd = GetDlgItem(IDC_BTN_COIN_ALL);
		pWnd->MoveWindow(420,178,60,23);
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","CoinAll","ȫ��"));
		pWnd->ShowWindow(SW_SHOW);

		//�����������λ��
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_ETBCONNOIN);
		if(Glb().m_bUseSpace)
			pEdit->SetLimitText(Glb().m_iMaxBit  + Glb().m_nPowerOfGold);
		else
			pEdit->SetLimitText(18 + Glb().m_nPowerOfGold);

		pWnd = GetDlgItem(IDC_ETBPWIN);
		pWnd->MoveWindow(280,205,120,20);
	//	pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","BoxCoin","�������е���Ϸ�ң�"));
		pWnd->ShowWindow(SW_SHOW);

		TCHAR buffer[16];
		memset(buffer,0,16);
		switch(Glb().m_nPowerOfGold)
		{
		case 0:
			_tcscat( buffer, TEXT("1"));
		case 1:
			_tcscat( buffer, TEXT("ʮ"));
			break;
		case 2:
			_tcscat( buffer, TEXT("��"));
			break;
		case 3:
			_tcscat( buffer, TEXT("ǧ"));
			break;
		case 4:
			_tcscat( buffer, TEXT("��"));
			break;
		}

		pWnd = GetDlgItem(IDC_ETBREGPW);
		pWnd->MoveWindow(164,270,230,20);
		//pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","RegPW",""));
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_RSAVE);
		pWnd->MoveWindow(206,291,80,20);
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","SaveGameCoin","������Ϸ��"));
		pWnd->ShowWindow(SW_SHOW);
		
		pWnd = GetDlgItem(IDC_RGET);
		pWnd->MoveWindow(206,316,80,20);
		//pWnd->MoveWindow(330,286,80,20);
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","GetGameCoin","ȡ����Ϸ��"));
		pWnd->ShowWindow(SW_SHOW);
		
		pWnd = GetDlgItem(IDC_MONEY_STATIC);
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","NumToChinese","�����(��д)��"));
		pWnd->MoveWindow(180,230,80,20);
		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_ET_MONEY);
		pWnd->MoveWindow(280,230,190,40);	
		//pWnd->SetWindowText("");
		pWnd->ShowWindow(SW_SHOW);

		if (m_uCheckInOrOut == 0)
		{
			((CButton*)GetDlgItem(IDC_RGET))->SetCheck(BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_RSAVE))->SetCheck(BST_CHECKED);
			OnBnClickedSave();
		}
		else
		{
			((CButton*)GetDlgItem(IDC_RGET))->SetCheck(BST_CHECKED);
			((CButton*)GetDlgItem(IDC_RSAVE))->SetCheck(BST_UNCHECKED);
			OnBnClickedGet();
		}
		m_bnSafe.ShowWindow(SW_HIDE);
		///{{ Added by zxd 20100709 �ͷ��������
		if (hRgn != NULL)
		{
			DeleteObject(hRgn);
		}
		///Added by zxd 20100709 �ͷ��������}}
	}
	else
	{
		GetDlgItem(IDC_ETACOIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETBOXCOIN)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_ETBCONNO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETBPW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETBCONNOIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETBPWIN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETBREGPW)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_RSAVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RGET)->ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_ETBREGPW, "");
		m_btnBok.ShowWindow(SW_HIDE);
		m_btnBactive.ShowWindow(SW_HIDE);
		m_btnBcharge.ShowWindow(SW_HIDE);

		GetDlgItem(IDC_MONEY_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ET_MONEY)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_BTN_COIN_ALL)->ShowWindow(SW_HIDE);

		//if(m_haveZhuanZhang)
	    	m_bnSafe.ShowWindow(SW_SHOW);
	}

	if (5 == m_nCurPage)
	{
		wsprintf(path,"..\\%sdialog\\BankDlg\\Changepw.png",skinfolder);
		Graphics *pG = Graphics::FromImage(m_pbgImg);
		wmemset(PathImg,0,MAX_PATH);
		MultiByteToWideChar(CP_ACP,0,path,strlen(path),PathImg,MAX_PATH);
#ifndef BZ_ZLIB
		Image img(PathImg);
#else
		CUnZipRes unZipRes;
		IStream * pStream=NULL;
		pStream=unZipRes.LoadFileResFromZip(path);
		Image img(pStream);
		if(pStream!=NULL)
		{
			pStream->Release();
			pStream=NULL;
		}
#endif
		pG->DrawImage(&img,Rect(0,0,m_pbgImg->GetWidth(),m_pbgImg->GetHeight()),0,0,m_pbgImg->GetWidth(),m_pbgImg->GetHeight(),UnitPixel);
		delete pG;

		wsprintf(path,"..\\%slogon\\button.bmp",skinfolder);
		btn.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
		HRGN hRgn = AFCBmpToRgn(btn,btn.GetPixel(0,0),RGB(1,1,1));
		m_btnChOK.LoadButtonBitmap(path,false);
		if(hRgn)
		{
			m_btnChOK.SetWindowRgn(hRgn,true);
			///{{ Added by zxd 20100709 �ͷ��������
			DeleteObject(hRgn);
			///Added by zxd 20100709 �ͷ��������}}
		}
		m_btnChOK.MoveWindow(223,282,btn.GetWidth()/4,btn.GetHeight());
		m_btnChOK.SetWindowText(fMsg.GetKeyVal("PBankDlg","OK","ȷ��"));
		m_btnChOK.ShowWindow(SW_SHOW);
	
		m_btnChClose.LoadButtonBitmap(path,false);
		if(hRgn)
			m_btnChClose.SetWindowRgn(hRgn,true);
		m_btnChClose.MoveWindow(345,282,btn.GetWidth()/4,btn.GetHeight());
		m_btnChClose.SetWindowText(fMsg.GetKeyVal("PBankDlg","Close","�ر�"));
		m_btnChClose.ShowWindow(SW_SHOW);
	
		pWnd = GetDlgItem(IDC_ETOLDPW);
		pWnd->MoveWindow(200,117,74,20);
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","OldPW","���������룺"));
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_ETOLDPWIN);
		pWnd->MoveWindow(278,115,120,20);
//		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","OldPW","���������룺"));
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_ETCHPW);
		pWnd->MoveWindow(200,231,260,20);
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","ChangeYourPW","���������ʼ��Ϊ��Ϸ��½���룬�뼰ʱ�޸ģ�"));
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_ETNEWPW);
		pWnd->MoveWindow(200,154,74,20);
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","NewPW","���������룺"));
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_ETNEWPWIN);
		pWnd->MoveWindow(278,150,120,20);
//		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","NewPW","���������룺"));
		pWnd->ShowWindow(SW_SHOW);
		
		pWnd = GetDlgItem(IDC_ETCONFIRM);
		pWnd->MoveWindow(200,190,74,20);
		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","ConfirmPW","ȷ�������룺"));
		pWnd->ShowWindow(SW_SHOW);

		pWnd = GetDlgItem(IDC_ETCONFIRMIN);
		pWnd->MoveWindow(278,186,120,20);
//		pWnd->SetWindowText(fMsg.GetKeyVal("PBankDlg","ConfirmPW","ȷ�������룺"));
		pWnd->ShowWindow(SW_SHOW);
			
		m_bnPW.ShowWindow(SW_HIDE);

		///{{ Added by zxd 20100709 �ͷ��������
		if (hRgn != NULL)
		{
			DeleteObject(hRgn);
		}
		///Added by zxd 20100709 �ͷ��������}}

	}
	else
	{
		GetDlgItem(IDC_BTNCHGOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTNCHGCLOSE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETCHPW)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_ETOLDPW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETOLDPWIN)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_ETNEWPW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETNEWPWIN)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_ETCONFIRM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ETCONFIRMIN)->ShowWindow(SW_HIDE);

		m_btnChOK.ShowWindow(SW_HIDE);
		m_btnChClose.ShowWindow(SW_HIDE);
		m_bnPW.ShowWindow(SW_SHOW);
	}

	wsprintf(path,"..\\%sdialog\\win_close.bmp",skinfolder);
	m_BtClose.LoadButtonBitmap(path,false);
	btn.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	//m_BtClose.SetWindowPos(NULL,0,0,btn.GetWidth()/4,btn.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);
	CRect rc;
	GetClientRect(&rc);
	m_BtClose.MoveWindow(rc.right-rc.left-btn.GetWidth()/4, 0, btn.GetWidth()/4, btn.GetHeight());

	Invalidate(FALSE);
}

void CPBankDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (point.y < 28)
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	}
}

void CPBankDlg::OnBnClickedSave()
{
	SetDlgItemText(IDC_ETBREGPW, "");
	m_uCheckInOrOut = 0;
	GetDlgItem(IDC_ETBPW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ETBPWIN)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_ETBREGPW)->ShowWindow(SW_HIDE);
}

void CPBankDlg::OnBnClickedGet()
{
	SetDlgItemText(IDC_ETBREGPW, "");
	m_uCheckInOrOut = 1;
	GetDlgItem(IDC_ETBPW)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_ETBPWIN)->ShowWindow(SW_SHOW);
	//GetDlgItem(IDC_ETBREGPW)->ShowWindow(SW_SHOW);
}

void CPBankDlg::OnBnClickedRTransId()
{
	m_bUseDestID = true;
	CBcfFile fMsg(CBcfFile::GetAppPath()+"..\\ClientMessage.bcf");
	GetDlgItem(IDC_ETTRANSUSERID)->SetWindowText(fMsg.GetKeyVal("PBankDlg","TransUserID","�Է�ID��"));
}

void CPBankDlg::OnBnClickedRTransNn()
{
	m_bUseDestID = false;
	CBcfFile fMsg(CBcfFile::GetAppPath()+"..\\ClientMessage.bcf");
	GetDlgItem(IDC_ETTRANSUSERID)->SetWindowText(fMsg.GetKeyVal("PBankDlg","TransUserNick","�Է��ǳƣ�"));
}

///
/// �޸����� ��ȷ����ť��Ӧ����
/// @param void
/// @return void
///
void CPBankDlg::OnBnClickedChPW()
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"..\\ClientMessage.bcf");
	CString strOld, strNew, str;
	TCHAR szMsg[MAX_PATH];

	/// �ȱȽ��������ȷ�������Ƿ���ͬ�������ͬ����ֱ�ӷ��أ���ͬ�ż���
	GetDlgItemText(IDC_ETNEWPWIN,strOld);//������
	GetDlgItemText(IDC_ETCONFIRMIN,strNew);//ȷ��������
	if (strOld != strNew)
	{
		BZSoundPlay(this, "..\\music\\������ʾ.mp3", 0);
		str = fMsg.GetKeyVal("BankDlg","ConfirmPasswdNotSame","�������������벻һ�£����������룡");
		wsprintf(szMsg, str.GetBuffer());
		AFCMessageBox(szMsg);
		return;
	}
	if ((strNew.GetLength()<6) ||(strNew.GetLength()>20))
	{
		BZSoundPlay(this, "..\\music\\������ʾ.mp3", 0);
		str = fMsg.GetKeyVal("BankDlg","NewPasswdBytes","������6-20�ַ���ɣ����������������룡");
		wsprintf(szMsg, str.GetBuffer());
		AFCMessageBox(szMsg);
		return;
	}

	/// ������ȷ�ϳɹ���ȡ����������󣬷�����Ϣ��������
	GetDlgItemText(IDC_ETOLDPWIN,strOld);//������

	ChangePasswd_t *pChangePasswd = new ChangePasswd_t();
	_tcscpy( pChangePasswd->szPwdOld, strOld.Left(20).GetBuffer() );
	_tcscpy( pChangePasswd->szPwdNew, strNew.Left(20).GetBuffer() );

	m_pGameRoom->PostMessage(WM_CHANGE_PASSWD, 0, LPARAM(pChangePasswd));
}
/// ��ȡǮ����½���
/// @param Opt 0-��ʾȡ����1-��ʾ����
/// @param nMoney ��ʾ�����������Ľ����
void CPBankDlg::UpdateUserMoney(int OptType, __int64 i64Money)
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"..\\ClientMessage.bcf");
	TCHAR szNum[128];
	GlbGetNumString(szNum, i64Money, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
	CString strMsg;
	CString strFormat = fMsg.GetKeyVal("PBankDlg", "CheckInOutTip", "�ɹ�%s %s ���");;
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
	if (GetSafeHwnd()!=NULL)
	{
		ShowInterface();
		/// ��ʾ��Ҵ�ȡǮ�ɹ���
		SetDlgItemText(IDC_ETBREGPW, strMsg.GetBuffer());
		GetDlgItem(IDC_ETBREGPW)->ShowWindow(SW_SHOW);
	}
}
/// ת�ʳɹ��������������
/// @param Opt 0-��ʾת�������ˣ�1-��ʾ�ɱ���ת����Լ�
/// @param nMoney ��ʾ�����������Ľ����
void CPBankDlg::UpdateUserMoneyByTransfer(int OptType, __int64 i64Money)
{
	if( OptType == 0 ) //ȡ��
	{
		m_i64BankMoney	-= i64Money;
	}
	else //����
	{
		m_i64BankMoney	+= i64Money;
	}
	if (GetSafeHwnd()!=NULL)
	{
		ShowInterface();
	}
}
void CPBankDlg::ShowMoneyBank()
{
	if (GetSafeHwnd()==NULL)
	{
		return;
	}
	TCHAR szNum[128], szBank[128];
	GlbGetNumString(szBank, m_i64BankMoney, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
	GlbGetNumString(szNum, m_i64WalletMoney, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
	SetDlgItemText(/*IDC_EDIT_BOXCOIN*/IDC_EDIT_GAMECOIN,szBank);//���б�����
	SetDlgItemText(IDC_EDIT_INFO_GOLD,szNum);//���ڽ��
}
///
/// ���òƸ���Ϣ
/// @param nBank ���д��
/// @param nWallet �����ֽ�
/// @return void
///
void CPBankDlg::SetWealthInfor(__int64 i64Bank, __int64 i64Wallet)
{
	m_i64BankMoney = i64Bank;
	m_i64WalletMoney = i64Wallet;
	ShowMoneyBank();
}

///
/// �ʺ�ת�� ��ȷ����ť��Ӧ����
/// @param void
/// @return void
///
void CPBankDlg::OnBnClickedAccountOK()
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"..\\ClientMessage.bcf");

	/// ȡ�ñ༭������ַ���
	TCHAR szMsg[256];
	CString strNum, str;
	GetDlgItemText(IDC_ETOUTCOIN, strNum);

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
		BZSoundPlay(this, "..\\music\\������ʾ.mp3", 0);
		str = fMsg.GetKeyVal("BankDlg","NotInputCoin","��������Ҫ��ȡ������");
		wsprintf(szMsg, str, Glb().m_nPowerOfGold+1, Glb().m_nPowerOfGold, szTail);
		AFCMessageBox(szMsg);
		return;
	}
	/// �������̫��
	if (strNum.GetLength()<Glb().m_nPowerOfGold)
	{
		BZSoundPlay(this, "..\\music\\������ʾ.mp3", 0);
		str = fMsg.GetKeyVal("BankDlg","NotGetCoin","������Ľ�������ϵͳ���ܣ�����������%dλ����ĩ%dλΪ %s ����������");
		wsprintf(szMsg, str, Glb().m_nPowerOfGold+1, Glb().m_nPowerOfGold, szTail);
		AFCMessageBox(szMsg);
		return;
	}
	/// �������̫��
	if (strNum.GetLength()>18+Glb().m_nPowerOfGold)
	{
		BZSoundPlay(this, "..\\music\\������ʾ.mp3", 0);
		str = fMsg.GetKeyVal("BankDlg","NumberTooLarge","�����������̫�󣬳�������Χ�������벻����%I64d%s����ĩ%dλΪ %s ����������");
		wsprintf(szMsg, str, 0x3FFFFFFFFFFFFFFF, szTail, Glb().m_nPowerOfGold, szTail);
		AFCMessageBox(szMsg);
		return;
	}

	/// ���ĩ��λ������
	CString strSubNum = strNum.Right(Glb().m_nPowerOfGold);
	if (strSubNum != szTail)
	{
		BZSoundPlay(this, "..\\music\\������ʾ.mp3", 0);
		TCHAR szMsg[256];
		str = fMsg.GetKeyVal("BankDlg","NotGetCoin2","������Ľ�������ϵͳ���ܣ�������ĩ%dλΪ %s ����������");
		wsprintf(szMsg, str, Glb().m_nPowerOfGold, szTail);
		AFCMessageBox(szMsg);
		return;
	}
	/// ��ȡǰNλ
	strSubNum = strNum.Left(strNum.GetLength()-Glb().m_nPowerOfGold);
	__int64 i64Num = atol(strSubNum.GetBuffer());
	if (i64Num>0x3FFFFFFFFFFFFFFF)
	{
		BZSoundPlay(this, "..\\music\\������ʾ.mp3", 0);
		str = fMsg.GetKeyVal("BankDlg","NumberTooLarge","�����������̫�󣬳�������Χ�������벻����%I64d%s����ĩ%dλΪ %s ����������");
		wsprintf(szMsg, str, 0x3FFFFFFFFFFFFFFF, szTail, Glb().m_nPowerOfGold, szTail);
		AFCMessageBox(szMsg);
		return;
	}

	GetDlgItemText(IDC_ETOTHERID, strNum); //�Է�ID
	GetDlgItemText(IDC_ETPW,str);//��������
	if (str.GetLength()<1)
	{
		BZSoundPlay(this, "..\\music\\������ʾ.mp3", 0);
		str = fMsg.GetKeyVal("BankDlg","PasswdBytes","���������룡");
		wsprintf(szMsg, str.GetBuffer());
		AFCMessageBox(szMsg);
		return;
	}
	if (i64Num<0)
	{
		BZSoundPlay(this, "..\\music\\������ʾ.mp3", 0);
		str = fMsg.GetKeyVal("BankDlg","NotGetCoin","������Ľ�������ϵͳ���ܣ�����������%dλ����ĩ%dλΪ %s ����������");
		wsprintf(szMsg, str, Glb().m_nPowerOfGold+1, Glb().m_nPowerOfGold, szTail);
		AFCMessageBox(szMsg);
		return;
	}

	TransferMoney_t *pTransferMoney = new TransferMoney_t();
	pTransferMoney->i64Money = i64Num;
	pTransferMoney->uDestUserID = atol(strNum);
	_tcscpy(pTransferMoney->szPWD, str.Left(20).GetBuffer());
	_tcscpy(pTransferMoney->szDestNickName, strNum.GetBuffer());

	pTransferMoney->bUseDestID = m_bUseDestID;

	/// ��ʾ�û�ȷ��ת��
	TCHAR szNum[128];
	GlbGetNumString(szNum, pTransferMoney->i64Money, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
	//CString strFormat = fMsg.GetKeyVal("PBankDlg", "TransferConfirm", "����Ҫ��ID��Ϊ[%d]�����ת��%s��ң��Ƿ�ȷ��ת�ʣ�");

	CString strFormat;
	CString test;
	test.Format("lxl is use nick name : %d",m_iUseNickName );
	OutputDebugString(test);
    if(m_bUseDestID)
    {
		BZSoundPlay(this, "..\\music\\������ʾ.mp3", 0);
		strFormat = fMsg.GetKeyVal("PBankDlg", "TransferConfirmId", "����Ҫ��ID��Ϊ[%d]�����ת��%s��ң��Ƿ�ȷ��ת�ʣ�");
		wsprintf( szMsg, strFormat, pTransferMoney->uDestUserID, szNum );
    }
    else
    {
		BZSoundPlay(this, "..\\music\\������ʾ.mp3", 0);
        // lxl, 2010-11-17, �����ǳ���ʾ
        strFormat = fMsg.GetKeyVal("PBankDlg", "TransferConfirmNn", "����Ҫ���ǳ�[%s]�����ת��%s��ң��Ƿ�ȷ��ת�ʣ�");
        wsprintf( szMsg, strFormat, pTransferMoney->szDestNickName,szNum );
    }	

	if (IDOK != AFCMessageBox(szMsg, "����",AFC_YESNO))
	{
		BZSoundPlay(this, "..\\music\\������ʾ.mp3", 0);
		delete pTransferMoney;
		pTransferMoney = NULL;
		return;
	}
	m_pGameRoom->PostMessage(WM_TRANSFER_MONEY, 0, LPARAM(pTransferMoney));
}

void CPBankDlg::OnBnClickedCoinAll()
{
	TCHAR szNum[128];
	if(m_uCheckInOrOut == 0)
	{
		int nLess = 0;
		CLoveSendClassInExe* pLoveSend = dynamic_cast<CLoveSendClassInExe*>(GetParent());   
		if(pLoveSend != NULL)
		{
			nLess = pLoveSend->m_pGameInfo->uLessPoint;
		}
		else
		{
			if(m_pLogic != NULL)
			{
				nLess = m_pLogic->GetGameInfo()->uLessPoint;
			}
		}
		GlbGetNumString(szNum, m_i64WalletMoney-nLess, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
	}
	else
	{
		GlbGetNumString(szNum, m_i64BankMoney, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
	}
	SetDlgItemText(IDC_ETBCONNOIN, szNum);
}


///
/// ���� �ʺ�ת�� �µĽ����
/// @param void
/// @return void
///
void CPBankDlg::SetAccountInfor()
{
	CString str;
	CString strTem;
	CBcfFile fMsg(CBcfFile::GetAppPath()+"..\\ClientMessage.bcf");

	TCHAR szNum[128], szBank[128];
	//str = fMsg.GetKeyVal("PBankDlg","UHaveCoin","������%s��ң�%s��Ϸ�ҡ�");
	str = fMsg.GetKeyVal("PBankDlg","UHaveCoin","��Ǯ������%s��ҡ�");
	GlbGetNumString(szNum, m_i64WalletMoney, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
	GlbGetNumString(szBank, m_i64BankMoney, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
	strTem.Format(str, szNum);
	SetDlgItemText(IDC_ETCOINNUM,strTem);
	SetDlgItemText(IDC_ETCOIN,szBank);
	str = fMsg.GetKeyVal("PBankDlg","CoinToGameCoin","ת����۳�һ���������ѡ�");
	strTem.Format(str);
	SetDlgItemText(IDC_ETTRANSNUM,strTem);
}
/// ȡ��smallDateTime���ַ���
/// @param[out] szBuffer �ⲿ������ַ����飬��СΪ256�ַ�
/// @param[in] nSmallDateTime SQL_SERVER���ݿ����smallDateTime����
/// @return ����szBufferָ��
TCHAR *GetTimeString(TCHAR *szBuffer, COleDateTime smallDateTime)
{
	//__time64_t tAll; 
	//tAll = HIWORD(nSmallDateTime)*24*60*60 + LOWORD(nSmallDateTime)*60; 
	//COleDateTime time(tAll); 
	_stprintf(szBuffer,_T("%02d-%02d-%02d %02d:%02d"),
		smallDateTime.GetYear(),
		smallDateTime.GetMonth(),
		smallDateTime.GetDay(),
		smallDateTime.GetHour(),
		smallDateTime.GetMinute()
		);
	return szBuffer;
}
///
/// ���� ����ת������
/// @param void
/// @return void
///
void CPBankDlg::InsertRecordList()
{
	TCHAR szTemp[256];
	if (!m_mapTransferRecord.empty())
	{
		m_SkinList.DeleteAllItems();
		int nCount = m_mapTransferRecord.size();
		for (int i=0; i<nCount; ++i)
		{
			wsprintf(szTemp, "%d", m_mapTransferRecord[i].nSeriNo);
			m_SkinList.InsertItem(i, szTemp);
			GetTimeString(szTemp, m_mapTransferRecord[i].oleDateTime);
			m_SkinList.SetItemText(i, 0, szTemp );
			wsprintf(szTemp, "%d", m_mapTransferRecord[i].dwUserID);
			m_SkinList.SetItemText(i, 1, szTemp );
			wsprintf(szTemp, "%d", m_mapTransferRecord[i].destUserID);
			m_SkinList.SetItemText(i, 2, szTemp );
			wsprintf(szTemp, "%I64d", m_mapTransferRecord[i].i64Money);
			m_SkinList.SetItemText(i, 3, szTemp );
		}
	}

	m_SkinList.SetRedraw(TRUE);
	ListView_SetExtendedListViewStyle(m_SkinList.m_hWnd,LVS_EX_FULLROWSELECT|LVS_EX_FLATSB|LVS_EX_INFOTIP|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE);
}

///
/// ˢ��ת�˼�¼
/// @return void
///
void CPBankDlg::RefreshRecordList()
{
	m_SkinList.DeleteAllItems();
	m_pGameRoom->PostMessage(WM_TRANSFER_RECORD, 0, 0);
}
///
/// �յ�һ��ת�˼�¼
/// @param void
/// @return void
///
void CPBankDlg::OnReceivedTransferRecord(MSG_GR_R_TransferRecord_t *pRecord)
{
	TCHAR szTemp[256];
	m_mapTransferRecord[pRecord->nSeriNo] = *pRecord;
	wsprintf(szTemp, "%d", pRecord->nSeriNo);
	int nPos = m_SkinList.InsertItem(m_SkinList.GetItemCount(), szTemp);
	GetTimeString(szTemp, pRecord->oleDateTime);
	//wsprintf(szTemp, "%d", pRecord->uDateTime);
	m_SkinList.SetItemText(nPos, 0, szTemp );
	wsprintf(szTemp, "%d", pRecord->dwUserID);
	m_SkinList.SetItemText(nPos, 1, szTemp );
	wsprintf(szTemp, "%d", pRecord->destUserID);
	m_SkinList.SetItemText(nPos, 2, szTemp );
	GlbGetNumString(szTemp, pRecord->i64Money, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
	m_SkinList.SetItemText(nPos, 3, szTemp );
}

///
/// ���� �����Ѱ�ť��Ӧ����
/// @param void
/// @return void
///
void CPBankDlg::OnBnClickedBankRecharge()
{
	return;
	CString str;
	str.Format(" Function  OnBnClickedBankRecharge()   Line %d",__LINE__);
	MessageBox(str);
}

///
/// ���� �ļ��ť��Ӧ����
/// @param void
/// @return void
///
void CPBankDlg::OnBnClickedBankActive()
{
	return;
	CString str;
	str.Format(" Function  OnBnClickedBankActive()   Line %d",__LINE__);
	MessageBox(str);
}


///
/// ���� ��ȷ����ť��Ӧ����
/// ���жϣ�������ȡǮ���Ǵ�Ǯ
/// @param void
/// @return void
///
void CPBankDlg::OnBnClickedBankOK()
{
	SetDlgItemText(IDC_ETBREGPW, "");
	CBcfFile fMsg(CBcfFile::GetAppPath()+"..\\ClientMessage.bcf");
	/// ȡ�ñ༭������ַ���
	TCHAR szMsg[256];
	CString strNum, str;
	GetDlgItemText(IDC_ETBCONNOIN, strNum);

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
		BZSoundPlay(this, "..\\music\\������ʾ.mp3", 0);
		str = fMsg.GetKeyVal("BankDlg","NotInputCoin","��������Ҫ��ȡ������");
		wsprintf(szMsg, str, Glb().m_nPowerOfGold+1, Glb().m_nPowerOfGold, szTail);
		AFCMessageBox(szMsg);
		return;
	}
	/// �������̫��
	if (strNum.GetLength()<Glb().m_nPowerOfGold)
	{
		BZSoundPlay(this, "..\\music\\������ʾ.mp3", 0);
		str = fMsg.GetKeyVal("BankDlg","NotGetCoin","������Ľ�������ϵͳ���ܣ�����������%dλ����ĩ%dλΪ %s ����������");
		wsprintf(szMsg, str, Glb().m_nPowerOfGold+1, Glb().m_nPowerOfGold, szTail);
		AFCMessageBox(szMsg);
		return;
	}
	/// �������̫��
	if (strNum.GetLength()>18+Glb().m_nPowerOfGold)
	{
		BZSoundPlay(this, "..\\music\\������ʾ.mp3", 0);
		str = fMsg.GetKeyVal("BankDlg","NumberTooLarge","�����������̫�󣬳�������Χ�������벻����%I64d%s����ĩ%dλΪ %s ����������");
		wsprintf(szMsg, str, 0x3FFFFFFFFFFFFFFF, szTail, Glb().m_nPowerOfGold, szTail);
		AFCMessageBox(szMsg);
		return;
	}

	/// ���ĩ��λ������
	CString strSubNum = strNum.Right(Glb().m_nPowerOfGold);
	if (strSubNum != szTail)
	{
		BZSoundPlay(this, "..\\music\\������ʾ.mp3", 0);
		TCHAR szMsg[256];
		str = fMsg.GetKeyVal("BankDlg","NotGetCoin2","������Ľ�������ϵͳ���ܣ�������ĩ%dλΪ %s ����������");
		wsprintf(szMsg, str, Glb().m_nPowerOfGold, szTail);
		AFCMessageBox(szMsg);
		return;
	}
	/// ��ȡǰNλ
	strSubNum = strNum.Left(strNum.GetLength()-Glb().m_nPowerOfGold);
	__int64 i64Num = _tstoi64(strSubNum.GetBuffer());
	if (i64Num>0x3FFFFFFFFFFFFFFF)
	{
		BZSoundPlay(this, "..\\music\\������ʾ.mp3", 0);
		str = fMsg.GetKeyVal("BankDlg","NumberTooLarge","�����������̫�󣬳�������Χ�������벻����%I64d%s����ĩ%dλΪ %s ����������");
		wsprintf(szMsg, str, 0x3FFFFFFFFFFFFFFF, szTail, Glb().m_nPowerOfGold, szTail);
		AFCMessageBox(szMsg);
		return;
	}
	if (i64Num<0)
	{
		BZSoundPlay(this, "..\\music\\������ʾ.mp3", 0);
		str = fMsg.GetKeyVal("BankDlg","NotGetCoin","������Ľ�������ϵͳ���ܣ�����������%dλ����ĩ%dλΪ %s ����������");
		wsprintf(szMsg, str, Glb().m_nPowerOfGold+1, Glb().m_nPowerOfGold, szTail);
		AFCMessageBox(szMsg);
		return;
	}

	/// ����Ϣ
	if (m_pGameRoom!=NULL)
	{
		/// ������ȡ���Ǵ棬��ֻ���������ڷ���һ����Ϣ����
		/// Ϊͳһ�ӿڣ���ǮҲ���������뷢�͵�������
		/// ���������룬ȡ���Ǵ�
		GetDlgItemText(IDC_ETBCONNOIN,str);
		GetDlgItemText(IDC_ETBPWIN,str);
		
		if ((m_uCheckInOrOut==1) && (str.GetLength()<1))
		{
			BZSoundPlay(this, "..\\music\\������ʾ.mp3", 0);
			str = fMsg.GetKeyVal("BankDlg","PasswdBytes","���������룡");
			wsprintf(szMsg, str.GetBuffer());
			AFCMessageBox(szMsg);
			return;
		}
		CheckMoney_t *pCheckMoney = new CheckMoney_t;
		pCheckMoney->uType = m_uCheckInOrOut;
		pCheckMoney->i64Money = i64Num;
		_tcscpy(pCheckMoney->szPWD, str.Left(20).GetBuffer());
		m_pGameRoom->PostMessage(WM_CHECK_MONEY, 0, (LPARAM)pCheckMoney);
	}
}


// lxl 2010-11-17 ���û�ȡ���ǳ�
void CPBankDlg::SetNickName(TCHAR* szNickName)
{
	//OutputDebugString("�����ǳ�lxl");
    m_strNickName = szNickName;
}

// lxl 2010-11-17 �����û�ID��ȡ�ǳ�
void CPBankDlg::OnIDKillFocus()
{
    if(m_iUseNickName <= 0) return;

    CString strTempID = "";
    GetDlgItemText(IDC_ETOTHERID, strTempID);   // �Է�ID

    // ��ͬ������
    if(m_strSaveID == strTempID)
    {
        return;
    }

    m_strNickName = " ";

    // ��Ч������
    if(atol(strTempID) <= 0)
    {
        return;
    }

    m_strSaveID = strTempID;
	//OutputDebugString("��Ϸ�з��ͻ�ȡ�ǳ� lxl");
    // ���ͻ�ȡ�û��ǳ���Ϣ
    GetNickNameOnID_t* pGetNickNameOnID = new GetNickNameOnID_t;
    pGetNickNameOnID->uUserID = atol(strTempID);
    m_pGameRoom->PostMessage(WM_BANK_GET_NICKNAME_ONID, 0, LPARAM(pGetNickNameOnID));
}

// lxl 2010-11-19  ���ת��Ϊ���Ĵ�д
void CPBankDlg::ChangeDigitToChinese(__int64 i64Num, CString &strResult)
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
			if(digitArray[i-1] != 0 && (strcmp(szUnit[i],"��")!=0 && strcmp(szUnit[i],"��")!=0)) //���ڡ���λΪ0ʱ����������ʱ�����㡣
				strResult += szDigit[digitArray[i]];
		}
		else 
			strResult += szDigit[digitArray[i]];		
	
		//���뵥λ //zht2011-11-29 �޸��ַ����Ƚ�
		if(digitArray[i] == 0 && (strcmp(szUnit[i],"��")!=0 && strcmp(szUnit[i],"��")!=0) ) //���ڡ���λΪ0ʱ����
			continue; 
		
		strResult += szUnit[i];

	}
	if(strResult.Find("����") != -1)//�ַ��������������������⴦��
		strResult.Replace("����", "��");
}
//ת�˽���д��ʾ
void CPBankDlg::OnEnChangeMoney()
{	
	CString strGetCoin = "";
	GetDlgItemText(IDC_ETBCONNOIN, strGetCoin);   // ��ȡ���ID

	//ȥ��������еķָ�����
	if(Glb().m_bUseSpace)
		strGetCoin.Replace(Glb().m_strSpaceChar, "");	//����ҳ��
		
	//��֤�ַ����Ƿ���0-9������
	for(int i = 0; i < strGetCoin.GetLength(); i++)
	{
		if(strGetCoin[i] < '0' || strGetCoin[i] > '9')
		{
			SetDlgItemText(IDC_ETCHINESE, "");
			return;
		}
	}	

	__int64 i64Num  = _tstoi64(strGetCoin.GetBuffer());
	TCHAR szNum[128];

	if(bFlag == false)	
	{		
		//�ַ����м���ָ���
			GlbGetNumString(szNum, i64Num, 0, Glb().m_bUseSpace, Glb().m_strSpaceChar);
		bFlag = true;
		SetDlgItemText(IDC_ETBCONNOIN, szNum);
		//���ù�굽�����
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_ETBCONNOIN);
		pEdit->SetSel(-1);		
	}
	else 
		bFlag = false;

	ChangeDigitToChinese(i64Num, strGetCoin);
	

	SetDlgItemText(IDC_ET_MONEY, strGetCoin);	
}
//ת�˽���д��ʾ
void CPBankDlg::OnEnChangeEtbconnoin()
{
	CString strOutCoin = "";
    GetDlgItemText(IDC_ETOUTCOIN, strOutCoin);   // ת�˽��ID	
	
	//ȥ��������еķָ�����
	if(Glb().m_bUseSpace)
		strOutCoin.Replace(Glb().m_strSpaceChar, "");	//ת��ҳ��		
	
	//��֤�ַ����Ƿ���0-9������
	for(int i = 0; i < strOutCoin.GetLength(); i++)
	{
		if(strOutCoin[i] < '0' || strOutCoin[i] > '9')
		{
			SetDlgItemText(IDC_ETCHINESE, "");
			return;
		}
	}

	__int64 i64Num  = _tstoi64(strOutCoin.GetBuffer());
	TCHAR szNum[128];

	if(bFlag == false)	
	{
		//�ַ����м���ָ���
		GlbGetNumString(szNum, i64Num, 0, Glb().m_bUseSpace, Glb().m_strSpaceChar);
		bFlag = true;
		SetDlgItemText(IDC_ETOUTCOIN, szNum);
		//���ù�굽�����
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_ETOUTCOIN);
		pEdit->SetSel(-1);				
	}
	else 
		bFlag = false;

	ChangeDigitToChinese(i64Num, strOutCoin);	

	SetDlgItemText(IDC_ETCHINESE, strOutCoin);			
}
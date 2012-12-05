// AppealDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AppealDlg.h"

#include "detours.h"
#include "skinsb.h"

// CAppealDlg �Ի���

IMPLEMENT_DYNAMIC(CAppealDlg, CDialog)

CAppealDlg::CAppealDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAppealDlg::IDD, pParent)
{	
	m_strUserName = "";
	m_strInfoFileWholeName = "";
	m_strInfoFileName = "";

	 m_bUpLoad = false;

}

CAppealDlg::~CAppealDlg()
{
	m_fileList.clear();
}

void CAppealDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_btnCancle);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDC_CLOSE, m_bnClose);
	DDX_Control(pDX, IDC_LIST1, m_FileListCtrl);
}


BEGIN_MESSAGE_MAP(CAppealDlg, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CLOSE, &CAppealDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDOK, &CAppealDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAppealDlg::OnBnClickedCancel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CAppealDlg::OnNMDblclkList1)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_EXCHANGE_SKIN,&CAppealDlg::OnExchangeSkin)
END_MESSAGE_MAP()


// CAppealDlg ��Ϣ�������

void CAppealDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CAppealDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CAppealDlg::Init()
{
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkin)
		skinfolder = f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder = f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());

	/*  // 2012.12.16 yyf ����
	sprintf_s(path,sizeof(path),"%sdialog\\appealdlgbk.bmp",skinfolder);
	m_imgDlgBk.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	CGameImageHelper	ImageHandle(&m_imgDlgBk);
	HRGN hRgn=AFCBmpToRgn(ImageHandle,m_imgDlgBk.GetPixel(0,0),RGB(1,1,1));
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
		DeleteObject(hRgn);
	}*/

	sprintf_s(path,sizeof(path),"%sdialog\\normal_bt.BMP",skinfolder);
	m_btnOk.LoadButtonBitmap(path,false);
	sprintf_s(path,sizeof(path),"%sdialog\\normal_bt.BMP",skinfolder);
	m_btnCancle.LoadButtonBitmap(path,false);
	


	//*-------��ʼ�� m_configDlg--------- 2012.10.16 yyf
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%s%sdialog\\configDlg\\CAppealDlgConfig\\configDlg.bcf",s,skinfolder);// 
	wsprintf(PathDefault,"%s%s",s,skinfolder);
	m_configDlg.LoadConfig(pathConfig,PathDefault);	

	int iwith,iHeight; 
	iwith=m_configDlg.GetDlgWide();
	iHeight=m_configDlg.GetDlgHigh(); 
	//-----------------------------------------------------*/
	
	// 2012.12.16 yyf ����
	//int iwith = m_imgDlgBk.GetWidth();
	//int iHeight = m_imgDlgBk.GetHeight();
	//MoveWindow(0,0,m_imgDlgBk.GetWidth(),m_imgDlgBk.GetHeight());

	MoveWindow(0,0,iwith,iHeight);// 2012.12.16 yyf 

	CenterWindow(GetParent());  //ʹ�Ի��������ʾ

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString str;
	str = fMsg.GetKeyVal("AppealDlg","GameName","��Ϸ����");
	m_FileListCtrl.InsertColumn(0,str,LVCFMT_LEFT,60);
	str = fMsg.GetKeyVal("AppealDlg","GameTime","��Ϸʱ��");
	m_FileListCtrl.InsertColumn(1,str,LVCFMT_LEFT,83);
	str = fMsg.GetKeyVal("AppealDlg","FileName","�ļ�����");
	m_FileListCtrl.InsertColumn(2,str,LVCFMT_LEFT,180);
	m_FileListCtrl.SetRedraw(true);
	m_FileListCtrl.SetExtendedStyle((m_FileListCtrl.GetStyle() | LVS_EX_FULLROWSELECT) & ~LVS_EX_CHECKBOXES);
	//m_FileListCtrl.SetBkColor(RGB(255,0,0));

	str = fMsg.GetKeyVal("AppealDlg","Tip",
		"��ʾ��1.˫���б��е�һ�п��Բ��Ÿ�¼���ļ�\n            2.��ѡ������Ϊ�����ʵ�һ����Ϸ��¼���ύ!");
	SetDlgItemText(IDC_STATIC_TIP,str);
	str = fMsg.GetKeyVal("AppealDlg","BTNOK","�� ��");
	SetDlgItemText(IDOK,str);
	str = fMsg.GetKeyVal("AppealDlg","BTNCANCEL","ȡ ��");
	SetDlgItemText(IDCANCEL,str);

	CString strPath = CBcfFile::GetAppPath();

	DWORD dwHandle = cfgOpenFile(strPath + "bzgame.bcf");
	if ( dwHandle >= 0x10)
	{
		m_strFtpAddress = cfgGetValue(dwHandle,Glb().m_key,"FtpAddress","");
		m_strFtpUserName = cfgGetValue(dwHandle,Glb().m_key,"FtpUsrName","");
		m_strFtpPwd = cfgGetValue(dwHandle,Glb().m_key,"FtpPwd","");

		cfgClose(dwHandle);
	}


	CRect rc;
	GetWindowRect(&rc);
	m_bnClose.MoveWindow(rc.Width()-36,0,17,17);


	wsprintf(path,"%s\\%sdialog\\log_exit_bt.bmp",CBcfFile::GetAppPath(true),m_skinmgr.GetSkinPath());//win_close.bmp
	m_bnClose.LoadButtonBitmap(path,false);


	//m_FileListCtrl.


	{
		CString s=CBcfFile::GetAppPath ();/////����·��
		CString strSkin = m_skinmgr.GetSkinBcfFileName();
		CBcfFile f( s + strSkin);
		TCHAR path[MAX_PATH];
		CString skinfolder;
		if ("skin0.bcf" == strSkin)
			skinfolder = f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
		else
			skinfolder = f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());
		wsprintf(path,"%sDialog\\scroll.bmp",skinfolder);
		HBITMAP hbmScorll = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
		SkinSB_Init(m_FileListCtrl.GetSafeHwnd(), hbmScorll);
		SkinSB_Init(GetDlgItem(IDC_EDIT_APPEAL_CONTENT)->GetSafeHwnd(), hbmScorll);

	}

	::SetWindowPos(m_hWnd,CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
	
	SetWndRgn();// ���ô���Բ��2012.10.17 yyf
}

//��¼���ļ���ʱ������Ȼ��Ӧ���ؼ���  yjj 090304
void CAppealDlg::ProcessRecordFile(const CString& strUserName)
{
	if (strUserName == "")
	{
		return;
	}
	GetDlgItem(IDC_EDIT_USERNAME)->SetWindowText(strUserName);
	GetDlgItem(IDC_EDIT_PHONE_NUM)->SetWindowText("");
	GetDlgItem(IDC_EDIT_APPEAL_EMAIL)->SetWindowText("");
	GetDlgItem(IDC_EDIT_APPEAL_CONTENT)->SetWindowText("");



	CString strPath = CBcfFile::GetAppPath(); //�õ���ǰ��Ŀ¼

	strPath += "log";
	strPath += "\\";

	CString gamenamefile = strPath + "gamename.bcf";
	DWORD dwHandle = cfgOpenFile(gamenamefile);

	if(dwHandle < 0x10)
		return;

	strPath += "log_";
	strPath += strUserName;
	strPath += "\\";

	CTime tCurTime = CTime::GetCurrentTime();
	CString direct = tCurTime.Format("%Y-%m-%d");

	CString strCurDir = strPath + direct;
	
	strCurDir += "\\";
	CFileFind finder;	
	strCurDir += _T("*.*");	
	int iFindFileCount = 0;
	m_fileList.clear();

    //�ӵ�ǰ����ǰ��6��Ŀ¼
	for (int i=0; i<6; i++)
	{
		BOOL bWorking = finder.FindFile(strCurDir);
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			if (finder.IsDots())
				continue;

			

		
			//�ҵ�һ���ļ�
			//CString sFileName = finder.GetFileName();	
			//CString sFilePath = finder.GetFilePath();
			RecordFileStruct recordfile;

			recordfile.strWholeName = finder.GetFilePath(); //�õ���������
			recordfile.strFileName = finder.GetFileName();     //�õ��ļ�����
			
			//wushuqun 2009.5.20
			recordfile.strGamePath = finder.GetFilePath();

			//if (recordfile.strFileName.Find(".zxh") == -1)
			//{
			//	continue;
			//}
			CString strNameId = GetFileNameID(recordfile.strFileName);
			recordfile.strGameName = GetGameName(dwHandle,strNameId); //�õ���Ϸ����

			if (recordfile.strGameName == "")
			{
				continue;
			}
			finder.GetCreationTime(recordfile.timeCreatTime);    //�õ������ļ�ʱ��
			recordfile.strGameTime = recordfile.timeCreatTime.Format("%m-%d %H:%M ");
			m_fileList.push_back(recordfile);  //���ļ���Ϣ��������
			iFindFileCount ++;
		}
		//��������Ŀ¼
		if (iFindFileCount >= 20)
		{			
			break;
		}
		//
		CTimeSpan ts(1,   0,   0,   0); 
		tCurTime -= ts;
		direct = tCurTime.Format("%Y-%m-%d");
		strCurDir = strPath + direct;

		strCurDir += "\\";
		strCurDir +=_T("*.*");	
		//����һ���Ŀ¼		
	}

    //����������

	

	m_fileList.sort();
	m_FileListCtrl.DeleteAllItems();
	

	list<RecordFileStruct>::iterator iter = m_fileList.begin();
	int iCount = 0;
	for (;iter != m_fileList.end();iter++)
	{
		RecordFileStruct recordfile = *iter;

		//RecordFileStruct* pRecordFile = iter;
		//iter ++;
		//CString strListNmae;
		//strListNmae.Format("%s  %s",recordfile.strGameName,recordfile.strGameTime);

		int iItem = m_FileListCtrl.InsertItem(m_FileListCtrl.GetItemCount(),recordfile.strGameName);
        m_FileListCtrl.SetItemText(iItem,1,recordfile.strGameTime);
		
		//wushuqun 2009.5.20
		//������һ�С��ļ�����"
		m_FileListCtrl.SetItemText(iItem,2,recordfile.strFileName);

		//m_FileListCtrl.SetItemData(iItem,(DWORD)&iter);
		iCount++;
		//m_ListBox.AddString(strListNmae);
		//m_ListBox.InsertString(iCount++,strListNmae);
		if (iCount == 20)
		{
			break;
		}
	}









	cfgClose(dwHandle);
	finder.Close();



}
//����Ƥ��
void CAppealDlg::LoadSkin()
{
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkin)
		skinfolder = f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder = f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());

	/*// 2012.12.16 yyf ����
	sprintf_s(path,sizeof(path),"%sdialog\\appealdlgbk.bmp",skinfolder);
	m_imgDlgBk.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	CGameImageHelper	ImageHandle(&m_imgDlgBk);
	HRGN hRgn=AFCBmpToRgn(ImageHandle,m_imgDlgBk.GetPixel(0,0),RGB(1,1,1));
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
		DeleteObject(hRgn);
	}*/
	
	//*-------��ʼ�� m_configDlg--------- 2012.10.16 yyf
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%s%sdialog\\configDlg\\CAppealDlgConfig\\configDlg.bcf",s,skinfolder);// 
	wsprintf(PathDefault,"%s%s",s,skinfolder);
	m_configDlg.LoadConfig(pathConfig,PathDefault);	

	int iwith,iHeight; 
	iwith=m_configDlg.GetDlgWide();
	iHeight=m_configDlg.GetDlgHigh(); 
	//-----------------------------------------------------*/

	sprintf_s(path,sizeof(path),"%sdialog\\normal_bt.BMP",skinfolder);
	m_btnOk.LoadButtonBitmap(path,false);
	sprintf_s(path,sizeof(path),"%sdialog\\normal_bt.BMP",skinfolder);
	m_btnCancle.LoadButtonBitmap(path,false);

	/*// 2012.12.16 yyf ����
	int iwith = m_imgDlgBk.GetWidth();
	int iHeight = m_imgDlgBk.GetHeight();

	MoveWindow(0,0,m_imgDlgBk.GetWidth(),m_imgDlgBk.GetHeight());*/

	MoveWindow(0,0,iwith,iHeight);// 2012.12.16 yyf 

	CenterWindow(GetParent());  //ʹ�Ի��������ʾ

	CString strPath = CBcfFile::GetAppPath();
	DWORD dwHandle = cfgOpenFile(strPath + "bzgame.bcf");
	if ( dwHandle >= 0x10)
	{
		m_strFtpAddress = cfgGetValue(dwHandle,Glb().m_key,"FtpAddress","");
		m_strFtpUserName = cfgGetValue(dwHandle,Glb().m_key,"FtpUsrName","");
		m_strFtpPwd = cfgGetValue(dwHandle,Glb().m_key,"FtpPwd","");

		cfgClose(dwHandle);
	}

	CRect rc;
	GetWindowRect(&rc);
	m_bnClose.MoveWindow(rc.Width()-36,0,17,17);

	wsprintf(path,"%s\\%sdialog\\log_exit_bt.bmp",CBcfFile::GetAppPath(true),m_skinmgr.GetSkinPath());//win_close.bmp
	m_bnClose.LoadButtonBitmap(path,false);

	{
		CString s=CBcfFile::GetAppPath ();/////����·��
		CString strSkin = m_skinmgr.GetSkinBcfFileName();
		CBcfFile f( s + strSkin);
		TCHAR path[MAX_PATH];
		CString skinfolder;
		if ("skin0.bcf" == strSkin)
			skinfolder = f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
		else
			skinfolder = f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());
		wsprintf(path,"%sDialog\\scroll.bmp",skinfolder);
		HBITMAP hbmScorll = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
		SkinSB_Uninit(m_FileListCtrl.GetSafeHwnd());
		SkinSB_Uninit(GetDlgItem(IDC_EDIT_APPEAL_CONTENT)->GetSafeHwnd());
		SkinSB_Init(m_FileListCtrl.GetSafeHwnd(), hbmScorll);
		SkinSB_Init(GetDlgItem(IDC_EDIT_APPEAL_CONTENT)->GetSafeHwnd(), hbmScorll);

	}

	::SetWindowPos(m_hWnd,CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);

	SetWndRgn();// ���ô���Բ��2012.10.16 yyf
}
//�����ļ����õ�nameid
CString CAppealDlg::GetFileNameID(const CString& strFileName)
{
	int iRet = strFileName.Find("-");
	CString strRet = "";
	if (iRet == 0)
	{
		return strRet;
	}
	strRet = strFileName.Mid(0,iRet);
	return strRet;

}
//��Ͷ�߶Ի���
void CAppealDlg::OpenAppealDlg(TCHAR* pUserName)
{
	//wushuqun 2009.5.22
	//�ϴ�������ʾ
	m_bUpLoad = false;

	//���û�����ֵ
	m_strUserName.Format("%s",pUserName);

	ProcessRecordFile(m_strUserName);

	
	
	
	
	ShowWindow(SW_SHOW);

}
//�����ļ������õ��ļ�����ʱ��
//CString CAppealDlg::GetFileTime(const CString& strFileName)
//{
//	int istrLen = strFileName.GetLength();
//	int iRet = strFileName.Find("-");
//	CString strRet = "";
//	if (iRet == 0)
//	{
//		return strRet;
//	}
//	strRet = strFileName.Mid(iRet+1,istrLen - iRet - strlen(".zxh") - 1);
//	return strRet;
//}
//��Ҫһ���ļ���ʹ��id����Ϸ��һһ��Ӧ
CString CAppealDlg::GetGameName(DWORD dwHandle,const CString& uGameId)
{
	if(dwHandle < 0x10)
		return NULL;	
	//CString strTempId;
	//strTempId.Format("%d",uGameId);
	CString strRet = cfgGetValue(dwHandle,"GameName",uGameId,"");
	return strRet;
}

//ͨ����ѡ���bcf logĿ¼�б���һ��bcf�ļ�
void CAppealDlg::WriteGameInfoFile(TCHAR* pstGameName,UINT uNameId)
{
	if (pstGameName == NULL || uNameId < 10000000)
	{
		return;
	}
	//����Ŀ¼
	CString s=CBcfFile::GetAppPath ();/////����·��
	s += "log\\";
	CreateDirectory(s,NULL);
	s += "gamename.bcf";

	DWORD dwHandle = cfgOpenFile(s);

	if(dwHandle < 0x10)
		return;

	

	CString strTempId;
	strTempId.Format("%d",uNameId);
	//if (cfgGetValue(dwHandle,"GameName",strTempId,"") == "")
	//{
	//	cfgClose(dwHandle);
	//	return;
	//}
	cfgSetValue(dwHandle,"GameName",strTempId,pstGameName);
	cfgClose(dwHandle);
}



void CAppealDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	/*// 2012.12.16 yyf ����
	CGameImageHelper helplt(&m_imgDlgBk);
	helplt.BitBlt(dc.GetSafeHdc(),0,0);*/

	//�滭��ܵ�ͼ 2012.10.16 yyf
	m_configDlg.DrawViewFrame(&dc);

	CFont font;
	font.CreateFont(-12,0,0,0,400,0,0,0,DEFAULT_CHARSET,3,2,1,2,TEXT("Arial"));
	CFont* pOldFont = (CFont*)dc.SelectObject(&font);
	dc.SetBkMode(TRANSPARENT);
	
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

	dc.SetTextColor(RGB(252,255,255));
	CString strTitle = fMsg.GetKeyVal("AppealDlg","UserFeedBack","�û�����");
	dc.TextOut(5,5,strTitle);
	//dc.TextOut(strTitle,5,5);
	dc.SetTextColor(RGB(0,0,0));
	strTitle = fMsg.GetKeyVal("AppealDlg","UserName","�û���:");
	dc.TextOut(20,50,strTitle);
	strTitle = fMsg.GetKeyVal("AppealDlg","Telephone","��ϵ�绰:");
	dc.TextOut(20,90,strTitle);
	strTitle = fMsg.GetKeyVal("AppealDlg","Email","Email:");
	dc.TextOut(20,130,strTitle);

	/// {{Added by zxd 20100709 �ͷ�GDI��Դ
	dc.SelectObject(pOldFont);
	font.DeleteObject();
	/// Added by zxd 20100709 �ͷ�GDI��Դ}}

}

void CAppealDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

	POSITION pos = m_FileListCtrl.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		//AFCMessageBox(fMsg.GetKeyVal("AppealDlg","SelectAppealGame","��ѡ��ҪͶ�ߵ���Ϸ"));
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,fMsg.GetKeyVal("AppealDlg","SelectAppealGame","��ѡ��ҪͶ�ߵ���Ϸ"));
	    
		return;
	}
	int iItem = m_FileListCtrl.GetNextSelectedItem(pos);
	//m_FileListCtrl
	if (iItem < 0)
	{
		return;
	}
	CString strComent = "";
	GetDlgItem(IDC_EDIT_APPEAL_CONTENT)->GetWindowText(strComent);
	if (strComent == "")
	{
		//AFCMessageBox(fMsg.GetKeyVal("AppealDlg","AppealReason","��д��ҪͶ�ߵ���Ϸ��ԭ��"));
		DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,fMsg.GetKeyVal("AppealDlg","AppealReason","��д��ҪͶ�ߵ���Ϸ��ԭ��"));
		return;
	}
	list<RecordFileStruct>::iterator iter = m_fileList.begin();
	int iIndex = 0;
	for (;iter!=m_fileList.end();)
	{
		
		if (iIndex == iItem)
		{
			break;

		}
		else
		{
			iter++;
		}
		iIndex++;
		
	}	

	RecordFileStruct record = *iter;	
	int iRet = record.strWholeName.Find(record.strFileName);

	CString szPath = record.strWholeName.Mid(0,iRet);


	//CString szInfoFile = szPath + m_strUserName + "_";

	m_strInfoFileName = m_strUserName + "_";

	CString szFileName =  record.strFileName.Mid(0,record.strFileName.Find("."));
	m_strInfoFileName += szFileName;
	m_strInfoFileName += ".txt";

	m_strInfoFileWholeName = szPath + m_strInfoFileName;




	CStdioFile file;
	if (file.Open(m_strInfoFileWholeName,CFile::modeCreate | CFile::modeReadWrite) == false)
	{
		//AFCMessageBox(fMsg.GetKeyVal("AppealDlg","AppealError","Ͷ�߹��̳������Ժ����ԣ�"));
		DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,fMsg.GetKeyVal("AppealDlg","AppealError","Ͷ�߹��̳������Ժ����ԣ�"));
		//wushuqun 2009.5.22
		return;
		
	}
	file.SeekToBegin();
	file.WriteString(m_strUserName);
	file.WriteString("\n");
	file.WriteString(fMsg.GetKeyVal("AppealDlg","AppealContent","Ͷ�����ݣ�"));
	file.WriteString("\n");
	file.WriteString(strComent);
	file.WriteString("\n");

	GetDlgItem(IDC_EDIT_PHONE_NUM)->GetWindowText(strComent);
	if (strComent != "")
	{
		file.WriteString(fMsg.GetKeyVal("AppealDlg","Telephone","��ϵ�绰��"));
		file.WriteString(strComent);
		file.WriteString("\n");

	}
	GetDlgItem(IDC_EDIT_APPEAL_EMAIL)->GetWindowText(strComent);
	if (strComent != "")
	{
		file.WriteString(fMsg.GetKeyVal("AppealDlg","Email","Email��"));
			file.WriteString(strComent);
		file.WriteString("\n");
	}
	file.Close();





	CString strRemotName = m_strUserName + "_" +record.strFileName;


	CStringArray csa2;
	csa2.SetSize(0,6);
	csa2.Add(m_strFtpAddress);
	csa2.Add(m_strFtpUserName);
	csa2.Add(m_strFtpPwd);
	csa2.Add(strRemotName);
	csa2.Add(record.strWholeName);
	int  b=UploadFile(&csa2,this->m_hWnd,WM_USER+1);
	//wushuqun 2009.5.22
	if (b>0)
	{
		//AFCMessageBox(fMsg.GetKeyVal("AppealDlg","SendAppealError","Ͷ���ļ��ϴ�ʧ��!"));
		DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,fMsg.GetKeyVal("AppealDlg","SendAppealError","Ͷ���ļ��ϴ�ʧ��!"));
		
	}
	CDialog::OnOK();
}

BOOL CAppealDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

	if (pMsg->message == WM_USER + 1)
	{
		if (pMsg->wParam == 1 || pMsg->wParam == 5)
		{
			//wushuqun 2009.5.22
			//AFCMessageBox(fMsg.GetKeyVal("AppealDlg","SendAppealError","Ͷ���ļ��ϴ�ʧ��!"));
		    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,fMsg.GetKeyVal("AppealDlg","SendAppealError","Ͷ���ļ��ϴ�ʧ��!"));

			return CDialog::PreTranslateMessage(pMsg);
		}
		if (m_strInfoFileName == "" || m_strInfoFileWholeName == "")
		{
			return CDialog::PreTranslateMessage(pMsg);
		}

		if (pMsg->wParam == 4) //��zxh¼���ļ����ͳɹ����ٴ���txt�ļ�
		{
			CStringArray csa;
			csa.SetSize(0,6);
			csa.Add(m_strFtpAddress);
			csa.Add(m_strFtpUserName);
			csa.Add(m_strFtpPwd);
			csa.Add(m_strInfoFileName);   //Զ���ļ�����
			csa.Add(m_strInfoFileWholeName);       //�����ļ�����
			int b=UploadFile(&csa,this->m_hWnd,WM_USER+2);
		}
	}
	else if(pMsg->message == WM_USER + 2)
	{
		if (pMsg->wParam == 1 || pMsg->wParam == 5)
		{
			//wushuqun 2009.5.22
			//AFCMessageBox(fMsg.GetKeyVal("AppealDlg","SendAppealError","Ͷ���ļ��ϴ�ʧ��!"));
		    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,fMsg.GetKeyVal("AppealDlg","SendAppealError","Ͷ���ļ��ϴ�ʧ��!"));

			return CDialog::PreTranslateMessage(pMsg);
		}
		if (pMsg->wParam == 4)
		{
			//AFCMessageBox(fMsg.GetKeyVal("AppealDlg","SendAppealSuccess","Ͷ���ļ��Ѿ��ϴ��ɹ�!"));
		    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,fMsg.GetKeyVal("AppealDlg","SendAppealSuccess","Ͷ���ļ��Ѿ��ϴ��ɹ�!"));
		}
		//ɾ���������м��ļ�

		//
		//ɾ�����ɵ���Ϣ�ļ�
		::DeleteFile(m_strInfoFileWholeName);

		m_strInfoFileName = "";
		m_strInfoFileWholeName = "";

	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CAppealDlg::OnBnClickedClose()
{
	OnCancel();
}

void CAppealDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CAppealDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CAppealDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	POSITION pos = m_FileListCtrl.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	int iItem = m_FileListCtrl.GetNextSelectedItem(pos);
	if (iItem < 0)
	{
		return;
	}
	list<RecordFileStruct>::iterator iter = m_fileList.begin();
	int iIndex = 0;
	for (;iter!=m_fileList.end();)
	{
		
		if (iIndex == iItem)
		{
			break;

		}
		else
		{
			iter++;
		}
		iIndex++;
		
	}	
	RecordFileStruct record = *iter;
	
	CString strFile = record.strGamePath ;
	//wushuqun 2009.6.9
	CString sCmd=CBcfFile::GetAppPath ();
	sCmd = sCmd + "GamePlayer.exe";
	//��¼���ļ�
	::ShellExecute(m_hWnd,"open",sCmd,strFile,NULL,SW_NORMAL);

	*pResult = 0;
}

HBRUSH CAppealDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

LRESULT CAppealDlg::OnExchangeSkin(WPARAM wpara,LPARAM lpara)
{
	LoadSkin();
	//OnPaint();
	Invalidate();
	return LRESULT(0);
}

// ���ô���Բ�� 2012.10.15 yyf
void CAppealDlg::SetWndRgn(void)
{
	POINT m_arPoints[8];
	CRect winRect;
	GetWindowRect(&winRect);
	ScreenToClient(&winRect);
 
    //���Ͻ�
	m_arPoints[0].x = winRect.left;
	m_arPoints[0].y = winRect.top+3;
	m_arPoints[1].x = winRect.left+3;
	m_arPoints[1].y = winRect.top;

	//���Ͻ�
	m_arPoints[2].x = winRect.right-3;
	m_arPoints[2].y = winRect.top;
	m_arPoints[3].x = winRect.right;
	m_arPoints[3].y = winRect.top+3;

	//���½�
	m_arPoints[4].x = winRect.right;
	m_arPoints[4].y = winRect.bottom-4;
	m_arPoints[5].x = winRect.right-4;
	m_arPoints[5].y = winRect.bottom;

	//���½�
	m_arPoints[6].x = winRect.left+4;
	m_arPoints[6].y = winRect.bottom;
	m_arPoints[7].x = winRect.left;
	m_arPoints[7].y = winRect.bottom-4;

	HRGN hRgn = CreatePolygonRgn(m_arPoints, 8, WINDING);
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
		DeleteObject(hRgn);
	}
}
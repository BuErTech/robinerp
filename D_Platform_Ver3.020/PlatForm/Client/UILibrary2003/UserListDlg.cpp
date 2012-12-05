//�û��б�
#include "StdAfx.h"
#include "Resource.h"
#include "UserListDlg.h"
#include "GameFrameDlg.h"
#include "afcdevhead.h"

BEGIN_MESSAGE_MAP(CGameUserListCtrl, CHappyGoDlg)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CPictureHelper, CGameFaceGo)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_USER_LIST, OnNMRclickUserList)
	ON_NOTIFY(NM_CLICK, IDC_USER_LIST, OnNMLclickUserList)
	ON_NOTIFY(NM_DBLCLK, IDC_USER_LIST, OnNMDBclickUserList)
END_MESSAGE_MAP()

/*******************************************************************************************************/

//�Ƿ��ַ���
UINT CGameUserListCtrl::m_uItemKind[15]=
	{
		IK_NAME,			//�û���
		IK_USERID,
		IK_DESKNO,			//����
		IK_MONEY,			//���
		IK_WEALTH_LEVEL,
		IK_POINT,			//�ȼ�
		IK_LEVEL,
		IK_CHARM,
		IK_CHARM_LEVEL,
		IK_SEX,
		IK_ALL_COUNT,		//�ܾ���
		IK_WINCOUNT,		//ʤ��
		IK_LOSTCOUNT,		//���
		IK_MIDCOUNT,		//�;�
		IK_CUT_PERCENT		//������
	};

//���캯��
CGameUserListCtrl::CGameUserListCtrl() : CHappyGoDlg(m_uItemKind)
{
		m_iniFile="image\\client\\"+Glb().m_skin+"\\game\\gameskini.bcf";// TEXT("image\\game\\skin.ini");
}

//��������
CGameUserListCtrl::~CGameUserListCtrl()
{
	m_NewUserStateBmp.DeleteObject();
}

//��ʼ������
void CGameUserListCtrl::InitListCtrl(GetOrderName * pOrderFunction, UINT uComType)
{
	CHappyGoDlg::InitListCtrl(pOrderFunction,uComType);
	m_uComType=uComType;
	//��������
	TCHAR szBuffer[10]=TEXT("����");
	//if (uComType==TY_MONEY_GAME) lstrcpy(szBuffer,TEXT("���"));
	if (uComType==TY_MATCH_GAME) 
		lstrcpy(szBuffer,TEXT("���"));

	//��������
	SetRedraw(FALSE);
	//InsertColumn(0,"�û���",LVCFMT_LEFT,115);
	
	//if (uComType==TY_MONEY_GAME)
	{
		int i = 0;
		InsertColumn(i++,"�ǳ�",LVCFMT_LEFT,115);


		InsertColumn(i++,"���",LVCFMT_LEFT,60);
		InsertColumn(i++,"�Ƹ�����",LVCFMT_LEFT,85);
		/////////////////////////////////
		////Kylin 20090119 ���ҳ���ʾ����
		if(uComType==TY_MATCH_GAME)
		{
			InsertColumn(i++,"����",LVCFMT_LEFT,55);
		}
		else
		{
			InsertColumn(i++,"����",LVCFMT_LEFT,55);
		}
		/////////////////////////////////
		InsertColumn(i++,"���ֵȼ�",LVCFMT_LEFT,85);
		InsertColumn(i++,"����ֵ",LVCFMT_LEFT,60);
		InsertColumn(i++,"��������",LVCFMT_LEFT,55);
		InsertColumn(i++,"�Ա�",LVCFMT_LEFT,40);
		////JMod-������ְҵ������������Ϣ  yjj 090506
		//InsertColumn(i++,"ְҵ",LVCFMT_LEFT,80);
		//InsertColumn(i++,"��������",LVCFMT_LEFT,80);
		InsertColumn(i++,"�ܾ���",LVCFMT_LEFT,50);
		InsertColumn(i++,"ʤ����",LVCFMT_LEFT,50);
		InsertColumn(i++,"�����",LVCFMT_LEFT,50);
		InsertColumn(i++,"�;���",LVCFMT_LEFT,50);
		InsertColumn(i++,"������",LVCFMT_LEFT,50);
		InsertColumn(i++,"�Թ۶���",LVCFMT_CENTER,100);
	}
	//else
	//{
	//	InsertColumn(0,"�ǳ�",LVCFMT_LEFT,115);
	//	InsertColumn(1,"����",LVCFMT_LEFT,55);
	//	InsertColumn(2,"���ֵȼ�",LVCFMT_LEFT,85);
	//	InsertColumn(3,"�Ա�",LVCFMT_LEFT,40);
	//	InsertColumn(4,"�ܾ���",LVCFMT_LEFT,50);
	//	InsertColumn(5,"ʤ����",LVCFMT_LEFT,50);
	//	InsertColumn(6,"�����",LVCFMT_LEFT,50);
	//	InsertColumn(7,"�;���",LVCFMT_LEFT,50);
	//	InsertColumn(8,"������",LVCFMT_LEFT,50);
	//	InsertColumn(9,"�Թ۶���",LVCFMT_CENTER,100);
	//}
	//InsertColumn(10,"�û�IP",LVCFMT_LEFT,120);

	SetRedraw(TRUE);

	return;
}

//�����Թ۶���
void CGameUserListCtrl::SetWatchUser(UserItemStruct * pUserItem, TCHAR * szWatchName)
{
	if (pUserItem==NULL) return;

	//�����û�
	LVFINDINFO FindInfo;
	memset(&FindInfo,0,sizeof(FindInfo));
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=(LPARAM)pUserItem;
	int iItem=FindItem(&FindInfo);
	if (iItem==-1) return;

	//�Թ۶���
	//if(m_uComType!=TY_MONEY_GAME)
	//{
	//	if (szWatchName!=NULL) 
	//		SetItem(iItem,11,1,szWatchName,0,0,0,0);
	//	else 
	//		SetItem(iItem,11,1,TEXT(""),0,0,0,0);
	//}
	//else if(m_uComType==TY_MONEY_GAME)
	//{
		if (szWatchName!=NULL) 
			SetItem(iItem,13,1,szWatchName,0,0,0,0);
		else 
			SetItem(iItem,13,1,"Ok",0,0,0,0);//TEXT(IDOK)
	//}
	return;
}

//�Ƿ�����鿴 IP
void CGameUserListCtrl::EnableIPView(bool bLookIP)
{
	//if (bLookIP&&m_uComType!=TY_MONEY_GAME)
	//	InsertColumn(12,"�û�IP",LVCFMT_LEFT,120);
	//else 
		if(bLookIP)//&&m_uComType==TY_MONEY_GAME)
		InsertColumn(16,"�û�IP",LVCFMT_LEFT,120);
	//else if(!bLookIP&&m_uComType!=TY_MONEY_GAME)
	//	DeleteColumn(12);
	else 
		if(!bLookIP)//&&m_uComType==TY_MONEY_GAME)
		DeleteColumn(16);
	return;
}

//�����û�
bool CGameUserListCtrl::AddGameUser(UserItemStruct * pUserItem)
{
	if (pUserItem==NULL) return false;

	//�����û�
	LVFINDINFO FindInfo;
	::memset(&FindInfo,0,sizeof(FindInfo));
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=(LPARAM)pUserItem;
	int iItem=FindItem(&FindInfo);
	if (iItem!=-1) 
	{
		UpdateGameUser(pUserItem);
		return false;
	}
	
	//�������
	CString strBuffer;
	UserInfoStruct * pUserInfo=&pUserItem->GameUserInfo;
	UINT uAllCount=pUserInfo->uWinCount+pUserInfo->uLostCount+pUserInfo->uMidCount;
	UINT uCutPercent=(pUserInfo->uCutCount>0)?(UINT)((double)(pUserInfo->uCutCount*100)/(double)uAllCount):0;
	
	//�û�����
	iItem=InsertItem(GetItemCount(),pUserInfo->szName,GetStateImageIndex(pUserItem));
	//�ݳ�
	SetItem(iItem,0,1,pUserInfo->nickName,0,0,0,0);
	
	{		
		
		int dwPoint=0,dwFascination;
        __int64 i64Money=0;
		i64Money=pUserInfo->i64Money;
		dwPoint=pUserInfo->dwPoint;
		dwFascination=pUserInfo->dwFascination;
		int i = 1;

		//���
		TCHAR szNum[128];
        GetNumString(szNum, i64Money, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
		strBuffer.Format(TEXT("%s"),szNum);
		SetItem(iItem,i++,1,strBuffer,0,0,0,0);
		//�Ƹ�����
		SetItem(iItem,i++,1,GetMoneyOrderName(i64Money,0),0,0,0,0);
			
		//���ּ���
		strBuffer.Format(TEXT("%d"),dwPoint);
		SetItem(iItem,i++,1,strBuffer,0,0,0,0);
		
		//�ȼ�
		SetItem(iItem,i++,1,m_pGetOrderFun(dwPoint,0),0,0,0,0);	

		//����
		strBuffer.Format(TEXT("%d"),dwFascination);
		SetItem(iItem,i++,1,strBuffer,0,0,0,0);

		//��������
		strBuffer.Format(TEXT("%s"),GetCharmGrade(dwFascination));
		SetItem(iItem,i++,1,strBuffer,0,0,0,0);		
		
		//�Ա�
		SetItem(iItem,i++,1,pUserItem->GameUserInfo.bBoy?"��":"Ů",0,0,0,0);
		////wushuqun 2009.5.19
		////���������Ϊ��ʡ.��"
		//SetItemText(iItem,i++,pUserInfo->szOccupation);
		//CString strAre;
		//strAre = pUserInfo->szProvince + CString(".") + pUserInfo->szCity;
		//SetItemText(iItem,i++,strAre);

		//�ܾ���
		strBuffer.Format(TEXT("%ld"),uAllCount);
		SetItem(iItem,i++,1,strBuffer,0,0,0,0);
		//ʤ��
		strBuffer.Format(TEXT("%ld"),pUserInfo->uWinCount);
		SetItem(iItem,i++,1,strBuffer,0,0,0,0);
		//���
		strBuffer.Format(TEXT("%d"),pUserInfo->uLostCount);
		SetItem(iItem,i++,1,strBuffer,0,0,0,0);
		//�;�
		strBuffer.Format(TEXT("%d"),pUserInfo->uMidCount);
		SetItem(iItem,i++,1,strBuffer,0,0,0,0);
		//������
		if (uCutPercent>0)
			strBuffer.Format(TEXT("%2ld%%"),uCutPercent);
		else
			strBuffer.Empty();
		SetItem(iItem,i++,1,strBuffer,0,0,0,0);
		//��Ϸ����
		//SetItem(iItem,8,1,pUserInfo->szClassName,0,0,0,0);
		////��Ϸ IP
		//strBuffer.Format(TEXT("%d.%d.%d.%d"),FOURTH_IPADDRESS(pUserInfo->dwUserIP),THIRD_IPADDRESS(pUserInfo->dwUserIP),
		//SECOND_IPADDRESS(pUserInfo->dwUserIP),FIRST_IPADDRESS(pUserInfo->dwUserIP));
		//SetItem(iItem,i++,1,strBuffer,0,0,0,0);
		
		//������չ��Ϣ
		SetItemData(iItem,(DWORD_PTR)pUserItem);
	}

	return true;
}

//ɾ���û�
bool CGameUserListCtrl::DeleteGameUser(UserItemStruct * pUserItem)
{
	if (pUserItem==NULL) return false;

	//�����û�
	LVFINDINFO FindInfo;
	::memset(&FindInfo,0,sizeof(FindInfo));
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=(LPARAM)pUserItem;
	int iItem=FindItem(&FindInfo);
	if (iItem==-1) return false;

	//ɾ���û�
	DeleteItem(iItem);
	return true;
}

//����״̬
bool CGameUserListCtrl::UpdateGameUser(UserItemStruct * pUserItem)
{
	if (pUserItem==NULL) return false;

	//�����û�
	LVFINDINFO FindInfo;
	memset(&FindInfo,0,sizeof(FindInfo));
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=(LPARAM)pUserItem;
	int iItem=FindItem(&FindInfo);
	if (iItem==-1) return false;

	//�������
	CString strBuffer;
	UserInfoStruct * pUserInfo=&pUserItem->GameUserInfo;
	UINT uAllCount=pUserInfo->uWinCount+pUserInfo->uLostCount+pUserInfo->uMidCount;
	UINT uCutPercent=(pUserInfo->uCutCount>0)?(UINT)((double)(pUserInfo->uCutCount*100)/(double)uAllCount):0;

	//����ͼ��
	SetItem(iItem,0,LVIF_IMAGE,NULL,GetStateImageIndex(pUserItem),0,0,0,0);
	//�ݳ�
	SetItemText(iItem,0,pUserItem->GameUserInfo.nickName);
	
	//�÷�
	int dwPoint;
    __int64 i64Money;
	dwPoint=pUserInfo->dwPoint;
	i64Money=pUserInfo->i64Money;
	int dwFascination=pUserInfo->dwFascination;
	//if(m_uComType==TY_MONEY_GAME)
	{
		int i = 1;


		//�Ƹ�
		TCHAR szNum[128];
        GetNumString(szNum, i64Money, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
		strBuffer.Format(TEXT("%d"),szNum);
		SetItemText(iItem,i++,strBuffer);
		//�Ƹ�����
		SetItemText(iItem,i++,GetMoneyOrderName(i64Money,0));
		//����ֵ
		strBuffer.Format(TEXT("%d"),dwPoint);
		SetItemText(iItem,i++,strBuffer);
	
		//�ȼ�
		SetItemText(iItem,i++,m_pGetOrderFun(dwPoint,0));

		//����ֵ
		strBuffer.Format(TEXT("%d"),dwFascination);
		SetItemText(iItem,i++,strBuffer);

		//��������
		strBuffer.Format(TEXT("%s"),GetCharmGrade(dwFascination));
		SetItemText(iItem,i++,strBuffer);		
		//�Ա�
		SetItemText(iItem,i++,pUserItem->GameUserInfo.bBoy?"��":"Ů");

		////wushuqun 2009.5.19
		////���������Ϊ��ʡ.��"
		//SetItemText(iItem,i++,pUserInfo->szOccupation);
		//CString strAre;
		//strAre = pUserInfo->szProvince + CString(".") + pUserInfo->szCity;
		//SetItemText(iItem,i++,strAre);

		//�ܾ���
		strBuffer.Format(TEXT("%ld"),uAllCount);
		SetItemText(iItem,i++,strBuffer);
		//ʤ��
		strBuffer.Format(TEXT("%ld"),pUserInfo->uWinCount);
		SetItemText(iItem,i++,strBuffer);
		//���
		strBuffer.Format(TEXT("%d"),pUserInfo->uLostCount);
		SetItemText(iItem,i++,strBuffer);
		//�;�
		strBuffer.Format(TEXT("%d"),pUserInfo->uMidCount);
		SetItemText(iItem,i++,strBuffer);
		//������
		if (uCutPercent>0) 
			strBuffer.Format(TEXT("%2ld%%"),uCutPercent);
		else strBuffer.Empty();
		SetItemText(iItem,i++,strBuffer);
		
		if (pUserInfo->bGameMaster>0)
		{
			strBuffer.Format(TEXT("%d.%d.%d.%d"),FOURTH_IPADDRESS(pUserInfo->dwUserIP),THIRD_IPADDRESS(pUserInfo->dwUserIP),
				SECOND_IPADDRESS(pUserInfo->dwUserIP),FIRST_IPADDRESS(pUserInfo->dwUserIP));
			SetItemText(iItem,14,strBuffer);
		}
	}

	return true;
}

void CGameUserListCtrl::SetIniFile(LPCTSTR path)
{
	m_iniFile = path;
}

void CGameUserListCtrl::LoadSkin(void)
{
	//������Ϣ
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + m_iniFile);
//	CINIFile f( s + "skin.ini");
	//CString key=Glb().m_skin;
	CString skinfolder= "image\\client\\skin0\\";
	skinfolder.Format("%s\\image\\client\\%s\\",CBcfFile::GetAppPath(),Glb().m_skin);
	//skinfolder=f.GetKeyVal(key,"skinfolder",);
	CString key=TEXT("config");
	//CString skinfolder;
	//skinfolder=f.GetKeyVal(key,"skinfolder1","image\\client\\skin0\\");
	int r,g,b;
	r=f.GetKeyVal(key,"glisttextclr",0);
	g=f.GetKeyVal(key,"glisttextclg",255);
	b=f.GetKeyVal(key,"glisttextclb",255);
	COLORREF textcr=RGB(r,g,b);

	r=f.GetKeyVal(key,"glistbkclr",0);
	g=f.GetKeyVal(key,"glistbkclg",233);
	b=f.GetKeyVal(key,"glistbkclb",233);
	COLORREF bkcr=RGB(r,g,b);
	//m_bkColor = bkcr;
	r=f.GetKeyVal(key,"gflisttextclr",0);
	g=f.GetKeyVal(key,"gflisttextclg",255);
	b=f.GetKeyVal(key,"gflisttextclb",255);
	COLORREF ftextcr=RGB(r,g,b);

	r=f.GetKeyVal(key,"gflistbkclr",0);
	g=f.GetKeyVal(key,"gflistbkclg",233);
	b=f.GetKeyVal(key,"gflistbkclb",233);
	COLORREF fbkcr=RGB(r,g,b);
	SetColor(bkcr,textcr,fbkcr,ftextcr);

	//�б�ͷ
	r=f.GetKeyVal(key,"glistheadr",0);
	g=f.GetKeyVal(key,"glistheadg",59);
	b=f.GetKeyVal(key,"glistheadb",108);
	COLORREF color=RGB(r,g,b);
	TCHAR path[MAX_PATH],path1[MAX_PATH];
	wsprintf(path,"%sroom_right\\list_head1.bmp",skinfolder);
	wsprintf(path1,"%sroom_right\\list_head2.bmp",skinfolder);
	m_ListHeader.SetBkImage(path,path1,color,color,1);

	SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE|LVS_EX_INFOTIP);
	SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT,LVS_EX_FULLROWSELECT);

	//����ͼƬ
	//if (m_UserStateList.GetSafeHandle()==NULL)
	{
		//װ��λͼ
		BITMAP BmpInfo;
		CGameImage	UserStateBmp;
		//CBitmap NewUserStateBmp;
		AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
		UserStateBmp.SetLoadInfo(".\\image\\user_state.bmp",true);;
		AfxSetResourceHandle(GetModuleHandle(NULL));
		//UserStateBmp.GetBitmap(&BmpInfo);
		BmpInfo.bmWidth=UserStateBmp.GetWidth();
		BmpInfo.bmHeight=UserStateBmp.GetHeight();

		//����λͼ
		CDC TargetDC, * pDC=GetDC();
		TargetDC.CreateCompatibleDC(pDC);
		m_NewUserStateBmp.CreateCompatibleBitmap(pDC,BmpInfo.bmWidth,BmpInfo.bmHeight);
		CBitmap * pOldBmp=TargetDC.SelectObject(&m_NewUserStateBmp);
		TargetDC.FillSolidRect(0,0,BmpInfo.bmWidth,BmpInfo.bmHeight,m_bkColor);

		AFCStretchImage(&TargetDC,0,0,BmpInfo.bmWidth,BmpInfo.bmHeight,UserStateBmp,0,0,BmpInfo.bmWidth,BmpInfo.bmHeight,RGB(255,255,255));
		TargetDC.SelectObject(pOldBmp);
		TargetDC.DeleteDC();

		//������Դ��
		if(m_UserStateList.m_hImageList)
		{
			for (int i=0; i<m_UserStateList.GetImageCount(); ++i)
			{
				m_UserStateList.Remove(0);
			}
			m_UserStateList.DeleteImageList();
		}
		if (m_UserStateList.GetImageCount()<=0)
		{
			m_UserStateList.Create(USER_STATE_WIDTH,USER_STATE_HEIGHT,ILC_COLOR16,0,0);
			m_UserStateList.Add(&m_NewUserStateBmp,RGB(0,0,255));
		}

		//������Դ
		UserStateBmp.Detach();
		//NewUserStateBmp.Detach();
		ReleaseDC(pDC);
	}
	SetImageList(&m_UserStateList,LVSIL_SMALL);
}
//���캯��
CPictureHelper::CPictureHelper() : CGameFaceGo(IDD_USER_LIST)
{
}
//��������
CPictureHelper::~CPictureHelper()
{
}

//�ż��󶨺���
void CPictureHelper::DoDataExchange(CDataExchange * pDX)
{
	CGameFaceGo::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USER_LIST, m_UserList);
}

//λ�ñ仯
void CPictureHelper::OnSize(UINT nType, int cx, int cy)
{
	CGameFaceGo::OnSize(nType, cx, cy);
	//�ƶ��ؼ�
	SafeMoveWindow(&m_UserList,0,0,cx,cy);

	return;
}

//����Ҽ��û��б���Ϣ
void CPictureHelper::OnNMRclickUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	if (pListNotify->iItem!=-1)
	{
		UserItemStruct * pUserItem=(UserItemStruct *)m_UserList.GetItemData(pListNotify->iItem);
		((CLoveSendClass *)GetParent())->OpenUserMenu(pUserItem);
	}
	*pResult=0;
}


//�������û��б���Ϣ
void CPictureHelper::OnNMLclickUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	if (pListNotify->iItem!=-1)
	{
		UserItemStruct * pUserItem=(UserItemStruct *)m_UserList.GetItemData(pListNotify->iItem);
		((CLoveSendClass *)GetParent())->OnLBClickUserList(pUserItem);
	}
	*pResult=0;
}

/*******************************************************************************************************/

void CPictureHelper::OnNMDBclickUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	if (pListNotify->iItem!=-1)
	{
		UserItemStruct * pUserItem=(UserItemStruct *)m_UserList.GetItemData(pListNotify->iItem);
		((CLoveSendClass *)GetParent())->OnLBDBClickUserList(pUserItem);
	}

	*pResult=0;
}


#include "Stdafx.h"
#include "UserListCtrl.h"
#include "GameRoomEx.h"	//Ϊ�Ŷӻ����

BEGIN_MESSAGE_MAP(CUserListCtrl, CHappyGoDlg)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//�Ƿ��ַ���
UINT CUserListCtrl::m_uItemKind[15]=
{
	IK_NAME,			//�û���
	IK_USERID,			//�û�ID
	IK_DESKNO,			//����
	IK_MONEY,			//���
	IK_WEALTH_LEVEL,	//�Ƹ��ȼ�
	IK_POINT,			//����
	IK_LEVEL,			//���ֵȼ�
	IK_CHARM,			//����
	IK_CHARM_LEVEL,		//�����ȼ�
	IK_SEX,				//�Ա�
	IK_ALL_COUNT,		//�ܾ���
	IK_WINCOUNT,		//ʤ��
	IK_LOSTCOUNT,		//���
	IK_MIDCOUNT,		//�;�
	IK_CUT_PERCENT		//������
};

//���캯��
CUserListCtrl::CUserListCtrl() : CHappyGoDlg(m_uItemKind)
{
	m_uVirtualUserCount=0;
	LoadColumnConfig();
}

//��������
CUserListCtrl::~CUserListCtrl()
{
}
static int VirtualList[BUFSIZ]={0};

const TCHAR COLOMN_NAME[][16] = 
{
	"�ǳ�,160", "ID,40", "����,40", "���,60", "��Ҽ���,90", "����,60", "���ּ���,90", "����,60", "��������,90", "�Ա�,40", "�ܾ���,50", "ʤ,50", "��,50", "��,50", "����,50"
};
///
///����������
///
///@param void
///
void CUserListCtrl::LoadColumnConfig()
{
	CString strColumnName;
	CString strColumnIndex;
	CString strColumnIndexAll;
	CString strConfig;
	BOOL    bConfigExist;
	
	CString strDefaultColumnIndexAll = "Nickname,UserID,DeskNo,Coin,WealthLevel,Score,ScoreLevel,Charm,CharmLevel,Sex,GameCount,WinGame,LoseGame,Peace,Escape,";
	bConfigExist = TRUE;

	CBcfFile fUserList(CBcfFile::GetAppPath()+"UserInforList.bcf");
	strColumnIndexAll = fUserList.GetKeyVal("InforList","_ColumnOrder","");
	if (strColumnIndexAll.IsEmpty())
	{
		bConfigExist = FALSE;
		strColumnIndexAll = strDefaultColumnIndexAll;
	}

	int iIndex = 0;
	int iWidth = 0;
	int nColCount = 0;
	int nHeadPos = 0;
	int nTailPos = strColumnIndexAll.Find(',', nHeadPos);
	while (nTailPos > nHeadPos)
	{
		ColumnStruct ColStr;

		strColumnIndex = strColumnIndexAll.Mid(nHeadPos, nTailPos-nHeadPos);
		if (bConfigExist)
			strConfig = fUserList.GetKeyVal("InforList",strColumnIndex,"");
		else
			strConfig = COLOMN_NAME[iIndex];

		if (!strConfig.IsEmpty())
		{
			DecodeString(strConfig,strColumnName,iWidth);
			ColStr.index = iIndex;
			ColStr.Width = iWidth;
			strncpy(ColStr.ColumnName,strColumnName,MAX_PATH/2);
			m_MapColumn.insert(pair<string,ColumnStruct>(strColumnIndex.GetBuffer(),ColStr));
			iIndex++;
		}
		
		nHeadPos = nTailPos + 1;
		nTailPos = strColumnIndexAll.Find(',', nHeadPos);
	}
}

///
///���ַ��������� ˳��ţ��������п�
///
///@param [in]str ����ֵ�ַ�����[out]strColumn ����ֵ ������[out]width ����ֵ �п�
///
///@return void
///
void CUserListCtrl::DecodeString(CString str, CString &strColumn, int &width)
{
	int pos;
	CString strTemp;
	
	strTemp = str;
	pos = strTemp.Find(',');
	if (pos >= 0)
	{
		strColumn = strTemp.Left(pos);

		strTemp = strTemp.Right(strTemp.GetLength()-pos-1);
		if (!strTemp.IsEmpty())
		{
			width = atoi(strTemp);
		}
	}	
}


///
///��ʼ������
///
///@param [in]pOrderFunction ������ַ�� [in]GameInfo �û���Ϣ
///
///@return void
///
void CUserListCtrl::InitListCtrl(GetOrderName * pOrderFunction, UINT uComType,GameInfoStruct *GameInfo)
{
	CHappyGoDlg::InitListCtrl(pOrderFunction,uComType);
	
	m_uComType = uComType;
	pGameInfo  = GameInfo;

	MapColumn::iterator *pIte;
	pIte = new MapColumn::iterator[m_MapColumn.size()];

	MapColumn::iterator ite;
	for (ite=m_MapColumn.begin(); ite!=m_MapColumn.end(); ite++)
	{
		pIte[ite->second.index] = ite;
	}

	int iCnt = m_MapColumn.size();
	for (int i=0; i<iCnt; i++)
	{
		InsertColumn(pIte[i]->second.index,pIte[i]->second.ColumnName,LVCFMT_CENTER,pIte[i]->second.Width);
	}
	delete [] pIte;

	SetRedraw(FALSE);
	return;
}

//�Ƿ�����鿴 IP
void CUserListCtrl::EnableIPView(bool bLookIP)
{
	if(bLookIP)
	{
		CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
		InsertColumn(16,fMsg.GetKeyVal("UserListCtrl","UserIP","�û�IP"),LVCFMT_LEFT,120);
	}
	else if(!bLookIP)
	{
		DeleteColumn(16);
	}
	return;
}

///
///��ȡ�û������λ��
///
///@param pUserItem �û���Ϣ
///
///@return int �����λ��
///
int CUserListCtrl::GetInsertPos(UserItemStruct * pUserItem)
{
	int iItemCnt;
	int iCnt = 1;
	UserItemStruct *pUser = NULL;
	iItemCnt= GetItemCount();

	if (0 == iItemCnt)
		return 0;
	while (iCnt<iItemCnt)
	{
		pUser = (UserItemStruct *)GetItemData(iCnt++);
		if (NULL != pUser)
		{
			if (pUserItem->GameUserInfo.bMember > pUser->GameUserInfo.bMember)
			{
				iCnt--;
				return iCnt;
			}
			else if (pUserItem->GameUserInfo.bMember == pUser->GameUserInfo.bMember) ///< ͬ�ȼ�VIP������Ǯ������
			{
				if (pUserItem->GameUserInfo.i64Money >= pUser->GameUserInfo.i64Money)
				{
					iCnt--;
					return iCnt;
				}
			}
		}
	}
	return iCnt;
}

///
///�����û�
///
///@param pUserItem �û���Ϣ
///
///@return bool
///
bool CUserListCtrl::AddGameUser(UserItemStruct * pUserItem)
{
	if (NULL == pUserItem)
		return false;

	int iItem;
	LVFINDINFO FindInfo;
	::memset(&FindInfo,0,sizeof(FindInfo));
	FindInfo.flags  = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)pUserItem;
	iItem = FindItem(&FindInfo);///< �����û�

	if (-1 != iItem) 
	{
		UpdateGameUser(pUserItem);
		return true;
	}

	PrepareDate(pUserItem); ///< ��������䵽MAP��
	//CGameRoomEx* pGameRoomEx = GetGameRoom();
	CGameRoomEx* pGameRoomEx = GetGameRoom(pGameInfo->uRoomID);
	if (!pGameRoomEx)
	{
		return false;
	}

	bool bNotMySelf = (pGameRoomEx->m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID
		!= pUserItem->GameUserInfo.dwUserID);
	if (bNotMySelf)
	{
		iItem = InsertItem(GetInsertPos(pUserItem),NULL,0); ///< ����
	}
	else
	{
		iItem = InsertItem(0,NULL,0); ///< ����
	}

	MapColumn::iterator ite;
	for (ite=m_MapColumn.begin(); ite!= m_MapColumn.end(); ite++)
	{
		if (-1 != ite->second.index)///< ���ص���˳���Ϊ-1
		{
			SetItemText(iItem,ite->second.index,ite->second.Value);
		}
	}
	///< ������չ��Ϣ
	SetItemData(iItem,(DWORD_PTR)pUserItem);

	return true;
}

///
///����״̬
///
///@param pUserItem �û���Ϣ
///
///@return bool
///
bool CUserListCtrl::UpdateGameUser(UserItemStruct * pUserItem)
{
	if (pUserItem==NULL && this->m_hWnd == NULL)
		return false;

	int iItem;
	LVFINDINFO FindInfo;

	::memset(&FindInfo,0,sizeof(FindInfo));
	FindInfo.flags  = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)pUserItem;
	iItem = FindItem(&FindInfo);///< �����û�
	if (iItem==-1)
		return false;

	PrepareDate(pUserItem); ///< ��������䵽MAP��

	MapColumn::iterator ite;
	for (ite=m_MapColumn.begin(); ite!= m_MapColumn.end(); ite++)
	{
		if (-1 != ite->second.index)///< ���ص���˳���Ϊ-1
		{
			SetItem(iItem,ite->second.index,1,ite->second.Value,0,0,0,0);
		}
	}

	return true;
} 

/// ��ȡ���ID��Ϣ���Ƿ�ɢ�����ر��˺ţ�ָ���˺�
/// @param UserID ���ID,NomalIDFromɢ��ID��ʼ��NomalIDEndɢ��ID����
/// @return 0��10��ָ���˺ź����0�ĸ�����-1Ϊɢ����-2Ϊ��Ч�˺ţ�-5Ϊ5���ţ�-6Ϊ6����
int CUserListCtrl::GetIDInformation(long UserID,long NomalIDFrom,long NomalIDEnd)
{
	char BufID[12]     = {0};
	int CntZero        = 0;   ///< 0�ĸ���
	int len            = 0;
	BOOL sameNumID     = TRUE;  ///< �Ƿ�����ID��11111 444444���Լ���ID��

	if (UserID>NomalIDFrom && UserID<NomalIDEnd) ///< ɢ��
	{
		return -1;
	}

	if (UserID < NomalIDFrom)
	{
		sprintf(BufID,"%ld",UserID);

		for (int i=0; 0!=BufID[i]; i++)
		{
			if ('0'==BufID[i])
			{
				CntZero++;
			}
			else if ('0'!=BufID[i] && 0!=CntZero) ///< ��Ч�˺�
			{
				return -2;
			}
			
			if (i>0 && BufID[0]!=BufID[i] && sameNumID)
			{
				sameNumID = FALSE;
			}
			len++;
		}

		if (sameNumID) ///< ������ID
		{
			if (len >= 5)
				return -len;
			else
				return -2; ///< ��Ч�˺�
		}

		char BufIDFrom[12] = {0};
		sprintf(BufIDFrom,"%ld",NomalIDFrom);
		if (strlen(BufIDFrom) == len)
		{
			if ('4' == BufID[len-CntZero-1]) ///< ��ְ
			{
				return CntZero+1; ///< 4�ڵ�����CntZero+1λ
			}
			return CntZero;
		}
		else
			return -2; ///< ��Ч�˺�
	}

	return -2; ///< ��Ч�˺�
}

///
///׼��Ҫ��������,�����ݷ���MAP��
///
///@param pUserItem �û���Ϣ
///
///@return void
///
void CUserListCtrl::PrepareDate(UserItemStruct *pUserItem)
{
	CString strBuffer;
	CString strTemp;
	MapColumn::iterator ite;

	UserInfoStruct * pUserInfo = &pUserItem->GameUserInfo;
	UINT uAllCount   = pUserInfo->uWinCount + pUserInfo->uLostCount + pUserInfo->uMidCount;
	UINT uCutPercent = (pUserInfo->uCutCount>0)?(UINT)((double)(pUserInfo->uCutCount*100)/(double)uAllCount):0;
	int dwPoint,dwFascination;
    __int64 i64Money;
	i64Money       = pUserInfo->i64Money;
	dwPoint       = pUserInfo->dwPoint;
	dwFascination = pUserInfo->dwFascination; 

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");

	if (pGameInfo)
	{
		///< Ϊ�Ŷӻ����
		//CGameRoomEx* pGameRoomEx = GetGameRoom();

		CGameRoomEx* pGameRoomEx = GetGameRoom(pGameInfo->uRoomID);
		if (!pGameRoomEx)
		{
			return;
		}

		//if(((pGameInfo->dwRoomRule & GRR_AUTO_SIT_DESK) || ((pGameInfo->dwRoomRule & GRR_QUEUE_GAME) > 0))
		//	&& (pGameRoomEx->m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID != pUserInfo->dwUserID)
		//	)
		if ((pGameInfo->dwRoomRule & GRR_NOTCHEAT) && pGameRoomEx->m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID != pUserInfo->dwUserID)
		{
			strBuffer = fMsg.GetKeyVal("UserListCtrl","Player","���"); ///< ����ǳ� ����
			strTemp   = "???"; ///< ���ID ����
		}
		else
		{
			strBuffer = pUserInfo->nickName; ///< ����ǳ� ��ʾ
			if (pUserInfo->dwUserID >= 0)
			strTemp.Format(TEXT("%d"),pUserInfo->dwUserID); ///< ���ID ��ʾ
		}
		
		ite = m_MapColumn.find("Nickname"); ///< ����ǳ�
		if (m_MapColumn.end() != ite)
		{
			strncpy(ite->second.Value,strBuffer,32);
		}

		ite = m_MapColumn.find("UserID"); ///< ���ID
		if (m_MapColumn.end() != ite)
		{
			strncpy(ite->second.Value,strTemp,32);
		}
		///< end of Ϊ�Ŷӻ����
	}
	else
	{
		strBuffer = pUserInfo->nickName; ///< ����ǳ� ��ʾ
		if (pUserInfo->dwUserID >= 0)
			strTemp.Format(TEXT("%d"),pUserInfo->dwUserID); ///< ���ID ��ʾ

		ite = m_MapColumn.find("Nickname"); ///< ����ǳ�
		if (m_MapColumn.end() != ite)
		{
			strncpy(ite->second.Value,strBuffer,32);
		}

		ite = m_MapColumn.find("UserID"); ///< ���ID
		if (m_MapColumn.end() != ite)
		{
			strncpy(ite->second.Value,strTemp,32);
		}
	}

	if (1&Glb().m_CenterServerPara.m_nFunction) ///< �Ƿ���«2������
	{
		ite = m_MapColumn.find("VIPLevel"); ///< VIP�ȼ�
		if (m_MapColumn.end() != ite)
		{
			int iVIP = GetIDInformation(pUserInfo->dwUserID,
				Glb().m_CenterServerPara.m_lNomalIDFrom,
				Glb().m_CenterServerPara.m_lNomalIDEnd); ///< ����ID����VIP�ȼ�
			if (iVIP>=0 && iVIP<7)
			{
				strBuffer.Format("VIPLevel%d",iVIP);
				strBuffer = fMsg.GetKeyVal("UserListCtrl",strBuffer,strBuffer);
			}
			else if(iVIP>-9 && iVIP<-4) ///< 5���ŵ�8����
			{
				strBuffer.Format("VIPLevel%d",-iVIP-2);
				strBuffer = fMsg.GetKeyVal("UserListCtrl",strBuffer,strBuffer);
			}
			else
			{
				strBuffer.Empty();
			}
			if (strBuffer.IsEmpty())
				ite->second.Value[0] = 0;
			else
			strncpy(ite->second.Value,strBuffer,32);
		}
	}

	//����
	if (pUserInfo->bDeskNO!=255 && m_uComType!=TY_MATCH_GAME)
	{
        // PengJiLin, 2010-5-19, �Ŷӻ�����ʾ���š�2010-9-16: �����жϱ������
        //if((NULL != pGameInfo) && ((pGameInfo->dwRoomRule & GRR_AUTO_SIT_DESK) || ((pGameInfo->dwRoomRule & GRR_QUEUE_GAME) > 0)))
		if (pGameInfo->dwRoomRule & GRR_NOTCHEAT)
            strBuffer = "???";
        else
		    strBuffer.Format(TEXT("%d"),pUserInfo->bDeskNO+1);
	}
	else
		strBuffer = "";

	ite = m_MapColumn.find("DeskNo"); ///< ����
	if (m_MapColumn.end() != ite)
	{
		strncpy(ite->second.Value,strBuffer,32);
	}
		
	//CGameRoomEx* pGameRoomEx = GetGameRoom();

	CGameRoomEx* pGameRoomEx = GetGameRoom(pGameInfo->uRoomID);
	if (!pGameRoomEx)
	{
		return;
	}

	//���
	TCHAR szNum[128];
	GetNumString(szNum, i64Money, Glb().m_nPowerOfGold,Glb().m_bUseSpace, Glb().m_strSpaceChar);
	strBuffer.Format(TEXT("%s"),szNum);
    //if((NULL != pGameInfo) && ((pGameInfo->dwRoomRule & GRR_AUTO_SIT_DESK) || ((pGameInfo->dwRoomRule & GRR_QUEUE_GAME) > 0)) && (pGameRoomEx->m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID != pUserInfo->dwUserID))
	
	if (((pGameInfo->dwRoomRule & GRR_NOTCHEAT) && pGameRoomEx->m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID != pUserInfo->dwUserID) || pGameInfo->dwRoomRule & GRR_CONTEST)
	{
        strBuffer = "???";
	}
	ite = m_MapColumn.find("Coin"); ///< ���
	if (m_MapColumn.end() != ite)
	{
		strncpy(ite->second.Value,strBuffer,32);
	}

	//�Ƹ�����
	strBuffer = GetMoneyOrderName(i64Money,pGameInfo->uNameID);
	ite = m_MapColumn.find("WealthLevel"); ///< �Ƹ�����
	if (m_MapColumn.end() != ite)
	{
		strncpy(ite->second.Value,strBuffer,32);
	}

	///< ����
	strBuffer.Format(TEXT("%d"),dwPoint-GetNormalOrderBasePoint(dwPoint));
	if (((pGameInfo->dwRoomRule & GRR_NOTCHEAT) && pGameRoomEx->m_GameInfo.pMeUserInfo->GameUserInfo.dwUserID != pUserInfo->dwUserID) || pGameInfo->dwRoomRule & GRR_CONTEST)
	{
        strBuffer = "???";
	}
	ite = m_MapColumn.find("Score"); ///< ����
	if (m_MapColumn.end() != ite)
	{
		strncpy(ite->second.Value,strBuffer,32);
	}

	///< ���ּ���
	strBuffer = GetPointOrderName(dwPoint,pGameInfo->uNameID);
	ite = m_MapColumn.find("ScoreLevel"); ///< ���ּ���
	if (m_MapColumn.end() != ite)
	{
		strncpy(ite->second.Value,strBuffer,32);
	}

	///< ����ֵ
	strBuffer.Format(TEXT("%d"),dwFascination);
	ite = m_MapColumn.find("Charm"); ///< ����ֵ
	if (m_MapColumn.end() != ite)
	{
		strncpy(ite->second.Value,strBuffer,32);
	}

	///< �����ȼ�
	strBuffer.Format(TEXT("%s"),GetCharmGrade(dwFascination));
	ite = m_MapColumn.find("CharmLevel"); ///< �����ȼ�
	if (m_MapColumn.end() != ite)
	{
		strncpy(ite->second.Value,strBuffer,32);
	}

	///< �Ա�
	strBuffer = pUserItem->GameUserInfo.bBoy?fMsg.GetKeyVal("UserListCtrl","Male","��"):fMsg.GetKeyVal("UserListCtrl","Female","Ů");
	ite = m_MapColumn.find("Sex"); ///< �Ա�
	if (m_MapColumn.end() != ite)
	{
		strncpy(ite->second.Value,strBuffer,32);
	}

	//�ܾ���
	strBuffer.Format(TEXT("%ld"),uAllCount);
	ite = m_MapColumn.find("GameCount"); ///< �ܾ���
	if (m_MapColumn.end() != ite)
	{
		strncpy(ite->second.Value,strBuffer,32);
	}

	///< ʤ��
	strBuffer.Format(TEXT("%ld"),pUserInfo->uWinCount);
	ite = m_MapColumn.find("WinGame"); ///< ʤ��
	if (m_MapColumn.end() != ite)
	{
		strncpy(ite->second.Value,strBuffer,32);
	}

	///< ���
	strBuffer.Format(TEXT("%d"),pUserInfo->uLostCount);
	ite = m_MapColumn.find("LoseGame"); ///< ���
	if (m_MapColumn.end() != ite)
	{
		strncpy(ite->second.Value,strBuffer,32);
	}

	///< �;�
	strBuffer.Format(TEXT("%d"),pUserInfo->uMidCount);
	ite = m_MapColumn.find("Peace"); ///< �;�
	if (m_MapColumn.end() != ite)
	{
		strncpy(ite->second.Value,strBuffer,32);
	}

	///< ������
	if (uCutPercent>0) 
		strBuffer.Format(TEXT("%2ld%%"),uCutPercent);
	else
		strBuffer.Empty();
	
	ite = m_MapColumn.find("Escape"); ///< ������
	if (m_MapColumn.end() != ite)
	{
		strncpy(ite->second.Value,strBuffer,32);
	}
}

//ɾ���û�
bool CUserListCtrl::DeleteGameUser(UserItemStruct * pUserItem)
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
//����
void CUserListCtrl::ChangeSkin()
{
	//������Ϣ
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	CString key=TEXT("UserList");
	CString skinfolder = f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath());;
	int r = 0,g = 0,b = 0;
	r=f.GetKeyVal(key,"rlisttextclr",255);
	g=f.GetKeyVal(key,"rlisttextclg",255);
	b=f.GetKeyVal(key,"rlisttextclb",255);
	COLORREF textcr=RGB(r,g,b);

	r=f.GetKeyVal(key,"rlistbkclr",233);
	g=f.GetKeyVal(key,"rlistbkclg",233);
	b=f.GetKeyVal(key,"rlistbkclb",233);
	COLORREF bkcr=RGB(r,g,b);

	r=f.GetKeyVal(key,"rflisttextclr",255);
	g=f.GetKeyVal(key,"rflisttextclg",255);
	b=f.GetKeyVal(key,"rflisttextclb",255);
	COLORREF ftextcr=RGB(r,g,b);

	r=f.GetKeyVal(key,"rflistbkclr",233);
	g=f.GetKeyVal(key,"rflistbkclg",233);
	b=f.GetKeyVal(key,"rflistbkclb",233);
	COLORREF fbkcr=RGB(r,g,b);
	SetColor(bkcr,textcr,fbkcr,ftextcr);

	//�б�ͷ
	r=f.GetKeyVal(key,"rlistheadr",0);
	g=f.GetKeyVal(key,"rlistheadg",59);
	b=f.GetKeyVal(key,"rlistheadb",108);
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
		 //{{Modified by Zxd 20100715 ������������Ϊ��Ա���������ͷ�
		if (m_NewUserStateBmp.m_hObject != NULL)
		{
			m_NewUserStateBmp.DeleteObject();
		}
		/// Modified by Zxd 20100715 ������������Ϊ��Ա���������ͷ�}}
		m_NewUserStateBmp.CreateCompatibleBitmap(pDC,BmpInfo.bmWidth,BmpInfo.bmHeight);
		CBitmap * pOldBmp=TargetDC.SelectObject(&m_NewUserStateBmp);
		TargetDC.FillSolidRect(0,0,BmpInfo.bmWidth,BmpInfo.bmHeight,m_bkColor);
		AFCStretchImage(&TargetDC,0,0,BmpInfo.bmWidth,BmpInfo.bmHeight,UserStateBmp,0,0,BmpInfo.bmWidth,BmpInfo.bmHeight,RGB(255,255,255));
		TargetDC.SelectObject(pOldBmp);
		TargetDC.DeleteDC();

		//������Դ��

		//if(m_UserStateList.m_hImageList)
			//m_UserStateList.Detach();
		//m_UserStateList.Create(USER_STATE_WIDTH,USER_STATE_HEIGHT,ILC_COLOR16,0,0);

		//m_UserStateList.Add(&m_NewUserStateBmp,RGB(0,0,255));

		//������Դ
		UserStateBmp.Detach();
		ReleaseDC(pDC);
	}
	//SetImageList(&m_UserStateList,LVSIL_SMALL);

	//InitUserTypeImage();

	//InitUserDiamondTypeImage();//������ʯ��ݱ�ʶ add by huangYuanSong 09.07.14
}
void CUserListCtrl::LoadSkin(void)
{
	//������Ϣ
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	CString key=TEXT("UserList");
	CString skinfolder = f.GetKeyVal(m_skinmgr.GetKeyVal(strSkin),"skinfolder",m_skinmgr.GetSkinPath());;

	int r,g,b;
	r=f.GetKeyVal(key,"rlisttextclr",255);
	g=f.GetKeyVal(key,"rlisttextclg",255);
	b=f.GetKeyVal(key,"rlisttextclb",255);
	COLORREF textcr=RGB(r,g,b);

	r=f.GetKeyVal(key,"rlistbkclr",233);
	g=f.GetKeyVal(key,"rlistbkclg",233);
	b=f.GetKeyVal(key,"rlistbkclb",233);
	COLORREF bkcr=RGB(r,g,b);

	r=f.GetKeyVal(key,"rflisttextclr",255);
	g=f.GetKeyVal(key,"rflisttextclg",255);
	b=f.GetKeyVal(key,"rflisttextclb",255);
	COLORREF ftextcr=RGB(r,g,b);

	r=f.GetKeyVal(key,"rflistbkclr",233);
	g=f.GetKeyVal(key,"rflistbkclg",233);
	b=f.GetKeyVal(key,"rflistbkclb",233);
	COLORREF fbkcr=RGB(r,g,b);
	SetColor(bkcr,textcr,fbkcr,ftextcr);

	//�б�ͷ
	r=f.GetKeyVal(key,"rlistheadr",0);
	g=f.GetKeyVal(key,"rlistheadg",59);
	b=f.GetKeyVal(key,"rlistheadb",108);
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
		/// {{Modified by Zxd 20100715 ������������Ϊ��Ա���������ͷ�
		if (m_NewUserStateBmp.m_hObject != NULL)
		{
			m_NewUserStateBmp.DeleteObject();
		}
		/// Modified by Zxd 20100715 ������������Ϊ��Ա���������ͷ�}}
		m_NewUserStateBmp.CreateCompatibleBitmap(pDC,BmpInfo.bmWidth,BmpInfo.bmHeight);
		CBitmap * pOldBmp=TargetDC.SelectObject(&m_NewUserStateBmp);
		TargetDC.FillSolidRect(0,0,BmpInfo.bmWidth,BmpInfo.bmHeight,m_bkColor);
		AFCStretchImage(&TargetDC,0,0,BmpInfo.bmWidth,BmpInfo.bmHeight,UserStateBmp,0,0,BmpInfo.bmWidth,BmpInfo.bmHeight,RGB(255,255,255));
		TargetDC.SelectObject(pOldBmp);
		TargetDC.DeleteDC();

		//������Դ��

		if(m_UserStateList.m_hImageList)
			m_UserStateList.Detach();
		m_UserStateList.Create(USER_STATE_WIDTH,USER_STATE_HEIGHT,ILC_COLOR16,0,0);
		
		m_UserStateList.Add(&m_NewUserStateBmp,RGB(0,0,255));

		//������Դ
		UserStateBmp.Detach();
		ReleaseDC(pDC);
	}
	SetImageList(&m_UserStateList,LVSIL_SMALL);

	InitUserTypeImage();

	InitUserDiamondTypeImage();//������ʯ��ݱ�ʶ add by huangYuanSong 09.07.14
}

/// ����������
///	@param uVirtualUserCount�����������
///	@return void
void CUserListCtrl::fillVirtualUser(int uVirtualUserCount)
{
	//return;///< ������������

	//wushuqun 2009.6.6
	//ʵʱ�����������
	m_uVirtualUserCount += uVirtualUserCount;

	if(uVirtualUserCount < 1)
		return;

	DWORD dwName = Glb().m_hRandVirUser;
	int uCount = 0;
	if(dwName)
		uCount = cfgGetValue(dwName,"USERNAME","COUNT",0);

	CBcfFile fMsg(CBcfFile::GetAppPath()+"ClientMessage.bcf");
	CString strMessage;

	CString str[7]=
	{  "����","��ݸ","����","��ɽ","��ͷ","�麣","ï��" };
	//str[0] = fMsg.GetKeyVal("UserListCtrl","City1","����");
	//str[1] = fMsg.GetKeyVal("UserListCtrl","City2","��ݸ");
	//str[2] = fMsg.GetKeyVal("UserListCtrl","City3","����");
	//str[3] = fMsg.GetKeyVal("UserListCtrl","City4","��ɽ");
	//str[4] = fMsg.GetKeyVal("UserListCtrl","City5","��ͷ");
	//str[5] = fMsg.GetKeyVal("UserListCtrl","City6","�麣");
	//str[6] = fMsg.GetKeyVal("UserListCtrl","City7","ï��");

	int nCityCnt = fMsg.GetKeyVal("UserListCtrl","CityCount",0);
	
	Sleep(20);///< ��֤������Ӳ�һ�� wushuqun 2009.6.15
	srand((unsigned)GetTickCount());	//�����������

	for(int c=0; c<uVirtualUserCount; c++)
	{
		UserItemStruct * pUserItem = new UserItemStruct;
		ZeroMemory(pUserItem,sizeof(UserItemStruct));

		//�������
		CString strBuffer;
		UserInfoStruct * pUserInfo = &pUserItem->GameUserInfo;

		if (0 == nCityCnt)
		{
			strMessage = fMsg.GetKeyVal("UserListCtrl","Province","�㶫ʡ");
			wsprintf(pUserInfo->szProvince,strMessage.GetBuffer());
			int nCityIndex = rand() % 7;
			wsprintf(pUserInfo->szCity,str[nCityIndex]);
		}
		else
		{
			int nCityIndex = rand()%nCityCnt;
			strMessage.Format(_T("City%d"),nCityIndex);
			strMessage = fMsg.GetKeyVal("UserListCtrl",strMessage,_T("�㶫ʡ,����"));
			wsprintf(pUserInfo->szProvince,strMessage.Left(strMessage.Find(',')));
			wsprintf(pUserInfo->szCity,strMessage.Right(strMessage.GetLength()-strMessage.Find(',')-1));
			
		}

		pUserInfo->uWinCount=rand()/(32767/300);
		pUserInfo->uLostCount=rand()/(32767/200);
		pUserInfo->uCutCount=rand()/(32767/30);
		pUserInfo->uMidCount=rand()/(32767/100);
		if (NULL != msgWnd)
			pUserInfo->i64Money = rand()/2+((CGameRoomEx*)msgWnd)->m_GameInfo.uLessPoint;
		else
			pUserInfo->i64Money = rand()/2+1000;

		pUserInfo->bUserState = USER_NO_STATE;       ///< ���״̬

		if(uCount>0)
		{
			CString s;
			s.Format("NAME%d",rand()%uCount);
			wsprintf(pUserInfo->nickName,"%s",cfgGetValue(dwName,"USERNAME",s,"���"));
		}
		else
			wsprintf(pUserInfo->nickName,"Player%d",c+10000);

		pUserInfo->dwUserID			 = -1000-c -1; ///< ���ID
		pUserInfo->isVirtual = 1;
		pUserInfo->dwPoint = rand()-16383;	//�����������
		pUserInfo->dwFascination=rand()/(32767/25);	//�����������
		pUserInfo->bDeskNO = 0xFF;  	//���ó�ʼ����
		pUserItem->GameUserInfo.bBoy = rand()%2;  ///< �Ա�
		pUserInfo->bLogoID			 = rand()%3; ///< ͷ��ID

		if(pUserItem->GameUserInfo.bBoy)
		{
			if(pUserInfo->bLogoID > 5)
				pUserInfo->bLogoID -= 6;
		}
		else
		{
			if(pUserInfo->bLogoID < 6)
				pUserInfo->bLogoID += 6;
		}

		AddGameUser(pUserItem);

		if(msgWnd)
			SetTimer(c+1000,rand()*(270000/32767),NULL);
	}

	BYTE iSend=0,iStation=0,iTemp=0;
	srand((unsigned)time(NULL));

	do
	{
		iStation=rand() % uVirtualUserCount;
		iTemp=VirtualList[iSend];
		VirtualList[iSend]=VirtualList[iStation];
		VirtualList[iStation]=iTemp;
		iSend++;
	}
	while (iSend<uVirtualUserCount);

}


//////////////////////////////////////////////////////////////////////////
//���� fillVirtualUser ���������������
void CUserListCtrl::OnTimer(UINT nIDEvent)
{
	if(nIDEvent>=1000 && msgWnd)
	{
		KillTimer(nIDEvent);
	//	DWORD_PTR dw=GetItemData(nIDEvent-1000+1);
		DWORD_PTR dw = (DWORD_PTR)GetIdelVirtualUser();
		if(dw)
		{
		//	UserItemStruct * pUserItem=(UserItemStruct * )dw;
			msgWnd->PostMessage(WM_USER+5,10,(LPARAM)dw);
			SetTimer(nIDEvent,rand()*(270000/32767),NULL);
		}

	}
	__super::OnTimer(nIDEvent);

}

//////////////////////////////////////////////////////////////////////////
////20081211 , Fred Huang
void CUserListCtrl::SetMSGReceiveWnd(CWnd * pWnd)
{
	msgWnd=pWnd;
}

///
/// ���ҿ��е��������
///
///@param void
///
///@return UserItemStruct ���������Ϣ
///
UserItemStruct * CUserListCtrl::GetIdelVirtualUser(void)
{
	int iCnt;
	UserItemStruct * pUserItem = NULL;
	iCnt = GetItemCount();

	for(int i=0; i<((iCnt+1)>>1); i++)
	{
//		UserItemStruct * pUserItem=(UserItemStruct * )GetItemData(VirtualList[i]);
		pUserItem = (UserItemStruct * )GetItemData(i);
		if(pUserItem &&
			pUserItem->GameUserInfo.dwUserID<0 &&
			pUserItem->GameUserInfo.bUserState!=USER_PLAY_GAME &&
			1==pUserItem->GameUserInfo.isVirtual)
			break;
		else
			pUserItem = NULL;

		pUserItem = (UserItemStruct * )GetItemData(iCnt-i-1);
		if(pUserItem &&
			pUserItem->GameUserInfo.dwUserID<0 &&
			pUserItem->GameUserInfo.bUserState!=USER_PLAY_GAME &&
			1==pUserItem->GameUserInfo.isVirtual)
			break;
		else
			pUserItem = NULL;
	}

	return pUserItem;
}

///
/// ����ָ���������
///
///@param long dwUserID ��Ҫ���ҵ��û�ID
///
///@return UserItemStruct �����Ϣ
///
UserItemStruct * CUserListCtrl::GetUserFromID(long dwUserID)
{
	int iCnt;
	UserItemStruct * pUserItem = NULL;
	iCnt = GetItemCount();

	for(int i=0; i<((iCnt+1)>>1); i++)
	{
		//		UserItemStruct * pUserItem=(UserItemStruct * )GetItemData(VirtualList[i]);
		pUserItem = (UserItemStruct * )GetItemData(i);
		if(pUserItem && pUserItem->GameUserInfo.dwUserID == dwUserID)
			break;
		else
			pUserItem = NULL;

		pUserItem = (UserItemStruct * )GetItemData(iCnt-i-1);
		if(pUserItem && pUserItem->GameUserInfo.dwUserID == dwUserID)
			break;
		else
			pUserItem = NULL;
	}

	return pUserItem;
}

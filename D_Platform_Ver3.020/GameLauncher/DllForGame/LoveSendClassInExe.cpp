// LoveSendClassInExe.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LoveSendClassInExe.h"
#include "Platform/bzsound.h"
#include "../common/gameroommessage.h"
#include "../common/gameplacemessage.h"
#include "Resource.h"
#include "Platform/confirmdlg.h"
#include "IMSetUser.h"
#include "IMMessage.h"

#include "detours.h"
#include "skinsb.h"

#include "PBank4GameWnd.h"		///< �ڶ�����Ϸ����
#include "RightFrame.h"			///��Ϸ�ұ߿�
#include "GameTrottingWnd.h"
#include "UseBoardCastWnd.h"    ///���ȵ��ߴ���

#include "BzCrashRpt.h"

#include "NewGameFrame/XLogic.h"

#define  _USE_PROP_PAGE_SW_

//�ؼ� ID
#define ID_GAME_VIEW						10							//��Ϸ��ͼ ID

//�û��˵���Ϣ����
#define IDM_COPY_USER_NAME					WM_USER+101					//�����û���
#define IDM_SET_TALK_OBJECT					WM_USER+102					//��Ϊ̸������ 
#define IDM_CANCEL_TALK						WM_USER+103					//ȡ��������� 
#define IDM_EMABLE_WATCH					WM_USER+104					//�����Թ� 
#define IDM_UNEMABLE_WATCH					WM_USER+105					//��ֹ�Թ�
#define IDM_SHIELD_USER						WM_USER+106					//�����û�
#define IDM_UNSHIELD_USER					WM_USER+107					//�����û�
#define IDM_KICK_USER						WM_USER + 108				//�����뿪(����)
#define IDM_SET_FRIEND						WM_USER+227                 //��Ϊ����
#define IDM_SET_EMENY                       WM_USER+228                 //���������
//#define IDM_GIVE_PROP						WM_USER+109

// PengJiLin, 2010-9-10, ���˿��Ĳ˵���Ϣ
#define IDM_KICK_USER_PROP                  WM_USER+110

// duanxiaohui 2011-11-11 �û���ϸ��Ϣ
#define IDM_USER_INFO                       WM_USER + 111

/// ����ȷ����ȡ���ļ��ĺ꣬�������ϷĿ¼�¶�ȡ���������ڴ���Ŀ¼�¶�ȡ�ļ�
/// ����ǰ������׼����skinHall����Ϊ�ļ���ʱ��skin0\�£���ʱ��skin0\game\��

#ifndef BZ_ZLIB
#ifndef GET_FILENAME
	#define GET_FILENAME(path, FormatStr,skinfolder) \
		wsprintf(path,FormatStr,skinfolder);\
		if (!CBcfFile::IsFileExist(path))\
		{\
		wsprintf(path,FormatStr,skinHall);\
		}
	#endif// GET_FILENAME
#else
	#define GET_FILENAME(path, FormatStr,skinfolder) \
		wsprintf(path,FormatStr,skinfolder);
#endif//GET_FILENAME


// ��Ϊ��̬��Ա������ʹ�����ڽ��̽���ʱ��ʽ����CBcfFile::ClearMap()�������ͷţ�����MFC�ᱨ�ڴ�й©
EXT_CLASS std::map<CString, CBcfFileNameItem*> CBcfFile::mapFile;
EXT_CLASS std::map<CString, FileItem*> CUnZipRes::mapFile;
unsigned char CUnZipRes::m_bPassWord[MAX_PASSWORD_LENGTH];

extern UINT AFCMessageBox(CString strInfo,CString strTitel,UINT style);
static CGlobal g_global;
CGlobal& Glb(){return g_global;}

//��̬����
CArray<LONG,LONG>				CLoveSendClassInExe::m_ShieldArray;			//�����û�

//�ȼ��жϺ���
TCHAR * GetGameOrderNameInExe(__int64 dwPoint)
{
	static __int64 uBasePoint[15]=
	{
		-300,0,100,200,
		400,700,1100,1600,
		2200,2900,3700,4500,
		5500,6700,8000
	};
	static TCHAR * szOrderName[15]=
	{
		TEXT("����"),TEXT("���"),TEXT("����"),TEXT("��Ʒ֥���"),
		TEXT("��Ʒ��ة"),TEXT("��Ʒ֪��"),TEXT("��Ʒͨ��"),TEXT("��Ʒ֪��"),
		TEXT("��Ʒ֪��"),TEXT("��Ʒ����ʹ"),TEXT("��ƷѲ��"),TEXT("һƷ��ѧʿ"),
		TEXT("����"),TEXT("��ү"),TEXT("�ʵ�")
	};
	for (int i=0;i<15;i++)	if (dwPoint<uBasePoint[i]) return szOrderName[i];
	return TEXT("�ʵ�");
}
/// ��ȡָ����ֵ���ַ���
/// ������ͬʱ֧��UNICODE��ANSI
/// @param[out] str �ַ������飬���ɵ����߷���ռ�
/// @param[in] nNum ����ʾ��������ֵ
/// @param[in] nPower ��10�ļ�������ʾ����123��ʾ��123000����˲���Ϊ3
/// @param[in] bHasSpace �ַ����Ƿ�ֽ���ʾ��Ĭ��ֵfalse��ʾ���ֲ��ֽڣ�true��ʾ�ÿո�ÿ��λ��һ��
/// @return ����str���ڵ�λ��
///
// PengJiLin, 2010-8-3, ���ӿ�������ָ���
// PengJiLin, 2011-7-8, 64λ�����ʾ
TCHAR *GlbGetNumString(TCHAR str[], __int64 nNum, int nPower, bool bHasSpace, TCHAR* chSpace)
{
	/// �ݴ���
	TCHAR strTmp[32];
	/// �����0����ֱ�ӷ���һ��0 
	if (0==nNum)
	{
		_tcscpy(str, TEXT("0"));
		return str;
	}

	wsprintf( strTmp, TEXT("%I64d"), nNum);

	//Add By YangJie 2011-02-21
	if (nPower >= 0) {
		for (int i=0; i<nPower; ++i)
		{
			_tcscat( strTmp, TEXT("0"));
		}
	} else {
		bool bFlag = false;
		if (nNum < 0) {
			::memset(strTmp, 0, sizeof(strTmp));
			wsprintf(strTmp, TEXT("%I64d"), -nNum);
			bFlag = true;
		}

		int iLen = _tcslen(strTmp);
		int tmpPower = -nPower;
		if (iLen > tmpPower) {
			for (int i = iLen; i > (iLen - tmpPower); i --) {
				strTmp[i] = strTmp[i-1];
			}
			strTmp[iLen-tmpPower] = TEXT('.');
			strTmp[iLen+1] = TEXT('\0');
		} else {
			TCHAR strTmp1[32];
			int i = 0;
			int j = 0;
			int k = 0;
			::memset(strTmp1, 0, sizeof(strTmp1));
			for (i = 0; i < (tmpPower - iLen + 1); i ++) {
				strTmp1[i] = TEXT('0');
			}
			for (j = i, k = 0; k < iLen;j ++, k ++) {
				strTmp1[j] = strTmp[k];
			}
			strTmp1[j++] = TEXT('\0');
			::memset(strTmp, 0, sizeof(strTmp));
			_tcscpy(strTmp, strTmp1);
			for (i = j; i > 1; i --) {
				strTmp[i] = strTmp[i-1];
			}
			strTmp[1] = TEXT('.');
			strTmp[j+1] = TEXT('\0');
		}

		if (bFlag) {
			iLen = _tcslen(strTmp);
			for (int i = iLen; i > 0; i --) {
				strTmp[i] = strTmp[i-1];
			}
			strTmp[0] = TEXT('-');
			strTmp[iLen+1] = TEXT('\0');
		}
	}


	// ���û�пո���ֱ�ӷ��������ַ���
	if (!bHasSpace)
	{
		_tcscpy(str, strTmp);
		return str;
	}

	if (nPower >= 0) {

		// PengJiLin, 2010-9-1, �������ö���λ��Ϊһ��, ��󲻳���8λ   
		CString s = CBcfFile::GetAppPath();
		CBcfFile f( s + "Order.bcf");
		BYTE byBits = f.GetKeyVal("VirtualMoney", "SpaceBits", 3);

		if(byBits < 1) byBits = 1;
		if(byBits > 8) byBits = 8;

		/// ��λ��ÿ��λ��һ����ʾ
		int nLen = _tcslen(strTmp);	// �ַ�������
		int nTimes = nLen % byBits/*3*/;		// ��λһ��ʱ�������λ��

		int nSrc = 0;
		int nDes = 0;

		// PengJiLin, 2010-8-3, ���ӿ�������ָ���
		TCHAR chUseSpace = 0;
		if(NULL == chSpace)
		{
			chUseSpace = ' ';
		}
		else
		{
			chUseSpace = chSpace[0];
		}

		for (int i=0; i<nLen; ++i)
		{
			/// ����ʱ����������ֵ�䲻�ӿո�
			if ((i>0) && (i%byBits==nTimes) && (!(i==1 && nNum<0)))
			{
				str[nDes++] = chUseSpace;
			}
			str[nDes++] = strTmp[nSrc++];
		}
		str[nDes] = TEXT('\0');
	} else {

		_tcscpy(str, strTmp);
	}

	return str;
}
void DebugPrintf(const char *p, ...)
{
	char szFilename[256];
	sprintf( szFilename, "E:\\bzw\\DebugInfo\\%s.txt", Glb().m_szProcessName);
	FILE *fp = fopen( szFilename, "a" );
	if (NULL == fp)
	{
		return;
	}
	va_list arg;
	va_start( arg, p );
	fprintf(fp,"[%s]--", CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));
	vfprintf(fp, p, arg );
	fprintf(fp,"\n");
	fclose(fp);
}

void DebugPrintH(const void *buf, int nSize)
{
	FILE *fp = fopen( "E:\\bzw\\DebugInfo\\Hex.txt", "a" );
	if (NULL == fp)
	{
		return;
	}
	BYTE *p = (BYTE *)buf;
	for (int i=0; i<nSize; ++i)
	{
		if (i%16 == 0)
		{
			fprintf(fp, "\n");
		}
		fprintf(fp, "%02X ", *p++);
	}
	fprintf(fp, "\n");
	fclose(fp);
}

// CLoveSendClassInExe �Ի���

IMPLEMENT_DYNAMIC(CLoveSendClassInExe, CLoadFileBmp)

BEGIN_MESSAGE_MAP(CLoveSendClassInExe, CLoadFileBmp)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_MESSAGE(WM_HIT_EXPMESSTION,OnExpression)
	ON_MESSAGE(WM_RICH_EDIT_HIT,OnRichEditHit)
	ON_MESSAGE(WM_CLOSE_GAME,OnCloseGame)
	ON_MESSAGE(WM_SPLIT_WND,OnSplitMessage)
	ON_MESSAGE(WM_RELOAD_TALKINI,OnReLoadTalkINI)
	ON_MESSAGE(WM_USE_PROP,OnPannelUseProp)		// �ӵ�������д���ʹ�õ�����Ϣ
	ON_MESSAGE(WM_PROP_BUY,OnPannelBuyProp)		// �ӵ�������д��ع��������Ϣ
	ON_MESSAGE(WM_GIVE_PROP,OnPannelGiveProp)	// �ӵ�������д������͵�����Ϣ
	ON_MESSAGE(WM_BUY_VIP,OnPannelBuyVIP)		// �ӵ�������д��ع���VIP��Ϣ     add by wyl   11-5-21
	ON_MESSAGE(WM_USE_BOARDCAST,OnUseBoardcast)
	ON_MESSAGE(WM_BANK_CHECK_OUT,OnBankCheckOut)	// �����������ȡǮ
	ON_MESSAGE(WM_GETFRIENDLIST,OnSendGetFriendList)		// �ӵ�������д��ع���VIP��Ϣ     add by wyl   11-5-21
	ON_MESSAGE(WM_CHECK_MONEY,OnGameBankMoneyCheck) //��Ϸ���д�ȡǮ jianguankun 2012.9.20
	ON_MESSAGE(WM_CHECK_MONEY2,OnGameBankMoneyCheck2) //��Ϸ���д�ȡǮ jianguankun 2012.9.20
	ON_MESSAGE(WM_BANK2_CLOSE,OnGameBankClose) //�ر���Ϸ���� jianguankun 2012.9.20
	//	ON_MESSAGE(IDM_GETPOINT,OnGetPoint)

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_EXIT, OnBnClickedExit)
	ON_BN_CLICKED(IDC_REST, OnBnClickedRest)
	ON_BN_CLICKED(IDC_QUIT_GAME, &CLoveSendClassInExe::OnBnClickedQuitGame)
	ON_BN_CLICKED(IDC_MAX, &CLoveSendClassInExe::OnBnClickedMax)
	ON_BN_CLICKED(IDC_HIDE, &CLoveSendClassInExe::OnBnClickedHide)
	ON_BN_CLICKED(IDC_MIN, &CLoveSendClassInExe::OnBnClickedMin)
	ON_WM_MOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_GAME_BTN_01, &CLoveSendClassInExe::OnBnClickedGameBtn01)
	ON_BN_CLICKED(IDC_GAME_BTN_02, &CLoveSendClassInExe::OnBnClickedGameBtn02)
	ON_BN_CLICKED(IDC_GAME_BTN_03, &CLoveSendClassInExe::OnBnClickedGameBtn03)
	ON_BN_CLICKED(IDC_GAME_BTN_04, &CLoveSendClassInExe::OnBnClickedGameBtn04)

	ON_BN_CLICKED(IDC_BUTTON_PROP1, &CLoveSendClassInExe::OnBnClickedButtonProp1)
	ON_BN_CLICKED(IDC_BUTTON_PROP2, &CLoveSendClassInExe::OnBnClickedButtonProp2)
	ON_BN_CLICKED(IDC_BUTTON_PROP3, &CLoveSendClassInExe::OnBnClickedButtonProp3)
	ON_BN_CLICKED(IDC_BUTTON_PROP4, &CLoveSendClassInExe::OnBnClickedButtonProp4)
	ON_BN_CLICKED(IDC_BUTTON_PROP5, &CLoveSendClassInExe::OnBnClickedButtonProp5)
	ON_BN_CLICKED(IDC_BUTTON_PROP6, &CLoveSendClassInExe::OnBnClickedButtonProp6)  
	ON_BN_CLICKED(IDC_BUTTON_PROP7, &CLoveSendClassInExe::OnBnClickedButtonProp7)
	ON_BN_CLICKED(IDC_BUTTON_PROP8, &CLoveSendClassInExe::OnBnClickedButtonProp8)
	ON_BN_CLICKED(IDC_BUTTON_PROP9, &CLoveSendClassInExe::OnBnClickedButtonProp9)
	ON_BN_CLICKED(IDC_BUTTON_PROP10, &CLoveSendClassInExe::OnBnClickedButtonProp10)

	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify)

	//ON_BN_CLICKED(IDC_SNDPLAY,OnSndPlay)
	//ON_BN_CLICKED(IDC_SNDPAUSE,OnSndPause)
	//ON_BN_CLICKED(IDC_SNDVOLDOWN,OnSndVolDown)
	//ON_BN_CLICKED(IDC_SNDVOLUP,OnSndVolUp)

	ON_CBN_SELCHANGE(IDC_INPUT, &CLoveSendClassInExe::OnCbnSelchangeInput)
	ON_BN_CLICKED(IDC_SEND, &CLoveSendClassInExe::OnBnClickedSend)
	ON_BN_CLICKED(IDC_SETCOLOR, &CLoveSendClassInExe::OnBnClickedSetcolor)
	ON_BN_CLICKED(IDC_EXPRESSION, &CLoveSendClassInExe::OnBnClickedExpression)
	ON_BN_CLICKED(IDC_GO_AHEAD, &CLoveSendClassInExe::OnBnClickedGoAhead)
	ON_BN_CLICKED(IDC_GO_NEXT, &CLoveSendClassInExe::OnBnClickedGoNext)
    ON_BN_CLICKED(IDC_GAME_BTN_05, OnBnClickedBlueDiamond) // PengJiLin, 2010-9-30, �������
	ON_WM_CLOSE()
	ON_WM_NCLBUTTONDBLCLK()

    // PengJiLin, 2010-10-12, �̳����߹���
    ON_BN_CLICKED(IDC_GAME_BTN_06,OnBnClickedGoAheadS)
    ON_BN_CLICKED(IDC_GAME_BTN_07,OnBnClickedGoNextS)

    ON_WM_ACTIVATE()                // PengJiLin, 2011-6-23, ������Ϣ

END_MESSAGE_MAP()

__int64                  g_PointArray[20];                  ///�ȼ���������
map<int,string>          g_LevelNameArray;                  ///�ȼ���������

CLoveSendClassInExe::CLoveSendClassInExe(CUsingSkinClass * pGameView)
: CLoadFileBmp(IDD_EXE_DIALOG), m_pGameView(pGameView), m_MessageHandle(&m_TalkMessage)
{
	m_bInit = false;
	m_CurpUserInfoClicked=NULL;
	m_pGameRoom=NULL;
	m_pGameInfo=NULL;
	m_dlgBroadcast = NULL;
	m_uTimeID=0;
	m_uTimeCount=0;
	m_bTimeStation=255;
	m_bSaveView=false;
	m_bWatchMode=false;
	m_bWatchOther=false;
	m_dwLastTalkTime=0L;
	m_dwCurrentUserID=0L;
	memset(m_pUserInfo,0,sizeof(m_pUserInfo));

	CString strBcfFilename = m_skinmgr.GetGameSkinBcfFileName();
	if (CBcfFile::IsFileExist(CBcfFile::GetAppPath() + "config\\" + strBcfFilename))
		m_iniFile= CBcfFile::GetAppPath() + "config\\" + strBcfFilename;
	else
		m_iniFile= CBcfFile::GetAppPath()+"..\\config\\" + strBcfFilename;


	m_BtPropArray[0] = IDC_BUTTON_PROP2;
	m_BtPropArray[1] = IDC_BUTTON_PROP3;
	m_BtPropArray[2] = IDC_BUTTON_PROP4;
	m_BtPropArray[3] = IDC_BUTTON_PROP5;
	m_BtPropArray[4] = IDC_BUTTON_PROP6;
	m_BtPropArray[5] = IDC_BUTTON_PROP7;
	m_BtPropArray[6] = IDC_BUTTON_PROP8;
	m_BtPropArray[7] = IDC_BUTTON_PROP9;
	m_BtPropArray[8] = IDC_BUTTON_PROP10;
	//m_BtPropArray[7] = IDC_BUTTON_PROP8;
	//m_BtPropArray[3] = IDC_BUTTON_PROP4;  // PengJiLin, 2010-10-12, ע�ͣ�ֻ��ʾ3��

	m_curPropPage=0;		//��ǰ����ҳ=0��ÿҳ4��
	m_pPropSelUser=NULL;//2008-04-18
	m_MessageHandle.SetColor(m_colorresource);

	szAdvUrl="";
	m_pPropPannel = NULL;

	m_sndListID=0;
	m_sndBtnCx=m_sndBtnCy=16;

	pSingnalimg=NULL;
	m_NetSignalLevel=0;
	m_SavedSingalIndex=0;
	m_pPersonBank = NULL;

	//�ڶ�����Ϸ���жԻ���
	m_pBank2Wnd = NULL;	

	m_pUserDetailsDlg = NULL;

	m_pRightFrame = NULL;

    // PengJiLin, 2010-9-9
    m_dlgKickProp = NULL;

    m_curPropPage_S = 0;

	//����ƿؼ�
	m_pPmdWnd = NULL;

    memset(&m_SendMoneyData, 0, sizeof(m_SendMoneyData));

	m_bIsQueueDisMiss = false;

	// ��ʼBzCrashRpt
// 	BzCrash_Initiation();
// 	BzCrash_DisableSetUnhandledExceptionFilter();
// 	BzCrash_SetProjectName("Dƽ̨����");
// 	BzCrash_SetEmailSender("bzbugrev@sina.com");
// 	BzCrash_SetEmailReceiver("bzbugrev@sina.com");
// 	BzCrash_SetSmtpServer("smtp.sina.com");
// 	BzCrash_SetSmtpUser("bzbugrev");
// 	BzCrash_SetSmtpPassword("1122334455");
// 	BzCrash_DeleteSended(false);

}

CLoveSendClassInExe::~CLoveSendClassInExe()
{
	if(m_pPersonBank != NULL)
	{
		delete m_pPersonBank;
		m_pPersonBank = NULL;
	}

	if (m_pUserDetailsDlg != NULL)
	{
		delete m_pUserDetailsDlg;
		m_pUserDetailsDlg = NULL;
	}

	delete m_pPropSelUser;
	delete m_dlgBroadcast;
	delete m_pPropPannel;
	m_dlgBroadcast = NULL;
	m_TalkMessage.DestroyWindow();
	UserItemStruct * pFindUserItem=NULL;
	INT_PTR uCount=m_ThisDeskPtrArray.GetCount();
	for (INT_PTR i=0;i<uCount;++i)
	{
		pFindUserItem=(UserItemStruct *)m_ThisDeskPtrArray.GetAt(i);
		delete pFindUserItem;
		pFindUserItem = NULL;
	}
	m_ThisDeskPtrArray.RemoveAll();
}

////д��־ add by wlr 20090806
//void CLoveSendClassInExe::WriteInfo(TCHAR * pch)
//{
//	SYSTEMTIME st;
//	GetLocalTime(&st);
//	CString cStr;
//	cStr.Format(_T("%d-%d-%d %02d:%02d:%02d ��Ϸ���id=%d,%s"),
//		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute,st.wSecond,
//		GetMeUserInfo()->dwUserID, pch);
//	WriteStr(cStr.GetBuffer());
//	return;
//}
////end of д��־ add by wlr 20090806

void CLoveSendClassInExe::DoDataExchange(CDataExchange* pDX)
{
	CLoadFileBmp::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDC_HOR_SPLIT, m_HorSplitBar);

	DDX_Control(pDX, IDC_INPUT, m_InputMessage);
	DDX_Control(pDX, IDC_SEND, m_BtSend);
	DDX_Control(pDX, IDC_EXPRESSION, m_BtExpression);
	DDX_Control(pDX, IDC_TALK_MESSAGE, m_TalkMessage);
	DDX_Control(pDX, IDC_QUIT_GAME, m_BtGameQuit);
	DDX_Control(pDX, IDC_SETCOLOR, m_BtSetColor);

	DDX_Control(pDX, IDC_HIDE, m_BtHide);
	DDX_Control(pDX, IDC_EXIT, m_BtExit);
	DDX_Control(pDX, IDC_MIN, m_BtMin);
	DDX_Control(pDX, IDC_MAX, m_btMax);
	DDX_Control(pDX, IDC_REST, m_btRest);

	DDX_Control(pDX, IDC_GAME_BTN_01, m_Btbt1);
	DDX_Control(pDX, IDC_GAME_BTN_02, m_Btbt2);
	DDX_Control(pDX, IDC_GAME_BTN_03, m_Btbt3);
	DDX_Control(pDX, IDC_GAME_BTN_04, m_Btbt4);
	DDX_Control(pDX, IDC_GAME_BTN_05, m_Btbt5);
	DDX_Control(pDX, IDC_GAME_BTN_06, m_Btbt6);//��������һ����ǰ
	DDX_Control(pDX, IDC_GAME_BTN_07, m_Btbt7);//              ���
	DDX_Control(pDX, IDC_GAME_BTN_10, m_Btbt10);

	//{add by zxj 2008/04/07
	DDX_Control(pDX, IDC_GO_AHEAD, m_BtPropGoAhead);//��������һ����ǰ
	DDX_Control(pDX, IDC_GO_NEXT, m_BtPropGoNext);
	DDX_Control(pDX, IDC_BUTTON_PROP1, m_BtProp1);
	DDX_Control(pDX, IDC_BUTTON_PROP2, m_BtProp2);
	DDX_Control(pDX, IDC_BUTTON_PROP3, m_BtProp3);
	DDX_Control(pDX, IDC_BUTTON_PROP4, m_BtProp4);
	DDX_Control(pDX, IDC_BUTTON_PROP5, m_BtProp5);
	DDX_Control(pDX, IDC_BUTTON_PROP6, m_BtProp6);
	DDX_Control(pDX, IDC_BUTTON_PROP7, m_BtProp7);
	DDX_Control(pDX, IDC_BUTTON_PROP8, m_BtProp8);
	DDX_Control(pDX, IDC_BUTTON_PROP9, m_BtProp9);
	DDX_Control(pDX, IDC_BUTTON_PROP10, m_BtProp10);
	//end add}

	DDX_Control(pDX, IDC_EXPLORER1, m_ie);

	//DDX_Control(pDX, IDC_SNDPLAY,m_btSndPlay);
	//DDX_Control(pDX, IDC_SNDPAUSE,m_btSndPause);
	//DDX_Control(pDX, IDC_SNDVOLDOWN,m_btVolDown);
	//DDX_Control(pDX, IDC_SNDVOLUP,m_btVolUp);
}



//��������
bool CLoveSendClassInExe::CreateFrame()
{

	if (GetSafeHwnd()==NULL) 
	{
		/*if(m_DlgShowLeft.GetSafeHwnd()!=NULL)
		{
		AfxMessageBox("d");
		m_DlgShowLeft.ShowWindow(0);
		}*/
		/*CWnd * pWnd = GetDesktopWindow();*/
		AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
		
		//OutputDebugString("CLoveSendClassInExe::CreateFrame");
		Create(IDD_EXE_DIALOG, GetDesktopWindow());
		//OutputDebugString("CLoveSendClassInExe::CreateFrame_end");

		AfxSetResourceHandle(GetModuleHandle(NULL));
		//ShowWindow(SW_HIDE);
		//MoveWindow(0,0,900,650);
		//CenterWindow();	
		//OnBnClickedMax();
		//	ShowWindow(SW_SHOW);
	}	
	/*CRect r;
	GetWindowRect(&r);*/
	m_SavedScreen.x=1024;//r.Width();
	m_SavedScreen.y=736;//r.Height();

	CBcfFile f(CBcfFile::GetAppPath() + _T("GameOption.bcf"));
	UINT uMaxType = f.GetKeyVal("FrameControl","max_type",1);

	if (uMaxType == 1) //��ͨ���
	{
		m_bShowMax=true;
	}
	else if (uMaxType == 2) //�����øı���Ļ�ֱ��ʵķ�ʽ
	{
		m_bShowMax=false;
	}

	
	m_CurpUserInfoClicked=GetMeUserInfo();
	ResetGameStation();
	// ��������
	//m_PlaySound.PlayMusic(NULL,m_BackMusicIni,m_BMusicKey,m_pGameInfo->bEnableBackSound);
	if(this->m_hWnd)
	{
		//		OnBnClickedMax();
		ShowWindow(SW_SHOW);
		SetForegroundWindow();
		this->m_pGameRoom->ShowWindow(SW_HIDE);
	}
	if(!m_bSndPause)
		BZSoundContinue(m_sndListID);
	//	AfxGetApp()->m_pMainWnd->PostMessage(WM_USER+100,2,0);

	//
	//m_iPaintCount=0;
	//	SetTimer(IDT_SHOWWIN,500,NULL);

	// duanxiaohui 2011-11-14 ������ϸ���Ͽ�
	m_pUserDetailsDlg = new CUserDetailsDlg();
	m_pUserDetailsDlg->m_uGameNameID = m_pGameInfo->uNameID;
	m_pUserDetailsDlg->m_uComType = m_pGameInfo->uComType;
	AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
	m_pUserDetailsDlg->Create(IDD_USER_DETAILS, this);
	AfxSetResourceHandle(GetModuleHandle(NULL));
	// end duanxiaohui

	m_pPmdWnd = new CGameTrottingWnd(this);
	m_pPmdWnd->init(this->GetSafeHwnd());
	m_pPmdWnd->SetLabelTextFont("����", 16);
	m_pPmdWnd->ShowWindow(SW_HIDE);

	return true;
}
//ɾ������
void CLoveSendClassInExe::DeleteFrame()
{
	if (GetSafeHwnd() != NULL)
		OnCancel();
	delete this;
}
//��Ƶ���øı�
LRESULT CLoveSendClassInExe::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//switch(message)
	//{
	//case WM_DISPLAYCHANGE:
	//	//ShowWindow(SW_NORMAL);
	//	ShowMaxSize();
	//	ShowWindow(SW_MINIMIZE);
	//	return true;
	//	/*case WM_CTLCOLORDLG:
	//	//case WM_CTLCOLORSTATIC :
	//	SetTextColor((HDC)wParam, RGB(0,0,0)) ;
	//	SetBkColor((HDC)wParam, m_PicShowMeBg.GetPixel(0,0));
	//	return (LRESULT)CreateSolidBrush(m_PicShowMeBg.GetPixel(0,0));*/
	//}

	//�����λ��ƥ�丸����
	//JianGuankun 2011.1.12
	if (message == WM_MOVE)
	{
		if (m_pPmdWnd)
		{
			if (this->GetSafeHwnd())
			{
				m_pPmdWnd->AdjustPos(this->GetSafeHwnd());
			}
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}
// �ô�ƽ̨��������Ϣ��ʼ��
bool CLoveSendClassInExe::InitFrame(GameInfoStructInExe * pGameInfo, CWnd *pGameRoom)
{
	m_pGameInfo=pGameInfo;
	m_pGameRoom=pGameRoom;
	_tcscpy(m_szGameRoomName, pGameInfo->szGameRoomName); //modify lxl 2010-10-18 ��ʾ�������� 
	Glb().m_szProcessName = pGameInfo->szProcessName;
	if (m_pGameView != NULL) m_pGameView->SetGameInfo(pGameInfo);//m_pGameView->SetGameInfo(m_pGameInfo);

	return true;
}
//�رմ���
bool CLoveSendClassInExe::AFCCloseFrame(bool bNotifyServer)
{
	//OutputDebugString("[aa]SDK CLoveSendClassInExe::AFCCloseFrame");
	//�������ò���
	WriteProfileInt(TEXT("CanLooking"),m_pGameInfo->bEnableWatch?1:0);
	WriteProfileInt(TEXT("PlaySound"),m_pGameInfo->bEnableSound?1:0);
	WriteProfileInt(TEXT("BackSound"),m_pGameInfo->bEnableBackSound?1:0);

	WriteProfileInt(TEXT("ShowUserInfo"),m_pGameInfo->bShowUserInfo?1:0);
	//	m_PlaySound.StopMusic();
	//m_DlgGetPoint.ShowWindow(false);
	//���ò���
	SetStationParameter(0);
	//ȥ����ʾ
	if(m_DlgShowLeft.Showing)
	{
		m_DlgShowLeft.KillTimer(1);
		m_DlgShowLeft.ShowWindow(0);
		m_DlgShowLeft.Showing=0;
		m_DlgShowLeft.DestroyWindow();
	}
	m_TalkMessage.DestroyWindow();

	// ���ͬ������嵥
	UserItemStruct * pFindUserItem=NULL;
	INT_PTR uCount=m_ThisDeskPtrArray.GetCount();
	for (INT_PTR i=0;i<uCount;++i)
	{
		pFindUserItem=(UserItemStruct *)m_ThisDeskPtrArray.GetAt(i);
		delete pFindUserItem;
		pFindUserItem = NULL;
	}
	m_ThisDeskPtrArray.RemoveAll();

	//���͹ر���Ϣ��������
	//::SendMessage(m_pGameRoom->m_hWnd,IDM_CLOSE_GAME,0,0);

	if (GetSafeHwnd()!=NULL) 
	{
		//		m_pGameView->ResetGameView();
		ResetGameFrame();
		this->ShowWindow (SW_HIDE);
		//this->DestroyWindow();
	}
	BZSoundPause(m_sndListID);
	CWnd * winApp=AfxGetApp()->m_pMainWnd;
	if(winApp && winApp->m_hWnd){
		winApp->PostMessage(WM_GAME_QUIT,bNotifyServer?0:1,0);	// �̲β�Ϊ0����ʾ��֪ͨ����
		//OutputDebugString("[aa]SDK CLoveSendClassInExe::AFCCloseFrame end");
	}
	if (NULL != m_pPersonBank)
	{
		if (NULL != m_pPersonBank->GetSafeHwnd())
		{
			Sleep(10);
		}
		delete m_pPersonBank;
		m_pPersonBank = NULL;
	}

	return true;
}

// CLoveSendClassInExe ��Ϣ�������

int CLoveSendClassInExe::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	//�����ҿ��
	//JianGuankun 2012.4.12
	m_pRightFrame = new CRightFrame(this);
	m_pRightFrame->Create(m_hWnd,NULL,UI_WNDSTYLE_DIALOG,0,0,0,0,0,NULL);
	m_UserListDlg.m_pUserListDu = m_pRightFrame->m_pUsersList;
	m_MessageHandle.SwitchToDuiVersion(m_pRightFrame);

	//m_InputMessage.Create(CBS_DROPDOWN|CBS_AUTOHSCROLL|WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,IDC_INPUT);//��������� �����б�

	return 0;
}
//006λ�ñ仯
void CLoveSendClassInExe::OnSize(UINT nType, int cx, int cy)
{
	if (cx<800)
	{
		cx = 800;
	}
	if (cy<600)
	{
		cy = 600;
	}
	FixControlStation(cx,cy);
	__super::OnSize(nType, cx, cy);
	////���������
	//if (m_HorSplitBar.GetSafeHwnd()) 
	//{
	//	SafeMoveWindow(&m_HorSplitBar,cx-209,414,204,SPLIT_WIDE);
	//	m_HorSplitBar.InitSplitBar(310,cy-100,true);
	//}
	return;
}

// PengJiLin, 2011-6-23, ����
void CLoveSendClassInExe::OnActivate(UINT uState, CWnd* pOldWnd, BOOL bMinisted)
{
	this->m_pGameRoom->PostMessage(WM_ACTIVE_TO_ROOM, uState, 1);
   /* if(WA_INACTIVE != uState)
    {
        this->m_pGameRoom->PostMessage(WM_ACTIVE_TO_ROOM, 0, 1);
	}
	else
	{
		this->m_pGameRoom->PostMessage(WM_ACTIVE_TO_ROOM, 1, 1);
    }*/
}

//����״̬����
void CLoveSendClassInExe::SetStationParameter(BYTE bGameStation)
{
	m_pGameInfo->bGameStation=bGameStation;
	this->m_pGameRoom->PostMessage(WM_SET_STATION, 0, bGameStation);
}

//�������ý���
void CLoveSendClassInExe::ResetGameFrame()
{
	//�������Ԫ��
	m_UserListDlg.DeleteAllUser();
	//for (int i=1;i<m_TargetPeople.GetCount();i++) 
	//	m_TargetPeople.DeleteString(i);
	//m_TargetPeople.SetCurSel(0);
	UpdateGameTitle();
	//�����û�����
	m_bSaveView=false;
	m_bWatchMode=true;
	m_bWatchOther=false;
	m_pGameInfo->bGameStation=0;
	//m_pGameInfo->bDeskOnwer=false;
	memset(m_pUserInfo,0,sizeof(m_pUserInfo));
	for (INT_PTR i=0;i<m_WatchPtrArray.GetCount();i++) 
		m_WatchPtrArray.SetAt(i,NULL);
	m_BtGameQuit.m_bIsMouseMove = false;
	m_BtExit.m_bIsMouseMove = false;
	m_Btbt3.m_bIsMouseMove = false;
	return;
}

//������Ϣ
LRESULT CLoveSendClassInExe::OnExpression(WPARAM wparam, LPARAM lparam)
{
	CTrueItem * pExpItem=m_ExpressWnd.GetExpressItem((UINT)wparam);
	if (pExpItem!=NULL)
	{
		CString strBuffer;
		m_InputMessage.GetWindowText(strBuffer);
		strBuffer+=pExpItem->m_szTrancelate;
		m_InputMessage.SetWindowText(strBuffer);
		m_InputMessage.SetFocus();
		m_InputMessage.SetEditSel(strBuffer.GetLength(),strBuffer.GetLength());
		if(m_pRightFrame)
		{
			m_pRightFrame->AddExpressionToTalkInput(pExpItem->m_szTrancelate);
		}
	}
	return 0;
}

//������Ϣ
LRESULT CLoveSendClassInExe::OnRichEditHit(WPARAM wparam, LPARAM lparam)
{
	if ((wparam!=NULL)&&((lparam==WM_LBUTTONDBLCLK)||(lparam==WM_LBUTTONDOWN)))
	{
		CString * pStrBuffer=(CString *)wparam;
		//int iSelect=m_TargetPeople.FindString(0,*pStrBuffer);
		//if (iSelect!=LB_ERR) m_TargetPeople.SetCurSel(iSelect);
		m_InputMessage.SetFocus();
	}
	return 0;
}
//�ر���Ϸ
LRESULT CLoveSendClassInExe::OnCloseGame(WPARAM wparam, LPARAM lparam)
{
	//ȥ����ʾ
	if(m_DlgShowLeft.Showing)
	{
		m_DlgShowLeft.KillTimer(1);
		m_DlgShowLeft.ShowWindow(0);
		m_DlgShowLeft.Showing=0;
		m_DlgShowLeft.DestroyWindow();
	}
	//�ر�
	AFCCloseFrame();
	return 0;
}
//�������Ϣ
LRESULT CLoveSendClassInExe::OnSplitMessage(WPARAM wparam, LPARAM lparam)
{
	//if (wparam==IDC_HOR_SPLIT)	//�������Ϣ
	//{
	//	//�ƶ������
	//	CRect ClientRect;
	//	GetClientRect(&ClientRect);
	//	SafeMoveWindow(&m_HorSplitBar,ClientRect.right-209,HIWORD(lparam),204,SPLIT_WIDE);

	//	//�ƶ������ؼ�
	//	FixControlStation(ClientRect.Width(),ClientRect.Height());
	//}

	return 0;
}
// �ӵ�������д���ʹ�õ�����Ϣ
LRESULT CLoveSendClassInExe::OnPannelUseProp(WPARAM wparam, LPARAM lparam)
{
	UsePropItem(lparam);
	return 0;
}
// �ӵ�������д��ع��������Ϣ
LRESULT CLoveSendClassInExe::OnPannelBuyProp(WPARAM wparam, LPARAM lparam)
{
	this->m_pGameRoom->PostMessage(WM_PROP_BUY, wparam, lparam);
	return 0;
}
// �ӵ�������д������͵�����Ϣ
LRESULT CLoveSendClassInExe::OnPannelGiveProp(WPARAM wparam, LPARAM lparam)
{
	this->m_pGameRoom->PostMessage(WM_GIVE_PROP, wparam, lparam);
	OnSetPropImages();
	return 0;
}

LRESULT CLoveSendClassInExe::OnPannelBuyVIP(WPARAM wparam, LPARAM lparam)
{
	this->m_pGameRoom->PostMessage(WM_BUY_VIP, wparam, lparam);
	return 0;
}

LRESULT CLoveSendClassInExe::OnSendGetFriendList(WPARAM wparam, LPARAM lparam)
{
	this->m_pGameRoom->PostMessage(WM_GETFRIENDLIST, wparam, lparam);
	return 0;
}

//��Ϸ���д�ȡǮ
LRESULT CLoveSendClassInExe::OnGameBankMoneyCheck(WPARAM wParam, LPARAM lParam)
{
	OnCheckMoney( (CheckMoney_t *)(lParam) );
	return 0;
}

LRESULT CLoveSendClassInExe::OnGameBankMoneyCheck2(WPARAM wParam, LPARAM lParam)
{
	OnCheckMoney2((bank_op_normal*)(lParam));
	return 0;
}

//�ر���Ϸ���д�����Ϣ
LRESULT CLoveSendClassInExe::OnGameBankClose(WPARAM wParam, LPARAM lParam)
{
	OnCloseBank2();
	return 0;
}

//�û�ʹ�ù㲥���Ի�����ȷ������������û�ID���û�����ֵ
LRESULT CLoveSendClassInExe::OnUseBoardcast(WPARAM wparam, LPARAM lparam)
{
	_TAG_BOARDCAST *pMsg = (_TAG_BOARDCAST *)lparam;
	pMsg->dwUserID = this->GetMeUserInfo()->dwUserID;
	//	int nType = *((int*)((BYTE*)lparam + sizeof(_TAG_BOARDCAST)));
	_tcscpy_s(pMsg->szUserName, 32, this->GetMeUserInfo()->nickName);

	//���ٵ�������
	int propCount=Glb().userPropLibrary.GetCount();
	_TAG_USERPROP *userProp=NULL;
	for(int i=0;i<propCount;i++)
	{
		userProp=Glb().userPropLibrary.GetAt(i);
		if(userProp->nPropID==pMsg->nPropID)
		{
			userProp->nHoldCount-=1;
            if(userProp->nHoldCount < 0)userProp->nHoldCount = 0;
			//if(userProp->nHoldCount<1)    // PengJiLin, 2010-10-14, �µĵ���ϵͳ��������Ϊ0������
			//{
			//	Glb().userPropLibrary.RemoveAt(i);
			//	delete userProp;
			//	userProp=NULL;
			//}
			//��Ҫ�޸ı��ؽ��
			break;
		}
	}

	this->m_pGameRoom->PostMessage(WM_USE_BOARDCAST, wparam, lparam);
	return 0;
}


BOOL CLoveSendClassInExe::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
	cs.style &= ~WS_VISIBLE;
	cs.style |= WS_CLIPSIBLINGS;
	cs.style |= WS_CLIPCHILDREN;

	return __super::PreCreateWindow(cs);
}

///< ��ȡ�ȼ���Ϣ��GetGameOrderNameʹ��
void CLoveSendClassInExe::ReadLevelData()
{
	CString strKey;
	CString strConfig = CBcfFile::GetAppPath();
	strConfig += _T("GameOption.bcf");
	CBcfFile fLevel(strConfig);

	for(int i=1; i<21; i++)
	{
		strKey.Format(_T("Order%d"),i);
		g_PointArray[i] = fLevel.GetKeyVal(_T("ScoreOrderName"),strKey,0);
		strKey = fLevel.GetKeyMemo(_T("ScoreOrderName"),strKey,_T("")); /// �����strKey��ֵֻ�ǽ��ô˱���
		g_LevelNameArray.insert(pair<int,string>(i,strKey.GetBuffer()));
	}
}

///< ��ȡ��Ϸ����Ļ��ּ���
///< @dwPoint��һ���
///< ��һ�������
TCHAR * CLoveSendClassInExe::GetGameOrderName(__int64 dwPoint)
{
	int low = 1;
	int high = 19;
	int mid;
	UINT point;
	TCHAR OrderName[MAX_PATH] = {0};

	dwPoint = dwPoint<0?0:dwPoint; ///����С����Ҳ����ͼ���

	while(low <= high)
	{
		mid = low + (high-low)/2;

		if (dwPoint<g_PointArray[mid])
		{
			high = mid-1;
		}
		else
		{
			low = mid+1;
		}
	}

    // PengJiLin, 2010-8-2, ����Խ�紦��
    int iFirst = 1;         // Key �� 1 ��ʼ
    int iLast = g_LevelNameArray.size();
    int iIndex = low-1;
    if(iIndex < iFirst)
    {
        iIndex = iFirst;
    }
    else if(iIndex > iLast)
    {
        iIndex = iLast;
    }

	map<int,string>::iterator ite = g_LevelNameArray.find(iIndex);

	return (TCHAR*)(*ite).second.c_str();
}

BOOL CLoveSendClassInExe::OnInitDialog()
{
	GetDlgItem(IDC_SNDPLAY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SNDPAUSE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SNDVOLDOWN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SNDVOLUP)->ShowWindow(SW_HIDE);

	CBcfFile forder( CBcfFile::GetAppPath() + "..\\Order.bcf");
	Glb().m_nPowerOfGold = forder.GetKeyVal("VirtualMoney", "PowerOfGold", 0);

    // PengJiLin, 2010-8-3, �����ʾ�ָ�������
    Glb().m_bUseSpace = forder.GetKeyVal("VirtualMoney", "UseSpace", 0);
    CString strSpace = forder.GetKeyVal("VirtualMoney", "SpaceChar", " ");
    memcpy(Glb().m_strSpaceChar, strSpace, sizeof(Glb().m_strSpaceChar)*sizeof(TCHAR));

	int _bit = forder.GetKeyVal("VirtualMoney", "SpaceBits", 3);
	if (_bit == 0) Glb().m_iMaxBit = 18;
	else Glb().m_iMaxBit = 18 + 18 / _bit - 1;

	CLoadFileBmp::OnInitDialog();

	////OutputDebugString("CLoveSendClassInExe::OnInitDialog begin");

	ModifyStyle( WS_CAPTION, WS_MINIMIZEBOX|WS_MAXIMIZEBOX, SWP_DRAWFRAME );

	AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
	m_PlaySound.CreateWnd(this,IDD_NULL);
	AfxSetResourceHandle(GetModuleHandle(NULL));
	m_pGameView->m_showcoredlg.m_ucomtype=GetComType();
	m_pGameView->m_showcoredlg.m_count=m_pGameInfo->uDeskPeople;
	//��ȡ���ò���
	m_pGameInfo->bEnableWatch=(GetProfileInt(TEXT("CanLooking"),FALSE)!=FALSE);
	m_pGameInfo->bEnableSound=(GetProfileInt(TEXT("PlaySound"),TRUE)!=FALSE);
	m_pGameInfo->bEnableBackSound=(GetProfileInt(TEXT("BackSound"),TRUE)!=FALSE);
	m_pGameInfo->bShowUserInfo=(GetProfileInt(TEXT("ShowUserInfo"),FALSE)!=FALSE);  //PengJiLin, 2010-10-15, ȱʡ����ʾ��ҽ�����Ϣ

	m_CurpUserInfoClicked=GetMeUserInfo();
	//if(m_Btbt5.m_hWnd)m_Btbt5.ShowWindow(SW_HIDE);
	//if(m_Btbt6.m_hWnd)m_Btbt6.ShowWindow(SW_HIDE);
	//if(m_Btbt7.m_hWnd)m_Btbt7.ShowWindow(SW_HIDE);
	//if(m_Btbt8.m_hWnd)m_Btbt8.ShowWindow(SW_HIDE);
	//if(m_Btbt9.m_hWnd)m_Btbt9.ShowWindow(SW_HIDE);
	if(m_Btbt10.m_hWnd)m_Btbt10.ShowWindow(SW_HIDE);
	CBcfFile bcffile(CBcfFile::GetAppPath() + _T("GameOption.bcf"));
	UINT uMaxType = bcffile.GetKeyVal("FrameControl","max_type",1);
	if (uMaxType != 0)
	{
	m_btRest.ShowWindow(SW_HIDE);
	m_btMax.ShowWindow(SW_SHOW);
	}
	//��ʾ��Ϸ���Ҵ���
	if (m_pRightFrame)
	{
		m_pRightFrame->ShowWindow(SW_SHOW);
	}

	LoadSkin();

	//������Ϸ��ͼ
	//HINSTANCE hInstance=AfxGetInstanceHandle();
	//AfxSetResourceHandle(hInstance);
	//m_ADDialog.Create(IDD_FLASH_DLG,this);
	//m_HorSplitBar.InitSplitBar(310,500,true);

	////OutputDebugString("CLoveSendClassInExe::OnInitDialog 3");

	//���ÿؼ�
	AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
	m_UserListDlg.Create(IDD_USER_LIST, this);
	AfxSetResourceHandle(GetModuleHandle(NULL));

	ReadLevelData(); ///��ȡ���ֵȼ�����
	m_pGameInfo->pOrderName = GetGameOrderName;///< ��ȡ��Ϸ����Ļ��ּ���,ȡ����ǰ����ϷDLL��������ֵȼ�

	////OutputDebugString("CLoveSendClassInExe::OnInitDialog 4");
	//wushuqun 2009.5.27
	//�޸���Ϣ��ȼ�������б��еĵȼ���ͬ
	//m_UserListDlg.InitListCtrl(GetGameOrderNameInExe,m_pGameInfo->uComType);
	if( m_pGameInfo->pOrderName == NULL )
	{
		MessageBox("m_pGameInfo->pOrderName == NULL");
		return FALSE;
	}
	m_UserListDlg.InitListCtrl(GetPointOrderName,m_pGameInfo->uComType);

	//OutputDebugString("CLoveSendClassInExe::OnInitDialog 5");

	m_pGameView->Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE,CRect(0,0,0,0),this,ID_GAME_VIEW,NULL);//������䳬��

	//if (IsQueueGameRoom())
	if (m_pGameInfo->dwRoomRule & GRR_NOTCHEAT)
	{
		m_pGameView->m_bShowRight = false;
	}
	UINT showright = (m_pGameView->m_bShowRight) ? SW_SHOW : SW_HIDE;
	showright = SW_HIDE;
	//OutputDebugString("CLoveSendClassInExe::OnInitDialog 5.666");

	m_TalkMessage.Init();

	m_TalkMessage.SetTextMode(TRANSPARENT);
	m_TalkMessage.IsGameMessage();						//�����������Ϣ���������治ͬ��ini�ļ����б��ڴ���

	PostMessageA(WM_RELOAD_TALKINI);

	////////////////////
	if (CUserPower::CanLookIP(m_pGameInfo->dwMasterPower)) 
		m_UserListDlg.EnableIPView(true);
	m_InputMessage.LimitText(CUserPower::CanSendMuch(m_pGameInfo->dwGamePower)?MAX_TALK_LEN:NORMAL_TALK_LEN);
	//���ù��

	//////////////////////////////////////////////////////////////////////////
	//��ȡ���ڹ���ַ,Fred Huang,2008-06-02
#ifdef _USE_IE_AD_
	CString s=CBcfFile::GetAppPath () + "..\\";//CINIFile::GetAppPath ();/////����·��
	CBcfFile f( s + "bzgame.bcf");
	szAdvUrl=f.GetKeyVal (Glb().m_key,"gameadvertise","");
	if(szAdvUrl=="")
		m_ie.MoveWindow(-10,-10,5,5);
	else
	{
		CString surl;
		if(szAdvUrl.Find("?")!=-1)
			surl.Format("%s&roomid=%d",szAdvUrl,m_pGameInfo->uRoomID);
		else
			surl.Format("%s?roomid=%d",szAdvUrl,m_pGameInfo->uRoomID);
		if (m_pGameView->m_bShowRight)
		{
			m_ie.Navigate(surl,0,0,0,0);
		}
	}
#else
	m_ie.ShowWindow(SW_HIDE);
#endif

	//////////////////////////////////////////////////////////////////////////	
	//UpdateGameTitle();
	if(GetComType()==TY_MATCH_GAME)
	{
		m_InputMessage.EnableWindow(false);
		m_BtExpression.EnableWindow(false);
		m_BtSend.EnableWindow(false);
		}	
	//�����
	if (m_pGameView->m_bShowRight)
	{
		m_TalkMessage.SetFocus();
	}

	//���ÿؼ�
	m_bInit = true;
	//{add by zxj 2008/04/07
	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	m_ToolTip.AddTool(GetDlgItem(IDC_GAME_BTN_01), TEXT("������Ϸ���ѡ��"));
	m_ToolTip.AddTool(GetDlgItem(IDC_GAME_BTN_02), TEXT("�������"));
	m_ToolTip.AddTool(GetDlgItem(IDC_GAME_BTN_03), TEXT("��ֵ"));
	m_ToolTip.AddTool(GetDlgItem(IDC_GAME_BTN_04), TEXT("����ҳ"));
	m_ToolTip.AddTool(GetDlgItem(IDC_GO_AHEAD),TEXT("��ǰ�鿴����"));
	//m_ToolTip.AddTool(GetDlgItem(IDC_GO_NEXT),TEXT("���鿴����"));
	m_ToolTip.AddTool(GetDlgItem(IDC_EXPRESSION),TEXT("ѡ�����ͷ��"));
	m_ToolTip.AddTool(GetDlgItem(IDC_SEND),TEXT("����������Ϣ"));
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_PROP1), "VIP����");
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_PROP2), LPSTR_TEXTCALLBACK);
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_PROP3), LPSTR_TEXTCALLBACK);
    m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_PROP4), LPSTR_TEXTCALLBACK);
    m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_PROP5), LPSTR_TEXTCALLBACK);
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_PROP6), LPSTR_TEXTCALLBACK);
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_PROP7), LPSTR_TEXTCALLBACK);
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_PROP8), LPSTR_TEXTCALLBACK);
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_PROP9), LPSTR_TEXTCALLBACK);
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_PROP10), LPSTR_TEXTCALLBACK);

	//wushuqun ���ֲ��Ű�ť����ʾ

	//m_ToolTip.AddTool(GetDlgItem(IDC_SNDPLAY),TEXT("��������")); 
	//m_ToolTip.AddTool(GetDlgItem(IDC_SNDPAUSE),TEXT("ֹͣ��������")); 
	//m_ToolTip.AddTool(GetDlgItem(IDC_SNDVOLDOWN),TEXT("��С����")); 
	//m_ToolTip.AddTool(GetDlgItem(IDC_SNDVOLUP),TEXT("��������"));

    // PengJiLin, 2010-10-12, �̳����߹���
    m_ToolTip.AddTool(GetDlgItem(IDC_GAME_BTN_06), TEXT("��ǰ�鿴����"));
    m_ToolTip.AddTool(GetDlgItem(IDC_GAME_BTN_07), TEXT("���鿴����"));


    // PengJilin, 2010-9-30
    m_ToolTip.AddTool(GetDlgItem(IDC_GAME_BTN_05),TEXT("����VIP"));

	LoadSoundList();

	OnInitPropList();


	if (uMaxType == 1) //��ͨ���
	{
		OnBnClickedMax();
	}
	else if (uMaxType == 2) //�����øı���Ļ�ֱ��ʵķ�ʽ
	{
		OnBnClickedRest();
	}

	
	m_UserListDlg.ShowWindow(showright);
	m_TalkMessage.ShowWindow(showright);
	m_Btbt1.ShowWindow(SW_SHOW);
	m_Btbt2.ShowWindow(SW_HIDE);						//modify by wyl   11-5-12
	m_Btbt3.ShowWindow(SW_HIDE);
	m_Btbt4.ShowWindow(SW_HIDE);
	m_Btbt5.ShowWindow(SW_HIDE);						//end
			
	m_BtPropGoAhead.ShowWindow(SW_HIDE);
	m_BtPropGoNext.ShowWindow(SW_HIDE);
	m_BtProp1.ShowWindow(showright);
	m_BtProp2.ShowWindow(showright);
	m_BtProp3.ShowWindow(showright);
	m_BtProp4.ShowWindow(showright);
	m_BtProp5.ShowWindow(showright);
	m_BtProp6.ShowWindow(showright);
	m_BtProp7.ShowWindow(showright);
	m_BtProp8.ShowWindow(showright);
	m_BtProp9.ShowWindow(showright);
	m_BtProp10.ShowWindow(showright);
	m_BtExpression.ShowWindow(showright);
	m_InputMessage.ShowWindow(showright);
	m_BtSend.ShowWindow(showright);

	//��ƽ̨�����ȡͬ���û�����
	BYTE bMeDeskStation=m_pGameInfo->uisMeUserInfo.GameUserInfo.bDeskNO;
	if (bMeDeskStation != 255)
	{
		GetOnLineUser( bMeDeskStation );
	}
	m_Btbt1.SetFocus();

	//Ϊ�Ŷӻ����
	if ((m_pGameInfo->dwRoomRule & GRR_NOTCHEAT) > 0)//(IsQueueGameRoom())
	{
		m_pGameView->m_bShowRight = false;
// 		m_BtHide.EnableWindow(FALSE);
// 		m_BtHide.ShowWindow(SW_HIDE);
		
		OnBnClickedHide();
	}
	//end of Ϊ�Ŷӻ����
	UpdateGameTitle();

	
    // PengJiLin, 2010-6-11, ��ΪŸ����Ŀ�����ð�ť�����ˣ�
    // ���Թ̶�����ΪTRUE�����������������ã������Աû�г�ʼ��
    BYTE byType = m_UserListDlg.m_UserList.GetUserListDefineType();
    switch(byType)
    {
    case 1:
        m_pGameInfo->bEnableWatch = true;
        m_pGameInfo->bEnableSound = true;
        m_pGameInfo->bEnableBackSound = true;
        m_pGameInfo->bShowUserInfo = false;     //PengJiLin, 2010-10-15, ȱʡ����ʾ��ҽ�����Ϣ
        break;
    default:
        break;
    }

    // PengJiLin, 2010-9-27, ������ʾ����
    m_BtProp1.SetTextColor(RGB(255, 255, 255), true);
    m_BtProp2.SetTextColor(RGB(255, 255, 255), true);
    m_BtProp3.SetTextColor(RGB(255, 255, 255), true);
    m_BtProp4.SetTextColor(RGB(255, 255, 255), true);
    m_BtProp5.SetTextColor(RGB(255, 255, 255), true);
    m_BtProp6.SetTextColor(RGB(255, 255, 255), true);
    m_BtProp7.SetTextColor(RGB(255, 255, 255), true);
    m_BtProp8.SetTextColor(RGB(255, 255, 255), true);
	m_BtProp9.SetTextColor(RGB(255, 255, 255), true);
	m_BtProp10.SetTextColor(RGB(255, 255, 255), true);
    
    m_Btbt5.ShowWindow(SW_HIDE);
    m_Btbt6.ShowWindow(showright);
    m_Btbt7.ShowWindow(showright);


	//��ȡԭ���ڳߴ�
	m_nWiondwMetricsX = ::GetSystemMetrics(SM_CXSCREEN); 
	m_nWiondwMetricsY = ::GetSystemMetrics(SM_CYSCREEN);

	CRect rect;
	GetClientRect(&rect);
	FixControlStation(rect.Width(),rect.Height());
	m_pGameView->UpdateViewFace(NULL);
	return false;
}


//��ȡ��ֵ
UINT CLoveSendClassInExe::GetProfileInt(TCHAR * szEntry, int iDefault)
{
	return AfxGetApp()->GetProfileInt(m_pGameInfo->szGameName,szEntry,iDefault);
}

///011��ȡ�ַ���
CString CLoveSendClassInExe::GetProfileString(TCHAR * szEntry, TCHAR * szDefault)
{
	return AfxGetApp()->GetProfileString(m_pGameInfo->szGameName,szEntry,szDefault);
}

//д����ֵ
BOOL CLoveSendClassInExe::WriteProfileInt(TCHAR * szEntry, int nValue)
{
	return AfxGetApp()->WriteProfileInt(m_pGameInfo->szGameName,szEntry,nValue);
}

//��ȡ�ַ���
BOOL CLoveSendClassInExe::WriteProfileString(TCHAR * szEntry, TCHAR * szValue)
{
	return AfxGetApp()->WriteProfileString(m_pGameInfo->szGameName,szEntry,szValue);
}

/// ���������ȡ��
LRESULT CLoveSendClassInExe::OnBankCheckOut(WPARAM wparam,LPARAM lparam)
{
	if (lparam != sizeof(MSG_GR_S_CheckMoney))
	{
		return E_FAIL;
	}
	MSG_GR_S_CheckMoney *pCheckMoney = (MSG_GR_S_CheckMoney *)wparam;

	//m_TCPSocket->SendData(&checkMoney,sizeof(checkMoney),MDM_GR_MONEY,ASS_GR_CHECK_OUT,0);
	SendGameData(pCheckMoney, sizeof(MSG_GR_S_CheckMoney), MDM_GR_MONEY,ASS_GR_CHECK_OUT_INGAME,0);
	delete pCheckMoney;
	pCheckMoney = NULL;
	return 0;
}

//���ͻ���
LRESULT CLoveSendClassInExe::OnGetPoint(WPARAM wparam,LPARAM lparam)
{
	TCHAR url[300];
	wsprintf(url,"%sapp/huodong.asp",Glb().m_CenterServerPara.m_strWebRootADDR);
	//wsprintf(url,"%schongzhi.%s?roomid=%d&un=%s",Glb().m_CenterServerPara.m_strHomeADDR,Glb().m_urlpostfix,m_pGameInfo->uRoomID,m_pGameInfo->uisMeUserInfo.GameUserInfo.szName);
	ShellExecute(NULL,"open",url,NULL,NULL,SW_MAXIMIZE|SW_SHOW);
	return 0;
	//	if((int)lparam != 0)
	//		return 0;
	MSG_GR_S_GetPoint getpoint;
	::memset(&getpoint,0,sizeof(getpoint));
	getpoint.dwPoint = (int)wparam;
	SendGameData(&getpoint, sizeof(getpoint), MDM_GR_POINT, ASS_GR_GET_POINT, 0);
	return 0;
}

// ����Ƥ�� 
void CLoveSendClassInExe::LoadSkin(void)
{
	int r,g,b;
	CString s=CBcfFile::GetAppPath ()+"..\\";//;/////����·��
	TCHAR path[MAX_PATH];
	//s += m_iniFile;
	//�ټ��� add by wlr 20090716
	CFileFind	fileFind;			//�����ļ�
	bool		bFileExist=false;	//�ļ��Ƿ���ڣ�ȱʡΪ������
	wsprintf(path,".\\config\\" + m_skinmgr.GetGameSkinBcfFileName()); /// add by wxx 

	bFileExist=fileFind.FindFile(path);
	fileFind.Close();
	if (bFileExist)
	{
		m_iniFile = path;   ///add by wxx
		s = "";
	}

	CString strBcfFilename = m_skinmgr.GetGameSkinBcfFileName();
	if (CBcfFile::IsFileExist(CBcfFile::GetAppPath() + "config\\" + strBcfFilename))
		m_iniFile= CBcfFile::GetAppPath() + "config\\" + strBcfFilename;
	else
		m_iniFile= CBcfFile::GetAppPath()+"..\\config\\" + strBcfFilename;

	//�ټ��� end of add by wlr 20090716
	CBcfFile f( m_iniFile);

	CString key=TEXT("config");
	
	CString skinfolder;
	skinfolder=f.GetKeyVal(key,"skinfolder","image\\client\\skin0\\game\\");
	skinfolder=s+skinfolder;
	DebugPrintf("s=%s",s);
	DebugPrintf("skinfolder=%s",skinfolder);
	skinfolder.Replace("Image","image");
	CString skinHall = CBcfFile::GetAppPath()+"..\\image\\client\\skin0\\";

	CString ph;
	//����ͼ
	ph=f.GetKeyVal(key,"backimages","image\\client\\skin0\\game\\backimages.bmp");

	//m_pGameView->m_bkimgs.SetLoadInfo(ph.GetBuffer(),true);
	m_backImage.SetLoadInfo(ph.GetBuffer(),true);
	//�����ĸ���
	ZeroMemory(path,sizeof(path));
	//wsprintf(path,"%s\\game_tc.bmp",skinfolder);
	GET_FILENAME(path,"%s\\game_tc.bmp",skinfolder);
	m_tc.SetLoadInfo(path,true);//CGameImageLink::m_bAutoLock);
	GET_FILENAME(path,"%s\\game_bc.bmp",skinfolder);
	m_bc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	GET_FILENAME(path,"%s\\game_rc.bmp",skinfolder);
	m_rc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	GET_FILENAME(path,"%s\\GameWnd.png",skinfolder);
	m_lc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	//�Ľ�
	GET_FILENAME(path,"%s\\game_lt.bmp",skinfolder);
	m_lt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	GET_FILENAME(path,"%s\game_lb.bmp",skinfolder);
	m_lb.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	GET_FILENAME(path,"%s\\game_rt.bmp",skinfolder);
	m_rt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	GET_FILENAME(path,"%s\\game_rb.bmp",skinfolder);
	m_rb.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	//�����ұ�
	GET_FILENAME(path,"%s\\game_right_tc.bmp",skinfolder);
	m_right_tc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	GET_FILENAME(path,"%s\\game_right_bc.bmp",skinfolder);
	m_right_bc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	GET_FILENAME(path,"%s\\game_right_lc.bmp",skinfolder);
	m_right_lc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	GET_FILENAME(path,"%s\\game_right_rc.bmp",skinfolder);
	m_right_rc.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	//�ĸ���
	GET_FILENAME(path,"%s\\game_right_lt.bmp",skinfolder);
	m_right_lt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	GET_FILENAME(path,"%s\\game_right_lb.bmp",skinfolder);
	m_right_lb.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	GET_FILENAME(path,"%s\\game_right_rt.bmp",skinfolder);
	m_right_rt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	GET_FILENAME(path,"%s\\game_right_rb.bmp",skinfolder);
	m_right_rb.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	GET_FILENAME(path,"%s\\UserInfo_bk.bmp",skinfolder);
	m_UserInfo_bk.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	//�м�
	//	wsprintf(path,"%s\\mid_c.bmp",skinfolder);
	//	m_iMid.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

    // PengJiLin, 2010-9-28, ��ť����ͼ
    GET_FILENAME(path,"%s\\probBack.bmp",skinfolder);
    m_propBackImage.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

    GET_FILENAME(path,"%s\\probBack2.bmp",skinfolder);
    m_propBackImage2.SetLoadInfo(path,CGameImageLink::m_bAutoLock);

	//
	ph=f.GetKeyVal(key,"backimageh",m_skinmgr.GetGameSkinPath() + "backimageh.bmp");


	m_pGameView->m_bkimgh.SetLoadInfo(ph.GetBuffer(),true);
	m_pGameView->m_ShowType=f.GetKeyVal(key,"showtype",0);
	m_pGameView->m_bShowRight=f.GetKeyVal(key,"showright",1);

	m_titlex=f.GetKeyVal(key,"titlex",12);
	m_titley=f.GetKeyVal(key,"titley",7);
	r=f.GetKeyVal(key,"titler",255);
	g=f.GetKeyVal(key,"titleg",234);
	b=f.GetKeyVal(key,"titleb",0);
	m_titlecr=RGB(r,g,b);
	m_logx=f.GetKeyVal(key,"flogx",198);
	m_logy=f.GetKeyVal(key,"flogy",40);
	m_u1x=f.GetKeyVal(key,"u1x",110);
	m_u1y=f.GetKeyVal(key,"u1y",64);
	m_u1w=f.GetKeyVal(key,"u1w",75);
	m_u1h=f.GetKeyVal(key,"u1h",18);
	m_u2x=f.GetKeyVal(key,"u2x",189);
	m_u2y=f.GetKeyVal(key,"u2y",158);
	m_u2w=f.GetKeyVal(key,"u2w",86);
	m_u2h=f.GetKeyVal(key,"u2h",18);
// 	m_vlistx=f.GetKeyVal(key,"vlistx",0);
// 	m_pGameView->m_vlistx=m_vlistx;

	int iWindowsWidth = GetSystemMetrics(SM_CXSCREEN);
	int iWindowsHeight = GetSystemMetrics(SM_CYSCREEN);
	if (m_pRightFrame)
	{
		if (iWindowsWidth >= 1280)
		{
			m_vlistx = m_pRightFrame->m_uMaxWidth;
			m_pGameView->m_vlistx = m_vlistx;
		}
		else
		{
			m_vlistx = m_pRightFrame->m_uMinWidth;
			m_pGameView->m_vlistx = m_vlistx;
		}
	}

	m_vlisty=f.GetKeyVal(key,"vlisty",0);

	r=f.GetKeyVal(key,"userr",0);
	g=f.GetKeyVal(key,"userg",0);
	//wushuqun 2009.6.10
	b=f.GetKeyVal(key,"userb",0);
	m_usercr=RGB(r,g,b);
	/////////////////////////////
	//Kylin 20090205 �����û���Ϣ����
	r=f.GetKeyVal(key,"userbkr",0);
	g=f.GetKeyVal(key,"userbkg",0);
	b=f.GetKeyVal(key,"userbkb",0);
	m_userbkcr=RGB(r,g,b);
	/////////////////////////////

	r=f.GetKeyVal(key,"bkr",45);
	g=f.GetKeyVal(key,"bkg",80);
	b=f.GetKeyVal(key,"bkb",133);
	m_colorresource.m_crTalkBkColor=RGB(r,g,b);
	r=f.GetKeyVal(key,"namer",255);
	g=f.GetKeyVal(key,"nameg",255);
	b=f.GetKeyVal(key,"nameb",255);
	m_colorresource.m_crTalkNameColor=RGB(r,g,b);
	r=f.GetKeyVal(key,"systemr",255);
	g=f.GetKeyVal(key,"systemg",255);
	b=f.GetKeyVal(key,"systemb",255);
	m_colorresource.m_crSystemTxColor=RGB(r,g,b);
	r=f.GetKeyVal(key,"systembdr",255);
	g=f.GetKeyVal(key,"systembdg",0);
	b=f.GetKeyVal(key,"systembdb",0);
	m_colorresource.m_crSystemBdColor=RGB(r,g,b);
	r=f.GetKeyVal(key,"normalr",255);
	g=f.GetKeyVal(key,"normalg",255);
	b=f.GetKeyVal(key,"normalb",255);
	m_colorresource.m_crNormalTxColor=RGB(r,g,b);
	////////////////////////////////////
	////Kylin 20090117 ������Ϣ��ɫ����
	r=f.GetKeyVal(key,"talkr",255);
	g=f.GetKeyVal(key,"talkg",255);
	b=f.GetKeyVal(key,"talkb",255);
	m_colorresource.m_crTalkTxColor=RGB(r,g,b);	
	////////////////////////////////////
	//�����ɫ
	r=f.GetKeyVal(key,"framecolorr",150);
	g=f.GetKeyVal(key,"framecolorg",191);
	b=f.GetKeyVal(key,"framecolorb",239);
	m_FrameColor = RGB(r,g,b);		
	//����
	r=f.GetKeyVal(key,"newscolorr",255);
	g=f.GetKeyVal(key,"newscolorg",255);
	b=f.GetKeyVal(key,"newscolorb",255);
	m_colorresource.m_crNewsTxColor=RGB(r,g,b);		
	r=f.GetKeyVal(key,"senttextr",0);
	g=f.GetKeyVal(key,"senttextg",0);
	b=f.GetKeyVal(key,"senttextb",255);
	m_colorresource.m_crSentTxColor=RGB(r,g,b);	
	r=f.GetKeyVal(key,"receivedtextr",255);
	g=f.GetKeyVal(key,"receivedtextg",0);
	b=f.GetKeyVal(key,"receivedtextb",0);
	m_colorresource.m_crReceivedTxColor=RGB(r,g,b);	

	//m_colorresource.m_crNormalTxColor=RGB(r,g,b);
	SetFillColor(m_colorresource.m_crTalkBkColor);	
	m_MessageHandle.SetColor(m_colorresource);

	CGameImage img;
	HRGN hRgn;
	int w = 18;
	int h = 18;
	GET_FILENAME(path,"%sgame_left_face.bmp",skinfolder);
	m_BtExpression.LoadButtonBitmap(path,false);
	img.SetLoadInfo(path,true);
	hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
	if(m_BtExpression)
	{
		if(hRgn)
			m_BtExpression.SetWindowRgn(hRgn,true);
		m_BtExpression.SetWindowPos(NULL,0,0,img.GetWidth()/4,img.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);
	}

	//������ɫ
	GET_FILENAME(path,"%sgame_setcolor.bmp",skinfolder);
	m_BtSetColor.LoadButtonBitmap(path,false);
	img.SetLoadInfo(path,true);
	hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
	if(m_BtExpression)
	{
		if(hRgn)
			m_BtSetColor.SetWindowRgn(hRgn,true);
		m_BtSetColor.SetWindowPos(NULL,0,0,img.GetWidth()/4,img.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);
	}

	GET_FILENAME(path,"%sgame_left_send.bmp",skinfolder);
	m_BtSend.LoadButtonBitmap(path,false);
	img.SetLoadInfo(path,true);
	if(m_BtSend.m_hWnd)
	{
		m_BtSend.SetWindowPos(NULL,0,0,img.GetWidth()/4,img.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);
		hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
		if(hRgn)
			m_BtSend.SetWindowRgn(hRgn,true);
	}
	//��ָ���,δ��
	//wsprintf(path,"%sgame_left_hsplit.bmp",skinfolder);
	//m_HorSplitBar.SetbkImage(path,path,path);

	GET_FILENAME(path,"%shide_bt.bmp",skinfolder);

	m_BtHide.LoadButtonBitmap(path,false);	
	img.SetLoadInfo(path,true);
	if(m_BtHide.m_hWnd)
	{
		m_BtHide.SetWindowPos(NULL,0,0,img.GetWidth()/4,img.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);
		hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
		if(hRgn)
			m_BtHide.SetWindowRgn(hRgn,true);
	}
	m_HidebtPath = path;
	GET_FILENAME(path,"%sshow_bt.bmp",skinfolder);
	m_ShowbtPath = path;

	w = 20;
	h = 20;
	CBcfFile bcffile(CBcfFile::GetAppPath() + _T("GameOption.bcf"));
	UINT uMaxType = bcffile.GetKeyVal("FrameControl","max_type",1);
	GET_FILENAME(path,"%sgame_min_bt.bmp",skinfolder);
	m_BtMin.LoadButtonBitmap(path,false);	
	if (0 != uMaxType)
	{
	GET_FILENAME(path,"%sgame_max_bt.bmp",skinfolder);
	m_btMax.LoadButtonBitmap(path,false);	
	GET_FILENAME(path,"%sgame_rest_bt.bmp",skinfolder);
	m_btRest.LoadButtonBitmap(path,false);	

	m_btRest.ShowWindow(SW_SHOW);
	m_btMax.ShowWindow(SW_HIDE);
	}
	else
	{
		m_btRest.ShowWindow(SW_HIDE);
		m_btMax.ShowWindow(SW_HIDE);
	}

	GET_FILENAME(path,"%sgame_close_bt.bmp",skinfolder);
	m_BtGameQuit.LoadButtonBitmap(path,false);
//	img.SetLoadInfo(path,true);


	GET_FILENAME(path,"%sgame_bt01.bmp",skinfolder);
	m_Btbt1.LoadButtonBitmap(path,false);
	img.SetLoadInfo(path,true);

	CGameImage sndImg;
	sndImg.SetLoadInfo(path,true);
	m_sndBtnCx=sndImg.GetWidth()/4;
	m_sndBtnCy=sndImg.GetHeight();


	if(m_Btbt1.m_hWnd)
	{
		hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
		if(hRgn)
			m_Btbt1.SetWindowRgn(hRgn,true);
	}

	GET_FILENAME(path,"%sgame_bt02.bmp",skinfolder);
	m_Btbt2.LoadButtonBitmap(path,false);
	img.SetLoadInfo(path,true);
	//m_Btbt2.SetWindowPos(NULL,0,0,img.GetWidth()/4,img.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);
	if(m_Btbt2.m_hWnd)
	{
		hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
		if(hRgn)
			m_Btbt2.SetWindowRgn(hRgn,true);
	}

	GET_FILENAME(path,"%sgame_bt03.bmp",skinfolder);
	m_Btbt3.LoadButtonBitmap(path,false);
	img.SetLoadInfo(path,true);
	//m_Btbt3.SetWindowPos(NULL,0,0,img.GetWidth()/4,img.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);
	if(m_Btbt3.m_hWnd)
	{
		hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
		if(hRgn)
			m_Btbt3.SetWindowRgn(hRgn,true);
	}

	GET_FILENAME(path,"%sgame_bt04.bmp",skinfolder);
	m_Btbt4.LoadButtonBitmap(path,false);
	img.SetLoadInfo(path,true);
	//m_Btbt4.SetWindowPos(NULL,0,0,img.GetWidth()/4,img.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);
	if(m_Btbt4.m_hWnd)
	{
		hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
		if(hRgn)
			m_Btbt4.SetWindowRgn(hRgn,true);
	}

    // PengJiLin, 2010-9-30, �������
    GET_FILENAME(path,"%sgame_btbluediamond.bmp",skinfolder);
    m_Btbt5.LoadButtonBitmap(path,false);
    img.SetLoadInfo(path,true);
    if(m_Btbt5.m_hWnd)
    {
        hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
        if(hRgn)
            m_Btbt5.SetWindowRgn(hRgn,true);
    }

	//GET_FILENAME(path,"%s\\SndPlay.bmp",skinfolder);
	//m_btSndPlay.LoadButtonBitmap(path,false);

	//GET_FILENAME(path,"%s\\SndPause.bmp",skinfolder);
	//m_btSndPause.LoadButtonBitmap(path,false);

	//GET_FILENAME(path,"%s\\SndVolUp.bmp",skinfolder);
	//m_btVolUp.LoadButtonBitmap(path,false);

	//GET_FILENAME(path,"%s\\SndVolDown.bmp",skinfolder);
	//m_btVolDown.LoadButtonBitmap(path,false);


	sndPlayX=f.GetKeyVal(key,"SndPlayX",100);
	sndPlayY=f.GetKeyVal(key,"SndPlayY",-200);

	sndPauseX=f.GetKeyVal(key,"SndPauseX",100);
	sndPauseY=f.GetKeyVal(key,"SndPauseY",-200);

	sndVolDownX=f.GetKeyVal(key,"SndVolDownX",100);
	sndVolDownY=f.GetKeyVal(key,"SndVolDownY",-200);

	sndVolUpX=f.GetKeyVal(key,"SndVolUpX",100);
	sndVolUpY=f.GetKeyVal(key,"SndVolUpY",-200);


	//if(m_Btbt5.m_hWnd)m_Btbt5.ShowWindow(SW_HIDE);
	//if(m_Btbt6.m_hWnd)m_Btbt6.ShowWindow(SW_HIDE);
	//if(m_Btbt7.m_hWnd)m_Btbt7.ShowWindow(SW_HIDE);
	//if(m_Btbt8.m_hWnd)m_Btbt8.ShowWindow(SW_HIDE);
	//if(m_Btbt9.m_hWnd)m_Btbt9.ShowWindow(SW_HIDE);
	if(m_Btbt10.m_hWnd)m_Btbt10.ShowWindow(SW_HIDE);

	m_MessageHandle.SetColor(m_colorresource);

	char wpath[MAX_PATH];
	//sprintf(wpath,"..\\image\\client\\%s\\game_signal.bmp",Glb().m_skin);
	GET_FILENAME(wpath,"%s\\game_signal.bmp",skinfolder);

	//AfxMessageBox(CW2A(wpath));
#ifndef BZ_ZLIB
	pSingnalimg = Image::FromFile(CA2W(wpath));
#else
	CUnZipRes unZipRes;
	IStream * pStream=NULL;
	//char * pchar=s.GetBuffer(s.GetLength()+1);
	pStream=unZipRes.LoadFileResFromZip(wpath);
	//DebugPrintf("Load zip %s",wpath);
	pSingnalimg=new Image(pStream);
	if(pStream!=NULL)
	{
		pStream->Release();
		pStream=NULL;
	}
	
#endif
	
	//�ȴ�����ͼ yjj 090227

	// ----- ����ָ����Ϸ�������ļ� -------- dxh
	char GameCode[20];
	::memset(GameCode,0,sizeof(GameCode));
	GetGameCode(GameCode);
	//if (str == "11901800"
	//	|| str == "10901800"
	//	|| str == "30501800")
	TCHAR szKey[10];
	CString sPath=CBcfFile::GetAppPath();
	CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
	wsprintf(szKey, "%d", m_pGameInfo->uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

	m_UserListDlg.m_UserList.SetIniFile(m_iniFile, TRUE);
	if (iResult>0)
	{
		m_btMax.ShowWindow(SW_HIDE);
	}
	// ----- ����ָ����Ϸ�������ļ� --------

    // PengJiLin, 2010-10-13, ������ǰ���ťͼ
    {
        CGameImage	img;
        HRGN	hRgn;
        CString s=CINIFile::GetAppPath ();/////����·��

        CString skinHall = CBcfFile::GetAppPath()+"..\\" + m_skinmgr.GetSkinPath();
        CBcfFile f( m_iniFile);

        CString key=TEXT("config");
        TCHAR path[MAX_PATH];
        CString skinfolder;
        skinfolder=f.GetKeyVal(key,"skinfolder","image\\game\\");
        TCHAR	szPropImagePath[MAX_PATH];
        wsprintf(szPropImagePath,TEXT("%s\\prop\\"),skinfolder);
		CString strGameSkinPath = CBcfFile::GetAppPath() + "config\\" + m_skinmgr.GetGameSkinBcfFileName();
       

		if (!CBcfFile::IsFileExist(strGameSkinPath))
        {
            wsprintf(szPropImagePath, TEXT("%s\\game\\prop\\"), skinHall );
        }

		CRect rect;
		GetClientRect(&rect);
		FixControlStation(rect.Width(),rect.Height());
		m_pGameView->UpdateViewFace(NULL);
        //////////////////////////////////////////////////////////////////////////
        //2008-08-11
        //������ǰ�����ť��ͼƬ
        m_curPropPage=0;
        TCHAR szTemp[MAX_PATH];

		m_Btbt6.LoadButtonBitmap(szTemp, false);
        img.SetLoadInfo(szTemp, true);
        hRgn =AFCBmpToRgn(img, RGB(255, 0, 255), RGB(1,1,1));
        if( hRgn )
            m_Btbt6.SetWindowRgn(hRgn, true);
        m_Btbt6.EnableWindow(FALSE);	//��ʼ��ʱǰ��ҳ��ť��Ч


        m_Btbt7.LoadButtonBitmap(szTemp, false);
        img.SetLoadInfo(szTemp, true);
        hRgn =AFCBmpToRgn(img, RGB(255, 0, 255), RGB(1,1,1));
        if( hRgn )
            m_Btbt7.SetWindowRgn(hRgn, true);
        m_Btbt7.EnableWindow(FALSE);	//��ʼ��ʱǰ��ҳ��ť��Ч

#ifdef _USE_PROP_PAGE_SW_
        wsprintf(szTemp, TEXT("%sPropPrev.bmp"), szPropImagePath);
        m_BtPropGoAhead.LoadButtonBitmap(szTemp, false);
        img.SetLoadInfo(szTemp, true);
        hRgn =AFCBmpToRgn(img, RGB(255, 0, 255), RGB(1,1,1));
        if( hRgn )
            m_BtPropGoAhead.SetWindowRgn(hRgn, true);
        m_BtPropGoAhead.EnableWindow(FALSE);	//��ʼ��ʱǰ��ҳ��ť��Ч

        wsprintf(szTemp, TEXT("%sPropNext.bmp"), szPropImagePath);
        m_BtPropGoNext.LoadButtonBitmap(szTemp, false);
        img.SetLoadInfo(szTemp, true);
        hRgn =AFCBmpToRgn(img, RGB(255, 0, 255), RGB(1,1,1));
        if( hRgn )
            m_BtPropGoNext.SetWindowRgn(hRgn, true);
        wsprintf(m_szPropEmptyImage, TEXT("%sPropEmpty.bmp"), szPropImagePath);
#endif
	}
}

LRESULT CLoveSendClassInExe::OnLoadSkin(WPARAM wparam, LPARAM lparam)
{
	//�˺�����ʱ��������
	LoadSkin();

	//m_UserListDlg.m_UserList.LoadSkin();

	CRect ClientRect;
	GetClientRect(&ClientRect);
	int cx,cy;
	cx=ClientRect.Width();
	cy=ClientRect.Height();
	OnSize(0,cx,cy);
	return true;
}
LRESULT CLoveSendClassInExe::OnReLoadTalkINI(WPARAM wparam, LPARAM lparam)
{
	//���ԭ������
	m_InputMessage.ResetContent();
	//��talkInGame.ini�ļ��ж�ȡ������
	//modify by wlr 20090716
	//CString s="..\\"; CINIFile::GetAppPath ();/////����·��
	//CBcfFile f( s + "talkInGame.bcf");
	CBcfFile f("talkInGame.bcf");
	//end of modify by wlr 20090716

	CString key="BZW_TALK";
	//	TCHAR path[MAX_PATH];
	CString szTalk;
	CString szNumber;
	m_nTalkDefaultCount=0;
	do
	{
		szNumber.Format("%d",++m_nTalkDefaultCount);
		szTalk=f.GetKeyVal(key,"T"+szNumber,"");
		if(!szTalk.IsEmpty())
			m_InputMessage.AddString(szTalk);
	}while(!szTalk.IsEmpty());
	m_nTalkDefaultCount--;
	return 0;
}
//���±���
void CLoveSendClassInExe::UpdateGameTitle()
{
	if (m_pGameInfo!=NULL)
	{
		CString strBuffer;

		//strBuffer.Format(TEXT("%s %d.%02d"),m_pGameInfo->szGameName,HIWORD(m_pGameInfo->dwGameMSVer),LOWORD(m_pGameInfo->dwGameLSVer));
		strBuffer.Format(TEXT("%s %d.%02d"),m_szGameRoomName,HIWORD(m_pGameInfo->dwGameMSVer),LOWORD(m_pGameInfo->dwGameLSVer));
		
		if (m_pGameInfo->uisMeUserInfo.GameUserInfo.bDeskNO!=255)
		{
			CString strDeskStation;
			// PengJiLin, 2010-5-19, �������Ŷӻ����������Ӻ�
			if(IsQueueGameRoom() || m_pGameInfo->uComType ==TY_MATCH_GAME ) 
				strDeskStation = TEXT(" - ĳ��Ϸ��");
			else
				strDeskStation.Format(TEXT(" - [%ld]����Ϸ��"),m_pGameInfo->uisMeUserInfo.GameUserInfo.bDeskNO+1);
			strBuffer+=strDeskStation;
		}
		SetWindowTitle(strBuffer);
		m_pGameView->SetViewTitle(strBuffer);
	}
	return;
}
/// ����ȫ�ֱ����еļ��ܷ�ʽ��ȡ�ü��ܺ�������ַ���������sha������MD5
/// �����ﲻ��ָ��ĺϷ������ж�
/// @param szMD5Pass ���ܺ���ַ���
/// @param szSrcPass Դ�ַ���
/// @return ���ܺ���ַ���ָ��
TCHAR *CLoveSendClassInExe::GetCryptedPasswd(TCHAR *szMD5Pass, TCHAR *szSrcPass)
{
	CenterServerMsg& par=Glb().m_CenterServerPara;
	//md5��ʽ
	if( par.m_nEncryptType == 1)
	{
		unsigned char szMDTemp[16];
		MD5_CTX Md5;
		Md5.MD5Update((unsigned char *)szSrcPass,lstrlen(szSrcPass));
		Md5.MD5Final(szMDTemp);
		for (int i = 0; i < 16; i ++) 
			wsprintf(&szMD5Pass[i * 2], "%02x", szMDTemp[i] );
	}
	//sha���ܷ�ʽ
	if(par.m_nEncryptType == 2)
	{
		unsigned szMDTemp[5]={0};
		CString strSha=szSrcPass;
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
	return szMD5Pass;
}
/// ���н��淢��ָ�����ת�ʼ�¼
/// @return ��
void CLoveSendClassInExe::OnTransferRecord()
{
	MSG_GR_S_TransferRecord_t transferRecord;
	transferRecord.dwUserID = GetMeUserInfo()->dwUserID;
	/// ֻ���û�ID�Ĵ���������Ϣ
	SendGameData(&transferRecord, sizeof(transferRecord), MDM_GR_MONEY, ASS_GR_TRANSFER_RECORD_INGAME, 0);
}
/// ���н��淢��ָ��޸�����
/// @param pChangePasswd �޸�����ṹָ�룬�ں���ִ�н�����Ҫɾ����ָ��
/// @return ��
void CLoveSendClassInExe::OnChangePasswd( ChangePasswd_t *pChangePasswd )
{
	MSG_GR_S_ChangePasswd_t changePasswd;
	changePasswd.UserID = GetMeUserInfo()->dwUserID;
	GetCryptedPasswd(changePasswd.szMD5PassOld, pChangePasswd->szPwdOld);
	GetCryptedPasswd(changePasswd.szMD5PassNew, pChangePasswd->szPwdNew);

	SendGameData(&changePasswd, sizeof(changePasswd), MDM_GR_MONEY, ASS_GR_CHANGE_PASSWD_INGAME, 0);
	delete pChangePasswd;
	pChangePasswd = NULL;
}
/// ���н��淢��ָ�ת��
/// @param pTransferMoney ��ȡǮ�ṹָ�룬�ں���ִ�н�����Ҫɾ����ָ��
/// @return ��
void CLoveSendClassInExe::OnTransferMoney( TransferMoney_t *pTransferMoney )
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"..\\ClientMessage.bcf");
	MSG_GR_S_TransferMoney transferMoney;
	transferMoney.i64Money = pTransferMoney->i64Money;
	transferMoney.destUserID = pTransferMoney->uDestUserID;
	transferMoney.UserID = GetMeUserInfo()->dwUserID;
	transferMoney.bUseDestID = pTransferMoney->bUseDestID;
	strcpy(transferMoney.szNickName,GetMeUserInfo()->nickName);		//add by lxl ת����ʾ��Ϣ��ʾ�ǳ�
	strcpy(transferMoney.szDestNickName, pTransferMoney->szDestNickName);
	if (transferMoney.UserID == transferMoney.destUserID)
	{
		delete pTransferMoney;
		pTransferMoney = NULL;
		CString str = fMsg.GetKeyVal("BankDlg","TransferToSelf","������ת�ʸ��Լ����Բ���ϵͳ�ݲ��ṩ�������");
		TCHAR szMsg[MAX_PATH];
		wsprintf(szMsg, str.GetBuffer());
		AFCMessageBox(szMsg);
		return;
	}
	GetCryptedPasswd(transferMoney.szMD5Pass, pTransferMoney->szPWD);

	SendGameData(&transferMoney,sizeof(transferMoney),MDM_GR_MONEY,ASS_GR_TRANSFER_MONEY_INGAME,0);
	delete pTransferMoney;
	pTransferMoney = NULL;
}
/// @param pCheckMoney ��ȡǮ�ṹָ�룬�ں���ִ�н�����Ҫɾ����ָ��
/// @return ��
void CLoveSendClassInExe::OnCheckMoney( CheckMoney_t *pCheckMoney )
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"..\\ClientMessage.bcf");
	/// �ж��Ǵ滹��ȡ
	if (pCheckMoney->uType==0) /// ��Ǯ������Ҫ����
	{
		__int64 i64Left = GetMeUserInfo()->i64Money - pCheckMoney->i64Money;
		if (i64Left < 0 || i64Left < m_pGameInfo->uLessPoint)
		{
			CString strBuffer;
			TCHAR szNum[128];
			GlbGetNumString(szNum, m_pGameInfo->uLessPoint, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
			CString str = fMsg.GetKeyVal("GameRoom","NeedCoins","�˷�����Ҫ�������� %s �Ľ��");
			strBuffer.Format(str, szNum);
			AFCMessageBox(strBuffer,m_pGameInfo->szGameName);
			/// �ǵ�ɾ��
			delete pCheckMoney;
			pCheckMoney = NULL;
			return;
		}
		MSG_GR_S_CheckMoney cm;
		cm.i64Count = pCheckMoney->i64Money;
		cm.UserID = GetMeUserInfo()->dwUserID;
		SendGameData(&cm,sizeof(cm),MDM_GR_MONEY,ASS_GR_CHECK_IN_INGAME,0);
	}
	else ///< ȡǮ����Ҫ����
	{
		MSG_GR_S_CheckMoneyWithPwd_t cm;
		cm.i64Count = pCheckMoney->i64Money;
		cm.UserID = GetMeUserInfo()->dwUserID;
		GetCryptedPasswd(cm.szMD5Pass, pCheckMoney->szPWD);
		SendGameData(&cm,sizeof(cm),MDM_GR_MONEY,ASS_GR_CHECK_OUT_INGAME,0);
	}
	/// �ǵ�ɾ��
	delete pCheckMoney;
	pCheckMoney = NULL;
}

/// ��2�����еĴ�ȡ��
/// @param pCheckMoney ��ȡǮ�ṹָ
/// @return ��
void CLoveSendClassInExe::OnCheckMoney2(bank_op_normal* pCheckMoney )
{
	SendGameData(pCheckMoney,sizeof(bank_op_normal),MDM_BANK,ASS_BANK_NORMAL,0);
	return;
}

// lxl, 2010-11-17, �����û�ID��ȡ�ǳ�
void CLoveSendClassInExe::OnGetNickNameOnID(GetNickNameOnID_t* pGetNickNameOnID)
{
	//OutputDebugString("������Ϣ��ExeForGame lxl");
    MSG_GR_S_GetNickNameOnID_t stGetNickNameOnID;
    stGetNickNameOnID.iUserID = pGetNickNameOnID->uUserID;

    SendGameData(&stGetNickNameOnID, sizeof(stGetNickNameOnID), MDM_GR_ROOM, ASS_GR_GET_NICKNAME_ONID_INGAME, 0);
    delete pGetNickNameOnID;
    pGetNickNameOnID = NULL;
}

//��Ϣ���ͺ���
BOOL CLoveSendClassInExe::PreTranslateMessage(MSG * pMsg)
{
	/// ���н��淢�����޸�����
	if (pMsg->message == WM_CHANGE_PASSWD)
	{
		OnChangePasswd( (ChangePasswd_t *)(pMsg->lParam) );
		return true;
	}

	/// ���н��淢����ת�ʼ�¼����
	if (pMsg->message == WM_TRANSFER_RECORD)
	{
		OnTransferRecord();
		return true;
	}	/// ���н��淢����ת����Ϣ
	if (pMsg->message == WM_TRANSFER_MONEY)
	{
		OnTransferMoney( (TransferMoney_t *)(pMsg->lParam) );
		return true;
	}

	/// ���н��淢���Ĵ�ȡǮ��Ϣ
// 	if (pMsg->message == WM_CHECK_MONEY)
// 	{
// 		OnCheckMoney( (CheckMoney_t *)(pMsg->lParam) );
// 		return true;
// 	}

	//�°����д�ȡ��
// 	if (pMsg->message == WM_CHECK_MONEY2)
// 	{
// 		OnCheckMoney2((bank_op_normal*)(pMsg->lParam));
// 		return true;
// 	}

	//JianGuankun 2012-1-2,�ر����д���ʱʹ��
// 	if (pMsg->message == WM_BANK2_CLOSE)
// 	{
// 		OnCloseBank2();
// 		return true;
// 	}
    
	// lxl, 2010-11-17, ���н��淢���Ļ�ȡ�û��ǳƵ���Ϣ
    if(WM_BANK_GET_NICKNAME_ONID == pMsg->message)
    {
        OnGetNickNameOnID((GetNickNameOnID_t*)(pMsg->lParam));
        return true;
    }
	
	if(pMsg->message==WM_SYSCOMMAND && pMsg->wParam==SC_RESTORE)
	{
		if(pMsg->wParam==SC_MAXIMIZE)
		{
			//�ټ��� modify by wlr 20090716
			/*m_bMax=true;
			CRect rect;
			::SystemParametersInfo(SPI_GETWORKAREA,NULL,&rect,NULL);
			MoveWindow(0,0,rect.Width(),rect.Height());*/
			OnBnClickedMax();//add by wlr 20090716
			//�ټ��� end of modify by wlr 20090716

			return TRUE;
		}
		if(pMsg->wParam==SC_RESTORE)
		{
			ShowWindow(SW_RESTORE);
			if(m_bShowMax)
			{
				//�ټ��� modify by wlr 20090716
				/*m_bMax=true;
				CRect rect;
				::SystemParametersInfo(SPI_GETWORKAREA,NULL,&rect,NULL);
				MoveWindow(0,0,rect.Width(),rect.Height());*/
				//�ټ��� end of modify by wlr 20090716
				OnBnClickedMax();//add by wlr 20090716
			}
			else
			{
				m_bMax=false;

				MoveWindow(0,0,m_SavedScreen.x,m_SavedScreen.y);
				CenterWindow();
			}
			SetForegroundWindow();
			return TRUE;
		}
	}
	//����ȡ����Ϣ�ͻس���Ϣ
	if(pMsg->message==UM_USEITEM_USERSELECTED)
	{
		for(int i=0; i<MAX_PEOPLE; i++)
		{
			if( m_pUserInfo[i] && m_pUserInfo[i]->GameUserInfo.dwUserID == pMsg->lParam )
			{
				int nRet = OnUseProp((int)pMsg->wParam, m_pUserInfo[i]->GameUserInfo.dwUserID);
				break;
			}
		}
		return TRUE;
	}
	if (pMsg->message==WM_KEYDOWN)
	{
		if (pMsg->wParam==VK_RETURN)
		{	
			SendTalkMessage();
			return TRUE;
		}
		else if (pMsg->wParam == VK_F1)
		{
			return TRUE;
		}
		else 
			if (pMsg->wParam==VK_ESCAPE) 
				return TRUE;
	}
#ifdef _USE_PROP_PAGE_SW_
	//{add by HuangYin 2008/04/07
	if(pMsg->message==WM_MOUSEMOVE)
	{
		if(this->GetFocus()==(CWnd*)&m_BtPropGoAhead || 
			this->GetFocus()==(CWnd*)&m_BtPropGoNext)
			this->SetFocus();
	}
#endif
	//wushuqun 2009.6.9
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (m_pGameInfo)
		{
			if (m_pGameInfo->bIsInRecord)
			{
				return true;
			}
		}
	}
	////�ټ��� ��󻯲����϶� del by wlr 20090717
	////wushuqun 2009.6.10
	////��󻯺����϶�
	//if((WM_NCLBUTTONDOWN == pMsg->message)
	//&& (HTCAPTION == pMsg->wParam)
	//&& (m_hWnd == pMsg->hwnd) && m_bMax)  // ���Լ�������Ϣ
	//{
	//	//�ټ��� add by wlr 20090717
	//	char GameCode[20];
	//	::memset(GameCode,0,sizeof(GameCode));
	//	GetGameCode(GameCode);
	//	CString str = GameCode;
	//	if (str != "10301800" &&
	//		str != "30501800" &&
	//		str != "10901800" &&
	//		str != "11901800" )
	//	{
	//		return TRUE;
	//	}
	//	//end of �ټ��� add by wlr 20090717
	//}
	////end of �ټ��� ��󻯲����϶� del by wlr 20090717

	if (m_pGameView->m_bShowRight)
	{
		m_ToolTip.RelayEvent(pMsg);
	}
	return __super::PreTranslateMessage(pMsg);
}

void CLoveSendClassInExe::SendTalkMessage(int nDefaultIndex)
{
	//�ж�״̬
	if ((GetMeUserInfo()->bDeskNO==255)
		||(GetMeUserInfo()->bDeskStation==255)) 
		return;

	//�ж���Ϣ
	if (m_pRightFrame)
	{
		m_pRightFrame->SetFocusToTalkInput();
		if (m_pRightFrame->GetTalkInputMessage(NULL) == 0) return;
	}
	else
	{
		m_InputMessage.SetFocus();
		if (m_InputMessage.GetWindowTextLength()==0) return;
	}
	

	try
	{
		//��ֹ��Ϸ����
		if (m_pGameInfo->dwRoomRule & GRR_FORBID_GAME_TALK)
			throw TEXT("��Ǹ,����Ϸ��ֹ����!");
		//�ж��Ƿ��������
		if (CUserPower::CanGameTalk(m_pGameInfo->dwGamePower)==false) 
			throw TEXT("��Ǹ������ʱû�������Ȩ��!");

		//��ȡ��Ϣ
		CString strBuffer;
		if (m_pRightFrame)
		{
			char szMsgBuf[MAX_TALK_LEN];
 			m_pRightFrame->GetTalkInputMessage(szMsgBuf);
			m_pRightFrame->ClearTalkInput();
			m_pRightFrame->SetFocusToTalkInput();
			strBuffer = szMsgBuf;
		}
		else
		{
			m_InputMessage.GetWindowText(strBuffer);
		}
		
		////�ж�Ŀ�����
		long int dwTargerID=0;

		if(m_CurpUserInfoClicked)
		{
			CString nick=m_CurpUserInfoClicked->nickName;
			nick+=":";
			int l=nick.GetLength();
			if(strBuffer.Left(l)==nick)
			{
				dwTargerID=m_CurpUserInfoClicked->dwUserID;
				strBuffer=strBuffer.Mid(l);
			}
		}

		if(strBuffer=="")
			return;

		//�ж���Ϣ����
		int iMaxSendLen=CUserPower::CanSendMuch(m_pGameInfo->dwGamePower)?MAX_TALK_LEN:NORMAL_TALK_LEN;
		if (strBuffer.GetLength()>iMaxSendLen)
			throw TEXT("���ڷ��͵���Ϣ̫��������ʧ�ܣ�");

		//������Ϣ
		if (strBuffer.Find(TEXT("{\\rtf"))!=-1) 
			throw TEXT("���ܷ��ͷǷ��ַ�������ʧ�ܣ�");

		for (int i=strBuffer.GetLength();i>0;i--)
			if ((strBuffer[i-1]>=-1)&&(strBuffer[i-1]<30))
				throw TEXT("���ܷ��ͷǷ��ַ�������ʧ�ܣ�");

		//�ж��ٶ�
		if (CUserPower::CanTalkFast(m_pGameInfo->dwGamePower)==false)
		{
			long int dwNowTime=(long int)time(NULL);
			if ((CUserPower::CanTalkFast(m_pGameInfo->dwGamePower)==false)&&((m_dwLastTalkTime+1L)>dwNowTime))
				throw TEXT("�밮�����컷�����벻ҪƵ��������Ϣ��");
			if (((m_dwLastTalkTime+3L)>dwNowTime)&&(m_strLastTalk.Compare(strBuffer)==0)) 
				throw TEXT("�밮�����컷�����벻Ҫ�ظ����ԣ�");
		}

		//����������Ϣ
		MSG_GR_RS_NormalTalk Talk;
		ZeroMemory(&Talk,sizeof(MSG_GR_RS_NormalTalk));
		Talk.iLength=strBuffer.GetLength();
		Talk.dwSendID=GetMeUserInfo()->dwUserID;
		Talk.dwTargetID=dwTargerID;
		Talk.nDefaultIndex=nDefaultIndex;
		lstrcpy(Talk.szMessage,strBuffer);
		Talk.crColor=m_colorresource.m_crNormalTxColor;

		SendGameData(&Talk,sizeof(Talk)-sizeof(Talk.szMessage)+Talk.iLength+1,MDM_GM_GAME_FRAME,ASS_GM_NORMAL_TALK,0);

		//������Ϣ
		m_strLastTalk=Talk.szMessage;
		if (m_InputMessage.FindString(0,m_strLastTalk)==-1) 
			m_InputMessage.InsertString(0,m_strLastTalk);

		if (m_InputMessage.GetCount()>40) 
			m_InputMessage.DeleteString(40-m_nTalkDefaultCount);

		//���ò���
		m_dwLastTalkTime=(ULONG)time(NULL);
		m_InputMessage.SetWindowText(TEXT(""));
	}
	catch (TCHAR * szErrorMessage) 
	{
		m_MessageHandle.InsertSystemMessage(szErrorMessage);
	}
	catch (...) 
	{ 
		m_MessageHandle.InsertSystemMessage(TEXT("��Ϣ����ʧ�ܣ�"));
	}
	::SendMessage(m_TalkMessage.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);

	return;
}
BOOL CLoveSendClassInExe::OnToolTipNotify(UINT id, NMHDR * pTTTStruct, LRESULT * pResult )
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pTTTStruct;    
	UINT nID =pTTTStruct->idFrom;			//�õ���Ӧ����ID���п�����HWND 
	CString stip="����������";
	int curPropIndex=m_curPropPage*PEER_PAGE;
    int curPropIndex_S=m_curPropPage_S*PEER_PAGE;   // PengJiLin, 2010-10-12
    BOOL bIsNormalProp = TRUE;     // ��ͨ����
	_TAG_USERPROP *userProp=NULL;
	int propCount=Glb().userPropLibrary.GetCount();
	if (pTTT->uFlags & TTF_IDISHWND)		//����nID�Ƿ�ΪHWND 
	{ 
		nID = ::GetDlgCtrlID((HWND)nID);	//��HWND�õ�IDֵ����Ȼ��Ҳ����ͨ��HWNDֵ���ж� 
		switch(nID) 
		{	
		case IDC_BUTTON_PROP1:
			break;
		case IDC_BUTTON_PROP2:
			curPropIndex+=0;
			break;
		case IDC_BUTTON_PROP3:
			curPropIndex+=1;
			break;
		case IDC_BUTTON_PROP4:
			curPropIndex+=2;
			break;
		case IDC_BUTTON_PROP5:
			curPropIndex+=3;
			break;
		case IDC_BUTTON_PROP6:
			curPropIndex+=4;
			break;
		case IDC_BUTTON_PROP7:
			curPropIndex+=5;
			break;
		case IDC_BUTTON_PROP8:
			curPropIndex+=6;
			break;
		case IDC_BUTTON_PROP9:
			curPropIndex+=7;
			break;
		case IDC_BUTTON_PROP10:
			curPropIndex+=8;
			break;
		/*case IDC_BUTTON_PROP6:
            bIsNormalProp = FALSE;
			curPropIndex_S+=2;
			break;
        case IDC_BUTTON_PROP4:
            bIsNormalProp = FALSE;
            curPropIndex_S+=0;
            break;
        case IDC_BUTTON_PROP5:
            bIsNormalProp = FALSE;
            curPropIndex_S+=1;*/
            break;
		default:
			break;
		}
		if(curPropIndex<propCount && TRUE == bIsNormalProp)
		{
			userProp=Glb().userPropLibrary.GetAt(curPropIndex);
			stip.Format("%s [%d]",userProp->szPropName,userProp->nHoldCount);
		}
        else if(curPropIndex_S < propCount && FALSE == bIsNormalProp)
        {
            userProp=Glb().userPropLibrary.GetAt(curPropIndex_S);
            stip.Format("%s",userProp->szPropName);
        }

		stip.Trim();
		lstrcpy(pTTT->lpszText, stip);	//������Ӧ����ʾ�ִ� 
	} 
	return FALSE; 
}
BOOL CLoveSendClassInExe::OnUseProp(int nPropIndex, int nDeskUserID)
{
	//////////////////////////////////////////////////////////////////////////
	//2008-08-11
	if(nPropIndex<0 || nPropIndex>=Glb().userPropLibrary.GetCount())
		return FALSE;

	_TAG_USERPROP * userProp=Glb().userPropLibrary.GetAt(nPropIndex);
	if(userProp->nHoldCount<=0)
		return FALSE;

	_TAG_USINGPROP  usingProp;
	ZeroMemory(&usingProp,sizeof(_TAG_USINGPROP));
	usingProp.dwUserID=GetMeUserInfo()->dwUserID;
	usingProp.dwTargetUserID=nDeskUserID;
	usingProp.nPropID=userProp->nPropID;
	usingProp.nPropActionAttrib=userProp->attribAction;
	usingProp.nPropValueAttrib=userProp->attribValue;

	memcpy(usingProp.szPropName,userProp->szPropName,sizeof(userProp->szPropName));
	SendGameData(&usingProp,sizeof(_TAG_USINGPROP),MDM_GR_PROP,ASS_PROP_USEPROP,0);

	// ���ؿ۳���Ӧ��������
	//if(userProp->nHoldCount <=0)
	//{
	//	Glb().userPropLibrary.RemoveAt(nPropIndex);
	//	delete userProp;
	//	userProp=NULL;
	//}
	return TRUE;
}
// ���������б�
void CLoveSendClassInExe::LoadSoundList(void)
{
	TCHAR sz[61],szPath[MAX_PATH];
	lstrcpy(sz,m_pGameInfo->szProcessName);
	sz[lstrlen(sz)-lstrlen(".ico")]='\0';
	wsprintf(szPath,"GameOption.bcf");
	//AfxMessageBox(szPath);
	//DebugPrintf("szPath = [%s]", szPath);
	CBcfFile f(szPath);
	int		soundCount=f.GetKeyVal("BGSOUND","SoundCount",0);
	//DebugPrintf("SoundCount = [%d]", soundCount);
	if(soundCount==0)
		return;

	m_sndListID=BZSoundCreateList(this,0);
	if(m_sndListID==0)
		return;
	CString soundFile;
	for(int i=0;i<soundCount;i++)
	{
		wsprintf(sz,"Sound%d",i);
		soundFile=f.GetKeyVal("BGSOUND",sz,"");
		wsprintf(szPath,"%s",soundFile);
		//DebugPrintf("szPath = [%s]", szPath);
		BZSoundAddList(m_sndListID,szPath);
	}
	int nVol=f.GetKeyVal("BGSOUND","SoundVolume",300);
	BZSoundSetVolume(m_sndListID,nVol);
	m_bSndPause=f.GetKeyVal("BGSOUND","PauseSound",0);
	if(m_bSndPause)
	{
		BZSoundPause(m_sndListID);
		//m_btSndPlay.ShowWindow(SW_SHOW);
		//m_btSndPause.ShowWindow(SW_HIDE);
	}
	else
	{
		BZSoundPlayList(m_sndListID);
		//m_btSndPlay.ShowWindow(SW_HIDE);
		//m_btSndPause.ShowWindow(SW_SHOW);
	}
	if(AfxGetApp()->m_pMainWnd)
		AfxGetApp()->m_pMainWnd->PostMessage(WM_USER+100,2,0);
}

//����λ��
void CLoveSendClassInExe::FixControlStation(int iWidth, int iHeight)
{
	if(!m_bInit)
		return ;
	if(!m_bMax)
	{
		CRect rcRect;
		GetWindowRect(rcRect);
		if(rcRect.Width()>320 && rcRect.Height()>200)
		{
			m_SavedScreen.x=rcRect.Width();
			m_SavedScreen.y=rcRect.Height();
		}
	}

	int iWindowsWidth = GetSystemMetrics(SM_CXSCREEN); 
	int iWindowsHeight = GetSystemMetrics(SM_CYSCREEN); 

	if (m_pRightFrame)
	{
		if (m_bShowMax)
		{
			if (iWindowsWidth >= 1280)
			{
				m_vlistx = m_pRightFrame->m_uMaxWidth;
			}		
			else
			{
				m_vlistx = m_pRightFrame->m_uMinWidth;
			}
		}
		else
		{
			m_vlistx = m_pRightFrame->m_uMinWidth;
		}
		m_pGameView->m_vlistx = m_vlistx;
	}

	CString strBcfFilename = m_skinmgr.GetGameSkinBcfFileName();
	if (CBcfFile::IsFileExist(CBcfFile::GetAppPath() + "config\\" + strBcfFilename))
		m_iniFile= CBcfFile::GetAppPath() + "config\\" + strBcfFilename;
	else
		m_iniFile= CBcfFile::GetAppPath()+"..\\config\\" + strBcfFilename;

	CString s=CINIFile::GetAppPath ();
	CBcfFile f( m_iniFile);
	//AfxMessageBox(m_iniFile);
	CString key=TEXT("config");
	bool showright=m_pGameView->m_bShowRight;
	//���λ
	int m_AdvX=-100;
	int	m_AdvY=-100;
	int m_AdvWidth=10;
	int m_AdvHeight=f.GetKeyVal(key,"advh",0);

    // PengJiLin, 2010-9-28, ˢ�¿ؼ�
#ifdef _USE_IE_AD_
    m_ie.Invalidate(FALSE);

	if(m_ie.m_hWnd && szAdvUrl!="" && showright)//ֻ����IE��Ч�͵�ַ��Ч������²���Ч
	{
		if(m_AdvHeight>0)
		{
			m_AdvX=f.GetKeyVal(key,"advx",-100);
			m_AdvY=f.GetKeyVal(key,"advy",-100);
			m_AdvWidth=f.GetKeyVal(key,"advw",10);
			m_ie.MoveWindow(iWidth-m_AdvX,m_AdvY,m_AdvWidth,m_AdvHeight);
		}
		else
		{
			m_ie.MoveWindow(-100,-100,0,0);
			//m_AdvHeight=0;
		}
	}
	else
	{
		m_ie.MoveWindow(-100,-100,0,0);
		//m_AdvHeight=0;
	}
#endif

	int x,y,w,h;
	CRect rect;
	// ��󻯵�����²�������
	if (showright)
	{

        x=f.GetKeyVal(key,"bluediamondx",190);
        y=f.GetKeyVal(key,"bluediamondy",368);
        m_Btbt5.SetWindowPos(NULL, iWidth-x, y , 0, 0, SWP_NOSIZE|SWP_NOZORDER);

		x=f.GetKeyVal(key,"ulistx",0);
		y=f.GetKeyVal(key,"ulisty",0);
		w=f.GetKeyVal(key,"ulistw",0);
		h=f.GetKeyVal(key,"ulisth",0);

		CGameImageHelper helprui(&m_UserInfo_bk);
		CGameImageHelper helprbc(&m_right_bc);
		CGameImageHelper helprrt(&m_right_rt);
		CGameImageHelper helprtc(&m_right_tc);
		CGameImageHelper helprt(&m_rt);
		CGameImageHelper helprb(&m_rb);

		//�û��б�
		y = helprt.GetHeight()+helprtc.GetHeight()+helprui.GetHeight();
		m_UserListRect.top = y;
		m_UserListRect.left = iWidth-x;
		m_UserListRect.right = m_UserListRect.left + w;
		m_UserListRect.bottom = m_UserListRect.top + h;
		SafeMoveWindow(	&m_UserListDlg, iWidth-x,y,w,h);
		m_userlistW = w;
		m_userlistH = h;


		// PengJiLin, 2010-9-28, ���߱���ͼλ��
		x=f.GetKeyVal(key,"propbackx",0);
		y=f.GetKeyVal(key,"propbacky",0);
		w=f.GetKeyVal(key,"propbackw",0);
		h=f.GetKeyVal(key,"propbackh",0);
		CRect rc;
		m_UserListDlg.GetWindowRect(&rc);
		y = helprt.GetHeight()+helprtc.GetHeight()+helprui.GetHeight()+rc.Height();
		m_propBackRect.left = iWidth - x;
		m_propBackRect.top = y;
		m_propBackRect.right = m_propBackRect.left + w;
		m_propBackRect.bottom = m_propBackRect.top + h;
		x=f.GetKeyVal(key,"propbackx2",0);
		y=f.GetKeyVal(key,"propbacky2",0);
		w=f.GetKeyVal(key,"propbackw2",0);
		h=f.GetKeyVal(key,"propbackh2",0);
		m_propBackRect2.left = iWidth - x;
		m_propBackRect2.top = y;
		m_propBackRect2.right = m_propBackRect2.left + w;
		m_propBackRect2.bottom = m_propBackRect2.top + h;


		x=f.GetKeyVal(key,"chatx",0);
		y=f.GetKeyVal(key,"chaty",0);
		w=f.GetKeyVal(key,"chatw",0);
		//h=f.GetKeyVal(key,"chath",0);
		//ע�⣺h�������߶�-�����ĸ߶ȣ�����Ҫͨ��iHeight-h��ȡ�������ʵ�ʵĸ߶�
		//���û��б��Ҳ���������ģ�����Ҫ��ȥ�û��б��ĸ߶�
		//Fred Huang 2008-05-29
		int chath=f.GetKeyVal(key,"chath",480);


		y = helprt.GetHeight()+helprtc.GetHeight()+helprui.GetHeight()+rc.Height()+m_propBackRect.Height();
		chath=iHeight - y - helprbc.GetHeight() - helprb.GetHeight();
		//�����
		SafeMoveWindow(	&m_TalkMessage, iWidth-x,y,w,chath);//223);	
		m_talkY = y;
		m_talkW = w;
		m_talkH = chath;
		//����
		//{modify by zxj 2008/04/08
		//x=f.GetKeyVal(key,"gamebt1x",188);
		x=f.GetKeyVal(key,"gamebt1x",190);
		//end modify}
		y=f.GetKeyVal(key,"gamebt1y",368);

		//{modify by zxj 2008/04/03
		//m_Btbt1.SetWindowPos(NULL,iWidth-x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
		//m_Btbt1.SetWindowPos(NULL,iWidth-x,y-10,0,0,SWP_NOSIZE|SWP_NOZORDER);
		m_Btbt1.MoveWindow(iWidth-x,y-10,18,18);
		//end modify}

		m_Btbt1.GetClientRect(&rect);
		m_iStatic1X = iWidth-x+10;
		m_iStatic1Y = y+rect.Height()+2;	

		//{add by zxj 2008/04/07
		//������岼�� modify by lxl 2010-12-27
		int itemWith = f.GetKeyVal(key,"itemwith",35); //���ߵļ��
		//��һ�ŵ���
#ifdef _USE_PROP_PAGE_SW_
		x=f.GetKeyVal(key,"itemprev1x",197);
		y=f.GetKeyVal(key,"itemprev1y",379);
		m_BtPropGoAhead.SetWindowPos(NULL, iWidth -x, y , 0, 0, SWP_NOSIZE|SWP_NOZORDER);	//��ǰ��ť
		x=f.GetKeyVal(key,"itemnext1x",30);
		m_BtPropGoNext.SetWindowPos(NULL, iWidth-x, y, 0, 0, SWP_NOSIZE|SWP_NOZORDER);		//���ť
#endif

// 		x=f.GetKeyVal(key,"itemrow1x",180);
// 		y=f.GetKeyVal(key,"itemrow1y",360);
		x = m_propBackRect.Width() - (m_propBackRect.Width()-itemWith*5)/2;
		y = m_propBackRect.top + (m_propBackRect.Height()-itemWith*2)/2;
		m_BtProp1.SetWindowPos(NULL, iWidth-x, y , 0, 0, SWP_NOSIZE|SWP_NOZORDER);
		x -= itemWith;
		m_BtProp2.SetWindowPos(NULL, iWidth-x, y , 0, 0, SWP_NOSIZE|SWP_NOZORDER);
		x -= itemWith;
		m_BtProp3.SetWindowPos(NULL, iWidth-x, y,  0, 0, SWP_NOSIZE|SWP_NOZORDER);
		x -= itemWith;
        m_BtProp4.SetWindowPos(NULL, iWidth-x, y , 0, 0, SWP_NOSIZE|SWP_NOZORDER);   
		x -= itemWith;
		m_BtProp5.SetWindowPos(NULL, iWidth-x, y , 0, 0, SWP_NOSIZE|SWP_NOZORDER);   
		
		//�ڶ��ŵ���
#ifdef _USE_PROP_PAGE_SW_
		x=f.GetKeyVal(key,"itemprev2x",197);
        y=f.GetKeyVal(key,"itemprev2y",383);
        m_Btbt6.SetWindowPos(NULL, iWidth -x, y , 0, 0, SWP_NOSIZE|SWP_NOZORDER);   //��ǰ��ť
        x=f.GetKeyVal(key,"itemnext2x",36);
        m_Btbt7.SetWindowPos(NULL, iWidth -x, y , 0, 0, SWP_NOSIZE|SWP_NOZORDER);   //���ť
#endif

// 		x=f.GetKeyVal(key,"itemrow2x",180);
//      y=f.GetKeyVal(key,"itemrow2y",395);
		x = m_propBackRect.Width() - (m_propBackRect.Width()-itemWith*5)/2;
		y = m_propBackRect.top + (m_propBackRect.Height()-itemWith*2)/2 + itemWith;
		m_BtProp6.SetWindowPos(NULL, iWidth-x, y, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
		x -= itemWith;
		m_BtProp7.SetWindowPos(NULL, iWidth-x, y, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
		x -= itemWith;
		m_BtProp8.SetWindowPos(NULL, iWidth-x, y, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
		x -= itemWith;
		m_BtProp9.SetWindowPos(NULL, iWidth-x, y, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
		x -= itemWith;
		m_BtProp10.SetWindowPos(NULL, iWidth-x, y, 0, 0, SWP_NOSIZE|SWP_NOZORDER);

		//end add}

		//{modify by zxj 2008/04/07
		//x=f.GetKeyVal(key,"gamebt2x",148);
		x=f.GetKeyVal(key,"gamebt2x",150);
		//end modify}
		y=f.GetKeyVal(key,"gamebt2y",368);

		//{modify by zxj 2008/04/03
		//m_Btbt2.SetWindowPos(NULL,iWidth-x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
		m_Btbt2.SetWindowPos(NULL,iWidth-x,y-10,0,0,SWP_NOSIZE|SWP_NOZORDER);
		//end modify}

		//m_Btbt2.GetClientRect(&rect);

		//{modify by zxj 2008/04/07
		//x=f.GetKeyVal(key,"gamebt3x",108);
		x=f.GetKeyVal(key,"gamebt3x",110);
		//end modify}
		y=f.GetKeyVal(key,"gamebt3y",368);

		//{modify by zxj 2008/04/03
		//m_Btbt3.SetWindowPos(NULL,iWidth-x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
		m_Btbt3.SetWindowPos(NULL,iWidth-x,y-10,0,0,SWP_NOSIZE|SWP_NOZORDER);
		//end modify}
		//m_Btbt3.GetClientRect(&rect);

		//{modify by zxj 2008/04/07 
		//x=f.GetKeyVal(key,"gamebt4x",68);
		x=f.GetKeyVal(key,"gamebt4x",70);
		//end modify}
		y=f.GetKeyVal(key,"gamebt4y",368);

		//{modify by zxj 2008/04/03
		//m_Btbt4.SetWindowPos(NULL,iWidth-x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
		m_Btbt4.SetWindowPos(NULL,iWidth-x,y-10,0,0,SWP_NOSIZE|SWP_NOZORDER);
		//end modify}


		//����ؼ�
		x=f.GetKeyVal(key,"expx",0);
		y=f.GetKeyVal(key,"expy",0);
		//SafeMoveWindow(&m_TargetPeople,x,iHeight-BUTTOM_HEIGHT+5,iWidth-x-50,200);	//Ŀ��
		//m_BtExpression.SetWindowPos(NULL,iWidth-x,iHeight-y,0,0,SWP_NOSIZE|SWP_NOZORDER);
		m_BtExpression.GetClientRect(&rect);
		m_BtExpression.MoveWindow(iWidth-x,iHeight-y,rect.Width(),rect.Height());

		//������ɫ��ť
		x=f.GetKeyVal(key,"setcolorx",0);
		y=f.GetKeyVal(key,"setcolory",0);
		m_BtSetColor.MoveWindow(iWidth-x,iHeight-y,18,18);

		x=f.GetKeyVal(key,"inputx",0);
		y=f.GetKeyVal(key,"inputy",0);
		w=f.GetKeyVal(key,"inputw",0);
		h=f.GetKeyVal(key,"inputh",0);
		SafeMoveWindow(&m_InputMessage,iWidth-x,iHeight-y,w,20);	//����
		x=f.GetKeyVal(key,"sendx",0);
		y=f.GetKeyVal(key,"sendy",0);
		//m_BtSend.SetWindowPos(NULL,iWidth-x,iHeight-y,0,0,SWP_NOSIZE|SWP_NOZORDER);
		m_BtSend.GetClientRect(&rect);
		m_BtSend.MoveWindow(iWidth-x,iHeight-y,rect.Width(),rect.Height());
	}

	///////////////////////////////////////
	////Kylin 20090209 �Զ���ӦͼƬ��С
	//���ϰ�ť
	CBcfFile bcffile(CBcfFile::GetAppPath() + _T("GameOption.bcf"));
	UINT uMaxType = bcffile.GetKeyVal("FrameControl","max_type",1);
	//��С��
	x=f.GetKeyVal(key,"minx",0);
	y=f.GetKeyVal(key,"miny",0);
	CString strPos;
	if (uMaxType != 0)
	{
	//m_BtMin.SetWindowPos(NULL,iWidth-x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_BtMin.GetClientRect(&rect);
	m_BtMin.MoveWindow(iWidth-x,y,rect.Width(),rect.Height());
		strPos.Format("rende116 X: %d , Y: %d",iWidth-x,y);
		OutputDebugString(strPos);
	}
	else
	{
		x = x + (f.GetKeyVal(key,"restx",0) - x);
		y = f.GetKeyVal(key,"resty",0);

		strPos.Format("rende116 X: %d , Y: %d",iWidth-x,y);
		OutputDebugString(strPos);
		m_BtMin.GetClientRect(&rect);
		m_BtMin.MoveWindow(iWidth-x,y,rect.Width(),rect.Height());
	}
	//�˳�
	x=f.GetKeyVal(key,"quitx",0);
	y=f.GetKeyVal(key,"quity",0);
	//m_BtGameQuit.SetWindowPos(NULL,iWidth-x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_BtGameQuit.GetClientRect(&rect);
	m_BtGameQuit.MoveWindow(iWidth-x,y,rect.Width(),rect.Height());
	//m_BtExit.GetClientRect(&rect);
	//m_BtExit.MoveWindow(iWidth-x,y,rect.Width(),rect.Height());

	//��󻯺ͻ�ԭ
	if (uMaxType != 0)
	{
	x=f.GetKeyVal(key,"restx",0);
	y=f.GetKeyVal(key,"resty",0);
	m_btMax.GetClientRect(&rect);
	m_btMax.MoveWindow(iWidth-x,y,rect.Width(),rect.Height());
	m_btRest.GetClientRect(&rect);
	m_btRest.MoveWindow(iWidth-x,y,rect.Width(),rect.Height());
	}

	y=f.GetKeyVal(key,"hideshowbt",350);
	if(showright)
	{//���ذ�ť
		m_BtHide.SetWindowPos(NULL,iWidth-m_vlistx,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
	}
	else
	{
		CRect rect;
		CGameImageHelper helprc(&m_rc);
		m_BtHide.GetClientRect(&rect);
		m_BtHide.SetWindowPos(NULL,iWidth-rect.Width()-helprc.GetWidth(),y,0,0,SWP_NOSIZE|SWP_NOZORDER);
	}
	//��Ϸ����
	x=f.GetKeyVal(key,"viewx",0);
	y=f.GetKeyVal(key,"viewy",0);
	w=f.GetKeyVal(key,"vieww",0);
	h=f.GetKeyVal(key,"viewh",0);

	if(m_pGameView->m_ShowType == 0)
	{
		if(showright)
			SafeMoveWindow(m_pGameView,x,y,iWidth-x-m_vlistx,iHeight-h-y);
		else
			SafeMoveWindow(m_pGameView,x,y,iWidth-w-x,iHeight-h-y);
	}
	else if(m_pGameView->m_ShowType == 1)
	{
		SafeMoveWindow(m_pGameView,x,y,iWidth-w-x,iHeight-h-y);
		//CRect rect;
		//m_pGameView->GetClientRect(&rect);
		//m_pGameView->OnSize(SIZE_RESTORED,rect.Width(),rect.Height());
		//m_pGameView->UpdateViewFace(NULL);
	}

	//{add by zxj 2008/04/09
	for( int i=0; i<MAX_PEOPLE; i++)
	{
		m_UserFrameRectArray[i].SetRect(m_pGameView->m_PtLogo[i].x, m_pGameView->m_PtLogo[i].y,\
			m_pGameView->m_PtLogo[i].x + m_pGameView->m_iUserFrame.GetWidth(), \
			m_pGameView->m_PtLogo[i].y + m_pGameView->m_iUserFrame.GetHeight());
	}
	//end add}

	//SafeMoveWindow(&m_btSndPlay,sndPlayX>0?sndPlayX:iWidth+sndPlayX,sndPlayY>0?sndPlayY:iHeight+sndPlayY,m_sndBtnCx,m_sndBtnCy);
	//SafeMoveWindow(&m_btSndPause,sndPauseX>0?sndPauseX:iWidth+sndPauseX,sndPauseY>0?sndPauseY:iHeight+sndPauseY,m_sndBtnCx,m_sndBtnCy);
	//SafeMoveWindow(&m_btVolDown,sndVolDownX>0?sndVolDownX:iWidth+sndVolDownX,sndVolDownY>0?sndVolDownY:iHeight+sndVolDownY,m_sndBtnCx,m_sndBtnCy);
	//SafeMoveWindow(&m_btVolUp,sndVolUpX>0?sndVolUpX:iWidth+sndVolUpX,sndVolUpY>0?sndVolUpY:iHeight+sndVolUpY,m_sndBtnCx,m_sndBtnCy);
	SafeMoveWindow(&m_Btbt1,iWidth-215,3,m_sndBtnCx,m_sndBtnCy);			//modify by wyl 11-5-12  
	/////////////////////////////////////
	///////Kylin 20090122 ��ӻ��ְ�������ɫ����
	int r=f.GetKeyVal(key,"showscorer",0);
	int g=f.GetKeyVal(key,"showscoreg",0);
	int b=f.GetKeyVal(key,"showscoreb",0);

	m_pGameView->m_showcoredlg.SetTextColor(RGB(r,g,b));
	/////////////////////////////////////

	if (m_bShowMax)
	{
		if (m_pRightFrame)
		{
			CRect rcRight;
			CRect rcMain;

			char szRcFile[64] = {0};
			_stprintf(szRcFile,"..\\config\\%s",m_skinmgr.GetGameSkinBcfFileName().GetBuffer());
			CBcfFile f(szRcFile);

			CRect rc[3];
			enum emSoduku
			{
				TC = 0, //���Ͻ�ͼ
				RC = 1, //���н�ͼ
				BC = 2, //���½�ͼ
			};

			char* szKey[3] = {"2_TC","5_RC","7_BC"};

			for(int i = 0; i < 3; ++i)
			{
				CString strPos;
				strPos = f.GetKeyVal(_T("Sudoku"),szKey[i],"0,0,0,0");
				rc[i].left	  = ::atoi(::strtok(strPos.GetBuffer(),","));
				rc[i].top	  = ::atoi(::strtok(NULL,","));
				rc[i].right  = rc[i].left + ::atoi(::strtok(NULL,","));
				rc[i].bottom = rc[i].top +  ::atoi(::strtok(NULL,","));
			}

			::GetWindowRect(m_pRightFrame->GetHWND(),&rcRight);
			::GetWindowRect(m_hWnd,rcMain);
			if (iWindowsWidth >= 1280)
			{
				::MoveWindow(m_pRightFrame->GetHWND(),rcMain.left + iWidth - m_pRightFrame->m_uMaxWidth - rc[RC].Width(),rc[TC].Height(),
					m_pRightFrame->m_uMaxWidth,iHeight- rc[TC].Height() - rc[BC].Height(),true);
			}
			else
			{
				::MoveWindow(m_pRightFrame->GetHWND(),rcMain.left + iWidth - m_pRightFrame->m_uMinWidth - rc[RC].Width(),rc[TC].Height(),
					m_pRightFrame->m_uMinWidth,iHeight- rc[TC].Height() - rc[BC].Height(),true);
			}
		}
	}
}

void CLoveSendClassInExe::OnInitPropList(int nRefleshType)
{
	// ֻ���������������»���
	if (nRefleshType>0)
	{
		//���µ������
		if (NULL != this->m_pPropPannel)
		{
			m_pPropPannel->ResetData();
		}
		return;
	}
	
    // PengJiLin, 2010-10-13, ��ť��Դ����ȫ���Ƶ�LoadSkin

	OnSetPropImages();
    //OnSetPropImagesS();     // PengJiLin, 2010-10-12
}
//====�������ɫ
void CLoveSendClassInExe::SetFillColor(COLORREF rgb)
{
	if(!m_TalkMessage.m_hWnd)
		return;

	m_TalkMessage.SetBackgroundColor(false,rgb);
	m_TalkMessage.m_bkColor=rgb;
}
//�ػ�����
void CLoveSendClassInExe::OnPaint()
{
	/// ��˫����İ취��дOnPaint����
	CPaintDC dc(this);

	/// ���òü����� zxd 20100330
	/// ȡ�������Ӵ�������
	CRgn rgn1, rgnWnd;
	rgn1.CreateRectRgnIndirect(&m_UserListRect);

	//��ȡλ��
	CRect ClientRect;
	GetClientRect(&ClientRect);
	CFont font,*pOldFont;
	int iWidth = ClientRect.Width();
	int iHeight = ClientRect.Height();

	rgnWnd.SetRectRgn(&ClientRect);
	rgn1.CombineRgn(&rgn1, &rgnWnd, RGN_AND);
	rgnWnd.CombineRgn(&rgnWnd, &rgn1, RGN_XOR);
	//dc.SelectClipRgn(&rgnWnd, RGN_AND);

	//add by wlr 20100118 ˫����
	CDC tempCDC;
	tempCDC.CreateCompatibleDC(&dc);
	CBitmap	BufferBmp;
	int width = ClientRect.Width();
	int height = ClientRect.Height();
	BufferBmp.CreateCompatibleBitmap(&dc,width,height);
	CBitmap* pOldBitmap = tempCDC.SelectObject(&BufferBmp);
	tempCDC.BitBlt(0,0,width,height,&dc,0,0,SRCCOPY);
	//end of add by wlr 20100118 ˫����

	tempCDC.SetBkMode(TRANSPARENT);	
	font.CreateFont(-12,0,0,0,400,NULL,NULL,NULL,134,3,2,1,2,TEXT("����"));
	pOldFont=tempCDC.SelectObject(&font);	

	DrawViewFrame(&tempCDC,iWidth,iHeight);
	//����
	TCHAR title[100];
	GetWindowText(title,99);

	//add by lxl 2010-10-20 ���������ʾ
	CSize size;
	size = dc.GetTextExtent(title,strlen(title));
	int iStrWidth = size.cx; // �õ���ʾtitle���ܿ��
	//m_titlex �ɸ�����󻯣���ԭ����̬����������ʾλ��
	if((iWidth - iStrWidth) / 2 >= 0)
		m_titlex = (iWidth - iStrWidth) / 2;
	
	tempCDC.SetTextColor(m_titlecr);
	tempCDC.TextOut(m_titlex,m_titley,title,lstrlen(title));

	if(m_pGameView->m_bShowRight)
	{
		DrawCurUserInfo(&tempCDC);
	}

	tempCDC.SetTextColor(RGB(0,0,0));
	tempCDC.SelectObject(pOldFont);

	font.DeleteObject();

	drawSignal(&tempCDC);
	dc.BitBlt(0,0,width,height,&tempCDC,0,0,SRCCOPY);

	tempCDC.SelectObject(pOldBitmap);
	BufferBmp.DeleteObject();
	tempCDC.DeleteDC();


	//if(m_ie.m_hWnd && szAdvUrl!="" && m_pGameView->m_bShowRight && !m_ie.get_Busy())
	//	m_ie.Refresh();
	return;
}
//���ƿ��
void CLoveSendClassInExe::DrawViewFrame(CDC * pDC, int iWidth, int iHeight)
{
	//����
	CGameImageHelper helptc(&m_tc);
	CGameImageHelper helpbc(&m_bc);
	//����
	CGameImageHelper helplc(&m_lc);
	CGameImageHelper helprc(&m_rc);

	//�߽�
	//��
	CGameImageHelper helplt(&m_lt);
	CGameImageHelper helprt(&m_rt);
	//�߽�down
	CGameImageHelper helplb(&m_lb);
	CGameImageHelper helprb(&m_rb);
//	CGameImageHelper helpGWnd(&m_gameWnd);



	if (false)
	{
		if(helptc.GetWidth() > 0)
		{
			//////////////////////////////////////
			int i = 0;
			for(; i < iWidth - helptc.GetWidth(); i+=helptc.GetWidth())
				helptc.BitBlt(pDC->GetSafeHdc(),i,0);

			///Kylin 20090104 ��ֹ����ͼƬ������ȫ��ˢ�²���������
			helptc.BitBlt(pDC->GetSafeHdc(),i,0,iWidth-i,helptc.GetHeight(),0,0);
			//////////////////////////////////////
		}	
		if(helpbc.GetWidth() > 0)
		{
			for(int i = 0; i < iWidth -helpbc.GetWidth() ; i += helpbc.GetWidth())
				helpbc.BitBlt(pDC->GetSafeHdc(),i,iHeight - helpbc.GetHeight());
		}
		if(helplc.GetWidth() > 0)
		{
			for(int i = 0; i < iHeight; i+=helplc.GetHeight())
			{
				helplc.BitBlt(pDC->GetSafeHdc(),0,i);
				helprc.BitBlt(pDC->GetSafeHdc(),iWidth-helprc.GetWidth(),i);
			}
		}
		/// ���Ʊ߿�
		helplt.BitBlt(pDC->GetSafeHdc(),0,0);	
		helprt.BitBlt(pDC->GetSafeHdc(),iWidth - helprt.GetWidth(),0);	
		helplb.BitBlt(pDC->GetSafeHdc(),0,iHeight - helplb.GetHeight());	
		helprb.BitBlt(pDC->GetSafeHdc(),iWidth - helprb.GetWidth(),iHeight - helprb.GetHeight());	

	}

	char szRcFile[64] = {0};
	_stprintf(szRcFile,"..\\config\\%s",m_skinmgr.GetGameSkinBcfFileName().GetBuffer());
	CBcfFile f(szRcFile);

	CRect rc[9];

	char* szKey[9] = {"1_TL","2_TC","3_TR","4_LC","5_RC","6_BL","7_BC","8_BR"};
	enum emSoduku
	{
		TL = 0,	//���Ͻ�ͼ
		TC = 1, //���Ͻ�ͼ
		TR = 2, //���Ͻ�ͼ
		LC = 3, //���н�ͼ
		RC = 4, //���н�ͼ
		BL = 5, //���½�ͼ
		BC = 6, //���½�ͼ
		BR = 7, //���½�ͼ
	};

	for(int i = 0; i < 9; ++i)
	{
		CString strPos;
		strPos = f.GetKeyVal(_T("Sudoku"),szKey[i],"0,0,0,0");
		rc[i].left	  = ::atoi(::strtok(strPos.GetBuffer(),","));
		rc[i].top	  = ::atoi(::strtok(NULL,","));
		rc[i].right  = rc[i].left + ::atoi(::strtok(NULL,","));
		rc[i].bottom = rc[i].top +  ::atoi(::strtok(NULL,","));
	}
	
	//TL = 0,���Ͻ�ͼ
	helplc.BitBlt(pDC->GetSafeHdc(),
		rc[TL].left,
		rc[TL].top,
		rc[TL].Width(),
		rc[TL].Height(),
		rc[TL].left,
		rc[TL].top);

	//TC = 1,���Ͻ�ͼ
	helplc.StretchBlt(pDC->GetSafeHdc(),
		rc[TC].left,
		rc[TC].top,
		iWidth - rc[TL].Width() - rc[TR].Width(),
		rc[TC].Height(),
		rc[TC].left,rc[TC].top,
		rc[TC].Width(),
		rc[TC].Height());

	//TR = 2,���Ͻ�ͼ
	helplc.BitBlt(pDC->GetSafeHdc(),
		iWidth - rc[TR].Width(),
		rc[TR].top,
		rc[TR].Width(),
		rc[TR].Height(),
		rc[TR].left,
		rc[TR].top);

	// LC = 3,���н�ͼ
	helplc.StretchBlt(pDC->GetSafeHdc(),
		rc[LC].left,
		rc[TL].Height(),
		rc[LC].Width(),
		iHeight - rc[TL].Height() - rc[BL].Height(),
		rc[LC].left,
		rc[LC].top,
		rc[LC].Width(),
		rc[LC].Height());

	// RC = 4,���н�ͼ
	helplc.StretchBlt(pDC->GetSafeHdc(),
		iWidth - rc[RC].Width(),
		rc[TR].Height(),
		rc[RC].Width(),
		iHeight - rc[TR].Height() - rc[BR].Height(),
		rc[RC].left,
		rc[RC].top,
		rc[RC].Width(),
		rc[RC].Height());

	// BL = 5,���½�ͼ
	helplc.BitBlt(pDC->GetSafeHdc(),
		rc[BL].left,
		iHeight - rc[BL].Height(),
		rc[BL].Width(),
		rc[BL].Height(),
		rc[BL].left,
		rc[BL].top);

	// BC = 6,���½�ͼ
	helplc.StretchBlt(pDC->GetSafeHdc(),
		rc[BL].Width(),
		iHeight - rc[BC].Height(),
		iWidth - rc[BL].Width() - rc[BR].Width(),
		rc[BC].Height(),
		rc[BC].left,
		rc[BC].top,
		rc[BC].Width(),
		rc[BC].Height());

	// BR = 7,���½�ͼ
	helplc.BitBlt(pDC->GetSafeHdc(),
		iWidth - rc[BR].Width(),
		iHeight - rc[BR].Height(),
		rc[BR].Width(),
		rc[BR].Height(),
		rc[BR].left,
		rc[BR].top);


    // PengJiLin, 2010-9-28, ��ť����ͼ
    CGameImageHelper helpPropBack(&m_propBackImage);
    CGameImageHelper helpPropBack2(&m_propBackImage2);
    CDC destdc;
    destdc.CreateCompatibleDC(pDC);
    destdc.SelectObject(helpPropBack);
    ::TransparentBlt(pDC->GetSafeHdc(),m_propBackRect.left, m_propBackRect.top, 
                     m_propBackRect.Width(), m_propBackRect.Height(),
                     destdc.GetSafeHdc(),0,0,helpPropBack.GetWidth(),helpPropBack.GetHeight(),RGB(255,0,255));
    destdc.SelectObject(helpPropBack2);
    ::TransparentBlt(pDC->GetSafeHdc(),m_propBackRect2.left, m_propBackRect2.top, 
                    m_propBackRect2.Width(), m_propBackRect2.Height(),
                    destdc.GetSafeHdc(),0,0,helpPropBack2.GetWidth(),helpPropBack2.GetHeight(),RGB(255,0,255));
    destdc.DeleteDC();

	DrawCurUserInfo(pDC);

}

void CLoveSendClassInExe::DrawCurUserInfo(CDC *pDC)
{
	//��ʾ�û���ϸ��Ϣ����������ʾ��Ŀ��modify by lxl 2010-10-15
	//��ȡ��ʾ�û���Ϣ�����ļ�
	CBcfFile bcfFile( CBcfFile::GetAppPath () + "..\\GameUserInfor.bcf");

	//��ʼ��Ĭ����ʾ��Ŀ�ı�ʶ
	bool bUserName = bcfFile.GetKeyVal(_T("InforDefaul"), _T("IsHaveUserName"), 0); //�û���
	bool bUserID = bcfFile.GetKeyVal(_T("InforDefaul"), _T("IsHaveUserID"), 0);		//ID
	bool bNickName = bcfFile.GetKeyVal(_T("InforDefaul"), _T("IsHaveNickName"), 0);	//�ǳ�
	bool bScore = bcfFile.GetKeyVal(_T("InforDefaul"), _T("IsHaveScore"), 0);	//����
	bool bCoin = bcfFile.GetKeyVal(_T("InforDefaul"), _T("IsHaveCoin"), 0);			//���	
	bool bCharm = bcfFile.GetKeyVal(_T("InforDefaul"), _T("IsHaveCharm"), 0);		//����
	bool bWealthLevel = bcfFile.GetKeyVal(_T("InforDefaul"), _T("IsHaveWealthLevel"), 0);	//�Ƹ��ȼ�

	CString szCharm = bcfFile.GetKeyVal(_T("InfoName"), _T("Charm"),_T("��  ����"));
	CString szCoin = bcfFile.GetKeyVal(_T("InfoName"), _T("Coin"),_T("��  �ң�"));
	CString szNickName = bcfFile.GetKeyVal(_T("InfoName"), _T("NickName"),_T("��  �ƣ�"));
	CString szScore = bcfFile.GetKeyVal(_T("InfoName"), _T("Score"),_T("��  �֣�"));
	CString szUserID = bcfFile.GetKeyVal(_T("InfoName"), _T("UserID"),_T("��  �ģ�"));
	CString szUserName = bcfFile.GetKeyVal(_T("InfoName"), _T("UserName"),_T("�û�����"));
	CString szWealthLevel = bcfFile.GetKeyVal(_T("InfoName"), _T("WealthLevel"),_T("��  ����"));


	/// �������ʾ�ұߴ��ڣ���ֱ�ӷ���
	if (!m_pGameView->m_bShowRight)
	{
		return;
	}
	if(m_CurpUserInfoClicked==NULL)
		return ;
	CRect ClientRect;
	//if(m_pGameView->m_ShowType == 1)
	//	m_pGameView->GetClientRect(&ClientRect);
	//else
	GetClientRect(&ClientRect);
	int x,y,h;
	TCHAR sz[100];
	y=0;
	//ͷ��
	//�ټ��� modify by wlr 20090716
	{
		char GameCode[20];
		::memset(GameCode,0,sizeof(GameCode));
		GetGameCode(GameCode);
		CString str = GameCode;
		//if (str != "10301800" && 
		//	str != "30501800" &&
		//	str != "10901800" &&
		//	str != "11901800" )
		//{
		//	m_pGameView->DrawLogo(pDC,m_CurpUserInfoClicked->bLogoID,ClientRect.right-m_logx,m_logy+8,false,m_CurpUserInfoClicked->dwUserID,false,true);
		//}
	}
	//m_pGameView->DrawLogo(pDC,m_CurpUserInfoClicked->bLogoID,ClientRect.right-m_logx,m_logy+8,false,m_CurpUserInfoClicked->dwUserID,false,true);
	//end of modify by wlr 20090716

	//TCHAR szBug[256]={0};
	//wsprintf(szBug,"CLoveSendClass::DrawCurUserInfo():%d %d",m_iPaintCount++,GetTickCount());
	////OutputDebugString(szBug);

	//��Ϣ
	x=ClientRect.right-m_u1x;
	//	SetRect(&rect,x,
	y=m_u1y;
	h=m_u1h;
	pDC->SetTextColor(m_usercr);
	char ch[30];
	::memset(ch,0,sizeof(ch));
	if (m_CurpUserInfoClicked->dwUserID == GetMeUserInfo()->dwUserID)
	{
		if ( lstrlen(m_CurpUserInfoClicked->szName)>28)
		{
			GetChar(m_CurpUserInfoClicked->szName,ch,28);
			ch[25]='.';
			ch[26]='.';
			ch[27]='.';
		}
		else
		{
			GetChar(m_CurpUserInfoClicked->szName,ch,28);
		}
	}
	else
	{
		if ( lstrlen(m_CurpUserInfoClicked->nickName)>28 )
		{
			GetChar(m_CurpUserInfoClicked->nickName,ch,28);
			ch[25]='.';
			ch[26]='.';
			ch[27]='.';
		}
		else
		{
			GetChar(m_CurpUserInfoClicked->nickName,ch,28);
		}
	}

	//���ΰټ�����ع��� add by wlr 20090716
	char GameCode[20];
	::memset(GameCode,0,sizeof(GameCode));
	GetGameCode(GameCode);
	CString str = GameCode;

	int i = 0;
	if(bUserName)
	{
		if (m_CurpUserInfoClicked->dwUserID == GetMeUserInfo()->dwUserID)
		{
			pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"%s%s",szUserName, ch));
		}
		else
		{
			pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,""));
		}
	}

	char ch1[30];
	::memset(ch1,0,sizeof(ch1));
	if ( lstrlen(m_CurpUserInfoClicked->nickName)>28 )
	{
		GetChar(m_CurpUserInfoClicked->nickName,ch1,28);
		ch[25]='.';
		ch[26]='.';
		ch[27]='.';
	}
	else
	{
		GetChar(m_CurpUserInfoClicked->nickName,ch1,28);
	}


	//OutputDebugMsg("ClientDLL>>�������[%d]��Ϣ�� x[%d] y[%d] ",m_CurpUserInfoClicked->dwUserID,x,y);

	
	if(NULL != m_CurpUserInfoClicked)
	{
		////pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"�ȼ�:%s",m_pGameInfo->pOrderName(m_CurpUserInfoClicked->dwPoint)));
		//if(m_pGameInfo->uComType!=TY_MONEY_GAME)
		//{
		//	//pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"����:%d",m_CurpUserInfoClicked->dwPoint-GetNormalOrderBasePoint(m_CurpUserInfoClicked->dwPoint)));
		//}	
		if (bNickName)
		{
			int tempy = y+h*i++;
			CRect rectName(x, tempy, x+400, tempy+20);
			wsprintf(sz,"%s%s",szNickName, ch1);
			pDC->DrawText(sz, rectName, DT_TOP|DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);//20090915�ǳ�̫���������ĳ�DrawText��ʽ��ʾ
		}
		
		if(bUserID)
			pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"%s%d",szUserID, m_CurpUserInfoClicked->dwUserID));
		
		

  //      // PengJiLin, 2010-6-12, Ÿ����ʽ��ʾ�����ݲ�һ��
  //      BYTE byType = m_UserListDlg.m_UserList.GetUserListDefineType();
  //      switch(byType)
  //      {
  //      case 1:         // Ÿ����ʽ
		//	if(bScore)
		//		pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"����  :%d",m_CurpUserInfoClicked->dwPoint));
  //          
		//	if(bWealthLevel)
		//		pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"�ȼ�  :%s",m_pGameInfo->pOrderName(m_CurpUserInfoClicked->dwPoint)));
  //          break;
  //      default:
  //          {
  //              //pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"�ǳ�  :%s",ch1));
  //              TCHAR szNum[32]; GlbGetNumString(szNum, m_CurpUserInfoClicked->dwMoney, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
		//		
		//		if(bCoin)
		//			pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"���  :%s",szNum));

		//		if(bCharm)
		//			pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"����  :%d",m_CurpUserInfoClicked->dwFascination));

		//		if(bWealthLevel)
		//			pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"�ȼ�  :%s",GetMoneyOrderName(m_CurpUserInfoClicked->dwMoney)) );

  //          }
  //          break;
		//}

		//modify by lxl 2010-10-15 ���������ļ���ʾ��Ϣ��Ŀ
		if(bScore)
			pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"%s%d",szScore, m_CurpUserInfoClicked->dwPoint));

		if(bCoin)
		{
			TCHAR szNum[128]; 
			GlbGetNumString(szNum, m_CurpUserInfoClicked->i64Money, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
			pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"%s%s",szCoin, szNum));
		}

		if(bCharm)
			pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"%s%d",szCharm, m_CurpUserInfoClicked->dwFascination));

		if(bWealthLevel)
			pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"%s%s",szWealthLevel, GetMoneyOrderName(m_CurpUserInfoClicked->i64Money)) );
	}
	else
	{
   //     // PengJiLin, 2010-6-12, Ÿ����ʽ��ʾ�����ݲ�һ��
   //     BYTE byType = m_UserListDlg.m_UserList.GetUserListDefineType();
   //     switch(byType)
   //     {
   //     case 1:
			//pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"ID    :"));
			//pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"�ǳ�  :"));
			//pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"����  :"));
			//pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"�ȼ�  :"));
   //         break;
   //     default:
			//pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"ID    :"));
			//pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"�ǳ�  :"));
			//pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"���  :"));
			//pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"����  :"));
			//pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,"�ȼ�  :"));
   //         break;
   //     }

		//modify by lxl, 2010-10-15, ���������ļ���ʾ��Ϣ��Ŀ
		if(bUserID)
			pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,szUserID));
		if(bNickName)
			pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,szNickName));
		if(bScore)
			pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,szScore));
		if(bCoin)
			pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,szCoin));
		if(bCharm)
			pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,szCharm));
		if(bWealthLevel)
			pDC->TextOut(x,y+h*i++,sz,wsprintf(sz,szWealthLevel));
	}
	/*pDC->TextOut(x,y+(2+k)*h,sz,wsprintf(sz,"Ӯ:%d",m_CurpUserInfoClicked->uWinCount));
	pDC->TextOut(x,y+(3+k)*h,sz,wsprintf(sz,"��:%d",m_CurpUserInfoClicked->uMidCount));
	pDC->TextOut(x,y+(4+k)*h,sz,wsprintf(sz,"��:%d",m_CurpUserInfoClicked->uLostCount));
	unsigned int count=m_CurpUserInfoClicked->uLostCount+m_CurpUserInfoClicked->uMidCount+m_CurpUserInfoClicked->uWinCount+m_CurpUserInfoClicked->uCutCount;
	pDC->TextOut(x,y+(5+k)*h,sz,wsprintf(sz,"�ܾ���:%d",count));
	UINT uCutPercent=(m_CurpUserInfoClicked->uCutCount>0)?(UINT)((double)(m_CurpUserInfoClicked->uCutCount*100)/(double)count):0;
	pDC->TextOut(x,y+(6+k)*h,sz,wsprintf(sz,"������:%d%%",uCutPercent));
	*/


	//if(m_iPaintCount<1)
	//	SetTimer(1,250,NULL);
}

//016�û�����
bool CLoveSendClassInExe::GameUserCome(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
{
	//====�û�����
	if (bDeskStation<MAX_PEOPLE)
	{
		//���ñ���
		if (bWatchUser) 
		{
			INT_PTR i=0;
			for (i=0;i<m_WatchPtrArray.GetCount();i++)
			{
				if (m_WatchPtrArray.GetAt(i)==NULL) break;
			}
			m_WatchPtrArray.SetAtGrow(i,pUserItem);
		}
		else
		{
			m_pUserInfo[bDeskStation]=pUserItem;
			//m_bDeskStation[bDeskStation]=1;	
			//this->InvalidateRect(m_UserInfoRect[bDeskStation]);
		}

		if(pUserItem->GameUserInfo.dwUserID==GetMeUserInfo()->dwUserID)
		{
			m_UserListDlg.AddGameUser(pUserItem, true);
			if (m_pRightFrame)
			{
				m_pRightFrame->ShowUserInfo(pUserItem);
			}
		}
		//���ý���
		if(GetComType()!=TY_MATCH_GAME)
		{//����ʱ,ֻ��ʾ�Լ�
			//if(pUserItem->GameUserInfo.dwUserID==GetMeUserInfo()->dwUserID)
			m_UserListDlg.AddGameUser(pUserItem);
		}

		if (bWatchUser) 
		{
			BYTE bDeskStation=pUserItem->GameUserInfo.bDeskStation;
			if (m_pUserInfo[bDeskStation]!=NULL)
				m_UserListDlg.SetWatchUser(pUserItem,m_pUserInfo[bDeskStation]->GameUserInfo.nickName);
		}
		else 
		{
			m_pGameView->SetUserInfo(ViewStation(bDeskStation),pUserItem);
		}

		return true;
	}
	return false;
}
//015�û��뿪
bool CLoveSendClassInExe::GameUserLeft(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
{
	if (bDeskStation<MAX_PEOPLE)
	{
		//���ñ���
		if (bWatchUser==true) 
		{
			UserItemStruct * pFindUserItem=NULL;
			for (INT_PTR i=0;i<m_WatchPtrArray.GetCount();i++)
			{
				pFindUserItem=(UserItemStruct *)m_WatchPtrArray.GetAt(i);
				if (NULL == pFindUserItem)
				{
					continue;
				}
				if (pUserItem->GameUserInfo.dwUserID==pFindUserItem->GameUserInfo.dwUserID)
				{
					m_WatchPtrArray.SetAt(i,NULL);
					break;
				}
			}
		}
		else
		{
			m_pUserInfo[bDeskStation]=NULL;
		}
		// �ӱ�������б����������
		UserItemStruct * pFindUserItem=NULL;
		INT_PTR uCount=m_ThisDeskPtrArray.GetCount();
		for (INT_PTR i=0;i<uCount;++i)
		{
			pFindUserItem=(UserItemStruct *)m_ThisDeskPtrArray.GetAt(i);
			if (pUserItem->GameUserInfo.dwUserID
				==pFindUserItem->GameUserInfo.dwUserID)
			{
				m_UserListDlg.DeleteGameUser(pFindUserItem);
				delete pFindUserItem;
				pFindUserItem = NULL;
				m_ThisDeskPtrArray.RemoveAt(i);
				if (bWatchUser==false)
				{
					if (m_pGameView) m_pGameView->SetUserInfo(ViewStation(bDeskStation),NULL);
				}
				break;
			}
		}

		//���ý���
		if(pUserItem->GameUserInfo.dwUserID==m_CurpUserInfoClicked->dwUserID)
		{
			m_CurpUserInfoClicked=GetMeUserInfo();
			NavigateVirFace();
		}

		return true;
	}
	return false;
}

//�û�ͬ��
bool CLoveSendClassInExe::UserAgreeGame(BYTE bDeskStation)
{
	if ((bDeskStation<MAX_PEOPLE)&&(m_pUserInfo[bDeskStation]!=NULL))
	{
		//���ý���
		bool bRet = m_UserListDlg.UpdateGameUser(m_pUserInfo[bDeskStation]);
		//DebugPrintf("UserAgreeGame() UpdateGameUser-%s--%d", bRet?"�ɹ�":"ʧ��",
		//	m_pUserInfo[bDeskStation]->GameUserInfo.bUserState);
		m_pGameView->UpdateUserInfo(ViewStation(bDeskStation));
		return true;
	}
	return false;
}
void CLoveSendClassInExe::NavigateVirFace()
{
	if(!m_CurpUserInfoClicked)
		return;
	//CRect rect;
	//GetClientRect(&rect);
	//rect.top+=24;
	//rect.left=rect.right-3-207;
	//rect.right=rect.left+207;
	//rect.bottom=rect.top+193;
	/// ֻ������Ҫ��ʾ�ұ�����ʱ��Ÿ��½���
	if(m_pGameView->m_bShowRight)
	{
#if 0
		InvalidateRect(&m_UserInfoRect,false);
#else
		InvalidateRect(&m_UserInfoRect);
#endif
	}
}
void CLoveSendClassInExe::OnSetPropImages()
{
	//OutputDebugString("�������·�ҳ��ť");
	int propCout = (int)Glb().userPropLibrary.GetCount();

//#ifdef _USE_PROP_PAGE_SW_
	//���÷�ҳ
	if(m_curPropPage*PEER_PAGE>=propCout)
		m_curPropPage--;
	if(m_curPropPage<0)
		m_curPropPage=0;
	if(m_curPropPage>0)
	{
		//wushuqun 2009.5.19
		//���ý���
		m_BtPropGoAhead.EnableWindow(TRUE);
		m_Btbt6.EnableWindow(TRUE);
		m_BtPropGoAhead.SetFocus();
	}
	else
	{
		m_BtPropGoAhead.EnableWindow(FALSE);
		m_Btbt6.EnableWindow(FALSE);
	}
	if((m_curPropPage+1)*PEER_PAGE<propCout)
	{
		//wushuqun 2009.5.19
		//���ý���
		m_BtPropGoNext.EnableWindow(TRUE);
		m_Btbt7.EnableWindow(TRUE);
		m_BtPropGoNext.SetFocus();
	}
	else
	{
		m_BtPropGoNext.EnableWindow(FALSE);
		m_Btbt7.EnableWindow(FALSE);
	}
//#else
//	m_curPropPage = 0;
//#endif

	_TAG_USERPROP *userProp=NULL;
	CString key=TEXT("config");
	//TCHAR path[MAX_PATH];
	CString skinfolder;

	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( m_iniFile);
	CString skinHall = CBcfFile::GetAppPath()+"..\\" + m_skinmgr.GetSkinPath();

	TCHAR	szPropImagePath[MAX_PATH];



	if (CBcfFile::IsFileExist(s + "config\\" + m_skinmgr.GetGameSkinBcfFileName()))  /// add by wxx 2010.7.1
	{ 
		wsprintf(szPropImagePath, TEXT("%s\\prop\\"), s + "image\\game");
	} 
	else 
	{ 
		wsprintf(szPropImagePath, TEXT("%sprop\\"), s + "..\\" + m_skinmgr.GetGameSkinPath());
	} 


	TCHAR szPropImage[MAX_PATH]={0};
	for(int i=0;i<PEER_PAGE;i++)
	{
		int curPropIndex=m_curPropPage*PEER_PAGE+i;
		CGameImage img;
		HRGN hrgn;
		CNormalBitmapButtonEx *pButton=(CNormalBitmapButtonEx*)GetDlgItem(m_BtPropArray[i]);//ȡ4������BUTTON

//         // PengJiLin, 2010-9-27
//         pButton->EnableToShowText(0, false);

		if(curPropIndex<propCout)
		{
            //����е���
            pButton->EnableWindow(TRUE);
			userProp=Glb().userPropLibrary.GetAt(curPropIndex);
			//wsprintf(szPropImage, TEXT("%sProp%d.bmp"), szPropImagePath, (userProp->attribAction&0x7F000000)>>24);
			wsprintf(szPropImage, TEXT("%sProp%d.bmp"), szPropImagePath, curPropIndex+1);
			img.SetLoadInfo(szPropImage,true);
			pButton->LoadButtonBitmap(szPropImage,false);
			m_ToolTip.UpdateTipText(userProp->szPropName,this,m_BtPropArray[i]);

            // PengJiLin, 2010-9-27
            pButton->EnableToShowText(userProp->nHoldCount, true);
		}
		else
		{
            //���û�е���
            pButton->EnableWindow(FALSE);
			img.SetLoadInfo(m_szPropEmptyImage,true);
			pButton->LoadButtonBitmap(m_szPropEmptyImage,false);
			m_ToolTip.UpdateTipText("����������",this,m_BtPropArray[i]);
		}

		hrgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
		if(hrgn)
			pButton->SetWindowRgn(hrgn,true);
	}
	//m_BtProp1 �̶�ΪVIP����

	if (m_pRightFrame)
	{
		m_pRightFrame->OnSetPropImages();
	}

	TCHAR	szPropPath[MAX_PATH];
	wsprintf(szPropPath,"..\\%sProp\\Prop9999.bmp",m_skinmgr.GetGameSkinPath());

	m_BtProp1.LoadButtonBitmap(szPropPath,false);
	//���µ������
	if (NULL != this->m_pPropPannel)
	{
		m_pPropPannel->ResetData();
	}

}

// PengJiLin, 2010-10-12, �̳����߹���
void CLoveSendClassInExe::OnSetPropImagesS()
{
    int propCout = (int)Glb().userPropLibrary.GetCount();

    //���÷�ҳ
    if(m_curPropPage_S*PEER_PAGE>=propCout)
        m_curPropPage_S--;
    if(m_curPropPage_S<0)
        m_curPropPage_S=0;
    if(m_curPropPage_S>0)
    {
        m_Btbt6.EnableWindow(TRUE);
        m_Btbt6.SetFocus();
    }
    else
    {
        m_Btbt6.EnableWindow(FALSE);
    }

    if((m_curPropPage_S+1)*PEER_PAGE<propCout)
    {
        m_Btbt7.EnableWindow(TRUE);
        m_Btbt7.SetFocus();
    }
    else
    {
        m_Btbt7.EnableWindow(FALSE);
    }


    _TAG_USERPROP *userProp=NULL;
    CString key=TEXT("config");
    CString skinfolder;

    CString s=CBcfFile::GetAppPath ();
    CBcfFile f( m_iniFile);
    CString skinHall = CBcfFile::GetAppPath()+"..\\" + m_skinmgr.GetSkinPath();
	OutputDebugString((LPCTSTR)skinHall);
    TCHAR	szPropImagePath[MAX_PATH];
	OutputDebugString(s + "config\\" + m_skinmgr.GetGameSkinBcfFileName());
    if (CBcfFile::IsFileExist(s + "config\\" + m_skinmgr.GetGameSkinBcfFileName()))
    { 
        wsprintf(szPropImagePath, TEXT("%s\\prop\\"), s + "image\\game");
    } 
    else 
    { 
        wsprintf(szPropImagePath, TEXT("%sprop\\"), s + "..\\" + m_skinmgr.GetGameSkinPath());
    } 


    TCHAR szPropImage[MAX_PATH]={0};
    for(int i=0;i<PEER_PAGE;i++)
    {
        int curPropIndex=m_curPropPage_S*PEER_PAGE+i;
        CGameImage img;
        HRGN hrgn;
        CNormalBitmapButtonEx *pButton = NULL;
        if(0 == i)pButton = (CNormalBitmapButtonEx*)GetDlgItem(IDC_BUTTON_PROP4);
        else if(1 == i)pButton = (CNormalBitmapButtonEx*)GetDlgItem(IDC_BUTTON_PROP5);
        else pButton = (CNormalBitmapButtonEx*)GetDlgItem(IDC_BUTTON_PROP6);

        if(curPropIndex<propCout)
        {
            //����е���
            pButton->EnableWindow(TRUE);
            userProp=Glb().userPropLibrary.GetAt(curPropIndex);
            wsprintf(szPropImage, TEXT("%sProp%d.bmp"), szPropImagePath, (userProp->attribAction&0x7F000000)>>24);
            img.SetLoadInfo(szPropImage,true);
            pButton->LoadButtonBitmap(szPropImage,false);
            m_ToolTip.UpdateTipText(userProp->szPropName,this,m_BtPropArray[i]);

        }
        else
        {
            //���û�е���
            pButton->EnableWindow(FALSE);
            img.SetLoadInfo(m_szPropEmptyImage,true);
            pButton->LoadButtonBitmap(m_szPropEmptyImage,false);
        }

        hrgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
        if(hrgn)
            pButton->SetWindowRgn(hrgn,true);
    }

}

void CLoveSendClassInExe::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedExit();
	//AfxGetApp()->m_pMainWnd->PostMessage(WM_GAME_QUIT,0,0);
}

void CLoveSendClassInExe::OnBnClickedExit()
{
	// ��Ҫ�ж��Ƿ�������Ϸ��
	if ((m_bWatchMode==false)&&GetStationParameter() >= 20 && GetStationParameter() < 23)//!CanLeftDesk())
	{
		//if (IDOK!=AFCMessageBox("��������Ϸ�У������˳����ᱻ�۷֣�ȷʵҪ�˳���",m_pGameInfo->szGameName,AFC_YESNO))//,MB_YESNO|MB_DEFBUTTON2|MB_ICONINFORMATION,this))
		//	return;
		//SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
	}
	OnCancel();
}

void CLoveSendClassInExe::OnBnClickedRest()
{
	/*
	//�ټ��� add by wlr 20090717
	char GameCode[20];
	::memset(GameCode,0,sizeof(GameCode));
	GetGameCode(GameCode);
	//if (str == "11901800"
	//	|| str == "10901800"
	//	|| str == "30501800")
	TCHAR szKey[10];
	CString sPath=CBcfFile::GetAppPath();
	CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
	wsprintf(szKey, "%d", m_pGameInfo->uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

	if (iResult>0)
	{
		return;
	}
	//end of �ټ��� add by wlr 20090717
	*/

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bMax=false;
	m_bShowMax=false;

	CBcfFile f(CBcfFile::GetAppPath() + _T("GameOption.bcf"));
	UINT uMaxType = f.GetKeyVal("FrameControl","max_type",1);

	if (uMaxType == 2) //�޸ķֱ���
	{
		DEVMODE lpDevMode;
		lpDevMode.dmBitsPerPel=32;
		lpDevMode.dmPelsWidth = m_nWiondwMetricsX;
		lpDevMode.dmPelsHeight = m_nWiondwMetricsY;
		lpDevMode.dmSize = sizeof(lpDevMode);
		lpDevMode.dmFields = DM_PELSWIDTH|DM_PELSHEIGHT|DM_BITSPERPEL;
		LONG result;
		result = ChangeDisplaySettings(&lpDevMode,0);
		if (result == DISP_CHANGE_SUCCESSFUL)
		{
			ChangeDisplaySettings(&lpDevMode,CDS_UPDATEREGISTRY);
			// ����//ʹ��CDS_UPDATEREGISTRY��ʾ���޸��ǳ־õģ�
			// ����//����ע�����д������ص����� 
		}
		else
		{
			//AfxMessageBox(L"�޸�ʧ�ܣ��ָ�ԭ������!");
			ChangeDisplaySettings(NULL,0);
		}
	}


	MoveWindow(0,0,1024, 738);
	//MoveWindow(0,0,1000,700);
	CenterWindow();
	m_btMax.ShowWindow(SW_SHOW);
	m_btRest.ShowWindow(SW_HIDE);

	this->m_pGameRoom->PostMessage(WM_WINDOW_MOVE,0,1);

	if (uMaxType == 2) //�޸ķֱ���
	{
		//֪ͨ����UI������Ӧ����
		this->m_pGameRoom->PostMessage(WM_SCREEN_SIZE_CHANGE,0,2);
	}

}

void CLoveSendClassInExe::OnBnClickedQuitGame()
{
    // PengJiLin, 2011-4-18, ����ʱ���������ͽ�� ѯ��
    CString strInfo = "";
    GetSendMoneyFormatInfo(strInfo);
    strInfo += "��ȷ��Ҫ�뿪��?";

    if(m_SendMoneyData.dwMoneyOnTimes > 0 || m_SendMoneyData.dwMoneyOnCounts > 0)
    {
        if(IDOK != AFCMessageBox(strInfo, "��ʾ", AFC_YESNO, this))
        {
            return;
        }
    }

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedExit();
}

void CLoveSendClassInExe::OnBnClickedMax()
{
	/*
	//�ټ��� add by wlr 20090717
	char GameCode[20];
	::memset(GameCode,0,sizeof(GameCode));
	GetGameCode(GameCode);
	//if (str == "11901800"
	//	|| str == "10901800"
	//	|| str == "30501800")
	TCHAR szKey[10];
	CString sPath=CBcfFile::GetAppPath();
	CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
	wsprintf(szKey, "%d", m_pGameInfo->uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

	if (iResult>0)
	{
		return;
	}
	//end of �ټ��� add by wlr 20090717
	*/
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bMax=true;
	m_bShowMax=true;

	//�޸ķֱ���

	CBcfFile f(CBcfFile::GetAppPath() + _T("GameOption.bcf"));
	UINT uMaxType = f.GetKeyVal("FrameControl","max_type",1);

	if (uMaxType == 1) //��ͨ���
	{
		ShowWindow(SW_RESTORE);
	}
	else if (uMaxType == 2) //�޸ķֱ���
	{

		DEVMODE lpDevMode;
		lpDevMode.dmBitsPerPel=32;
		lpDevMode.dmPelsWidth=1024;
		lpDevMode.dmPelsHeight=768;
		lpDevMode.dmSize = sizeof(lpDevMode);
		lpDevMode.dmFields = DM_PELSWIDTH|DM_PELSHEIGHT|DM_BITSPERPEL;
		LONG result;
		result = ChangeDisplaySettings(&lpDevMode,0);
		if (result == DISP_CHANGE_SUCCESSFUL)
		{
			ChangeDisplaySettings(&lpDevMode,CDS_UPDATEREGISTRY);
			// ����//ʹ��CDS_UPDATEREGISTRY��ʾ���޸��ǳ־õģ�
			// ����//����ע�����д������ص����� 
		}
		else
		{
			//AfxMessageBox(L"�޸�ʧ�ܣ��ָ�ԭ������!");
			ChangeDisplaySettings(NULL,0);
		}
	}

	CRect rect;
	::SystemParametersInfo(SPI_GETWORKAREA,NULL,&rect,NULL);
	if (uMaxType == 1) //��ͨ���
	{
		MoveWindow(0,0,rect.Width(),rect.Height());
	}
	m_btMax.ShowWindow(SW_HIDE);
	m_btRest.ShowWindow(SW_SHOW);

	if (uMaxType == 2) //�޸ķֱ���
	{
		//֪ͨ����UI������Ӧ����
		this->m_pGameRoom->PostMessage(WM_SCREEN_SIZE_CHANGE,0,1);
	}

	SetForegroundWindow();
}

void CLoveSendClassInExe::OnBnClickedHide()
{
	//���ΰټ�����ع��� add by wlr 20090716
	TCHAR szKey[10];
	CString sPath=CBcfFile::GetAppPath();
	CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
	wsprintf(szKey, "%d", m_pGameInfo->uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

	if (iResult>0)
	{
		return;
	}
	//end of add by wlr 20090716

	CGameImage img;
	HRGN hRgn;
	if(m_pGameView->m_bShowRight)
	{
		m_pGameView->m_bShowRight = false;
		m_BtHide.LoadButtonBitmap(m_ShowbtPath.GetBuffer(),false);
		img.SetLoadInfo(m_ShowbtPath.GetBuffer(),true);
		m_BtHide.SetWindowPos(NULL,0,0,img.GetWidth()/4,img.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);
		hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
		if(hRgn)
			m_BtHide.SetWindowRgn(hRgn,true);

		if (m_pRightFrame)
		{
			m_pRightFrame->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		m_pGameView->m_bShowRight = true;
		m_BtHide.LoadButtonBitmap(m_HidebtPath.GetBuffer(),false);
		img.SetLoadInfo(m_HidebtPath.GetBuffer(),true);
		m_BtHide.SetWindowPos(NULL,0,0,img.GetWidth()/4,img.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);
		hRgn=AFCBmpToRgn(img,RGB(255,0,255),RGB(1,1,1));
		if(hRgn)
			m_BtHide.SetWindowRgn(hRgn,true);

		if (m_pRightFrame)
		{
			m_pRightFrame->ShowWindow(SW_SHOW);
		}

	}
	UINT showright = (m_pGameView->m_bShowRight) ? SW_SHOW : SW_HIDE;
	showright = SW_HIDE;
	m_UserListDlg.ShowWindow(showright);
	m_TalkMessage.ShowWindow(showright);
	//	m_BtGameSet.ShowWindow(showright);
	m_Btbt1.ShowWindow(SW_SHOW);
//	m_Btbt2.ShowWindow(showright);
//	m_Btbt3.ShowWindow(showright);
//	m_Btbt4.ShowWindow(showright);

	//{add by zxj 2008/04/07
	m_BtPropGoAhead.ShowWindow(SW_HIDE);
	m_BtPropGoNext.ShowWindow(SW_HIDE);
	m_BtProp1.ShowWindow(showright);
	m_BtProp2.ShowWindow(showright);
	m_BtProp3.ShowWindow(showright);
    m_BtProp4.ShowWindow(showright);
    m_BtProp5.ShowWindow(showright);
	m_BtProp6.ShowWindow(showright);
	m_BtProp7.ShowWindow(showright);
	m_BtProp8.ShowWindow(showright);
	m_BtProp9.ShowWindow(showright);
	m_BtProp10.ShowWindow(showright);
	//end add}

	//m_Btbt5.ShowWindow(showright);
	//m_Btbt6.ShowWindow(showright);
	//m_Btbt7.ShowWindow(showright);
	m_BtExpression.ShowWindow(showright);
	m_InputMessage.ShowWindow(showright);
	m_BtSend.ShowWindow(showright);

    // PengJiLin, 2010-10-8
//    m_Btbt5.ShowWindow(showright);
//    m_Btbt6.ShowWindow(showright);
//    m_Btbt7.ShowWindow(showright);


	CRect rect;
	GetClientRect(&rect);
	FixControlStation(rect.Width(),rect.Height());
	m_pGameView->UpdateViewFace(NULL);
	return;
}

void CLoveSendClassInExe::OnBnClickedMin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->ShowWindow(SW_MINIMIZE);

	//PostMessage(WM_BANK2_TEST,0,0);

	//AFCMessageBox("WM_BANK2_TEST0");
	
}
//014���ͺ���
int CLoveSendClassInExe::SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	if (m_pGameInfo->uisMeUserInfo.GameUserInfo.bDeskNO==255) 
		return 0;

	UINT nBufferLen = uBufLen + sizeof(SendGameDataStructWithoutParam);
	BYTE *buffer = new BYTE[nBufferLen];
	SendGameDataStructWithoutParam *pHead = (SendGameDataStructWithoutParam *)buffer;
	pHead->bMainID		= bMainID;
	pHead->bAssistantID = bAssistantID;
	pHead->bHandleCode	= bHandleCode;
	memcpy(buffer+sizeof(SendGameDataStructWithoutParam), pData, uBufLen);

	m_pGameRoom->PostMessage(WM_SEND_GAME_DATA, (WPARAM)nBufferLen, (LPARAM)buffer);
	return 0;
}
//���ͺ���
int CLoveSendClassInExe::SendGameData(BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode)
{
	if (m_pGameInfo->uisMeUserInfo.GameUserInfo.bDeskNO==255)
		return 0;
	SendGameDataStructWithoutParam *pHead = new SendGameDataStructWithoutParam;
	pHead->bMainID		= bMainID;
	pHead->bAssistantID = bAssistantID;
	pHead->bHandleCode	= bHandleCode;
	WORD nLen = sizeof(SendGameDataStructWithoutParam);

	if (bAssistantID == ASS_GM_AGREE_GAME && m_pGameInfo->dwRoomRule & GRR_QUEUE_GAME)
	{
		CBcfFile f(CBcfFile::GetAppPath() + _T("GameOption.bcf"));
		UINT _bExit = f.GetKeyVal("QueueRoom", "ContinueExit",1);
		//pHead->bHandleCode = 10;
		if (_bExit == 1 || m_bIsQueueDisMiss || m_pGameInfo->dwRoomRule & GRR_CONTEST)
		{
			pHead->bHandleCode = 1;
			m_pGameRoom->SendMessage(WM_SEND_GAME_DATA, (WPARAM)nLen, (LPARAM)pHead);
			m_pGameRoom->PostMessage(WM_GAME_QUIT,0,0);
			return 0;
		}
	}

	m_pGameRoom->PostMessage(WM_SEND_GAME_DATA, (WPARAM)nLen, (LPARAM)pHead);
	return 0;
}

void CLoveSendClassInExe::OnMove(int x, int y)
{
	int iWindowsWidth = GetSystemMetrics(SM_CXSCREEN);
	int iWindowsHeight = GetSystemMetrics(SM_CYSCREEN);
	if (m_pRightFrame)
	{
		if (m_bShowMax)
		{
			if (iWindowsWidth >= 1280)
			{
				m_vlistx = m_pRightFrame->m_uMaxWidth;
			}
			else
			{
				m_vlistx = m_pRightFrame->m_uMinWidth;
			}
		}
		else
		{
			m_vlistx = m_pRightFrame->m_uMinWidth;
		}
		m_pGameView->m_vlistx = m_vlistx;
	}
	
	if (x != -32000 && y != -32000)
	{
		this->m_pGameRoom->PostMessage(WM_WINDOW_MOVE, 0, 1);
	}
	else
	{
		this->m_pGameRoom->PostMessage(WM_ACTIVE_TO_ROOM, 4, 1);
	}
	// TODO: �ڴ˴������Ϣ����������
	CLoadFileBmp::OnMove(x, y);

	//{{Deleted by ZXD
	CRect rect;
	GetClientRect(&rect);
	FixControlStation(rect.right,rect.bottom);
	//Deleted by ZXD}}

	CRect rc;
	::GetWindowRect(m_hWnd,rc);
	int iWRight = rc.Width();
	int iHRight = rc.Height();
// 	CString str;
// 	str.Format("DClient: 1 x = %d, y = %d, Main(%d,%d) %d",x,y,iWRight,iHRight,m_bShowMax);
// 	OutputDebugString(str);

	if (m_pRightFrame)
	{

		CRect rcRight;
		CRect rcMain;
		::GetWindowRect(m_pRightFrame->GetHWND(),&rcRight);
		::GetWindowRect(m_hWnd,rcMain);

		char szRcFile[64] = {0};
		_stprintf(szRcFile,"..\\config\\%s",m_skinmgr.GetGameSkinBcfFileName().GetBuffer());
		CBcfFile f(szRcFile);

		CRect rc[3];
		enum emSoduku
		{
			TC = 0, //���Ͻ�ͼ
			RC = 1, //���н�ͼ
			BC = 2, //���½�ͼ
		};

		char* szKey[3] = {"2_TC","5_RC","7_BC"};

		for(int i = 0; i < 3; ++i)
		{
			CString strPos;
			strPos = f.GetKeyVal(_T("Sudoku"),szKey[i],"0,0,0,0");
			rc[i].left	  = ::atoi(::strtok(strPos.GetBuffer(),","));
			rc[i].top	  = ::atoi(::strtok(NULL,","));
			rc[i].right  = rc[i].left + ::atoi(::strtok(NULL,","));
			rc[i].bottom = rc[i].top +  ::atoi(::strtok(NULL,","));
		}


		if (m_bShowMax)
		{
			if (iWindowsWidth >= 1280)
			{			
				::MoveWindow(m_pRightFrame->GetHWND(),x + rcMain.Width() - m_pRightFrame->m_uMaxWidth - rc[RC].Width(),
					y + rc[TC].Height(),m_pRightFrame->m_uMaxWidth,rcMain.Height() - rc[TC].Height() - rc[BC].Height(),
					true);
			}
			else
			{
				::MoveWindow(m_pRightFrame->GetHWND(),x + rcMain.Width() - m_pRightFrame->m_uMinWidth - rc[RC].Width(),
					y + rc[TC].Height(),m_pRightFrame->m_uMinWidth,rcMain.Height() - rc[TC].Height() - rc[BC].Height(),
					true);
			}
		}
		else
		{
			::MoveWindow(m_pRightFrame->GetHWND(),x + rcMain.Width() - m_pRightFrame->m_uMinWidth - rc[RC].Width(),
				y + rc[TC].Height(),m_pRightFrame->m_uMinWidth,rcMain.Height() - rc[TC].Height() - rc[BC].Height(),
				true);
		}
		
	}
}

BOOL CLoveSendClassInExe::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//wushuqun 2009.6.9
	return true;
	return CLoadFileBmp::OnEraseBkgnd(pDC);
}

void CLoveSendClassInExe::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bMax)
	{
		if (point.y < 28)
		{
			PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
		}
		CDialog::OnLButtonDown(nFlags, point);
	}
}

BOOL CLoveSendClassInExe::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch (LOWORD(wParam))
	{
	case IDM_SAVE_MESSAGE:				//����������Ϣ
		{
			m_TalkMessage.OnSave();
			return TRUE;
		}
	case IDM_CLEAR_SCREEN:				//���������Ļ
		{
			m_TalkMessage.OnClearAll();
			return TRUE;
		}
	case IDM_AUTO_SCROLL:				//�Զ�������Ļ
		{
			m_TalkMessage.OnAutoScroll();
			return TRUE;
		}
	case IDM_SET_FRIEND:
		{
			m_pGameRoom->PostMessage(WM_FRIEND, 0, m_dwCurrentUserID);
			return TRUE;
		}
	case IDM_SET_EMENY:
		{
			m_pGameRoom->PostMessage(WM_EMENY, 0, m_dwCurrentUserID);
			return TRUE;
		}
	case IDM_GAME_RULE:					//��Ϸ����
		{
			//CString strBuffer;
			//ShellExecute(NULL,TEXT("open"),TEXT(Glb().m_CenterServerPara.m_strHelpADDR),NULL,NULL,SW_MAXIMIZE);
			return TRUE;
		}
	case IDM_TALK_COLOR:				//����������ɫ
		{
			CColorDialog Color(CGameImageLink::m_ColorResource.m_crTalkTxColor,CC_FULLOPEN,this);
			if (Color.DoModal()==IDOK) 
				CGameImageLink::m_ColorResource.m_crTalkTxColor=Color.GetColor();
			m_InputMessage.SetFocus();
			return TRUE;
		}
	case IDM_USER_INFO:                 //�鿴�û���ϸ����
		{
			return OnCmdShowUserInfo();
		}
	case IDM_COPY_USER_NAME:			//�����û�����
		{
			return OnCmdCopyUserInfo();
		}
	case IDM_GIVE_PROP:
		{
			::PostMessage(m_pGameRoom->GetSafeHwnd(),WM_USER+157,m_dwCurrentUserID,0);
			return TRUE;
		}
	case IDM_KICK_USER://����
		{
			return OnCmdKickUser();
		}
    case IDM_KICK_USER_PROP:        // PengJiLin, 2010-9-10, ���˿������˹���
        {
            return OnCmdKickUserProp();
        }
	case IDM_SET_TALK_OBJECT:			//����̸������
		{
			if (m_dwCurrentUserID!=0L)
			{
				//��������
				UserItemStruct * pUserItem=FindOnLineUser(m_dwCurrentUserID);
				if (pUserItem==NULL) return TRUE;

				//�����������
				//if (pUserItem!=NULL)
				//{
				//	int iIndex=m_TargetPeople.FindString(1,pUserItem->GameUserInfo.nickName);
				//	if (iIndex!=CB_ERR) 
				//		m_TargetPeople.SetCurSel(iIndex);
				//}
			}
			m_InputMessage.SetFocus();
			return TRUE;
		}
	case IDM_CANCEL_TALK:				//ȡ��̸������
		{
			//m_TargetPeople.SetCurSel(0);
			m_InputMessage.SetFocus();
			return TRUE;
		}
	case IDM_JOIN_GAME:					//������Ϸ
		{
			//Ч������
			if ((m_bWatchMode==false)||(m_pGameInfo->uisMeUserInfo.GameUserInfo.bDeskNO==255)) 
				return TRUE;

			//��������

			return TRUE;
		}
	case IDM_EMABLE_WATCH:				//�����Թ�
	case IDM_UNEMABLE_WATCH:			//�������Թ�
		{
			if ((m_dwCurrentUserID!=0)&&(m_bWatchMode==false))
			{
				MSG_GM_WatchSet WatchSet;
				WatchSet.dwUserID=m_dwCurrentUserID;
				SendGameData(&WatchSet,sizeof(WatchSet),MDM_GM_GAME_FRAME,ASS_GM_WATCH_SET,LOWORD(wParam)==IDM_EMABLE_WATCH);
			}
			return TRUE;
		}
	case IDM_SHIELD_USER:				//�����û�
		{
			//Ч������
			if ((m_dwCurrentUserID==0L)||(m_dwCurrentUserID==GetMeUserInfo()->dwUserID)) 
				return TRUE;

			//��������
			INT_PTR i=0;
			for (i=0;i<m_ShieldArray.GetCount();i++)
			{
				if ((m_ShieldArray.GetAt(i)==0L)||(m_ShieldArray.GetAt(i)==m_dwCurrentUserID))
					break;
			}
			if (i==m_ShieldArray.GetCount()) 
				m_ShieldArray.SetAtGrow(i,m_dwCurrentUserID);

			//������Ϣ
			UserItemStruct * pUserItem=FindOnLineUser(m_dwCurrentUserID);
			if (pUserItem!=NULL)
			{
				CString strBuffer;
				strBuffer.Format(TEXT("������� [ %s ] �ɹ�"),pUserItem->GameUserInfo.nickName);
				m_MessageHandle.InsertSystemMessage(strBuffer.GetBuffer());
				strBuffer.ReleaseBuffer();
			}

			return TRUE;
		}
	case IDM_UNSHIELD_USER:				//��������û�
		{
			//Ч������
			if ((m_dwCurrentUserID==0L)||(m_dwCurrentUserID==GetMeUserInfo()->dwUserID)) 
				return TRUE;

			//��������
			for (INT_PTR i=0;i<m_ShieldArray.GetCount();i++)
			{
				if (m_ShieldArray.GetAt(i)==m_dwCurrentUserID) 
				{
					m_ShieldArray.RemoveAt(i);
					break;
				}
			}

			//������Ϣ
			UserItemStruct * pUserItem=FindOnLineUser(m_dwCurrentUserID);
			if (pUserItem!=NULL)
			{
				CString strBuffer;
				strBuffer.Format(TEXT("����������[%s]�ɹ�"),pUserItem->GameUserInfo.nickName);
				m_MessageHandle.InsertSystemMessage(strBuffer.GetBuffer());
				strBuffer.ReleaseBuffer();
			}

			return TRUE;
		}
	case IDM_RED:						
	case IDM_BULE:
	case IDM_GRAY:
	case IDM_GREEN:
	case IDM_WHITE:
	case IDM_YELLOW:
	case IDM_POWER_RED:					//����������ɫ
		{
			UINT uColorMenuID[7]={IDM_RED,IDM_YELLOW,IDM_BULE,IDM_GREEN,IDM_WHITE,IDM_POWER_RED,IDM_GRAY};
			COLORREF rcColor[8]={RGB(255,0,0),RGB(255,255,0),RGB(0,255,255),RGB(0,255,0),RGB(255,255,255),RGB(255,0,255),RGB(192,192,192),RGB(255,255,255)};
			UINT i=0;
			for (i=0;i<sizeof(uColorMenuID)/sizeof(uColorMenuID[0]);i++)
			{
				if (LOWORD(wParam)==uColorMenuID[i]) break;
			}
			CGameImageLink::m_ColorResource.m_crTalkTxColor=rcColor[i];
			m_InputMessage.SetFocus();
			return TRUE;
		}
	}
	return __super::OnCommand(wParam, lParam);
}
// ����û��б�
void CLoveSendClassInExe::OnLBClickUserList(UserItemStruct * pUserItem)
{
	if(m_pPropPannel != NULL)
	{
		m_pPropPannel->m_ctlGiveUser.SetWindowText(pUserItem->GameUserInfo.nickName);
		//m_pPropPannel->UpdateData(true);
	}
	if(m_CurpUserInfoClicked==&pUserItem->GameUserInfo)
		return ;
	m_CurpUserInfoClicked=&pUserItem->GameUserInfo;
	NavigateVirFace();
}
// ˫���û��б�
void CLoveSendClassInExe::OnLBDBClickUserList(UserItemStruct * pUserItem)
{
	if(GetMeUserInfo()->dwUserID==pUserItem->GameUserInfo.dwUserID)
		return;
	m_CurpUserInfoClicked=&pUserItem->GameUserInfo;
	CString nick=pUserItem->GameUserInfo.nickName;
	m_InputMessage.SetWindowText(nick+":");
	m_InputMessage.SetFocus();
	int l=nick.GetLength()+1;
	m_InputMessage.SetEditSel(l,l);

}
//���Ҽ��˵�
void CLoveSendClassInExe::OpenUserMenu(UserItemStruct * pUserItem)
{
	// add duanxiaohui 20111111 ����������ҹ���(����ʹ��, �����ο�����), �ڴ�����һ���µĺ���
	return OpenUserMenuEx(pUserItem);
	// end duanxiaohui

	//Ч������
	if (pUserItem==NULL) 
	{
		m_dwCurrentUserID=0L;
		return;
	}
	//��������
	UserInfoStruct * pUserInfo=&pUserItem->GameUserInfo;
	if (!m_pRightFrame)
	{
		m_dwCurrentUserID=pUserItem->GameUserInfo.dwUserID;
	}

	//�����˵�
	CString strBuffer;
	CMenu UserInfoMenu;
	UserInfoMenu.CreatePopupMenu();
	MENUINFO mi;
	UserInfoMenu.GetMenuInfo(&mi);
	mi.cbSize=sizeof(MENUINFO);
	mi.fMask=MIM_BACKGROUND; 
	mi.hbrBack = CreateSolidBrush(RGB(0xD1,0xF1,0xFF));
	UserInfoMenu.SetMenuInfo(&mi);

	//���ܲ˵�
	UserInfoMenu.AppendMenu(0,IDM_COPY_USER_NAME,TEXT("�����û���"));

	//////////////////////////////////////////////////////////////////////////
	//��ʱ���� by fred huang for ���ܶ� 20090403
	if(m_dwCurrentUserID != GetMeUserInfo()->dwUserID)
	{
		UserInfoMenu.AppendMenu(0,IDM_SET_FRIEND,TEXT("��Ӻ���"));
		//UserInfoMenu.AppendMenu(0,IDM_GIVE_PROP,TEXT("���͵���..."));
	}
	//////////////////////////////////////////////////////////////////////////
	//�������˹���
	/// �ټ������͵���Ϸ�����˹���
	CString stime;
	stime.Format("%d",CTime::GetCurrentTime());        //��ǰ��ʱ�䣬String��ʽ
	int curtime=atoi(stime);                        //ת��Ϊint
	TCHAR szKey[10];
	CString sPath=CBcfFile::GetAppPath();
	CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
	wsprintf(szKey, "%d", m_pGameInfo->uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

	if (iResult==0)
	{
		if(GetMeUserInfo()->iVipTime>curtime)//��Ա���д˹���
        {
			UserInfoMenu.AppendMenu(0,IDM_KICK_USER,TEXT("�����뿪"));
        }
        else        // PengJiLin, 2010-9-10, �������Ƿ������˿������˹���
        {
            CBcfFile bcfFile( sPath + "..\\Order.bcf");
            int iValue = bcfFile.GetKeyVal(_T("ClassInExe"), _T("UseKickUserProp"), 0);
            if(iValue > 0)  // ��ʾ���˲˵�
            {
                UserInfoMenu.AppendMenu(0,IDM_KICK_USER_PROP,TEXT("�����뿪"));
            }
        }
	}

	//if (m_dwCurrentUserID!=GetMeUserInfo()->dwUserID)
	//{
	//	strBuffer.Format(TEXT("�� %s ����"),pUserInfo->nickName);
	//	//UserInfoMenu.AppendMenu(0,IDM_SET_TALK_OBJECT,strBuffer);
	//}
	//int iStation=m_TargetPeople.GetCurSel();
	//if ((iStation!=CB_ERR)&&(iStation!=0))
	//{
	//	CString strName;
	//	m_TargetPeople.GetLBText(iStation,strName);
	//	strBuffer.Format(TEXT("ȡ���� %s ������"),strName);
	//	//UserInfoMenu.AppendMenu(0,IDM_CANCEL_TALK,strBuffer);
	//}
	//�������ι���
	INT_PTR i;
	for (i=0;i<m_ShieldArray.GetCount();i++)
	{
		if (m_ShieldArray.GetAt(i)==m_dwCurrentUserID) break;
	}
	if (i==m_ShieldArray.GetCount()) 
	{
		if (m_dwCurrentUserID!=GetMeUserInfo()->dwUserID)
		{
			strBuffer.Format(TEXT("���� %s ����Ϣ"),pUserInfo->nickName);
			UserInfoMenu.AppendMenu(0,IDM_SHIELD_USER,strBuffer);
		}
	}
	else
	{
		strBuffer.Format(TEXT("��� %s ����Ϣ����"),pUserInfo->nickName);
		UserInfoMenu.AppendMenu(0,IDM_UNSHIELD_USER,strBuffer);
	}

	//���������Թ�
	if ((m_bWatchMode==false)&&(pUserInfo->bDeskStation==GetMeUserInfo()->bDeskStation)
		&&(m_dwCurrentUserID!=GetMeUserInfo()->dwUserID))
	{
		UserInfoMenu.AppendMenu(0,MF_SEPARATOR);
		strBuffer.Format(TEXT("���� %s �Թ���Ϸ"),pUserInfo->nickName);
		UserInfoMenu.AppendMenu(0,IDM_EMABLE_WATCH,strBuffer);
		strBuffer.Format(TEXT("������ %s �Թ���Ϸ"),pUserInfo->nickName);
		UserInfoMenu.AppendMenu(0,IDM_UNEMABLE_WATCH,strBuffer);
	}


	//�û���Ϣ�˵�
	//UINT uAllCount=pUserInfo->uWinCount+pUserInfo->uLostCount+pUserInfo->uCutCount+pUserInfo->uMidCount;		//���ڶ��ߡ�ǿ�ˣ������ݿ��������֣����У����ܾ����ﲻ�ٰѶ��ߡ�ǿ��������㷶Χ zxj 2009-9-29
	UINT uAllCount=pUserInfo->uWinCount+pUserInfo->uLostCount+pUserInfo->uMidCount;

	UINT uCutPercent=(pUserInfo->uCutCount>0)?(UINT)((double)(pUserInfo->uCutCount*100)/(double)uAllCount):0;
	UserInfoMenu.AppendMenu(0,MF_SEPARATOR);


	if(pUserInfo->iVipTime>curtime)
		strBuffer.Format("�û�����%s ( VIP ��Ա )",pUserInfo->nickName);
	else
		strBuffer.Format("�û�����%s",pUserInfo->nickName);
	/*
	if (pUserInfo->bMember==0) strBuffer.Format(TEXT("�û�����%s"),pUserInfo->nickName);
	else 
	if (pUserInfo->bMember==1) strBuffer.Format(TEXT("�û�����%s ( ��ͨ��Ա )"),pUserInfo->nickName);
	else 
	if (pUserInfo->bMember==2) strBuffer.Format(TEXT("�û�����%s ( �߼���Ա )"),pUserInfo->nickName);
	else
	if (pUserInfo->bMember==3) strBuffer.Format(TEXT("�û�����%s ( �����α� )"),pUserInfo->nickName);
	else 
	if (pUserInfo->bMember==4) strBuffer.Format(TEXT("�û�����%s ( ���û�Ա )"),pUserInfo->nickName);
	else 
	strBuffer.Format(TEXT("�û�����%s ( Ԫ����� )"),pUserInfo->nickName);*/
	UserInfoMenu.AppendMenu(0,0,strBuffer);



	strBuffer.Format(TEXT("���֣�%d"),pUserInfo->dwPoint-GetNormalOrderBasePoint(pUserInfo->dwPoint));
	UserInfoMenu.AppendMenu(0,0,strBuffer);
	strBuffer.Format(TEXT("�ȼ���%s"),m_pGameInfo->pOrderName(pUserInfo->dwPoint));
	UserInfoMenu.AppendMenu(0,0,strBuffer);

    // PengJiLin, 2010-6-11, �����Ÿ����ʽ����Щ�˵�����ʾ
    BYTE byType = m_UserListDlg.m_UserList.GetUserListDefineType();
    TCHAR szNum[128]; 
    switch(byType)
    {
    case 1:
        break;
    default:
        GlbGetNumString(szNum, pUserInfo->i64Money, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
        strBuffer.Format(TEXT("��ң�%s"),szNum);
        UserInfoMenu.AppendMenu(0,0,strBuffer);
        strBuffer.Format(TEXT("�Ƹ�����%s"),GetMoneyOrderName(pUserInfo->i64Money));
        UserInfoMenu.AppendMenu(0,0,strBuffer);
        strBuffer.Format(TEXT("�ܾ�����%d"),uAllCount);
        UserInfoMenu.AppendMenu(0,0,strBuffer);
        strBuffer.Format(TEXT("�����ʣ�%d"),uCutPercent);
        UserInfoMenu.AppendMenu(0,0,strBuffer);
        break;
    }
	
	/*	if (pUserInfo->szClassName[0]!=0)
	{
	strBuffer.Format(TEXT("���ţ�%s"),pUserInfo->szClassName);
	UserInfoMenu.AppendMenu(0,0,strBuffer);
	}*/
	if (CUserPower::CanLookIP(m_pGameInfo->dwMasterPower)==true)
	{
		ULONG dwUserIP=pUserInfo->dwUserIP;
		strBuffer.Format(TEXT("�û� IP��%d.%d.%d.%d"),FOURTH_IPADDRESS(dwUserIP),THIRD_IPADDRESS(dwUserIP),
			SECOND_IPADDRESS(dwUserIP),FIRST_IPADDRESS(dwUserIP));
		UserInfoMenu.AppendMenu(0,0,strBuffer);
	}
	if (pUserInfo->bGameMaster!=0)
	{
		if (pUserInfo->bGameMaster==1) UserInfoMenu.AppendMenu(0,0,TEXT("����ְ�ƣ��ڲ�����Ա"));
		else if (pUserInfo->bGameMaster==2) UserInfoMenu.AppendMenu(0,0,TEXT("����ְ�ƣ��ⲿ����Ա"));
		else if (pUserInfo->bGameMaster==3) UserInfoMenu.AppendMenu(0,0,TEXT("����ְ�ƣ���������Ա"));
		else if (pUserInfo->bGameMaster==4) UserInfoMenu.AppendMenu(0,0,TEXT("����ְ�ƣ�����"));
		else if (pUserInfo->bGameMaster==5) UserInfoMenu.AppendMenu(0,0,TEXT("����ְ�ƣ���̫������"));
		else if (pUserInfo->bGameMaster==6) UserInfoMenu.AppendMenu(0,0,TEXT("����ְ�ƣ��ϵ�"));
	}

	//��ʾ�˵�
	POINT MousePoint;
	GetCursorPos(&MousePoint);
	UserInfoMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,MousePoint.x,MousePoint.y,this);
	DeleteObject(mi.hbrBack);
	UserInfoMenu.DestroyMenu();
	return;
}

// ���Ҽ��˵�
void CLoveSendClassInExe::OpenUserMenuEx(UserItemStruct * pUserItem)
{
	// Ч������
	if (pUserItem==NULL) 
	{
		m_dwCurrentUserID = 0L;
		return;
	}

	// ��������
	UserInfoStruct * pUserInfo = &pUserItem->GameUserInfo;
	UserInfoStruct * pMeUserInfo = GetMeUserInfo();
	if (!m_pRightFrame)
	{
		m_dwCurrentUserID = pUserItem->GameUserInfo.dwUserID;
	}

	CBcfFile fMsg(CBcfFile::GetAppPath() + "ClientMessage.bcf");

	// �����˵�
	CGameMenu * pMenu = new CGameMenu;
	pMenu->CreatePopupMenu();
	pMenu->SetSelectedBarColor(RGB(0x66, 0x99, 0xff));
	pMenu->SetBackColor(RGB(227, 243, 251));
	pMenu->SetSideBarStartColor(RGB(40, 119, 183));
	pMenu->SetSideBarEndColor(RGB(40, 119, 183));
	pMenu->SetTextSpace(-10);
	pMenu->AddSideBar(new CGameMenuSideBar(14, ""));

	// PengJiLin, 2010-8-17, �Ƿ��ֹ˽��
	int iForbitSay = 8 & Glb().m_CenterServerPara.m_nFunction;

	CString str, strBuffer;

	/// �ټ������͵���Ϸ�����˹���
	CString stime;
	stime.Format("%d",CTime::GetCurrentTime());        // ��ǰ��ʱ�䣬String��ʽ
	int curtime = atoi(stime);                         // ת��Ϊint
	TCHAR szKey[10];
	CString sPath = CBcfFile::GetAppPath();
	CBcfFile fsr(sPath + "..\\SpecialRule.bcf");
	wsprintf(szKey, "%d", m_pGameInfo->uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

	if (/*iResult == 0 && */pMeUserInfo->dwUserID != m_dwCurrentUserID)
	{
		if(GetMeUserInfo()->iVipTime > curtime) // ��Ա���д˹���
		{
			pMenu->AppendODMenu(IDM_KICK_USER, TEXT("�����뿪"));
			pMenu->AppendSeparator();
		}
		else  // PengJiLin, 2010-9-10, �������Ƿ������˿������˹���
		{
			CBcfFile bcfFile(sPath + "..\\Order.bcf");
			int iValue = bcfFile.GetKeyVal(_T("ClassInExe"), _T("UseKickUserProp"), 0);
			if(iValue > 0)  // ��ʾ���˲˵�
			{
				pMenu->AppendODMenu(IDM_KICK_USER_PROP, TEXT("�����뿪"));
				pMenu->AppendSeparator();
			}
		}
	}

	pMenu->AppendODMenu(IDM_USER_INFO, "��ϸ��Ϣ");
	pMenu->AppendODMenu(IDM_COPY_USER_NAME, "�����û���");

	//���ܲ˵�
	if (!iForbitSay && pMeUserInfo->dwUserID != m_dwCurrentUserID  && !(m_pGameInfo->dwRoomRule & GRR_AUTO_SIT_DESK))
	{
		pMenu->AppendSeparator();

		str = fMsg.GetKeyVal("GameRoom", "AddFriend", TEXT("��Ϊ����"));
		pMenu->AppendODMenu(IDM_SET_FRIEND, str);

		str = fMsg.GetKeyVal("GameRoom", "AddEmeny", TEXT("���������"));
		pMenu->AppendODMenu(IDM_SET_EMENY, str);
	}

	// ��ʾ�˵�
	POINT point;
	::GetCursorPos(&point);
	pMenu->TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, this);
	delete pMenu;
	pMenu = NULL;
}

void CLoveSendClassInExe::OnGetBuyItemData(int iPropID)
{
	this->m_pPropSelUser;
	if(!m_pPropPannel)
	{
		m_pPropPannel=new CPropPannel();
		m_pPropPannel->m_pGameInfo = this->m_pGameInfo;
		m_pPropPannel->m_pParent = this;
		m_pPropPannel->m_bUsingNewUI = false;
		AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
		m_pPropPannel->Create(IDD_PROPPANNEL,this);
		AfxSetResourceHandle(GetModuleHandle(NULL));
		m_pPropPannel->CenterWindow();
	}
	m_pPropPannel->openPannel(0,"", iPropID);
	m_pPropPannel->BringWindowToTop();
	m_pPropPannel->Invalidate();
}

void CLoveSendClassInExe::OnBnClickedGameBtn01()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnGameSetting();
	return ;
}
// ������ߣ��򿪵������
void CLoveSendClassInExe::OnBnClickedGameBtn02()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���ΰټ�����ع��� add by wlr 20090716
	char GameCode[20];
	::memset(GameCode,0,sizeof(GameCode));
	GetGameCode(GameCode);
	//if (str == "11901800"
	//	|| str == "10901800"
	//	|| str == "30501800")
	TCHAR szKey[10];
	CString sPath=CBcfFile::GetAppPath();
	CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
	wsprintf(szKey, "%d", m_pGameInfo->uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

	if (iResult>0)
		return ;
	//end of add by wlr 20090716

    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString s=CBcfFile::GetAppPath () + "..\\";
    CBcfFile f( s + "bzgame.bcf");
    CString url;
    url=f.GetKeyVal("BZW","gamepropurl","http://www.kfgame.com");

    //CString surl;
    //surl.Format("%s/app/userlogin.asp?userid=%d&token=%s&url=%s",
    //	Glb().m_CenterServerPara.m_strWebRootADDR,m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID,Glb().m_TML_SN,url);
    //surl.Format("%s/app/userlogin.asp?userid=%d&token=",
    //            url,m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID);
    //ShellExecute(NULL,"open",surl,NULL,NULL,SW_MAXIMIZE|SW_SHOW);
    ShellExecute(NULL,"open",url,NULL,NULL,SW_MAXIMIZE|SW_SHOW);
	return ;
}


void CLoveSendClassInExe::OnBnClickedGameBtn03()
{
	//���ΰټ�����ع��� add by wlr 20090716
	char GameCode[20];
	::memset(GameCode,0,sizeof(GameCode));
	GetGameCode(GameCode);
	CString str = GameCode;
	//if (str == "11901800"
	//	|| str == "10901800"
	//	|| str == "30501800")
	TCHAR szKey[10];
	CString sPath=CBcfFile::GetAppPath();
	CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
	wsprintf(szKey, "%d", m_pGameInfo->uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

	if (iResult>0)
		return ;
	//end of add by wlr 20090716

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString s=CBcfFile::GetAppPath () + "..\\";// CINIFile::GetAppPath ();/////����·��
	CBcfFile f( s + "bzgame.bcf");
	CString url;
	url=f.GetKeyVal("BZW","gameurl1",Glb().m_CenterServerPara.m_strHomeADDR);

	CString surl;
	//surl.Format("%s/app/userlogin.asp?userid=%d&token=%s&url=%s",
	//	Glb().m_CenterServerPara.m_strWebRootADDR,m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID,Glb().m_TML_SN,url);
	surl.Format("%s/app/userlogin.asp?userid=%d&token=",
		url,m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID);
	DebugPrintf(surl);
	//ShellExecute(NULL,"open",surl,NULL,NULL,SW_MAXIMIZE|SW_SHOW);
	ShellExecute(NULL,"open",url,NULL,NULL,SW_MAXIMIZE|SW_SHOW);
	return ;
}

void CLoveSendClassInExe::OnBnClickedGameBtn04()
{
	//���ΰټ�����ع��� add by wlr 20090716
	char GameCode[20];
	::memset(GameCode,0,sizeof(GameCode));
	GetGameCode(GameCode);
	//if (str == "11901800"
	//	|| str == "10901800"
	//	|| str == "30501800")
	TCHAR szKey[10];
	CString sPath=CBcfFile::GetAppPath();
	CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
	wsprintf(szKey, "%d", m_pGameInfo->uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

	if (iResult>0)
		return ;
	//end of add by wlr 20090716

	CString s= CBcfFile::GetAppPath () + "..\\";//INIFile::GetAppPath ();/////����·��
	CBcfFile f( s + "bzgame.bcf");
	CString url;
	url=f.GetKeyVal("BZW","gameurl2",Glb().m_CenterServerPara.m_strHomeADDR);

	CString surl;
	//surl.Format("%s/app/userlogin.asp?userid=%d&token=%s&url=%s",
	//	Glb().m_CenterServerPara.m_strWebRootADDR,m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID,Glb().m_TML_SN,url);

	DebugPrintf(surl);
	//ShellExecute(NULL,"open",surl,NULL,NULL,SW_MAXIMIZE|SW_SHOW);
	ShellExecute(NULL,"open",url,NULL,NULL,SW_MAXIMIZE|SW_SHOW);
}
// ʹ�õ���     // PengJiLin, 2010-10-14, ����һ����
void CLoveSendClassInExe::UsePropItem(int propIndex, BOOL bCheckNum)
{
	//////////////////////////////////////////////////////////////////////////
	//Modify Fred Huang,2008-04-23
	if(GetMeUserInfo()->bUserState==USER_WATCH_GAME)
	{
		AFCMessageBox("�Բ����Թ�ʱ����ʹ�õ��ߣ�","��ʾ");
		return;
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//2008-08-11
	int propCount=(int)Glb().userPropLibrary.GetCount();
	if(propIndex<0 || propIndex>=propCount)
	{
		//û�е��ߣ��������
		OnGetBuyItemData(1);
		return;
	}
	_TAG_USERPROP * userProp=Glb().userPropLibrary.GetAt(propIndex);
	if(userProp==NULL) return;

    // PengJiLin, 2010-10-14, ���ߵ���������Ϊ0���봦��
    if(userProp->nHoldCount <= 0 && TRUE == bCheckNum)
    {
        TCHAR chInfo[256] = {0};
        wsprintf(chInfo, "��Ǹ��[%s]�Ѿ�ʹ����ϣ��Ƿ����ڹ���ʹ��?", userProp->szPropName);
        if(IDOK == AFCMessageBox(chInfo, TEXT("��ʾ"), AFC_YESNO, this))  // ȷ������
        {
            BuyAndUseProp(propIndex);
            return;
        }
        else        // ȡ������
        {
            return;
        }
    }

	if(userProp->attribAction & 0x80 || userProp->attribAction &0x100)
	{//�����Ⱥ�С����
		this->UseBoardCast( userProp );
		return;
	}

    // PengJiLin, 2010-6-1, ������ ����
    if(userProp->attribAction & 0x200)
    {
        this->UseLookCardProp(userProp);
        return;
    }

    // PengJiLin, 2010-9-9, ���˿�����
    if(userProp->attribAction & 0x400)
    {
        this->UseKickUserProp(userProp);
        return;
    }

    // PengJiLin, 2010-9-9, ���߿�����
    if(userProp->attribAction & 0x800)
    {
        this->UseAntiKickProp(userProp);
        return;
    }

	//���ֶ��Լ�ʹ�ã����Ƕ�������ʹ��
	if(userProp->attribAction & 0x01)
	{//��������ʹ��
		if(m_pPropSelUser==NULL)
		{
			m_pPropSelUser=new CPropSelUser;
			AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
			m_pPropSelUser->Create(IDD_PROP_SEL_USER,this);
			AfxSetResourceHandle(GetModuleHandle(NULL));
		}
		for(int i=0;i<MAX_PEOPLE;i++)
			m_pPropSelUser->m_pUserItem[i]=m_pUserInfo[i];
		m_pPropSelUser->dwUsedPropID=propIndex;
		m_pPropSelUser->dwLocalUserID=GetMeUserInfo()->dwUserID;
		m_pPropSelUser->propItemName=userProp->szPropName;
		m_pPropSelUser->resetInformation();
		m_pPropSelUser->CenterWindow();
		m_pPropSelUser->ShowWindow(SW_SHOW);		
	}
	else
	{//���Լ�ʹ��
		/*
		CString stip;
		stip.Format("��ȷ��Ҫʹ�õ��� %s ��?",userProp->szPropName);
		if(userProp->attribAction & 0x2)
		stip+="\n�Ὣ���ĸ���������!";

		*/
		////////////////////////////////////////////////////
		///2008 1216 Kylin ����ʱʹ�ø�������û����ʾ
		CString stip;
		stip.Format("��ȷ��Ҫʹ�õ��� %s ��?",userProp->szPropName);
		if(userProp->attribAction & 0x2)
		{
			if(GetMeUserInfo()->dwPoint>=0)
			{
				stip.Format("���Ļ���Ϊ����ʹ�õ��� %s ����������һ�����߶�û��Ч����ȷ��ʹ����?",userProp->szPropName);
			}
			else stip+="\n�Ὣ���ĸ���������!";
		}

		UINT uRet = AFCMessageBox(stip,"��ʾ",AFC_YESNO,this);
		if(uRet==IDOK)
		{
			OnUseProp(propIndex,GetMeUserInfo()->dwUserID);
		}
		////////////////////////////////////////////////////
	}
	return;
}
void CLoveSendClassInExe::UseBoardCast(_TAG_USERPROP * userProp)
{
	//�Ƚ�������
	AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
	if(!m_dlgBroadcast)
	{
		m_dlgBroadcast=new CBoardCast();;
		m_dlgBroadcast->Create(IDD_BOARDCAST,GetDesktopWindow());
		m_dlgBroadcast->pParentWnd=(CWnd*)this;
	}

	m_dlgBroadcast->m_nPropID=userProp->nPropID;

	if(userProp->attribAction & 0x80)
		m_dlgBroadcast->SetValue(BRD_MSG_BIG);
	else
		m_dlgBroadcast->SetValue(BRD_MSG_SMALL);
	AfxSetResourceHandle(GetModuleHandle(NULL));
	m_dlgBroadcast->ShowWindow(SW_SHOW);
}
void CLoveSendClassInExe::OnBnClickedButtonProp1()
{
	// duanxiaohui 20111103 ���鳡(��ѳ�) ����ʹ�õ���
	if (m_pGameInfo->dwRoomRule & GRR_EXPERCISE_ROOM)
	{
		AFCMessageBox("�Բ���, ���鳡(��ѳ�)����ʹ����Ϸ����", "��ܰ��ʾ");
		return ;
	}
	// end duanxiaohui

	//���ΰټ�����ع��� add by wlr 20090716
	char GameCode[20];
	::memset(GameCode,0,sizeof(GameCode));
	GetGameCode(GameCode);
	//if (str == "11901800"
	//	|| str == "10901800"
	//	|| str == "30501800")
	TCHAR szKey[10];
	CString sPath=CBcfFile::GetAppPath();
	CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
	wsprintf(szKey, "%d", m_pGameInfo->uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

	if (iResult>0)
	{
		AFCMessageBox("�Բ���,�޷��ڰ�������Ϸʹ�õ���","��ʾ");
		return ;
	}
	//end of add by wlr 20090716

	//UsePropItem(m_curPropPage*PEER_PAGE+0);
	OnGetBuyItemData(9999);
	return;
}

void CLoveSendClassInExe::OnBnClickedButtonProp2()
{
	// duanxiaohui 20111103 ���鳡(��ѳ�) ����ʹ�õ���
	if (m_pGameInfo->dwRoomRule & GRR_EXPERCISE_ROOM)
	{
		AFCMessageBox("�Բ���, ���鳡(��ѳ�)����ʹ����Ϸ����", "��ܰ��ʾ");
		return ;
	}
	// end duanxiaohui

	//���ΰټ�����ع��� add by wlr 20090716
	char GameCode[20];
	::memset(GameCode,0,sizeof(GameCode));
	GetGameCode(GameCode);
	//if (str == "11901800"
	//	|| str == "10901800"
	//	|| str == "30501800")
	TCHAR szKey[10];
	CString sPath=CBcfFile::GetAppPath();
	CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
	wsprintf(szKey, "%d", m_pGameInfo->uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

	if (iResult>0)
	{
		AFCMessageBox("�Բ���,�޷��ڰ�������Ϸʹ�õ���","��ʾ");
		return ;
	}
	//end of add by wlr 20090716

	//UsePropItem(m_curPropPage*PEER_PAGE+1);
	OnGetBuyItemData(1);
	return;
}

void CLoveSendClassInExe::OnBnClickedButtonProp3()
{
	// duanxiaohui 20111103 ���鳡(��ѳ�) ����ʹ�õ���
	if (m_pGameInfo->dwRoomRule & GRR_EXPERCISE_ROOM)
	{
		AFCMessageBox("�Բ���, ���鳡(��ѳ�)����ʹ����Ϸ����", "��ܰ��ʾ");
		return ;
	}
	// end duanxiaohui


	//���ΰټ�����ع��� add by wlr 20090716
	char GameCode[20];
	::memset(GameCode,0,sizeof(GameCode));
	GetGameCode(GameCode);
	//if (str == "11901800"
	//	|| str == "10901800"
	//	|| str == "30501800")
	TCHAR szKey[10];
	CString sPath=CBcfFile::GetAppPath();
	CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
	wsprintf(szKey, "%d", m_pGameInfo->uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

	if (iResult>0)
	{
		AFCMessageBox("�Բ���,�޷��ڰ�������Ϸʹ�õ���","��ʾ");
		return ;
	}
	//end of add by wlr 20090716

	//UsePropItem(m_curPropPage*PEER_PAGE+2);
	OnGetBuyItemData(2);
	return;
}

// PengJiLin, 2010-10-12, �̳����߹���
void CLoveSendClassInExe::OnBnClickedButtonProp4()
{
	// duanxiaohui 20111103 ���鳡(��ѳ�) ����ʹ�õ���
	if (m_pGameInfo->dwRoomRule & GRR_EXPERCISE_ROOM)
	{
		AFCMessageBox("�Բ���, ���鳡(��ѳ�)����ʹ����Ϸ����", "��ܰ��ʾ");
		return ;
	}
	// end duanxiaohui


    //���ΰټ�����ع��� add by wlr 20090716
    char GameCode[20];
    ::memset(GameCode,0,sizeof(GameCode));
    GetGameCode(GameCode);
    //if (str == "11901800"
    //	|| str == "10901800"
    //	|| str == "30501800")
    TCHAR szKey[10];
    CString sPath=CBcfFile::GetAppPath();
    CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
    wsprintf(szKey, "%d", m_pGameInfo->uNameID);
    int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

	if (iResult>0)
	{
		AFCMessageBox("�Բ���,�޷��ڰ�������Ϸʹ�õ���","��ʾ");
		return ;
	}
    //end of add by wlr 20090716

    //BuyAndUseProp(m_curPropPage_S*PEER_PAGE+0);
	OnGetBuyItemData(3);
}
void CLoveSendClassInExe::OnBnClickedButtonProp5()
{
	// duanxiaohui 20111103 ���鳡(��ѳ�) ����ʹ�õ���
	if (m_pGameInfo->dwRoomRule & GRR_EXPERCISE_ROOM)
	{
		AFCMessageBox("�Բ���, ���鳡(��ѳ�)����ʹ����Ϸ����", "��ܰ��ʾ");
		return ;
	}
	// end duanxiaohui


    //���ΰټ�����ع��� add by wlr 20090716
    char GameCode[20];
    ::memset(GameCode,0,sizeof(GameCode));
    GetGameCode(GameCode);
    //if (str == "11901800"
    //	|| str == "10901800"
    //	|| str == "30501800")
    TCHAR szKey[10];
    CString sPath=CBcfFile::GetAppPath();
    CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
    wsprintf(szKey, "%d", m_pGameInfo->uNameID);
    int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

	if (iResult>0)
	{
		AFCMessageBox("�Բ���,�޷��ڰ�������Ϸʹ�õ���","��ʾ");
		return ;
	}
    //end of add by wlr 20090716

    //BuyAndUseProp(m_curPropPage_S*PEER_PAGE+1);
	OnGetBuyItemData(4);
}
void CLoveSendClassInExe::OnBnClickedButtonProp6()
{
	// duanxiaohui 20111103 ���鳡(��ѳ�) ����ʹ�õ���
	if (m_pGameInfo->dwRoomRule & GRR_EXPERCISE_ROOM)
	{
		AFCMessageBox("�Բ���, ���鳡(��ѳ�)����ʹ����Ϸ����", "��ܰ��ʾ");
		return ;
	}
	// end duanxiaohui


    //���ΰټ�����ع��� add by wlr 20090716
    char GameCode[20];
    ::memset(GameCode,0,sizeof(GameCode));
    GetGameCode(GameCode);
    //if (str == "11901800"
    //	|| str == "10901800"
    //	|| str == "30501800")
    TCHAR szKey[10];
    CString sPath=CBcfFile::GetAppPath();
    CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
    wsprintf(szKey, "%d", m_pGameInfo->uNameID);
    int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

	if (iResult>0)
	{
		AFCMessageBox("�Բ���,�޷��ڰ�������Ϸʹ�õ���","��ʾ");
		return ;
	}
    //end of add by wlr 20090716

    //BuyAndUseProp(m_curPropPage_S*PEER_PAGE+2);
	OnGetBuyItemData(5);
}

void CLoveSendClassInExe::OnBnClickedButtonProp7()
{
	// duanxiaohui 20111103 ���鳡(��ѳ�) ����ʹ�õ���
	if (m_pGameInfo->dwRoomRule & GRR_EXPERCISE_ROOM)
	{
		AFCMessageBox("�Բ���, ���鳡(��ѳ�)����ʹ����Ϸ����", "��ܰ��ʾ");
		return ;
	}
	// end duanxiaohui


	//���ΰټ�����ع��� add by wlr 20090716
	char GameCode[20];
	::memset(GameCode,0,sizeof(GameCode));
	GetGameCode(GameCode);

	TCHAR szKey[10];
	CString sPath=CBcfFile::GetAppPath();
	CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
	wsprintf(szKey, "%d", m_pGameInfo->uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

	if (iResult>0)
	{
		AFCMessageBox("�Բ����޷��ڰ�������Ϸʹ�õ���","��ʾ");
		return ;
	}
	//end of add by wlr 20090716

	OnGetBuyItemData(6);
	return;
}

void CLoveSendClassInExe::OnBnClickedButtonProp8()
{
	// duanxiaohui 20111103 ���鳡(��ѳ�) ����ʹ�õ���
	if (m_pGameInfo->dwRoomRule & GRR_EXPERCISE_ROOM)
	{
		AFCMessageBox("�Բ���, ���鳡(��ѳ�)����ʹ����Ϸ����", "��ܰ��ʾ");
		return ;
	}
	// end duanxiaohui


	//���ΰټ�����ع��� add by wlr 20090716
	char GameCode[20];
	::memset(GameCode,0,sizeof(GameCode));
	GetGameCode(GameCode);

	TCHAR szKey[10];
	CString sPath=CBcfFile::GetAppPath();
	CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
	wsprintf(szKey, "%d", m_pGameInfo->uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

	if (iResult>0)
	{
		AFCMessageBox("�Բ���,�޷��ڰ�������Ϸʹ�õ���","��ʾ");
		return ;
	}
	//end of add by wlr 20090716

	OnGetBuyItemData(7);
	return;
}

void CLoveSendClassInExe::OnBnClickedButtonProp9()
{
	// duanxiaohui 20111103 ���鳡(��ѳ�) ����ʹ�õ���
	if (m_pGameInfo->dwRoomRule & GRR_EXPERCISE_ROOM)
	{
		AFCMessageBox("�Բ���, ���鳡(��ѳ�)����ʹ����Ϸ����", "��ܰ��ʾ");
		return ;
	}
	// end duanxiaohui


	//���ΰټ�����ع��� add by wlr 20090716
	char GameCode[20];
	::memset(GameCode,0,sizeof(GameCode));
	GetGameCode(GameCode);

	TCHAR szKey[10];
	CString sPath=CBcfFile::GetAppPath();
	CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
	wsprintf(szKey, "%d", m_pGameInfo->uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

	if (iResult>0)
	{
		AFCMessageBox("�Բ���,�޷��ڰ�������Ϸʹ�õ���","��ʾ");
		return ;
	}
	//end of add by wlr 20090716

	OnGetBuyItemData(8);
	return;
}

void CLoveSendClassInExe::OnBnClickedButtonProp10()
{
	// duanxiaohui 20111103 ���鳡(��ѳ�) ����ʹ�õ���
	if (m_pGameInfo->dwRoomRule & GRR_EXPERCISE_ROOM)
	{
		AFCMessageBox("�Բ���, ���鳡(��ѳ�)����ʹ����Ϸ����", "��ܰ��ʾ");
		return ;
	}
	// end duanxiaohui


	//���ΰټ�����ع��� add by wlr 20090716
	char GameCode[20];
	::memset(GameCode,0,sizeof(GameCode));
	GetGameCode(GameCode);

	TCHAR szKey[10];
	CString sPath=CBcfFile::GetAppPath();
	CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
	wsprintf(szKey, "%d", m_pGameInfo->uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

	if (iResult>0)
	{
		AFCMessageBox("�Բ���,�޷��ڰ�������Ϸʹ�õ���","��ʾ");
		return ;
	}
	//end of add by wlr 20090716

	OnGetBuyItemData(9);
	return;
}

void CLoveSendClassInExe::BuyAndUseProp(int propIndex)
{
	// duanxiaohui 20111103 ���鳡(��ѳ�) ����ʹ�õ���
	if (m_pGameInfo->dwRoomRule & GRR_EXPERCISE_ROOM)
	{
		AFCMessageBox("�Բ���, ���鳡(��ѳ�)����ʹ����Ϸ����", "��ܰ��ʾ");
		return ;
	}
	// end duanxiaohui


    if(GetMeUserInfo()->bUserState==USER_WATCH_GAME)
    {
        AFCMessageBox("�Բ����Թ�ʱ����ʹ�õ��ߣ�","��ʾ");
        return;
    }

    _TAG_USERPROP * userProp=Glb().userPropLibrary.GetAt(propIndex);
    if(userProp==NULL) return;

    if(userProp->nHoldCount > 0)    // ��������Ϊ0��ʹ�ü����ù���
    {
        TCHAR chInfo[256] = {0};
        wsprintf(chInfo, "[%s]��������Ϊ0������ʹ���̵�ļ����ù���", userProp->szPropName);
        m_MessageHandle.InsertSystemMessage(chInfo);  
        return;
    }

    _TAG_PROP_BUY *pPropBuy = new _TAG_PROP_BUY;
    pPropBuy->dwUserID = GetMeUserInfo()->dwUserID;
    pPropBuy->nPropID = userProp->nPropID;
    pPropBuy->iPropPayMoney = 0;
    pPropBuy->nPropBuyCount = 1;

    this->m_pGameRoom->PostMessage(WM_PROP_BUY_NEW, sizeof(_TAG_PROP_BUY), (LPARAM)pPropBuy);
}


//��Ϣ����������Ҫ���أ�
bool CLoveSendClassInExe::OnSystemMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
	ASSERT(pNetHead->bMainID==MDM_GM_MESSAGE);
	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_SYSTEM_MESSAGE:		//ϵͳ��Ϣ
		{
			//Ч������
			MSG_GA_S_Message * pMessage=(MSG_GA_S_Message *)pNetData;
			AFCPlaySound(AfxGetInstanceHandle(),TEXT("SYSTEM_MSG"));

			//��������
			if ((pMessage->bShowStation&SHS_TALK_MESSAGE)!=0)
			{
				m_MessageHandle.InsertSystemMessage(pMessage->szMessage,pMessage->bFontSize);
			}
			if ((pMessage->bShowStation&SHS_MESSAGE_BOX)!=0)
			{
				m_bSaveView=true;
				SetStationParameter(20);
				AFCMessageBox("�����������",m_pGameInfo->szGameName);//,MB_ICONQUESTION,this);
				//AFCMessageBox(pMessage->szMessage,m_pGameInfo->szGameName);//,MB_ICONQUESTION,this);
				m_bSaveView=false;
				if (pMessage->bCloseFace==TRUE)
					AFCCloseFrame(false);
			}
			::SendMessage(m_TalkMessage.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);

			//OnBnClickedEnd();
			return true;
		}
	}
	return true;
}

//�����Ϣ������������Ҫ���أ�
bool CLoveSendClassInExe::OnFrameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
	ASSERT(pNetHead->bMainID==MDM_GM_GAME_FRAME);
	switch (pNetHead->bAssistantID)
	{
	case ASS_GM_GAME_INFO:		//��Ϸ��Ϣ
		{
			//Ч������
			MSG_GM_S_GameInfo * pGameInfo=(MSG_GM_S_GameInfo *)pNetData;

			//��������
			m_bWatchOther=(pGameInfo->bWatchOther==TRUE);
			m_bWaitTime=pGameInfo->bWaitTime;
			m_bWatchMode=(m_pGameInfo->uisMeUserInfo.GameUserInfo.bUserState==USER_WATCH_GAME);
			SetStationParameter(pGameInfo->bGameStation);
			//��ʾ��Ϣ
			if (uDataSize>(sizeof(MSG_GM_S_GameInfo)-sizeof(pGameInfo->szMessage)))
			{
				m_MessageHandle.InsertSystemMessage(pGameInfo->szMessage);
			}
			::SendMessage(m_TalkMessage.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);

			return true;
		}
	case ASS_GM_GAME_STATION:	//��Ϸ״̬
		{	////	AfxMessageBox("111111111");
			return SetGameStation(pNetData,uDataSize);//����DLL�еĺ��������齫��uDataSizeΪ51
		}
	case ASS_GM_NORMAL_TALK:	//��ͨ����
		{
			//Ч������
			MSG_GR_RS_NormalTalk * pNormalTalk=(MSG_GR_RS_NormalTalk *)pNetData;

			//��������
			UserItemStruct * pSendUserItem=FindOnLineUser(pNormalTalk->dwSendID);
			if (pSendUserItem==NULL) 
				return true;

			//���˲��ܻ�ӭ�û�
			if (pSendUserItem->uConnection==CM_ENEMY) 
				return true;

			//���������û�
			for (INT_PTR i=0;i<m_ShieldArray.GetCount();i++)
			{
				if (pSendUserItem->GameUserInfo.dwUserID==m_ShieldArray.GetAt(i)) 
					return true;
			}
			TCHAR msg[502];//������Ϣ
			lstrcpy(msg,pNormalTalk->szMessage);
			//MessageBox(msg);
			//������Ϣ
			if (pNormalTalk->dwTargetID!=0L)
			{
				UserItemStruct * pTargerUserItem=FindOnLineUser(pNormalTalk->dwTargetID);
				if (pTargerUserItem!=NULL) 
				{
					m_MessageHandle.InsertUserTalk(GetComType()!=TY_MATCH_GAME?pSendUserItem->GameUserInfo.nickName:"���",GetComType()!=TY_MATCH_GAME?pTargerUserItem->GameUserInfo.nickName:"���",
						pNormalTalk->szMessage,pNormalTalk->crColor,false);
					lstrcpy(pNormalTalk->szMessage,msg);
					ShowUserTalk(pNormalTalk);
					return true;
				}
			}
			m_MessageHandle.InsertUserTalk(GetComType()!=TY_MATCH_GAME?pSendUserItem->GameUserInfo.nickName:"���",NULL,pNormalTalk->szMessage,pNormalTalk->crColor,false);
			lstrcpy(pNormalTalk->szMessage,msg);
			if(GetComType()!=TY_MATCH_GAME)
				ShowUserTalk(pNormalTalk);
			if(pNormalTalk->nDefaultIndex>0)
			{
// 				CString soundFile;
// 				soundFile.Format("..\\Music\\Talk\\%d.wav",pNormalTalk->nDefaultIndex);
// 				BZSoundPlay(this,soundFile.GetBuffer(),0,1);

				CBcfFile fMsg(CBcfFile::GetAppPath() + "TalkInGame.bcf");
				CString str = fMsg.GetKeyVal("BZW_TALK","path","..\\Music\\Talk");

				//������������
				TCHAR szSpeakPath[MAX_PATH];
				if (pSendUserItem->GameUserInfo.bBoy)
				{
					_stprintf(szSpeakPath,"%s\\male\\%s.wav",str,pNormalTalk->szMessage);	
				}
				else
				{
					_stprintf(szSpeakPath,"%s\\female\\%s.wav",str,pNormalTalk->szMessage);
				}
				BZSoundPlay(this,szSpeakPath,0);
			}
			return true;
		}
	case ASS_GM_WATCH_SET:		//�Թ�����
		{

			ASSERT(m_bWatchMode==true);
			bool bNotifyClient=(m_bWatchOther!=(pNetHead->bHandleCode==TRUE));
			if (bNotifyClient==true)
			{
				m_bWatchOther=(pNetHead->bHandleCode==TRUE);
				if (m_bWatchOther == true)
				{
					m_MessageHandle.InsertSystemMessage(TEXT("�㱻�����Թ�"));
				}
				else 
				{
					m_MessageHandle.InsertSystemMessage(TEXT("�㱻��ֹ�Թ�"));
				}
				OnWatchSetChange();
			}
			return true;
		}
    case ASS_GM_USE_KICK_PROP:      // PengJiLin, 2010-9-10, ʹ�����˿�
        {
            return OnUseKickPropResult(pNetHead, pNetData, uDataSize, TRUE);
        }
    case ASS_GM_USE_ANTI_KICK_PROP: // PengJiLin, 2010-9-10, ʹ�÷��߿�
        {
            return OnUseKickPropResult(pNetHead, pNetData, uDataSize, FALSE);;
        }
	}
	return false;
}
//������Ϣ 
UINT CLoveSendClassInExe::OnControlMessage(UINT uControlCode, void * pControlData, UINT uDataSize)
{
	//�жϴ����Ƿ���Ч
	if (GetSafeHwnd()==NULL)
	{
		DebugPrintf("���ڻ�û�����ɹ�������");
		return 0;
	}

	//DebugPrintf("uControlCode = [%d]",uControlCode);
	//��������
	switch (uControlCode)
	{
	case IPC_USER_PROP_RESULT:
		{
			NetMessageHead * pNetHead = new NetMessageHead;
			pNetHead->bAssistantID = ASS_PROP_RESULT;
			m_pPropPannel->OnNetMessage(pNetHead,(void *)pControlData,uDataSize);
		}
	case CM_USER_SEND_TIMES_MONEY:
		{
			if (uDataSize!=sizeof(CM_UserState_Send_Times_Money_For_Exe)) return 1;
			CM_UserState_Send_Times_Money_For_Exe *pMoneyChange = (CM_UserState_Send_Times_Money_For_Exe*)pControlData;

			if (pMoneyChange->dwUserID != m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID)
				return 0;

            // PengJiLin, 2011-4-18, ����ʱ�����������ͽ�ҹ���
            m_SendMoneyData.dwUserID = pMoneyChange->dwUserID;
            m_SendMoneyData.dwGetMoney = pMoneyChange->dwGetMoney;
            m_SendMoneyData.dwMoneyOnTimes = pMoneyChange->dwMoneyOnTimes;
            m_SendMoneyData.dwMoneyOnCounts = pMoneyChange->dwMoneyOnCounts;
            m_SendMoneyData.dwTimesNeed = pMoneyChange->dwTimesNeed;
            m_SendMoneyData.dwCountsNeed = pMoneyChange->dwCountsNeed;

            if(pMoneyChange->dwGetMoney > 0)
            {
                CString str;
                TCHAR szNum[128]; 
                GlbGetNumString(szNum, pMoneyChange->dwGetMoney, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);

				CBcfFile fMsg(CBcfFile::GetAppPath()+"..\\ClientMessage.bcf");
				str = fMsg.GetKeyVal("GameFrame","SendMoney_Time","/:54���ϵͳ����%s���/:54");
				str.Format(str,szNum);

                m_MessageHandle.InsertNormalMessage(str.GetBuffer());
            }

			CString str;
			str.Format("zhtlog:pMoneyChange->dwGetMoney=%d pMoneyChange->dwTimesNeed=%d pMoneyChange->dwCountsNeed =%d",pMoneyChange->dwGetMoney,pMoneyChange->dwTimesNeed,pMoneyChange->dwCountsNeed );
			OutputDebugString(str);
           /* if(pMoneyChange->dwGetMoney > 0 || pMoneyChange->dwTimesNeed > 0 || pMoneyChange->dwCountsNeed > 0)
            {
               
            }*/

			return 0;

		}
		break;
	case CM_USER_STATE:		//�û�״̬�ı�
		{

			//Ч������
			if (uDataSize!=sizeof(CM_UserState_Change_ForExe))
			{
				DebugPrintf("���ݴ�С����uDataSize =[%d], Ӧ��Ϊ[%d]",sizeof(CM_UserState_Change_ForExe));
				return 1;
			}
			
			CM_UserState_Change_ForExe* pStateChange=(CM_UserState_Change_ForExe *)pControlData;

			if (pStateChange->bActionCode == ACT_USER_BANKMONEY)
			{
				INT_PTR nUserCountInThisDesk = m_ThisDeskPtrArray.GetCount();
				for (INT_PTR i=0; i<nUserCountInThisDesk; ++i)
				{
					__int64 i64BankMoney = pStateChange->uisUserItem.GameUserInfo.i64Bank;
					UserItemStruct *pFindItem = (UserItemStruct *)m_ThisDeskPtrArray.GetAt(i);
					if (pFindItem->GameUserInfo.dwUserID == pStateChange->uisUserItem.GameUserInfo.dwUserID)
					{
						pFindItem->GameUserInfo.i64Bank = i64BankMoney;
						if (m_pBank2Wnd)
						{
							m_pBank2Wnd->SetWealthInfor(i64BankMoney,GetMeUserInfo()->i64Money);
						}
						break;
					}
				}
				return 1;
			}

			INT_PTR nUserCountInThisDesk = m_ThisDeskPtrArray.GetCount();
			for (INT_PTR i=0; i<nUserCountInThisDesk; ++i)
			{
				UserItemStruct *pFindItem = (UserItemStruct *)m_ThisDeskPtrArray.GetAt(i);
				if (pFindItem->GameUserInfo.dwUserID == pStateChange->uisUserItem.GameUserInfo.dwUserID)
				{
					memcpy(pFindItem, &pStateChange->uisUserItem, sizeof(UserItemStruct));
				}
			}

			//��������CS
			switch (pStateChange->bActionCode)
			{
			case ACT_USER_UP:		//�û�����
			case ACT_WATCH_UP:		//�Թ�����
				{
					//��������
					if (pStateChange->uisUserItem.GameUserInfo.dwUserID
						!= m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID)//ͬ�����������뿪
					{
						GameUserLeft(pStateChange->bDeskStation, &pStateChange->uisUserItem, pStateChange->bActionCode==ACT_WATCH_UP);

						if ((GetComType()!=TY_MATCH_GAME)
							            &&m_pGameInfo->bShowUserInfo==true
							            &&0 == pStateChange->uisUserItem.GameUserInfo.isVirtual)
						{	
							m_MessageHandle.InsertUserName(pStateChange->uisUserItem.GameUserInfo.nickName);

							char szTalk[255];
							sprintf(szTalk, "%s �뿪��", pStateChange->uisUserItem.GameUserInfo.nickName);
							m_MessageHandle.InsertNormalMessage(szTalk);

							if (pStateChange->bActionCode == ACT_WATCH_UP)
							{
								;
							}
							else 
							{
								//��ʾ�����ڣ���ʾ��ʾ���ڣ�ʲô��������
								if(!m_DlgShowLeft.Showing && GetSafeHwnd()!=NULL
									&&(m_pGameInfo->dwRoomRule & GRR_ALL_ON_DESK)== 0L)//��Ҫȫ�����뿪
								{
									char GameCode[20];
									::memset(GameCode,0,sizeof(GameCode));
									GetGameCode(GameCode);
									//CString str = GameCode;
									//if (str != "10301800" && 
									//	str != "30501800" &&
									//	str != "10901800" &&
									//	str != "11901800" )
									TCHAR szKey[10];
									CString sPath=CBcfFile::GetAppPath();
									CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
									wsprintf(szKey, "%d", m_pGameInfo->uNameID);
									int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

									if (iResult==0)

									{
										m_DlgShowLeft.GetNickName(pStateChange->uisUserItem.GameUserInfo.nickName);
										AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
										m_DlgShowLeft.Create(IDD_SHOW_LEFT,this);
										AfxSetResourceHandle(GetModuleHandle(NULL));
										RECT rc;
										GetClientRect(&rc);
										ScreenToClient(&rc);
										m_DlgShowLeft.ShowWindow(1);
										m_DlgShowLeft.Showing=1;
										m_DlgShowLeft.BeginTimer();
										EnableWindow(FALSE);
									}
								}
							}
						}
					}
					else//�Լ��뿪
					{
						//if (m_uTimeID!=0L) 
						//	KillGameTimer(m_uTimeID);
						//if (m_bSaveView==false)
						//{
						//	this->ResetGameStation();
						//	m_pGameView->ResetGameView();
						//	ResetGameFrame();
						//}
						////�����뿪
						//if(!m_DlgShowLeft.Showing)
						//	AFCCloseFrame();
						DebugPrintf("�Լ��뿪��");
					}

					::SendMessage(m_TalkMessage.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);

					return 0;
				}
			case ACT_USER_SIT:		//�û�����
			case ACT_WATCH_SIT:		//�Թ�����
				{
					//��������
					// ���Ǳ������£���ֱ��֪ͨ
					if (pStateChange->uisUserItem.GameUserInfo.dwUserID
						!= m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID)//ͬ����������
					{
						OnUserListAtDesk(&pStateChange->uisUserItem);
						if (m_pGameInfo->bShowUserInfo==true&&(GetComType()!=TY_MATCH_GAME))
						{
							char szTalk[255];
							sprintf(szTalk, "%s ������", pStateChange->uisUserItem.GameUserInfo.nickName);
							m_MessageHandle.InsertNormalMessage(szTalk);
						}
					}
					else	// �����Լ�����ĳ�����ӣ�����Ҫ�ҵ�ͬ��������ҵ���Ϣ����������Ϣ�����ڻ���
					{
						//OnBnClickedMax();
						//Ч��״̬
						//m_bWatchMode = pStateChange->uisUserItem.GameUserInfo.bDeskStation == USER_WATCH_GAME;
						BYTE bMeDeskStation=m_pGameInfo->uisMeUserInfo.GameUserInfo.bDeskNO;
						//DebugPrintf("�Լ�������[%d]����Ϣ������[%d]��",
						//	m_pGameInfo->uisMeUserInfo.GameUserInfo.bDeskNO,
						//	pStateChange->uisUserItem.GameUserInfo.bDeskNO
						//	);

						//���ͻ�ȡ��Ϣ
						MSG_GM_S_ClientInfo ClientInfo;
						ClientInfo.bEnableWatch=m_pGameInfo->bEnableWatch;
						SendGameData(&ClientInfo,sizeof(ClientInfo),MDM_GM_GAME_FRAME,ASS_GM_GAME_INFO,0);
						UpdateGameTitle();
					}

					::SendMessage(m_TalkMessage.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);

					return 0;
				}
			case ACT_USER_CUT:		//��Ҷ���orǿ����
				{

					//��������
					m_UserListDlg.UpdateGameUser(&pStateChange->uisUserItem);
					m_pGameView->UpdateUserInfo(pStateChange->bDeskStation);
					//�����˶��߲�����ʾ��Ϣ201103-16
					if(0 == pStateChange->uisUserItem.GameUserInfo.isVirtual)
					{
						//��ʾ��Ϣ
						char szTalk[255];
						if(GetComType() != TY_MATCH_GAME)
						{
							sprintf(szTalk, "%s ������", pStateChange->uisUserItem.GameUserInfo.nickName);
						}
						else
						{
							sprintf(szTalk, "������", pStateChange->uisUserItem.GameUserInfo.nickName);
						}
						
						m_MessageHandle.InsertNormalMessage(szTalk);

					}
					//m_MessageHandle.InsertNormalMessage(TEXT("�й���"));
					//OutputDebugString("[aa]SDK ǿ����");

					::SendMessage(m_TalkMessage.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);

					return 0;
				}
			case ACT_USER_AGREE:	//�û�ͬ��
				{
					UserAgreeGame(pStateChange->bDeskStation);
					return 0;
				}
			case ACT_GAME_BEGIN:	//��Ϸ��ʼ
			case ACT_GAME_END:		//��Ϸ����
				{
					//m_bWatchMode = pStateChange->uisUserItem.GameUserInfo.bDeskStation == USER_WATCH_GAME;
					for (BYTE i=0;i<m_pGameInfo->uDeskPeople;i++)
					{
						
						if (m_pUserInfo[i]!=NULL) 
						{
							//DebugPrintf("m_pUserInfo[%d].GameUserInfo.nickName=%s",i,m_pUserInfo[i]->GameUserInfo.nickName);
							m_pUserInfo[i]->GameUserInfo.bUserState = USER_PLAY_GAME;
							m_UserListDlg.UpdateGameUser(m_pUserInfo[i]);
							//DebugPrintf("��Ϸ��ʼʱ��m_pUserInfo[%d].state = %d����%s", i,
							//	m_pUserInfo[i]->GameUserInfo.bUserState,
							//	m_pGameInfo->uisMeUserInfo.GameUserInfo.nickName);
						}
					}
					m_pGameView->UpdateUserInfo(ViewStation(m_pGameInfo->uDeskPeople));
					if (m_pGameView->m_bShowRight)	///< ��ֹ�Ŷӻ��ͷ����׷�����˸
					{
						::SendMessage(m_TalkMessage.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);

					}
					return 0;
				}
			case ACT_USER_POINT:	//�û�����ֵ
				{
					UserItemStruct * pUserItem=FindOnLineUser(pStateChange->uisUserItem.GameUserInfo.dwUserID);
					if (pUserItem == NULL)
					{
						return 0;
					}
					memcpy( pUserItem, &pStateChange->uisUserItem, sizeof(UserItemStruct) );
					m_UserListDlg.UpdateGameUser(pUserItem);
					if (GetMeUserInfo()->dwUserID == pStateChange->uisUserItem.GameUserInfo.dwUserID)// add by ramon 09.05.31 �޸�������Ϣ����ˢ��
					{
						memcpy(&m_pGameInfo->uisMeUserInfo, &pStateChange->uisUserItem, sizeof(UserItemStruct));
					}
					if(pUserItem->GameUserInfo.dwUserID==m_CurpUserInfoClicked->dwUserID)
					{
						//wushuqun 2009.5.20
						//�޸Ļ��ֲ�ˢ��BUG
						m_CurpUserInfoClicked = &pUserItem->GameUserInfo;
						NavigateVirFace();
						m_pRightFrame->ShowUserInfo();
					}

					CString str;
					//str.Format("��� %s,���� %d,��� %d",pUserItem->GameUserInfo.nickName,pUserItem->GameUserInfo.dwPoint,
					//	pUserItem->GameUserInfo.dwMoney);
					//OutputDebugString(str);

					if (pUserItem->GameUserInfo.bUserState!=USER_WATCH_GAME)
					{
						m_pGameView->UpdateUserInfo(pStateChange->bDeskStation);
					}

					//����Ѵ���Ϸ���У��������е���ֵ
					//Add by JianGuankun 2012.1.2
					if (GetMeUserInfo()->dwUserID == pStateChange->uisUserItem.GameUserInfo.dwUserID) //�Լ���������
					{
						if (m_pBank2Wnd)
						{
							m_pBank2Wnd->SetWealthInfor(GetMeUserInfo()->i64Bank,GetMeUserInfo()->i64Money);
						}
					}
					//End Add

					//���˻��ָ����ж��Ƿ���Ҫ�뿪
					if(GetMeUserInfo()->dwUserID == pUserItem->GameUserInfo.dwUserID)//�Ƿ���Ҫ�뿪��Ϸ��
					{
						if(GetMeUserInfo()->bUserState!= USER_PLAY_GAME && GetMeUserInfo()->i64Money < m_pGameInfo->uLessPoint)
						{
							//OnBnClickedQuitGame();    // PengJiLin, 2011-4-18
                            OnBnClickedExit();          // PengJiLin, 2011-4-18
						}	
					}
					if (m_pGameView->m_bShowRight)
					{
#if 0	//2010-7-16 yangyan change
						InvalidateRect(m_UserInfoRect,false);
#else
						InvalidateRect(&m_UserInfoRect);
#endif
					}
					::SendMessage(m_TalkMessage.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);

					return 0;
				}
			case ACT_USER_CONTESTPOINT:
				{
					UserItemStruct * pUserItem=FindOnLineUser(pStateChange->uisUserItem.GameUserInfo.dwUserID);
					if (pUserItem == NULL)
					{
						return 0;
					}
					memcpy( pUserItem, &pStateChange->uisUserItem, sizeof(UserItemStruct) );
					m_UserListDlg.UpdateGameUser(pUserItem);
					if (GetMeUserInfo()->dwUserID == pStateChange->uisUserItem.GameUserInfo.dwUserID)// add by ramon 09.05.31 �޸�������Ϣ����ˢ��
					{
						memcpy(&m_pGameInfo->uisMeUserInfo, &pStateChange->uisUserItem, sizeof(UserItemStruct));
					}
					if(pUserItem->GameUserInfo.dwUserID==m_CurpUserInfoClicked->dwUserID)
					{
						//wushuqun 2009.5.20
						//�޸Ļ��ֲ�ˢ��BUG
						m_CurpUserInfoClicked = &pUserItem->GameUserInfo;
						NavigateVirFace();
					}

					CString str;
					//str.Format("��� %s,���� %d,��� %d",pUserItem->GameUserInfo.nickName,pUserItem->GameUserInfo.dwPoint,
					//	pUserItem->GameUserInfo.dwMoney);
					//OutputDebugString(str);

					if (pUserItem->GameUserInfo.bUserState!=USER_WATCH_GAME)
					{
						m_pGameView->UpdateUserInfo(pStateChange->bDeskStation);
					}
					//���˻��ָ����ж��Ƿ���Ҫ�뿪
					if(GetMeUserInfo()->dwUserID == pUserItem->GameUserInfo.dwUserID)//�Ƿ���Ҫ�뿪��Ϸ��
					{
						if(GetMeUserInfo()->bUserState!= USER_PLAY_GAME && GetMeUserInfo()->i64ContestScore < m_pGameInfo->i64LowChip)
						{
							//OnBnClickedQuitGame();    // PengJiLin, 2011-4-18
							OnBnClickedExit();          // PengJiLin, 2011-4-18
						}	
					}
					if (m_pGameView->m_bShowRight)
					{
#if 0	//2010-7-16 yangyan change
						InvalidateRect(m_UserInfoRect,false);
#else
						InvalidateRect(&m_UserInfoRect);
#endif
					}
					::SendMessage(m_TalkMessage.m_hWnd,WM_VSCROLL,SB_BOTTOM,0);
					return 0;
				}


			case ACT_USER_QUEUE_DISMISS:
				{
					CString debug;
					debug.Format("xxxxxx �û�״̬�ı�ΪACT_USER_QUEUE_DISMISSACT_USER_QUEUE_DISMISSACT_USER_QUEUE_DISMISS");
					OutputDebugString(debug);
					m_bIsQueueDisMiss = true;
					return 0;
				}

			case ACT_USER_CHARM://��������ֵ
				{
					UserItemStruct * pUserItem=FindOnLineUser(pStateChange->uisUserItem.GameUserInfo.dwUserID);
					if (pUserItem == NULL)
					{
						return 0;
					}
					memcpy( pUserItem, &pStateChange->uisUserItem, sizeof(UserItemStruct) );
					m_UserListDlg.UpdateGameUser(pUserItem);
					if (GetMeUserInfo()->dwUserID == pStateChange->uisUserItem.GameUserInfo.dwUserID)// add by ramon 09.05.31 �޸�������Ϣ����ˢ��
					{
						memcpy(&m_pGameInfo->uisMeUserInfo, &pStateChange->uisUserItem, sizeof(UserItemStruct));
					}
					if(pUserItem->GameUserInfo.dwUserID==m_CurpUserInfoClicked->dwUserID)
					{
						NavigateVirFace();
					}
					if (pUserItem->GameUserInfo.bUserState!=USER_WATCH_GAME)
					{
						m_pGameView->UpdateUserInfo(pStateChange->bDeskStation);
					}
					return 0;
				}

			case ACT_USER_AVATAR_CHANGE: //����������
				{
					UserItemStruct * pUserItem = FindOnLineUser(pStateChange->uisUserItem.GameUserInfo.dwUserID);
					if (pUserItem == NULL)
					{
						return 0;
					}

					//������Ϸ�ұ߿��ͷ��
					if (m_pRightFrame)
					{
						m_pRightFrame->ShowUserInfo();
					}

					m_pGameView->UpdateViewFace(NULL);

					return 0;
				}
			}
		}
		break;
	case IPC_TROTTING:
		{
			TMailItem* pMailMsg = (TMailItem*)pControlData;
			m_pPmdWnd->AdjustPos(this->GetSafeHwnd());
			m_pPmdWnd->StartPmdLabel(120000,pMailMsg->szMsg,20,true);
			ShowWindow(SW_SHOWNORMAL);
			m_pPmdWnd->ShowCtrl();
			return 0;
		}
		break;

	case IPC_EX_SKIN:
		{
			//����֪ͨ
			CString strUserName = (static_cast<MSG_GP_R_LogonResult*>(pControlData))->szName;
			m_skinmgr.SetLogOnUserName(strUserName);
			m_skinmgr.Notify();
			
			LoadSkin();
			Invalidate();

			if (m_TalkMessage.IsWindowVisible())
			{
				m_TalkMessage.LoadScrollSkin();
				m_TalkMessage.Invalidate();
			}

			if (m_TalkMessage.m_DiyChat.IsWindowVisible())
			{
				m_TalkMessage.m_DiyChat.LoadSkin();
				m_TalkMessage.m_DiyChat.Invalidate();
			}

			if (m_DlgShowLeft.IsWindowVisible())
			{
				m_DlgShowLeft.LoadSkin();
				m_DlgShowLeft.Invalidate();
			}

			if (m_pUserDetailsDlg != NULL)
			{
				m_pUserDetailsDlg->LoadSkin();
				m_pUserDetailsDlg->Invalidate();
			}

			if (m_UserListDlg.m_UserList.IsWindowVisible())
			{
				m_UserListDlg.m_UserList.InitIniFile();
				m_UserListDlg.m_UserList.LoadSkin();
				m_UserListDlg.m_UserList.Invalidate();
			}

			if (m_dlgBroadcast != NULL)
			{
				m_dlgBroadcast->LoadSkin();
				m_dlgBroadcast->Invalidate();
			}

			if (m_dlgKickProp != NULL)
			{
				m_dlgKickProp->LoadSkin();
				m_dlgKickProp->Invalidate();
			}

			if (m_pPropPannel != NULL)
			{
				m_pPropPannel->LoadSkin();
				m_pPropPannel->Invalidate();
				if (m_pPropPannel->m_pLogic != NULL)
				{
					if (m_pPropPannel->m_pLogic->m_dlgBroadcast != NULL)
					{
						m_pPropPannel->m_pLogic->m_dlgBroadcast->LoadSkin();
						m_pPropPannel->m_pLogic->m_dlgBroadcast->Invalidate();
					}
					if (m_pPropPannel->m_pLogic->m_pPropSelUser != NULL)
					{
						m_pPropPannel->m_pLogic->m_pPropSelUser->LoadSkin();
						m_pPropPannel->m_pLogic->m_pPropSelUser->Invalidate();
					}
					if (m_pPropPannel->m_pLogic->m_pPersonBank != NULL)
					{
						m_pPropPannel->m_pLogic->m_pPersonBank->LoadSkin();
						m_pPropPannel->m_pLogic->m_pPersonBank->Invalidate();
					}
				}
			}
		}

		break;
	}

	return 0;
}
//��Ϸ��Ϣ������
bool CLoveSendClassInExe::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
	//OutputDebugString(_T("SDK�� HandleGameMessage ����"));
	//CString cStr1;
	//cStr1.Format(_T("MDM_GM_GAME_NOTIFY_bMainID=%d,bAssistantID=%d"),pNetHead->bMainID,pNetHead->bAssistantID);
	//OutputDebugString(cStr1.GetBuffer());

	if (pNetHead->bMainID == MDM_GM_GAME_NOTIFY)
	{
		switch (pNetHead->bAssistantID)
		{	
		case ASS_PROP:				//����
			{
				if (uDataSize!=sizeof(_TAG_PROP_MOIVE_USE)) {
					//WriteInfo(_T("CLoveSendClassInExe::HandleGameMessage 00"));
					return FALSE;
				}
				_TAG_PROP_MOIVE_USE *propMoive=(_TAG_PROP_MOIVE_USE*)pNetData;

				UserProp(propMoive);// pstProp->sendDeskStation,pstProp->recvDeskStation,pstProp->iValue);
				return true;
			}
		}
	}
	CString cStr;
	cStr.Format(_T("CLoveSendClassInExe::HandleGameMessage 11 Main_id=%d,AssID=%d"),pNetHead->bMainID,pNetHead->bAssistantID);
	//WriteInfo(cStr.GetBuffer());
	return false;
}

//�������Ϣ������
bool CLoveSendClassInExe::HandlePointMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
	return false;
}

bool CLoveSendClassInExe::UserProp(_TAG_PROP_MOIVE_USE *propMoive)
{
	int attribAction=propMoive->nPropActionAttrib;
	int attribValue=propMoive->nPropValueAttrib;


	if(attribAction & 0x40)
	{
		int iValue=(attribValue & 0x7F000000)>>24;
		m_stProp.iValue=iValue;
		m_stProp.sendDeskStation=propMoive->nUserStation;
		m_stProp.recvDeskStation=propMoive->nTargetUserStation;
		m_pGameView->SetGif(ViewStation(propMoive->nUserStation),iValue,2000);
		SetTimer(ID_GIF_TIMER,150,NULL);	
		//{��Ӷ����������� zxj 2009-9-17
		//CString soundFile;
		//soundFile.Format("..\\image\\prop\\PropGif%d%s.mp3",m_stProp.iValue%1000, m_stProp.iValue<1000?"A":"B");
		//BZSoundPlay(this,soundFile.GetBuffer(),0,1);
		//��ӽ���}
	}
	return true;
}
//���Ż��㵽��ͼλ��
BYTE CLoveSendClassInExe::ViewStation(BYTE bDeskStation)
{
	BYTE bViewStation=(bDeskStation+m_pGameInfo->uDeskPeople-m_pGameInfo->uisMeUserInfo.GameUserInfo.bDeskStation);
	switch (m_pGameInfo->uDeskPeople)
	{
	case 1: { bViewStation+=1; break; }
	case 2: { bViewStation+=1; break; }
	case 3: { bViewStation+=1; break; }
	case 4: { bViewStation+=2; break; }
	case 5: { bViewStation+=2; break; }
	case 6: { bViewStation+=3; break; }
	case 7: { bViewStation+=3; break; }
	case 8: { bViewStation+=4; break; }
	}
	return bViewStation%m_pGameInfo->uDeskPeople;
}
//���ö�ʱ��
void CLoveSendClassInExe::SetGameTimer(BYTE bDeskStation, UINT uMaxTime, UINT uTimeID)
{
	if (m_uTimeID!=0L) KillGameTimer(m_uTimeID);
	if ((bDeskStation<m_pGameInfo->uDeskPeople)&&(uMaxTime>0L))
	{
		m_uTimeID=uTimeID;
		m_uTimeCount=uMaxTime;
		m_bTimeStation=bDeskStation;
		SetTimer(uTimeID,1000,NULL);
		m_pGameView->SetUserTimer(ViewStation(bDeskStation),m_uTimeCount);
	}
	return;
}
//ɾ����ʱ��
void CLoveSendClassInExe::KillGameTimer(UINT uTimeID)
{
	//������ʱ��
	if (m_uTimeID==0L) return;
	if (uTimeID==0L) uTimeID=m_uTimeID;
	if (m_uTimeID==uTimeID)
	{
		//�������
		KillTimer(m_uTimeID);
		if (m_bTimeStation!=255)
			m_pGameView->SetUserTimer(ViewStation(m_bTimeStation),0);

		//��������
		m_uTimeID=0L;
		m_uTimeCount=0L;
		m_bTimeStation=255;
	}
	return;
}
//��ʱ����Ϣ
void CLoveSendClassInExe::OnTimer(UINT nIDEvent)
{
	//if(nIDEvent==1)
	//{
	//	KillTimer(1);
	//	Invalidate(FALSE);
	//}
	switch(nIDEvent)//����
	{
	case 1:
		{
			KillTimer(1);
			int nDefaultIndex=m_InputMessage.GetCurSel();
			int nTalkCount=m_InputMessage.GetCount();
			if(nTalkCount>m_nTalkDefaultCount)
				nDefaultIndex-=nTalkCount-m_nTalkDefaultCount;
			if(nDefaultIndex>=0)
				SendTalkMessage(nDefaultIndex+1);
		}
		break;
	case ID_GIF_TIMER:
		if(m_pGameView->m_pGif == NULL)
		{
			KillTimer(ID_GIF_TIMER);
			m_pGameView->UpdateViewFace(NULL);
			PlayNextGif();
		}
		else
		{
			m_pGameView->UpdateViewFace(NULL);
			m_pGameView->CountFrame();
		}
		break;
	case ID_GIF_TIMER2:
		if(m_pGameView->m_pGif == NULL)
		{
			KillTimer(ID_GIF_TIMER2);
			m_pGameView->UpdateViewFace(NULL);
		}
		else
		{
			m_pGameView->UpdateViewFace(NULL);
			m_pGameView->CountFrame();
		}
		break;
	}

	//ɾ����ʱ��
	if (nIDEvent==m_uTimeID)
	{
		if ((m_uTimeCount>0L)&&(m_bTimeStation!=255))
		{
			m_uTimeCount--;
			bool bHandle=OnGameTimer(m_bTimeStation,m_uTimeID,m_uTimeCount);
			if ((bHandle==false)||(m_uTimeCount==0))
				KillGameTimer(m_uTimeID);
			else m_pGameView->SetUserTimer(ViewStation(m_bTimeStation),m_uTimeCount);
		}
		else KillGameTimer(m_uTimeID);
	}
	__super::OnTimer(nIDEvent);
	return;
}
//������һ��gif����
bool CLoveSendClassInExe::PlayNextGif()
{
	m_pGameView->SetGif(ViewStation(m_stProp.recvDeskStation),m_stProp.iValue + 1000,2000);
	SetTimer(ID_GIF_TIMER2,150,NULL);
	return true;
}
// ����ƽ̨����ͬ����ҵ�����
void CLoveSendClassInExe::GetOnLineUser( BYTE bMeDeskStation )
{
	CWnd * winApp=AfxGetApp()->m_pMainWnd;
	if(winApp && winApp->m_hWnd)
		winApp->PostMessage(WM_REQUEST_PLAYERLIST, (WPARAM)bMeDeskStation, 0);
}
// �����û�ID�ұ���������Ƿ���
UserItemStruct *CLoveSendClassInExe::FindOnLineUser(long int dwUserID)
{
	INT_PTR uActiveCount=m_ThisDeskPtrArray.GetCount();
	for (int i=0;i<uActiveCount;i++)
	{
		UserItemStruct * pFindUserItem=(UserItemStruct *)m_ThisDeskPtrArray.GetAt(i);
		if ((pFindUserItem!=NULL)&&(pFindUserItem->GameUserInfo.dwUserID==dwUserID)) 
			return pFindUserItem;
	}
	return NULL;
}
// ����һ����ҵ�������
void CLoveSendClassInExe::OnUserListAtDesk(void *pUserItemVoid)
{
	UserItemStruct *pUserItemToAdd = (UserItemStruct *)pUserItemVoid;
	//���������û�
	bool bNewItem=false;
	if (pUserItemToAdd->GameUserInfo.dwUserID == m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID)
	{
		memcpy(&m_pGameInfo->uisMeUserInfo, pUserItemToAdd, sizeof(UserItemStruct));
	}
	UserItemStruct * pUserItem=FindOnLineUser(pUserItemToAdd->GameUserInfo.dwUserID);
	if (pUserItem==NULL)
	{
		//�����ڴ�
		bNewItem = true;
		pUserItem =new UserItemStruct;
		ASSERT(pUserItem!=NULL);
	}
	// ����Ѿ����˵ģ�ҲҪ�������ݣ���Ϊ�п��������ͬһ���ڻ�����ͬλ��
	memcpy( pUserItem, pUserItemToAdd, sizeof(UserItemStruct) );
	//�������
	if (bNewItem==true) m_ThisDeskPtrArray.Add(pUserItem);

	GameUserCome(pUserItem->GameUserInfo.bDeskStation,
		pUserItem,
		pUserItem->GameUserInfo.bUserState==USER_WATCH_GAME);
}

//�����ȡ��Ϣ
bool CLoveSendClassInExe::OnSocketReadEvent(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
	//���˴���
	if (m_pGameInfo->uisMeUserInfo.GameUserInfo.bDeskNO==255) 
		return true;
	//������Ϣ
	switch (pNetHead->bMainID)
	{
	case MDM_GR_NETSIGNAL:
		{
			//wushuqun 2009.6.18
			//�˴���MDM_GR_NETSIGNAL��ϢΪIPCģ����Ϣ
			//Ϊ����������л�������Թ�ģʽBUG
			if (pNetHead->bAssistantID == MDM_GR_NETSIGNAL && pNetHead->bHandleCode == MDM_GR_NETSIGNAL)
			{
				m_bWatchMode = false;				
				m_bWatchOther = true;
				return true;
			}
			//////////////////////////////////////
			getNetSignal(pNetData);
			return true;
		}
	case MDM_GM_GAME_FRAME:		//�����Ϣ
		{
			if (GetSafeHwnd()==NULL)
				return true;
			return OnFrameMessage(pNetHead,pNetData,uDataSize);
		}
	case MDM_GM_MESSAGE:		//��Ϣ��Ϣ
		{
			if (GetSafeHwnd()==NULL) 
				return true;
			return OnSystemMessage(pNetHead,pNetData,uDataSize);
		}
	case MDM_GM_GAME_NOTIFY:	//��Ϸ��Ϣ
		{
			if (this == NULL)
			{
				AFCMessageBox("o");
				return true;
			}
			if (!this->m_hWnd) 
				return true;
			//wushuqun 2009.6.16
			//OutputDebugString("SDK ����HandleGameMessage֮ǰ");
			bool bRet = HandleGameMessage(pNetHead,pNetData,uDataSize);
			if (m_pGameInfo->bIsInRecord)
			{
				SetRecordView();
			}

			return bRet;
		}
	case MDM_GR_POINT:		//�����
		{
			if(NULL == GetSafeHwnd())
				return true;
			return HandlePointMessage(pNetHead,pNetData,uDataSize);
		}
	case MDM_GR_PROP:		//ʹ�õ���
		{
			if(!GetSafeHwnd())
				return true;
			return OnPropMessage(pNetHead,pNetData,uDataSize);
		}
	case MDM_GR_MONEY:		// �������е���Ϣ
		{
			if(NULL == GetSafeHwnd())
				return true;
			return OnAboutMoney(pNetHead,pNetData,uDataSize);
		}
	case MDM_BANK:
		{
			if (m_pBank2Wnd)
			{
				m_pBank2Wnd->OnNetMessage(pNetHead,pNetData,uDataSize);
			}
		}
		break;
    case MDM_GR_ROOM2GAME:  // PengJiLin, 2010-10-14, �ɷ��䷢��������Ϣ
        {
            switch (pNetHead->bAssistantID)
            {
            case ASS_GR_R2G_USEPROP:    // PengJiLin, 2010-10-14, ��Ϸ�˵��ߵļ����ù���
                {
                    if(uDataSize != sizeof(_TAG_USERPROP)) return false;

                    _TAG_USERPROP* pSendProp = (_TAG_USERPROP*)pNetData;

                    int propCount=Glb().userPropLibrary.GetCount();
                    _TAG_USERPROP *userProp=NULL;
                    bool bPropExist=false;
                    int iPropIndex = 0;
                    for(int i=0;i<propCount;i++)
                    {
                        userProp=Glb().userPropLibrary.GetAt(i);
                        if(userProp->nPropID==pSendProp->nPropID)
                        {
                            bPropExist=true;
                            iPropIndex = i;
                            break;
                        }
                    }

                    // �ҵ�
                    if(true == bPropExist)
                    {
						if (pNetHead->bHandleCode == DTK_GP_PROP_BUYANDUSE_SUCCEED)
						{
							if (m_pRightFrame && m_pRightFrame->m_pBoardCastWnd)
								m_pRightFrame->m_pBoardCastWnd->UseProp_Buy(iPropIndex);
						}
                        else
							UsePropItem(iPropIndex, FALSE);
                    }

                    return true;
                }
                break;
			case ASS_GR_GET_NICKNAME_ONID_INGAME:      // lxl, 2010-11-17, �����û�ID��ȡ�ǳ�
			{
				OutputDebugString("��Ϸ�гɹ��յ��������������ǳ�");
				if(ASS_GR_GETNICKNAME_SUCCESS == pNetHead->bHandleCode) // �����ɹ�
				{
                // Ч������
					if (uDataSize != sizeof(MSG_GR_S_GetNickNameOnID_t)) return false;
					MSG_GR_S_GetNickNameOnID_t* pGetNickNameOnID = (MSG_GR_S_GetNickNameOnID_t *)pNetData;

					m_pPersonBank->SetNickName(pGetNickNameOnID->szNickName); 
					return true;
				}
				return true;
			}
			break;
            default:
                break;
            }
        }
	}
	return false;
}
/// �������е���Ϣ
/// ��ͻ��˵�GameRoomEx�д�ͬС�죬ֻ�������ؽ����������������ݵı仯
/// @param[in] pNetHead Э��ͷ
/// @param[in] pNetData ���ݰ�
/// @param[in] uDataSize ���ݰ���С
/// @return ����ɹ������򷵻�true������false��ʾ���������Ϣʧ�ܣ�����һ�ֿ�������ΪЭ�鲻ƥ�䣬�����޸���ͷ�ļ�ȴû�ж����±���
/// 
bool CLoveSendClassInExe::OnAboutMoney(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
	CBcfFile fMsg(CBcfFile::GetAppPath()+"..\\ClientMessage.bcf");

	/// ������Э��ID�Ĳ�ͬ�����ݳ���Ҳ������ͬ�������ÿ��case�������ж�
	/// ��Ҫ�������Ϣ�У������ڷ����������������к�Ǯ�����仯����Ϣ
	switch (pNetHead->bAssistantID)
	{
	case ASS_GR_TRANSFER_RECORD_INGAME:	///< ת�ʼ�¼
		{
			//Ч������
			if(uDataSize!=sizeof(MSG_GR_R_TransferRecord_t))
			{
				return false;
			}
			MSG_GR_R_TransferRecord_t *pRecord = (MSG_GR_R_TransferRecord_t *)pNetData;
			m_pPersonBank->OnReceivedTransferRecord( pRecord );
			break;
		}
	case ASS_GR_CHANGE_PASSWD_INGAME:	///< �޸�����
		{
			/// ֻ��ҪbHandelCode�Ϳ�����
			CString strBuffer;
			CString str;
			if (pNetHead->bHandleCode == ASS_GR_OPEN_WALLET_SUC)
			{
				str = fMsg.GetKeyVal("GameRoom","ChangePasswdOK","�޸���������ɹ��������Ʊ��������롣");
			}
			else
			{
				str = fMsg.GetKeyVal("GameRoom","ChangePasswdFailed","�޸���������ʧ�ܣ�");
			}
			strBuffer.Format(str);
			AFCMessageBox(strBuffer,m_pGameInfo->szGameName);//,MB_OK,this);
			break;
		}
	case ASS_GR_PAY_MONEY:
		{
			switch(pNetHead->bHandleCode)
			{
			case 1://�ɹ�
				{
					break; // �Ѿ��ɴ����������������һ��Э�鴫���˸ı����ݣ�������ﲻ���ظ�����
				}
			case 0: //���ɹ�
				{
					CString strBuffer;
					strBuffer.Format(TEXT("���Ľ�Ҷ���˴�֧��!"));
					AFCMessageBox(strBuffer,m_pGameInfo->szGameName);//,MB_OK,this);
					break;
				}
			}
			break;
		}
	case ASS_GR_OPEN_WALLET_INGAME:
	case ASS_GR_OPEN_WALLET:
		{
			switch (pNetHead->bHandleCode)
			{
			case ASS_GR_OPEN_WALLET_ERROR:
				{
					CString strBuffer;
					strBuffer.Format(TEXT("��Ǯ��ʧ��,���Ժ�����!"));
					AFCMessageBox(strBuffer,m_pGameInfo->szGameName);//,MB_OK,this);
					break;
				}
			case ASS_GR_OPEN_WALLET_ERROR_PSW:
				{
					CString strBuffer;
					strBuffer.Format(TEXT("�����������,������!"));
					AFCMessageBox(strBuffer,m_pGameInfo->szGameName);//,MB_OK,this);
					break;
				}
			case ASS_GR_OPEN_WALLET_SUC:
				{
					DebugPrintf("������");
					//Ч������
					MSG_GR_UserBank * pBank=(MSG_GR_UserBank *)pNetData;
					CString strBuffer;
					if(uDataSize!=sizeof(MSG_GR_UserBank)) 
					{
						CString str;
						str = fMsg.GetKeyVal("GameRoom","ErrorInGettingMoney","��Ǯ��ʧ��,���Ժ�����!");
						strBuffer.Format(str);
						AFCMessageBox(strBuffer,m_pGameInfo->szGameName);//,MB_OK,this);
					}

					/*if (pBank->nVer == 1) //�ϰ�����
					{
						AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
						//��������
						if (m_pPersonBank == NULL) 
						{
							try
							{
								m_pPersonBank=new CPBankDlg(this);
								m_pPersonBank->Create(IDD_BANK,this);
								if (m_pPersonBank==NULL)
									return false;
							}
							catch (...) { return false; }
						}
						if (m_pPersonBank!=NULL)
						{
							DebugPrintf("��Ϸ�и����г�Ա��ֵdwBank=[%d]dwMoney=[%d]---%d��", pBank->i64Bank, m_pGameInfo->uisMeUserInfo.GameUserInfo.i64Money
								, __LINE__);
							/// �ȸ�ֵ
							m_pPersonBank->SetWealthInfor(pBank->i64Bank, m_pGameInfo->uisMeUserInfo.GameUserInfo.i64Money);
							if (m_pPersonBank->GetSafeHwnd()!=NULL)
							{
								AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
								/// �����еĳ�Ա��ֵ
								DebugPrintf("��Ϸ��׼�������н���[%d]---%d��", m_pPersonBank->GetSafeHwnd()
									, __LINE__);
								m_pPersonBank->CenterWindow();
								m_pPersonBank->ShowWindow(SW_SHOW);
								DebugPrintf("��Ϸ�����н����˳�[%d]---%d��", m_pPersonBank->GetSafeHwnd()
									, __LINE__);
							}
						}
						AfxSetResourceHandle(GetModuleHandle(NULL));
					}
					else if (pBank->nVer == 2) //�°�����*/
					{	
						if (!m_pBank2Wnd)
						{
							//�ǰټ�����Ϸ���ô���Ϸ����
// 							TCHAR szKey[10];
// 							CString sPath=CBcfFile::GetAppPath();
// 							CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
// 							wsprintf(szKey, "%d", m_pGameInfo->uNameID);
// 							int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);
// 
// 							if (iResult == 0)
// 							{
// 								return true;
// 							}

							m_pBank2Wnd = new CPBank4GameWnd(this,m_pGameInfo);
							m_pBank2Wnd->Create(this->m_hWnd,NULL,UI_WNDSTYLE_DIALOG,0,0,0,0,0,NULL);
							m_pBank2Wnd->SetWealthInfor(pBank->i64Bank,GetMeUserInfo()->i64Money);
							m_pBank2Wnd->SetBankVersion(pBank->nVer);
							m_pBank2Wnd->CenterWindow();
							strcpy(m_pBank2Wnd->m_szTwoPassword,pBank->szTwoPassword);
						}

						if(m_pBank2Wnd)
						{
							m_pBank2Wnd->ShowWindow(SW_SHOW);
						}

					}

					break;
				}
			}
			break;
		}
	case ASS_GR_CHECK_OUT://ȡǮ
	case ASS_GR_CHECK_OUT_INGAME://��Ϸ��ȡǮ
		{
			if(pNetHead->bHandleCode==ASS_GR_CHECKOUT_ERROR)
			{
				if (ASS_GR_CHECK_OUT_INGAME == pNetHead->bAssistantID)
				{
					CString strBuffer;
					CString str;
					str = fMsg.GetKeyVal("GameRoom","NetWorkBusy","ȡ��ʧ�ܣ�����ԭ�������벻��ȷ�����㣡");
					strBuffer.Format(str);
					DUIMessageBox(NULL,MB_OK|MB_ICONERROR,m_pGameInfo->szGameName,false,strBuffer.GetBuffer());
				}
			}

			if(pNetHead->bHandleCode==ASS_GR_CHECKOUT_ERROR_PLAYING)
			{
				if (ASS_GR_CHECK_OUT_INGAME == pNetHead->bAssistantID)
				{
					CString strBuffer;
					CString str;
					str = fMsg.GetKeyVal("GameRoom","NoDrawForPlaying","��������Ϸ�У�����ȡ�");
					strBuffer.Format(str);
					DUIMessageBox(NULL,MB_OK|MB_ICONINFORMATION,m_pGameInfo->szGameName,false,strBuffer.GetBuffer());
				}
			}

			if(pNetHead->bHandleCode==ASS_GR_CHECKOUT_SUC)
			{
				//Ч������
				if (uDataSize!=sizeof(MSG_GR_CheckMoney)) return false;
				MSG_GR_CheckMoney * pCheckOut=(MSG_GR_CheckMoney *)pNetData;
				if(pCheckOut->dwUserID==m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID)//�Լ�
				{
					//���¶Ի���
					if (m_pPersonBank!=NULL) 
					{
						if (m_pPersonBank->GetSafeHwnd()!=NULL) 
						{
							m_pPersonBank->UpdateUserMoney(0, pCheckOut->i64CheckOut);
						}
					}
				}
				//���½����Ϣ
				FlushUserMoney(pCheckOut->dwUserID,pCheckOut->i64CheckOut);
			}
			break;
		}
	case ASS_GR_CHECK_IN://��Ǯ
	case ASS_GR_CHECK_IN_INGAME://��Ϸ�д�Ǯ
		{
			if(pNetHead->bHandleCode==ASS_GR_CHECKIN_ERROR)
			{
				if (ASS_GR_CHECK_IN_INGAME == pNetHead->bAssistantID)
				{
					CString strBuffer;
					CString str;
					str = fMsg.GetKeyVal("GameRoom","NetWorkBusySave","����ӵ��,����ʧ��!���Ժ�����!");
					strBuffer.Format(str);
					 DUIMessageBox(NULL,MB_OK|MB_ICONINFORMATION,m_pGameInfo->szGameName,false,strBuffer.GetBuffer());
				}
			}
			if(pNetHead->bHandleCode==ASS_GR_CHECKIN_ERROR_PLAYING)
			{
				if (ASS_GR_CHECK_IN_INGAME == pNetHead->bAssistantID)
				{
					CString strBuffer;
					CString str;
					str = fMsg.GetKeyVal("GameRoom","NoSaveForPlaying","��������Ϸ�У����ܴ�");
					strBuffer.Format(str);
					 DUIMessageBox(NULL,MB_OK|MB_ICONINFORMATION,m_pGameInfo->szGameName,false,strBuffer.GetBuffer());
				}
			}

			if(pNetHead->bHandleCode==ASS_GR_CHECKIN_ZHUANGJIA)  //�ټ����� ��ׯ�в��ܴ�� add by lxl 2010-11-5
			{
				if (ASS_GR_CHECK_IN_INGAME == pNetHead->bAssistantID)
				{
					CString strBuffer;
					CString str;
					str = fMsg.GetKeyVal("GameRoom","NoSaveForZhuangjia","��������ׯ�У����ܴ�");
					strBuffer.Format(str);
					 DUIMessageBox(NULL,MB_OK|MB_ICONINFORMATION,m_pGameInfo->szGameName,false,strBuffer.GetBuffer());
				}
			}

			if(pNetHead->bHandleCode==ASS_GR_CHECKIN_SUC)
			{
				//Ч������
				if (uDataSize!=sizeof(MSG_GR_CheckMoney)) return false;
				MSG_GR_CheckMoney * pCheckIn=(MSG_GR_CheckMoney *)pNetData;

				if(pCheckIn->dwUserID==m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID)//�Լ�
				{
					//���¶Ի���
					if (m_pPersonBank!=NULL) 
					{
						m_pPersonBank->UpdateUserMoney(1, pCheckIn->i64CheckOut);
					}
				}
				FlushUserMoney(pCheckIn->dwUserID,0-pCheckIn->i64CheckOut);
			}

			break;
		}
	case ASS_GR_TRANSFER_MONEY:		   //ת��
	case ASS_GR_TRANSFER_MONEY_INGAME: //��Ϸ��ת��
		{
			CString strBuffer;
			CString str;
			TCHAR szNum[128], szBank[128];
			//Ч������
			if (uDataSize!=sizeof(MSG_GR_S_TransferMoney)) return false;
			MSG_GR_S_TransferMoney * pTransfer=(MSG_GR_S_TransferMoney *)pNetData;
			/// ���ת�ʲ��ɹ���������ƽ̨������ת��ָ��򵯳��Ի���������Ϸ�з���ָ���ƽֻ̨��Ӧ�Ի����¼�
			if (ASS_GR_TRANSFER_MONEY_INGAME == pNetHead->bAssistantID)
			{
				switch(pNetHead->bHandleCode)
				{
				case ASS_GR_TRANSFER_SUC:	///< ת�ʳɹ�					
					{
						/////Added by xqm 20101109
						/////��Z����������ת�ʳɹ�����Ϣ,Ȼ����Z��������ת��Ŀ��ͻ��˷���һ��ת�ʹ�������Ϣ
						//OutputDebugString("��Ϸ����Z������");
						//SendGameData(pNetData, uDataSize, MDM_GR_MONEY, ASS_GR_TRANSFER_MONEY,ASS_GR_TRANSFER_SUC);	
						//SendGameData(pNetData, uDataSize, MDM_GR_MONEY, ASS_GR_TRANSFER_MONEY,ASS_GR_TRANSFER_SUC);	
						///SendGameData((void *)pNetData, uDataSize, pNetHead->bMainID, ASS_GR_TRANSFER_SUC, 0);
						//SendGameData(pNetData, uDataSize, MDM_GR_MONEY, ASS_GR_TRANSFER_MONEY,ASS_GR_TRANSFER_SUC);
						break;
					}
				case ASS_GR_PASSWORD_ERROR:												///< ת���������
					{
						str = fMsg.GetKeyVal("GameRoom","TransferPassWdError","�����������û��ͨ����֤���������������룡");
						strBuffer.Format(str, szNum);
						AFCMessageBox(strBuffer,m_pGameInfo->szGameName);//,MB_OK,this);
						return true;
					}
				case ASS_GR_TRANSFER_TOTAL_LESS:										///< �����ܶ�̫С�������ʸ�
					{
						GlbGetNumString(szNum, pTransfer->i64ActualTransfer, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
						str = fMsg.GetKeyVal("GameRoom","TransferTotalLess","����ʱ��������ת�ʣ����д��ﵽ�򳬹�%s��Ҳſ���ת�ʣ�");
						strBuffer.Format(str, szNum);
						AFCMessageBox(strBuffer,m_pGameInfo->szGameName);//,MB_OK,this);
						return true;
					}
					break;
				case ASS_GR_TRANSFER_TOO_LESS:											///< ����ת����Ŀ̫��
					{
						GlbGetNumString(szNum, pTransfer->i64ActualTransfer, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
						str = fMsg.GetKeyVal("GameRoom","TransferTooLess","ת�ʲ��ɹ�������ת�ʽ��������%s��ҡ�");
						strBuffer.Format(str, szNum);
						AFCMessageBox(strBuffer,m_pGameInfo->szGameName);//,MB_OK,this);
						return true;
					}
					break;
				case ASS_GR_TRANSFER_MULTIPLE:											///< ����ת����Ŀ������ĳ���ı���
					{
						GlbGetNumString(szNum, pTransfer->i64ActualTransfer, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
						str = fMsg.GetKeyVal("GameRoom","TransferMultiple","ת�ʲ��ɹ���ת�ʽ�������%s����������");
						strBuffer.Format(str, szNum);
						AFCMessageBox(strBuffer,m_pGameInfo->szGameName);//,MB_OK,this);
						return true;
					}
				case ASS_GR_TRANSFER_NOT_ENOUGH:										///< ���н�������ת��
					{
						GlbGetNumString(szNum, pTransfer->i64Money, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
						GlbGetNumString(szBank, pTransfer->i64ActualTransfer, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
						str = fMsg.GetKeyVal("GameRoom","TransferNotEnoughMoney","ת�ʲ��ɹ�����������ֻ��%s��ң�����֧������ת��%s��ҡ�");
						strBuffer.Format(str, szBank, szNum);
						AFCMessageBox(strBuffer,m_pGameInfo->szGameName);//,MB_OK,this);
						return true;
					}
				case ASS_GR_TRANSFER_NO_DEST:											///< ת��Ŀ�겻����
					{
						str = fMsg.GetKeyVal("GameRoom","AccountNotExist","ת��Ŀ�겻����,���ʵ�ʺ�����!");
						strBuffer.Format(str);
						AFCMessageBox(strBuffer,m_pGameInfo->szGameName);//,MB_OK,this);
						return true;
					}
					break;
				default:
					{
						CString strBuffer;
						CString str;
						str = fMsg.GetKeyVal("GameRoom","NetWorkBusySave","����ӵ��,����ʧ��!���Ժ�����!");
						strBuffer.Format(str);
						AFCMessageBox(strBuffer,m_pGameInfo->szGameName);//,MB_OK,this);
						return true;
					}
				}
			}
			/// ת�ʳɹ������ô�����Һ����н�����
			if(pNetHead->bHandleCode==ASS_GR_TRANSFER_SUC)
			{
				TCHAR szAct[128];
				CString strBuffer;
				CString str;
				GlbGetNumString(szNum, pTransfer->i64Money, Glb().m_nPowerOfGold/*, false*/, Glb().m_bUseSpace, Glb().m_strSpaceChar);
				GlbGetNumString(szAct, pTransfer->i64ActualTransfer, Glb().m_nPowerOfGold/*, false*/, Glb().m_bUseSpace, Glb().m_strSpaceChar);
				/// ���Լ�ת������
				if(pTransfer->UserID == m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID)//�Լ�
				{
					if (ASS_GR_TRANSFER_MONEY_INGAME == pNetHead->bAssistantID)
					{
						//��ʾ�û�
						if(pTransfer->bUseDestID)
						{
							str = fMsg.GetKeyVal("GameRoom","TransferCoinsId","��ת��ID[%d]����� %s ��ң��۳������Ѻ�ʵ�ʵ��� %s ���!");
							strBuffer.Format(str, pTransfer->destUserID, szNum, szAct);
						}
						else
						{
							str = fMsg.GetKeyVal("GameRoom","TransferCoinsNn","��ת���ǳ�[%s]����� %s ��ң��۳������Ѻ�ʵ�ʵ��� %s ���!");
							strBuffer.Format(str, pTransfer->szDestNickName, szNum, szAct);
						}
						AFCMessageBox(strBuffer,m_pGameInfo->szGameName);//,MB_OK,this);
					}
					//���¶Ի���
					if (m_pPersonBank!=NULL) 
					{
						m_pPersonBank->UpdateUserMoneyByTransfer(0, pTransfer->i64Money);
					}
				}
				/// ת��Ŀ�����Լ�
				else if (pTransfer->destUserID==m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID)
				{
					if (ASS_GR_TRANSFER_MONEY_INGAME == pNetHead->bAssistantID)
					{
						str = fMsg.GetKeyVal("GameRoom","BeTransfered","���[%d]ת���� %s ��ң��۳������Ѻ�ʵ�ʵ��� %s ���!");
						strBuffer.Format(str, pTransfer->UserID, szNum, szAct);
						AFCMessageBox(strBuffer,m_pGameInfo->szGameName);//,MB_OK,this);
					}
					//���¶Ի���
					if (m_pPersonBank!=NULL) 
					{
						m_pPersonBank->UpdateUserMoneyByTransfer(1, pTransfer->i64ActualTransfer);
					}
				}
			}
			break;
		}
	default:
		break;
	}
	return true;
}
/// ˢ���û��б��еĽ��
/// @param dwUserID �û�ID��
/// @param uCount ��Ϊ��������ֵ����˼Ϊ�ӻ��
bool CLoveSendClassInExe::FlushUserMoney(long dwUserID,__int64 i64Count)
{
	UserItemStruct * pUserItem=FindOnLineUser(dwUserID);
	if(pUserItem==NULL)
		return false;

	//��������
	pUserItem->GameUserInfo.i64Money += i64Count;
	pUserItem->GameUserInfo.i64Bank -= i64Count;//���н��

	//������λ���û�
	m_UserListDlg.UpdateGameUser(pUserItem);
	if(pUserItem->GameUserInfo.dwUserID==m_CurpUserInfoClicked->dwUserID)
	{
		NavigateVirFace();
	}
	return true;
}

/// ����Ϸ�����д����н���
void CLoveSendClassInExe::OnVisitBank()
{
	//AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
	////��������
	//if (m_pPersonBank == NULL) 
	//{
	//	try
	//	{
	//		m_pPersonBank=new CPBankDlg(this);
	//		if (m_pPersonBank==NULL)
	//			return;
	//	}
	//	catch (...) { return; }
	//}
	//if (m_pPersonBank!=NULL)
	//{
	//	/// �ȸ�ֵ
	//	m_pPersonBank->SetWealthInfor(100000, 500000);
	//	if (m_pPersonBank->GetSafeHwnd()==NULL)
	//	{
	//		AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
	//		m_pPersonBank->DoModal();//Create(IDD_DIALOGBANK,this);
	//	}
	//}
	//AfxSetResourceHandle(GetModuleHandle(NULL));

	// duanxiaohui 20111103 ���鳡(��ѳ�) ����ʹ�����й���
	if (m_pGameInfo->dwRoomRule & GRR_EXPERCISE_ROOM)
	{
		AFCMessageBox("�Բ���, ���鳡(��ѳ�)����ʹ�����й���", "��ܰ��ʾ");
		return ;
	}
	// end duanxiaohui

	if((m_pPersonBank->GetSafeHwnd() != NULL) && (::IsWindowVisible(m_pPersonBank->GetSafeHwnd())))
	{
		/// ��������Ѿ��򿪣��ٴε��ʱ�ر�����
		::PostMessage(m_pPersonBank->GetSafeHwnd(), WM_COMMAND, MAKELONG(IDCANCEL,BN_CLICKED), 0);
	}
	else
	{
		SendGameData(MDM_GR_MONEY,ASS_GR_OPEN_WALLET_INGAME,0);
	}
}

//������Ϣ
bool CLoveSendClassInExe::OnPropMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
	switch(pNetHead->bAssistantID)
	{
		//wushuqun 2008.9.28
		//�޸�С����ʹ�ú�û�и��¿ͻ��˵��������ʾ 
	case ASS_PROP_SMALL_BOARDCASE:
		{
            // PengJiLin, 2010-10-13, �ڷ���ʹ��С���ȣ���Ϸ�˵�����û�иı䣬���������´���
            // boardCast->iLength ����Ϊ���䴫�ݹ�����������ֱ��ʹ��
            _TAG_BOARDCAST *boardCast=(_TAG_BOARDCAST*)pNetData;
            int propCount=Glb().userPropLibrary.GetCount();
            _TAG_USERPROP *userProp=NULL;
            for(int i=0;i<propCount;i++)
            {
                userProp=Glb().userPropLibrary.GetAt(i);
                if(userProp->nPropID==boardCast->nPropID)
                {
                    userProp->nHoldCount = boardCast->iLength;                        
                }
            }
			
			OnSetPropImages();
			break;
		}
	case ASS_PROP_USEPROP:
		return OnUsePropResult(pNetHead,pNetData,uDataSize);
		break;
	case ASS_PROP_VIP_CHECKTIME:
		return OnPropVipCheckTime(pNetHead,pNetData,uDataSize);
		break;
    case ASS_PROP_NEW_KICKUSER:    // PengJiLin, 2010-9-14, ʹ�����˿����˽����Ϣ
        return OnNewKickUserResult(pNetHead,pNetData,uDataSize);
        break;
	}
	return true;
}
//����ͬ���������
void CLoveSendClassInExe::ResetUserInfo(void *pUserInfoVoid)
{
	UserItemStruct *pUserInfo =(UserItemStruct *)pUserInfoVoid;
	INT_PTR nPlayerCount = m_ThisDeskPtrArray.GetCount();
	if (GetMeUserInfo()->dwUserID == pUserInfo->GameUserInfo.dwUserID)
	{
		memcpy(&m_pGameInfo->uisMeUserInfo, pUserInfo, sizeof(UserItemStruct));
		if (NULL != m_pPropPannel)
		{
			m_pPropPannel->ResetData();
		}
	}
	for (INT_PTR i=0; i<nPlayerCount; ++i)
	{
		UserItemStruct *pFindUserItem = (UserItemStruct *)m_ThisDeskPtrArray.GetAt(i);
		if (pFindUserItem->GameUserInfo.dwUserID == pUserInfo->GameUserInfo.dwUserID)
		{
			memcpy(pFindUserItem, pUserInfo, sizeof(UserItemStruct));
			bool bRet = m_UserListDlg.UpdateGameUser(pFindUserItem);
			//DebugPrintf("ResetUserInfo %s bUserState = %d", bRet?"�ɹ�":"ʧ��", pFindUserItem->GameUserInfo.bUserState);
			break;
		}
	}
	if( pUserInfo->GameUserInfo.dwUserID== this->m_CurpUserInfoClicked->dwUserID)
	{
		NavigateVirFace();
	}
}

void CLoveSendClassInExe::OnAddFriend(bool bIsHave)
{
	CString s;
	s.Format("xxx CLoveSendClassInExe OnAddFriend %d, %d", bIsHave, m_dwCurrentUserID);
	OutputDebugString(s);
	if (bIsHave)
	{
		AFCMessageBox("�Բ��𣬸��û��Ѿ������ĺ��ѣ�����Ҫ����ӣ�", "��ʾ");
	}
	else
	{
		//��������
		UserItemStruct * pUserItem=FindOnLineUser(m_dwCurrentUserID);
		if ((pUserItem==NULL))
			return;

		//Ctest _test;
		//�����������
		CIMSetUser imsu;
		imsu.dwUser = m_dwCurrentUserID;
		imsu.szName = pUserItem->GameUserInfo.nickName;
		AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
		if(imsu.DoModal()==IDCANCEL)
			return;
		AfxSetResourceHandle(GetModuleHandle(NULL));

		MSG_IM_C_ADDREQUEST* msg = new MSG_IM_C_ADDREQUEST;
		ZeroMemory(msg,sizeof(MSG_IM_C_ADDREQUEST));

		//CMainRoomEx *pWnd=(CMainRoomEx*)pParentWnd;
		msg->dwUserID= GetMeUserInfo()->dwUserID;
		msg->dwRequestedUserID= m_dwCurrentUserID;
		memcpy(&(msg->sUserName), GetMeUserInfo()->nickName,sizeof(GetMeUserInfo()->nickName));
		memcpy(&(msg->sRequestNotes),imsu.imWords,imsu.imWords.GetLength());
		msg->nMsgLength=imsu.imWords.GetLength();
		msg->cbSize=sizeof(MSG_IM_C_ADDREQUEST)-sizeof(msg->sRequestNotes)+msg->nMsgLength+1;//ʵ�ʳ���

		//MSG_IM_C_ADDREQUEST_Game t;
		//t.dwUserID1 = GetMeUserInfo()->dwUserID;
		//t.dwUserID = m_dwCurrentUserID;
		m_pGameRoom->PostMessage(WM_FRIEND_R, sizeof(MSG_IM_C_ADDREQUEST), LPARAM(msg));
		return;
	}
}

void CLoveSendClassInExe::OnGetFriendList(void* pBuff, int nSize)
{
	MSG_IM_C_GETFRIENDLIST *msg = (MSG_IM_C_GETFRIENDLIST*)pBuff;

	if(nSize == 0)
	{
		m_FriendList.erase(m_FriendList.begin(), m_FriendList.end());
	}
	else if(nSize == sizeof(MSG_IM_C_GETFRIENDLIST))
	{
		m_FriendList.push_back(*msg);
	}
	return;
}

// duanxiaohui 2011-11-11 ���������
void CLoveSendClassInExe::OnAddEmeny(bool bIsHave)
{
	//��������
	UserItemStruct * pUserItem = FindOnLineUser(m_dwCurrentUserID);
	if (pUserItem == NULL)
	{	
		return;
	}

	MSG_IM_C_SETGROUP * msg = new MSG_IM_C_SETGROUP;
	ZeroMemory(msg, sizeof(MSG_IM_C_SETGROUP));

	msg->dwUserID = GetMeUserInfo()->dwUserID;
	msg->dwRemoteUserID = m_dwCurrentUserID;
	msg->groupID = 3;   // ��ID��1-���ѣ�3-������,0-ɾ��

	m_pGameRoom->PostMessage(WM_EMENY_R, sizeof(MSG_IM_C_SETGROUP), LPARAM(msg));
	return;
}

//�鿴�û���Ϣ
BOOL CLoveSendClassInExe::OnCmdShowUserInfo()
{
	if (m_dwCurrentUserID != 0L)
	{
		UserItemStruct* pUserInfo = FindOnLineUser(m_dwCurrentUserID);

		m_pUserDetailsDlg->SetUserItem(pUserInfo);

		POINT point;
		::GetCursorPos(&point);

		CRect rect;
		m_pUserDetailsDlg->GetClientRect(&rect);

		if (m_pRightFrame)
		{
			RECT rcRightWnd;
			::GetWindowRect(m_pRightFrame->GetHWND(),&rcRightWnd);
			m_pUserDetailsDlg->MoveWindow(rcRightWnd.left - rect.Width(), point.y - 20, rect.Width(), rect.Height());
		}
		else
		{
			CRect listRect;
			m_UserListDlg.GetWindowRect(&listRect);
			m_pUserDetailsDlg->MoveWindow(listRect.left - rect.Width() + 15, point.y - 10, rect.Width(), rect.Height());
		}

		m_pUserDetailsDlg->ShowWindow(SW_SHOW);
	}

	return TRUE;
}

//�����û���Ϣ
BOOL CLoveSendClassInExe::OnCmdCopyUserInfo()
{
	if (m_dwCurrentUserID != 0)
	{
		UserItemStruct* pUserItem = FindOnLineUser(m_dwCurrentUserID);

		//��������
		if ((pUserItem==NULL)||(OpenClipboard()==FALSE))
			return TRUE;
		if (EmptyClipboard()==FALSE) 
		{
			CloseClipboard();
			return TRUE;
		}

		//��������
		HANDLE hData=GlobalAlloc(GMEM_MOVEABLE,sizeof(pUserItem->GameUserInfo.nickName));
		if (hData==NULL) 
		{
			CloseClipboard();
			return TRUE;
		}

		LPTSTR szMemName=(LPTSTR)GlobalLock(hData);

		// duanxiaohui 2011-11-21 ���Ƶ�ճ��������Ҫת����Unicode
		//lstrcpy(szMemName,pUserItem->GameUserInfo.nickName);

		CString strText;
		strText.Format("%s", pUserItem->GameUserInfo.nickName);
		if (0 == MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strText, strText.GetLength() + 1, (WCHAR*)szMemName,  1024)) 
		{ 
			GlobalUnlock(hData); 
			CloseClipboard(); 
			GlobalFree(hData); 
			return TRUE; 
		} 
		// end duanxiaohui

		SetClipboardData(CF_UNICODETEXT,hData);
		GlobalUnlock(hData); 
		GlobalFree(hData);
		CloseClipboard();

		//��ʾ����
		CString strBuffer;
		m_InputMessage.GetWindowText(strBuffer);
		strBuffer+=pUserItem->GameUserInfo.nickName;
		m_InputMessage.SetWindowText(strBuffer);
		m_InputMessage.SetFocus();
	}

	return TRUE; 
}

//����
BOOL CLoveSendClassInExe::OnCmdKickUser()
{
	if (m_dwCurrentUserID!=0L)
	{
		//wushuqun 2009.5.19
		//�Թ�ʱ����T��
		if (GetMeUserInfo()->bUserState == USER_WATCH_GAME)
		{
			AFCMessageBox("�Բ����������Թ���Ϸ�����ܽ��д˲���", "��ʾ");
			return true;
		}

		if(GetMeUserInfo()->dwUserID == m_dwCurrentUserID)
		{
			AFCMessageBox("�Բ���,�޷����Լ����д������","��ʾ");
			return true;
		}

		CString stime;
		stime.Format("%d",CTime::GetCurrentTime());        //��ǰ��ʱ�䣬String��ʽ
		int curtime=atoi(stime);                        //ת��Ϊint
		if(GetMeUserInfo()->iVipTime < curtime)
		{
			AFCMessageBox("�Բ���,ֻ��vip���д����","��ʾ");
			return true;
		}
		///����Ƿ�Ϊ�Թ� add by rmaon 09.04.27
		if( m_bWatchMode )
		{
			AFCMessageBox("�Բ���,�Թ�״̬��������","��ʾ");
			return true;
		}
		//��������
		UserItemStruct * pUserItem=FindOnLineUser(m_dwCurrentUserID);
		if (pUserItem==NULL) 
			return TRUE;
		if(pUserItem->GameUserInfo.iVipTime>curtime)
		{
			AFCMessageBox("�Բ���,�Է���Vip,����ʧ��!","��ʾ");
			return TRUE;
		}

		// PengJiLin, 2010-9-10, �������Ƿ������˿������˹���
		CString sPath=CBcfFile::GetAppPath();
		CBcfFile bcfFile( sPath + "..\\Order.bcf");
		int iValue = bcfFile.GetKeyVal(_T("ClassInExe"), _T("UseKickUserProp"), 0);
		if(iValue > 0)      // ��������˿������߿����ܣ�ʹ���µ����˹���
		{
			_TAG_NEW_KICKUSER stKick;
			stKick.dwUserID = GetMeUserInfo()->dwUserID;
			stKick.dwDestID = m_dwCurrentUserID;
			SendGameData(&stKick,sizeof(stKick),MDM_GR_PROP,ASS_PROP_NEW_KICKUSER,0);
		}
		else
		{
			_TAG_KICKUSER stKick;
			stKick.dwUserID = GetMeUserInfo()->dwUserID;
			stKick.dwDestID = m_dwCurrentUserID;
			SendGameData(&stKick,sizeof(stKick),MDM_GR_PROP,ASS_PROP_KICKUSER,0);
		}
	}

	return TRUE;
}

//����
BOOL CLoveSendClassInExe::OnCmdKickUserProp()
{
	if (m_dwCurrentUserID!=0L)
	{
		// �Թ�ʱ����T��
		if (GetMeUserInfo()->bUserState == USER_WATCH_GAME)
		{
			AFCMessageBox("�Բ����������Թ���Ϸ�����ܽ��д˲���", "��ʾ");
			return true;
		}
		// ����Ƿ�Ϊ�Թ�
		if( m_bWatchMode )
		{
			AFCMessageBox("�Բ���,�Թ�״̬��������","��ʾ");
			return true;
		}
		if(GetMeUserInfo()->dwUserID == m_dwCurrentUserID)
		{
			AFCMessageBox("�Բ���,�޷����Լ����д������","��ʾ");
			return true;
		}

		CString stime;
		stime.Format("%d",CTime::GetCurrentTime());         // ��ǰ��ʱ�䣬String��ʽ
		int curtime=atoi(stime);                            // ת��Ϊint
		UserItemStruct * pUserItem=FindOnLineUser(m_dwCurrentUserID);
		if (pUserItem==NULL) 
		{
			AFCMessageBox("�Բ���,�Է�״̬�쳣,����ʧ��!","��ʾ");
			return TRUE;
		}
		if(pUserItem->GameUserInfo.iVipTime>curtime)
		{
			AFCMessageBox("�Բ���,�Է�Ҳ��Vip,����ʧ��!","��ʾ");
			return TRUE;
		}

		// �������ݷ������ȽϷŵ��洢������ʵ��
		_TAG_NEW_KICKUSER stKick;
		stKick.dwUserID = GetMeUserInfo()->dwUserID;
		stKick.dwDestID = m_dwCurrentUserID;
		SendGameData(&stKick,sizeof(stKick),MDM_GR_PROP,ASS_PROP_NEW_KICKUSER,0);
	}

	return TRUE;
}

//���������Ϣ
bool CLoveSendClassInExe::OnUsePropResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
	if(pNetHead->bHandleCode!=DTK_GR_PROP_USE_SUCCEED)
	{
		switch(pNetHead->bHandleCode)
		{
		case DTK_GP_PROP_GIVE_NOTARGETUSER:
			ShowMsgInGame("Ŀ����Ҳ���������״̬��");
			break;
		default:
			break;
		}
		return true;
	}
	if(sizeof(_TAG_USINGPROP)!= uDataSize)
		return false;
	_TAG_USINGPROP * usingProp = (_TAG_USINGPROP *)pNetData;
	UserItemStruct *pUserInfo =FindOnLineUser(usingProp->dwTargetUserID);
	UserItemStruct *pUserInfoUsed =FindOnLineUser(usingProp->dwUserID);
	if(pUserInfo == NULL || pUserInfoUsed == NULL)
		return true;
	// ������Լ�ʹ�õĵ��ߣ�Ҫ�۳���������
	if (usingProp->dwUserID == GetMeUserInfo()->dwUserID)
	{
		_TAG_USERPROP *userProp = NULL;
		for (int i=0; i<Glb().userPropLibrary.GetCount(); ++i)
		{
			userProp=Glb().userPropLibrary.GetAt(i);
			if ((userProp!=NULL) && (userProp->nPropID == usingProp->nPropID))
			{
				break;
			}
		}
		if((userProp!=NULL) && (userProp->nHoldCount<=0))
		{
			userProp->nHoldCount-=1;
            if(userProp->nHoldCount < 0)userProp->nHoldCount = 0;
		}
	}
	
	//ʹ���ߵ��ǳ�
	CString name1=pUserInfoUsed->GameUserInfo.nickName;// rewrite by ramon
	//��ʹ���ߵ��ǳ�
	CString name2=pUserInfo->GameUserInfo.nickName;	// rewrite by ramon

	if(usingProp->dwUserID==m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID)
		name1="��";
	if(usingProp->dwTargetUserID==m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID)
		name2="��";
	CString name3=name2;
	if(usingProp->dwUserID==usingProp->dwTargetUserID)
		name2="�Լ�";

	int attribAction=usingProp->nPropActionAttrib;
	int attribValue=usingProp->nPropValueAttrib;

	CString stip;
	stip.Format("%s �� %s ʹ���� %s ����",name1,name2,usingProp->szPropName);
	m_MessageHandle.InsertSystemMessageM(stip.GetBuffer());

	CString _s = "B";
	if (usingProp->dwTargetUserID==m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID)
	{
		_s = "B";
	}
	if (usingProp->dwUserID==m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID)
	{
		_s = "A";
	}
	if (_s != "" && pUserInfo->GameUserInfo.bDeskNO == m_pGameInfo->uisMeUserInfo.GameUserInfo.bDeskNO)
	{
		CString soundFile;
		soundFile.Format("..\\image\\prop\\Prop%d%s.mp3", usingProp->nPropID, _s);
		BZSoundPlay(this,soundFile.GetBuffer(),0,1);
	}
	
	//if(m_hWnd)
	//	Invalidate(FALSE);

	//˫������
	if(attribAction & 0x4)
	{
		pUserInfo->GameUserInfo.iDoublePointTime=usingProp->iDoubleTime;
		if(usingProp->dwTargetUserID==m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID)
		{
			time_t t=(time_t)usingProp->iDoubleTime;
			CTime expiresTime(t);
			stip.Format(TEXT("����˫�����ֹ��ܿ���ʹ�õ� %s��"),expiresTime.Format("%Y-%m-%d %H:%M:%S"));
			m_MessageHandle.InsertSystemMessageM(stip.GetBuffer());
		}
	}
	if(attribAction & 0x8)
	{
		pUserInfo->GameUserInfo.iProtectTime=usingProp->iProtectTime;
		if(usingProp->dwUserID == m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID)
		{
			time_t t=(time_t)usingProp->iProtectTime;
			CTime expiresTime(t);
			stip.Format(TEXT("���Ļ��������(���˲��ۻ���)����ʹ�õ� %s��"),expiresTime.Format("%Y-%m-%d %H:%M:%S"));
			m_MessageHandle.InsertSystemMessageM(stip.GetBuffer());
		}
	}

	if(attribAction & 0x10)
	{
		int fasciChange=(attribValue & 0xFF0000)>>16;
		stip.Format("%s ������ֵ���� %d ��",name3,fasciChange);
		if(attribAction & 0x20)
		{
			stip.Format("%s ������ֵ���� %d ��",name3,fasciChange);
			fasciChange=-fasciChange;
		}
		//	pUserInfo->GameUserInfo.dwFascination+=fasciChange;
		m_MessageHandle.InsertSystemMessageM(stip.GetBuffer());
		m_UserListDlg.Invalidate(FALSE);
	}
	if(attribAction & 0x2)
	{
		CString str;
		str.Format("������0���߽��:%d ������0",usingProp->dwTargetUserID);
		//OutputDebugString(str);
		if(usingProp->dwTargetUserID==m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID)
		{
			if(m_pGameInfo->uisMeUserInfo.GameUserInfo.dwPoint<0)
				m_pGameInfo->uisMeUserInfo.GameUserInfo.dwPoint=0;
		}
		if (pUserInfoUsed->GameUserInfo.dwPoint<0)
		{
			pUserInfoUsed->GameUserInfo.dwPoint = 0;
		}
		if (pUserInfoUsed->GameUserInfo.dwUserID == m_CurpUserInfoClicked->dwUserID)
		{
			NavigateVirFace();
		}
	}

	this->m_pGameRoom->PostMessage(WM_ACTIVE_TO_ROOM, 1, 1);
	//��������б�
	bool bRet = m_UserListDlg.UpdateGameUser(pUserInfo);
	//OnSetPropImages();
	//m_BtPropGoAhead.Invalidate(FALSE);
	//m_BtPropGoNext.Invalidate(FALSE);
	//m_BtProp1.Invalidate(FALSE);
	//m_BtProp2.Invalidate(FALSE);
	//m_BtProp3.Invalidate(FALSE);
	//m_BtProp4.Invalidate(FALSE);
//	this->Invalidate(FALSE);
	return true;
}
//vipʱ����
bool CLoveSendClassInExe::OnPropVipCheckTime(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
	if(uDataSize!=sizeof(_TAG_PROP_TIME))
		return false;
	_TAG_PROP_TIME * pData = (_TAG_PROP_TIME *)pNetData;
	CString stime;
	stime.Format("%d",CTime::GetCurrentTime());        //��ǰ��ʱ�䣬String��ʽ
	int curtime=atoi(stime);                        //ת��Ϊint

	if(GetMeUserInfo()->iVipTime-curtime < 10*60 && GetMeUserInfo()->iVipTime > curtime)
	{
		//��������
		CString strMessage;
		strMessage.Format(TEXT("����Vip��ݽ��� %d ���Ӻ����!"),GetMeUserInfo()->iVipTime/60);
		m_MessageHandle.InsertSystemMessage(strMessage.GetBuffer());
	}
	else
	{
		if(GetMeUserInfo()->iVipTime <=curtime)
		{
			//��������
			CString strMessage;
			strMessage.Format(TEXT("����Vip����Ѿ��������������¹���"));
			m_MessageHandle.InsertSystemMessage(strMessage.GetBuffer());
		}
	}
	//OutputDebugString("OnPropVipCheckTime");
	OnSetPropImages();
	return true;
}

// PengJiLin, 2010-9-14, ʹ�����˿����˽����Ϣ
bool CLoveSendClassInExe::OnNewKickUserResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
    if(uDataSize!=sizeof(_TAG_NEW_KICKUSER)) return false;

    _TAG_NEW_KICKUSER* pKickResult = (_TAG_NEW_KICKUSER*)pNetData;
    if(pKickResult != NULL && pKickResult->dwUserID == m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID)   // �Լ��߱���
    {
        switch(pNetHead->bHandleCode)
        {
        case ERR_GR_NEW_KICKUSER_SUCCESS:       // �ɹ�
            {
                UserItemStruct* pUserInfo = NULL;
                for (int i = 0; i < MAX_PEOPLE; ++i)
                {
                    pUserInfo = m_pUserInfo[i];
                    if(pUserInfo != NULL && pUserInfo->GameUserInfo.dwUserID == pKickResult->dwDestID)
                    {
                        break;
                    }
                    pUserInfo = NULL;
                }
                CString strInfo = "";
                if(NULL != pUserInfo)
				{
					strInfo.Format("���%s�ѱ��߳���Ϸ��", pUserInfo->GameUserInfo.nickName);
				}
                else
				{
					strInfo.Format("���%d�ѱ��߳���Ϸ��", pKickResult->dwDestID);
				}
                
                //ShowNewKickPropDlg(TRUE, FALSE, TRUE, strInfo);
				m_MessageHandle.InsertSystemMessage(strInfo.GetBuffer());
            }
            break;
        case ERR_GR_NEW_KICKUSER_NOTIME:        // �Լ��ĵ����ѹ��ڣ�����û�е���
            {
                CString strInfo = "";
                strInfo.Format("��û��\"���˿�\"���ߣ������Ѿ�����");
                //ShowNewKickPropDlg(TRUE, TRUE, TRUE, strInfo);
				m_MessageHandle.InsertSystemMessage(strInfo.GetBuffer());
            }
            break;
        case ERR_GR_NEW_KICKUSER_HAD_ANTI:      // �Է��з��߿�ʱ��
            {
                CString strInfo = "";
                strInfo.Format("�����ʹ����\"���߿�\"�������ܽ����߳���Ϸ��");
                //ShowNewKickPropDlg(TRUE, FALSE, TRUE, strInfo);
				m_MessageHandle.InsertSystemMessage(strInfo.GetBuffer());
            }
            break;
        case ERR_GR_NEW_KICKUSER_HAD_VIP:       // �Է���VIPʱ��
            {
                CString strInfo = "";
                strInfo.Format("�������VIP��Ա�������ܽ����߳���Ϸ��");
                //ShowNewKickPropDlg(TRUE, FALSE, TRUE, strInfo);
				m_MessageHandle.InsertSystemMessage(strInfo.GetBuffer());
            }
            break;
        case ERR_GR_NEW_KICKUSER_PLAYING:       // ��Ϸ�в�����
            m_MessageHandle.InsertSystemMessage("����ʧ�ܣ��Է�������Ϸ");
            break;
        default:
            break;
        }
    }
	return true;
}

//�ر�����
void CLoveSendClassInExe::OnCloseBank2()
{
	if (m_pBank2Wnd)
	{	
		delete m_pBank2Wnd ;
		m_pBank2Wnd = NULL;
		//���رգ�ֻ����
		//m_pBank2Wnd->ShowWindow(SW_HIDE);
	}

	return;
}

//������Ϸ������Ϣ
void CLoveSendClassInExe::ShowMsgInGame(TCHAR * szMsg, BYTE bNewOrSys,UINT uSize, TCHAR * szFontName,TCHAR *Title)
{
	if (NULL == szMsg)
	{
		SetForegroundWindow();
		//OnBnClickedMax();
	}
	else
	{
		m_MessageHandle.InsertTitleMessage(szMsg,bNewOrSys,uSize,szFontName,Title);
	}
}

//desc:��ȡ��Ϸ���
//remark:�ټ��� add by wlr 20090716
void CLoveSendClassInExe::GetGameCode(TCHAR * szGameCode)
{
	::strcpy(szGameCode,"00000000");
}
//end of  add by wlr 20090716

///�ж��Ƿ����Ŷӻ�����
///@return true:���Ŷӻ����䣻false�������Ŷӻ�����
bool CLoveSendClassInExe::IsQueueGameRoom() const
{
	return(((m_pGameInfo->dwRoomRule & GRR_QUEUE_GAME) > 0) ? true : false);
}

//��������Ϣ
void CLoveSendClassInExe::ShowMsgDudu(TCHAR * szSendName, TCHAR * szTargetName,TCHAR * szCharString, COLORREF crTextColor, bool bShowTime, UINT uSize, TCHAR * szFontName,int iDuduType)
{
	//wushuqun 2009.5.27
	//�����������Ϣ
// 	if (strcmp(szSendName ,this->GetMeUserInfo()->nickName) == 0)
// 	{
// 		//OutputDebugString("ShowMsgDudu");
// 		this->OnSetPropImages();
// 	}
	m_MessageHandle.InsertDudu(szSendName,szTargetName,szCharString,crTextColor,false,0,0,iDuduType);
}

void CLoveSendClassInExe::OnCbnSelchangeInput()
{
	SetTimer(1,10,NULL);
}

void CLoveSendClassInExe::OnBnClickedSend()
{
	SendTalkMessage();
	return;
}

void CLoveSendClassInExe::OnBnClickedSetcolor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CLoveSendClassInExe::OnBnClickedExpression()
{
	//�������鴰��
	if (m_ExpressWnd.GetSafeHwnd()==NULL) m_ExpressWnd.CreateExpressionWnd(this);
	
	//�ƶ�����
	CRect ButtonRect;
	CSize ExpressSize;
	CRect ClientRect;
	GetClientRect(&ClientRect);
	m_ExpressWnd.GetFixSize(ExpressSize);
	m_BtExpression.GetWindowRect(&ButtonRect);
	SafeMoveWindow(&m_ExpressWnd,ButtonRect.right-ExpressSize.cx+176,ButtonRect.top-ExpressSize.cy-3,ExpressSize.cx,ExpressSize.cy);
	//SafeMoveWindow(&m_ExpressWnd,min(ButtonRect.right,max(0,ClientRect.right-ExpressSize.cx)) + 320,ButtonRect.top-ExpressSize.cy,ExpressSize.cx,ExpressSize.cy);
	m_ExpressWnd.ShowWindow(SW_SHOW);
	m_ExpressWnd.SetFocus();

	return;
}

void CLoveSendClassInExe::OnBnClickedGoAhead()
{
	//OutputDebugString("OnBnClickedGoAhead");
	m_curPropPage--;
	OnSetPropImages();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

// PengJiLin, 2010-10-12, �̳����߹���
void CLoveSendClassInExe::OnBnClickedGoAheadS()
{
	m_curPropPage--;
	OnSetPropImages();
	if (m_pPropPannel != NULL)
	{
		m_pPropPannel->SetFocus();
	}
}

void CLoveSendClassInExe::OnBnClickedGoNext()
{
	//OutputDebugString("���˵������°�ť");
	m_curPropPage++;
	OnSetPropImages();

	//OutputDebugString("OnBnClickedGoNext");
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CLoveSendClassInExe::OnBnClickedGoNextS()
{
	m_curPropPage++;
	OnSetPropImages();
	if (m_pPropPannel != NULL)
	{
		m_pPropPannel->SetFocus();
	}
}

// PengJiLin, 2010-9-30, �������
void CLoveSendClassInExe::OnBnClickedBlueDiamond()
{
    //���ΰټ�����ع��� add by wlr 20090716
    char GameCode[20];
    ::memset(GameCode,0,sizeof(GameCode));
    GetGameCode(GameCode);
    CString str = GameCode;
    //if (str == "11901800"
    //	|| str == "10901800"
    //	|| str == "30501800")
    TCHAR szKey[10];
    CString sPath=CBcfFile::GetAppPath();
    CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
    wsprintf(szKey, "%d", m_pGameInfo->uNameID);
    int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

    if (iResult>0)
        return ;
    //end of add by wlr 20090716

    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString s=CBcfFile::GetAppPath () + "..\\";
    CBcfFile f( s + "bzgame.bcf");
    CString url;
    url=f.GetKeyVal("BZW","gameurl5","http://www.kfgame.com");

    //CString surl;
    //surl.Format("%s/app/userlogin.asp?userid=%d&token=%s&url=%s",
    //	Glb().m_CenterServerPara.m_strWebRootADDR,m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID,Glb().m_TML_SN,url);
    //surl.Format("%s/app/userlogin.asp?userid=%d&token=",
    //            url,m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID);
    //ShellExecute(NULL,"open",surl,NULL,NULL,SW_MAXIMIZE|SW_SHOW);
    ShellExecute(NULL,"open",url,NULL,NULL,SW_MAXIMIZE|SW_SHOW);
    return ;
}



void CLoveSendClassInExe::OnClose()
{
	// ֪ͨƽ̨���Ѿ��˳�
	//OnBnClickedExit();
	CLoadFileBmp::OnClose();
}

void CLoveSendClassInExe::OnCancel()
{
	if(m_pPersonBank != NULL)
	{
		if(m_pPersonBank->GetSafeHwnd()!=NULL)
			m_pPersonBank->EndDialog(0);
		delete m_pPersonBank;
		m_pPersonBank = NULL;
	}
	//OutputDebugString("[aa]SDK CLoveSendClassInExe::OnCancel() begin 0");
	// TODO: �ڴ����ר�ô����/����û���
	m_TalkMessage.DestroyWindow();
	CWnd * winApp=AfxGetApp()->m_pMainWnd;
	if(winApp && winApp->m_hWnd){
		winApp->PostMessage(WM_GAME_QUIT,0,0);
		//OutputDebugString("[aa]SDK CLoveSendClassInExe::OnCancel() end 0");
	}

	//��ԭ���ڵķֱ���
	if (m_bShowMax)
	{

		CBcfFile f(CBcfFile::GetAppPath() + _T("GameOption.bcf"));
		UINT uMaxType = f.GetKeyVal("FrameControl","max_type",1);

		if (uMaxType == 2) //�޸ķֱ���
		{
			DEVMODE lpDevMode;
			lpDevMode.dmBitsPerPel=32;
			lpDevMode.dmPelsWidth = m_nWiondwMetricsX;
			lpDevMode.dmPelsHeight = m_nWiondwMetricsY;
			lpDevMode.dmSize=sizeof(lpDevMode);
			lpDevMode.dmFields = DM_PELSWIDTH|DM_PELSHEIGHT|DM_BITSPERPEL;
			LONG result;
			result=ChangeDisplaySettings(&lpDevMode,0);
			if (result == DISP_CHANGE_SUCCESSFUL)
			{
				ChangeDisplaySettings(&lpDevMode,CDS_UPDATEREGISTRY);
				// ����//ʹ��CDS_UPDATEREGISTRY��ʾ���޸��ǳ־õģ�
				// ����//����ע�����д������ص����� 
			}
			else
			{
				ChangeDisplaySettings(NULL,0);
			}

			//֪ͨ����UI������Ӧ����
			this->m_pGameRoom->PostMessage(WM_SCREEN_SIZE_CHANGE,0,2);
		}
		
	}
}
/************************************************************************/
//Function:checkNetSignal()
//Parameter:none
//Author:Fred Huang 2008-06-27
//Note:�������������������Ϣ
/************************************************************************/
void CLoveSendClassInExe::checkNetSignal(void)
{
	if(m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID==0)        //û��ID�ţ�����
		return;
	//����ϴε���Ϣ��û���յ��������񼫲����Ϣ����Ҫ��
	if(m_NetSignalIndex>m_SavedSingalIndex)
	{
		m_NetSignalLevel=0;

	}
	drawSignal(&CWindowDC(this));
	//����ǲ���Ҫ����Ϣ��

}

/************************************************************************/
//Function:checkNetSignal()
//Parameter:
//                pNetData        :���������ص���Ϣ��
//Author:Fred Huang 2008-06-27
//Note:�������������ص���Ϣ
/************************************************************************/
void CLoveSendClassInExe::getNetSignal(void * pNetData)
{
	MSG_GP_Game_NETSignal *netSignal=(MSG_GP_Game_NETSignal*)pNetData;
	//������Ƿ����Լ��ģ�����
	if(m_pGameInfo->uisMeUserInfo.GameUserInfo.dwUserID!=netSignal->dwUserID)
		return;
	//�Ƚ�������0
	m_NetSignalLevel=0;
	//����ʱ��
	UINT uTimeDiff=GetTickCount()-netSignal->dwTimeFlag;

	m_NetSignalIndex = netSignal->dwSignalIndex;
	//ֻ���ǵ�ǰ����Ϣ�����ż��㼶��
	if(netSignal->dwIndex==m_NetSignalIndex)
	{
		if(uTimeDiff<=50)
			m_NetSignalLevel=1;
		else if(uTimeDiff>50 && uTimeDiff<=200)
			m_NetSignalLevel=2;
		else if(uTimeDiff>200 && uTimeDiff<=500)
			m_NetSignalLevel=3;
		else if(uTimeDiff>500 && uTimeDiff<=1000)
			m_NetSignalLevel=4;
		else                 
			m_NetSignalLevel=5;

	}
	//���浱ǰ�յ������
	m_SavedSingalIndex=netSignal->dwIndex;
	//��ͼ
	drawSignal(&CWindowDC(this));
	//Invalidate(true);
}

/************************************************************************/
//Function:drawSignal()
//Parameter:��
//Author:Fred Huang 2008-06-27
//Note:��ͼ
/************************************************************************/
void CLoveSendClassInExe::drawSignal(CDC * pDc)
{
	if (NULL == pDc || NULL == pDc->m_hDC )
		return;


	if(!pSingnalimg || pSingnalimg->GetFlags()==0)
		return;
	CString s="..\\";// CINIFile::GetAppPath();

	// ----- ����ָ����Ϸ�������ļ� -------- dxh
	char GameCode[20];
	::memset(GameCode,0,sizeof(GameCode));
	GetGameCode(GameCode);
	//if (str == "11901800"
	//	|| str == "10901800"
	//	|| str == "30501800")
	TCHAR szKey[10];
	CString sPath=CBcfFile::GetAppPath();
	CBcfFile fsr( sPath + "..\\SpecialRule.bcf");
	wsprintf(szKey, "%d", m_pGameInfo->uNameID);
	int iResult = fsr.GetKeyVal (_T("BJLType"), szKey, 0);

	if (iResult>0)
	{
		s = ".\\config\\";  ///<add by wxx 2010.7.1
	}
	//else
	//{
	//	s="..//config//"; ///<add by wxx 2010.07.14
	//	//return;
	//}
	// ----- ����ָ����Ϸ�������ļ� --------

	CBcfFile f( s + "GameOption.bcf");

	CString key= m_skinmgr.GetKeyVal(m_skinmgr.GetSkinBcfFileName());
	CRect rc;
	GetClientRect(rc);
	int x = rc.Width() - f.GetKeyVal(key, _T("game_signalx"), 230) - pSingnalimg->GetWidth();

	Graphics g(this->m_hWnd);
	g.DrawImage(pSingnalimg,Rect(x,f.GetKeyVal(key, _T("game_signaly"),3),pSingnalimg->GetWidth(),pSingnalimg->GetHeight()/6),0,m_NetSignalLevel*pSingnalimg->GetHeight()/6,pSingnalimg->GetWidth(),pSingnalimg->GetHeight()/6,UnitPixel);
}
void CLoveSendClassInExe::OnSndPlay()
{
	m_bSndPause=false;
	BZSoundContinue(m_sndListID);
	//m_btSndPlay.ShowWindow(SW_HIDE);
	//m_btSndPause.ShowWindow(SW_SHOW);

	TCHAR sz[MAX_PATH] = {0};
	TCHAR szPath[MAX_PATH] = {0};
	lstrcpy(sz,m_pGameInfo->szProcessName);
	sz[lstrlen(sz)-lstrlen(".ico")]='\0';
	wsprintf(szPath,"Music\\%s.bcf",sz,sz);
	CBcfFile f(szPath);
	f.SetKeyValString("BGSOUND","PauseSound","0");

}
void CLoveSendClassInExe::OnSndPause()
{
	m_bSndPause=true;
	BZSoundPause(m_sndListID);
	//m_btSndPlay.ShowWindow(SW_SHOW);
	//m_btSndPause.ShowWindow(SW_HIDE);

	TCHAR sz[MAX_PATH] = {0};
	TCHAR szPath[MAX_PATH] = {0};
	lstrcpy(sz,m_pGameInfo->szProcessName);
	sz[lstrlen(sz)-lstrlen(".ico")]='\0';
	wsprintf(szPath,"Music\\%s.bcf",sz,sz);
	CBcfFile f(szPath);
	f.SetKeyValString("BGSOUND","PauseSound","1");
}
void CLoveSendClassInExe::OnSndVolDown()
{
	int nVol=BZSoundGetVolume(m_sndListID);
	BZSoundSetVolume(m_sndListID,nVol-50);

	TCHAR sz[MAX_PATH] = {0};
	TCHAR szPath[MAX_PATH] = {0};
	lstrcpy(sz,m_pGameInfo->szProcessName);
	sz[lstrlen(sz)-lstrlen(".ico")]='\0';
	wsprintf(szPath,"Music\\%s.bcf",sz,sz);
	CBcfFile f(szPath);
	CString sini;
	sini.Format("%d",(nVol-50)>1?nVol-50:1);
	f.SetKeyValString("BGSOUND","SoundVolume",sini);

}
void CLoveSendClassInExe::OnSndVolUp()
{
	int nVol=BZSoundGetVolume(m_sndListID);
	BZSoundSetVolume(m_sndListID,nVol+50);

	TCHAR sz[MAX_PATH] = {0};
	TCHAR szPath[MAX_PATH] = {0};
	lstrcpy(sz,m_pGameInfo->szProcessName);
	sz[lstrlen(sz)-lstrlen(".ico")]='\0';
	wsprintf(szPath,"Music\\%s.bcf",sz,sz);
	CBcfFile f(szPath);
	CString sini;
	sini.Format("%d",(nVol+50)<1000?nVol+50:1000);
	f.SetKeyValString("BGSOUND","SoundVolume",sini);
}

// PengJiLin, 2010-6-1, ���� ������ �ӿ�
void CLoveSendClassInExe::UseLookCardProp(_TAG_USERPROP* userProp)
{
    return;
}

// PengJiLin, 2010-9-9, ���˿�����߿����ܽӿ�
void CLoveSendClassInExe::UseKickUserProp(_TAG_USERPROP* userProp)
{
    m_MessageHandle.InsertSystemMessage(TEXT("������ʹ�á����˿������ߡ�"));    // test

    MSG_GR_RS_KickProp KickPro;
    ZeroMemory(&KickPro,sizeof(MSG_GR_RS_KickProp));
    KickPro.dwUserID = GetMeUserInfo()->dwUserID;
    KickPro.iPropID = userProp->nPropID;

    SendGameData(&KickPro, sizeof(KickPro), MDM_GM_GAME_FRAME, ASS_GM_USE_KICK_PROP, 0);

    return;
}
void CLoveSendClassInExe::UseAntiKickProp(_TAG_USERPROP* userProp)
{
    m_MessageHandle.InsertSystemMessage(TEXT("������ʹ�á����߿������ߡ�"));    // test

    MSG_GR_RS_KickProp KickPro;
    ZeroMemory(&KickPro,sizeof(MSG_GR_RS_KickProp));
    KickPro.dwUserID = GetMeUserInfo()->dwUserID;
    KickPro.iPropID = userProp->nPropID;

    SendGameData(&KickPro, sizeof(KickPro), MDM_GM_GAME_FRAME, ASS_GM_USE_ANTI_KICK_PROP, 0);

    return;
}

// PengJiLin, 2010-9-10, ��������ʽ��Ϊʱ����ĸ�ʽ
void CLoveSendClassInExe::SecondFormat(int iSecond, CString& strOutput)
{
    if(iSecond < 0) iSecond = 0;

    int iSec = 0;
    int iMin = 0;
    int iHour = 0;

    iSec = iSecond%60;
    iMin = iSecond/60;

    iHour = iMin/60;
    iMin = iMin%60;

    if(iHour > 0)
    {
        strOutput.Format("%dСʱ%d��%d��", iHour, iMin, iSec);
    }
    else if(iMin > 0)
    {
        strOutput.Format("%d��%d��", iMin, iSec);
    }
    else
    {
        strOutput.Format("%d��", iSec);
    }
}

// PengJiLin, 2010-9-10, ���˿������߿�ʹ�ý��
bool CLoveSendClassInExe::OnUseKickPropResult(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize, BOOL bIsKick)
{
    MSG_GR_RS_KickProp* pKickProp = (MSG_GR_RS_KickProp*)pNetData;

    // ���ҵ��ߣ�ͬʱ���ٵ�������
    int propCount=Glb().userPropLibrary.GetCount();
    _TAG_USERPROP * userProp = NULL;
    int iPropValue = 0;
    for(int i = 0; i < propCount; ++i)
    {
        userProp = Glb().userPropLibrary.GetAt(i);
        if(userProp->nPropID == pKickProp->iPropID)
        {
            iPropValue = userProp->attribValue & 0xFFFFFF;
            userProp->nHoldCount -= 1;
            if(userProp->nHoldCount < 0)userProp->nHoldCount = 0;
            //if(userProp->nHoldCount < 1)  // PengJiLin, 2010-10-14, �µĵ���ϵͳ��������Ϊ0������
            //{
            //    Glb().userPropLibrary.RemoveAt(i);
            //    delete userProp;
            //    userProp=NULL;
            //}
            break;
        }
        userProp = NULL;
    }

    if(TRUE == bIsKick)
    {
        CString strValue = "";
        SecondFormat(iPropValue, strValue);

        CString strTotalValue = "";
        SecondFormat(pKickProp->iTotalTime, strTotalValue);

        CString strInfo = ""; 
        strInfo.Format("���ѳɹ�ʹ�á����˿������ߣ�����������Ϸ�Ҳ��б����Ҽ����Ŀ����ҽ����߳���Ϸ�����ߵ�����Чʱ��%s����ʱ��%s", 
                        strValue, strTotalValue);

		m_MessageHandle.InsertSystemMessage(strInfo.GetBuffer());

        //ShowNewKickPropDlg(TRUE, FALSE, TRUE, strInfo);
    }
    else
    {
        CString strValue = "";
        SecondFormat(iPropValue, strValue);

        CString strTotalValue = "";
        SecondFormat(pKickProp->iTotalTime, strTotalValue);

        CString strInfo = ""; 
        strInfo.Format("���ѳɹ�ʹ�á����߿������ߣ���������޷������߳���Ϸ�����ߵ�����Чʱ��%s����ʱ��%s", 
                        strValue, strTotalValue);

		m_MessageHandle.InsertSystemMessage(strInfo.GetBuffer());

        //ShowNewKickPropDlg(TRUE, FALSE, FALSE, strInfo);
    }

    // ˢ��
    OnSetPropImages();

    return true;
}

// PengJiLin, 2010-9-14, ���˿������߿���ʾ����ʾ
void CLoveSendClassInExe::ShowNewKickPropDlg(BOOL bUseForOther, BOOL bUseLink, 
                                             BOOL bIsTOut, CString& strInfo)
{
    //�Ƚ�������
    AfxSetResourceHandle(GetModuleHandle(FACE_DLL_NAME));
    
    if(!m_dlgKickProp)
    {
        m_dlgKickProp=new CBoardCast();
        m_dlgKickProp->FuncForProp(bUseForOther, bUseLink, bIsTOut);
        m_dlgKickProp->SetPropInfo(strInfo);
        m_dlgKickProp->Create(IDD_BOARDCAST,GetDesktopWindow());
        m_dlgKickProp->pParentWnd=(CWnd*)this;
    }

    m_dlgKickProp->FuncForProp(bUseForOther, bUseLink, bIsTOut);
    m_dlgKickProp->SetPropInfo(strInfo);

    AfxSetResourceHandle(GetModuleHandle(NULL));
    m_dlgKickProp->ShowWindow(SW_SHOW);
    return;
}


// PengJiLin, 2011-3-31, ����ʱ���������ͽ�ҹ��ܣ���ȡ��ʽ����ʾ��Ϣ
void CLoveSendClassInExe::GetSendMoneyFormatInfo(CString& strInfo)
{
    CString strTimeInfo = "";
    CString strCountInfo = "";
    strInfo = "";

	CBcfFile fMsg(CBcfFile::GetAppPath()+"..\\ClientMessage.bcf");

    // ����֧��    
    char tempStr0[100] = {0};
    GlbGetNumString(tempStr0, m_SendMoneyData.dwMoneyOnTimes, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
    char tempStr1[100] = {0};
    GlbGetNumString(tempStr1, m_SendMoneyData.dwMoneyOnCounts, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);

    if(m_SendMoneyData.dwMoneyOnTimes > 0 && m_SendMoneyData.dwTimesNeed > 0)
    {
        UINT uSec = 0;
        UINT uMin = 0;
        uMin = m_SendMoneyData.dwTimesNeed / 60;
        uSec = m_SendMoneyData.dwTimesNeed % 60;
        
		strTimeInfo = fMsg.GetKeyVal("GameFrame","SendMoney_TimeInfo","����%d����%d��, �Ϳɻ���%s���");
		strTimeInfo.Format(strTimeInfo, uMin, uSec, tempStr0);

    }

    if(m_SendMoneyData.dwMoneyOnCounts > 0 && m_SendMoneyData.dwCountsNeed > 0)
    {
		strCountInfo = fMsg.GetKeyVal("GameFrame","SendMoney_CountInfo","����%d��, �Ϳɻ���%s���");
		strCountInfo.Format(strTimeInfo, m_SendMoneyData.dwCountsNeed, tempStr1);
    }

    strInfo += strTimeInfo;
    strInfo += strCountInfo;
}

// PengJiLin, 2011-4-19, ����ʱ���������ͽ��ͼ����ʾ
void CLoveSendClassInExe::ShowGetMoneyImage()
{
    return;
}
// PengJiLin, 2011-5-10, ����ʱ���������ͽ�ҹ��ܣ���ȡ��ʽ����ʾ��Ϣ
void CLoveSendClassInExe::GetSendMoneyFormatInfo(CString& strTime, CString& strCount)
{
    CString strTimeInfo = "";
    CString strCountInfo = "";
    strTime = "";
    strCount = "";

	CBcfFile fMsg(CBcfFile::GetAppPath()+"..\\ClientMessage.bcf");

    // С����֧��
    char tempStr0[100] = {0};
    GlbGetNumString(tempStr0, m_SendMoneyData.dwMoneyOnTimes, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);
    char tempStr1[100] = {0};
    GlbGetNumString(tempStr1, m_SendMoneyData.dwMoneyOnCounts, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);

    if(m_SendMoneyData.dwMoneyOnTimes > 0 && m_SendMoneyData.dwTimesNeed > 0)
    {
        UINT uSec = 0;
        UINT uMin = 0;
        uMin = m_SendMoneyData.dwTimesNeed / 60;
        uSec = m_SendMoneyData.dwTimesNeed % 60;

		strTimeInfo = fMsg.GetKeyVal("GameFrame","SendMoney_TimeInfo","����%d����%d��, �Ϳɻ���%s���");
		strTimeInfo.Format(strTimeInfo, uMin, uSec, tempStr0);
    }

    if(m_SendMoneyData.dwMoneyOnCounts > 0 && m_SendMoneyData.dwCountsNeed > 0)
    {
		strCountInfo = fMsg.GetKeyVal("GameFrame","SendMoney_CountInfo","����%d��, �Ϳɻ���%s���");
		strCountInfo.Format(strTimeInfo, m_SendMoneyData.dwCountsNeed, tempStr1);
    }

    if(strTimeInfo.GetLength() < 1 && strCountInfo.GetLength() > 1)
    {
        strCountInfo += ", ����Ŭ����!";
    }
    if(strCountInfo.GetLength() < 1 && strTimeInfo.GetLength() > 1)
    {
        strTimeInfo += ", ����Ŭ����!";
    }
    if(strTimeInfo.GetLength() > 1 && strCountInfo.GetLength() > 1)
    {
        strCountInfo += ", ����Ŭ����!";
    }

    strTime = strTimeInfo;
    strCount = strCountInfo;
}

// PengJiLin, 2011-5-10, ����ʱ���������ͽ�ҹ��ܣ������ʹ�õĸ�ʽ����Ϣ
void CLoveSendClassInExe::GetSendMoneyFormatInfo(CString& strGet, CString& strTime, CString& strCount)
{
    // С����֧��
    TCHAR tempStr[100] = {0};
    GlbGetNumString(tempStr, m_SendMoneyData.dwGetMoney, Glb().m_nPowerOfGold, Glb().m_bUseSpace, Glb().m_strSpaceChar);

	CString strMessage;
	CBcfFile fMsg(CBcfFile::GetAppPath()+"..\\ClientMessage.bcf");
	strMessage = fMsg.GetKeyVal("GameFrame","SendMoney"," �����ϵͳ���� %s ���");
	strMessage.Format(strMessage,tempStr);

    strGet = "";
    if(m_SendMoneyData.dwGetMoney > 0) 
    {
        strGet = strMessage;
    }

    GetSendMoneyFormatInfo(strTime, strCount);
}

UserItemStruct* CLoveSendClassInExe::GetUserItemInfo(int iIdx)
{
	return m_pUserInfo[iIdx];
}

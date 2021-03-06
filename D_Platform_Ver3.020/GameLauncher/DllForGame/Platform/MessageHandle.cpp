#include "../stdafx.h"
#include "MessageHandle.h"
#include "AFCExpressionWnd.h"
#include "afcfunction.h"
#include "RightFrame.h"

//基础高度
#define BASE_HEIGHT						180
#define NORMAL_FONT						TEXT("宋体")

//构造函数
CLookingClass::CLookingClass(CNewMiniDlg * pRichEdit)
{
	m_pRichEdit=pRichEdit;
	//====用所有的初始颜色
	Color_Struct.m_crDeskTxColor=CGameImageLink::m_ColorResource.m_crSysHeadTxColor;
	Color_Struct.m_crListBkColor=CGameImageLink::m_ColorResource.m_crListBkColor;
	Color_Struct.m_crListTxColor=CGameImageLink::m_ColorResource.m_crListTxColor;

	Color_Struct.m_crNormalTxColor=CGameImageLink::m_ColorResource.m_crNormalTxColor;
	Color_Struct.m_crSysHeadTxColor=CGameImageLink::m_ColorResource.m_crSysHeadTxColor;
	Color_Struct.m_crSystemTxColor=CGameImageLink::m_ColorResource.m_crSystemTxColor;
	Color_Struct.m_crSystemBdColor=CGameImageLink::m_ColorResource.m_crSystemBdColor;

	Color_Struct.m_crTalkBkColor=CGameImageLink::m_ColorResource.m_crTalkBkColor;
	Color_Struct.m_crTalkNameColor=CGameImageLink::m_ColorResource.m_crTalkNameColor;
	Color_Struct.m_crTalkTxColor=CGameImageLink::m_ColorResource.m_crTalkTxColor;
	Color_Struct.m_crNewsTxColor=CGameImageLink::m_ColorResource.m_crNewsTxColor;

	Color_Struct.m_crSentTxColor=CGameImageLink::m_ColorResource.m_crSentTxColor;
	Color_Struct.m_crReceivedTxColor=CGameImageLink::m_ColorResource.m_crReceivedTxColor;

	m_pRightFrame = NULL;
}

void CLookingClass::SetColor(CAFCColorResource Col_Str)
{
	Color_Struct.m_crDeskTxColor=Col_Str.m_crDeskTxColor;
	Color_Struct.m_crListBkColor=Col_Str.m_crListBkColor;
	Color_Struct.m_crListTxColor=Col_Str.m_crListTxColor;

	Color_Struct.m_crNormalTxColor=Col_Str.m_crNormalTxColor;
	Color_Struct.m_crSysHeadTxColor=Col_Str.m_crSysHeadTxColor;
	Color_Struct.m_crSystemTxColor=Col_Str.m_crSystemTxColor;
	Color_Struct.m_crSystemBdColor=Col_Str.m_crSystemBdColor;

	Color_Struct.m_crTalkBkColor=Col_Str.m_crTalkBkColor;
	Color_Struct.m_crTalkNameColor=Col_Str.m_crTalkNameColor;
	Color_Struct.m_crTalkTxColor=Col_Str.m_crTalkTxColor;
	Color_Struct.m_crNewsTxColor=Col_Str.m_crNewsTxColor;

	Color_Struct.m_crSentTxColor=Col_Str.m_crSentTxColor;
	Color_Struct.m_crReceivedTxColor=Col_Str.m_crReceivedTxColor;
}

//切换RichEdit,Added by JianGuankun 2012.4.28
void CLookingClass::SwitchToDuiVersion(CRightFrame* pRightFrame)
{
	m_pRightFrame = pRightFrame;
}

//析构函数
CLookingClass::~CLookingClass(void)
{
}

//新闻信息
void CLookingClass::InsertNews(TCHAR * szCharString, UINT uSize, TCHAR * szFontName)
{
	//插入消息
	m_pRichEdit->InsertCharString(TEXT("【新闻】"),Color_Struct.m_crNewsTxColor,false,uSize,szFontName);

	m_pRichEdit->InsertCharString(szCharString,Color_Struct.m_crNewsTxColor,true,uSize,szFontName);
	m_pRichEdit->InsertCharString(TEXT("\n"),Color_Struct.m_crNewsTxColor);

	char szGBuffer[512] = {0};
	char szTempBuffer[256] = {0};

	::strcat(szGBuffer,"<p 4>");

	szTempBuffer[0] = '\0';
	_stprintf(szTempBuffer,"<c %08x>【新闻】</c><c %08x>%s</c>",0x00000000,0x00000000,szCharString);
	::strcat(szGBuffer,szTempBuffer);

	::strcat(szGBuffer,"</p>");

	if (m_pRightFrame)
	{
		m_pRightFrame->InsertTalkMessage(szGBuffer);
	}


	return;
}
void CLookingClass::InsertNews(CString szCharString, UINT uSize, TCHAR * szFontName)
{
	//插入消息
	TCHAR  sz[1000];
	::lstrcpy(sz,szCharString);
	InsertNews(sz, uSize, szFontName);
	return;
}

//聊天记录中显示的用户名字〖 哈哈哈哈哈哈 〗 说： 
void CLookingClass::InsertUserName(TCHAR * szUserName)
{
	//m_pRichEcit->InsertCharString(TEXT(""),Color_Struct.m_crTalkNameColor);
	m_pRichEdit->InsertCharString(szUserName,Color_Struct.m_crTalkNameColor);//,true);
	//m_pRichEcit->InsertCharString(TEXT(""),Color_Struct.m_crTalkNameColor);
	return;
}

//特殊用户名字
void CLookingClass::InsertSuperUserName(TCHAR * szUserName)
{
	int nt=rand()%10;
	if(nt==0)
		m_pRichEdit->InsertCharString(szUserName,RGB(156,251,123));//,true);
	if(nt==1)
		m_pRichEdit->InsertCharString(szUserName,RGB(206,239,123));//,true);
	if(nt==2)
		m_pRichEdit->InsertCharString(szUserName,RGB(237,115,247));//,true);
	if(nt==3)
		m_pRichEdit->InsertCharString(szUserName,RGB(64,0,0));//,true);
	if(nt==4)
		m_pRichEdit->InsertCharString(szUserName,RGB(249,145,30));//,true);
	if(nt==5)
		m_pRichEdit->InsertCharString(szUserName,RGB(128,255,30));//,true);
	if(nt==6)
		m_pRichEdit->InsertCharString(szUserName,RGB(30,245,186));//,true);
	if(nt==7)
		m_pRichEdit->InsertCharString(szUserName,RGB(221,221,15));//,true);
	if(nt==8)
		m_pRichEdit->InsertCharString(szUserName,RGB(47,47,186));//,true);
	if(nt==9)
		m_pRichEdit->InsertCharString(szUserName,RGB(179,14,230));//,true);
	return;
}
//用户聊天信息
void CLookingClass::InsertUserTalk(TCHAR * szSendName, TCHAR * szTargetName,TCHAR * szCharString, COLORREF crTextColor, bool bShowTime, UINT uSize, TCHAR * szFontName)
{
	char szGBuffer[512] = {0};
	char szTempBuffer[256] = {0};

	::strcat(szGBuffer,"<p 4>");

	//插入时间
	if (bShowTime)
	{
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);
		TCHAR szBuffer[20];
		memset(szBuffer,0,sizeof(szBuffer));
		wsprintf(szBuffer,TEXT("(%02d:%02d:%02d )"),SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);
		m_pRichEdit->InsertCharString(szBuffer,Color_Struct.m_crTalkNameColor);
		m_pRichEdit->InsertCharString(TEXT("\n"),RGB(0,0,0));

		szTempBuffer[0] = '\0';
		_stprintf(szTempBuffer,"<c #%06f>(%02d:%02d:%02d)\n</c>",Color_Struct.m_crTalkNameColor,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);
		::strcat(szGBuffer,szTempBuffer);

	}

	InsertUserName(szSendName);
	::strcat(szGBuffer,szSendName);


	if (szTargetName!=NULL)
	{
		m_pRichEdit->InsertCharString(TEXT("对"),Color_Struct.m_crTalkTxColor);
		InsertUserName(szTargetName);

		szTempBuffer[0] = '\0';
		_stprintf(szTempBuffer,"<c #%06f>对%s</c>",Color_Struct.m_crTalkTxColor,szTargetName);
		::strcat(szGBuffer,szTempBuffer);
	}
	////////////////////////////
	////Kylin 20090117 说和用户名，保持一样风格
	InsertUserName(TEXT("说："));
	::strcat(szGBuffer,"说：");
	////////////////////////////
	//m_pRichEcit->InsertCharString(TEXT("说："),Color_Struct.m_crNormalTxColor);
	TCHAR buf[MAX_TALK_LEN+2];
	lstrcpy(buf,szCharString);

	//插入消息
	TranslateInsertChar(szCharString,crTextColor,uSize,szFontName);
	m_pRichEdit->InsertCharString(TEXT("\n"),RGB(0,0,0));

	szTempBuffer[0] = '\0';
	_stprintf(szTempBuffer,"<c %08x>%s</c>",0x000000FF,buf);
	::strcat(szGBuffer,szTempBuffer);

	::strcat(szGBuffer,"</p>");

	if (m_pRightFrame)
	{
		m_pRightFrame->InsertTalkMessage(szGBuffer);
	}

	return;
}
//用户聊天信息
void CLookingClass::InsertDudu(TCHAR * szSendName, TCHAR * szTargetName,TCHAR * szCharString, COLORREF crTextColor, bool bShowTime, UINT uSize, TCHAR * szFontName,int iDuduType)
{

	char szGBuffer[1024] = {0};
	char szTempBuffer[512] = {0};

	::strcat(szGBuffer,"<p 4>");

	//插入时间
	if (bShowTime)
	{
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);
		TCHAR szBuffer[20];
		memset(szBuffer,0,sizeof(szBuffer));
		wsprintf(szBuffer,TEXT("(%02d:%02d:%02d )"),SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);
		m_pRichEdit->InsertCharString(szBuffer,Color_Struct.m_crTalkNameColor);
		m_pRichEdit->InsertCharString(TEXT("\n"),RGB(0,0,0));

		szTempBuffer[0] = '\0';
		_stprintf(szTempBuffer,"<c #%06f>(%02d:%02d:%02d)\n</c>",Color_Struct.m_crTalkNameColor,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);
		::strcat(szGBuffer,szTempBuffer);
	}

	InsertUserName(szSendName);
	::strcat(szGBuffer,szSendName);

	if (szTargetName!=NULL)
	{
		if(iDuduType==BRD_MSG_BIG)
		{
			//m_pRichEdit->InsertCharString(TEXT("拿着大喇叭喊"),Color_Struct.m_crTalkTxColor,true);
			szTempBuffer[0] = '\0';
			_stprintf(szTempBuffer,"<c %08x>拿着大喇叭喊</c>",0x000000FF);
			::strcat(szGBuffer,szTempBuffer);
		}			
		else
		{
			//m_pRichEdit->InsertCharString(TEXT("拿着小喇叭喊"),Color_Struct.m_crTalkTxColor,true);
			szTempBuffer[0] = '\0';
			_stprintf(szTempBuffer,"<c %08x>拿着小喇叭喊</c>",0x000000FF);
			::strcat(szGBuffer,szTempBuffer);
		}
			
		InsertUserName(szTargetName);
		::strcat(szGBuffer,szTargetName);
	}
	//m_pRichEcit->InsertCharString(TEXT("说:"),Color_Struct.m_crNormalTxColor);

	//插入消息
// 	TranslateInsertChar(szCharString,crTextColor,uSize,szFontName);
// 	m_pRichEdit->InsertCharString(TEXT("\n"),RGB(0,0,0),true);

	szTempBuffer[0] = '\0';
	_stprintf(szTempBuffer,"<c %08x>%s</c>",0x00FF0000,szCharString);
	::strcat(szGBuffer,szTempBuffer);

	::strcat(szGBuffer,"</p>");

	if (m_pRightFrame)
	{
		m_pRightFrame->InsertTalkMessage(szGBuffer);
	}

	return;
}


//系统消息
void CLookingClass::InsertSystemMessage(TCHAR * szCharString, UINT uSize, TCHAR * szFontName)
{
	// add xqm 2010-11-19
	AFCPlaySound(GetModuleHandle(FACE_DLL_NAME), TEXT("SYSTEM_MSG"));	

	//插入消息
// 	m_pRichEcit->InsertCharString(TEXT("【系统消息】"),Color_Struct.m_crSystemTxColor,false,uSize,szFontName,true);
// 	TranslateInsertChar(szCharString,Color_Struct.m_crSystemTxColor,uSize,szFontName);
// 	m_pRichEcit->InsertCharString(TEXT("\n"),Color_Struct.m_crSystemTxColor,false);	

	CString skinfolder;
	TCHAR path[MAX_PATH];
	CBcfFile f(CBcfFile::GetAppPath () + "skin.bcf");
	skinfolder = f.GetKeyVal(Glb().m_skin,"skinfolder","image\\client\\skin0\\");
	CGameImage bmp;
	wsprintf(path,"%s..\\%sdialog\\xtxx.bmp",CBcfFile::GetAppPath (), skinfolder);
	bmp.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_pRichEdit->InsertBitmap((HBITMAP)bmp);
	m_pRichEdit->InsertCharString(TEXT(" "),Color_Struct.m_crSystemTxColor);
	TranslateInsertChar(szCharString,Color_Struct.m_crSystemTxColor,uSize,szFontName);
	m_pRichEdit->InsertCharString(TEXT("\n"),Color_Struct.m_crTalkNameColor);

	char szGBuffer[512] = {0};
	char szTempBuffer[256] = {0};

	::strcat(szGBuffer,"<p 6>");

	szTempBuffer[0] = '\0';
	_stprintf(szTempBuffer,"<i dialog\\xtxx.png 0> <c %08x>%s</c>",0x00FF0000,szCharString);
	::strcat(szGBuffer,szTempBuffer);

	::strcat(szGBuffer,"</p>");

	if (m_pRightFrame)
	{
		m_pRightFrame->InsertTalkMessage(szGBuffer);
	}

	return;
}

void CLookingClass::InsertSystemMessageM(TCHAR * szCharString, UINT uSize, TCHAR * szFontName)
{
	CString skinfolder;
	TCHAR path[MAX_PATH];
	CBcfFile f(CBcfFile::GetAppPath () + "skin.bcf");
	skinfolder = f.GetKeyVal(Glb().m_skin,"skinfolder","image\\client\\skin0\\");
	CGameImage bmp;
	wsprintf(path,"%s..\\%sdialog\\xtxx.bmp",CBcfFile::GetAppPath (), skinfolder);
	bmp.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_pRichEdit->InsertBitmap((HBITMAP)bmp);
	m_pRichEdit->InsertCharString(TEXT(" "),Color_Struct.m_crSystemTxColor);
	TranslateInsertChar(szCharString,Color_Struct.m_crSystemTxColor,uSize,szFontName);
	m_pRichEdit->InsertCharString(TEXT("\n"),Color_Struct.m_crTalkNameColor);

	char szGBuffer[512] = {0};
	char szTempBuffer[256] = {0};

	::strcat(szGBuffer,"<p 6>");

	szTempBuffer[0] = '\0';
	_stprintf(szTempBuffer,"<i dialog\\xtxx.png 0> <c %08x>%s</c>",0x00FF0000,szCharString);
	::strcat(szGBuffer,szTempBuffer);

	::strcat(szGBuffer,"</p>");

	if (m_pRightFrame)
	{
		m_pRightFrame->InsertTalkMessage(szGBuffer);
	}

	return;
}

//系统公告
void CLookingClass::InsertSystemBoard(TCHAR * szCharString, UINT uSize, TCHAR * szFontName)
{
	//插入消息
	CString skinfolder;
	TCHAR path[MAX_PATH];
	CBcfFile f(CBcfFile::GetAppPath () + "skin.bcf");
	skinfolder = f.GetKeyVal(Glb().m_skin,"skinfolder","image\\client\\skin0\\");
	CGameImage bmp;
	wsprintf(path,"%sdialog\\xigk.bmp",skinfolder);
	bmp.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_pRichEdit->InsertBitmap((HBITMAP)bmp);
	m_pRichEdit->InsertCharString(TEXT(" "),Color_Struct.m_crSystemBdColor);
	TranslateInsertChar(szCharString,Color_Struct.m_crSystemBdColor,uSize,szFontName);
	m_pRichEdit->InsertCharString(TEXT("\n"),Color_Struct.m_crTalkNameColor);

	char szGBuffer[512] = {0};
	char szTempBuffer[256] = {0};

	::strcat(szGBuffer,"<p 6>");

	szTempBuffer[0] = '\0';
	_stprintf(szTempBuffer,"<i dialog\\xigk.png 0> <c %08x>%s</c>",0x000000FF,szCharString);
	::strcat(szGBuffer,szTempBuffer);

	::strcat(szGBuffer,"</p>");

	if (m_pRightFrame)
	{
		m_pRightFrame->InsertTalkMessage(szGBuffer);
	}

	// add xqm 2010-11-19
//	AFCPlaySound(GetModuleHandle(FACE_DLL_NAME),TEXT("SYSTEM_MSG"));
	return;
}

//自定义头消息
void CLookingClass::InsertTitleMessage(TCHAR * szMsg, BYTE bNewOrSys,UINT uSize, TCHAR * szFontName,TCHAR *Title)
{
	//插入消息
	if(bNewOrSys==0)//系统消息
	{
		if(uSize==0)
			InsertSystemMessage(szMsg);
		else
			InsertSystemMessage(szMsg,uSize,szFontName);
	}
	else if(bNewOrSys==1) //新闻
	{
		if(uSize==0)
			InsertNews(szMsg);
		else
			InsertNews(szMsg,uSize,szFontName);
	}
	else if(bNewOrSys==2)//小喇叭
	{
		//插入消息
		m_pRichEdit->InsertCharString(Title,Color_Struct.m_crSystemTxColor,false,uSize,szFontName);
		TranslateInsertChar(szMsg,Color_Struct.m_crSystemTxColor,uSize,szFontName);
		m_pRichEdit->InsertCharString(TEXT("\n"),Color_Struct.m_crSystemTxColor);
	}
	else if (bNewOrSys == 3)
	{
		if(uSize==0)
			InsertSystemMessageM(szMsg);
		else
			InsertSystemMessageM(szMsg,uSize,szFontName);
	}
	return;
}

//简单信息
void CLookingClass::InsertNormalMessage(TCHAR * szCharString, bool bEnter, UINT uSize, TCHAR * szFontName)
{
	TranslateInsertChar(szCharString,Color_Struct.m_crNormalTxColor,uSize,szFontName);
	if (bEnter==true) 
		m_pRichEdit->InsertCharString(TEXT("\n"),RGB(0,0,0));

	char szGBuffer[512] = {0};
	char szTempBuffer[256] = {0};

	::strcat(szGBuffer,"<p 4>");

	szTempBuffer[0] = '\0';
	_stprintf(szTempBuffer,"<c %08x>%s</c>",0x00000000,szCharString);
	::strcat(szGBuffer,szTempBuffer);

	::strcat(szGBuffer,"</p>");

	if (m_pRightFrame)
	{
		m_pRightFrame->InsertTalkMessage(szGBuffer);
	}


	return;
}

//解释插入
bool CLookingClass::TranslateInsertChar(TCHAR * szCharString, COLORREF crTextColor, UINT uSize, TCHAR * szFontName)
{
	CTrueItem * pItem=NULL;
	int iCharLength=lstrlen(szCharString),iHandleLength=0;
	do
	{
		int iPassCharCount=0;
		pItem=TranslateChar(szCharString+iHandleLength,iCharLength-iHandleLength,iPassCharCount);
		if (pItem!=NULL)
		{
			szCharString[iHandleLength+iPassCharCount]=0;
			m_pRichEdit->InsertCharString(szCharString+iHandleLength,crTextColor,false,uSize,szFontName);
			iHandleLength+=iPassCharCount+pItem->m_uTranceLength;
			m_pRichEdit->InsertBitmap(pItem->m_ExpPicImage);
		}
		else 
		{
			m_pRichEdit->InsertCharString(szCharString+iHandleLength,crTextColor,false,uSize,szFontName);
			break;
		}
	} while (pItem!=NULL);
	return true;
}

//解释表情字符
CTrueItem * CLookingClass::TranslateChar(TCHAR * szCharString, int iCharLength, int & iPassCharCount)
{
	CTrueItem * pItem=NULL;
	int iItemLength=0,iInputLenth=0;
	for (int i=0;i<iCharLength-2;i++)
	{
		if ((szCharString[i]==TEXT('/'))&&(szCharString[i+1]==TEXT(':')))
		{
			for (int j=0;j<MAX_EXPRESS_ITEM;j++)
			{
				pItem=&CGameTalkText::m_ExpreesionItem[j];
				iInputLenth=iCharLength-i;
				iItemLength=pItem->m_uTranceLength;
				if (iItemLength>iInputLenth) return NULL;
				int k=2;
				for (k=2;k<iItemLength;k++)
				{
					if ((szCharString[i+k]!=pItem->m_szTrancelate[k])&&(szCharString[i+k]!=(pItem->m_szTrancelate[k]+32))) break;
				}
				if (k==iItemLength) 
				{
					iPassCharCount=i;
					return pItem;
				}
			}
			i++;
		}
	}
	return NULL;
}

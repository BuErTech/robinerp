#include"stdafx.h"
#include "playcardsound.h"
#include "../GameMessage/upgradelogic.h"
#pragma comment(lib,"vfw32.lib")

CUpGradeGameLogic		m_Logic;					//�����߼�

//����Mp3����
HWND PlayCardMusic(TCHAR FileName[],HWND hParent)
{	
	HWND hmciwnd;
	//hmciwnd = MCIWndCreate(this->m_hWnd,AfxGetInstanceHandle(),WS_CHILD,FileName);
	hmciwnd=MCIWndCreate(hParent,AfxGetInstanceHandle(),WS_CHILD|MCIWNDF_NOMENU|MCIWNDF_NOOPEN|MCIWNDF_NOPLAYBAR|MCIWNDF_NOERRORDLG,FileName);
	if(hmciwnd)
		MCIWndPlay(hmciwnd);	
	return hmciwnd;
}
/*
���ų�������
parameter iSenceSound ��������
parameter isBoy		�Ա�
parameter hParent    ������
parameter iVal		����ָ����������(δ���ô�ֵ,�������)
*/
//���ų�������
HWND SetSceneSound(BYTE iSceneSound,HWND hParent,BOOL IsBoy,int iVal ,int iDialectType )
{
	TCHAR str[100]="";
	TCHAR filename[MAX_PATH];//path[MAX_PATH];
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "cardsound.bcf");
	CString key = TEXT("cardscene");
	//	TCHAR path[MAX_PATH];
	CString folder,extendname,name;

	if(iDialectType == 1)
	{
		folder=f.GetKeyVal(key,"folder","music\\shipo\\cardwav\\scene\\");
	}
	else if(iDialectType == 2)
	{
		folder=f.GetKeyVal(key,"folder","music\\zhongxin\\cardwav\\scene\\");
	}
	else if(iDialectType == 3)
	{
		folder=f.GetKeyVal(key,"folder","music\\pucheng\\cardwav\\scene\\");
	}
	else
	{
		folder=f.GetKeyVal(key,"folder","music\\cardwav\\scene\\");
	}
	
	extendname=f.GetKeyVal(key,"extendname",".mp3");

	//	TCHAR filename[MAX_PATH],str[100]="";
	srand(GetCurrentTime());
	int count;
	switch(iSceneSound)
	{
	case SOUND_WIN://ʤ��
		if(IsBoy)
		{	
			count = f.GetKeyVal(key,"bwincount",10);
			if(count == 0)
				return NULL;
			wsprintf(str,"b(win%d)",rand()%count);
		}
		else
		{
			count = f.GetKeyVal(key,"gwincount",10);
			if(count == 0)
				return NULL;
			wsprintf(str,"g(win%d)",rand()%count);
		}
		break;
	case SOUND_LOSE://ʧ��
		if(IsBoy)
		{
			count = f.GetKeyVal(key,"blosecount",10);
			if(count == 0)
				return NULL;
			wsprintf(str,"b(lose%d)",rand()%count);
		}
		else
		{
			count = f.GetKeyVal(key,"glosecount",10);
			if(count == 0)
				return NULL;
			wsprintf(str,"g(lose%d)",rand()%count);
		}
		break;
	case SOUND_WAITING://�ȴ�
		if(IsBoy)
		{
			count = f.GetKeyVal(key,"bwaitcount",10);
			if(count == 0)
				return NULL;
			wsprintf(str,"b(wait%d)",rand()%count);
		}
		else
		{
			count = f.GetKeyVal(key,"gwaitcount",10);
			if(count == 0)
				return NULL;
			wsprintf(str,"g(wait%d)",rand()%count);
		}
		break;
	case SOUND_ERROR://���Ƴ�����
		if(IsBoy)
		{
			count = f.GetKeyVal(key,"berrorcount",10);
			if(count == 0)
				return NULL;
			wsprintf(str,"b(error%d)",rand()%count);
		}
		else
		{
			count = f.GetKeyVal(key,"gerrorcount",10);
			if(count == 0)
				return NULL;
			wsprintf(str,"g(error%d)",rand()%count);
		}
		break;
	case SOUND_READY://����׼��
		if(IsBoy)
		{
			count = f.GetKeyVal(key,"breadycount",10);
			if(count == 0)
				return NULL;
			wsprintf(str,"b(ready%d)",rand()%count);
		}
		else
		{
			count = f.GetKeyVal(key,"greadycount",10);
			if(count == 0)
				return NULL;
			wsprintf(str,"g(ready%d)",rand()%count);
		}
		break;
	case SOUND_START://�ļ�׼����ʼ
		{
			if(IsBoy)
			{
				count = f.GetKeyVal(key,"bstartcount",10);
				if(count == 0)
					return NULL;
				wsprintf(str,"b(start%d)",rand()%count);
			}
			else
			{
				count = f.GetKeyVal(key,"gstartcount",10);
				if(count == 0)
					return NULL;
				wsprintf(str,"g(start%d)",rand()%count);
			}
			break;
		}
	case SOUND_START_OUT_CARD://��ʼ��Ϸ(���)
		if(IsBoy)
		{
			count = f.GetKeyVal(key,"bstartoutcardcount",10);
			if(count == 0)
				return NULL;
			wsprintf(str,"b(startoutcard%d)",rand()%count);
		}
		else
		{
			count = f.GetKeyVal(key,"gstartcount",10);
			if(count == 0)
				return NULL;
			wsprintf(str,"g(start%d)",rand()%count);
		}
		break;
	case SOUND_CALL_SCORE://�з�
		{
			if(IsBoy)
			{
				wsprintf(str,"b(jiaofen%d)",iVal);
			}
			else
			{
				wsprintf(str,"g(jiaofen%d)",iVal);
			}
			/*if ( iVal == 3 )
			{
				extendname=f.GetKeyVal(key,"extendname",".wav");
				name=f.GetKeyVal(key,str,str);
				wsprintf(filename,"%s%s%s",folder,name,extendname);
				return 	PlayCardMusic(filename,hParent);

			}*/
			break;
		}
	case SOUND_OUT_CARD_FINISH://������
		{
			if(IsBoy)
			{
				count = f.GetKeyVal(key,"boutcardfinishcount",10);
				if(count == 0)
					return NULL;
				wsprintf(str,"b(outcardfinish%d)",rand()%count);
			}
			else
			{
				count = f.GetKeyVal(key,"goutcardfinishcount",10);
				if(count == 0)
					return NULL;
				wsprintf(str,"g(outcardfinish%d)",rand()%count);
			}
			break;
		}
	case SOUND_FRIEND_APPEAR://���ѳ��F
		{
			if(IsBoy)
			{
				count = f.GetKeyVal(key,"bfriendcount",10);
				if(count == 0)
					return NULL;
				wsprintf(str,"b(friend%d)",rand()%count);
			}
			else
			{
				count = f.GetKeyVal(key,"gfriendcount",10);
				if(count == 0)
					return NULL;
				wsprintf(str,"g(friend%d)",rand()%count);
			}
			break;
		}
	case SOUND_USER_RUNAWARY:	//����
		{
			if(IsBoy)
			{
				count = f.GetKeyVal(key,"brunawarycount",10);
				if(count == 0)
					return NULL;
				wsprintf(str,"b(runawary%d)",rand()%count);
			}
			else
			{
				count = f.GetKeyVal(key,"grunawarycount",10);
				if(count == 0)
					return NULL;
				wsprintf(str,"g(runawary%d)",rand()%count);
			}
			break;
		}
	case SOUND_USER_LEFT:	//�����뿪
		{
			if(IsBoy)
			{
				count = f.GetKeyVal(key,"bleftcount",10);
				if(count == 0)
					return NULL;
				wsprintf(str,"b(left%d)",rand()%count);
			}
			else
			{
				count = f.GetKeyVal(key,"gleftcount",10);
				if(count == 0)
					return NULL;
				wsprintf(str,"g(left%d)",rand()%count);
			}
			break;
		}
	case SOUND_GIVE_BACK://���
		{
			if(IsBoy)
			{
				count = f.GetKeyVal(key,"bgivebackcount",10);
				if(count == 0)
					return NULL;
				wsprintf(str,"b(giveback%d)",rand()%count);
			}
			else
			{
				count = f.GetKeyVal(key,"ggivebackcount",10);
				if(count == 0)
					return NULL;
				wsprintf(str,"g(giveback%d)",rand()%count);
			}
			break;
		}
	default:
		if(IsBoy)
			wsprintf(str,"b(sence_0)");
		else
			wsprintf(str,"g(sence_0)");
		break;
	}
	name=f.GetKeyVal(key,str,str);
	wsprintf(filename,"%s%s%s",folder,name,extendname);
	return 	PlayCardMusic(filename,hParent);
}

/*
����ͨ������
ʱ�侯��
��ʾ��Ч
����
*/
//��Ϸ����
BOOL SetPlaySound(BYTE bSoundType,bool bisboy )
{
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "cardsound.bcf");
	CString key = TEXT("system");
	CString folder,filename;
	folder=f.GetKeyVal(key,"folder","music\\cardwav\\");
	TCHAR fi[MAX_PATH+1];
	switch(bSoundType)
	{
	case SOUND_WARNING://����
		{
			filename = f.GetKeyVal(key,"warning","warning");
			break;
		}
	case SOUND_INVALIDATE://��Ч
		{
			filename = f.GetKeyVal(key,"invalidate","invalidate");
			break;
		}
	case SOUND_SEND_CARD://����
		{
			filename = f.GetKeyVal(key,"sendcard","sendcard");
			break;
		}
	case  SOUND_OUT_CARD_DEF://����
		{
			filename = f.GetKeyVal(key,"outcarddef","outcarddef");
			break;
		}
	case SOUND_OUT_CARD_BOMB://ը��
		{
			filename = f.GetKeyVal(key,"bomb","bomb");
			break;
		}
	case SOUND_OUT_CARD_PASS://��Ҫ
		{
			filename = f.GetKeyVal(key,"pass","pass");
			break;
		}
	case SOUND_START_DEF://��ʼ
		{
			filename = f.GetKeyVal(key,"startdef","startdef");
			break;
		}
	case SOUND_GAME_OVER://��Ϸ����
		{
			filename = f.GetKeyVal(key,"gameover","gameover");
			break;
		}
	case SOUND_ADD_DOUBLE://�Ӱ�
		{
			filename = f.GetKeyVal(key,"adddouble","adddouble");
			break;
		}
	case SOUND_ROB_NT://������
		{
			if ( bisboy )
			{	
				filename = f.GetKeyVal(key,"robnt","brobnt");
			}
			else
			{
				filename = f.GetKeyVal(key,"robnt","grobnt");
			}
			break;
		}
	case SOUND_SHOW_CARD://��������
		{
			filename = f.GetKeyVal(key,"showcard","showcard");
			break;
		}

	case SOUND_WIN:
		{
			filename = f.GetKeyVal(key,"win","win");
			break;
		
		}
	case SOUND_LOSE:
		{
			filename = f.GetKeyVal(key,"lost","lost");
			break;
			
		}
	}
	wsprintf(fi,"%s%s%s%s",s,folder,filename,".wav");
	::PlaySound(fi,NULL,SND_FILENAME|SND_ASYNC|SND_NOSTOP|SND_NOWAIT|SND_NODEFAULT);
	return true;
}

//�û�Ѻע����(רΪ���׼��)
BOOL SetNoteSound(BYTE bNoteStyle,BOOL bIsBoy)
{
	TCHAR str[MAX_PATH+1]="";
	TCHAR filename[MAX_PATH];//path[MAX_PATH];
	CString s=CBcfFile::GetAppPath ();/////����·��
	CBcfFile f( s + "cardsound.bcf");
	CString key = TEXT("shnote");
	//	TCHAR path[MAX_PATH];
	CString folder,extendname,name;
	folder=f.GetKeyVal(key,"folder","music\\sh\\");
	extendname=f.GetKeyVal(key,"extendname",".wav");
	TCHAR Val[50];
	srand(GetCurrentTime());
	int count =1;//f.GetKeyVal(key,"count",1);
	switch(bNoteStyle)
	{
	case TYPE_SHUTTLE:
		{	
			wsprintf(Val,"%c(show%d)",bIsBoy?'b':'g',rand()%count);
			name = f.GetKeyVal(key,Val,Val);
			break;
		}
	case TYPE_FOLLOW:
		{
			wsprintf(Val,"%c(follow%d)",bIsBoy?'b':'g',rand()%count);
			name = f.GetKeyVal(key,Val,Val);
			break;
		}
	case TYPE_ADD:
		{
			wsprintf(Val,"%c(add%d)",bIsBoy?'b':'g',rand()%count);
			name = f.GetKeyVal(key,Val,Val);
			break;
		}
	case TYPE_NOTE:
		{
			wsprintf(Val,"%c(note%d)",bIsBoy?'b':'g',rand()%count);
			name = f.GetKeyVal(key,Val,Val);
			break;
		}
	case TYPE_GIVE_UP:
		{
			wsprintf(Val,"%c(pass%d)",bIsBoy?'b':'g',rand()%count);
			name = f.GetKeyVal(key,Val,Val);
			break;
		}	
	default:
		{
			wsprintf(str,"%c(def)",bIsBoy?'b':'g');
			name = f.GetKeyVal(key,Val,Val);
			break;
		}	
	}
	wsprintf(filename,"%s%s%s",folder,name,extendname);

	::PlaySound(filename,NULL,SND_FILENAME|SND_ASYNC|SND_NOSTOP|SND_NOWAIT|SND_NODEFAULT);
	return true;
}

/*
parameter iCardList ���ƶ���
parameter iCardCount ��������
parameter IsBoy		�Ա�
parameter IsFirst	�Ƿ��һ����(�粻֧��������,����������ȫ������Ĭ�ϵ����������,����)
*/
//���ó�����������(רΪ�ĳ�׼��
HWND SetOutCardTypeSound(BYTE iCardList[],int iCardCount,HWND hParent,BOOL IsBoy,BOOL IsFirest )
{

	return NULL;
}

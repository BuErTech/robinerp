#include "StdAfx.h"
#include "PlaceResource.h"

//�������� 
CImageList											CPlaceResource::m_GameListImage;		//��Ϸͼ��
CArray<IconIDInfoStruct,IconIDInfoStruct>			CPlaceResource::m_IconIndex;			//����

//DLL �ӿں�������
typedef HICON (GetGameIconFunc)(UINT,bool);
// �������������Ϣ
extern void DebugPrintf(const char *p, ...);


//��ʼ������
void CPlaceResource::Init(COLORREF color)
{
	//��ʼ����Ϸ�б�ͼ��
	UINT uIconID[]=
	{
		//�̶�ͼ��
		IDI_LIST_KIND,
		IDI_LIST_ROOM,
		IDI_LIST_NONAME,
		IDI_LIST_NOINSTALL,
		IDI_LIST_ROOM,
		IDI_LIST_ROOM,

		//��������
		IDI_LIST_ROOT,
		IDI_LIST_ROOM_LOCK,

		// duanxiaohui 20111107 add ������Ϸ�б����Ŀ¼ͼ��(�����Ŀ¼Icon�±�λ��Ϊ13, ��8��9��10��11��12 ֮ǰ�±�λû��Icon, ��ʱȫ�����һ����)
		IDI_LIST_KIND2,
		IDI_LIST_KIND2,
		IDI_LIST_KIND2,
		IDI_LIST_KIND2,
		IDI_LIST_KIND2,
		IDI_LIST_KIND2
		// end duanxiaohui
	};
	m_GameListImage.DeleteImageList();
	m_GameListImage.Create(16,16,ILC_COLOR16|ILC_MASK,sizeof(uIconID)/sizeof(uIconID[0]),sizeof(uIconID)/sizeof(uIconID[0]));
	m_GameListImage.SetBkColor(color);
	for (UINT i=0;i<sizeof(uIconID)/sizeof(uIconID[0]);i++)
	{
		HICON hIcon=AfxGetApp()->LoadIcon(uIconID[i]);
		m_GameListImage.Add(hIcon);
		DestroyIcon(hIcon);
	}
	return;
}

void CPlaceResource::SetColor(COLORREF color)
{
	m_GameListImage.SetBkColor(color);
}
//��ȡͼ������
UINT CPlaceResource::GetListIconIndex(ComNameInfo * pNameItem)
{
	//Ѱ��ͼ��
	for (INT_PTR i=0;i<m_IconIndex.GetCount();i++)
	{
		IconIDInfoStruct * pIconInfo=&m_IconIndex[i];
		if (pIconInfo->uNameID==pNameItem->uNameID)	return pIconInfo->uIconIndex;
	}

	CString gamename=pNameItem->szGameProcess;
	gamename.MakeLower();
	if(gamename.Right(4)==".ico")
	{//ԭ�з�ʽ
		//��ȡDLL�е�ͼ��
		HINSTANCE hDllHandle=::LoadLibrary(pNameItem->szGameProcess);
		if (hDllHandle!=NULL)
		{
			//��ȡͼ��
			GetGameIconFunc * pGetIcon=(GetGameIconFunc *)GetProcAddress(hDllHandle,TEXT("BeginInitGameIcon"));
			if (pGetIcon!=NULL)
			{
				IconIDInfoStruct IconInfo;
				memset(&IconInfo,0,sizeof(IconInfo));
				IconInfo.uNameID=pNameItem->uNameID;
				HICON hListIcon=pGetIcon(pNameItem->uNameID,true);
				if (hListIcon!=NULL) 
				{
					IconInfo.uIconIndex=m_GameListImage.Add(hListIcon);
					DestroyIcon(hListIcon);
				}
				else IconInfo.uIconIndex=IND_LIST_NO_ICON;
				::FreeLibrary(hDllHandle);
				return IconInfo.uIconIndex;
			}
			::FreeLibrary(hDllHandle);
			return IND_LIST_NO_ICON;///��ͼ�꣬ʹ��Ĭ��ͼ��
		}
		return IND_LIST_NO_INSTALL;//û��װ
	}
	else
	{//���뷽ʽ
		//��ȡDLL�е�ͼ��
		//{{Modified by zxd ��ȷ�ж���Ϸ�����ļ���
		TCHAR szIcoName[MAX_PATH];
		IconIDInfoStruct IconInfo;
		memset(&IconInfo,0,sizeof(IconInfo));
		IconInfo.uNameID=pNameItem->uNameID;

		wsprintf(szIcoName,"%d\\GameIcon.ico",pNameItem->uNameID);
		HICON hListIcon=(HICON)LoadImage(NULL,szIcoName,IMAGE_ICON,0,0,LR_LOADFROMFILE);//LoadImage()  LoadIcon(AfxGetInstanceHandle(),szIcoName);
		if(hListIcon)
		{
			IconInfo.uIconIndex=m_GameListImage.Add(hListIcon);
			DestroyIcon(hListIcon);
		}
		else
			IconInfo.uIconIndex=IND_LIST_NO_INSTALL;
		return IconInfo.uIconIndex;
	}



/*

//û��װ*/
}

//��ȡ����ͼ��
HICON CPlaceResource::GetRoomIcon(CAFCRoomItem * pRoomItem, TCHAR * szProcessName)
{
	HICON hRoomIcon=NULL;
	//{{Modify By ZXDΪ������ȷװ����Ϸͼ�꣬�ӷ�������������exe��չ��
	TCHAR szIcoName[MAX_PATH];
	int nLen = _tcslen(szProcessName);
	if ((szProcessName[nLen-3] == TCHAR('e') || szProcessName[nLen-3] == TCHAR('E'))
		&&(szProcessName[nLen-2] == TCHAR('x') || szProcessName[nLen-2] == TCHAR('X'))
		&&(szProcessName[nLen-1] == TCHAR('e') || szProcessName[nLen-1] == TCHAR('E'))
		)
	{
		wsprintf(szIcoName,"%d\\GameIcon.ico",pRoomItem->m_RoomInfo.uNameID);
		hRoomIcon=LoadIcon(AfxGetInstanceHandle(),szIcoName);
		if (hRoomIcon==NULL) hRoomIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_LOGON));
		return hRoomIcon;
	}
	else
	{
		HINSTANCE hDllHandle=::LoadLibrary(szProcessName);
		if (hDllHandle!=NULL)
		{
			//��ȡͼ��
			GetGameIconFunc * pGetIcon=(GetGameIconFunc *)GetProcAddress(hDllHandle,TEXT("BeginInitGameIcon"));
			if (pGetIcon!=NULL) hRoomIcon=pGetIcon(pRoomItem->m_RoomInfo.uNameID,false);
			::FreeLibrary(hDllHandle);
		}

		//��ȡĬ��ͼ��
		if (hRoomIcon==NULL) hRoomIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_LOGON));
		return hRoomIcon;
	}
}

UINT CPlaceResource::LoadExtendIcon(CString szIconPath)
{
	HICON hExtendIcon=NULL;
	CString spath;
	spath.Format("%s\\image\\client\\%s",CBcfFile::GetAppPath(),szIconPath);
	hExtendIcon=(HICON)LoadImage(NULL,spath,IMAGE_ICON,0,0,LR_LOADFROMFILE);
	if(hExtendIcon)
	{
		UINT uIconIndex=m_GameListImage.Add(hExtendIcon);
		DestroyIcon(hExtendIcon);
		return uIconIndex;
	}
	return 0;
}

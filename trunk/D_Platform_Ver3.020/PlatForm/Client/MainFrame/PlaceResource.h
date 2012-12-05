#pragma once
#ifndef PLACERESOURCE_HEAD_H
#define PLACERESOURCE_HEAD_H
#include "StdAfx.h"
#include "Resource.h"
#include "AfxTempl.h"
#include "GameListCtrl.h"

//�̶�ͼ��
#define IND_KIND_ICON				0							//��Ϸ����
#define IND_ROOM_ICON				1							//��Ϸ����
#define IND_LIST_NO_ICON			2							//û��ͼ��
#define IND_LIST_NO_INSTALL			3							//û�а�װ
#define IND_ROOM_NO_ICON			4							//û��ͼ��
#define IND_NO_ROOM					5							//û�з���

//��չͼ��
#define IND_LIST_ROOT				6							//��Ϸ�б�
#define IND_LIST_BANK				7							//��������
#define IND_LIST_DOWN				8							//��Ϸ����
#define IND_LIST_FORUM				9							//��Ϸ��̳
#define IND_LIST_LINE				10							//��Ϸ����
#define IND_LIST_PROTECT			11							//���뱣��
#define IND_LIST_CLASS				12							//��Ϸ����

// duanxiaohui 20111107 add ��������Ŀ¼ͼ��
#define IND_KIND2_ICON              13                          //��Ϸ����
// end duanxiaohui

//ͼ������
struct IconIDInfoStruct
{
	UINT						uNameID;						//���� ID
	UINT						uIconIndex;						//ͼ������
};

//��Ϸ������Դ��
class CPlaceResource
{
	//��������
protected:
	static CArray<IconIDInfoStruct,IconIDInfoStruct>			m_IconIndex;			//����
	

	//��������
public:
	static CImageList											m_GameListImage;		//ͼ��
	//��ʼ������
	static void Init(COLORREF color);
	//��ɫ
	static void SetColor(COLORREF color);
	//��ȡͼ����
	static CImageList * GetGameListIcon() { return & m_GameListImage; }
	//��ȡͼ������
	static UINT GetListIconIndex(ComNameInfo * pNameItem);
	//��ȡ����ͼ��
	static HICON GetRoomIcon(CAFCRoomItem * pRoomItem, TCHAR * szProcessName);
public:
	static UINT LoadExtendIcon(CString szIconPath);
};
#endif
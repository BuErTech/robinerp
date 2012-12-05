#pragma once
#ifndef GAMELISTCTRL_HEAD_H
#define GAMELISTCTRL_HEAD_H

#include "StdAfx.h"
#include "GamePlace.h"
#include "SkinMgr.h"

//��˵��
class CGameListCtrl;
class CAFCNameItem;
struct MSG_GP_SR_OnLineStruct;

typedef vector<MSG_GP_R_SellGameListResult> SELLLISTINFO;

class CAFCGameListItemBase
{
public:
	UINT										m_uDataType;					//��������
};

// class CAFCGameUrlItem: public CAFCGameListItemBase
// {
// public:
// 	HTREEITEM									m_hCurrentItem;					//��ǰ����
// 	CString										m_stName;
// 	CString										m_stUrl;
// 	CAFCGameUrlItem()
// 	{
// 		m_uDataType = 0;
// 		m_hCurrentItem = NULL;
// 		m_stName = "";
// 		m_stUrl = "";
// 	}
// 	CAFCGameUrlItem(UINT uDataType, HTREEITEM hCurrentItem, CString stName, CString stUrl)
// 	{
// 		m_uDataType = uDataType;
// 		m_hCurrentItem = hCurrentItem;
// 		m_stName = stName;
// 		m_stUrl = stUrl;
// 	}
// };

/*******************************************************************************************************/
//��Ϸ�б�����
class CAFCGameListItem: public CAFCGameListItemBase
{
	//���ݱ���
public:
	LONG										m_uUpdateTime;					//����ʱ��
	CPtrArray									m_ItemPtrArray;					//��������
	CAFCGameListItem							* m_pParentItem;				//����ָ��

	//��ͼ����
public:
	HTREEITEM									m_hUpdateItem;					//��ȡ����
	HTREEITEM									m_hCurrentItem;					//��ǰ����
	static CGameListCtrl						* m_pTreeCtrl;					//���ؼ�ָ��

	//��������
public:
	//��������
	CAFCGameListItem();
	//��������
	virtual ~CAFCGameListItem();

	//���ܺ���
public:
	//��ʼ����
	bool Init(CAFCGameListItem * pParentItem);
	//������Ϸ����
	bool AddGameKind(ComKindInfo * pGameKind,int initType = GLK_GAME_KIND);
	//������Ϸ����
	CAFCNameItem* AddGameName(ComNameInfo * pGameName,int initType = GLK_GAME_NAME);
	//������Ϸ����
	bool AddGameRoom(ComRoomInfo * pGameRoom); 
	//�����������
	bool AddContestItem(ComRoomInfo * pGameRoom);

	//�����ڲ�����
	bool AddInsideData(GameInsideInfo * pInsideData, UINT uIconIndex);
	//չ������
	bool ExpandItem(int iExpendCount);
	//�Ƿ���Ҫ����
	bool IsNeedUpdate();
	//���»�ȡ������
	bool UpdateGetListItem();
	//ɾ����ʱ���� 
	bool DeleteOutTimeItem();

	//��������
protected:
	//��������
	HTREEITEM AddTreeData(TCHAR * szText, UINT uIconIndex, LPARAM lParam);

private:
	CSkinMgr m_skinmgr;
};

/*******************************************************************************************************/
class CAFCGameUrlItem: public CAFCGameListItem
{
public:
	HTREEITEM									m_hCurrentItem;					//��ǰ����
	CString										m_stName;
	CString										m_stUrl;
	CAFCGameUrlItem()
	{
		m_uDataType = 0;
		m_hCurrentItem = NULL;
		m_stName = "";
		m_stUrl = "";
	}
	CAFCGameUrlItem(UINT uDataType, HTREEITEM hCurrentItem, CString stName, CString stUrl)
	{
		m_uDataType = uDataType;
		m_hCurrentItem = hCurrentItem;
		m_stName = stName;
		m_stUrl = stUrl;
	}
};
//��Ϸ��������
class CAFCKindItem : public CAFCGameListItem
{
	//��������
public:
	ComKindInfo								m_KindInfo;						//��������

	//��������
public:
	//���캯��
	CAFCKindItem() 
	{ 
		m_uDataType=GLK_NO_DATA;
		memset(&m_KindInfo,0,sizeof(m_KindInfo)); 
		m_KindInfo.Head.uSize=sizeof(m_KindInfo);
		m_KindInfo.Head.bDataType=GLK_GAME_KIND;
	};
	//��������
	virtual ~CAFCKindItem() {};
};

/*******************************************************************************************************/

//��Ϸ��������
class CAFCNameItem : public CAFCGameListItem
{
	//��������
public:
	///UINT										m_uOnLineCount;					//��������
	ComNameInfo								m_NameInfo;						//��������

	//��������
public:
	//���캯��
	CAFCNameItem() 
	{ 
		m_hUpdateItem=NULL;
		m_uDataType=GLK_GAME_NAME;
		memset(&m_NameInfo,0,sizeof(m_NameInfo)); 
		m_NameInfo.Head.uSize=sizeof(m_NameInfo);
		m_NameInfo.Head.bDataType=GLK_GAME_NAME;
	};
	//��������
	virtual ~CAFCNameItem() {};
	//������������
	bool UpdateOnLineCount();
};

/*******************************************************************************************************/

//��Ϸ��������
class CAFCRoomItem : public CAFCGameListItem
{
	//��������
public:
	ComRoomInfo								m_RoomInfo;						//��������

	bool									m_bIsContest;		//�Ƿ�������
	int										m_iContestID;		//����ID
	int										m_iContestPeopleCount; //��������
	vector<ComRoomInfo>						m_RoomContest;		//��������

	//��������
public:
	//���캯��
	CAFCRoomItem() 
	{ 
		m_uDataType=GLK_GAME_ROOM;
		m_bIsContest = false;
		m_iContestID = -1;
		m_iContestPeopleCount = 0;
		m_RoomContest.clear();
		memset(&m_RoomInfo,0,sizeof(m_RoomInfo)); 
		m_RoomInfo.Head.uSize=sizeof(m_RoomInfo);
		m_RoomInfo.Head.bDataType=GLK_GAME_ROOM;
	};
	//��������
	virtual ~CAFCRoomItem() {};
};

/*******************************************************************************************************/

//��Ϸ�ڲ�����
class CAFCInsideItem : public CAFCGameListItem//��Ϸ�б�
{
	//��������
public:
	GameInsideInfo								m_InsideInfo;					//�ڲ�����

	//��������
public:
	//���캯��
	CAFCInsideItem()
	{
		m_uDataType=GLK_GAME_USE;
		memset(&m_InsideInfo,0,sizeof(m_InsideInfo)); 
		m_InsideInfo.Head.uSize=sizeof(m_InsideInfo);
		m_InsideInfo.Head.bDataType=GLK_GAME_USE;
	}
	//��������
	virtual ~CAFCInsideItem() {};
};

/*******************************************************************************************************/

//��Ϸ�б������

class CGameListCtrl : public CTreeCtrl////////////��������һ������
{
	//��������
protected:
	CPtrArray									m_InsideArray;					//��Ϸ�ڲ�����
	CPtrArray									m_GameUrl;

	vector<TLoginGameInfo>						m_LoginGameInfo;
	
public:
	void SetItemFont(HTREEITEM,   LOGFONT&); 
	void SetItemBold(HTREEITEM,   BOOL); 
	void SetItemColor(HTREEITEM,   COLORREF);
	void SetItemStatusColor(HTREEITEM hItem,UINT roomID,const COLORREF& color,const COLORREF& statusColor,const CString& strStatusText);
	BOOL GetItemFont(HTREEITEM,   LOGFONT   *); 
	BOOL GetItemBold(HTREEITEM); 
	COLORREF GetItemColor(HTREEITEM); 

protected: 
	struct   Color_Font
	{
		COLORREF	color; 
		LOGFONT		logfont;
		COLORREF	statuscolor;
		LOGFONT		statusfont;
		Color_Font()
		{
			logfont.lfFaceName[0] = '\0';
			statusfont.lfFaceName[0] = '\0';
			color = RGB(0,0,0);
			statuscolor = RGB(0,0,0);
		}
	};
	CMap<void*,void*,Color_Font,Color_Font&>	m_mapColorFont; 
	static std::map<UINT,Color_Font> m_mapStatusColor;  //����ID�ͷ���״̬��ɫ
	static std::map<void*,UINT> m_mapHitemRoomID;		//��Ϸ���������ͷ���IDӳ��
	COLORREF m_crGridLine;
	COLORREF m_crGridHotBk1;
	COLORREF m_crGridHotBk2;
	COLORREF m_crGridHotTxt1;
	COLORREF m_crGridHotTxt2;

	CGameImage m_imgLogo;		//�б���ڵ�logo
	CGameImage m_imgFold;		//�б��۵�logo
	CGameImage m_imgUnFold;		//�б��۵�logo
	HTREEITEM  m_hLastMMItem;	//�����������Item


	CGameImage m_imgAD;			//���������������ı����ͱ����Ѿ���ʼ�Ѿ�����

	CNormalBitmapButtonNew	m_btRule;	 //�鿴����ť


	HCURSOR m_curHand;		//�������1
	HCURSOR m_curArrow;		//�������2

protected: 
	//{{AFX_MSG(CTreeCtrlEx) 
	afx_msg void OnPaint(); 
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
 	//afx_msg void OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult);
// 	afx_msg void OnTvnItemexpanded(NMHDR *pNMHDR, LRESULT *pResult);
 	afx_msg BOOL OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnNMDlbClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG cga

	DECLARE_MESSAGE_MAP()
	//��������

	void OnClickReadRule();

public:
	///void UpdateAllOnLineData();
	SELLLISTINFO								m_SellListVec;					//�����б�

//��������
    bool HandleNameOnLineData(DL_GP_RoomListPeoCountStruct * pOnLineCountPtr, UINT uCount);
	// ������Ϸ
	CAFCNameItem *FindNameItem(UINT uID, UINT uKind = -1);
	// ���ҷ���
	CAFCRoomItem *FindRoomItem(UINT uRoomID);

	// ���ҷ���,���ڱ�������
	CAFCRoomItem *FindRoomItemEx(UINT uRoomID, UINT uKind = -1);

	void IEnterARoomOrLeave(UINT uRoomID,bool bEnter);
	///void IEnterARoomOrLeave(CAFCRoomItem * pGameRoomItem,bool bEnter);
	bool ExpandItemFirstRoom();
	bool HandleRoomOnLineData(DL_GP_RoomListPeoCountStruct * pOnLineCountPtr, UINT uCount);
	//��������
	CGameListCtrl();
	//��������
	virtual ~CGameListCtrl();
	
	//���ܺ���
public:
	//��ʼ������
	bool Init();
	//
	bool Init2();
	//չ������
	bool ExpandItem(int iExpendCount);
	//�������
	bool AddRootItem(GameInsideInfo * pInsideData, UINT uIconIndex);
	//��ȡ����
	//////////////////////////////////
	///Kylin 20090107  ��������Ϸ�б�
	CAFCGameListItem * GetKindParentItem() 
	{ 
		if(Glb().m_baFavor.GetCount()) 
			return (CAFCGameListItem *)m_InsideArray.GetAt(1);
		else return (CAFCGameListItem *)m_InsideArray.GetAt(0); 
	} 
	CAFCGameListItem * GetSellListParentItem() 
	{ 
		return (CAFCGameListItem *)m_InsideArray.GetAt(1); 
	} 
	//////////////////////////////////
	//��������
	bool HandleIOnLineData(MSG_GP_SR_OnLineStruct * pOnLineCountPtr, UINT uCount);
	//��������
	bool HandleKindData(ComKindInfo * pKindInfoPtr, UINT uKindCount);

	//��������
	bool HandleUrlData(CAFCGameUrlItem * pUrlInfoPtr, UINT uDatatype);

	bool HandleLoginGame(int iGameID);
	bool HandleLoginGameName(ComNameInfo * pNameInfoPtr);
	void ClearLoginGameName();

	//��������
	bool HandleNameData(ComNameInfo * pNameInfoPtr, UINT uNameCount);
	//��������
	bool HandleRoomData(ComRoomInfo * pRoomInfoPtr, UINT uRoomCount, UINT uKindID, UINT uNameID);
	//����ȡ������
	bool FillGetListItem();
	//ɾ����ʱ���� 
	bool DeleteOutTimeRoom(UINT uKindID, UINT uNameID);
	//��������������
	bool UpdateOnLineCount();
	//��ȡ��������
	bool GetProcessName(CAFCRoomItem * pGameRoomItem, TCHAR * szProcessName, UINT uBufferSize);
	//��ȡ��Ϸ����
	bool GetGameName(CAFCRoomItem * pGameRoomItem, TCHAR * szGameName, UINT uBufferSize);
	void UpdataItem(CAFCGameListItem* pItem);
	//��ȡItem�Ĳ��ֵ
	int GetItemLevel(HTREEITEM hItem);

    // PengJiLin, 2011-7-20, ��Ϸ�����б�֧����ʾ��������������ʾ��˵����ʾ(���С���æ)
    static bool m_bUseNumShow;      // ʹ�����ֻ���˵��
    static void KindOnLineShow(TCHAR* pchDest, TCHAR* pchName, UINT uNum);
    static void RoomOnLineShow(TCHAR* pchDest, ComRoomInfo* pRoomInfo,COLORREF& statusColor,CString& statusText);
    static void RootOnLineShow(TCHAR* pchDest, TCHAR* pchFormat, UINT uNum);
    void CheckIfUseNumShow();       // ��Ⲣȷ����ʾ��ʽ

	//��������
private:
	//������������
	CAFCKindItem * FindKindItem(UINT uKindID);
	//������������
	CAFCNameItem * FindNameItem(CAFCKindItem * pKindItem, UINT uNameID);
	CAFCNameItem * FindNameItem(CAFCGameListItem * pKindItem, UINT uNameID);
	//���ҷ�������
	CAFCRoomItem * FindRoomItem(CAFCNameItem * pNameItem, UINT uRoomID, ComRoomInfo* pRoomInfo = NULL);

	//���ҷ�������
	CAFCRoomItem * FindRoomItemByContest(CAFCNameItem * pNameItem, ComRoomInfo* pRoomInfo);
	//���ұ�����������
	ComRoomInfo * FindContestRoomItem(CAFCRoomItem * pRoomItem, int iRoomID);
public:
	void UpdateAllIcon(void);
	void JoinAllItem(void);
	CAFCKindItem * FindSellKindItem(UINT uKindID);
public:
	bool ExpandAll(bool ExpName=true);

private:
	CSkinMgr m_skinmgr;

public:
	void LoadSkin();
public:
	LRESULT OnExchangeSkin(WPARAM wpara,LPARAM lpara);
};

/*******************************************************************************************************/

//�����б������
class CFunListCtrl : public CTreeCtrl////////////��������һ������
{
	//��������
protected:
	CPtrArray									m_InsideArray;					//��Ϸ�ڲ�����
	DECLARE_MESSAGE_MAP()
	//��������
public:

	//��������
	CFunListCtrl();
	//��������
	virtual ~CFunListCtrl();
	
	//���ܺ���
public:
	//��ʼ������
	bool Init();
	CStringArray m_saUrl;

private:
	CSkinMgr m_skinmgr;
public:
	void LoadSkin();
};
#endif
/*******************************************************************************************************/

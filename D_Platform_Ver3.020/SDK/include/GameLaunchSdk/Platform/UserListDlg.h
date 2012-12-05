#pragma once
#ifndef USERLISTDLG_HEAD_H
#define USERLISTDLG_HEAD_H

#include "PublicDefine.h"
#include "AFCButton.h"
#include "AFCDialog.h"
#include "AFCListCtrl.h"
#include "../../../../SDK/include/NewGameFrame/Client/DirectUI/BzDui.h"
#include "../SkinMgr.h"

//���ֿռ�����
/*using namespace AFC;
using namespace AFC::AFCDialog;
using namespace AFC::AFCButton;
using namespace AFC::AFCListCtrl;*/

/*******************************************************************************************************/

//�б���
class EXT_CLASS CGameUserListCtrl  : public CHappyGoDlg
{
	//��������
private:
	static UINT					m_uItemKind[12];					//��������
	int							m_uComType;
	CString						m_iniFile;
	BOOL                        m_bMode;                            // ����ģʽ

    // PengJiLin, 2010-6-11, �û��б��������ʽ��Ÿ��
    // 0Ϊԭ�ȵ���ʽ��1ΪŸ����ʽ��2...
    BYTE m_byUserListType;

	//��������
public:
	//���캯��
	CGameUserListCtrl();  
	//��������
	virtual ~CGameUserListCtrl();
	//��ʼ��
	virtual void InitListCtrl(GetOrderName * pOrderFunction, UINT uComType);
	//�Ƿ�����鿴 IP
	void EnableIPView(bool bLookIP);
	//�����Թ۶���
	void SetWatchUser(UserItemStruct * pUserItem, TCHAR * szWatchName);

    // PengJiLin, 2010-6-11, ��ȡ�û��б��������ʽ��Ÿ��
    inline BYTE GetUserListDefineType(){return m_byUserListType;}

	//��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()

	//���غ���
public:
	void SetIniFile(LPCTSTR path, BOOL bMode = FALSE);
	//�����û�
	virtual bool AddGameUser(UserItemStruct * pUserItem, bool bIsMySelf = false);
	//ɾ���û�
	virtual bool DeleteGameUser(UserItemStruct * pUserItem);
	//����״̬
	virtual bool UpdateGameUser(UserItemStruct * pUserItem);
	//��Ƥ��
	virtual void LoadSkin(void);
private:
	/// �����������ȡ����Ҫ�����λ��
	int GetInsertPos(UserItemStruct * pUserItem);
	/// ��ȡ���ID��Ϣ���Ƿ�ɢ�����ر��˺ţ�ָ���˺�
	/// @param UserID ���ID,NomalIDFromɢ��ID��ʼ��NomalIDEndɢ��ID����
	/// @return 0��8��ָ���˺ź����0�ĸ�����-1Ϊɢ����-2Ϊ��Ч�˺ţ�-5Ϊ5���ţ�-6Ϊ6����
	int GetIDInformation(long UserID,long NomalIDFrom,long NomalIDEnd);
	/// ����������
	void LoadColumnConfig();
	/// ���ַ��������� ˳��ţ��������п�
	void DecodeString(CString str,CString &strColumn,int &width);
	/// ����ÿһ�е�ֵ
	void PrepareDate(UserItemStruct * pUserItem);

private:
	CSkinMgr	m_skinmgr;

public:
	void InitIniFile();
};

//�б���(Bz DirectUI��)
class EXT_CLASS CGameUserListCtrlDu
{

private:

	int							m_uComType;
	CString						m_iniFile;
	BOOL                        m_bMode;                            //����ģʽ
	GetOrderName				*m_pPointOrderFun;					//�ȼ�����

public:
	UINT						m_uGameNameID;						//��ϷID

public:

	//���캯��
	CGameUserListCtrlDu(BzDui::CListUI* pListCtrl);

	//��ʼ��
	virtual void InitListCtrl(GetOrderName* pOrderFunction, UINT uComType);

	//�Ƿ�����鿴 IP
	void EnableIPView(bool bLookIP);

	//�����Թ۶���
	void SetWatchUser(UserItemStruct* pUserItem, TCHAR* szWatchName);

	//��ȡ�û�״̬ ID
	UINT GetStateImageIndex(UserItemStruct* pUserItem);

	//��ȡ��n�е��û�����
	UserItemStruct* GetUser(int nRow);

//���غ���
public:

	//�����û�
	virtual bool AddGameUser(UserItemStruct* pUserItem,bool bIsMySelf = false);

	//����״̬
	virtual bool UpdateGameUser(UserItemStruct * pUserItem);

	//ɾ���û�
	virtual bool DeleteGameUser(UserItemStruct * pUserItem);

	//ɾ������
	virtual void DeleteAllItems();

//��Ϣ������
public:

	//�б��ı��ػ�
	void OnGetItemText(UserItemStruct* pUserItem,int iIndex,int iSubItem,char* szout,bool bSecret = false, bool bshowname = true);
private:

	/// ����������
	void LoadColumnConfig();

	/// ���ַ��������� ˳��ţ��������п�
	void DecodeString(CString str,CString &strColumn,int &width);

	//����ÿһ�е�ֵ
	void PrepareDate(UserItemStruct* pUserItem,BzDui::CListTextElementUI* pElementItem);

	//��������������б��λ��
	int FindItem(UserItemStruct* pUserItem,VtDuTable::iterator* _out_it_found = NULL);

	//��������б���Ĳ���λ��
	int GetInsertPos(UserItemStruct* pUserItem,VtDuTable::iterator* _out_it_found = NULL);

public:

	int FindItem(BzDui::CControlUI* pItemCtrl);

//�ؼ�����
public:

	//�б�ؼ�
	BzDui::CListUI* m_pListCtrl;

//����
protected:

	//�����б�item��ʱ��Ϣ
	TDuItem m_TempItem;

	//�б�����item��Ϣ
	VtDuTable m_vTable;

	//�б�ͷ˳��
	std::vector<string> m_vtHeader;

	//�Լ�����Ϣ
	UserItemStruct* m_pMeUser;

};


/*******************************************************************************************************/

//�û��б�Ի���
class EXT_CLASS CPictureHelper/*C-UserListDlg*/ : public CGameFaceGo
{
	//�ؼ�����
public:
	CGameUserListCtrl						m_UserList;						//�û��б� MFC��
	CGameUserListCtrlDu*					m_pUserListDu;					//�û��б� BzDui��

	//��������
public:
	//���캯�� 
	CPictureHelper();
	//��������
	virtual ~CPictureHelper();

	DECLARE_MESSAGE_MAP()

	//���غ���
protected:
	//�ż��󶨺���
	virtual void DoDataExchange(CDataExchange * pDX);
	//ȷ������
	virtual void OnOK() { return; }
	//ȡ������
	virtual void OnCancel() { return; }

	//���ܺ���
public:
	//��ʼ��
	inline void InitListCtrl(GetOrderName * pOrderFunction, UINT uComType);
	//�����û�
	inline bool AddGameUser(UserItemStruct * pUserItem, bool bIsMySelf = false);
	//ɾ���û�
	inline bool DeleteGameUser(UserItemStruct * pUserItem);
	//����״̬
	inline bool UpdateGameUser(UserItemStruct * pUserItem);
	//�Ƿ�����鿴 IP
	inline void EnableIPView(bool bLookIP);
	//ɾ������
	inline void DeleteAllUser();
	//�����Թ۶���
	inline void SetWatchUser(UserItemStruct * pUserItem, TCHAR * szWatchName);

	//��Ϣ���� 
public:
	//λ�ñ仯
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//����Ҽ��û��б���Ϣ
	afx_msg void OnNMRclickUserList(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnNMLclickUserList(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnNMDBclickUserList(NMHDR * pNMHDR, LRESULT * pResult);

private:
	CSkinMgr m_skinmgr;
};

#endif
/*******************************************************************************************************/

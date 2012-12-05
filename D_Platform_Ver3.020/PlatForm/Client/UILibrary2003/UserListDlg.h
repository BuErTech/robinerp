#pragma once
#ifndef USERLISTDLG_HEAD_H
#define USERLISTDLG_HEAD_H

#include "PublicDefine.h"
#include "AFCButton.h"
#include "AFCDialog.h"
#include "AFCListCtrl.h"

//���ֿռ�����
/*using namespace AFC;
using namespace AFC::AFCDialog;
using namespace AFC::AFCButton;
using namespace AFC::AFCListCtrl;*/

/*******************************************************************************************************/

//�б���
class CGameUserListCtrl  : public CHappyGoDlg
{
	//��������
private:
	static UINT					m_uItemKind[15];					//��������
	int							m_uComType;
	CString						m_iniFile;
	CBitmap						m_NewUserStateBmp;
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

	//��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()

	//���غ���
public:
	void SetIniFile(LPCTSTR path);
	//�����û�
	virtual bool AddGameUser(UserItemStruct * pUserItem);
	//ɾ���û�
	virtual bool DeleteGameUser(UserItemStruct * pUserItem);
	//����״̬
	virtual bool UpdateGameUser(UserItemStruct * pUserItem);
	//��Ƥ��
	virtual void LoadSkin(void);
};

/*******************************************************************************************************/

//�û��б�Ի���
class EXT_CLASS CPictureHelper/*C-UserListDlg*/ : public CGameFaceGo
{
	//�ؼ�����
public:
	CGameUserListCtrl						m_UserList;						//�û��б�

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
	inline void InitListCtrl(GetOrderName * pOrderFunction, UINT uComType) { return m_UserList.InitListCtrl(pOrderFunction,uComType); }
	//�����û�
	inline bool AddGameUser(UserItemStruct * pUserItem) { return m_UserList.AddGameUser(pUserItem); };
	//ɾ���û�
	inline bool DeleteGameUser(UserItemStruct * pUserItem) { return m_UserList.DeleteGameUser(pUserItem); };
	//����״̬
	inline bool UpdateGameUser(UserItemStruct * pUserItem) { return m_UserList.UpdateGameUser(pUserItem); };
	//�Ƿ�����鿴 IP
	inline void EnableIPView(bool bLookIP) { m_UserList.EnableIPView(bLookIP); };
	//ɾ������
	inline void DeleteAllUser() { m_UserList.DeleteAllItems(); };
	//�����Թ۶���
	inline void SetWatchUser(UserItemStruct * pUserItem, TCHAR * szWatchName) { m_UserList.SetWatchUser(pUserItem,szWatchName); }

	//��Ϣ���� 
public:
	//λ�ñ仯
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//����Ҽ��û��б���Ϣ
	afx_msg void OnNMRclickUserList(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnNMLclickUserList(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnNMDBclickUserList(NMHDR * pNMHDR, LRESULT * pResult);
};

#endif
/*******************************************************************************************************/

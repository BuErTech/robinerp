#pragma once
#ifndef UESRLISTCTRL_HEAD_H
#define USERLISTCTRL_HEAD_H

#include "StdAfx.h"
#include "Afxcmn.h"
#include "Resource.h"
#include "SkinMgr.h"

///
///CUserListCtrl �û��б���
///
///@Modify yc.yan
///@version 1.0
///@date
///
class CUserListCtrl  : public CHappyGoDlg
{
	//��������
private:
	static UINT					m_uItemKind[15];					//��������
	int							m_uComType;
	GameInfoStruct				*pGameInfo;
	CWnd						*msgWnd;							//20081211 , Fred Huang
	UINT						m_uVirtualUserCount;
	//��������
public:
	//���캯��
	CUserListCtrl();  
	//��������
	virtual ~CUserListCtrl();
	//��ʼ��
	virtual void InitListCtrl(GetOrderName * pOrderFunction, UINT uComType,GameInfoStruct *GameInfo);
	//�Ƿ�����鿴 IP
	void EnableIPView(bool bLookIP);

	//��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()

	//���غ���
public:
	///
	///�����û�
	///
	///@param pUserItem �û���Ϣ
	///
	///@return int �����λ�ã�-1����ʧ��
	///
	virtual bool AddGameUser(UserItemStruct * pUserItem);
	//ɾ���û�
	virtual bool DeleteGameUser(UserItemStruct * pUserItem);
	//����״̬
	virtual bool UpdateGameUser(UserItemStruct * pUserItem);
	//��Ƥ��
	virtual void LoadSkin(void);

public:
	//���� add by rende on 2012-8-31
	void ReLoadSkin();

public:
	/// ����������
	///	@param uVirtualUserCount�����������
	///	@return void
	void fillVirtualUser(int uVirtualUserCount);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	void SetMSGReceiveWnd(CWnd * pWnd);//20081211 , Fred Huang
public:
	/// ���ҿ��е��������
	///@param void
	///@return UserItemStruct ���������Ϣ
	UserItemStruct * GetIdelVirtualUser(void);
	/// ����ָ���������
	///@param long dwUserID ��Ҫ���ҵ��û�ID
	///@return UserItemStruct �����Ϣ
	UserItemStruct * GetUserFromID(long dwUserID);
	/// ��ȡ�û������λ��
	int GetInsertPos(UserItemStruct * pUserItem);
	/// ����������
	void LoadColumnConfig();
	/// ���ַ��������� ˳��ţ��������п�
	void DecodeString(CString str,CString &strColumn,int &width);
	/// ����ÿһ�е�ֵ
	void PrepareDate(UserItemStruct * pUserItem);
	/// ��ȡ���ID��Ϣ���Ƿ�ɢ�����ر��˺ţ�ָ���˺�
	/// @param UserID ���ID,NomalIDFromɢ��ID��ʼ��NomalIDEndɢ��ID����
	/// @return 0��8��ָ���˺ź����0�ĸ�����-1Ϊɢ����-2Ϊ��Ч�˺ţ�-5Ϊ5���ţ�-6Ϊ6����
	int GetIDInformation(long UserID,long NomalIDFrom,long NomalIDEnd);

private:
	CSkinMgr m_skinmgr;

public:
	void ChangeSkin();
};
#endif
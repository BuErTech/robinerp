#pragma once

//////////////////////////////////////////////////////////////////////////
///�����������ڣ����û���д��Ӧ���ݣ�Ȼ��֪ͨ������
///2010.8.12 rongqiufen
//////////////////////////////////////////////////////////////////////////

#include "afxwin.h"

#include "GM_MessageDefine.h"
#include "CommandMessage.h"
#include "comstruct.h"

#include <map>
#include <vector>
using namespace std;

// CAwardDlg �Ի���

class CAwardDlg : public CDialog
{
	DECLARE_DYNAMIC(CAwardDlg)

public:
	CAwardDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAwardDlg();

	///��ʾ����ǰ����������Щ��Ϣ

	///�����û���Ϣ�����ڴ�����ǰ���䣩
	void SetUserInfo(UserInfoStruct *userinfo, int iRoomID);
	///�����û���Ϣ�����ڴ����ǵ�ǰ���䣬�Ӻ�������W�ģ�
	void SetUserInfo(GM_OperatedPlayerInfo_t *userinfo);
	///���ý���������
	void SetAwardProjectList(map<int, GM_AwardType_t*> vAwardProjectList);
	///���õ������Ʊ�
	void SetAwardList(map<int, GM_GetPropInfo_t*> vAwardList);

// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	UINT m_UserID;							///�û�ID
	CString m_UserNickName;					///�û��ǳ�
	CString m_UserLoginName;				///�û���¼��
	CComboBox m_AwardCmb;					///��������������
	CString m_Reason;						///ԭ��
	CString m_Details;						///��������	
	int m_RoomID;							///����ID

	vector<GM_AwardType_t*> m_AwardType;	///����������
	map<int, GM_GetPropInfo_t*> m_PropList;	///�������Ʊ�

	GM_OperateByID_t	m_GMOperate;		///�洢������������
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnCbnSelchangeCombo1();
protected:
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();

};

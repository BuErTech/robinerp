#ifndef USERINFO_H_2003_7
#define USERINFO_H_2003_7

#include "AFCPtrArray.h"
#include "../common/ComStruct.h"
///#include "itemdatemessage.h"
#include "../common/GameRoomMessage.h"
///�궨��
#define ERROR_SOCKET_INDEX					9999L					///��Ч����

/*******************************************************************************************************/

///�󶨷����ṹ
struct UserChangePoint
{
	UINT						uWinCount;							///ʤ��
	UINT						uLostCount;							///���
	UINT						uCutCount;							///ǿ��
	UINT						uMidCount;							///�;�
	int							dwPoint;							///����
	int							dwMoney;							///���
	int							dwTaxCom;							///˰��
};

/*******************************************************************************************************/

///�û�������
class AFX_EXT_CLASS CGameUserInfo
{
	///��������
public:
	bool						m_bAccess;							///���Է���
	char						m_szMD5Pass[50];					///MD5 ����
	int							m_dwScrPoint;						///�û�ԭ��
	long int					m_dwAccessLogID;					///��¼��ʶ
	long int					m_dwLogonTime;						///��½ʱ��
	long int					m_dwGamePower;						///�û�Ȩ��
	long int					m_dwMasterPower;					///����Ȩ��
	long int					m_dwPlayCount;						///��Ϸʱ��
	UINT						m_uSocketIndex;						///��������
	DWORD						m_dwHandleID;						///�����ʶ
	UserRuleStruct				m_Rule;								///��������
	UserInfoStruct				m_UserData;							///�û�����
	UserChangePoint				m_ChangePoint;						///�����仯


	int							m_SelectWeight; 
///	tagItemData					m_HoldItemData;						/// ����ƽ̨��Ʒ���ݣ�

	///�������� 
public:
	///���캯�� 
	CGameUserInfo(void);
	///��������
	~CGameUserInfo(void);

	///���ܺ���
public:
	///�ж��Ƿ�����û���Ϣ
	bool IsAccess() { return m_bAccess; }
	///�����û�����
	bool SetMD5Pass(char szMD5Pass[50]);
	///�����û�ԭ����
	bool SetUserScrPoint(int dwScrPoint);
	///�����û����͵Ľ��
	bool SetUserSendedMoeny(int dwSendMoney);
	///������Ϣ
	bool SetUserData(UserInfoStruct & UserInfo, UINT uSocketIndex, DWORD dwHandleID, long int dwGamePower, long int dwMasterPower, long int dwAccessLogID=0);
	///����״̬
	bool SetUserState(BYTE bUserState, BYTE bDeskNO, BYTE bDeskStation, bool bSetDeskStation);
	///�����û�����
	bool ChangePoint(__int64 dwPoint, __int64 dwTaxCom, bool bWin, bool bLost, bool bMid, bool bCut, long int dwPlayCount, __int64 &dwMoney);
	///���ù���
	bool SetRule(UserRuleStruct * pRule);
	///�Աȹ����Ƿ��ʺ�
	bool IsFixRule(UserRuleStruct * pRule, UserInfoStruct * pCheckUserInfo, BYTE & bErrorResult);
	///�����û���Ϣ
	bool CleanData();
	///��ȡ�û��ܾ���
	UINT GetUserAllCount();
	///��ȡ�û���Ϣ
	UserInfoStruct * GetUserData(UserInfoStruct * pUserInfoStruct);
	///��ȡ��������
	UserChangePoint * GetChangePointInfo(UserChangePoint * pChangePoint);

	//���ӱ�ѡ�е�Ȩֵ
	int GetWeigth();


	///��������
public:
	///��ȡ�����ʶ
	DWORD GetHandleID() { return m_dwHandleID; }
	///��ȡ��������
	UINT GetSocketIndex() { return m_uSocketIndex; }
	///��ȡ��½ʱ��
	long int GetLogonTime() { return (long int)time(NULL)-m_dwLogonTime; }
};

/*******************************************************************************************************/

///�û�������
class AFX_EXT_CLASS CGameUserInfoManage
{
	///��������
public:
	UINT										m_uOnLineCount;					///�����û�����
	CAFCPtrArray<CGameUserInfo>					m_OnLineUserInfo;				///�����û���Ϣ
	CAFCPtrArray<CGameUserInfo>					m_NetCutUserInfo;				///�����û���Ϣ

	///��������
public:
	///���캯��
	CGameUserInfoManage(void);
	///��������
	~CGameUserInfoManage(void);

	///���ܺ���
public:
	///��ʼ������
	bool Init(UINT uMaxOnLineCount, UINT uMaxNetCutCount,UINT uVipReseverCount);
	///ȡ����ʼ��
	bool UnInit();
	///�����û�
	CGameUserInfo * ActiveUser(UserInfoStruct * pUserInfo, UINT uSocketIndex, DWORD dwHandleID, long int dwGamePower, long int dwMasterPower, long int lAccessLogID);
	///�ͷ��û�
	bool FreeUser(CGameUserInfo * pGameUserInfo, bool bOnLinePeople);
	///ע���û�����
	CGameUserInfo * RegNetCutUser(CGameUserInfo * pNetCutUserInfo);
	///��ȡ�û�
	CGameUserInfo * GetOnLineUserInfo(UINT uIndex);
	///��ȡ�����û� 
	UINT FillOnLineUserInfo(char * pOnLineBuffer, UINT uBufferSize, UINT & uIndex, UINT & uFillCount, bool & bFinish);
	///��ȡ�����û� 
	UINT FillNetCutUserInfo(char * pNetCutBuffer, UINT uBufferSize, UINT & uBeginPos, bool & bFinish);
	///�����û�
	CGameUserInfo * FindOnLineUser(long int dwUserID);
	///�����û�
	CGameUserInfo * FindOnLineUser(char * szName);
	///�����û�
	CGameUserInfo * FindNetCutUser(long int dwUserID);
	///�����û�
	CGameUserInfo * FindNetCutUser(char * szName);
	///��ȡ��������
	UINT GetOnLineUserCount() { return m_uOnLineCount; }
	///��ȡ��������
	UINT GetNetCutUserCount() { return m_NetCutUserInfo.GetActiveCount(); }
	///��ȡȫ������
	UINT GetGameRoomCount() { return m_uOnLineCount+m_NetCutUserInfo.GetActiveCount(); }
};

/*******************************************************************************************************/

#endif
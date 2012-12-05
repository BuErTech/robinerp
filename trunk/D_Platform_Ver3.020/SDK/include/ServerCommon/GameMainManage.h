#pragma once

#ifndef GAMEMAINMANAGE_H_FILE
#define GAMEMAINMANAGE_H_FILE

#include "Afxtempl.h"
#include "MainManage.h"
#include "GameDesk.h"
#include "UserInfo.h"
#include "AFCSocket.h"
#include "AFCDataBase.h"
#include "GameRoomLogonDT.h"
///#include "AFCItemData.h"
#include "PropService.h"

///{{ Added by zxd 20100727  GM����������ͷ�ļ�
#include "GM_MessageDefine.h"
#include "InterfaceForGM.h"
/// Added by zxd 20100727}}


#include "BankService.h"
//#include "GameUserInfoManage.h"

#include "CmdGameLogic.h"

#include "IAutoAllotDesk.h"

using namespace std;

///��˵��
class CGameDesk;
class CBaseMainManage;
class CGameUserInfoManage;

///ģʽ��ֵ����
#define ACCEPT_THREAD_COUNT				1									///Ӧ���߳���Ŀ
#define SOCKET_THREAD_COUNT				4									///�����߳���Ŀ

///�ṹ˵��
struct MSG_GR_SR_MatchUser;
struct DL_GR_I_RegisterLogon;
struct DL_GR_O_RegisterLogon;

///�궨��
#define TIME_SPACE						50L									///��Ϸ ID ���
#define TIME_START_ID					100L								///��ʱ����ʼ ID

/*******************************************************************************************************/

///��Ϸ���ݿ⴦��
class AFX_EXT_CLASS CGameDataBaseHandle : public CDataBaseHandle
{
public:
	CGameDataBaseHandle(){m_ErrorSQLCount = 0;};
	///��������
private:
	///���ݿ⴦��ӿ�
	virtual UINT HandleDataBase(DataBaseLineHead * pSourceData);
	///������
private:
	///������½����
	UINT OnRoomLogon(DataBaseLineHead * pSourceData);
	///�û��뿪����
	UINT OnUserLeft(DataBaseLineHead * pSourceData);
	///���·�������Ϣ
	UINT OnUpdateServerInfo(DataBaseLineHead * pSourceData);
	///��¼��Ϸ��Ϣ
	UINT OnRecordGameInfo(DataBaseLineHead * pSourceData);
	///������ݿ�����
	UINT OnCheckDataConnect(DataBaseLineHead * pSourceData);
	///����Ƿ������
	UINT OnTimeServiceControl(DataBaseLineHead * pSourceData);
	///����GM����
	UINT OnCallGM(DataBaseLineHead * pSourceData);
	///����Ȩ���޸�
	UINT OnGamePower(DataBaseLineHead * pSourceData);
	///����Ա���˶�����¼
	UINT OnGMKickUserRecord(DataBaseLineHead * pSourceData);
	///����Ա��������Ϣ��¼
	UINT OnGMWarningUserRecord(DataBaseLineHead * pSourceData);
	///��Ǯ��
	UINT OnOpenWallet(DataBaseLineHead *pSourceData, UINT uCode = DTK_GR_DEMAND_MONEY);
	///ȡ�����
	UINT OnCheckOutMoney(DataBaseLineHead *pSourceData, UINT uCode= DTK_GR_CHECKOUT_MONEY);
	///������
	UINT OnCheckInMoney(DataBaseLineHead *pSourceData, UINT uCode= DTK_GR_CHECKIN_MONEY);
	///ת��
	UINT OnTransferMoney(DataBaseLineHead *pSourceData, UINT uCode= DTK_GR_TRANSFER_MONEY);
	///�޸�����
	UINT OnChangePasswd(DataBaseLineHead *pSourceData, UINT uCode= DTK_GR_CHANGE_PASSWD);
	///ת�ʼ�¼
	UINT OnTransferRecord(DataBaseLineHead *pSourceData, UINT uCode= DTK_GR_TRANSFER_RECORD);
	///�����û���Ϣ
	UINT OnUpdateUserResult(DataBaseLineHead * pSourceData);
	///һ���Ը���ͬ�������û���Ϣ
	UINT OnUpdateAllUserResult(DataBaseLineHead * pSourceData);

	///�����û�������Ϣ
	UINT OnUpdateContestInfo(DataBaseLineHead * pSourceData);

	///�õ��û�������Ϣ
	UINT OnGetContestInfo(DataBaseLineHead * pSourceData);

	///��ʱ���·������
	UINT OnModifyUserMoney(DataBaseLineHead * pSourceData);
	///��ʱ�޸��������ֵ
	UINT OnUpdateUserCharm(DataBaseLineHead * pSourceData);
	///ƽ̨��������
	///UINT OnGetUserHoldPlatformItemCount(DataBaseLineHead *pSourceData);
	///ʹ�õ���
	UINT OnUserUseOneProp(DataBaseLineHead *pSourceData);
	UINT OnUseSmallBoardcast(DataBaseLineHead * pSourceData);
	///ͨ�����մ����ͽ����û��ͽ��
	UINT OnSendUserMoenyByTimes(DataBaseLineHead *pSourceData);
	///��ȡ��������б�

	///UINT OnGetBuyItemData(DataBaseLineHead *pSourceData);
	///�������
	///UINT OnBuyItem(DataBaseLineHead *pSourceData);

	///UINT BufferIMMessage(DataBaseLineHead *pSourceData);

	///UINT OnFriendManage(DataBaseLineHead *pSourceData);
	///
	///20090203 Kylin ��ֹ�������ݿ���û���½ʧ��
	///ɾ����½��Ϣ
	bool UnRegisterUserLogon(long int dwUserID, bool bAllLogonInfo);
	///�����û�����
	UINT OnUnRegisterOnLine(DataBaseLineHead * pSourceData);
	///
	
	///wushuqun 2009.5.22
	///�޸Ľ�ֹ��¼���⣬�޸Ĳ���TWLoginRecord ��¼ʱ��
	///ִ��ʱ������¼�����������Ϸ�����б�ִ��
    UINT OnWriteTWLoginRecord(DataBaseLineHead * pSourceData);
	///wushuqun 2009.6.6
	///��ʱ��������
	UINT OnAloneDeskInTime(DataBaseLineHead * pSourceData);

	///��������

	UINT OnClearNetCutUserOnlineFlag(DataBaseLineHead * pSourceData);	///2009-4-17��� zxj

	UINT OnReWriteWLoginRecord(DataBaseLineHead * pSourceData);		///2009-4-28��� zxj

	UINT OnContestUserLeft(DataBaseLineHead * pSourceData);		///2009-4-28��� zxj
	/// ��¼�޸ķ�������
	UINT OnRoomPWChangeRecord(DataBaseLineHead * pSourceData);

    // PengJiLin, 2010-8-23, �����û�ID��ȡ�ǳ�
    UINT OnGetNickNameOnID(DataBaseLineHead * pSourceData);

    // PengJiLin, 2010-9-10, ʹ�����˿������߿�����
    UINT OnUseKickProp(DataBaseLineHead * pSourceData, BOOL bIsKickUser);

    // PengJiLin, 2010-9-13, ʹ�����˿����˹���
    UINT OnNewKickUserProp(DataBaseLineHead * pSourceData);


	UINT OnBankOperateNormal(DataBaseLineHead * pSourceData);

	UINT OnBankOperateTransfer(DataBaseLineHead * pSourceData);

	UINT OnBankOperateDeposit(DataBaseLineHead * pSourceData);

	UINT OnBankGetWallet(DataBaseLineHead * pSourceData);

	UINT OnBankQueryDeposits(DataBaseLineHead * pSourceData);

	HANDLE sqlSPSetNameEx(HANDLE & hHandle,LPCTSTR szSPName,bool bReturnValue = FALSE);
	int                 m_ErrorSQLCount;


};

/*******************************************************************************************************/

///������Ϣ�ṹ
struct MatchInfoStruct
{
	long int									dwUserID;							///�û� ID
	TCHAR										szUserNname[61];					///�û�����
	UINT										uPlayCount;							///���ƾ���
	long int									dwPlayPoint;						///���Ʒ���
};

///����������Ϣ
struct LimitTalkStruct
{
	long int									dwUserID;							///�û� ID
	long int									dwLimitedTime;						///����ʱ��
};

/*******************************************************************************************************/

///�������ݹ�����
class AFX_EXT_CLASS CGameMainManage : public CBaseMainManage
{
	friend class CGameDesk;

	///��������
protected:
	UINT										m_uDeskCount;						///��Ϸ����Ŀ
	CGameDesk									* * m_pDesk;						///��Ϸ��ָ��
	CGameDesk									* m_pDeskArray;						///��Ϸ��ָ��

	///״̬����
protected:
	bool										m_bStopLogon;						///ֹͣ��½

	///������Ϣ
private:
	UINT										m_uMatchUserCount;					///��������
	MatchInfoStruct								* m_pMatchInfo;						///������Ϣ
	MSG_GR_SR_MatchUser							* m_pMatchUser;						///�����û�

	///����������Ϣ
private:
	CArray<LimitTalkStruct,LimitTalkStruct>		m_RoomLimited;						///��������
	CArray<LimitTalkStruct,LimitTalkStruct>		m_GameLimited;						///��Ϸ����

	///��Ϣ����
protected:
	TCHAR										m_szMsgRoomLogon[500];				///������Ϣ
	TCHAR										m_szMsgGameLogon[500];				///��Ϸ��Ϣ
	TCHAR                                       m_szVIPPassWord[50];                ///< VIP��������
	long                                        m_lVIPID;                           ///< �������һ�����������VIP ID
	UINT										m_uVIPIndex;                        ///< �������һ�����������VIP���׽�������
	BOOL										m_bIsVIPLogon;                      ///< �Ƿ����������VIP�ط����뷿��

public:
	CGameUserInfoManage							m_UserManage;						///�û�����
	BYTE                                        m_GameFinishCount;                  ///��Ϸ��������
	list<CGameUserInfo*>						m_pGameUserInfoList;				///<�Ŷӻ�list
	int											m_nRate; ///< ˰�ʵķ�ĸ.100���ʾ�ٷּ���1000��ǧ�ּ��������� m_pDataManage-m_InitData.uTax
	bool                                        m_bPresentCoin; ///< �Ƿ����ͽ�ҡ�20�ѣ�ÿ����ҵ�������ֵʱ�������ɽ�ҡ�
private:	
	///	CAFCItemData								*m_ItemDate;
	CBankService								*m_pBankService;
	CPropService								*m_PropService;
	

    UINT                m_uQueuerMaxPeople;      // PengJiLin, 2010-5-20, �Ŷӻ�������
	int					m_uQueuerPeople;
	int					m_iQueueTime;
	
	IAutoAllotDesk*		m_pIAutoAllotDesk;	///< �Զ������������ӿ�ָ�룬�������OnInit��������Ϊ�գ���ʾ����Ҫ��������{{By zxd on 20101129}}
	BYTE				*m_pQueueUserSitBuffer;	///< �Զ������������ʱ���͵ļ������һ�����µĻ�����������ÿ�ζ�new����

	
	///��������
public:
	///���캯��
	CGameMainManage(void);
	///��������
	virtual ~CGameMainManage(void);
	CGameMainManage(CGameMainManage&);
	CGameMainManage& operator=(CGameMainManage&); 

	///�ж��Ƿ����Ŷӻ�����
	///@return true:���Ŷӻ����䣻false�������Ŷӻ�����
	bool IsQueueGameRoom();


		/// �����Զ���������
	virtual IAutoAllotDesk *CreateAutoAllotDesk(int iDeskPeopleCount, int iDeskCount);
		   ///����ӿں��� �����̵߳��ã�
	///�Ŷӻ�����û�
	virtual int AutoAllotDeskAddUser(CGameUserInfo *pInfo);
	///�Ŷӻ�ɾ���û�
	virtual int AutoAllotDeskDeleteUser(CGameUserInfo *pInfo);

	void NewQueueGame();
private:
	///���ݹ���ģ���ʼ��
	virtual bool OnInit(ManageInfoStruct * pInitData, KernelInfoStruct * pKernelData);
	///���ݹ���ģ��ж��
	virtual bool OnUnInit();
	///���ݹ���ģ������
	virtual bool OnStart();
	///���ݹ���ģ��ر�
	virtual bool OnStop();
	
	///������չ�ӿں��� ���������̵߳��ã�
private:
	///SOCKET ���ݶ�ȡ
	virtual bool OnSocketRead(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	///SOCKET �ر�
	virtual bool OnSocketClose(ULONG uAccessIP, UINT uSocketIndex, long int lConnectTime);
	///���ݿ⴦����
	virtual bool OnDataBaseResult(DataBaseResultLine * pResultData);
	///��ʱ����Ϣ
	virtual bool OnTimerMessage(UINT uTimerID);
	///���紦����չ

public:
	/// ת���û�ID�Ƚ�
	/// @param UserID ����Լ���ID��UserIDDestĿ��ID,ɢ��ID NomalIDFrom��ʼ��ɢ��ID NomalIDEnd����
	/// @return  0����ת�ˣ�1 ����ת��,��˰��2 ����ת��,����˰.3 ����ת�˶�Ϊ���ر�ID�����ת�˽��
	static int TransferIDCompare(long UserID,long UserIDDest,long NomalIDFrom,long NomalIDEnd);
	/// ��ȡ���ID��Ϣ���Ƿ�ɢ�����ر��˺ţ�ָ���˺�
	/// @param UserID ���ID,NomalIDFromɢ��ID��ʼ��NomalIDEndɢ��ID����
	/// @return 0��8��ָ���˺ź����0�ĸ�����-1Ϊɢ����-2Ϊ��Ч�˺ţ�-5Ϊ5���ţ�-6Ϊ6����
	static int GetIDInformation(long UserID,long NomalIDFrom,long NomalIDEnd);
	/// ��ȡĳ����Ϸ����ָ��
	const CGameDesk* GetGameDesk(BYTE bDeskNo);

private:
	//{{ Added by zxd 20100806
	/// GM��Ϣ����ģ��
	bool OnGMSocketMessage(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	// Added by zxd 20100806}}

	///�û���½���� 
	bool OnUserLogonMessage(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	///�û���������
	bool OnUserActionMessage(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	///�û����䴦��
	bool OnRoomMessage(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	///������Ϣ����
	bool OnManageMessage(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	///Ǯ���˴���
	bool OnAboutMoney(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);

	/// ��ӡ����������ҵ���Ϣ
	/// Added By JianGuankun 2011.12.5
	void PrintUsersLog();
	static bool Writeline(CFile* p,CString str);

	///����д���
	bool OnPointMessage(NetMessageHead * pNetHead, void * pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	///�����ӹ���2008.3.5
	///С����
	///bool OnBroadcastMessage(NetMessageHead *pNetHead, void* pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	///����
	bool OnFriendManageMsg(NetMessageHead *pNetHead, void* pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	///�ŶӴ���
	bool OnQueueMsg(NetMessageHead *pNetHead, void* pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);
	
	///�Ŷӻ�����
	void queuerGame();

	///������Ϣ2008.3.20

	bool IsContestGameOver();


	///���ѹ�����
	bool OnFriendManageDBOperateResult(DataBaseResultLine *pResultData);
	///����������
	bool OnBufferIMMessageDBOperateResult(DataBaseResultLine *pResultData);
	///���ݿ⴦����չ

public:
	//����ʱ�䵽��
	bool IsLeaseTimeOver();



private:
	///�����½����
	bool OnRoomLogonResult(DataBaseResultLine * pResultData);
	///������Ӵ���
	bool OnCheckConnect(DataBaseResultLine * pResultData);
	///�������ܽ������
	bool OnCallGMResult(DataBaseResultLine * pResultData);
	///����Ȩ�����ý������
	bool OnGamePowerSetResult(DataBaseResultLine * pResultData);
	///Ǯ��
	bool OnAboutMoneyResult(DataBaseResultLine * pResultData, UINT uCode=ASS_GR_OPEN_WALLET);
	///���䣭��Ǯ��
	bool OnCheckInMoney(DataBaseResultLine * pResultData, UINT uCode=DTK_GR_CHECKIN_MONEY);
	///���䣼��Ǯ��
	bool OnCheckOutMoney(DataBaseResultLine * pResultData, UINT uCode=DTK_GR_CHECKOUT_MONEY);
	///ת��
	bool OnTransferMoney(DataBaseResultLine * pResultData, UINT uCode=DTK_GR_TRANSFER_MONEY);
	///�޸�������
	bool OnChangePasswd(DataBaseResultLine * pResultData, UINT uCode=DTK_GR_CHANGE_PASSWD);
	///ת�ʼ�¼���
	bool OnTransferRecord(DataBaseResultLine * pResultData, UINT uCode=DTK_GR_TRANSFER_RECORD);
	///���ͽ�ҽ����add by yjj 080703
	bool SendUserMoenyByTimesResult(DataBaseResultLine * pResultData);
	
	///wushuqun 2009.6.5
	///��ս�����¼�������
	bool OnBattleRoomRecord(DataBaseResultLine * pResultData);
	///

    // PengJiLin, 2010-8-23, �����û�ID��ȡ�ǳ�
    bool OnGetNickNameOnIDResult(DataBaseResultLine * pResultData);

    // PengJiLin, 2010-9-10, ���˿������߿�����
    bool OnUseKickPropResult(DataBaseResultLine * pResultData, BOOL bIsKickUser);

    // PengJiLin, 2010-9-13, ���˿����˹���
    bool OnNewKickUserProp(NetMessageHead *pNetHead, void* pData, UINT uSize, ULONG uAccessIP, UINT uIndex, DWORD dwHandleID);

    // PengJiLin, 2010-9-13, ʹ�����˿�����
    bool OnNewKickUserPropResult(DataBaseResultLine * pResultData);

    // PengJiLin, 2011-4-15, ����ʱ���������ͽ��
    bool OnSendMoneyOnTimesAndCounts(DataBaseResultLine * pResultData);

	//JianGuankun,2012.7.16��ȡ������ǰ�������ɼ���
	bool OnGetContestRankResult(DataBaseResultLine * pResultData);

	///�ڲ�����
private:
	///�뿪���Ӻ���
	bool UserLeftDesk(CGameUserInfo * pUserInfo);
	///�û��Ƿ���Ϸ
	bool IsUserPlayGame(CGameUserInfo * pUserInfo);
	///�û��Ƿ���Ϸ
	bool IsUserPlayGameByStation(CGameUserInfo * pUserInfo);
	///�����û�����
	bool CleanUserInfo(CGameUserInfo * pUserInfo);
	///�Ƿ���������
	bool IsLimitedTalk(CGameUserInfo * pUserInfo, bool bRoom);
	///���ͷ�����Ϣ

	/// �����û���Ϣ
	/// @param pUserInfo�û���Ϣ,uSocketIndex�׽�������,pLogonResult��½��Ϣ
	/// @return �ɹ�Ϊtrue
	bool SendRoomInfo(CGameUserInfo * pUserInfo, UINT uSocketIndex, DWORD dwHandleID,DL_GR_O_LogonResult * pLogonResult);

	///���ͱ�����Ϣ
	bool SendMatchInfo(UINT uSocketIndex, DWORD dwHandleID);
	///���ͷ�����Ϣ
	bool SendRoomMessage(UINT uSocketIndex, DWORD dwHandleID, TCHAR * szMessage, BYTE bShowStation=SHS_TALK_MESSAGE, BYTE bFontSize=0, BYTE bCloseFace=FALSE);
	///������Ϸ��Ϣ
	bool SendGameMessage(UINT uSocketIndex, DWORD dwHandleID, TCHAR * szMessage, BYTE bShowStation=SHS_TALK_MESSAGE, BYTE bFontSize=0, BYTE bCloseFace=FALSE);

	///�ڲ�����
private:
	///��ʼ����Ϸ��
	bool InitGameDesk(UINT uDeskCount, UINT	uDeskType);///�ټ��� UINT uDeskType add by wlr 20090714 
	///��ȡ��Ϸ������Ϣ
	virtual CGameDesk * CreateDeskObject(UINT uInitDeskCount, UINT & uDeskClassSize)=0;
public:
	virtual CGameDesk * GetDeskObject(BYTE bIndex);

	HANDLE sqlSPSetNameEx(HANDLE & hHandle,LPCTSTR szSPName,bool bReturnValue = FALSE);

};

typedef CGameUserInfo *PGameUserInfo; 

class compare
{       
public:       
    bool operator ()(PGameUserInfo& t1,  PGameUserInfo& t2) const       
    {       
		return t1->GetWeigth() > t2->GetWeigth();  
    }       
};  


/*******************************************************************************************************/

///��Ϸ���ݹ�����ģ��
template <class GameDeskClass, UINT uBasePoint, UINT uLessPointTimes> class CGameMainManageTemplate : public CGameMainManage
{
	///��������
public:
	///���캯��
	CGameMainManageTemplate(void) {};
	///��������
	virtual ~CGameMainManageTemplate(void) {};

	///���غ���
private:
	///��ȡ��Ϣ���� ���������أ�
	virtual bool PreInitParameter(ManageInfoStruct * pInitData, KernelInfoStruct * pKernelData)
	{
		///���ð汾��Ϣ
		pKernelData->bMaxVer=GAME_MAX_VER;
		pKernelData->bLessVer=GAME_LESS_VER;

		///����ʹ������
		pKernelData->bStartTCPSocket=TRUE;

		///�������ݿ���Ϣ
		pKernelData->bLogonDataBase=TRUE;
		pKernelData->bNativeDataBase=TRUE;
		pKernelData->bStartSQLDataBase=TRUE;

		///�����߳���Ŀ
		pKernelData->uAcceptThreadCount=ACCEPT_THREAD_COUNT;
		pKernelData->uSocketThreadCount=SOCKET_THREAD_COUNT;

		///������Ϸ��Ϣ
		pKernelData->uNameID=NAME_ID;
		pKernelData->uDeskPeople=PLAY_COUNT;

		CString _NameID;
		_NameID.Format("%d", pKernelData->uNameID);
		CBcfFile fConfig(CBcfFile::GetAppPath()+"SpecialRule.bcf");
		pKernelData->uMinDeskPeople = pKernelData->uDeskPeople;
		//pKernelData->uMinDeskPeople = fConfig.GetKeyVal("MinDeskPeople", _NameID, pKernelData->uDeskPeople);


		///��������
		if ((pInitData->uDeskCount*PLAY_COUNT+50)<pInitData->uMaxPeople)
		{
			pInitData->uMaxPeople=pInitData->uDeskCount*PLAY_COUNT+50;
		}

		///���ڱ�����
		if (lstrcmp(pInitData->szLockTable,TEXT("N/A"))==0) pInitData->szLockTable[0]=0;
		if (lstrcmp(pInitData->szIPRuleTable,TEXT("N/A"))==0) pInitData->szIPRuleTable[0]=0;
		if (lstrcmp(pInitData->szNameRuleTable,TEXT("N/A"))==0) pInitData->szNameRuleTable[0]=0;
		CString msg;
		msg.Format("%d.dll",NAME_ID);

		if (lstrcmp(pInitData->szGameTable,TEXT("N/A"))==0) lstrcpy(pInitData->szGameTable,msg);

		if ((pInitData->uComType==TY_MATCH_GAME)&&(pInitData->szLockTable[0]==0)) 
		{
			AfxMessageBox("�������䣬����û������һ����ComLogonMoney");
			return false;
		}

		///��ҳ������¼��־
		if (pInitData->uComType==TY_MONEY_GAME)
		{
			pInitData->dwRoomRule|=GRR_RECORD_GAME;
		}

		///���ڻ�����
		if (pInitData->uBasePoint==0) pInitData->uBasePoint=uBasePoint;///ͳһΪ1
		if ((pInitData->uComType==TY_MATCH_GAME)||(pInitData->uComType==TY_MONEY_GAME))
		{
			if (pInitData->uLessPoint<pInitData->uBasePoint)///*uLessPointTimes)///��С�볡��Ϊ������
				pInitData->uLessPoint=pInitData->uBasePoint;///*uLessPointTimes;
			
            // PengJiLin, 2011-7-15, ��Ϊ����ѵ����ÿ��Ժܴ󣬲������ޡ�
            if(pInitData->uTax<0)
				pInitData->uTax=0;

		}

		return true;
	};

	///��ȡ��Ϸ������Ϣ
	virtual CGameDesk * CreateDeskObject(UINT uInitDeskCount, UINT & uDeskClassSize)
	{
		uDeskClassSize=sizeof(GameDeskClass);
		return new GameDeskClass [uInitDeskCount];
	};
};

/*******************************************************************************************************/

///��Ϸģ����ģ��
template <class GameDeskClass, UINT uBasePoint, UINT uLessPointTimes> class CGameModuleTemplate : public IModuleManageService
{
	///��������
public:
	CGameDataBaseHandle														m_DataBaseHandle;	///���ݿ⴦��ģ��
	CGameMainManageTemplate<GameDeskClass,uBasePoint,uLessPointTimes>		m_GameMainManage;	///��Ϸ���ݹ���

	///��������
public:
	///���캯��
	CGameModuleTemplate(void) {};
	///��������
	virtual ~CGameModuleTemplate(void) {};

	///�ӿں���
public:
	///��ʼ������ 
	virtual bool InitService(ManageInfoStruct * pInitData)
	{
		KernelInfoStruct KernelData;
		memset(&KernelData,0,sizeof(KernelData));
		return m_GameMainManage.Init(pInitData,&m_DataBaseHandle);
	}
	///ж�غ��� 
	virtual bool UnInitService() { return m_GameMainManage.UnInit(); }
	///��ʼ���� 
	virtual bool StartService(UINT &errCode) { return m_GameMainManage.Start(); }
	///ֹͣ���� 
	virtual bool StoptService() { return m_GameMainManage.Stop(); }
	///ɾ������
	virtual bool DeleteService() { delete this; return true; }

};

/*******************************************************************************************************/

#endif
#pragma once

#ifndef GAMEDESK_H_FILE
#define GAMEDESK_H_FILE

#include "UserInfo.h"
#include "AFCSocket.h"
#include "GameMainManage.h"
#include "../common/PropMessage.h"
///#include "GameRoomLogonDT.h"

///��Ϸ������־
#define GFF_FORCE_FINISH			0										///< ǿ�н��
#define GFF_SAFE_FINISH				1										///< �����Ϸ

///��ʼģʽ
#define FULL_BEGIN					0x00									///< ���˲ſ�ʼ
#define ALL_ARGEE					0x01									///< ������ͬ��Ϳ�ʼ
#define SYMMETRY_ARGEE				0x02									///< �Գ�ͬ�⿪ʼ
#define FOUR_SIX_ARGEE				0x03									///< 6����λ��ֻҪ��4����ͬ�⿪ʼ


///��˵��
class CGameMainManage;
struct MatchInfoStruct;
struct UpdateUserInfo_t;

/// CGameDesk�࣬��Ϸ����
/// �����࣬��Ϸ�����������ɱ�������
/// �ṩ��Ϸ����Ҫ�õ��Ĺ������ݿ���û����ݲ����Ľӿ�
/// 
class AFX_EXT_CLASS CGameDesk
{
	///������Ϣ
private:
	bool							m_bLock;								///< �Ƿ�����
	UINT							m_uDeskType;							///< ��Ϸ�����ͣ���ҪΪ��Ӱټ��� add by wlr 20090714
	bool                            m_bIsVirtualLock;                       ///< �Ƿ�Ϊ��������  yjj 090325
	char							m_szLockPass[61];						///< ��������
	long int						m_dwOwnerUserID;						///< ̨�� ID
	long							m_dwZhuangjiaUserID;					///< �ټ�������Ϸ ׯ��ID  add by lxl 2010-11-5
 	
	bool							m_bConnect[MAX_PEOPLE];					///< �Ƿ�����

	///��Ϸ��Ϣ
private:
	bool							m_bPlayGame;							///< ��Ϸ�Ƿ�ʼ��־
	BYTE							m_bEnableWatch;							///< �Թ۱�־
	long int						m_dwBeginTime;							///< ��ʼʱ��
	MatchInfoStruct					* m_pMatchInfo[MAX_PEOPLE];				///< ������Ϣ

	///��¼��Ϣ
private:
	long int						m_dwTax;								///< ÿ����ҽ�˰
	long int						m_dwGameUserID[MAX_PEOPLE];				///< ��ұ�ʶ
	int								m_dwScrPoint[MAX_PEOPLE];				///< ԭ������
	__int64							m_dwChangePoint[MAX_PEOPLE];			///< ���ķ���
	__int64							m_dwChangeMoney[MAX_PEOPLE];			///< ���Ľ��
	__int64							m_dwTaxCom[MAX_PEOPLE];					///< ����˰Ŀ
	BYTE							m_byDoublePointEffect;					///< ˫�����Ƿ���Ч������λ�㣩20081125

	///������Ϣ
private:
	bool							m_bCutGame[MAX_PEOPLE];					///< �Ƿ����
	BYTE							m_bBreakCount[MAX_PEOPLE];				///< ���ߴ���
	long int						m_uCutTime[MAX_PEOPLE];					///< ����ʱ��

	///��������
protected:
	BYTE							m_bWaitTime;							///< �ȴ�ʱ��
	BYTE							m_bGameStation;							///< ��Ϸ״̬
	///BYTE                            m_bGameFinishCount;                     ///< game over count
	CRITICAL_SECTION				m_csLock_;								///< �ؼ���///�ټ��� add by wlr 20090714

	///ֻ������
public:
	
	BYTE							m_bDeskIndex;							///< ���Ӻ���
	BYTE							m_bMaxPeople;							///< ��Ϸ����
	const BYTE						m_bBeginMode;							///< ͬ��ģʽ
	CPtrArray						m_WatchUserPtr;							///< �Թ��û���Ϣָ��
	CGameUserInfo					* m_pUserInfo[MAX_PEOPLE];				///< �û���Ϣָ�룬������������һ������ҵ���λ������
	CGameMainManage					* m_pDataManage;						///< ���ݹ���ָ��
	int								m_DeskBasePoint;						///< ̨�ӱ�����̨���趨��
	///��������

public:
	CTime							m_tBeginTime;							///��Ϸ��ʼ��ʱ��
	CTime							m_tEndTime;								///��Ϸ������ʱ��

public:
	///���캯��
	explicit CGameDesk(BYTE bBeginMode);
	///��������
	virtual ~CGameDesk(void);

	///���ܺ���
public:
	///��ʼ������
	///�ټ���UINT uDeskType add by wlr 20090714
	bool Init(BYTE bDeskIndex, BYTE bMaxPeople, CGameMainManage * pDataManage, UINT	uDeskType);
	///��ʼ����Ϸ�߼�
	virtual bool InitDeskGameStation(){return true;};
	///���ñ�����Ϣ
	bool SetMatchInfo(BYTE bDeskStation, MatchInfoStruct * pMatchInfo, bool bNotify);
	///�û������뿪
	virtual bool UserNetCut(BYTE bDeskStation, CGameUserInfo * pLostUserInfo);
	///�û���������
	virtual bool UserReCome(BYTE bDeskStation, CGameUserInfo * pNewUserInfo);
	///�ټ��� add by wlr 20090714
	BYTE FindStation(CGameUserInfo * pUserInfo);
	///�ڲ����� 
	
	///�ټ�������Ϸ add by lxl 2010-11-5
	///�ж��Ƿ��ǰټ�������Ϸ
	bool IsBJLGameType() const {return m_uDeskType == DESK_TYPE_BJL;};
	///���õ�ǰ���ӵ�ׯ��ID 
	void SetCurrentZhuangjia(long dwUserID) {m_dwZhuangjiaUserID = dwUserID;};
	///�ж�����Ƿ���ׯ�� 
	bool IsZhuangjia(long dwUserID) const {return m_dwZhuangjiaUserID == dwUserID && dwUserID != 0L;};

private:
	///�����������
	bool CleanCutGameInfo();
protected:
	///����ָ���������
	bool CleanCutGameInfo(BYTE bDeskStation);
protected:
	///��¼��Ϸ��Ϣ
	bool RecoderGameInfo(__int64 *ChangeMoney);

	///�õ�˫�����ֿ���Ч��Ϣ
	BYTE GetDoublePointEffect(){return m_byDoublePointEffect;};

	///���ߺ���
public:
	///�Ƿ�����
	bool IsLock() { return m_bLock; };
	///����
	void LockDesk() { m_bLock=true;};
	///����
	void UnlockDesk() { m_bLock=false;};

	///������������  yjj 090325
	bool IsVirtualLock() {return m_bIsVirtualLock;}
	void VirtualLockDesk() {m_bIsVirtualLock = true;}
	void UnVirtualLockDesk(){m_bIsVirtualLock = false;}


	///�Ƿ������Թ�
	
	bool IsEnableWatch(BYTE bDeskStation);
	///���ö�ʱ��
	bool SetTimer(UINT uTimerID, int uElapse);
	///�õ����ӻ�����
	int GetTableBasePoint() { return m_DeskBasePoint;};
	///
	void SetTableBasePoint(int BasePoint) {m_DeskBasePoint=BasePoint;};
	///̨��
	long GetTableOwnerID(){return m_dwOwnerUserID;};
	///ɾ����ʱ��
	bool KillTimer(UINT uTimerID);
	///��ȡ��Ϸʱ��
	long int GetPlayTimeCount();
	/// �޸��û�������Ǯ��
	/// �ú�����Ҫ��ɵĹ���
	/// �������������������û�ʵ����Ӯ�Ľ�������������������m_dwChangeMoney��Ա�У�ͬʱд�����ݿ�
	/// @param[in] dwPoint �û��������飬�����СΪm_bMaxPeople������֧�ֵ�����������ɵ����߱�֤���鲻Խ��
	/// @param[in] bCut �û��Ƿ�������飬�����СΪm_bMaxPeople������֧�ֵ�����������ɵ����߱�֤���鲻Խ��
	/// @param[in] nTaxIndex -1��ʾ��������ҵ�˰������ֻ�۸��û���Ǯ
	/// �ú������޸�m_dwChangeMoney[]�����Ա��ֵ
	/// ��Ϸ���������������˺����ģ���ͨ��RecordGameInfo������ȡ��ֵ
	bool ChangeUserPointint64(__int64 *arPoint, bool *bCut, int nTaxIndex = -1);

	bool ChangeUserPointContest(__int64 *arPoint, bool *bCut, int nTaxIndex = -1);

	bool ChangeUserPointContest(int *arPoint, bool *bCut, int nTaxIndex = -1);

	bool ChangeUserPoint(int *arPoint, bool *bCut, int nTaxIndex = -1);
	/// �޸��û���������ȥ�Ľ��㺯�������º�����Ч����������������
	bool ChangeUserPoint_old(int *dwPoint, bool *Cut,int *dwMoney=NULL,bool IsCheckInMoney=false);

	///������Ϸ״̬
	bool SendGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser, void * pStationData, UINT uSize);
	///�������ݺ��� ��������Ϣ����Ϸ�ߣ�
	bool SendGameData(BYTE bDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);
	///�����Թ����� ��������Ϣ���Թ��ߣ�
	bool SendWatchData(BYTE bDeskStation, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);
	///�������ݺ��� ��������Ϣ����Ϸ�ߣ�
	bool SendGameData(BYTE bDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);
	///�����Թ����� ��������Ϣ���Թ��ߣ�
	bool SendWatchData(BYTE bDeskStation, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);
	///�����Թ����� ��������Ϣ���Թ��ߣ�
	bool SendWatchDataByID(UINT uSendSocketID, void * pData, UINT uSize, BYTE bMainID, BYTE bAssID, BYTE bHandleCode);
	///�û�����
	int GetOrderClass(int dwPoint);
	///д���������ݿ�
	void UpdateUserInfo(long dwUserID,int Point,int Money, int taxCom,bool bcut,long playTime,long logonTime);
	/// ����������ж�����,Ϊ�Ŷӻ����
	int GetDeskPlayerNum();
	/// ��������е�һ��û���˵���λ����,Ϊ�Ŷӻ����
	int GetDeskNoPlayerStation();
	/// �жϴ�������ҵ�ip�Ƿ�����ͬ��,Ϊ�Ŷӻ����
	bool IsDeskIpSame(CGameUserInfo * pUserInfo);
	/// ���������е������Ϣ,Ϊ�Ŷӻ����
	void SetDeskUserInfo(CGameUserInfo * pUserInfo, int iDeskStation);
	/// ������Ϸ��,20��
	/// param void
	/// return void
	void PresentCoin();

	///�������غ���
public:
	///�Ƿ���Կ�ʼ��Ϸ
	virtual bool CanBeginGame();
	///�Ƿ���Զ���
	virtual bool CanNetCut(BYTE bDeskStation);
	///�û�������Ϸ��
	virtual BYTE UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo);
	///�û��뿪��Ϸ��
	virtual BYTE UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);
	///�Թ��û�������Ϸ��
	virtual BYTE WatchUserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo);
	///�Թ��û��뿪��Ϸ��
	virtual BYTE WatchUserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo);
	///�û�ͬ�⿪ʼ
	virtual bool UserAgreeGame(BYTE bDeskStation);
	///�����Ϣ������
	virtual bool HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);
	///�Թ۲����仯
	virtual bool OnWatchSetChange(BYTE bDeskStation, long int dwTargetUserID, bool bEnableWatch) { return true; }
public:
	/// ������ҵĽ�Ǯ(��Ҫ����ָ����)��ʹ�������� pp 2010-8-26
	/// @param pbDeskStationΪҪ�ı���֣����ң�����λ����
	/// @param arPointΪҪ�ı�Ļ��֣����ң�������
	/// @param nCountΪҪ�ı���ֵ�����
	void UpdateUserPoint(BYTE *pbDeskStation,__int64 *arPoint,int nCount);              
	/// ����������ҵ����ݿ�����
	/// zxd �� 20100125����
	/// �˺���ֻ����һ�δ洢���̣��Ӷ�����������ҵ����ݣ�������һ��һ����ҵ���
	/// @param pUpdateUserInfo ʵʱnew���Ķ���û�����ָ��
	/// @param nTotalPlayerCount ���θ�����ҵĸ���
	/// @return �޷���ֵ
	void UpdateAllUserInfo( UpdateUserInfo_t *pUpdateUserInfo, int nCount, int iroundtime = 0);
	/// ǿ��������ߺ���
	/// �����Ƿ�����Ϸ�У���ǿ��������ߣ������������д����������ߺ����Ϸ����
	/// ��ձ������еĸ������Ϣ�����ý����󣬸�����뿪�����䣬ͬʱm_pUserInfo[bDeskStation]�ᱻ���NULL
	/// ��������UserLeftDesk���������ڣ��������Ƿ���������������ϵĽ�����������ڷ��������жϵ�����������������˳����䡣��
	/// UserLeftDesk���������ɿͻ��˷���������Ϣ��������������ƽ̨���õĺ�����������Ϸ��������Ĺ�������Ӧ����������Ϸ
	/// ����״̬����һ����������˳�������Ϸ�򷿼�
	/// 
	/// @param bDeskStation ����������е�λ�ú�
	/// @return ���bDeskStation���ڱ�����һ�������������m_pUserInfo[bDeskStation]�Ѿ��ǿ�ֵNULL���򷵻�false�����򷵻�true
	bool MakeUserOffLine(BYTE bDeskStation);
	///�������غ���
public:
	///��Ϸ��ʼ
	virtual bool GameBegin(BYTE bBeginFlag);
	///��Ϸ����
	/// @param bDeskStation �쳣����ʱ���������������������е�λ�úţ���������ʱ������
	/// @param bCloseFlag  �쳣������Ϸʱ�ı�־����������ʱΪ0
	virtual bool GameFinish(BYTE bDeskStation, BYTE bCloseFlag);


	///
	virtual bool GameFinishContest(BYTE bDeskStation, BYTE bCloseFlag);


	///��ʱ����Ϣ
	virtual bool OnTimer(UINT uTimerID);
	///�ж���Ϸ���ϵ�ĳ������Ƿ�ʼ��Ϸ��
	virtual bool IsPlayGame(BYTE bDeskStation) { return m_bPlayGame; }
	///�жϴ���Ϸ���Ƿ�ʼ��Ϸ
	virtual BOOL IsPlayingByGameStation(){ return m_bPlayGame; }
	///��Ϸ���ݰ�������
	virtual bool HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser);

	///�������غ���
protected:
	///��ȡ��Ϸ״̬��Ϣ
	virtual bool OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)=0;
	///������Ϸ״̬
	virtual bool ReSetGameState(BYTE bLastStation)=0;
	///����д���ݿ�
	virtual BOOL UpdateUserDB(LONG dwUserID,int dwMoney = 0,int dwPoint = 0);
public:
	///����
	virtual bool UserUseProp(BYTE bsendDeskStation,BYTE brecvDeskStation,_TAG_PROP_MOIVE_USE * propMoive);
	///��������ֵ
};

#endif
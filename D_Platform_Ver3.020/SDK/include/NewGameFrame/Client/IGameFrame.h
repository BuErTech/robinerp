#pragma once
#include "CommonDefine.h"

/// ��Ϸʹ�õĽ�������
struct AwardTypeForGame
{
    TCHAR PropName[32]; //��������
    int nPropCount;     //���߸���
    int nGold;          //�����
    int nPropType;      //����ID��
};

/// ������Ϣ
struct GameInfoForGame
{
    int iPower;     // ����
};

/// �û���Ϣ
struct UserInfoForGame
{
    TCHAR			szNickName[61];										///<�ǳ�  
    __int64			i64Money;												///�û����								    						    
    long int		dwUserID;							                ///ID ����
	ULONG			dwUserIP;							                ///��¼IP��ַ
	UINT			bLogoID;							                ///ͷ�� ID ����
	bool			bBoy;								                ///�Ա�
	bool            bIsRobot;                                           ///�Ƿ������
	BYTE			bGameMaster;						                ///����ȼ�

	BYTE						bDeskNO;							///��Ϸ����
	BYTE						bDeskStation;						///����λ��
	char						szCity[61];							///���ڳ���
};


class CGlobal;
class IGameFrame
{
public:
    /// ����Ϣ
    virtual bool SendGameData(void * pData, UINT uBufLen, BYTE bMainID, BYTE bAssistantID, BYTE bHandleCode) = 0;
    ///��Ϸ�ӿ���л�ȡ����Ϣ
    virtual int GetMyDeskStation()=0;
    ///����Լ������Ӻ�
    virtual int GetMyDeskNum()=0;
    ///���ﴫ������ͼ����
    virtual int GetUserStation(int nIndex)=0;  
    ///����Ϣ���ݷ��͵���Ϸ������
    virtual int SendGameData(WORD wSubId,void *buffer, int nLen)=0;
    ///ֻ����Ϣ���͵��ͻ��˴���
    virtual int SendIpcData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)=0;
    ///��ͼת��������λ ��Ϊ������Ϸ��Ҫ�õ��������ڿ���д���
    virtual int ViewStation2DeskStation(int nViewIndex)=0;
    ///��λ��ת��ͼ��
    virtual int DeskStation2View(int nDeskStation)=0;
    ///��ȡ����ǳ�
    virtual TCHAR * GetUserNickName(int nDeskStation)=0;
    ///�ر���Ϸ�ͻ���
    virtual void CloseClient()=0;
    ///�������
    virtual int ClickContinue() = 0;
    ///ǿ�ƹر���Ϸ
    virtual void CloseClientforce()=0;
    ///����ʱ��
    virtual void SetTimer(int nEventID, DWORD dwTime) = 0;
    ///�ر�ʱ��
    virtual void KillTimer(int nEventID) = 0;
    ///����һ��ϵͳ��Ϣ
    virtual int InsertSystemMessage(TCHAR* szMessage)=0;
    /// ������ͨ��Ϣ
    virtual int InsertNormalMessage(TCHAR* szMessage)=0;
    ///��ȡ��������
    virtual bool GetAwardType(AwardTypeForGame &Type)=0;	
    ///���Ž�������
    virtual bool PlayAwardAnimation(const AwardTypeForGame &type)=0;
    ///��ȡ����
    virtual bool GetGameBaseInfo(GameInfoForGame &Info)=0;
    ///��ȡ�û���Ϣ
    virtual bool GetUserInfo(int nDeskStation, UserInfoForGame &Info)=0;
    /// ��������
    virtual bool PlaySound(const char *szFileName)=0;
    ///��ȡ��Ϸ����
    virtual int GetVolume()=0; 
    /// ĳ��λ�Ƿ�ʹ����Ƶ
    virtual bool SetUseVideo(int nViewStation, bool bUse)=0;
    /// ��ȡ�Ƿ�Ϊ�Թ�״̬
    virtual bool GetIsWatching()=0;
    /// ��ȡ�Ƿ������Թ�
    virtual bool GetIsEnableWatch()=0;
    /// ����Ϸ�����д����н���
    virtual void OnVisitBank()=0;
    /// ��ȡȫ����Ϣ
    virtual CGlobal* GetGlb()=0;
    /** @brief ʹ��Сͷ��
     */
    virtual bool SetUseSmallLogo(int nViewStation, bool bUseSmall=true)=0;
    /** @brief ��ȡ���ڴ�С
     */
    virtual RECT GetGameWindowRect()=0;

	/// ��ȡ��Ϸ��Ϣ2011-7-7
	virtual const GameInfoStructInExe* GetGameInfo()=0;

	 //״̬�ı�
    virtual void SetStationParameter(BYTE bGameStation)=0;
};
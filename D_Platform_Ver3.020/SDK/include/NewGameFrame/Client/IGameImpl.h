#pragma once

#ifdef GAMEIMPLEMENT_EXPORTS
#define GAMEIMPLEMENT_API __declspec(dllexport)
#else
#define GAMEIMPLEMENT_API __declspec(dllimport)
#endif //GAMELOGIC_EXPORTS

class IGameFrame;
struct IUserInterface;
struct TUIMessage;
class IGameImpl
{
public:
    virtual ~IGameImpl(){}
    /// ��ʼ��
    virtual int Initial()=0;
    /// ��ʼ��UI
    virtual int InitUI()=0;
    /// ��Ϸ��Ϣ
    virtual int HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen)=0;
    /// ������Ϣ
    virtual int OnUIMessage(TUIMessage* pMessage)=0;
    /// 
    virtual int ResetGameFrame(void)=0;
    /// ��ҽ���
    virtual int GameUserCome(void)=0;
    /// ����뿪
    virtual int GameUserLeft(int nIndex)=0;
    /// ��Ҷ���
    virtual int GameUserOffLine(void)=0;
    // �Ƿ�������Ϸ������Ϸʵ��
    // �Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
    virtual bool IsPlayingGame()=0;          
    /// ��ʱ����Ϣ
    virtual void OnTimer(int nTimerID) = 0;
    /// ��״̬
    virtual void SetStationParameter(BYTE bGameStation) = 0;
    //virtual void GameInfoChange(TGameInfo GameInfo) = 0;
    virtual bool OnGameQuiting() = 0;
};


extern "C" GAMEIMPLEMENT_API IGameImpl *CreateNewGameImpl(IGameFrame* pIBase, IUserInterface *pIUI);
extern "C" GAMEIMPLEMENT_API void DestroyNewGameImpl(IGameImpl **ppImp);

typedef IGameImpl *(* CREATEGAMEIMPLEMENT)(IGameFrame* pIBase, IUserInterface *pIUI);
typedef void (* DESTROYGAMEIMPLEMENT)(IGameImpl **ppImp);
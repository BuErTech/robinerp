#pragma once
#include <Windows.h>

#ifdef GAMELOGIC_EXPORTS
#define GAMELOGIC_API __declspec(dllexport)
#else
#define GAMELOGIC_API __declspec(dllimport)
#endif //GAMELOGIC_EXPORTS


struct TUIMessage;
struct IPC_Head;
class CChannelService;
/// ��Ϸ�߼����ʼ�������ṹ
struct LogicInitParam_t
{
    HINSTANCE   hInstance;  ///< ��Ϸʵ��
    void *pUI;				///< UI�ӿ�ָ��
    void *pIpc;				///< IPCģ��ӿ�ָ��
    void *pGameInfo;        ///< ��Ϸ��Ϣ
    void *pEngine;          ///< ����ӿ�
    HWND hwndHall;			///< �������ھ��  
    
    LogicInitParam_t(){ZeroMemory(this, sizeof(LogicInitParam_t)); }
};
/// �߼�ģ�����
/// �����ģ����õĽӿ���
class XGameLogicBase
{
public:
    /// ������������������ɾ��ʱ�ͷ���Դ
    virtual ~XGameLogicBase(){};    
    /// ��ʼ��
    virtual int Initial(LogicInitParam_t &lip)=0;
    /// IPC��Ϣ
    virtual bool OnIPCMessage(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)=0;
    /// IPC�������
    virtual bool OnIPCHeartBeat()=0;
    /// UI��Ϣ
    virtual int OnUIMessage(TUIMessage *p)=0;
    /// ʱ��
    virtual void OnTimer(int nEventID)=0;
    /// �뿪��Ϸ
    virtual void QuitGame(bool bNotify = true,bool bNeedCheck = false/*�Ƿ�Ҫ������ʾ��*/ )=0;
    /// ���ڴ�С�ı�
    virtual void OnSize(RECT &rect)=0;

public:
    /// ʹ�õ���
    virtual void OnPannelUseProp(WPARAM wParam, LPARAM lParam)=0;    
    /// �������
    virtual void OnPannelBuyProp(WPARAM wParam, LPARAM lParam)=0;
    /// ���͵���
    virtual void OnPannelGiveProp(WPARAM wParam, LPARAM lParam)=0;
    /// ������ʹ�õ���
    virtual void OnSelOtherUseProp(WPARAM wParam, LPARAM lParam)=0;
    //�û�ʹ�ù㲥���Ի�����ȷ������������û�ID���û�����ֵ
    virtual void OnUseBoardcast(WPARAM wparam, LPARAM lparam)=0;   
    //���н��淢������Ϣ
    virtual void OnBankMessage(UINT message,WPARAM wParam,LPARAM lParam)=0;
    //�� VIP
    virtual void OnPropBuyVIP(UINT message,WPARAM wParam,LPARAM lParam)=0;
};

extern "C" GAMELOGIC_API XGameLogicBase *CreateGameLogic();
extern "C" GAMELOGIC_API void DestroyGameLogic(XGameLogicBase **ppLogic);

typedef XGameLogicBase *(* CREATEGAMELOGIC)();
typedef void (* DESTROYGAMELOGIC)(XGameLogicBase **ppLogic);
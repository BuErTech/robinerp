#pragma once

#ifdef FRAME_EXPORTS
#define FRAME_API _declspec(dllexport)
#else
#define FRAME_API _declspec(dllimport)
#endif //FRAME_EXPORT

struct IPC_Head;
struct EngineInit_t
{
    HINSTANCE   hInstance;      /// ��Ϸico���
    void        *pIPC;          /// IPCģ��
    void        *pGameInfo;     /// ��Ϸ��Ϣ
};
/// 
class IGameEngine
{
public:
    /// ��ʼ��
    virtual int Initial(EngineInit_t &init)=0;
    /// IPC��Ϣ
    virtual bool OnIPCMessage(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)=0;
    /// IPC�������
    virtual bool OnIPCHeartBeat()=0;
    /// �滭
    virtual int DrawAll()=0;
    ///����ʱ��
    virtual void SetTimer(int nEventID, DWORD dwTime)=0;
    ///�ر�ʱ��
    virtual void KillTimer(int nEventID)=0;
    ///��ȡHWND
    virtual CWnd* GetCWnd()=0;
    ///�л�����뻹ԭ
    virtual void SwitchSize()=0;
};


extern FRAME_API IGameEngine *CreateGameEngine();
extern FRAME_API void CloseGameEngine(IGameEngine **ppGameEngine);
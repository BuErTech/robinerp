#pragma once
#include "IGameEngine.h"


class CChannelService;
class XGraphEngine;
class XUIBase;
interface IUserInterface;
class XGameLogicBase;

/// ��Ϸ����
/// ���ڴ��� UI�� �߼���֮��Ĺ�ϵ
class XGameEngine 
    : public CWnd
    , public virtual IGameEngine
{
public:
    XGameEngine();
    ~XGameEngine();
private:
    XGameEngine(const XGameEngine &other);
    XGameEngine &operator = (const XGameEngine &other);

public:
    /// ��ʼ��
    virtual int Initial(EngineInit_t &init);
    /// IPC��Ϣ
    virtual bool OnIPCMessage(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);
    /// IPC�������
    virtual bool OnIPCHeartBeat();
    /// �滭
    virtual int DrawAll();
    ///����ʱ��
    virtual void SetTimer(int nEventID, DWORD dwTime);
    ///�ر�ʱ��
    virtual void KillTimer(int nEventID);
    ///��ȡHWND
    virtual CWnd* GetCWnd();
    ///�л�����뻹ԭ
    virtual void SwitchSize();

protected:
    /// ������
    virtual LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam );

private:
    //
    const TCHAR *GetAppPath();

    void OnTimer(UINT_PTR nIDEvent);

private:
    XGameLogicBase              *m_pLogic;
    HINSTANCE                   m_pGameInstance;
    CChannelService             *m_pIPCMod;

    XGraphEngine                *m_pGraphEngine;
    XUIBase                     *m_pUIBase;
    IUserInterface              *m_pIUI;
    bool                        m_bUIReady;
    bool                        m_bIsInitial;

	bool						m_bDUIShow;			//Ϊ�˷�ֹDirectUI�Ĵ�����Ⱦ��bbwUI�죬�������ʾ�ұ߿�����ʾ��Ϸ��Ĳ������֣�\
													  �����m_bRightFrameShow��Ŀ����DirectUI��Ⱦ��ɺ��������أ���bbwUI��Ⱦ��ɺ�����DirectUI������ʾ��\
													  jianguankun 2012.9.7

};
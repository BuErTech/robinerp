
#ifndef	__LOUDSPEAKERMSGWND__H__
#define	__LOUDSPEAKERMSGWND__H__

#include "StdAfx.h"
#include "IPC/ChannelService.h"

class CUseBoardCastWnd : 
	public BzDui::CWindowWnd,
	public BzDui::INotifyUI,
	public BzDui::IMessageFilterUI
{
public:
	CUseBoardCastWnd();
	virtual ~CUseBoardCastWnd();

public:

	LPCTSTR GetWindowClassName() const;

	UINT GetClassStyle() const;

	void OnFinalMessage(HWND hWnd);

	void Init();

	void Notify(BzDui::TNotifyUI& msg);

	void CenterWindow();

public:

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	LRESULT OnSendBuyProp(WPARAM wParam, LPARAM lParam);

	LRESULT OnExpress(WPARAM wParam, LPARAM lParam);


	void LoadNormalCtrlSkin();
public:
	//ȷ��
	void OnOK();

	//���õ���ID
	void SetPropID(int iPropID);

	int GetPropPrice(int iPropID);

	_TAG_USERPROP* GetProp(int iPropID);

	//���ø�����ָ��
	void SetParentWnd(CWnd * pParWnd);

	void OnClickEXpress();


	_TAG_BOARDCAST	m_BoardCast;
	void UseProp_Buy(int nPropIndex);

private:
	//��ͼ
	BzDui::CPaintManagerUI	m_pm;
	//������ָ��
	CWnd					*m_pParentWnd;

	//��������
	int						m_iBroadcastStyle;


	//����
	BzDui::CButtonUI		*m_btExpress;
	//���ȱ༭����Ϣ
	BzDui::CRichEditUI*		m_pREdtCtrl;
	//�û������͵�������Ϣ
	BzDui::CStdString		m_userInfo;
	//�û���Ϣ����
	UINT					m_uiUserInfoLen;
	//����ID��
	int						m_nPropID;
	//��ɫ
	COLORREF				m_RGB;
	//�ŵ�
	CChannelService						*m_ChannelService;

	//������Ϣ��ʾ
	BzDui::CTextUI*		m_TextTip;

	BzDui::CLabelUI*	m_LabCaption;


	CGameTalkText		m_ExpressWnd;						//���鴰��
};



#endif
#ifndef CAFCACTWND_HEAD_FILE
#define CAFCACTWND_HEAD_FILE


#define WM_SHOWGAMEWINDOW			WM_USER+1					//���͸�����, �����Ƿ���ʾ��Ϸ
#define WM_LOADSKIN					WM_USER+2

#define TIMER_AD					1							//����ƶ���ʱ��

#include "SkinMgr.h"
class  CActWnd: public CWnd
{
	//��Ϣ����
protected:
	CHyperLink						m_copyright;				//��Ȩ����
	CGameImage						m_actl,m_actr,m_actc;		//����Ʊ���
	int								m_actadx,m_actady;			//�����
	CString							m_ad;						//���������
	CRect							m_adRect,m_addrawrect;		//�������
	CSize							m_adsize;					//������С

	bool							m_bCopyRight;				//�Ƿ��Ȩ
	HCURSOR							m_cursor;
	TCHAR							m_szURL[300];
	//skin
	int								m_x,m_y;
	TCHAR							m_pathn[MAX_PATH],m_patha[MAX_PATH];
	COLORREF						m_cn,m_ca,m_actadcolor;
	//��������
public:
	//���캯��
	CActWnd();
	//��������
	virtual ~CActWnd();

	DECLARE_MESSAGE_MAP()

	//���ܺ���
public:
	//��������
	bool CreateWnd(CWnd * pParentWnd, UINT uWndID);
	//��ʾ��Ȩ��Ϣ
	bool ShowCopyRight(bool bShow=true);

	//�ڲ�����
private:
	//������С
	bool FixControlStation();
	//�滭���
	void DrawFrame(CDC * pDC);
	//��Ϣ���� 
public:
	//�ػ�����
	afx_msg void OnPaint();
	//λ����Ϣ
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//��갴����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�õ�����
	afx_msg void OnSetFocus(CWnd * pOldWnd);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg void OnTimer(UINT nIDEvent);
	LRESULT OnLoadSkin(WPARAM wparam, LPARAM lparam);

	LRESULT OnExchangeSkin(WPARAM wparam, LPARAM lparam);
	
public:
	void LoadSkin(void);
public:
	void SetAd(TCHAR ad[]);
	//��������λ��
	void AutoSize(int cx,int cy);
private:
	//����
	CSkinMgr m_skinmgr;
};

#endif
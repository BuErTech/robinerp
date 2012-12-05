#ifndef _MY_GAME_SET_DLG_H_
#define _MY_GAME_SET_DLG_H_

#include "stdafx.h"
#include "Resource.h"

#define IDD_MUSIC_SCROLLBAR  100
#define IDD_SOUND_SCROLLBAR  101
#define IDD_SCROLLBAR_SLIDE  102

class MyScrollBar 
{
public:
	/** 
	 * ��ǰ��ť��״̬
	 */
	enum ButtonState
	{
		BUTTONSTATE_NORMAL = 0,          /// ����״̬
		BUTTONSTATE_HOVER,               /// ��������ϵ�״̬
		BUTTONSTATE_DOWND,               /// ����״̬
		BUTTONSTATE_NULL,
	};

	/// ���캯��
	MyScrollBar();
	/// ��������
	~MyScrollBar();

	/// ����ͼƬ�ļ�
	void LoadImage(CString filepath);
	/// ���û���������ʾλ��
	void SetDisplayPosition(CPoint pos) 
	{ 
		m_DisplayPos = pos; 
	}
	/// �õ�����������ʾλ��
	CPoint GetDisplayPosition(void) { return m_DisplayPos; }
	/// ���õ�ǰ������״̬
	inline void SetState(MyScrollBar::ButtonState state) { m_ButtonState = state; }
	/// �õ���ǰ������״̬
	inline MyScrollBar::ButtonState GetState(void) { return m_ButtonState; }
	/// ��������
	void Draw(CDC *pDC,int x,int y);
	/// �¼�����
	bool OnEvent(MSG *pMsg);
	/// ����Ƿ�������ؼ���
	bool IsInside(CPoint pos);

private:
	ButtonState m_ButtonState;             /**< ��ǰ��ť��״̬ */
	CGameImage  m_buttonImage;             /**< ��ťʹ�õ�ͼƬ */
	CPoint      m_DisplayPos;              /**< ����������ʾλ�� */
};

class MyGameScrollbar : public CWnd
{
public:
	/**
	 * ������������
	 */
	enum ScrollBarType
	{
		SCROLL_TYPE_MUSIC = 0,          /// ����
		SOROLL_TYPE_SOUND,              /// ��Ч
		SOROLL_TYPE_NULL               
	};

	/// ���캯��
	MyGameScrollbar();
	/// ��������
	virtual ~MyGameScrollbar();

	/// �õ���ǰ�������İٷֱ�
	float GetCurrentProcess(void);
	/// ���õ�ǰ�������İٷֱ�
	void SetCurrentProcess(float pro);
	/// ���ù���������
	inline void SetType(MyGameScrollbar::ScrollBarType type)
	{
		m_ScrollBarType = type;	
	}
	/// �õ�����������
	inline MyGameScrollbar::ScrollBarType GetType(void)
	{
		return m_ScrollBarType;
	}

private:
	/// ��������
	void DrawScrollbar(CDC *pDC);

private:
	MyScrollBar			           m_btslide;                      /**< ���� */
	bool                           m_IsMoving;                     /**< �Ƿ��ƶ����� */
	int                            m_SlideDisPos;                  /**< �����ĵ�ǰ��ʾλ�� */

	CDC							   m_memDC;
	bool						   m_bDCStored;		           /**< �Ƿ��Ѿ����汳��ͼ */

	CGameImage                     m_ImageFirst;                  /**< ��������ǰ��  */
	CGameImage                     m_ImageMiddle;                 /**< ���������ж� */
	CGameImage                     m_ImageLast;					  /**< ����������� */	

	ScrollBarType                  m_ScrollBarType;               /**< ������������ */

protected:
	///��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	/// ����뿪
	afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);  

	DECLARE_MESSAGE_MAP()
};

class MyGameSetDlg : public CLoadFileBmp
{
public:
	/// ���캯��
	MyGameSetDlg(CWnd* pParent=NULL);
	/// ��������
	virtual ~MyGameSetDlg();

	/// �Ƿ񲥷�����
	inline bool IsPlayMusic(void)
	{
		return m_btCheckMusic.GetCheck() > 0 ? true : false;
	}
	/// �Ƿ񲥷���Ч
	inline bool IsPlaySound(void)
	{
		return m_btCheckSound.GetCheck() > 0 ? true : false;		
	}
	/// �õ����ֲ��ŵĴ�С
	inline float GetMusicVolume(void)
	{
		return m_MusicScrollbar.GetCurrentProcess();
	}
	/// �õ���Ч���ŵĴ�С
	inline float GetSoundVolume(void)
	{
		return m_SoundScrollbar.GetCurrentProcess();
	}

	/// �������������ļ�
	void LoadConfig(CString path);
	/// �������������ļ�
	void SaveConfig(void);

	///��ʼ������
	virtual BOOL OnInitDialog();

	enum { IDD = IDD_GAME_SET };

private:
	CGameImage                             m_imgBk;

	CNormalBitmapButtonEx                  m_btOk;
	CNormalBitmapButtonEx                  m_btCancel;
	CButton                                m_btCheckMusic;
	CButton                                m_btCheckSound;
	MyGameScrollbar                        m_MusicScrollbar;
	MyGameScrollbar                        m_SoundScrollbar;
	CString                                m_SoundConfig;

protected:
	///DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange * pDX); 
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();

	///��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	DECLARE_MESSAGE_MAP()
};

#endif

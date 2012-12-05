#pragma once
#ifndef SHOWSCORE_HEAD_H
#define SHOWSCORE_HEAD_H
#include "PublicDefine.h"

#include"AFCImage.h"
#include"ClientComStruct.h"
#include "SkinMgr.h"
// CShowScore �Ի���
#define		IDR_NONE	0
#define		IDR_DOWN	1
#define		IDR_DELAY	2
#define		IDR_UP		3
// ���ְ���
class EXT_CLASS CShowScore : public CDialog
{
	DECLARE_DYNAMIC(CShowScore)

public:
	CShowScore(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShowScore();

// �Ի�������
//	enum { IDD = IDD_SHOWSCORE };
public:
	int		m_listx,m_listy;	// �б�ʼ��λ��
	int		m_showh;			// ������ʾʱ���صĸ߶�
	int     m_itemh,m_w1,m_w2,m_w3,m_w4,m_w5;	// �б���߶�
	int		m_count,w,h;		// �б�����,�ߺͿ�ļ��

	int		m_iShowType;

	UserItemStruct					* m_pUserItem[MAX_PEOPLE];	// �û���Ϣָ��
	__int64		m_allscore[MAX_PEOPLE];		// �ܷ�
	__int64		m_lastscore[MAX_PEOPLE];	// �ϴη�
	__int64     m_allmoney[MAX_PEOPLE];		// �ܽ��
	__int64     m_lastmoney[MAX_PEOPLE];	// �ϴν��
	int     m_ucomtype;					// ��Ϸ����
	COLORREF	m_textcr;				// ��ɫ
protected:
	bool	m_havemouse;				// ����Ƿ��ڴ�����
	int		m_curx,cury;				// ����
	int		m_state;					// ��ǰ״̬
protected:
	CGameImage	m_bk;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	
	afx_msg LRESULT OnMouseLeave(WPARAM w, LPARAM l);
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnCancel();
	virtual void OnOK();
public:
	void SetBkImage(TCHAR  path[]);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	void UpdateUser(int station, UserItemStruct * pUserItem);
public:
	void SetUserScore(int station,__int64 fen, __int64 money=0);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	void SetTextColor(COLORREF color);
public:
	void Show(void);
public:
	void Delay(void);
public:
	void Hide(void);

	bool m_bShow;
	void ShowWnd();
	void HideWnd();
public:
	void End(void);
	//
	void ClearUser(void);
	//
	void ClearScore(void);

	BOOL SetWindowPos(const CWnd* pWndInsertAfter, int x, int y,
		int cx, int cy, UINT nFlags);
private:
	CSkinMgr	m_skinmgr;
};
#endif
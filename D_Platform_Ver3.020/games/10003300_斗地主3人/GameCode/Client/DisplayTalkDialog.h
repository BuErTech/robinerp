#pragma once
#include "Stdafx.h"
#include "Resource.h"
#include "afxcmn.h"

// CDisplayTalkDialog dialog

class CDisplayTalkDialog : public CDialog
{
	DECLARE_DYNAMIC(CDisplayTalkDialog)

public:
	CDisplayTalkDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDisplayTalkDialog();

// Dialog Data
	enum { IDD = IDD_DISPLAY_TALK };
private:
	CString m_strCurrentText;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	//---------------�Ի����е�һЩ����--------------------
	CFont  m_font;
	CString m_msg;
	COLORREF  m_crFont;
	CGameImage m_img;
	int		m_Tran;	//͸���̶�
 	//-------------����2������������ʹ��-------------------
	CRect m_rectCurrentLine; //��ǰ�еķ�Χ,ͨ��ֻ��yֵ
	CPoint m_ptCurrent;      //��ǰ�е����λ�ã�ֻ��x��Ч

	CRect  m_rectClient;
	BOOL   m_bWriteDC;  //�Ƿ񻹿�����DC�������
	CDC*   m_pDC;       //ΪOnPaint�ػ���ͳһ��DC.
    int    m_fontHeight;  //ѡ��DC������ĸ߶�
	int		m_viewid;
	int    m_nJianGe;  //�������ͼƬbottom�ļ��
	BOOL   m_bPreIsText; //������ͼƬ֮ǰ�Ƿ�������
	BOOL   m_bIsFirst;   //��ʾ�������ͼƬ���������Ƿ�Ϊ��һ�Σ���Ϊ��һ���������������
	int    m_nLineJianGe;  //������֮���֮��
	int    m_nTextToImage;  //������ͼƬ֮��ļ��,Ҳ��ͼƬ��ͼƬ��ļ��
public:
	virtual BOOL OnInitDialog();
	void DisplayTalkText(TCHAR* szCharString);   //�趨��������------�ⲿ�ӿ�

    CTrueItem* TranslateChar(TCHAR * szCharString, int iCharLength, int & iPassCharCount);
    void InsertBitmapToDC(HBITMAP hBitmap);
    HBITMAP CopyBitmap(HBITMAP hSourceHbitmap);
    void TransparentBltBitmap(HBITMAP hBitmap,CPoint ptCurrent,COLORREF crTrans);  //͸��������λͼ��DC

    void InsertCharToDC(const TCHAR * szCharString);
 	BOOL IsNewLine(const TCHAR* szMsg);            //�ж���ʾszMsgʱ�Ƿ���Ҫ����
	int  ThisLineContainChar(const TCHAR* szMsg);  //�жϱ��п����ɵ��ַ���
	void CopyNumChar(const TCHAR* pSrc,int iStart,int iEnd,TCHAR* pDst);   //��pSrc�Ե�iStart����iEnd���ַ����Ƶ�pDst��,��'\0'

	void DisplayOmit();        //��ʾʡ�Ժ�
public:
 	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnPaint();
 public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

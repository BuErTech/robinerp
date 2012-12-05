#pragma once

#include "StdAfx.h"
#include "Resource.h"
#define JETON_W 31
#define JETON_H 30
class CeasyNtList : public CDialog
{
	//�ؼ�����
public:
	CGameImage				m_bk;								//����ͼ
	CGameImage				m_PicNum;							//ʱ������
	//CNormalBitmapButtonEx    m_Bm4;						//��ť
	//CNormalBitmapButtonEx    m_Bm3;						//��ť
	//CNormalBitmapButtonEx    m_Bm2;						//��ť
	//CNormalBitmapButtonEx    m_Bm1;						//��ť
	//CNormalBitmapButtonEx    m_Bm5;						//��ť
	//CNormalBitmapButtonEx    m_Bm6;						//��ť
	CNormalBitmapButtonEx    m_BmMax;					//��ť
	CNormalBitmapButtonEx    m_BmCancel;				//��ť
	CNormalBitmapButtonEx    m_BmOk;				     //��ť
	CRect					m_BtnRect0,m_BtnRect1,m_BtnRect2;	//���ο�
	int						m_BtnIndex0,m_BtnIndex1,m_BtnIndex2;//����
	int						m_iStart0,m_iStart1,m_iStart2,m_iEnd;//������ע������ʾ������ע
	//CNormalBitmapButtonEx	m_BmOk;
//CNormalBitmapButtonEx	m_BmCancel;
//	CBitmapButton		m_Bmmp;							//��ť
//	CBitmapButton		m_Bmnt;							//��ť
	int					m_Num;							//��ǰ��ע��
	int				m_iMinNote;				    	//��Сע
	int				m_iThisLimit;					    //��ǰ����ע��
	enum			{ IDD = IDD_EASY_LIST };		     	//ID ����

	//��������
public:
	//���캯��
	CeasyNtList(CWnd * pParent = NULL);
	//�����б�
	void UpdateNumList(int b1=0,int b2=0,int b3=0,int b4=0,int b5=0,int b6=0);
	//������ע
	void SetNumList(int iThisTurnLimit,int MinNote);
	//����ѡ��ť
	void UpdateBtnList(BOOL up1=false,BOOL up2=false,BOOL up3=false,BOOL max=false,BOOL cancel=false);
	//�滭����
	void DrawNum(CDC * pDC);
	//�滭����
	void DrawNum(CDC * pDC, int iXPos, int iYPos, UINT uNum);

protected:
	//DDX ֧��
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//��Ϣ����
protected:
	//ȷ������
	virtual void OnOK();
	//ȡ������
	virtual void OnCancel();
	//��ʼ������
	virtual BOOL OnInitDialog();
	//��ע1��Ϣ
	afx_msg void OnNote1();
	//��ע2��Ϣ
	afx_msg void OnNote2();
	//��ע3��Ϣ
	afx_msg void OnNote3();
	//�����Ϣ
	afx_msg void OnMax();	
	//ȡ����Ϣ
	afx_msg void OnResetCancel();
   //ȷ����Ϣ
	afx_msg void OnResetOk();
	afx_msg void OnPaint();

	//afx_msg void OnNT()   ;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
	public:
	afx_msg void OnBnClicked7();
public:
	afx_msg void OnBnClicked13();
	//����������
	void CeasyNtList::SetJetonStart(int iStart1,int iStart2 = 0,int iStart3 = 0,int iEnd = 0);
	//��ȡ����ֵ
	int GetIndex(int iScore);

};


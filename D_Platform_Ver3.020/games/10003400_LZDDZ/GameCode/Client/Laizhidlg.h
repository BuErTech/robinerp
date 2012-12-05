#pragma once

#include "StdAfx.h"
#include "Resource.h"
#define    IDC_NEXT_GAME   12345
#define    IDC_CANCEL_GAME 12346

//��������
#define	UG_HUA_MASK					0xF0							//��ɫ����
#define	UG_VALUE_MASK				0x0F							//��������
//��Ϸ��Ϣ��
class CLaiZhiDlg : public CWnd
{
	//��������
protected:
	//CFont				m_Font;						//��ʾ����
//	CString				m_strMessage;				//�ַ���Ϣ
//	CBitmap				m_BackBmp;					//����
	ReplaceStruct	    m_Replace;
	int                 m_ThisCount;                //��ǰ��ʾ��Ϊ�ڼ���������¼��ǰҳ
	bool                m_bPassShow;                //�Ƿ�治����ť.
	CGameImage			m_bk;
	CGameImage          m_OptionalCardsType;          //�ڶԻ���ʾ�Ŀ�ѡ������
	CGameImage          m_LaiZiCard;                  //�����
	CGameImage			m_Flag;						//���
	BYTE				m_iViewStation;				//�Ƿ�Ϊ�����
	CRect				m_BtnRect0,m_BtnRect1,m_BtnRect2,m_BtnRect3;	//���ο�
//	enum					{ IDD = IDD_GAME_END };			//�Ի�������
	//��������
public:
	CNormalBitmapButtonEx   m_btNext;       //��һҳ
	CNormalBitmapButtonEx   m_btCancel;     //ȡ��
	CWnd *m_pParnet;
	//���캯��
	CLaiZhiDlg();
	//��������
	virtual ~CLaiZhiDlg();
	//�Զ�������С
	void AutoSize(int x,int y);
	//������Ϣ
	void SetMessage(ReplaceStruct Replace, bool PassShow, bool bFlag = false);
	//��һҳ
	void OnHitNext();
	//ȡ��
	void OnHitCancel();
	//�õ��ַ�
	BOOL GetString(BYTE CardList[], int CardCount,  CString & str);
	int GetCardNum(BYTE iCard) { return (iCard&0x0F)+1; }
	void OnSelect(int Id);
	//��Ϣ����
public:
	//�ػ�����
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	//��ȡ�˿���ֵ
	BYTE GetCardValue(BYTE bCard) 
	{ 
		//return (bCard&UG_VALUE_MASK);
		BYTE bkk=(bCard&UG_VALUE_MASK)+1;
		if (bkk==14) bkk=1;
		return bkk; 
	}
	//��ȡ�˿˻�ɫ
	BYTE GetHuaKind(BYTE bCard) { return bCard&UG_HUA_MASK; };
	//��ȡ�˿�λ��
	BYTE GetCardXPos(BYTE bCard) { return GetCardValue(bCard)-1; };
	//��ȡ�˿�λ��
	BYTE GetCardYPos(BYTE bCard) { return GetHuaKind(bCard)>>4; };

};

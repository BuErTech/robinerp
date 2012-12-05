#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "GameCard.h"

/// ״̬��ͼ
class CStationView : public CWnd
{
	DECLARE_DYNAMIC(CStationView)

	/// ��������
public:
	CUpGradeGameCard			m_PointCard;			///< �����˿�

	/// ��Ϣ����
	char						m_iEndStation;			///< ����λ��
	char						m_iMeStation;			///< ����λ��
	char						m_iUpGrade;				///< ׯ��λ��
	char						m_iBeginStation;		///< ��ʼλ��
	char						m_iOtherStation;		///< �з�λ��
	char						m_iNTStation;			///< ����λ��
	char						m_iNTCount;				///< ������Ŀ
	char						m_iNTHua;				///< ���ƻ�ɫ
	char						m_iPlayCardCount;		///< �˿���Ŀ
	char						m_iLessPlay;			///< ���ٻغ�
	char						m_iPlayCount;			///< ս���غ�
	int							m_iTurePoint;			///< ��ʵ����
	int							m_iPunishPoint;			///< ��Ϸ����
	BOOL						m_bShowMessage;			///< �Ƿ���ʾ
	
	CGameImage					m_PicBottomBar;			///< �׶���ͼ
	CGameImage					m_PicBottomBarLeft;		///< �׶���ͼ
	CGameImage					m_PicBottomBarRight;	///< �׶���ͼ
	CBitmap						m_NTLogo;				///< ��־ͼ��


	/// ��������
public:
	/// ���캯��
	CStationView();
	/// ��������
	virtual ~CStationView();
	DECLARE_MESSAGE_MAP()

	/// ���ܺ���
public:
	BOOL Create(CWnd * pParentWnd, UINT nID);

	/// ��Ϣ����
public:
	/// ������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	/// �滭��Ϣ
	afx_msg void OnPaint();

	/// ���ܺ���
public:
	/// ����ׯ��
	void SetUpGrade(int iUpGradeStation);
	/// ���ñ����ƾ���Ϣ
	void SetMeSideInfo(int iBeginStation, int iEndStation, int iPlayStation);
	/// ���õз��ƾ���Ϣ
	void SetOtherSideInfo(int iBeginStation, int iEndStation, int iPlayStation);
	/// ����������Ϣ
	void SetNTInfo(int iNTStation, int iNTHua, int iPlayCardCount);
	/// ���ûغ���Ϣ
	void SetPlayInfo(int iPlayCount, int iLessCount);
	/// �������
	void AddPointCard(BYTE iCardList[], int iCardCount);
	/// �������
	void SetTurePoint(int iTurePoint);
	/// ���뷣��
	void AddPunishPoint(int iPunishPoint);
	/// ������Ϣ
	void CleanAllInfo(BOOL bAll);
	/// �Ƿ���ʾ��Ϣ
	void SetShowMessage(BOOL bShow);
	void DrawNtLogo(CDC * pDC, int x, int y, int iHuaKind);
};



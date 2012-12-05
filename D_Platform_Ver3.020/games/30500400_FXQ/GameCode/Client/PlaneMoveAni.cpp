#include "StdAfx.h"
#include "PlaneMoveAni.h"

CPlaneMoveAni::CPlaneMoveAni(void)
{
	m_cbMySide   = 255 ; 
	m_cbMoveSide = 255 ; 
	m_inDex = 255 ; 

	m_iXstep = 0 ;
	m_iYStep = 0 ; 

	m_iActiveTyple = 0  ; 

	m_iNeedByNodeCount = 0 ; 

	m_iHaveByNodeCount = 0 ;

	m_bRun = false ; 

	m_bFinish = false ; 

	memset(m_cbDestInDex , 0 , sizeof(m_cbDestInDex)) ; 

	m_ptCurrentPos.SetPoint(0 , 0 ) ;

	for(int i = 0 ; i<MAX_NODE ; i++)
	{
		m_ptDestPoint[i].SetPoint(0 , 0) ; 
		m_cbDirect[i] = 255 ;
	}
}

CPlaneMoveAni::~CPlaneMoveAni(void)
{
}
///��ʼ������
void  CPlaneMoveAni::IniData(void)
{
	m_cbMySide   = 255 ; 

	m_cbMoveSide = 255 ; 

	m_inDex = 255 ; 

	m_iXstep = 0 ;
	m_iYStep = 0 ; 

	m_iNeedByNodeCount = 0 ; 

	m_iHaveByNodeCount = 0 ;

	m_iActiveTyple = 0 ; 

	m_bRun = false ; 

	m_bFinish = false ; 

	memset(m_cbDestInDex , 0 , sizeof(m_cbDestInDex)) ; 

	m_ptCurrentPos.SetPoint(0 , 0 ) ;

	for(int i = 0 ; i<MAX_NODE ; i++)
	{
		m_ptDestPoint[i].SetPoint(0 , 0) ; 
		m_cbDirect[i] = 255 ;
	}

}
void  CPlaneMoveAni::SetPlaneData(BYTE cbMySide , BYTE cbMoveSide , BYTE inDex)
{
	m_cbMySide = cbMySide ; 
	m_cbMoveSide = cbMoveSide ; 
	m_inDex = inDex ; 
}
///���õ�ǰ������
void  CPlaneMoveAni::SetPlaneFlyData(int iNeedByNode , BYTE cbPathIndex[] ,CPoint ptDest[] , BYTE cbDirect[]) 
{
	m_iNeedByNodeCount = iNeedByNode ;
	m_iHaveByNodeCount = 1 ; 
	m_ptCurrentPos = ptDest[0] ; 
	m_bRun = true ; 
	m_bFinish = false ; 

	for(int i = 0 ; i<iNeedByNode ; i++)
	{
		m_cbDestInDex[i] = cbPathIndex[i] ; 
		m_ptDestPoint[i] = ptDest[i] ; 
		m_cbDirect[i] = cbDirect[i] ;
	}
}
///������󻯺���С����������������
void CPlaneMoveAni:: ResetPlanePoint(CPoint  ptPix) 
{
	for(int i =  0 ; i<m_iNeedByNodeCount ; i++)
	{
		m_ptDestPoint[i].Offset(ptPix) ;
	}
}

///��������
void  CPlaneMoveAni::RunAni(void)
{
	if(m_bFinish)
	{
		return ; 
	}

	m_iXstep = 5*GetsignByTwoNum(m_ptDestPoint[m_iHaveByNodeCount].x , m_ptCurrentPos.x) ;
	m_iYStep = 5*GetsignByTwoNum(m_ptDestPoint[m_iHaveByNodeCount].y , m_ptCurrentPos.y) ; 

	m_ptCurrentPos.x += m_iXstep; 
	m_ptCurrentPos.y += m_iYStep ; 

	CString strInfo ; 
	//strInfo.Format("wysoutfxqinfo::��ǰ��m_iXstep==%d, m_iYStep==%d" ,m_iXstep,m_iYStep) ; 
	//OutputDebugString(strInfo) ; 

	CRect  iRect ; 
	iRect.left   = m_ptDestPoint[m_iHaveByNodeCount].x - STEP_WIDE ; 
	iRect.right  = m_ptDestPoint[m_iHaveByNodeCount].x + STEP_WIDE ; 
	iRect.top    = m_ptDestPoint[m_iHaveByNodeCount].y - STEP_WIDE ; 
	iRect.bottom = m_ptDestPoint[m_iHaveByNodeCount].y + STEP_WIDE ; 
	
	if(iRect.PtInRect(m_ptCurrentPos)||(m_ptDestPoint[m_iHaveByNodeCount] == m_ptCurrentPos))
	{
		m_iHaveByNodeCount ++;

		if(m_iHaveByNodeCount == m_iNeedByNodeCount) ///�Ѿ������ĵ�
		{
			m_ptCurrentPos = m_ptDestPoint[m_iHaveByNodeCount - 1] ; 
			m_bFinish = true ; 
		}
	}
}




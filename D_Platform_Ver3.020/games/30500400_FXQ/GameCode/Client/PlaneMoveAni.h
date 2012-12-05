#pragma once

const  int   MAX_NODE     = 52 ; 
///ÿ���ƶ��Ĵ�С
const  int   STEP_WIDE  = 6 ; 

class CPlaneMoveAni
{
public:
	CPlaneMoveAni(void);
public:
	~CPlaneMoveAni(void);

public: 
	BYTE     m_cbMySide ;        ///��ǰ�Լ���λ��
	BYTE     m_cbMoveSide  ;     ///�ƶ�������������һ��������
	BYTE     m_inDex ;          ///�ƶ��ķɻ�������


	int      m_iNeedByNodeCount;  ///��Ҫ�����ĵ�ĸ���
	int      m_iHaveByNodeCount ;  //�Ѿ������˵Ľڵ�
	BYTE     m_cbDestInDex[MAX_NODE] ;  ///�����Ҫ�����ĵ�
	BYTE     m_cbDirect[MAX_NODE] ;      ///�ĵ㷽��

	CPoint   m_ptDestPoint[MAX_NODE] ;  ///��Ҿ����ĵ��λ��
	
	CPoint   m_ptCurrentPos ;        ///��ǰ������λ��

	BYTE    m_iActiveTyple ;   ///��������

	bool     m_bRun;              ///�Ƿ����ڲ��Ŷ���
	bool     m_bFinish ;          ///�Ƿ����

	int      m_iXstep ;           //x�����ϵ��ƶ��ٶ�
	int      m_iYStep ;           //y�����ϵ��ƶ��ٶ�

public:
	 ///��ʼ������
	 void   IniData(void) ;
	 ///��ȡ����֮��ķ���
	 inline  int  GetsignByTwoNum(long iNum1, long iNum2) 
	 {
		 if(abs(iNum1 - iNum2) < 5)
		 {
			 return 0 ; 
		 }
		 return iNum1 >iNum2?1:-1 ; 
	 }

public:
	 ///��ȡ��ǰ�Ƿ����ڲ��Ŷ���
	bool   GetPlayState(void) { return m_bRun ; }
	///��ȡ��ǰ�ߵ���λ��
	CPoint GetCurrentPos(void ) { return  m_ptCurrentPos ;}
	///��ȡ��ǰ�ɻ��ķ���
	BYTE   GetCurrentDirect(void){return m_cbDirect[m_iHaveByNodeCount -1] ;}
	///��ȡ��ǰ�ɻ�����ɫ
	BYTE   GetPlaneColor(void) { return  m_cbMoveSide ; }
     ///����ִ����
	void   RunAni(void) ; 
	///������ҵ�����
	void   SetPlaneData(BYTE cbMySide , BYTE cbMoveSide , BYTE  inDex) ; 
	 ///���õ�ǰ������
    void   SetPlaneFlyData(int iNeedByNode , BYTE cbPathIndex[] ,CPoint ptDest[] , BYTE cbDirect[]) ;
	///����������Ϸ���ƶ��ɻ�������
	void   ResetPlanePoint(CPoint  ptPix) ;
	
};

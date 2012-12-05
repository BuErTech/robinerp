#pragma once

#include "StdAfx.h"
#include <vector>
#include "../GameMessage/JqLogic0.h"

/**
 * A�ǽṹ
 */
typedef struct tagAStarStruct
{
	UINT FVal;	//Fֵ��G+H��
	UINT GVal;	//Gֵ����������������ɵ�·����һ��λ�õ��ƶ�����,�ٶ������֮���ƶ��ľ���Ϊ10����б�ǣ�
	UINT HVal;	//Hֵ���Ӹ���λ�õ�Ŀ��λ�õĹ����ƶ����룩
	POINT ptParent;	//�����λ��
	POINT ptPos;	//�Լ�λ��
}AStarStruct;

class CJqBoard :public CJqLogic0
{
protected:	
	 CPStack g_StepLine;
	 CPStack m_StepLine;
     int g_CurI,g_CurJ,g_startqi;
	 int m_startqi,m_endqi;
	 bool m_bShowStartChess;

private:
	 /// ���д����б�
	 std::vector<AStarStruct> m_AllList;
	 /// �����б�
	 std::vector<AStarStruct> m_OpenList;
	 /// �ر��б�
	 std::vector<POINT> m_CloseList;
	 /// ��ǰλ��
	 AStarStruct m_CurrentPos;

public :
	bool bFinishBuzhen();
	bool bCanIMove(void);
	void  ToMyCord(int station,int& toi,int& toj,int fi,int fj);
	void UserMove(tagMoveResult&   result);
	bool m_bICanMove;
	int GetJq(int station,int i,int j);
	void CopyOtherBuZhen(tagBuZhen& bz);//CPtrList & from,int DevSite);
	void FinishBuZhen ();
	void StartBuzhen();
	void Init();
	void LoadBuZhen();
	void SaveBuZhen();
	void OnLBUp(CPoint& pt);
	void OnPaint(CDC *pdc);
	void DrawPath(CDC * pDC);
	bool m_bFinishBuzhen;
	CPoint m_ptBoardStartCenter;
    bool MoveQiZi(int iTo,int jTo,int iFrom,int jFrom,bool IsIPut=true);
	CJqBoard(void);
	~CJqBoard(void);
	/// ���ѡ�����꣨�Լ���������ʱ�����ã�
	void SetStartEndXY(){ m_iStartI =m_iStartJ= m_iEndI = m_iEndJ = -1; }
	/// ��ʼ���켣
	void InitPath();
	/// ����ÿ���켣
	void UserPath(int station, int x, int y, POINT start, POINT end);
	/// �����ƶ��켣�е�����
	void UserMovePathCheen(int station, POINT ptFront, POINT ptStart, POINT ptEnd, int iFinalEndX, int iFinalEndY);
	/// ����ˢ������
	void GetRefurbishRECT(RECT & rc, POINT start);
	/// ������˸
	void SetFlashChess(int &iX, int &iJ, bool bFlash);
	/// ����Ƿ���˸
	bool GetFlashChess() const{ return m_bShowStartChess; }
	
protected:
	
	void SetAQiZi(int ii,int jj,CDC* MemDC,bool showfront = false);
	bool  Exchang2QiZiInBuZhen(int iTo,int jTo,int iFrom,int jFrom);
	void ShowAllJunQiFlag(CDC *pdc);
	/// ���ҵĲ��ӷ�Χ
	static bool bIsInMyBuZhenArea(int i,int j);
	/// ��������
	static bool bIsInRailway(int i,int j);
	/// ��������Чλ��
	static bool bIsInBoard(int i,int j);
	/// �ھ�Ӫ
    static bool bIsInJuYin(int i,int j);
	/// �ڴ�Ӫ
    static bool bIsInDaBenYin(int i,int j);

private:
	bool bPushChaXian(int iTo,int jTo,int ii,int jj,CPStack& s);
	bool CanGoCircle(int iTo,int jTo,int iFrom,int jFrom);

	bool bGBGoRailway(int iTo,int jTo,int iFrom,int jFrom);
	bool bGBGoRailway1(int iTo,int jTo,int iFrom,int jFrom);

	bool CanGoI(int fromi,int toi,int jj);
	bool CanGoJ(int fromj,int toj,int ii);

	bool IsCanMove2Qi(int toQi,int fromQi);
	bool MoveQiZi1(int iTo,int jTo,int iFrom,int jFrom);
	
	int m_station,m_viewstation,m_MyColor;
	void  EmptyStepLine();
    bool bCanGoThrough(int iTo,int jTo,int ii,int jj);
		
private:
	/// ����Gֵ
	UINT CountGVal();
	/// ����Hֵ
	UINT CountHVal(int x, int y, int iToX, int iToY);
	/// ����Fֵ
	bool SetFVal(AStarStruct & POS);
	/// ���Ҹ�λ���Ƿ��ڹر�λ��
	bool FindCloseList(POINT pt);
	/// �����СFֵ
	AStarStruct GetMinFVal();
	/// ���Ҹ�λ���Ƿ������б
	bool GetGoTiltPath(POINT pt);
	/// �����Ƿ����м����Ӹ�λ��
	bool GetCenterFarmWordPath(POINT pt);
	/// ���뿪���б�
	void AddOpenList(int iTo,int jTo, POINT ptParent);
	/// ��������
	bool bCanArrival(int iTo,int jTo,int iFrom,int jFrom);
};

struct BuZhenNode
{
	int i,j,jqname;
};
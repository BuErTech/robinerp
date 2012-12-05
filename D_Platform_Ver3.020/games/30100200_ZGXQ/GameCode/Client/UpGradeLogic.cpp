#include "StdAfx.h"
#include "UpGradeLogic.h"

/**
 * ���캯��
 */
CUpGradeGameLogic::CUpGradeGameLogic(void)
{
	
}


/*
//�Ƿ���Գ���
BOOL CUpGradeGameLogic::CanOutCard(BYTE iOutCard[], int iOutCount, BYTE iBaseCard[], 
								   int iBaseCount, BYTE iHandCard[], int iHandCount)
{
	return TRUE;
}
*/

/**
 * ����������ʲô��
 * @return ����Ϊ0���򷵻���ID����Ϊ0�������塣
 */
int  CUpGradeGameLogic::GetChess(int xChess,int yChess,int chessArray[][9])
{
	::CopyMemory(m_iQiZi,chessArray,sizeof(chessArray));
	if (xChess > 0 && xChess < 10)
	{
		if (yChess > 0 && yChess < 9)
		{
			return m_iQiZi[xChess][yChess];
		}
	}
	return XQ_QIZI_NONE;
}

void CUpGradeGameLogic::InitBoard(int iMeFlag, int qi[][9])
{
	m_iSideFlag = iMeFlag;
	for (int i = 0;i < 10;i++)
		for (int j = 0;j < 9;j++)
			qi[i][j] = XQ_QIZI_NONE;


	if (iMeFlag == XQ_FLAG_RED)
	{
		qi[0][0] = qi[0][8] = XQ_BLACK_JU;
		qi[0][1] = qi[0][7] = XQ_BLACK_MA;
		qi[0][2] = qi[0][6] = XQ_BLACK_XIANG;
		qi[0][3] = qi[0][5] = XQ_BLACK_SHI;
		qi[0][4]		    = XQ_BLACK_JIANG;
		qi[2][1] = qi[2][7] = XQ_BLACK_PAO;
		qi[3][0] = qi[3][2] = qi[3][4] = qi[3][6] = qi[3][8] =XQ_BLACK_BIN;


		qi[6][0] = qi[6][2] = qi[6][4] = qi[6][6] = qi[6][8] =XQ_RED_BIN;
		qi[9][0] = qi[9][8] = XQ_RED_JU;
		qi[9][1] = qi[9][7] = XQ_RED_MA;
		qi[9][2] = qi[9][6] = XQ_RED_XIANG;
		qi[9][3] = qi[9][5] = XQ_RED_SHI;
		qi[9][4]		    = XQ_RED_JIANG;
		qi[7][1] = qi[7][7] = XQ_RED_PAO;

	}

	else if (iMeFlag == XQ_FLAG_BLACK)
	{
		qi[0][0] = qi[0][8] = XQ_RED_JU;
		qi[0][1] = qi[0][7] = XQ_RED_MA;
		qi[0][2] = qi[0][6] = XQ_RED_XIANG;
		qi[0][3] = qi[0][5] = XQ_RED_SHI;
		qi[0][4]		    = XQ_RED_JIANG;
		qi[2][1] = qi[2][7] = XQ_RED_PAO;
		qi[3][0] = qi[3][2] = qi[3][4] = qi[3][6] = qi[3][8] =XQ_RED_BIN;


		qi[6][0] = qi[6][2] = qi[6][4] = qi[6][6] = qi[6][8] =XQ_BLACK_BIN;
		qi[9][0] = qi[9][8] = XQ_BLACK_JU;
		qi[9][1] = qi[9][7] = XQ_BLACK_MA;
		qi[9][2] = qi[9][6] = XQ_BLACK_XIANG;
		qi[9][3] = qi[9][5] = XQ_BLACK_SHI;
		qi[9][4]			= XQ_BLACK_JIANG;
		qi[7][1] = qi[7][7] = XQ_BLACK_PAO;
	}


	return;
}

/**
 * �ܷ�����
 */
BOOL CUpGradeGameLogic::CanMoveQiZi(int fromi, int fromj, int toi, int toj, int qi[][9],bool check)
{
	::CopyMemory(m_iQiZi,qi,sizeof(m_iQiZi));
	if (
		(toi == fromi && toj == fromj) ||
		(toi < 0 || toj < 0)		   ||
		(toi > 9 || toj > 8)		   ||
		(fromi < 0 || fromj < 0)       ||
		(fromi > 9 || fromj > 8)
		)
		
	{
		
		return FALSE;
	}
	
	int qizi = m_iQiZi[fromi][fromj];
	if (qizi == XQ_QIZI_NONE)
		return FALSE;
	int flag;
	if (qizi > 0) 
		flag = XQ_FLAG_RED;
	else 
		if (qizi < 0) 
			flag = XQ_FLAG_BLACK;
	else
		return FALSE;
	
	if (flag * m_iQiZi[toi][toj] > 0) return FALSE;

	BOOL ret = FALSE;
	
	if (qizi == XQ_RED_JU || qizi == XQ_BLACK_JU)
	{
		ret = MoveZhu(fromj,fromi,toj,toi);
		goto retu;
	}
	if (qizi == XQ_RED_MA || qizi == XQ_BLACK_MA)
	{
		ret = MoveMa(fromj,fromi,toj,toi);
		goto retu;
	}
	if (qizi == XQ_RED_PAO || qizi == XQ_BLACK_PAO)
	{
		ret = MovePao(fromj,fromi,toj,toi,flag);
		goto retu;
	}
	if (qizi == XQ_RED_JIANG || qizi == XQ_BLACK_JIANG)
	{
		ret = MoveShuai(fromj,fromi,toj,toi);
		goto retu;
	}
	if (qizi == XQ_RED_SHI || qizi == XQ_BLACK_SHI)
	{
		ret = MoveShi(fromj,fromi,toj,toi);
		
		goto retu;
	}
	if (qizi == XQ_RED_XIANG || qizi == XQ_BLACK_XIANG)
	{
		ret = MoveXiang(fromj,fromi,toj,toi);
		goto retu;
	}
	if (qizi == XQ_RED_BIN || qizi == XQ_BLACK_BIN)
	{
		ret = MoveBin(fromj,fromi,toj,toi);
		
		goto retu;
	}

retu:
	
//	if (qizi == XQ_RED_JIANG || qizi == XQ_BLACK_JIANG)
//	{
//		TCHAR sz[200];
//		wsprintf(sz,"qizi=%d,ret=%d",qizi,ret);
//		WriteStr(sz,3,3);
//	}
	if (!ret) return FALSE;
	if(check)
		return true;
//	if(CheckOtherJiangJun(fromi,fromj,flag,m_iQiZi))
//		return 0;
	////AfxMessageBox("MoveShi");
/*	m_iQiZi[toi][toj] = qizi;
	m_iQiZi[fromi][fromj] = XQ_QIZI_NONE;
	::CopyMemory(qi,m_iQiZi,sizeof(m_iQiZi));
*/	return TRUE;
}

/**
 * ��������ֵ(���ڻ���)
 */
BOOL CUpGradeGameLogic::MoveQiZi(int fromi, int fromj, int toi, int toj, int qi[][9])
{
	::CopyMemory(m_iQiZi,qi,sizeof(m_iQiZi));
	int qizi = m_iQiZi[fromi][fromj];
	m_iQiZi[toi][toj] = qizi;
	m_iQiZi[fromi][fromj] = XQ_QIZI_NONE;
	::CopyMemory(qi,m_iQiZi,sizeof(m_iQiZi));	
	return true;
}

/**
 * ��������ֵ(���ڻ���)
 */
BOOL CUpGradeGameLogic::SetQiZi(int fx,int fy,int value)
{
	m_iQiZi[fx][fy] = value;
	return true;
}

/**
 * ��ȡ����ֵ
 */
int CUpGradeGameLogic::GetQiZiValue(int fx, int fy)
{
	return m_iQiZi[fy][fx];
}

BOOL CUpGradeGameLogic::MoveBin(int fx, int fy, int tx, int ty)
{
	if (abs(tx - fx) > 1 || abs(ty - fy) > 1) return FALSE;	//ֻ����һ��
	if ((tx != fx) && (ty != fy)) return FALSE;	//ֻ�ܺ��߻�����
	
	//TCHAR sz[200];
	////wsprintf(sz,"fx=%d,fy=%d,Դ%d,Ŀ��%d,%d",fx,fy,GetQiZiValue(fx,fy),GetQiZiValue(tx,ty),m_iSideFlag);
//	WriteStr(sz,m_iSideFlag);

	//�б���˭�ı�
	if(GetQiZiValue(fx,fy) * m_iSideFlag > 0)
	{
		if (ty > fy) return FALSE;	//ֻ��ǰ�����ܺ���
		if (fy > 4)	//û����
		{
			if (tx != fx) return FALSE;	//ֻ������
		}
	}
	else//�Է��ı�
	{
		//if(GetQiZiValue(tx,ty)*m_iSideFlag == 6 )//||GetQiZiValue(tx,ty) == 6) //������
		//{
			//TCHAR sz[200];
		//	wsprintf(sz,"fx=%d,fy=%d,Դ%d,Ŀ��%d,%d",fx,fy,GetQiZiValue(fx,fy),GetQiZiValue(tx,ty),m_iSideFlag);
		//	WriteStr(sz,m_iSideFlag);
		//}
		if(ty < fy) return FALSE;	//���ܺ���
		if(fy < 5)					//�мұ�δ����
		{
			if( tx != fx) return FALSE;
		}
	}
	return TRUE;
}

BOOL CUpGradeGameLogic::MoveMa(int fx, int fy, int tx, int ty)
{
	if (abs(tx - fx) > 2 || abs(ty - fy) > 2) return FALSE;
	if (abs(ty - fy) == 2)
	{
		if (abs(tx - fx) != 1) return FALSE;
		if (fy > ty)
		{
			if (m_iQiZi[fy - 1][fx] != XQ_QIZI_NONE) return FALSE;	//������

		}
		else
		{
			if (m_iQiZi[fy + 1][fx] != XQ_QIZI_NONE) return FALSE; //������
		}
	}
	else if (abs(tx - fx) == 2)
	{
		if (abs(ty - fy) != 1) return FALSE;
		if (fx > tx)
		{
			if (m_iQiZi[fy][fx - 1] != XQ_QIZI_NONE) return FALSE; //������
		}
		else
		{
			if (m_iQiZi[fy][fx + 1] != XQ_QIZI_NONE) return FALSE; //������
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CUpGradeGameLogic::MovePao(int fx, int fy, int tx, int ty, int flag)
{
	int has = 0;
	if (tx != fx && ty != fy) return FALSE;
	if (ty != fy)
	{
		if (fy > ty)
		{
			for (int i = ty + 1;i < fy;i++)
			{
				if (m_iQiZi[i][fx] != XQ_QIZI_NONE) has++;
			}
			if (has > 1) return FALSE;
			if (has == 1)
			{
				if (flag * m_iQiZi[ty][tx] < 0) return TRUE;
				else return FALSE;
			}
			if (m_iQiZi[ty][tx] != XQ_QIZI_NONE) return FALSE;
		}

		else
		{
			for (int i = fy + 1;i < ty;i++)
			{
				if (m_iQiZi[i][fx] != XQ_QIZI_NONE) has++;
			}
			if (has > 1) return FALSE;
			if (has == 1) 
			{
				if (flag * m_iQiZi[ty][tx] < 0) return TRUE;
				else return FALSE;
			}
			if (m_iQiZi[ty][tx] != XQ_QIZI_NONE) return FALSE;
		}
	}

	else
	{
		if (fx > tx)
		{
			for (int i = tx + 1;i < fx;i++)
			{
				if (m_iQiZi[fy][i] != XQ_QIZI_NONE) has++;
			}
			if (has > 1) return FALSE;
			if (has == 1)
			{
				if (flag * m_iQiZi[ty][tx] < 0) return TRUE;
				else return FALSE;
			}
			if (m_iQiZi[ty][tx] != XQ_QIZI_NONE) return FALSE;
		}
		else
		{
			for (int i = fx + 1;i < tx;i++)
			{
				if (m_iQiZi[fy][i] != XQ_QIZI_NONE) has++;
			}
			if (has > 1) return FALSE;
			if (has == 1)
			{
				if (flag * m_iQiZi[ty][tx] < 0) return TRUE;
				else return FALSE;
			}
			if (m_iQiZi[ty][tx] != XQ_QIZI_NONE) return FALSE;
		}
	}
	
	return TRUE;
}

/**
 * ʿ������·��ֻ���ǾŹ��ڵ�б��
 */
BOOL CUpGradeGameLogic::MoveShi(int fx, int fy, int tx, int ty)
{	
	
	if (abs(tx - fx) > 1 || abs(ty - fy) > 1) return FALSE;
	
	if (ty < 7 || tx < 3 || tx > 5) return FALSE;
	if (tx == fx || ty == fy) return FALSE;
	return TRUE;
}

/**
 * ��ֻ���ھŹ����߶������ߺ�����
 */
BOOL CUpGradeGameLogic::MoveShuai(int fx, int fy, int tx, int ty)
{
	if (ty < 7 && m_iQiZi[ty][tx] + m_iQiZi[fy][fx] == 0 && tx == fx)//������(�Ԍ��Ҍ�)
	{
		for (int i = ty + 1;i < fy;i++)
		{
			if (m_iQiZi[i][fx] != XQ_QIZI_NONE) return FALSE;
		}
		return TRUE;
	}
	if (fy < 3 && m_iQiZi[ty][tx] + m_iQiZi[fy][fx] == 0 && tx == fx)//������(���ҳԱ��Ҍ�)
	{
		for (int i = fy + 1;i < ty;i++)
		{
			if (m_iQiZi[i][fx] != XQ_QIZI_NONE) return FALSE;
		}
		return TRUE;
	}

	if (ty < 7 || tx < 3 || tx > 5) return FALSE;
	if (abs(tx - fx) > 1 || abs(ty - fy) > 1) return FALSE;
	if (tx != fx && ty != fy) return FALSE;
	return TRUE;
}

/**
 * �಻�ܹ��ӣ�ֻ���ߡ��
 */
BOOL CUpGradeGameLogic::MoveXiang(int fx, int fy, int tx, int ty)
{
	if (ty < 5) return FALSE;
	if (abs(tx - fx) != 2 || abs(ty - fy) != 2) return FALSE;
	if (m_iQiZi[(fy + ty) / 2][(fx + tx) / 2] != XQ_QIZI_NONE) return FALSE;
	return TRUE;
}

BOOL CUpGradeGameLogic::MoveZhu(int fx, int fy, int tx, int ty)
{
	if (tx != fx && ty != fy) return FALSE;
	if (ty != fy)
	{
		if (fy > ty)
		{
			for (int i = ty + 1;i < fy;i++)
			{
				if (m_iQiZi[i][fx] != XQ_QIZI_NONE) return FALSE;
			}
		}
		else
		{
			for (int i = fy + 1;i < ty;i++)
			{
				if (m_iQiZi[i][fx] != XQ_QIZI_NONE) return FALSE;
			}
		}
	}
	else
	{
		if (fx > tx)
		{
			for (int i = tx + 1;i < fx;i++)
			{
				if (m_iQiZi[fy][i] != XQ_QIZI_NONE) return FALSE;
			}
		}
		else
		{
			for (int i = fx + 1;i < tx;i++)
			{
				if (m_iQiZi[fy][i] != XQ_QIZI_NONE) return FALSE;
			}
		}
	}
	return TRUE;
}

/**
 * ������߀�ǔ�����
 */
int CUpGradeGameLogic::CheckOtherOrMeJiang(int Startx, int Starty,int Endx,int Endy,int meflag,int MeBoard[][9])
{
	//����������2
	if(CheckOtherJiangJun( Startx,  Starty, Endx, Endy, meflag, MeBoard,false))
		return 2;
	//����������1
	if(CheckMeJiangJun(Startx,  Starty, Endx, Endy, meflag, MeBoard))
		return 1;
	return 0;
}

/**
 * ����Ƿ񽫾�(ԭ��,��ȡ�з�����˧λ��,���������ӱ���һ��,���Ƿ����ӿ��Դﵽ�Է���˧��λ��)
 */
int CUpGradeGameLogic::CheckMeJiangJun(int Startx, int Starty,int Endx,int Endy,int meflag,int MeBoard[][9])
{
	int endi,endj,icount=0;

	//���ҵз���
	CPoint point = GetQiZiKing(-1*meflag,MeBoard);
	endi=point.x;
	endj=point.y;
	if(endi >= 10 ||endj >=9)		//��Ч��λ
		return 0;
	CPoint MePoint[16];				//һ��16����
	icount=GetMeQiZi(MePoint,meflag,MeBoard);

	if(icount<=0)					//��Ч��
		return 0;

	for(int i=0;i<icount;i++)
	{
		if(CanMoveQiZi(MePoint[i].x,MePoint[i].y,endi,endj,MeBoard,true))		//������
			return 1;
	}
	return 0;
}

/**
 * �з�������(����ȷ���Ƿ�����ƶ�ĳЩ��)�����ƶ���ɶԷ�����
 */
int CUpGradeGameLogic::CheckOtherJiangJun(int Startx, int Starty,int Endx,int Endy,int meflag,int MeBoard[][9],BOOL bTest)
{
	int TestBoard[10][9];
	::CopyMemory(TestBoard,MeBoard,sizeof(TestBoard));

	if(bTest)	//����ֵ
	{
		TestBoard[Endx][Endy] = TestBoard[Startx][Starty];
		TestBoard[Startx][Starty] = 0;
	}
	int endi,endj,icount=0;
	//������λ��
	CPoint point = GetQiZiKing(meflag,TestBoard);
	endi=point.x;
	endj=point.y;
	if(endi == 10 ||endj >=9)		//��Ч��λ
		return 0;
	CPoint MePoint[16];				//һ��16����

	icount=GetMeQiZi(MePoint,-1*meflag,TestBoard);//�з�����

	if(icount <= 0)					//��Ч��
		return 0;
	for(int i = 0;i < icount;i ++)
	{
		/*if(MePoint[i].y == 4)
		{
			TCHAR sz[200];
			wsprintf(sz,"Դx=%d,y=%d,%d,Ŀ��x=%d,y=%d,%d,BOOL = %d",
				MePoint[i].x,MePoint[i].y,TestBoard[MePoint[i].x][MePoint[i].y],
				endi,endj,TestBoard[endi][endj],
				CanMoveQiZi(MePoint[i].x,MePoint[i].y,endi,endj,TestBoard,true));
			WriteStr(sz);
		}*/
		if(CanMoveQiZi(MePoint[i].x,MePoint[i].y,endi,endj,TestBoard,true))		//�з���
			return 2;
	}
	return 0;
}

/**
 * ��������λ��
 */
CPoint CUpGradeGameLogic::GetQiZiKing(int meflag,int meboard[][9])
{
	CPoint point(10,9);

	for(int i=0;i<10;i++)
	{
		for(int j=0;j<9;j++)
		{
			if(meboard[i][j]*meflag == 6)
			{
				point=CPoint(i,j);
				break;
			}
		}
	}
	return  point;
}

/**
 * ���ұ�����������λ��
 */
int CUpGradeGameLogic::GetMeQiZi(CPoint point[],int meflag,int MeBoard[][9])
{
	int count=0;
	for(int i = 0;i < 10;i++ )
	{
		for(int j=0;j<9;j++)
		{
			if(MeBoard[i][j]*meflag > 0)		//��������
			{
				point[count++]=CPoint(i,j);
			}
		}
	}
	return count;
}

/**
 * ���ұ����Ƿ�����ɶ�
 */
BOOL CUpGradeGameLogic::HaveQiZiMove(int meflag,int MeBoard[][9])
{
	if( IsLose( meflag,MeBoard)) return false;	//˧�Ѿ�����
	//if( IsLive(meflag,MeBoard) )	 return true;							//�����ӿɶ�
	//���Լ����Ʊ�������,�����̱���һ��
	if( IsDieCheck(meflag,MeBoard)) return false;							//����
	//if(	CanEatKing(meflag,MeBoard)) return false;						//���ԳԶԷ���
	return true;// CheckOtherJiangJun(0,0,0,0,meflag,MeBoard,false);
	//return true;
}

/**
 * ʤ��
 */
BOOL CUpGradeGameLogic::IsLose(int meflag, int MeBoard[][9])
{
	//������λ��
	CPoint point = GetQiZiKing(meflag,MeBoard);
	int endi=point.x;
	int endj=point.y;
	if(endi == 10 ||endj >=9)		//��Ч��λ
		return true;
	return false;	
}

/**
 * �Ƿ��л��,�Ҳ����ƶ�������ɶԷ�����
 */
BOOL CUpGradeGameLogic::IsLive(int meflag,int MeBoard[][9])
{
	return true;
}

/**
 * �Ƿ��л��,�Ҳ����ƶ�������ɶԷ�����
 */
BOOL CUpGradeGameLogic::CanEatKing(int meflag,int MeBoard[][9])
{
	int endi,endj,icount=0;

	//���ҵз���
	CPoint point = GetQiZiKing(-1*meflag,MeBoard);
	endi=point.x;
	endj=point.y;
	if(endi >= 10 ||endj >=9)		//��Ч��λ
		return 0;
	CPoint MePoint[16];				//һ��16����
	icount=GetMeQiZi(MePoint,meflag,MeBoard);

	if(icount<=0)					//��Ч��
		return 0;

	for(int i=0;i<icount;i++)
	{
		if(CanMoveQiZi(MePoint[i].x,MePoint[i].y,endi,endj,MeBoard,true))		//������
			return 1;
	}
	return false;
}

/**
 * �Ƿ�����
 */
BOOL CUpGradeGameLogic::IsDieCheck(int meflag,int MeBoard[][9])
{
	int iCount = 0;
	CPoint MePoint[16];				//һ��16����
	iCount=GetMeQiZi(MePoint,meflag,MeBoard);

	if(iCount<=0)					//��Ч��
		return 0;

	//���Լ������Ӷ����̽���һ�α���.�ٲ���Ƿ������˽���״̬
	for(int i=0;i<iCount;i++)
	{
		for(int x=0;x<10;x++)
			for( int y=0;y<9;y++)
			{
				//�ܷ��ƶ�����λ��
				if(CanMoveQiZi(MePoint[i].x,MePoint[i].y,x,y,MeBoard,false))
				{
					//�ƶ�ĳһ�������Ӳ�����״̬
					if(!CheckOtherJiangJun(MePoint[i].x,MePoint[i].y,x,y,meflag,MeBoard,true))
						return false;
				}
			}

	}
	return true;
}

void CUpGradeGameLogic::SetBoard(int iMeFlag, int qi[][9])
{
	m_iSideFlag = iMeFlag;
	::CopyMemory(m_iQiZi,qi,sizeof(m_iQiZi));
}

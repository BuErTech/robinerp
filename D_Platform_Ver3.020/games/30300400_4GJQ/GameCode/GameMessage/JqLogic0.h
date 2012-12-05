#pragma once

#include "JqConst.h"

#define    MAX_I  17 
#define 	MAX_J  17 

typedef int JQBOARD[MAX_I][MAX_J];

enum PathDirection
{
	_up = 0,		//�Ϸ�
	_right = 1,		//�ҷ�
	_down = 2,		//�·�
	_left = 3,		//��
	_RUp = 4,		//����
	_RDown = 5,		//����
	_LDown = 6,		//����
	_LUp = 7		//����	
};

void  Testlog(TCHAR *pMessage);
class CJqLogic0 
{
public :
	int m_iStartI,m_iStartJ,m_iEndI,m_iEndJ;
	int m_iJingQiI,m_iJingQiJ;
	int m_iNowOutPeople;
	bool m_bWatchOther;
	void Rotate(int& iTo,int& jTo,int ifrom,int jfrom ,int angle);
	int Compare(int ToQi,int FromQi);
	JQBOARD mBoard;
	BYTE m_Path[MAX_I][MAX_J];
	int m_iPlayerCount;
	int	m_iStation;									///< ����ҵ�λ��, �����ж���ȷ���ӿ������
	 int GetStationFromColor(int color)
   {
	    if(color ==JQ_TYPE_COLOR_RED )return 0;
		if(color ==JQ_TYPE_COLOR_GRAY) return 1;
		if(color ==JQ_TYPE_COLOR_BLACK) return 2;
		if(color ==JQ_TYPE_COLOR_BLUE) return 3;

		return -1;
   }

	 int GetColorFromStation(int station)
	 {
		 if(station == 0)return JQ_TYPE_COLOR_RED;
		 if(station == 1) return JQ_TYPE_COLOR_GRAY;
		 if(station == 2) return JQ_TYPE_COLOR_BLACK;
		 if(station == 3) return JQ_TYPE_COLOR_BLUE;

		 return -1;
	 }

   void ClearColorAllQiZi(int color)
   {
	  /// int col=GetColor(station);
	   for(int i=0;i<MAX_I;i++)
		for(int j=0;j<MAX_J;j++)
		{
			if(color == GetAJqCol(mBoard[i][j]))
				mBoard[i][j]=JQ_TYPE_NONE;
		}
   }

   /// �õ����ӿ��ߵ����
   void GetDeadPeople(bool color[])
   {
	   // Testlog("��ʼ");
	   int col=-1;
	   for(int i=0; i<4; i++)
		   color[i]=false;
	   for(int i=0;i<MAX_I;i++)
	   {
		   for(int j=0;j<MAX_J;j++)
		   {
			   int jq=mBoard[i][j];			   
			   if(jq == JQ_TYPE_NONE)
				   continue;	
#ifdef JJ_TEST_DEAD
			   TCHAR sz[100];
			   wsprintf(sz,"���� %d %d %d",jq,i,j);
			   Testlog(sz);
#endif
			   if((0==i&&7==j) || (0==i&&9==j)
				   ||(7==i&&0==j) || (9==i&&0==j)
				   ||(7==i&&16==j) || (9==i&&16==j)
				   ||(16==i&&7==j) || (16==i&&9==j))
			   {//��Ӫ�ڵ����Ӳ�����
#ifdef JJ_TEST_DEAD
				   wsprintf(sz,"��Ӫ�ڵ����Ӳ�����%d %d %d\r\n",mBoard[i][j],i,j);
				   Testlog(sz);
#endif
				   continue;
			   }			   
			   col = GetAJqCol(jq);
			   if(col<0 || col>3)
			   {
#ifdef JJ_TEST_DEAD
				   Testlog("��ɫ����\r\n");
#endif
				   continue;
			   }
			   //if(color[col])
				//   continue;
			   if(16==i)
			   {
				   int jqn=GET_A_JQ_NAME(mBoard[i-1][j]);
				   if(JQ_TYPE_NAME_JQ==jqn || JQ_TYPE_NAME_DL==jqn)
				   {
#ifdef JJ_TEST_DEAD
					   wsprintf(sz,"���2����Ϊ���׻���� %d %d\r\n",jq,jqn);
					   Testlog(sz);
#endif
					   continue;
				   }
			   }
				
			   if(0==i)
			   {
				   int jqn=GET_A_JQ_NAME(mBoard[i+1][j]);
				   if(JQ_TYPE_NAME_JQ==jqn || JQ_TYPE_NAME_DL==jqn)
					 {
#ifdef JJ_TEST_DEAD
					    wsprintf(sz,"���0����Ϊ���׻���� %d %d\r\n",jq,jqn);
					    Testlog(sz);
#endif
						continue;
				   }
			   }
			   
			   if(0==j)
			   {
				   int jqn=GET_A_JQ_NAME(mBoard[i][j+1]);
				   if(JQ_TYPE_NAME_JQ==jqn || JQ_TYPE_NAME_DL==jqn)
				   {
#ifdef JJ_TEST_DEAD
					    wsprintf(sz,"���3����Ϊ���׻���� %d %d\r\n",jq,jqn);
					    Testlog(sz);
#endif
						continue;
				   }
			   }
			   
			   if(16==j)
			   {
				   int jqn=GET_A_JQ_NAME(mBoard[i][j-1]);
				   if(JQ_TYPE_NAME_JQ==jqn || JQ_TYPE_NAME_DL==jqn)
				   {
#ifdef JJ_TEST_DEAD
					   wsprintf(sz,"���1����Ϊ���׻���� %d %d\r\n",jq,jqn);
					   Testlog(sz);
#endif
					   continue;
				   }
			   }			  
			   if(	GET_A_JQ_NAME(jq) == JQ_TYPE_NAME_JQ 
				   || GET_A_JQ_NAME(jq) == JQ_TYPE_NAME_DL)
			   {
#ifdef JJ_TEST_DEAD
				   Testlog("����Ϊ���׻����\r\n");
#endif
				   continue;
			   }
#ifdef JJ_TEST_DEAD
			   Testlog("��������\r\n");
#endif
			   color[col]=true;
		   }
	   }
#ifdef JJ_TEST_DEAD
	   Testlog("����");
#endif
   }

public :
	CJqLogic0(){ }
	~CJqLogic0(void){}

};



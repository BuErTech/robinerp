#pragma once

#include <vector>

const  int  SPACE_WIDE    = 40 ;
const  int  SPACE_HEIGHT  = 40 ; 

///�õ���ҵ������ͼλ��
inline BYTE  GetUserRelativePos(BYTE  iMyStation ,BYTE iStation ) 
{
	return  (iStation - iMyStation + 2 + PLAY_COUNT )%PLAY_COUNT  ;
}
// ͨ���ɻ�������ҵ�λ�ú��ҵ�enSide�ó��ɻ��ڹ���·���ϵľ���λ��   
inline int GetPublicStepIndex(enumSide enMySide,int nMyStep)
{
	if(enMySide == emInvalidate || nMyStep == 255) 
		return 255;

	if(enMySide == emGreen) 
		return nMyStep;

	if (enMySide == emRed) 
	{
		if(nMyStep >= 0 && nMyStep <= 38) 
			return nMyStep + 13;

		if(nMyStep >= 39 && nMyStep < 52) 
			return nMyStep - 39;
	}

	if (enMySide == emYellow)
	{
		if(nMyStep >= 0 && nMyStep <= 25) 
			return nMyStep + 26;

		if(nMyStep >= 26 && nMyStep < 52) 
			return nMyStep - 26;
	}
	if (enMySide == emBlue)
	{
		if(nMyStep >= 0 && nMyStep <= 12) 
			return nMyStep + 39;

		if(nMyStep >= 13 && nMyStep < 52) 
			return nMyStep - 13;
	}

	return 255;
}

// ͨ���ɻ��ڹ���·���ϵľ���λ�õó��ɻ�������ҵ�enSide��λ��
inline int GetMyStepIndex(enumSide enMySide,int nPubStep)
{
	if(enMySide == emInvalidate || nPubStep == 255) 
		return 255;

	return  (nPubStep + 13 *enMySide )%52 ; 
}
///�������̵�λ��
struct PATH	// �����������̵�����(�����ҵ���ɫ������)
{
	int     PathWidth ;
	int     PathHeight ; 
	CPoint  ptBasePos ; 
	// ����·�ߵ�����
	STEP PubPath[PUBLIC_PATH_STEP];	
	// ͣ��ƺ������
	struct PRVPATH
	{
		STEP Park[PLAYER_PLANE_NUM];	// �ļܷɻ�ͣ�ŵĸ���
		STEP TakeOff;					// ��ɵĸ���
		STEP Landing[PRIVATE_PATH_STEP]; // ��½�ĸ���
		STEP DashedS;					// ����Ŀ�ʼ����
		STEP DashedE;					// ����Ľ�������
	}PrvPath[PLAY_COUNT]; 

	///���������׼��
	void  SetBasePt(int iWidth , int iHeight , CPoint pt)
	{
		PathWidth  = iWidth ; 
		PathHeight = iHeight ; 
		ptBasePos = pt ;
	}
	// �����ҵ���ɫ����������Ϊ��������ϵ,����ÿ����񣬷ɻ�ͣ��λ�ã�����λ��
	void IniCoordinate(enumSide enMySide)		
	{
		for(int i = 0; i < PUBLIC_PATH_STEP; i += 4)
		{
			PubPath[i].cbSide   = emBlue;
			PubPath[i+1].cbSide = emGreen ;
			PubPath[i+2].cbSide = emRed;
			PubPath[i+3].cbSide = emYellow;
		}

		int nStartStepX = 0,nStartStepY = 0,nStepSize = 0;
		int i = 0 ; 
		// ��������һ����ʱ������
		STEP tPubPath[PUBLIC_PATH_STEP]; 
		for(i = 0; i <= 3; i++)		
		{
			tPubPath[i].enPlaneDt = emUp;
			tPubPath[i].ptPathPos.SetPoint(ptBasePos.x - 3*PathWidth ,ptBasePos.y + (7 -i)*PathHeight) ;
		}

		for(i = 4; i <= 6; i++)
		{
			tPubPath[i].enPlaneDt = emLeft;
			tPubPath[i].ptPathPos.SetPoint(ptBasePos.x - i*PathWidth ,ptBasePos.y + 3*PathHeight) ;
		}
		for(i = 7; i <= 12; i++)
		{
			tPubPath[i].enPlaneDt = emUp;
			tPubPath[i].ptPathPos.SetPoint(ptBasePos.x - 7*PathWidth ,ptBasePos.y + (10 -i)*PathHeight) ;
		}

		for(i = 13; i <= 16; i++)	
		{
			tPubPath[i].enPlaneDt = emRight;	
			tPubPath[i].ptPathPos.SetPoint(ptBasePos.x + (i -20 )*PathWidth ,ptBasePos.y -3*PathHeight) ;
		}

		for(i = 17; i <= 19; i++)	
		{
			tPubPath[i].enPlaneDt = emUp;
			tPubPath[i].ptPathPos.SetPoint(ptBasePos.x - 3*PathWidth ,ptBasePos.y +(13-i)*PathHeight) ;
		}

		for(i = 20; i <= 25; i++)	
		{
			tPubPath[i].enPlaneDt = emRight;	
			tPubPath[i].ptPathPos.SetPoint(ptBasePos.x +(i -23 )*PathWidth ,ptBasePos.y -7*PathHeight) ;
		}

		for (i = 26; i <= 29; i++)
		{
			tPubPath[i].enPlaneDt = emDown;
			tPubPath[i].ptPathPos.SetPoint(ptBasePos.x +3*PathWidth ,ptBasePos.y + (i - 33)*PathHeight) ;
		}

		for(i = 30; i <= 32; i++)	
		{
			tPubPath[i].enPlaneDt = emRight;	
			tPubPath[i].ptPathPos.SetPoint(ptBasePos.x + (i - 26)*PathWidth ,ptBasePos.y -3*PathHeight) ;
		}
		for(i = 33; i <= 38; i++)	
		{
			tPubPath[i].enPlaneDt = emDown;
			tPubPath[i].ptPathPos.SetPoint(ptBasePos.x + 7*PathWidth ,ptBasePos.y + (i - 36)*PathHeight) ;
		}
		for(i = 39; i <= 42; i++)	
		{
			tPubPath[i].enPlaneDt = emLeft;	
			tPubPath[i].ptPathPos.SetPoint(ptBasePos.x + ( 46-i)*PathWidth ,ptBasePos.y + 3*PathHeight) ;
		}
		for(i = 43; i <= 45; i++)	
		{
			tPubPath[i].enPlaneDt = emDown;
			tPubPath[i].ptPathPos.SetPoint(ptBasePos.x + 3*PathWidth ,ptBasePos.y + (i - 39)*PathHeight) ;
		}

		for(i = 46; i <= 51; i++)	
		{
			tPubPath[i].enPlaneDt = emLeft;
			tPubPath[i].ptPathPos.SetPoint(ptBasePos.x + ( 49- i)*PathWidth ,ptBasePos.y + 7*PathHeight) ;
		}
		tPubPath[3].enPlaneDt  = emLeftUp;
		tPubPath[16].enPlaneDt = emUpRight;
		tPubPath[29].enPlaneDt = emRightDown;
		tPubPath[42].enPlaneDt = emDownLeft;

		//// �����ҵ���ɫ���������ҵ�����ϵ
		for(i = 0; i < PUBLIC_PATH_STEP; i++)
		{
			// ��仰�����������:��������˵��λ��0,ʵ���ǹ���·����λ��GetPublicStepIndex(enMySide,i)
			PubPath[GetPublicStepIndex(enMySide, i)].ptPathPos = tPubPath[i].ptPathPos;
			PubPath[GetPublicStepIndex(enMySide, i)].enPlaneDt = tPubPath[i].enPlaneDt;
		}

		for(int i = 0 ; i<PLAY_COUNT ; i++)
		{
			int  xSign  ;
			int  ySign  ; 

			int  iViewPos = GetUserRelativePos(enMySide , i) ; 

			for(int  j = 0 ; j< PLAYER_PLANE_NUM ; j++)
			{
				 xSign = ((j%2 == 0)?-1 : 1) ;
				 ySign =  ((j>1)?1 : -1) ;

				if(iViewPos%2 == 0)
				{
					PrvPath[i].Park[j].ptPathPos.x = (PubPath[i*13 + 6].ptPathPos.x + PubPath[i*13 + 7].ptPathPos.x)/2 + xSign*SPACE_WIDE ; 
					PrvPath[i].Park[j].ptPathPos.y = (PubPath[i*13].ptPathPos.y + PubPath[i*13 + 1].ptPathPos.y)/2  + ySign*SPACE_WIDE; 
				}
				else
				{
					PrvPath[i].Park[j].ptPathPos.x = (PubPath[i*13].ptPathPos.x + PubPath[i*13 + 1].ptPathPos.x)/2 + xSign*SPACE_WIDE ; 
					PrvPath[i].Park[j].ptPathPos.y = (PubPath[i*13 + 6].ptPathPos.y + PubPath[i*13 + 7].ptPathPos.y)/2 + ySign*SPACE_WIDE;
				}

				if(iViewPos == 0 )
				{
					PrvPath[i].Park[j].enPlaneDt = emLeft ; 
				}
				else if(iViewPos == 1)
				{
					PrvPath[i].Park[j].enPlaneDt = emUp ; 
				}
				else if(iViewPos == 2 )
				{
					PrvPath[i].Park[j].enPlaneDt = emRight ; 
 				}
				else if(iViewPos == 3 )
				{
					PrvPath[i].Park[j].enPlaneDt = emDown ; 
				}
				else 
				{
					PrvPath[i].Park[j].enPlaneDt = emErrorDir ; 
				}
			}
    
			xSign = ((iViewPos>1)?-1:1);
		    ySign = ((iViewPos == 0 || iViewPos == 3 )?-1:1) ; 

			PrvPath[i].TakeOff.ptPathPos.x = PubPath[i*13].ptPathPos.x + xSign *PathWidth ; 
			PrvPath[i].TakeOff.ptPathPos.y = PubPath[i*13].ptPathPos.y + ySign *PathHeight ; 

			if(iViewPos == 0 )
			{
				PrvPath[i].TakeOff.enPlaneDt = emDownLeft ; 
			}
			else if(iViewPos == 1)
			{
				PrvPath[i].TakeOff.enPlaneDt = emLeftUp ; 
			}
			else if(iViewPos == 2 )
			{
				PrvPath[i].TakeOff.enPlaneDt = emUpRight ; 
			}
			else if(iViewPos == 3 )
			{
				PrvPath[i].TakeOff.enPlaneDt = emRightDown ; 
			}
			else 
			{
				PrvPath[i].TakeOff.enPlaneDt = emErrorDir ; 
			}

			if(iViewPos%2 ==  0)
			{
				xSign = 0 ; 
				ySign = ((iViewPos == 2)?-1:1); 
			}
			else
			{
				xSign = ((iViewPos == 1)?-1:1) ; 
				ySign = 0 ;
			}

			for(int  j = 0 ; j<PRIVATE_PATH_STEP ; j++)
			{
				PrvPath[i].Landing[j].ptPathPos.x =  PubPath[(i+3)%PLAY_COUNT *13 +10].ptPathPos.x + xSign *(j + 1)*PathWidth ;
				PrvPath[i].Landing[j].ptPathPos.y =  PubPath[(i+3)%PLAY_COUNT *13 +10].ptPathPos.y + ySign *(j + 1) *PathHeight ;

				if(iViewPos == 0 )
				{
					PrvPath[i].Landing[j].enPlaneDt = emDown ; 
				}
				else if(iViewPos == 1)
				{
					PrvPath[i].Landing[j].enPlaneDt = emLeft ; 
				}
				else if(iViewPos == 2 )
				{
					PrvPath[i].Landing[j].enPlaneDt = emUp ; 
				}
				else if(iViewPos == 3 )
				{
					PrvPath[i].Landing[j].enPlaneDt = emRight ; 
				}
				else 
				{
					PrvPath[i].Landing[j].enPlaneDt = emErrorDir ; 
				}
			}
		}
	}
};


///��ҵ�����(������˱�������)
struct   PLAYER
{
	BYTE            cbSide;		               //��ҵ���ɫ
	PLANE           Plane[PLAYER_PLANE_NUM];   //��ҵķɻ�
	MAGIC           Magic[MAX_MAGIC_NUM] ;     //���ӵ�е�ħ������ǰֻ���ɽ趫�磩
	
	PLAYER()
	{
		cbSide = 255 ; 
		memset(Plane , 0 , sizeof(Plane)) ; 
		memset(Magic , 0 , sizeof(Magic)) ;
	}
	void Reset()
	{
		cbSide = 255 ; 
		memset(Plane , 0 , sizeof(Plane)) ; 
		memset(Magic , 0 , sizeof(Magic)) ;
	}
} ;
///��Ϸ����(���ͻ���ʹ��)
struct  GameScence
{
	BYTE          cbMySide ;                                   ///���Լ��ķ�λ
	BYTE          cbGameStation[PLAY_COUNT] ;                  ///��Ϸ״̬
	PLANE         PlaneIndex[PLAY_COUNT][PLAYER_PLANE_NUM] ;   ///��ǰÿ���˷ɻ���״��
	MAGIC         MagicIndex[PLAY_COUNT][MAX_MAGIC_NUM] ;      ///���ӵ�е�ħ������ǰֻ���ɽ趫�磩
	
	GameScence()
	{
		cbMySide = 255 ; 
		memset(cbGameStation , 0 , sizeof(cbGameStation)) ; 
		memset(MagicIndex , 0 , sizeof(MagicIndex)) ; 
		for(int i = 0 ; i<PLAY_COUNT ; i++)
		{
		  for (int j = 0 ; j<PLAYER_PLANE_NUM ; j++)
		  {
			  PlaneIndex[i][j].Reset() ; 
		  }
		}
	}
	void Reset()
	{
		cbMySide = 255 ; 
		memset(cbGameStation , 0 , sizeof(cbGameStation)) ; 
		memset(MagicIndex , 0 , sizeof(MagicIndex)) ; 
		for(int i = 0 ; i<PLAY_COUNT ; i++)
		{
			for (int j = 0 ; j<PLAYER_PLANE_NUM ; j++)
			{
				PlaneIndex[i][j].Reset() ; 
			}
		}
	}
};
///�������߼�
class CUpGradeGameLogic
{
public:
	///���캯��		
	CUpGradeGameLogic(void);
	///��������
	virtual ~CUpGradeGameLogic();

public:
	///�����Ϣ
	PLAYER   m_PlayerSerInfo[PLAY_COUNT] ;    ///�ĸ��˵ķɻ���Ϣ������ڷ���˵���Ϣ���Ǵ�0��ʼ��

	int      m_iPlayerfight[2] ;           ///˫����ս��


public:
	///��ʼ������
	void  InitServerData(void) ;
	///������˵�����ת��Ϊ�ͻ��˵�����
	void  ChangeDate(BYTE  iDeskStation) ; 
	///��������Ƿ��л�ķɻ������ػ�ɻ�������
	int   FindActivePlaneCount(BYTE iDeskStation ) ; 
	///�����м��ܷɻ��Ѿ���ɷ�������
	int   FindPlaneFinishCount(BYTE iDeskStation) ; 
	///�ҵ�һ�����Էɵķɻ�
	int   FindOnePlaneToFly(BYTE  bDeskStation , BYTE cbDiceValue) ; 
	///��ȡ�õ��ж��ٸ��з�������
	int   GetEnemyInStepCount(BYTE cbSide, BYTE cbStep ,PLANE Plane[])  ;
	///��ȡ�õ��ж��ٸ��з�������
	int   GetEnemyInStepCount(BYTE cbSide, BYTE cbStep ,GameScence ScenceData ) ;
	///��ȡ�ĵ��ж��ٸ��Լ�������
	int   GetMyselfInStepCount(BYTE  cbSide , BYTE cbStep , PLANE Plane[]) ; 
	///��ȡ�ĵ��ж��ٸ��Լ�������
	int   GetMyselfInStepCount(BYTE cbSide , BYTE cbStep , GameScence ScenceData) ;
	///��ȡ�ڸõ����Ƿ��ܷ���
	bool  IsCanFly(BYTE cbSide , BYTE cbStep ) ; 
	///�Ƿ�����߽ݾ�
	bool  IsCanShortCut(BYTE  cbSide , BYTE cbStep ) ; 
	///�ж�����Ƿ�����߽ݾ�
	bool  IsCanShortCut(BYTE cbSide , BYTE cbStep ,GameScence ScenceData ) ;
	///�ж�ָ���ķɻ��Ƿ��ܹ�����
	bool  IsCanMove(BYTE bDeskStation , BYTE inDex ,BYTE cbDiceValue) ;
	///��ҷ��������ͻ����Ǹ��ݷ����������ı��
	int   PlayerPlaneFly(BYTE  bDeskStation , BYTE cbDiceValue , int inDex , PlaneFlyStruct  PlaneActiveTrack[]) ; 
	///�����Ϸ�Ƿ����
	bool  CheckGameFinish(BYTE bDeskStation ) ; 
	///�ҵ�һ�����Էɵķɻ�
	int   FindOnePlaneToFly(BYTE  bDeskStation , BYTE cbDiceValue ,GameScence ScenceData) ; 
	/// ��ȡ�ɻ����е�λ��
	int  GetPlaneFlyPos(BYTE bDeskStation , int inDex ,int cbDiceValue , GameScence ScenceData) ;
	///��ȡ�ɻ���ƫ������
	int  GetPlanePixIndex(BYTE bDeskStation ,int inDex ,  BYTE  cbStep , GameScence ScenceData ) ;

};
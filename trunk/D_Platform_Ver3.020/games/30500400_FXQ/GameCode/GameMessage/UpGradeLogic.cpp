#include "StdAfx.h"
#include "UpGradeLogic.h"

//���캯��
CUpGradeGameLogic::CUpGradeGameLogic(void)
{
	memset(m_iPlayerfight , 0 , sizeof(m_iPlayerfight)) ; 
	
}
//���캯��
CUpGradeGameLogic::~CUpGradeGameLogic()
{

}
///��ʼ������
void CUpGradeGameLogic::InitServerData(void)
{
	///�涨�߼�0��λ��Ϊ�̷�
	for (int i = 0 ; i<PLAY_COUNT ; i++)
	{
		m_PlayerSerInfo[i].Reset() ;

		m_PlayerSerInfo[i].cbSide = i; 
		for(int  j = 0 ; j< PLAYER_PLANE_NUM ; j++)
		{
			m_PlayerSerInfo[i].Plane[j].emSide = i;   
			m_PlayerSerInfo[i].Plane[j].emPlaneState =  emStandby ; 
			m_PlayerSerInfo[i].Plane[j].cbStep = STEP_HOME ; 
			m_PlayerSerInfo[i].Plane[j].cbAirPortPos  = i; 
			m_PlayerSerInfo[i].Plane[j].cbLandPos = j ; 
		}

		for(int j = 0 ; j<MAX_MAGIC_NUM ; j++)
		{
			m_PlayerSerInfo[i].Magic[j].cbMagic = j+1 ;  ///ħ�����
			m_PlayerSerInfo[i].Magic[j].cbCount = 1; ///ħ������
		}
	}

	memset(m_iPlayerfight , 0 , sizeof(m_iPlayerfight)) ; 

	return ; 
}
///�ж�����Ƿ��п��Ի�ķɻ���ͬʱ���طɻ��Ƿ��ܷ���
int  CUpGradeGameLogic::FindActivePlaneCount(BYTE iDeskStation)
{
	if(iDeskStation>=PLAY_COUNT)
	{
		return  0; 
	}
	int  iCount = 0 ; 
	for(int i = 0 ; i<PLAYER_PLANE_NUM ; i++)
	{
		if(m_PlayerSerInfo[iDeskStation].Plane[i].emPlaneState>emStandby 
			&& m_PlayerSerInfo[iDeskStation].Plane[i].emPlaneState<emStop)
		{
			iCount ++ ; 
		}
	}

	return iCount ; 
}
///����м��ܷɻ���ɷ�������
int  CUpGradeGameLogic::FindPlaneFinishCount(BYTE iDeskStation) 
{
	if(iDeskStation>=PLAY_COUNT)
	{
		return  0; 
	}
	int  iCount = 0 ; 

	for(int i = 0 ; i<PLAYER_PLANE_NUM ; i++)
	{
		if(m_PlayerSerInfo[iDeskStation].Plane[i].emPlaneState == emStop) 
		{
			iCount ++ ; 
		}
	}
	return iCount ; 
}
///�ҵ�һ�����Է��еķɻ�
int CUpGradeGameLogic::FindOnePlaneToFly(BYTE  bDeskStation , BYTE cbDiceValue) 
{
	for(int i = 0 ; i<PLAYER_PLANE_NUM ; i++)
	{
		if(cbDiceValue == 6)
		{
			if(m_PlayerSerInfo[bDeskStation].Plane[i].emPlaneState == emStandby) ///Ϊ6������ڷɻ����ķɻ�
			{
				return i ;
			}
			else
			{
				if(m_PlayerSerInfo[bDeskStation].Plane[i].emPlaneState != emStop)
				{
					return i ; 
				}
			}
		}
		else
		{
			if(m_PlayerSerInfo[bDeskStation].Plane[i].emPlaneState>emStandby 
				&& m_PlayerSerInfo[bDeskStation].Plane[i].emPlaneState<emStop)
			{
				return i ; 
			}
		}
	}
	return  -1 ; 
}
///�ҵ�һ�����Է��еķɻ�
int CUpGradeGameLogic::FindOnePlaneToFly(BYTE  bDeskStation , BYTE cbDiceValue ,GameScence ScenceData) 
{
	for(int i = 0 ; i<PLAYER_PLANE_NUM ; i++)
	{
		if(cbDiceValue == 6)
		{
			if(ScenceData.PlaneIndex[bDeskStation][i].emPlaneState == emStandby) ///Ϊ6������ڷɻ����ķɻ�
			{
				return i ;
			}
			else
			{
				if(ScenceData.PlaneIndex[bDeskStation][i].emPlaneState != emStop)
				{
					return i ; 
				}
			}
		}
		else
		{
			if(ScenceData.PlaneIndex[bDeskStation][i].emPlaneState>emStandby 
				&& ScenceData.PlaneIndex[bDeskStation][i].emPlaneState<emStop)
			{
				return i ; 
			}
		}
	}
	return  -1 ; 
}
///��ȡ�ĵ��ж��ٸ��Լ�������
int CUpGradeGameLogic::GetMyselfInStepCount(BYTE  cbSide , BYTE cbStep , PLANE Plane[]) 
{
	if(cbSide == emInvalidate )
	{
		return 0 ;  ///
	}

	int  iPlaneCount = 0 ;

	for(int  j = 0 ; j<PLAY_COUNT ; j++)
	{
		if(m_PlayerSerInfo[cbSide].Plane[j].emPlaneState == emFlying ||m_PlayerSerInfo[cbSide].Plane[j].emPlaneState== emLanding)
		{
			if(m_PlayerSerInfo[cbSide].Plane[j].cbStep == cbStep)
			{
				Plane[iPlaneCount] = m_PlayerSerInfo[cbSide].Plane[j] ; 
				iPlaneCount ++ ;
			}
		}
	}
	return iPlaneCount ;

}
///��ȡ�ĵ��ж��ٸ��Լ�������
int CUpGradeGameLogic::GetMyselfInStepCount(BYTE cbSide , BYTE cbStep , GameScence ScenceData) 
{
	if(cbSide == emInvalidate )
	{
		return 0 ;  ///
	}

	int  iPlaneCount = 0 ;

	for(int  j = 0 ; j<PLAY_COUNT ; j++)
	{
		if(ScenceData.PlaneIndex[cbSide][j].emPlaneState == emFlying || ScenceData.PlaneIndex[cbSide][j].emPlaneState== emLanding)
		{
			if(ScenceData.PlaneIndex[cbSide][j].cbStep == cbStep)
			{
				iPlaneCount ++ ;
			}
		}
	}
	return iPlaneCount ;
}
///��ȡ�õ��ж��ٸ��з�������
int  CUpGradeGameLogic::GetEnemyInStepCount(BYTE cbSide, BYTE cbStep ,PLANE Plane[]) 
{
	if(cbSide == emInvalidate )
	{
		return 0 ;  ///
	}

	if(cbStep >49)  ///�ڴ˴��ǲ������ез��ķɻ���
	{
		return 0 ; 
	}

	int  iPlaneCount = 0 ; 

	for(int  i = 0 ; i<PLAY_COUNT ; i++)
	{
		if((PLAY_COUNT + cbSide - i)%2 == 0 ) ///�Լ����ķɻ�
		{
			continue;
		}

		for(int  j = 0 ; j<PLAY_COUNT ; j++)
		{
			if(m_PlayerSerInfo[i].Plane[j].emPlaneState != emFlying) ///���ڹ�����·�ķɻ�
			{
				continue; 
			}

			if(GetMyStepIndex((enumSide)i ,m_PlayerSerInfo[i].Plane[j].cbStep) == GetMyStepIndex((enumSide)cbSide,cbStep))
			{
				Plane[iPlaneCount ++ ] = m_PlayerSerInfo[i].Plane[j] ; 
			}
		}
	}

	return iPlaneCount ; 
}
///���¹��ͻ���ʹ��
///��ȡ�õ��ж��ٸ��з�������
int  CUpGradeGameLogic::GetEnemyInStepCount(BYTE cbSide, BYTE cbStep ,GameScence ScenceData ) 
{
	if(cbSide == emInvalidate )
	{
		return 0 ;  ///
	}
	if(cbStep >49)  ///�ڴ˴��ǲ������ез��ķɻ���
	{
		return 0 ; 
	}

	int  iPlaneCount = 0 ; 

	for(int  i = 0 ; i<PLAY_COUNT ; i++)
	{
		if((PLAY_COUNT + cbSide - i)%2 == 0 ) ///�Լ����ķɻ�
		{
			continue;
		}

		for(int  j = 0 ; j<PLAY_COUNT ; j++)
		{
			if(ScenceData.PlaneIndex[i][j].emPlaneState != emFlying) ///���ڹ�����·�ķɻ�
			{
				continue; 
			}

			if(GetMyStepIndex((enumSide)i ,ScenceData.PlaneIndex[i][j].cbStep) == GetMyStepIndex((enumSide)cbSide,cbStep))
			{
				iPlaneCount ++ ;
			}
		}
	}

	return iPlaneCount ; 
}
///��������Ƿ���ɷ�������
bool CUpGradeGameLogic::CheckGameFinish(BYTE bDeskStation)
{
	return (FindPlaneFinishCount(bDeskStation) == PLAYER_PLANE_NUM) ; 
}
///��ȡ�ڸõ����Ƿ��ܷ���
bool CUpGradeGameLogic::IsCanFly(BYTE cbSide , BYTE cbStep) 
{
	if(cbSide == emInvalidate )
	{
		return false ;  ///
	}
	if(cbStep <1 ||cbStep >=49) ///�����Լ���λ�þͲ��ܷ���
	{
		return false ; 
	}
	if((cbStep + 3 )%PLAY_COUNT == 0 ) ///���Լ��ĸ���
	{
		return true ; 
	}

	return false ; 
}
///�ж�����Ƿ�����߽ݾ�
bool   CUpGradeGameLogic::IsCanShortCut(BYTE cbSide , BYTE cbStep )
{
	if(cbStep == emInvalidate)
	{
		return false ; 
	}
	if(cbStep != SHORTCUT_STEP_VALUE)
	{
		return false ; 
	}

	PLANE planeIndex[8] ; 
	if(GetEnemyInStepCount(cbSide ,SHORTCUT_STEP_VALUE ,planeIndex)>1)
	{
		return false ; 
	}
	return true ;  

}
///�ж�һ���ɻ��Ƿ��ܷ���
bool  CUpGradeGameLogic::IsCanMove(BYTE bDeskStation , BYTE inDex ,BYTE cbDiceValue)
{
	if(bDeskStation >=PLAY_COUNT)
	{
		return false ;
	}

	if(m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState == emStop)
	{
		return false ; 
	}

	if(cbDiceValue == 6)
	{
		return true ; 
	}
	else
	{
		if(m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState<emTakeoff 
			||m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState>emLanding)
		{
			return false ; 
		}
	}

	return true ; 
}
///��������ж�
int  CUpGradeGameLogic::PlayerPlaneFly(BYTE bDeskStation ,BYTE cbDiceValue ,int inDex ,PlaneFlyStruct PlaneActiveTrack[])
{
	if(bDeskStation <0 ||bDeskStation >=PLAY_COUNT)
	{
		return 0 ; 
	}

	int iActivePlaneCount = 0 ; //��Ҫ����ķɻ������� 0��Ϊָ�����Լ��ķɻ�

	if(inDex < 0 ||inDex >=PLAYER_PLANE_NUM )
	{
		return 0 ; 
	}
	switch(m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState)
	{
	case  emStandby:
		{
			int  iComeByCount = 0 ;       ///��Ҫ�ƶ��Ĳ���

			if(cbDiceValue == 6 ) ///ֻ��6��������
			{
				PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = STEP_HOME ;
				PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = STEP_TAKEOFF ;
				PlaneActiveTrack[iActivePlaneCount].cbStepCount = iComeByCount ; 

				PlaneActiveTrack[iActivePlaneCount].cbSide   = bDeskStation ;
				PlaneActiveTrack[iActivePlaneCount].cbIndex   = inDex ;  
				PlaneActiveTrack[iActivePlaneCount].cbOperateType = emTakeOfFActive ; 
				iActivePlaneCount ++ ; 
				m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState = emTakeoff ;
				m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep = STEP_TAKEOFF ; 
			}
			return iActivePlaneCount ; 
		}
	case  emTakeoff :
	case  emFlying:
		{
			bool   bNeedBack  = false ;     ///�Ƿ���Ҫ����  
			bool   bCanFly    = false ;        ///�Ƿ���Է���
			int    iComeByCount = 0 ;       ///��Ҫ�ƶ��Ĳ���
			int    cbTempStep = -1 ;       ///�е��ӵ�λ��
			int    cbStartPos ;         ///��ʼλ��

			int    iMyselfCount = 0 ;

			PLANE  planeIndex[8] ;
			PLANE  mySelfIndex[4] ; //�Լ��ķɻ�����
			memset(planeIndex , 0 , sizeof(planeIndex)) ; 
			memset(mySelfIndex , 0 , sizeof(mySelfIndex)) ; 

			if(m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState == emTakeoff)
			{
				cbStartPos     = -1 ;
				iMyselfCount   = 1 ;
				mySelfIndex[0] = m_PlayerSerInfo[bDeskStation].Plane[inDex] ;

				PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = STEP_TAKEOFF ; 
			}
			else
			{
				cbStartPos = m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep ; 
				PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = cbStartPos ; 

				iMyselfCount = GetMyselfInStepCount(bDeskStation ,cbStartPos ,mySelfIndex) ; 
			}

			PlaneActiveTrack[iActivePlaneCount].cbSide = bDeskStation ; 
			PlaneActiveTrack[iActivePlaneCount].cbIndex = inDex ; 

			for(int i = cbStartPos + 1 ; i < cbStartPos + cbDiceValue  ; i++) ///�ݲ����������һ��(���һ�������з���)
			{
				PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = i ;

				if(GetEnemyInStepCount(bDeskStation,i ,planeIndex) >iMyselfCount)  ///1��ʱ���ᵽ������
				{
					bNeedBack = true ; 
					cbTempStep = i ;                ///������Ͳ���������
					break;
				}
			}

			if(bNeedBack == true) ///�з��е��ӵ����(����״����û��ײ����)
			{
				int iEndPos = 2*cbTempStep - cbDiceValue - cbStartPos; 

				if( iEndPos >=0)
				{
					for(int j = cbTempStep - 1 ; j >= iEndPos ; j-- )
					{
						PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] =  j ;
					}

					PlaneActiveTrack[iActivePlaneCount].cbStepCount = iComeByCount ; 

					m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep = iEndPos ;
					m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState = emFlying ; 
				}
				else
				{
					for(int j = cbTempStep - 1 ; j>=0 ; j-- )
					{
						PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] =  j ;
					}

					PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = STEP_TAKEOFF ;
					PlaneActiveTrack[iActivePlaneCount].cbStepCount = iComeByCount ; 

					m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep = STEP_TAKEOFF ;
					m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState = emTakeoff ;  ///���»ص����λ��
				}

				PlaneActiveTrack[iActivePlaneCount].cbOperateType = emBackActive ; 
				iActivePlaneCount ++ ;
				
			}
			else  ///���������û�е���,������һ��
			{
				int  iEnemyPlaneCount =  GetEnemyInStepCount(bDeskStation ,cbDiceValue + cbStartPos ,planeIndex) ;
				Msg("wysoutfxqlogic��ʼ���ɻ������һ�����Ĳ�cbDiceValue==%d ,cbStartPos==%d ,%d ,�з��ɻ��ĸ���==%d",cbDiceValue , cbStartPos ,cbDiceValue + cbStartPos ,iEnemyPlaneCount); 
			
				if(iEnemyPlaneCount >0 ) ///���һ���ел�����������
				{
					if(iEnemyPlaneCount <=iMyselfCount) ///����Լ��ķɻ����ڵ��ڵз��ķɻ�
					{
						PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = cbDiceValue + cbStartPos;
						PlaneActiveTrack[iActivePlaneCount].cbOperateType = emFlyingActive ; 

						m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep = cbDiceValue + cbStartPos ;
						m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState = emFlying ;

						m_iPlayerfight[bDeskStation%2] += iEnemyPlaneCount ;
					}
					else
					{
						PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = cbDiceValue + cbStartPos;
						PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = STEP_HOME;
						PlaneActiveTrack[iActivePlaneCount].cbOperateType = emHomeActive ; 

						m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep = STEP_HOME ;
						m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState = emStandby  ; ///�ؼ���

						//m_iPlayerfight[(bDeskStation+1)%2] += iMyselfCount ; 
					}

					PlaneActiveTrack[iActivePlaneCount].cbStepCount = iComeByCount ; 

					for(int i = 0 ; i <iEnemyPlaneCount ; i++) ///����з��ķɻ�
					{
						iActivePlaneCount ++ ;
						iComeByCount = 0 ;  ///�����������ɻ��Ĵ���
						int   bOtherDesk  = planeIndex[i].emSide ;
						int   bOtherIndex = planeIndex[i].cbLandPos ;  
						BYTE  cbStartStep =planeIndex[i].cbStep; 

						PlaneActiveTrack[iActivePlaneCount].cbSide = bOtherDesk ; 
						PlaneActiveTrack[iActivePlaneCount].cbIndex = bOtherIndex ; 

						PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = cbStartStep ; 
						PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = STEP_HOME ;
						PlaneActiveTrack[iActivePlaneCount].cbStepCount = iComeByCount ; 
						PlaneActiveTrack[iActivePlaneCount].cbOperateType = emHomeActive ; 

						m_PlayerSerInfo[bOtherDesk].Plane[bOtherIndex].cbStep = STEP_HOME ;
						m_PlayerSerInfo[bOtherDesk].Plane[bOtherIndex].emPlaneState = emStandby ; 

						//m_iPlayerfight[bDeskStation%2] ++ ; 
					}

					iActivePlaneCount ++ ; 

				}
				else ///���һ��û�ез��ķɻ�
				{
					if(IsCanShortCut(bDeskStation ,cbDiceValue + cbStartPos))  ///�Ƿ���Է���
					{
						int  iOtherStepCount = GetEnemyInStepCount(bDeskStation ,SHORTCUT_OTHER_STEP ,planeIndex) ;  

						if( iOtherStepCount== iMyselfCount)  //��ȵ�����´���з��ķɻ�
						{
							PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = cbDiceValue + cbStartPos ;
							PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = SHORTCUT_OTHER_STEP;

							PlaneActiveTrack[iActivePlaneCount].cbStepCount = iComeByCount ; 
							PlaneActiveTrack[iActivePlaneCount].cbOperateType = emShortCutActive ; 

							m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep = SHORTCUT_OTHER_STEP ;
							m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState = emFlying ; 

							iActivePlaneCount ++ ; 

							for(int  i = 0 ; i < iOtherStepCount ; i++)
							{
								iComeByCount = 0 ; 

								PlaneActiveTrack[iActivePlaneCount].cbSide = planeIndex[i].emSide ; 
								PlaneActiveTrack[iActivePlaneCount].cbIndex = planeIndex[i].cbLandPos ; 

								PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = SHORTCUT_OTHER_STEP ; 
								PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = STEP_HOME ; 

								PlaneActiveTrack[iActivePlaneCount].cbStepCount = iComeByCount ; 

								m_PlayerSerInfo[planeIndex[i].emSide].Plane[planeIndex[i].cbLandPos].cbStep = STEP_HOME ;
								m_PlayerSerInfo[planeIndex[i].emSide].Plane[planeIndex[i].cbLandPos].emPlaneState = emStandby ; 

								m_iPlayerfight[bDeskStation%2] ++ ; 

								iActivePlaneCount ++ ; 
							}
						}
						else if(iOtherStepCount <iMyselfCount)
						{
							bCanFly = true ; 
							int inDexEnemy = 0 ; 
							for(int  i = SHORTCUT_OTHER_STEP;i<SHORTCUT_OTHER_STEP +4 ; i++ )
							{
								if(GetEnemyInStepCount(bDeskStation ,i,planeIndex) >iMyselfCount)
								{
									bCanFly = false ;
									i = inDexEnemy ; 
									break;
								}
							}

							if(bCanFly == true) ///�����ٷ�һ��
							{
								PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = SHORTCUT_STEP_VALUE ;
								for(int  i = SHORTCUT_OTHER_STEP;i<SHORTCUT_OTHER_STEP +5 ; i++ )
								{
									PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = i ;
								}
								PlaneActiveTrack[iActivePlaneCount].cbOperateType = emShortCutActive ; 
								PlaneActiveTrack[iActivePlaneCount].cbStepCount = iComeByCount ; 

								m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep = SHORTCUT_OTHER_STEP +4 ;
								m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState = emFlying ;

								iActivePlaneCount ++ ; 
								
							}
							else  //���ٷ���
							{
								PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = SHORTCUT_STEP_VALUE ;
								PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = SHORTCUT_OTHER_STEP ;

								PlaneActiveTrack[iActivePlaneCount].cbStepCount = iComeByCount  ; 

								m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep = SHORTCUT_OTHER_STEP ;
								m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState = emFlying ;

								iActivePlaneCount ++ ; 
							}
						}

						if(iOtherStepCount <=iMyselfCount)  ///����������ȿ�����
						{
							BYTE  iSpecifySide    = m_PlayerSerInfo[bDeskStation].Plane[inDex].emSide ; 
							BYTE  iSpecifyStation = m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState ; 
							BYTE  iSpecifyStep    =  m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep ; 

							///��һ���Լ��ķɻ��ж��ټ�
							for(int  i = 0 ; i <iMyselfCount ; i++)   ///���������ͬһλ�÷ɻ���·��
							{
								BYTE  cbMyTempside = mySelfIndex[i].emSide ; 
								BYTE  cbMyInDex    = mySelfIndex[i].cbLandPos ;

								if(cbMyInDex == inDex)
								{
									continue;
								}

								PlaneActiveTrack[iActivePlaneCount] = PlaneActiveTrack[0] ; 

								PlaneActiveTrack[iActivePlaneCount].cbSide  = cbMyTempside ;
								PlaneActiveTrack[iActivePlaneCount].cbIndex = cbMyInDex ; 

								m_PlayerSerInfo[cbMyTempside].Plane[cbMyInDex].cbStep = iSpecifyStep ;
								m_PlayerSerInfo[cbMyTempside].Plane[cbMyInDex].emPlaneState = iSpecifyStation ; 

								iActivePlaneCount ++ ; 
							}

							return iActivePlaneCount ; 
						}
					}

					if(IsCanFly(bDeskStation , cbDiceValue + cbStartPos )) ///�������Լ��ĸ��ӣ�
					{
						bCanFly = true ; 
						for(int  i = cbDiceValue + cbStartPos;i<cbDiceValue + cbStartPos +4 ; i++ )
						{
							if(GetEnemyInStepCount(bDeskStation ,i,planeIndex) >iMyselfCount)
							{
								bCanFly = false ; 
								break;
							}
						}
					}

					if(bCanFly == true) ///��Ҫ����4�� �������һ��
					{
						for(int  i = 0 ; i<5 ; i++)
						{
							PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = cbDiceValue + i + cbStartPos ;
						}

						PlaneActiveTrack[iActivePlaneCount].cbStepCount = iComeByCount ; 
						PlaneActiveTrack[iActivePlaneCount].cbOperateType = emFlyingActive ;

						m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep = cbDiceValue + cbStartPos + 4 ;
						m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState = emFlying ;
					}
					else  ///�϶����ﲻ�����л��˵Ŀ���
					{
						PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = cbDiceValue + cbStartPos ;
						
						PlaneActiveTrack[iActivePlaneCount].cbOperateType = emFlyingActive ; 

						m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep = cbDiceValue + cbStartPos ;

						if((cbDiceValue + cbStartPos) >=50 && (cbDiceValue + cbStartPos) < (FINISH_PATH_STEP - 1))
						{
							m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState = emLanding ;
						}
						else if(cbDiceValue + cbStartPos == FINISH_PATH_STEP - 1)
						{
							PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = STEP_HOME ;

							PlaneActiveTrack[iActivePlaneCount].cbOperateType = emArriveHome ; 

							m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState = emStop ;
							m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep  = STEP_HOME ; 
						}
						else
						{
							m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState = emFlying ;
						}

						PlaneActiveTrack[iActivePlaneCount].cbStepCount = iComeByCount ; 
						
					}

					if(IsCanShortCut(bDeskStation ,m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep)) //�����߽ݾ�
					{
						memset(planeIndex , 0 , sizeof(planeIndex)) ; 

						int  iLastStepEnemyCount = GetEnemyInStepCount(bDeskStation ,SHORTCUT_OTHER_STEP ,planeIndex) ; 

						if( iLastStepEnemyCount <iMyselfCount)
						{
							PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = SHORTCUT_STEP_VALUE ;
							PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = SHORTCUT_OTHER_STEP ;

							PlaneActiveTrack[iActivePlaneCount].cbStepCount = iComeByCount  ;
							PlaneActiveTrack[iActivePlaneCount].cbOperateType = emShortCutActive ; 

							m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep = SHORTCUT_OTHER_STEP ;
							m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState = emFlying ;

						}
						else if(iLastStepEnemyCount == iMyselfCount)
						{
							PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = SHORTCUT_STEP_VALUE ;
							PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = SHORTCUT_OTHER_STEP ;

							PlaneActiveTrack[iActivePlaneCount].cbStepCount = iComeByCount  ; 
							PlaneActiveTrack[iActivePlaneCount].cbOperateType = emShortCutActive ; 

							m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep = SHORTCUT_OTHER_STEP ;
							m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState = emFlying ;
							
							iActivePlaneCount ++ ; 

							for(int  i = 0 ; i < iLastStepEnemyCount ; i++)
							{
								iComeByCount = 0 ; 

								PlaneActiveTrack[iActivePlaneCount].cbSide = planeIndex[i].emSide ; 
								PlaneActiveTrack[iActivePlaneCount].cbIndex = planeIndex[i].cbLandPos ; 

								PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = SHORTCUT_OTHER_STEP ; 
								PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = STEP_HOME ; 

								PlaneActiveTrack[iActivePlaneCount].cbStepCount = iComeByCount ; 

								m_PlayerSerInfo[planeIndex[i].emSide].Plane[planeIndex[i].cbLandPos].cbStep = STEP_HOME ;
								m_PlayerSerInfo[planeIndex[i].emSide].Plane[planeIndex[i].cbLandPos].emPlaneState = emStandby ; 

								m_iPlayerfight[bDeskStation%2] ++ ; 

								iActivePlaneCount ++ ; 
							}
						}
					}

					BYTE cbTermalStep = m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep ; 

					if(cbTermalStep>=0 &&cbTermalStep<50)
					{
						///�����ʵ�����һ���Ƿ��зɻ�
						memset(planeIndex , 0 ,sizeof(planeIndex)) ; 

						Msg("wysoutfxqlogic��ʼ���ɻ������һ�����Ĳ�cbDiceValue==%d ,cbStartPos==%d ",cbDiceValue , cbStartPos); 

						int  iEnemyPlaneCount =  GetEnemyInStepCount(bDeskStation ,cbTermalStep ,planeIndex) ;

						if(iEnemyPlaneCount >0 ) ///���һ���ел�����������
						{
							if(iEnemyPlaneCount <=iMyselfCount) ///����һ�ܷɻ��ؼ���
							{
								PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] =  cbTermalStep;
								PlaneActiveTrack[iActivePlaneCount].cbOperateType = emFlyingActive ; 

								m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep = cbTermalStep ;
								m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState = emFlying ;
								m_iPlayerfight[bDeskStation%2]+= iEnemyPlaneCount ;
							}
							else
							{
								PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = STEP_HOME;
								PlaneActiveTrack[iActivePlaneCount].cbOperateType = emHomeActive ; 

								m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep = STEP_HOME ;
								m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState = emStandby  ; ///�ؼ���

								//m_iPlayerfight[(bDeskStation + 1)%2] += iMyselfCount;
							}

							PlaneActiveTrack[iActivePlaneCount].cbStepCount = iComeByCount ; 

							for(int i = 0 ; i <iEnemyPlaneCount ; i++) ///����з��ķɻ�
							{
								iActivePlaneCount ++ ;
								iComeByCount = 0 ;  ///�����������ɻ��Ĵ���
								int   bOtherDesk  = planeIndex[i].emSide ;
								int   bOtherIndex = planeIndex[i].cbLandPos ;  
								BYTE  cbStartStep =planeIndex[i].cbStep; 

								PlaneActiveTrack[iActivePlaneCount].cbSide = bOtherDesk ; 
								PlaneActiveTrack[iActivePlaneCount].cbIndex = bOtherIndex ; 

								PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = cbTermalStep ; 
								PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = STEP_HOME ;
								PlaneActiveTrack[iActivePlaneCount].cbStepCount = iComeByCount ; 
								PlaneActiveTrack[iActivePlaneCount].cbOperateType = emHomeActive ; 

								m_PlayerSerInfo[bOtherDesk].Plane[bOtherIndex].cbStep = STEP_HOME ;
								m_PlayerSerInfo[bOtherDesk].Plane[bOtherIndex].emPlaneState = emStandby ;

								//m_iPlayerfight[bDeskStation%2] ++ ;
							}
						}
					}

					iActivePlaneCount ++ ; 
				}
			}

			BYTE  iSpecifySide    = m_PlayerSerInfo[bDeskStation].Plane[inDex].emSide ; 
			BYTE  iSpecifyStation = m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState ; 
			BYTE  iSpecifyStep    =  m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep ; 

			///��һ���Լ��ķɻ��ж��ټ�
			for(int  i = 0 ; i <iMyselfCount ; i++)   ///���������ͬһλ�÷ɻ���·��
			{
				BYTE  cbMyTempside = mySelfIndex[i].emSide ; 
				BYTE  cbMyInDex    = mySelfIndex[i].cbLandPos ;

				if(cbMyInDex == inDex)
				{
					continue;
				}

				PlaneActiveTrack[iActivePlaneCount] = PlaneActiveTrack[0] ; 

				PlaneActiveTrack[iActivePlaneCount].cbSide  = cbMyTempside ;
				PlaneActiveTrack[iActivePlaneCount].cbIndex = cbMyInDex ; 

				m_PlayerSerInfo[cbMyTempside].Plane[cbMyInDex].cbStep = iSpecifyStep ;
				m_PlayerSerInfo[cbMyTempside].Plane[cbMyInDex].emPlaneState = iSpecifyStation ; 

				iActivePlaneCount ++ ; 
			}
			return iActivePlaneCount ; 
		}
	case  emLanding:
		{
			int    iComeByCount = 0 ;       ///��Ҫ�ƶ��Ĳ���
			int   cbStartPos = m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep ; 

			PLANE   mySelfIndex[PLAY_COUNT] ;
			memset(mySelfIndex , 0 , sizeof(mySelfIndex)) ; 

			int  iMyselfCount = GetMyselfInStepCount(bDeskStation ,cbStartPos ,mySelfIndex) ; 

			PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = cbStartPos ; 
			PlaneActiveTrack[iActivePlaneCount].cbSide = bDeskStation ; 
			PlaneActiveTrack[iActivePlaneCount].cbIndex = inDex ; 

			if(cbStartPos + cbDiceValue == FINISH_PATH_STEP -1) ///�ɻ���ɷ�������
			{
				for(int i = cbStartPos + 1; i<= cbStartPos + cbDiceValue  ; i++)
				{
					PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = i;
				}

				PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++]  = STEP_HOME ;
				PlaneActiveTrack[iActivePlaneCount].cbStepCount = iComeByCount ; 
				PlaneActiveTrack[iActivePlaneCount].cbOperateType = emArriveHome ; 

				m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep = STEP_HOME ; 
				m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState = emStop ; 
				
			}
			else if(cbStartPos + cbDiceValue >FINISH_PATH_STEP -1) ///��Ҫ����
			{
				for(int i = cbStartPos + 1; i<  FINISH_PATH_STEP   ; i++)
				{
					PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = i;
				}
				for(int i =  FINISH_PATH_STEP - 2 ; i>= 2*(FINISH_PATH_STEP-1) - cbStartPos- cbDiceValue ; i--)
				{
					PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = i;
				}

				PlaneActiveTrack[iActivePlaneCount].cbStepCount = iComeByCount ; 
				PlaneActiveTrack[iActivePlaneCount].cbOperateType = emBackActive ; 

				m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep = 2*(FINISH_PATH_STEP-1) - cbStartPos- cbDiceValue;
			}
			else
			{
				for(int i = cbStartPos + 1; i<=  cbStartPos + cbDiceValue   ; i++)
				{
					PlaneActiveTrack[iActivePlaneCount].cbComeByStepIndex[iComeByCount ++] = i;
				}

				PlaneActiveTrack[iActivePlaneCount].cbStepCount = iComeByCount ; 
				PlaneActiveTrack[iActivePlaneCount].cbOperateType = emFlyingActive ; 

				m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep = cbStartPos + cbDiceValue;
			}

			iActivePlaneCount ++ ; 

			BYTE  iSpecifySide    = m_PlayerSerInfo[bDeskStation].Plane[inDex].emSide ; 
			BYTE  iSpecifyStation = m_PlayerSerInfo[bDeskStation].Plane[inDex].emPlaneState ; 
			BYTE  iSpecifyStep    =  m_PlayerSerInfo[bDeskStation].Plane[inDex].cbStep ; 

			///��һ���Լ��ķɻ��ж��ټ�
			for(int  i = 0 ; i <iMyselfCount ; i++)   ///���������ͬһλ�÷ɻ���·��
			{
				BYTE  cbMyTempside = mySelfIndex[i].emSide ; 
				BYTE  cbMyInDex    = mySelfIndex[i].cbLandPos ;

				if(cbMyInDex == inDex)
				{
					continue;
				}

				PlaneActiveTrack[iActivePlaneCount] = PlaneActiveTrack[0] ; 

				PlaneActiveTrack[iActivePlaneCount].cbSide  = cbMyTempside ;
				PlaneActiveTrack[iActivePlaneCount].cbIndex = cbMyInDex ; 

				m_PlayerSerInfo[cbMyTempside].Plane[cbMyInDex].cbStep = iSpecifyStep ;
				m_PlayerSerInfo[cbMyTempside].Plane[cbMyInDex].emPlaneState = iSpecifyStation ; 

				iActivePlaneCount ++ ; 
			}

			return iActivePlaneCount ; 
		}
		break ;
	case  emStop:
	case  emError:
		{
			return  0; 
		}
	}
	return  iActivePlaneCount ;
}



///�ж�����Ƿ�����߽ݾ�
bool   CUpGradeGameLogic::IsCanShortCut(BYTE cbSide , BYTE cbStep ,GameScence ScenceData )
{
	if(cbStep == emInvalidate)
	{
		return false ; 
	}
	if(cbStep != SHORTCUT_STEP_VALUE)
	{
		return false ; 
	}

	if(GetEnemyInStepCount(cbSide ,SHORTCUT_STEP_VALUE ,ScenceData)>1)
	{
		return false ; 
	}
	return true ;  

}


///���ݵ�ǰ�ĳ�������ҷɻ�����������ȷ����ҵķɻ��ܷɵ���λ��
int  CUpGradeGameLogic::GetPlaneFlyPos(BYTE bDeskStation , int inDex ,int cbDiceValue , GameScence  ScenceData)
{
	if(bDeskStation >=PLAY_COUNT)
	{
		return -1 ; 
	}
	if(inDex <0 ||inDex>=PLAYER_PLANE_NUM)
	{
		return -1 ; 
	}
	int  iDestineStep  = -1 ;  
	switch(ScenceData.PlaneIndex[bDeskStation][inDex].emPlaneState)
	{
	case  emStandby:
		{
			if(cbDiceValue != 6)
			{
				return -1 ; 
			}
			else
			{
				return STEP_TAKEOFF ; 
			}
			break;
		}
	case  emTakeoff:
	case  emFlying:
		{
			bool   bNeedBack  = false ;     ///�Ƿ���Ҫ����  
			bool   bCanFly  = false ;        ///�Ƿ���Է���
			int    iComeByCount = 0 ;       ///��Ҫ�ƶ��Ĳ���
			int    cbTempStep = -1 ;       ///�е��ӵ�λ��
			int    cbStartPos ;         ///��ʼλ��

			int    iMyselfCount = 0 ;

			if(ScenceData.PlaneIndex[bDeskStation][inDex].emPlaneState == emTakeoff)
			{
				iMyselfCount   = 1 ;
				cbStartPos = -1 ;
			}
			else
			{
				cbStartPos = ScenceData.PlaneIndex[bDeskStation][inDex].cbStep ; 
				iMyselfCount = GetMyselfInStepCount(bDeskStation ,cbStartPos ,ScenceData) ; 
			}

			for(int i = cbStartPos +1 ; i < cbStartPos + cbDiceValue  ; i++) ///�ݲ����������һ��(���һ�������з���)
			{
				if(GetEnemyInStepCount(bDeskStation,i,ScenceData) >iMyselfCount)  ///1��ʱ���ᵽ������
				{
					bNeedBack = true ; 
					cbTempStep = i ;                ///������Ͳ���������
					break;
				}
			}

			if(bNeedBack == true) ///�з��е��ӵ����(����״����û��ײ����)
			{
				int iEndPos = 2*cbTempStep - cbDiceValue - cbStartPos; 
				iDestineStep = (iEndPos >0 ?iEndPos:STEP_TAKEOFF) ; 
			}
			else  ///���������û�е���
			{
				int  iEnemyPlaneCount =  GetEnemyInStepCount(bDeskStation ,cbDiceValue + cbStartPos ,ScenceData) ;

				if(iEnemyPlaneCount >0 ) ///���һ���ел�
				{
					if(iEnemyPlaneCount <=iMyselfCount) ///����һ�ܷɻ��ؼ���
					{
						iDestineStep = cbDiceValue + cbStartPos ; 
					}
					else
					{
						iDestineStep = STEP_HOME ; 
					}
				}
				else ///���һ��û�ез��ķɻ�
				{
					if(IsCanShortCut(bDeskStation ,cbDiceValue + cbStartPos))  ///�Ƿ���Է���
					{
						if(GetEnemyInStepCount(bDeskStation ,SHORTCUT_OTHER_STEP ,ScenceData) == iMyselfCount)
						{
							iDestineStep = SHORTCUT_STEP_VALUE  ; 
						}
						else if( GetEnemyInStepCount(bDeskStation ,SHORTCUT_OTHER_STEP ,ScenceData) <iMyselfCount)
						{
							bCanFly = true ; 
							
							for(int  i = SHORTCUT_OTHER_STEP;i<SHORTCUT_OTHER_STEP +4 ; i++ )
							{
								if(GetEnemyInStepCount(bDeskStation ,i,ScenceData) >iMyselfCount)
								{
									bCanFly = false ;
									break;
								}
							}

							if(bCanFly == true) ///�����ٷ�һ��
							{
								iDestineStep = SHORTCUT_OTHER_STEP +4 ;
							}
							else  //���ٷ���
							{
								iDestineStep = SHORTCUT_OTHER_STEP ; 
							}
						}

						return iDestineStep ; 
					}

					if(IsCanFly(bDeskStation , cbDiceValue + cbStartPos)) ///���Է���
					{
						bCanFly = true ; 
						for(int  i = cbDiceValue + cbStartPos ; i<cbDiceValue + cbStartPos +4 ; i++ )
						{
							if(GetEnemyInStepCount(bDeskStation ,i,ScenceData) >iMyselfCount)
							{
								bCanFly = false ; 
								break;
							}
						}
					}

					iDestineStep = (bCanFly?(cbDiceValue + cbStartPos + 4):(cbDiceValue + cbStartPos)); 

					if(IsCanShortCut(bDeskStation ,iDestineStep)) //�����߽ݾ�
					{
						if(GetEnemyInStepCount(bDeskStation ,SHORTCUT_OTHER_STEP ,ScenceData)  < iMyselfCount +1)
						{
							iDestineStep = SHORTCUT_OTHER_STEP ;
						}
					}
				}
			}
			break;
		}
	case  emLanding:
		{
			int  cbStartPos = ScenceData.PlaneIndex[bDeskStation][inDex].cbStep ; 

			if(cbStartPos + cbDiceValue > FINISH_PATH_STEP -1) ///�ɻ���ɷ�������
			{
				iDestineStep = 2*(FINISH_PATH_STEP-1) - cbStartPos- cbDiceValue;
			}
			else
			{
				iDestineStep = cbStartPos + cbDiceValue ; 
			}

			break;
		}
	default:
		{
			break;
		}
	}
	return  iDestineStep ; 
}
/// ���طɻ�������(���������ж��ټܷɻ�)
int CUpGradeGameLogic::GetPlanePixIndex(BYTE bDeskStation ,int inDex , BYTE  cbStep ,GameScence ScenceData ) 
{
	int inDexCount = 0 ; 

	if(cbStep == STEP_TAKEOFF)
	{
		for(int i = 0 ; i<inDex ; i++)
		{
			if(ScenceData.PlaneIndex[bDeskStation][i].emPlaneState == emTakeoff)
			{
				inDexCount ++ ; 
			}
		}
	}
	else if(cbStep>49 && cbStep <55)
	{
		for(int i = 0 ; i<inDex ; i++)
		{
			if(ScenceData.PlaneIndex[bDeskStation][i].cbStep == cbStep)
			{
				inDexCount ++ ; 
			}
		}
	}
	else if(cbStep>=0 &&cbStep<50)
	{
		for(int i = 0 ; i<bDeskStation ; i++)
		{
			for(int j = 0 ; j<PLAYER_PLANE_NUM ; j++)
			{
				if(ScenceData.PlaneIndex[i][j].emPlaneState != emFlying)
				{
					continue;
				}
				
				if((GetMyStepIndex((enumSide)bDeskStation , cbStep) == GetMyStepIndex((enumSide)i ,ScenceData.PlaneIndex[i][j].cbStep)))
				{
					inDexCount ++  ; 
				}
			}
		}

		for(int j = 0 ; j<inDex ; j ++)
		{
			if(ScenceData.PlaneIndex[bDeskStation][j].emPlaneState != emFlying)
			{
				continue;
			}
			if(ScenceData.PlaneIndex[bDeskStation][j].cbStep == cbStep)
			{
				inDexCount ++ ; 
			}
		}
	}
	return inDexCount ; 
}

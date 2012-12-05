/** @file	IMJStateEvent.h
 *	@brief	�ṩ���û���������ຯ���Ľӿڲ���
 *	@author edward
 *	@date 08.08.04
 *
 *  IMJStateEvent��Ҫ�ṩ��ȡ��ǰ��Ϸ��״̬�ĺ���
 */
#ifndef ___AFX_IMJSTATEEVENT_H__C459615F_7FFB_48E4_8742_44148C42F1A3__INCLUDED___
#define ___AFX_IMJSTATEEVENT_H__C459615F_7FFB_48E4_8742_44148C42F1A3__INCLUDED___

#include "typeDef.h"

namespace MyGame
{

/**
 * @ingroup MJFrame
 * @brief IMJStateEvent
 *
 * <p>�� 
 *		IMJStateEvent, ״̬����Ľӿ�
 * </p>
 * @sa
 */	
class IMJStateEvent  
{
public:
	IMJStateEvent();
	virtual ~IMJStateEvent();
	

	/**
	 *  @brief ��ȡ��ǰ���齫״̬
	 *  @return ���ص�ǰ���齫״̬
	 *  
	 *  SO��C�˵��齫״̬��һ����ͬ
	 */	
	virtual int GetMJState();

    /**
    *  @brief ����״̬
    *  @param[in]  nState    ���õ�״̬
    *  @param[in]  bTimelag  TRUE: ��ʱ����  FALSE: ����ʱ
    *
    *  ��������״̬������״ִ̬����صĲ���
    */	
    virtual void SetMJState( int nState, BOOL bTimelag = FALSE );

};

}
#endif // !defined(AFX_IMJSTATEEVENT_H__C459615F_7FFB_48E4_8742_44148C42F1A3__INCLUDED_)

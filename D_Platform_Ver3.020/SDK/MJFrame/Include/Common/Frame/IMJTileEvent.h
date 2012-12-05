/** @file	IMJTileEvent.h
 *	@brief	�ṩ���û���������ຯ���Ľӿڲ���
 *	@author edward
 *	@date 08.08.04
 *
 *  IMJTileEvent��Ҫ�ṩ��ȡ��ǰ��Ϸ�г�ǽ���ݵĺ���
 */
#if !defined(AFX_IMJTILEEVENT_H__145E99E0_1F35_496C_9AC1_8406E9074115__INCLUDED_)
#define AFX_IMJTILEEVENT_H__145E99E0_1F35_496C_9AC1_8406E9074115__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "typeDef.h"

namespace MyGame
{

/**
 * @ingroup MJFrame
 * @brief IMJTileEvent
 *
 * <p>�� 
 *		IMJTileEvent, ��ǽ����Ľӿ�
 * </p>
 * @sa
 */	
class IMJTileEvent  
{
public:
	IMJTileEvent();
	virtual ~IMJTileEvent();

	/**
	 *  @brief ��ȡ��ǰ�����
	 *  @return ���ص�ǰ�����
	 */	
	virtual int GetTurn();

	/**
	 *  @brief ���õ�ǰ�����
	 *  @param[in] nTurn ���õ����
	 */	
	virtual void SetTurn( int nTurn );

	/**
	 *  @brief ��ȡׯ��
	 *  @return ����ׯ��
	 */	
	virtual int GetDealer() { return 0; }

};

}
#endif // !defined(AFX_IMJTILEEVENT_H__145E99E0_1F35_496C_9AC1_8406E9074115__INCLUDED_)

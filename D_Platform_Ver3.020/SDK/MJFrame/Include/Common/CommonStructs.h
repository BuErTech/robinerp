
#ifndef __COMMON_STRUCTS_H__
#define __COMMON_STRUCTS_H__


#include "typeDef.h"

#include "playerManagerCommon.h"

namespace MyGame
{

	//���¶������˵�ԭ��ֵ, ����0 ~ 999Ϊ���ʹ�õĿռ�, 1000 ~ 32767 Ϊ��Ϸʹ�õĿռ�
	enum KICK_REASONS
	{
		kick_upgrade = 0,			        //����ϵͳ�������������ʱ�뿪
		kick_multilogin = 1,		        //�ظ���½֪ͨʹ�õ�ԭ��ֵ
		kick_dull		= 2,		        //������ҷ����������뿪
		kick_clear		= 3,	        	//�����ר�ã��������
		
		kick_clear_in_dogfight_room = 4,    //��ս����ר�ã��������

		kick_offline = 1000,	        	//������ҵ��ߣ���Ϸ������뿪
		kick_nomoney = 1001,		        //������ҲƸ�ֵ��������Ϸ������뿪
		kick_notallowholdon = 1004,	        //
		kick_bekicked_byuser = 1010,        //�����T
		kick_fail_lock_money = 1011,	    //������Ϸ��ʧ��
		kick_fail_lock_happybean = 1012,	//�������ֶ�ʧ��
		kick_more_than_Upper     = 1013,	//��������
		kick_less_than_ticket    = 1014,    //������Ʊ
		kick_less_than_lower     = 1015,	//��������
		kick_game_has_start      = 1016,	//��Ϸ�Ѿ���ʼ

		Kick_Vip_kick 			= 1017,     //vip ����
		Kick_gm_kick			= 1018,	    //GM ����
		kick_user_normal_exit	 = 1020		//������������˳����µ��º�������Ϸ��ʼ��һ��Ҫ��kick
    };

	struct TCMD_HEADER
	{
		UINT    cCmdID;         // ��Ϣ��ʶ
		UINT    cCmdPara;
		UINT    nDataLen;       // ������Ϣ�ĳ���
	};
	

}


#endif


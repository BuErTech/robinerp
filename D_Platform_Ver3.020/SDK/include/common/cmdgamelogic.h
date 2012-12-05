#ifndef __CMD_GAMELOGIC_H_
#define __CMD_GAMELOGIC_H_

#include "..\..\MJFrame\Include\Common\CommonStructs.h"
#include "..\..\MJFrame\Include\Common\Frame\mjframebasedata.h"


//������汾
#define DEV_LIB_VER				        1				///������汾


using namespace MyGame;

namespace MyGame
{

#pragma pack(1)

    // ������Ա�����GameMessage��ID��Ҫ��51�Ժ�
    #define SC_MJ_NOTIFY_DEALER       52
    #define CS_MJ_REQUEST_DEAL_FINISH 56
 
	/**
	* @ingroup MJFrame
	* @brief ֪ͨׯ��λ��
	*/	
	struct NCMJ_TCMD_NOTIFY_DEALER : public TCMD_HEADER
	{
		BYTE byDealer;	  // ׯ��λ��

		NCMJ_TCMD_NOTIFY_DEALER ()
		{
			memset(this, 0, sizeof(NCMJ_TCMD_NOTIFY_DEALER));
			cCmdPara = SC_MJ_NOTIFY_DEALER;
			cCmdID = MJFRAME_MOUDLE_MSG;
			nDataLen = sizeof(NCMJ_TCMD_NOTIFY_DEALER);
		}
	};

    /**
    * @ingroup MJFrame
    * @brief ���������
    */	
    struct NCMJ_TCMD_REQUEST_DEAL_FINISH : public TCMD_HEADER
    {
        NCMJ_TCMD_REQUEST_DEAL_FINISH ()
        {
            memset(this, 0, sizeof(NCMJ_TCMD_REQUEST_DEAL_FINISH));
            cCmdPara = CS_MJ_REQUEST_DEAL_FINISH;
            cCmdID = MJFRAME_MOUDLE_MSG;
            nDataLen = sizeof(NCMJ_TCMD_REQUEST_DEAL_FINISH);
        }
    };


#pragma pack()
	
}

#endif



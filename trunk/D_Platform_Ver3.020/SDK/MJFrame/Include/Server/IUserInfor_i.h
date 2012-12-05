#ifndef __IUSER_INFOR_I_H__
#define __IUSER_INFOR_I_H__

#include "typeDef.h"
/**
* @ingroup GameSDK
* @brief IUserInfor
*
* <p>�� 
*		IUserInfor �û���Ϣ�ӿ�
* </p>
* @sa
*/	

class IUserInfor
{
public:
   /**
    *  @brief               ȡ���û�ID
    *  @return              �����û�ID
    *  @sa 
    */
	virtual UINT GetUserID(void) = 0;
};

#endif
#ifndef __USER_INFOR_ITEM_H__
#define __USER_INFOR_ITEM_H__

#include "afxwin.h"
#include "..\UserInfo.h"
#include "IUserInfor_i.h"
#include "SoUserManager_i.h"
/*******************************************************************************************************/


/*******************************************************************************************************/
///�û�������
class  CGameUserInfoEx : public CGameUserInfo, public IUserInfor
{
    ///��������
    MyGame::ISoPlayer  *m_pSoPlayer;  /// ָ���û����Ⱪ�Ľӿ���                                  
};

/*********************************************************************************************************************/

#endif
#ifndef __IMAIN_MANAGE_I_H__
#define __IMAIN_MANAGE_I_H__
/**
* @ingroup GameSDK
* @brief IGameUserInfoManger
*
* <p>�� 
*		IGameUserInfoManger �����������߼����IMainManager���һЩ�����������޵Ĳ���
* </p>
* @sa
*/	

#include "ComStruct.h"
#include "gameobject_i.h"

class IMainManger : public MyGame::IGameObject
{
public:

   /**
	*  @brief				ȡ�� comStruct.h ���涨��� ManageInfoStruct �ṹ
	*  @return				���� ManageInfoStruct �ṹ
	*  @sa 
	*/
	virtual ManageInfoStruct* GetManageInforStr() = 0;

 //  /**
	//*  @brief				���ö�ʱ��
	//*  @timeId		        ���ö�ʱ��ID
	//*  @arg	                ��ʱ���Ĳ���
	//*  @intervalTime        ��ʱ�����м���ʱ��	
	//*  @return				
	//*  @sa 
	//*/
	//virtual VOID SetTimerEx(UINT tableIndex, UINT timeId, VOID* arg, UINT intervalTime)= 0;

 //  /**
	//*  @brief				ɾ����ʱ��
	//*  @timeId			    ��ʱ��ID
	//*  @return				
	//*  @sa 
	//*/
	//virtual VOID DelTimerEx(UINT tableIndex, UINT timeId)= 0;

 //  /**
	//*  @brief				���ĳ���û����ڴ���Ϣ
	//*  @pUserInfo			ָ���û���Ϣָ��
	//*  @return				�Ƿ�ɹ������û�в��ɹ��ģ�һ�㶼��ɹ�
	//*  @sa 
	//*/
	//virtual BOOL CleanUserInfoEx(IUserInfor *pUserInfo)= 0;
};

#endif
#ifndef __IUSER_MANAGE_I_H__
#define __IUSER_MANAGE_I_H__
#include "GameObject_i.h"
/**
* @ingroup GameSDK
* @brief IGameUserInfoManger
*
* <p>�� 
*		IGameUserInfoManger ���ļ����������SerLib�����Ӷ���ģ������û��������޵Ĳ���
*							���ҵ�ĳ���û�ѽ���ȵ�
* </p>
* @sa
*/	

class CGameUserInfo;

class IGameUserInfoManger : public MyGame::IGameObject
{
public:
   /**
	*  @brief				�ҵ������Ѷ��ߵ��û�
	*  @dwUserID			�����û�ID
	*  @return				���ص����û���Ϣ
	*  @sa 
	*/
	virtual CGameUserInfo * FindNetCutUser(int dwUserID) = 0;

   /**
	*  @brief				�ҵ������Ѷ��ߵ��û�
	*  @dwUserID			�����û���
	*  @return				���ص����û���Ϣ�ṹ
	*  @sa 
	*/
	virtual CGameUserInfo * FindNetCutUser(char * szName) = 0;

   /**
	*  @brief				�ҵ����������ߵ��û�
	*  @dwUserID			�����û���
	*  @return				���ص����û���Ϣ�ṹ
	*  @sa 
	*/
	virtual CGameUserInfo * FindOnLineUser(char * szName) = 0;

   /**
	*  @brief				�ҵ����������ߵ��û�
	*  @dwUserID			�����û�ID
	*  @return				���ص����û���Ϣ�ṹ
	*  @sa 
	*/
	virtual CGameUserInfo * FindOnLineUser(int dwUserID) = 0;

   /**
	*  @brief				�ҵ����������ߵ��û�
	*  @dwUserID			�����û�ID
	*  @return				���ص����û���Ϣ�ṹ
	*  @sa 
	*/
	virtual CGameUserInfo * GetOnLineUserInfo(UINT uIndex) = 0;

   /**
	*  @brief				�ͷ�����һ���û����Ӷ����û��������ڵ��û��ﶼ����
	*  @pGameUserInfo		�������ڷŵ����û�ָ��
	*  @return				���سɹ���ʧ��
	*  @sa 
	*/
	virtual BOOL ClearUser(CGameUserInfo * pGameUserInfo) = 0;
};

#endif
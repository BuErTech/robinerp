#ifndef _IMAIN_GAMEFACTORY_I_H
#define _IMAIN_GAMEFACTORY_I_H
#include "GameObject_i.h"
/**
* @ingroup GameSDK
* @brief IMainGameFactory
*
* <p>�� 
*		IMainGameFactory ��������Ĺ����࣬���ڿ�����������ڲ��ӿ�
* </p>
* @sa
*/	

#include "typeDef.h"
#include "GameObject_i.h"

class IMainGameFactory
{
public:
	
	virtual BOOL RegisterInterface(int nModuleID, MyGame::IGameObject* pModule) = 0;
	virtual BOOL UnRegisterInterface(int nModuleID) = 0;
    virtual MyGame::IGameObject* QueryInterface(int nModuleID) = 0;

};

#endif

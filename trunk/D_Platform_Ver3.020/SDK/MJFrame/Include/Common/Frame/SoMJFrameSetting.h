/* 
   ʵ�֣�
	���ڷ������������ļ���ֻ��Ҫʵ������3�����輴��
	���ȣ�m_pGameSetting->SetCfgPath("../config/dgz.cfg");  ���÷������������ļ�·��
	Ȼ����Ϸ����Ҫ������ ��XSETTING_DATA�ṹ���ж���
	����ڡ�void LoadCfg();��������sdk�ṩ�Ķ�ȡ���õĽӿ�����ȡ���ɣ��������ʲôʱ��load��reload

   ʹ�ã�
	��������SoCardsGameLogic�оͿ������� m_xGameSetting.GetSettingData(XSETTING_DATA)����ȡ����������

ע��
	1���Ƿ�ʹ��������  ��һ����˵���� [CardsFrame] �е� bTileConfig ��0���� 1���ǣ�
	2����������Щ����Ϸ��������Եģ���Ҫ��CSoCardsGameLogic::OnCollectBaseMsg���͵��ͻ���
 */
//////////////////////////////////////////////////////////////////////
#ifndef __SOMJGAME_SETTING_H____
#define __SOMJGAME_SETTING_H____

#include "gameobject_i.h"
#include "SoModuleimpl_t.h"
#include "gameeventimpl_t.h"

#include "SoGameSetting_i.h"
#include "SoGameProcessor_i.h"
#include "SoGameLogger_i.h"

#include "SoBaseLogic.h"
#include "XSoGameSetting_i.h"

#include "mjcfgdef.h"

using namespace MyGame;

namespace MyGame
{

typedef TModuleImpl<IXSoGameSetting> CMJSoGameSettingImp;

class CSoMJFrameSetting : 
			public CMJSoGameSettingImp,
			public TEventImpl<ISoGameSettingEvent>
{
public:
	CSoMJFrameSetting ();
	virtual ~CSoMJFrameSetting ();

	virtual void Initialize(CSoBaseLogic* pSoGameLogic, int nGameID);

	// ISoGameSettingEvent
	virtual int  OnLoadCfg();
	virtual int  OnReloadCfg();

	// IXSoGameSetting
	virtual void GetSettingData(XSETTING_DATA &rstSettingData) {rstSettingData = m_stSettingData;}

	virtual int  GetCoolTime(){return m_stSettingData.nCoolTime;}

	virtual void GetMJSettingData( MJ_SETTING_DATA &tagMjSettingData);

	virtual void LoadCfg();

protected:
	ISoGameLogger    *m_pSoGameLogger;
	ISoGameSetting   *m_pGameSetting;

	// ����������
	XSETTING_DATA     m_stSettingData;
	MJ_SETTING_DATA	  m_tagMJSettingData;
 	BOOL			  m_bTileConfig;

    // ��ϷID�뷿��ID
    int               m_nGameID;
    int               m_nRoomID;
};

};


#endif 




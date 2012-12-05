/*
    ����������漸�ַ�ʽȥʹ�ô˽ӿ�

    m_pGameSetting->GetItemValue("CardsFrame/bTileConfig", m_bTileConfig, 0);
	m_pGameSetting->GetItemValue("nGiveCardTime", m_stSettingData.nGiveCardTime, 15);
	m_pGameSetting->GetItemValue("test2", temp, sizeof(temp), "dddddddd");

    m_pGameSetting->GetItemValue("sjdc/", val, 0);
	m_pGameSetting->GetItemValue("nTrusteeTime", m_stSettingData.nTrusteeTime, 3);
	m_pGameSetting->GetItemValue("nBasePoint", m_stSettingData.nBasePoint, 1);

    m_pGameSetting->GetItemValue("Normal/", val, 0);

 */
#if !defined(SO_GAME_SETTING_I_H)
#define SO_GAME_SETTING_I_H

#include "Svrdef.h"
#include "gameevent_i.h"

namespace MyGame
{
	class ISoGameSetting :public IGameObject,public IEventProducer
	{
	public:
		//��ϷLogic����
		//ֱ�Ӵ��������ļ�������"zhaocha.cfg"���ڲ��Զ��������·��  (�ɲ����׺��Ĭ�Ϻ�׺Ϊ.cfg��ʽ)
		//���磺  "../config/zhaocha.cfg"
		virtual BOOL SetCfgPath(char *lpszCfgPath) = 0;

		// lpszNameΪNULLʱ�����ݷ��������������ȡ (��ʹ��all/LogFlag)
		// �����Ҫͨ��server���������(��free/RoomName��freeΪ����������)��ֱ��free)
		virtual BOOL GetItemValue(char *lpszName, int &nValue, int nDefaultValue) = 0;

        virtual BOOL GetItemValue(char *lpszName, char &nValue, int nDefaultValue) = 0;
		// lpszNameΪNULLʱ�����ݷ��������������ȡ
		virtual BOOL GetItemValue(char *lpszName, char *lpszReturnedString, int nSize, char* pszDefaultValue) = 0;
	};

	class ISoGameSettingEvent : public IEvent
	{
	public:
		virtual int OnLoadCfg() = 0;
		virtual int OnReloadCfg() = 0;
	};
}
#endif


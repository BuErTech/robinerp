#ifndef __SO_GAME_SETTING_H__
#define __SO_GAME_SETTING_H__

#include "afxwin.h"
#include "typeDef.h"
#include "SoGameSetting_i.h"
#include "SoFireEvent_t.h"

namespace MyGame
{
class CDeskLogicImpl;
class SoGameSettingImpl :public TSoEventProducerImpl<ISoGameSetting, 1> 
{
private:
        CDeskLogicImpl*     m_pDeskLogicImpl;
        char               m_SectName[32];
        BOOL               m_bSectBeable;
public:
        SoGameSettingImpl(CDeskLogicImpl* pDeskLogicImpl);
        ~SoGameSettingImpl(void);

        //��ϷLogic����
		//ֱ�Ӵ��������ļ�������"zhaocha.cfg"���ڲ��Զ��������·��  (�ɲ����׺��Ĭ�Ϻ�׺Ϊ.cfg��ʽ)
		//���磺  "../config/zhaocha.cfg"
		virtual BOOL SetCfgPath(char *lpszCfgPath);

		// lpszSectionNameΪNULLʱ�����ݷ��������������ȡ (��ʹ��all/LogFlag��
		//�����Ҫͨ��server���������(��free/RoomName��freeΪ����������)��ֱ��free)
		virtual BOOL GetItemValue(char *lpszName, int &nValue, int nDefaultValue);
        virtual BOOL GetItemValue(char *lpszName, char &nValue, int nDefaultValue);
		// lpszSectionNameΪNULLʱ�����ݷ��������������ȡ
		virtual BOOL GetItemValue(char *lpszName,  char *lpszReturnedString, int nSize, char* pszDefaultValue);

private:
        BOOL PaserSectName(char* lpszName, char* pSectName, char* pValueName);
};

};

#endif

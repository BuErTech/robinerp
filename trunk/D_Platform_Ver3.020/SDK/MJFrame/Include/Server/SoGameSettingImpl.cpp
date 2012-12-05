#include "stdafx.h"
#include "SoGameSettingImpl.h"
#include "SoIddeFine.h"
#include "SoDeskLogicImpl.h"
#include "ComConfig.h"

using namespace MyGame;

#pragma message("here!******************************************************************************")


//����Ĵ��뻹û��ʵ�֣��ں�����ʵ�ְ�
SoGameSettingImpl::SoGameSettingImpl(CDeskLogicImpl* pDeskLogicImpl)
{
    m_pDeskLogicImpl = pDeskLogicImpl;
    m_pDeskLogicImpl->m_pGameFactory->RegisterInterface(MODULE_GAMESETTING, this);

    memset(m_SectName, 0, sizeof(m_SectName));
    m_bSectBeable = FALSE;
}

SoGameSettingImpl::~SoGameSettingImpl(void)
{
    
}

BOOL SoGameSettingImpl::SetCfgPath(char *lpszCfgPath)
{
    BOOL bRet = FALSE;
    char  cuPath[256];
    ZeroMemory(cuPath, sizeof(cuPath));
    DWORD dwLength = GetModuleFileNameA(GetModuleHandle(NULL), cuPath, sizeof(cuPath));
    char *p = _tcsrchr(cuPath, TEXT('\\'));
    *p = '\0';
    strcat(cuPath,"/");
    strcat(cuPath, lpszCfgPath);

    ComConfig* pConfig = ComConfig::GetInstance();

    /*�����Ƿ���ͬһ�������ļ�����Ȼ�Ļ���Ҫ���¼����ļ���һ�����ڿ���ʱ��*/
    bRet = pConfig->PaserConfigPath(cuPath);
   
    /*�������һ�¿��������ļ��ص�*/
    if (bRet == TRUE)
    {
        FireEvent(&ISoGameSettingEvent ::OnLoadCfg);
    }

    return bRet;
}

BOOL SoGameSettingImpl::GetItemValue(char *lpszName, char &nValue, int nDefaultValue)
{
    int  nValue32= 0;

    BOOL ret =  GetItemValue(lpszName, nValue32, nDefaultValue);

    nValue = (char)nValue32;
    return ret;
}

BOOL SoGameSettingImpl::GetItemValue(char *lpszName, int &nValue, int nDefaultValue)
{
    char szSectName[32];
    char szValueName[32];

    memset(szSectName, 0,  sizeof(szSectName));
    memset(szValueName, 0,  sizeof(szValueName));
    
    //����������û��"/"
    if (PaserSectName(lpszName, szSectName, szValueName) == FALSE)
    {
        return FALSE;
    }

    if (m_bSectBeable)
    {
        //����ڲ����ж����ֵ��趨������ֱ��������
        return ComConfig::GetInstance()->GetItemValue(m_SectName, szValueName, nValue, nDefaultValue);
    }
    else
    {   
        //�ڲ�û�ж����֣�����ֱ����normal�ֶε�����,�ڲ�������������
        return ComConfig::GetInstance()->GetItemValue(szSectName, szValueName, nValue, nDefaultValue);
    }

    return FALSE;
}

BOOL SoGameSettingImpl::GetItemValue(char *lpszName,  char *lpszReturnedString, int nSize, char* pszDefaultValue)
{
    char szSectName[32];
    char szValueName[32];

    memset(szSectName, 0,  sizeof(szSectName));
    memset(szValueName, 0,  sizeof(szValueName));

    //����������û��"/"
    if (PaserSectName(lpszName, szSectName, szValueName) == FALSE)
    {
        return FALSE;
    }

    if (m_bSectBeable)
    {
        //����ڲ����ж����ֵ��趨������ֱ��������
        return ComConfig::GetInstance()->GetItemValue(m_SectName, szValueName, lpszReturnedString, nSize, pszDefaultValue);
    }
    else
    {   
        //�ڲ�û�ж����֣�����ֱ����normal�ֶε�����
        return ComConfig::GetInstance()->GetItemValue(szSectName, szValueName, lpszReturnedString, nSize, pszDefaultValue);
    }

    return FALSE;
}


BOOL SoGameSettingImpl::PaserSectName(char* lpszName, char* pSectName, char* pValueName)
{
    char* pSectNameEnd = strstr(lpszName, "/");

    if (pSectNameEnd)
    {
        if ((pSectNameEnd - lpszName) >= (32 - 1)) 
        {
            return FALSE;
        }

        memcpy(pSectName, lpszName, pSectNameEnd - lpszName);

        if ((*(pSectNameEnd+1)) != 0)
        {
            //���������ֱ�Ӵ����������ֱ��ʹ�����������
            strcpy(pValueName, pSectNameEnd+1);
            return TRUE;
        }   
        else
        {
            //"newSet/" �������������ֱ�ӰѶ����ּ�ס������ֻ�������־ͺõģ�
            memcpy(m_SectName, pSectName, sizeof(m_SectName));
            m_bSectBeable = TRUE;
            return TRUE;
        }
    }
    else
    {
        strcpy(pSectName, "Normal");
        strcpy(pValueName, lpszName);
        return TRUE;
    }

    return FALSE;
}



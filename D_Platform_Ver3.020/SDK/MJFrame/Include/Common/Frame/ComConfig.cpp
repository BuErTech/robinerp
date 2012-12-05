#include "stdafx.h"
#include "ComConfig.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

/*****************************************   SectItem   *********************************/
SectItem::SectItem()
{
    m_bBeAble = FALSE;
    m_pSectStart = NULL;
    m_pSectEnd = NULL;
}

SectItem::~SectItem()
{
    m_bBeAble = FALSE;
    m_pSectStart = NULL;
    m_pSectEnd = NULL;
}


/*******************************************   ComConfig   ***********************************/

ComConfig* ComConfig::m_pStance = NULL;
ComConfig::ComConfig(void)
{
    memset(m_cuPathName, 0, sizeof(m_cuPathName));      
    m_pMemFile = NULL;          
    m_cuMemSizeLen = 0;   
    m_pStance = NULL;
    m_bRePaser = TRUE;     //��ʼ��ΪҪ���½��������ǽ������Ҫ���½������ܽ�����
}

ComConfig::~ComConfig(void)
{
    
}

ComConfig* ComConfig::GetInstance()
{
    if (m_pStance == NULL)
    {
        m_pStance = new ComConfig();
    }

    return m_pStance;
}

static int configStrCmp(char *str1, char *str2)
{
	if (!str1 || !str2)
	{
		return 1;
	}

	while(*str1 == *str2)
	{
		if (!(*str1) || !(*str2))
		{
			break;
		}
		str1 ++;
		str2 ++;
	}

	if (!(*str1) || !(*str2))
	{
		return 0;
	}

	return 1;
}

static STATUS ReadStringFromTag(char* pMemFile, UINT memFileLen, char *pTag, char **ppRetStart, char **ppRetEnd)
{
	char *pCu = NULL, *pOldCu = NULL;
	char *pLimit = pMemFile + memFileLen;

    *ppRetStart = 0;
    *ppRetEnd = 0;

    // ���������ֶΰ�����ǰ���ҵ��ֶ�, ������Ҫѭ��������һ��
    /*while (pOldCu = strstr(pMemFile, pTag))
    {
        pMemFile = pOldCu + strlen(pTag) + 5;
        pCu = pOldCu;
    }*/

	if (pOldCu = strstr(pMemFile, pTag))
	{
		pMemFile = pOldCu + strlen(pTag) + 5;
		pCu = pOldCu;
	}

	if (!pCu)
	{
		return RE_FAIL;
	}

    while(pCu < pLimit)
    {
        if (configStrCmp(pCu, "\r\n") == 0)
        {
            *ppRetEnd = pCu;
            break;
        }

        if (configStrCmp(pCu, "\n") == 0)
        {
            *ppRetEnd = pCu;
            break;
        }

        if (pCu == (pLimit-1))
        {
            *ppRetEnd = pCu;
            break;
        }

        if (*pCu == '=')
        {
            *ppRetStart = pCu+1;
        }
    
        pCu++;
    }

    if ((*ppRetStart == 0) || (*ppRetEnd == 0))
    {
        return RE_FAIL;
    }

    if (((*ppRetEnd) - (*ppRetStart)) <= 0)
    {
        return RE_FAIL;
    }
    
	return RE_SUCCEED;
}

static STATUS ReadIntFromTag(char* pMemFile, UINT memFileLen, char *pTag, UINT *pRet)
{
	char *pStart = NULL;
	char *pEnd = NULL;
	UINT cuMemLen = 0;
	char tempBuf[512];
	int readIntRet = 0;

    STATUS sta = ReadStringFromTag(pMemFile, memFileLen, pTag, &pStart, &pEnd);
    if (sta == RE_FAIL)
    {
        return RE_FAIL;
    }

    cuMemLen = pEnd - pStart;

    if (cuMemLen > (sizeof(tempBuf) - 1))
    {
       return RE_FAIL; 
    }

    memcpy(tempBuf, pStart, cuMemLen);

    tempBuf[cuMemLen] = 0;
	
	readIntRet = atoi(tempBuf);

	*pRet = readIntRet;

	return RE_SUCCEED;
}

BOOL ComConfig::PaserConfigPath(char* pConfigPath)
{
    UINT iFileLen = 0;
    FILE *pFile = NULL;
    UINT cuIndex = 0;

    //ֻ���ڳ�ʼ��ʱ���з���һ�Σ�Ȼ���������³�ʼʱ���Ѹı�һ���ڲ���״̬�����ܽ���
    if (m_bRePaser != TRUE)
    {
        return TRUE;
    }
    
    iFileLen = 0;
    pFile = fopen( pConfigPath, "r" );
    if ( pFile == NULL )
    {
           return FALSE;
    }

    fseek( pFile, 0, SEEK_END );
    iFileLen = ftell( pFile );
    if (iFileLen >= (1024 * 1024))
    {
        return FALSE;
    }

    rewind( pFile );
    m_pMemFile = new char[iFileLen+1];
    if (!m_pMemFile)
    {
        goto fail;
    }

    fread( m_pMemFile, 1, iFileLen, pFile );
    m_pMemFile[iFileLen] = 0;

    if (pFile)
    {
        fclose( pFile ); 
    }
    strncpy(m_cuPathName, pConfigPath, sizeof(m_cuPathName));
    m_cuMemSizeLen = iFileLen;

    m_bRePaser = FALSE;

    m_cuSectNum = 0;
    memset(m_SectArry, 0, sizeof(m_SectArry));

    PaserSect();

    printf("PaserSect %s succeed\n", pConfigPath);
    return TRUE;

fail:
    if (pFile)
    {
        fclose( pFile ); 
    }
    return FALSE;
}

char* ComConfig::GetNextSect(char* pMemStr, char* pLimit)
{
    char* pStart = NULL;
    char* pEnd = NULL;
    
    //��Ϊ���һ����Ҳ�Ѹ�0�ˣ����Կ��������������
    char* pSectStrStart = strstr(pMemStr, "[");
    if (pSectStrStart == NULL)
    {
        return NULL;
    }
    pSectStrStart += 1;
    char* pSectStrEndt = strstr(pSectStrStart + 1, "]");
    if (pSectStrEndt == NULL)
    {
        return NULL;
    } 

    if ((pSectStrEndt - pSectStrStart) > (sizeof(m_SectArry[0].m_SectName) - 1))
    {
        return NULL;
    }

    if (m_cuSectNum >= (MAX_CU_SECT_NUM-1))
    {
        return NULL;        
    }
    //����һ�����к�
    pStart = strstr(pSectStrEndt+1, "\n");
    if (pStart == NULL)
    {
        return NULL;
    }
    pStart = pStart+1;

    //������һ���Σ�����Ҳ����ˣ����ǵ��������
    pEnd = strstr(pStart+1, "[");
    if (pEnd == NULL)
    {
        pEnd = pLimit;
    }

    memset(m_SectArry[m_cuSectNum].m_SectName, 0, sizeof(m_SectArry[m_cuSectNum].m_SectName));
    memcpy(m_SectArry[m_cuSectNum].m_SectName, pSectStrStart, (pSectStrEndt - pSectStrStart));
    m_SectArry[m_cuSectNum].m_bBeAble = TRUE;
    m_SectArry[m_cuSectNum].m_pSectStart = pStart;
    m_SectArry[m_cuSectNum].m_pSectEnd = pEnd;

    m_cuSectNum++;

    if (pEnd == pLimit)
    {
        return NULL;
    }

    //�����������ַ�
    return pEnd;
}

BOOL ComConfig::PaserSect()
{   
    char* pReturn = NULL;
    m_cuSectNum = 0;
    char* pCu = m_pMemFile;
    char* pLimit = m_pMemFile + m_cuMemSizeLen;

    for(UINT cuIndex=0; cuIndex<MAX_CU_SECT_NUM;cuIndex++)
    {
        pReturn = GetNextSect(pCu, 
                            pLimit); 
        if (pReturn == NULL)
        {
            return FALSE;
        }

        pCu = pReturn;
    }
    return TRUE;
}

//ȫ�ֵ���һ��
BOOL ComConfig::RePaserConfig()
{
    m_bRePaser = TRUE;
    return PaserConfigPath(m_cuPathName);
}

STATUS ComConfig::FindSectName(char* lpszSectName, char** ppStart, UINT *pbufLen)
{
    for(UINT index=0; index<MAX_CU_SECT_NUM; index++)
    {
        if (m_SectArry[index].m_bBeAble == FALSE)
        {
            return RE_FAIL;
        }


        if (strncmp(m_SectArry[index].m_SectName, 
                    lpszSectName, 
                    sizeof(m_SectArry[index].m_SectName)) == 0)
        {   
            *ppStart = m_SectArry[index].m_pSectStart;
            *pbufLen = m_SectArry[index].m_pSectEnd - m_SectArry[index].m_pSectStart;

            return RE_SUCCEED;
        }
    }
    return RE_FAIL;
}

BOOL ComConfig::GetItemValue(char* lpszSectName, char *lpszName, int &nValue, int nDefaultValue)
{
    STATUS sta = RE_FAIL;
    UINT  retValue = 0; 

    char* pMemStart = NULL;
    UINT bufLen = 0;

    if (FindSectName(lpszSectName, &pMemStart, &bufLen) == RE_FAIL)
    {
		nValue = nDefaultValue;
        return FALSE;
    }

    sta = ReadIntFromTag(pMemStart, bufLen, lpszName, &retValue);
    if (sta != RE_SUCCEED)
    {
        nValue = nDefaultValue;
        return FALSE;      
    }
    
    nValue = retValue;

    return TRUE;
}   

BOOL ComConfig::GetItemValue(char* lpszSectName, char *lpszName,  char *lpszReturnedString, int nSize, char* pszDefaultValue)
{
    STATUS sta = RE_FAIL;
    char*  pStart = NULL;
    char*  pEnd = NULL;
   
    char* pMemStart = NULL;
    UINT bufLen = 0;

    if (FindSectName(lpszSectName, &pMemStart, &bufLen) == RE_FAIL)
    {
        return FALSE;
    }

    sta = ReadStringFromTag(pMemStart, bufLen, lpszName, &pStart, &pEnd);
    if (sta != RE_SUCCEED)
    {
        strncpy(lpszReturnedString, pszDefaultValue, nSize);
        return FALSE;      
    }

    if ((pEnd - pStart) >= (nSize -1))
    {
        strncpy(lpszReturnedString, pszDefaultValue, nSize);
        return FALSE;
    }

    memcpy(lpszReturnedString, pStart, (pEnd - pStart));
    
    lpszReturnedString[pEnd - pStart] = 0;              //�����������
    return TRUE;
}







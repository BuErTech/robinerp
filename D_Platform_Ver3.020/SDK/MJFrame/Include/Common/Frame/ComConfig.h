#ifndef __COM_CONFIG_H__
#define __COM_CONFIG_H__

#include "afxwin.h"
#include "typeDef.h"

#define MAX_CU_SECT_NUM     100

class SectItem
{
public:
    BOOL   m_bBeAble;
    char   m_SectName[32];
    char*  m_pSectStart;
    char*  m_pSectEnd;
public:
    SectItem();
    ~SectItem();
};

/*���ڻ�û�зֶεĹ��ܣ��ں����ټ���*/

/*�˶����ǹ��õ�ȫ��Ψһ������*/
class ComConfig
{
public:
    char   m_cuPathName[256];          //·����
    char   *m_pMemFile;                //�ڴ��ļ�
    UINT m_cuMemSizeLen;             //�����ڴ�ĳ���
    BOOL   m_bRePaser;                 //�Ƿ�Ҫ���½���һ��
    UINT m_cuSectNum;

    static  ComConfig*  m_pStance;
    
    SectItem    m_SectArry[MAX_CU_SECT_NUM];       //������

public:
    ComConfig(void);
    ~ComConfig(void);

    static ComConfig* GetInstance(); 

    /*�����β��ǵ�һ�εĻ����ͻ����»����µ��ļ����ݵ�MEM*/
    BOOL PaserConfigPath(char* pConfigPath);

    //�ٴ����µĽ��������ļ����ڲ����ļ���������ǰ���ļ���
    BOOL RePaserConfig();

    BOOL GetItemValue(char* lpszSectName, char *lpszName, int &nValue, int nDefaultValue);

	BOOL GetItemValue(char* lpszSectName, char *lpszName,  char *lpszReturnedString, int nSize, char* pszDefaultValue);

private:    
    //ȡ����һ��Sectbuf ָ��,ֻ��������ʱʹ��һ��
    char* GetNextSect(char* pMemStr, char* pLimit); 

    STATUS FindSectName(char* lpszSectName, char** ppStart, UINT *pbufLen);

    /*�����ε�����*/
    BOOL PaserSect();
};

#endif

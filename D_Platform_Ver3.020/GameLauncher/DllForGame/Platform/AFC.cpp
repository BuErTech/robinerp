#include "../stdafx.h"
//#include <AfxDllX.h>
#include "AFCResource.h"

//ͨ��·����ȡ��ϷID
UINT GetGameNameIDByPath();

int GetAvatarIndexByMoney(__int64 i64Money,UINT uGameNameID);
int GetAvatarIndexByPoint(__int64 dwPoint,UINT uGameNameID);

//�Ƿ�VIP����Чʱ��
bool IsVipEffectTime(long lUserVipTime);

CBcfFile g_cfgModual(CBcfFile::GetAppPath() + "../Module.bcf");	//ģ�������ļ�
CBcfFile g_cfgOrder(CBcfFile::GetAppPath() + "../Order.bcf");		//Order�����ļ�

UINT g_GameNameID = GetGameNameIDByPath();

//���ֿռ�
//using namespace AFC;
//using namespace AFC::AFCResource;

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

//static AFX_EXTENSION_MODULE AFCDLL = { NULL, NULL };

//DLL ��ں���
//extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
//{
//	UNREFERENCED_PARAMETER(lpReserved);
//
//	if (dwReason == DLL_PROCESS_ATTACH)
//	{
//		if (!AfxInitExtensionModule(AFCDLL, hInstance)) return 0;
//		new CDynLinkLibrary(AFCDLL);
//	}
//	else if (dwReason == DLL_PROCESS_DETACH)
//	{
//		AfxTermExtensionModule(AFCDLL);
//	}
//	return 1;
//}

//ͨ��·����ȡ��ϷID
UINT GetGameNameIDByPath()
{
	CString str = CBcfFile::GetAppPath().Right(9).Left(8);

	return atol(str.GetBuffer());
}

//Ĭ�Ͻ�ҵȼ�����
TCHAR * GetMoneyOrderName(__int64 dwPoint)
{

	CString strSection;
	CString strKey;
	strSection.Format("%d",g_GameNameID);

	static TCHAR orderName[18] = "";

	//��Ϸû�ж��弶�𣬾�ʹ�ù�������
	if (g_cfgOrder.GetKeyVal(strSection,"Coint_Lv01",-1) == -1)
	{
		strSection = "Public";
	}

	int idx = 1;
	CString strMoney = "";

	for (int i = 1; i <= 50; ++i)
	{
		strKey.Format("Coint_Lv%02d",i);

		strMoney = g_cfgOrder.GetKeyVal(strSection,strKey,"-1");

		if (strMoney == "-1")
		{
			idx =  i - 1;
			break;
		}
		else
		{
			if (dwPoint < _atoi64(strMoney.GetBuffer()))
			{
				idx = i - 1;
				break;
			}
		}
	}

	strKey.Format("Coint_Lv%02d",idx);

	::strcpy(orderName,g_cfgOrder.GetKeyMemo(strSection,strKey,""));

	return orderName;

}

//Ĭ�Ͻ�ҵȼ�����
TCHAR * GetPointOrderName(__int64 dwPoint)
{

	CString strSection;
	CString strKey;
	strSection.Format("%d",g_GameNameID);

	static TCHAR orderName[18] = "";

	//��Ϸû�ж��弶�𣬾�ʹ�ù�������
	if (g_cfgOrder.GetKeyVal(strSection,"Point_Lv01",-1) == -1)
	{
		strSection = "Public";
	}

	int idx = 1;
	CString strMoney = "";

	for (int i = 1; i <= 50; ++i)
	{
		strKey.Format("Point_Lv%02d",i);

		strMoney = g_cfgOrder.GetKeyVal(strSection,strKey,"-1");

		if (strMoney == "-1")
		{
			idx =  i - 1;
			break;
		}
		else
		{
			if (dwPoint < _atoi64(strMoney.GetBuffer()))
			{
				idx = i - 1;
				break;
			}
		}
	}

	strKey.Format("Point_Lv%02d",idx);

	::strcpy(orderName,g_cfgOrder.GetKeyMemo(strSection,strKey,""));

	return orderName;

}

///< ��ȡ��Ҽ���
///< @param dwPoint[in] ����һ����ֵ�������Ǻܴ����
///< @return int ����

int GetMoneyOrderLevel(__int64 dwPoint)
{
	CString s = CBcfFile::GetAppPath();
	CBcfFile f("GameOption.bcf");
	CString m_key;
	m_key.Format("PlatMoneyOrderName");
	static int szOrderNum[50];
	CString strKey,strTmp;
	int tmpNum = 0;
	int nCount = 0;
	for (int i = 1;i <=50;i++)
	{
		strKey.Format("platOrder_%d",i);
		tmpNum=f.GetKeyVal(m_key,strKey,0);
		szOrderNum[i-1] = tmpNum;
		strTmp = f.GetKeyMemo(m_key,strKey,"");
		if (strTmp == "")
		{
			nCount = i-1;
			break;
		}
	}
	for (int i=0;i<nCount;i++)	
		if (dwPoint<szOrderNum[i]) 
			return i;
	return nCount;
}

//Ĭ�Ͻ�ҵȼ�����
int GetMoneyGrade(__int64 dwPoint)
{
	static int uBasePoint[6]=
	{
		10000,		100000,		1000000,	10000000,
		50000000,	100000000
	};
	for (int i=0;i<6;i++)	
		if (dwPoint<uBasePoint[i]) 
			return i;
	return 5;
}

//Ĭ�ϱ����ȼ�����
TCHAR * GetMatchOrderName(__int64 dwPoint)
{
	static int uBasePoint[19]=
	{
		-500L,	-200L,	-100L,	50L,
		150,	300,	500,	700,
		900,	1200,	1500,	1800,
		2100,	2400,	2700,	3000,
		3500,	4000,	5000,
	};

	static TCHAR * szOrderName[19]=
	{
		TEXT("�ż�����"),TEXT("�˼�����"),TEXT("�߼�����"),TEXT("��������"),
		TEXT("�弶����"),TEXT("�ļ�����"),TEXT("��������"),TEXT("��������"),
		TEXT("һ������"),TEXT("�ż�����"),TEXT("�˼�����"),TEXT("�߼�����"),
		TEXT("��������"),TEXT("�弶����"),TEXT("�ļ�����"),TEXT("��������"),
		TEXT("��������"),TEXT("һ������"),TEXT("����")
	};

	for (int i=0;i<19;i++)	
		if (dwPoint<uBasePoint[i]) 
			return szOrderName[i];

	return TEXT("����");
}

//Ĭ����ͨ�ȼ�����
TCHAR * GetNormalOrderName(__int64 dwPoint)
{
	static int uBasePoint[19]=
	{
		400,		800,		1600,		3200,
		6400,		12800,		25600,		51200,
		102400,		204800,		409600,		819200,
		1638400,	3276800,	6553600,	13107200,
		26214400,	52428800,	104857600
	};
	static TCHAR * szOrderName[19]=
	{
		TEXT("���"),TEXT("�б�"),TEXT("�µȱ�"),	TEXT("�ϵȱ�"),
		TEXT("��ʿ"),TEXT("��ʿ"),TEXT("��ʿ"),		TEXT("��ξ"),
		TEXT("��ξ"),TEXT("��ξ"),TEXT("��ξ"),		TEXT("��У"),
		TEXT("��У"),TEXT("��У"),TEXT("��У"),		TEXT("�ٽ�"),
		TEXT("�н�"),TEXT("�Ͻ�"),TEXT("��")
	};
	for (int i=0;i<19;i++)	
	{
		if (dwPoint<uBasePoint[i]) 
			return szOrderName[i];
	}
	return TEXT("Ԫ˧");
}

//Ĭ����ͨ�ȼ�������
int GetNormalOrderBasePoint(__int64 dwPoint)
{
	return 0;
	static int uBasePoint[19]=
	{
		400,		800,		1600,		3200,
		6400,		12800,		25600,		51200,
		102400,		204800,		409600,		819200,
		1638400,	3276800,	6553600,	13107200,
		26214400,	52428800,	104857600
	};
	for (int i = 0; i < 19; i++)	
	{		
		if (dwPoint < uBasePoint[i]) 
		{
			if(i == 0) 
				return 0;

			return uBasePoint[i - 1];
		}
	}
		
	return 104857600;
}
//Ĭ����ͨ�ȼ�����
// ����0 ~ 20
int GetNormalOrderLevel(__int64 dwPoint)
{
	static int uBasePoint[19]=
	{
		400,			800,		1600,		3200,
		6400,			12800,		25600,		51200,
		102400,			204800,		409600,		819200,
		1638400,		3276800,	6553600,	13107200,
		26214400,		52428800,	104857600
	};
	for (int i=0;i<19;i++)	
	{
		if (dwPoint<uBasePoint[i]) 
			return i+1;
	}
	return 20;
}


//��������
TCHAR * GetCharmGrade(__int64 dwFascination)
{
	static int uBasePoint[19]=
	{
		400,		800,		1600,		3200,
		6400,		12800,		25600,		51200,
		102400,		204800,		409600,		819200,
		1638400,	3276800,	6553600,	13107200,
		26214400,	52428800,	104857600
	};
	static TCHAR * szOrderName[19]=
	{
		TEXT("����"),TEXT("����"),TEXT("����"),		TEXT("����"),
		TEXT("����"),TEXT("����"),TEXT("����"),		TEXT("����"),
		TEXT("����"),TEXT("����"),TEXT("����"),		TEXT("��У"),
		TEXT("����"),TEXT("����"),TEXT("����"),		TEXT("����"),
		TEXT("����"),TEXT("����"),TEXT("����")
	};
	for (int i=0;i<19;i++)	
	{
		if (dwFascination<uBasePoint[i]) 
			return szOrderName[i];
	}
	return TEXT("����");
}


///��ȡ�û�������ͼƬ����(��Ϸ����ʹ��)
/// @param[in] uLogoID     �û���LogoID;
/// @param[in] i64Bank     �û����;
/// @param[in] i64Money    �û����Ͻ��;
/// @param[in] dwPoint     �û�����;
/// @param[in] uGameNameID ��ϷID;
/// @param[in] uComType    �������ͣ�1-Ϊ���ֳ���3-Ϊ��ҳ�
/// @return ��������ͼƬ����
UINT GetAvatarIndex(UINT uLogoID,__int64 i64Bank,__int64 i64Money,long dwPoint,UINT uGameNameID,UINT uComType)
{
	CString str;
	str.Format("DClient:GetAvatarIndexInRoom(uLogoID = %d,i64Bank = %I64d,i64Money = %I64d,dwPoint = %d,uGameNameID = %d,uComType = %d)",\
		uLogoID,i64Bank,i64Money,dwPoint,uGameNameID,uComType);
	OutputDebugString(str);

// 	CString str;
// 	str.Format("DClient:GetAvatarIndexInMain(uLogoID = %d,i64Bank = %I64d,i64Money = %I64d)",uLogoID,i64Bank,i64Money);
// 	OutputDebugString(str);

	/*�û����������ʽ 
		0-�û�ʹ�ý�ҹ���ʽ 
		1-�û�ӵ�н�Ҿ��� 
		2-�û�ӵ�н�һ���־���*/
	int nChangeMethod = g_cfgModual.GetKeyVal("Avatar","ChangeMethod",0);

	if (nChangeMethod == 0) //�û�ʹ�ý�ҹ���ʽ 
	{
		return uLogoID;
	}
	else if (nChangeMethod == 1) //�û�ӵ�н�Ҿ��� 
	{
		return GetAvatarIndexByMoney(i64Money,uGameNameID);
	}
	else if (nChangeMethod == 2) //�û�ӵ�н�һ���־���
	{
		if (uComType == 1)
		{
			return GetAvatarIndexByPoint(dwPoint,uGameNameID);
		}
		else if (uComType == 3)
		{
			return GetAvatarIndexByMoney(i64Money,uGameNameID);
		}
	}

	return 0;
}

///������Ϸ�Ľ�һ�ȡ����
/// @param[in] i64Money    �û����Ͻ��;
/// @param[in] uGameNameID ��ϷID;
/// @return ��������ͼƬ����
int GetAvatarIndexByMoney(__int64 i64Money,UINT uGameNameID)
{
	CString strSection;
	CString strKey;
	strSection.Format("%d",uGameNameID);

	//��Ϸû�ж��弶�𣬾�ʹ�ù�������
	if (g_cfgOrder.GetKeyVal(strSection,"Coint_Lv01",-1) == -1)
	{
		strSection = "Public";
	}

	CString strMoney = "";

	for (int i = 1; i <= 50; ++i)
	{
		strKey.Format("Coint_Lv%02d",i);

		strMoney = g_cfgOrder.GetKeyVal(strSection,strKey,"-1");

		if (strMoney == "-1")
		{
			return i - 2;
		}
		else
		{
			if (i64Money < _atoi64(strMoney.GetBuffer()))
			{
				return i - 2;
			}
		}
	}

	return 0;
}

///������Ϸ�Ļ��ֻ�ȡ����
/// @param[in] dwPoint     �û�����;
/// @param[in] uGameNameID ��ϷID;
/// @return ��������ͼƬ����
int GetAvatarIndexByPoint(__int64 dwPoint,UINT uGameNameID)
{
	CString strSection;
	CString strKey;
	strSection.Format("%d",uGameNameID);

	//��Ϸû�ж��弶�𣬾�ʹ�ù�������
	if (g_cfgOrder.GetKeyVal(strSection,"Point_Lv01",-1) == -1)
	{
		strSection = "Public";
	}

	CString strMoney = "";

	for (int i = 1; i <= 50; ++i)
	{
		strKey.Format("Point_Lv%02d",i);

		strMoney = g_cfgOrder.GetKeyVal(strSection,strKey,"-1");

		if (strMoney == "-1")
		{
			return i - 2;
		}
		else
		{
			if (dwPoint < _atoi64(strMoney.GetBuffer()))
			{
				return i - 2;
			}
		}

	}

	return 0;
}


//�Ƿ�VIP����Чʱ��
bool IsVipEffectTime(long lUserVipTime)
{
	CString stime;
	stime.Format("%d",CTime::GetCurrentTime());
	return (lUserVipTime > atol(stime));
}
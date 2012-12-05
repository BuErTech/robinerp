#include "Stdafx.h"
#include <AfxDllX.h>
#include "AFCResource.h"

//���ֿռ�
//using namespace AFC;
//using namespace AFC::AFCResource;

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

static AFX_EXTENSION_MODULE AFCDLL = { NULL, NULL };
// ��Ϊ��̬��Ա������ʹ�����ڽ��̽���ʱ��ʽ����CBcfFile::ClearMap()�������ͷţ�����MFC�ᱨ�ڴ�й©
EXT_CLASS std::map<CString, CBcfFileNameItem*> CBcfFile::mapFile;
EXT_CLASS std::map<CString,FileItem*> CUnZipRes::mapFile;
unsigned char CUnZipRes::m_bPassWord[MAX_PASSWORD_LENGTH];
int g_nGameID = -1;

int GetAvatarIndexByMoney(__int64 i64Money,UINT uGameNameID);
int GetAvatarIndexByPoint(__int64 dwPoint,UINT uGameNameID);

CBcfFile g_cfgModual(CBcfFile::GetAppPath() + "Module.bcf");	//ģ�������ļ�
CBcfFile g_cfgOrder(CBcfFile::GetAppPath() + "Order.bcf");		//Order�����ļ�


//DLL ��ں���
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(AFCDLL, hInstance)) return 0;
		new CDynLinkLibrary(AFCDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(AFCDLL);
	}
	return 1;
}

///Ĭ�ϵȼ�����
/// @param dwPoint[in] ����һ����ֵ�������Ǻܴ����
/// @return �ַ���ָ�룬���û�ж�ȡȡBCF�ļ����򷵻ؿ��ַ���

extern "C" __declspec(dllexport) void SetGameID(int iGameID)
{
	g_nGameID = iGameID;
}

extern "C" __declspec(dllexport) TCHAR * GetMoneyOrderName(__int64 dwPoint,UINT uGameNameID)
{
	CString strSection;
	CString strKey;
	strSection.Format("%d",uGameNameID);

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

extern "C" __declspec(dllexport) TCHAR * GetPointOrderName(__int64 dwPoint,UINT uGameNameID)
{

	CString strSection;
	CString strKey;
	strSection.Format("%d",uGameNameID);

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

//Ĭ�����ӵȼ�����
extern "C" __declspec(dllexport) int GetMoneyGrade(__int64 dwPoint)
{
	CString s = CBcfFile::GetAppPath();
	CBcfFile f("Order.bcf");
	CString m_key;
	m_key.Format("PlatMoneyOrderName%d", g_nGameID);
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
	//static int uBasePoint[6]=
	//{
	//	10000,		100000,		1000000,	10000000,
	//	50000000,	100000000
	//};
	//for (int i=0;i<6;i++)	
	//	if (dwPoint<uBasePoint[i]) 
	//		return i;
	//return 5;
}

//Ĭ�ϱ����ȼ�����
extern "C" __declspec(dllexport) TCHAR * GetMatchOrderName(__int64 dwPoint,UINT uGameNameID)
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
extern "C" __declspec(dllexport) TCHAR * GetNormalOrderName(__int64 dwPoint,UINT uGameNameID)
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
extern "C" __declspec(dllexport) int GetNormalOrderBasePoint(__int64 dwPoint)
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
extern "C" __declspec(dllexport) int GetNormalOrderLevel(__int64 dwPoint)
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
extern "C" __declspec(dllexport) TCHAR * GetCharmGrade(__int64 dwFascination)
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

/// ��ȡָ����ֵ���ַ���
/// ������ͬʱ֧��UNICODE��ANSI
/// @param[out] str �ַ������飬���ɵ����߷���ռ�
/// @param[in] nNum ����ʾ��������ֵ
/// @param[in] nPower ��10�ļ�������ʾ����123��ʾ��123000����˲���Ϊ3
/// @param[in] bHasSpace �ַ����Ƿ�ֽ���ʾ��Ĭ��ֵfalse��ʾ���ֲ��ֽڣ�true��ʾ�ÿո�ÿ��λ��һ��
/// @return ����str���ڵ�λ��
///
// PengJiLin, 2010-8-3, ���ӿ�������ָ���
// PengJiLin, 2011-7-8, 64λ����޸�
extern "C" __declspec(dllexport) TCHAR *GetNumString(TCHAR str[], __int64 nNum, int nPower, bool bHasSpace, TCHAR* chSpace)
{
	/// �ݴ���
	TCHAR strTmp[32];
	/// �����0����ֱ�ӷ���һ��0 
	if (0==nNum)
	{
		_tcscpy(str, TEXT("0"));
		return str;
	}

    wsprintf( strTmp, TEXT("%I64d"), nNum);

	//Add By YangJie 2011-02-21
	if (nPower >= 0) {
		for (int i=0; i<nPower; ++i)
		{
			_tcscat( strTmp, TEXT("0"));
		}
	} else {
		bool bFlag = false;
		if (nNum < 0) {
			::memset(strTmp, 0, sizeof(strTmp));
			wsprintf(strTmp, TEXT("%I64d"), -nNum);
			bFlag = true;
		}

		int iLen = _tcslen(strTmp);
		int tmpPower = -nPower;
		if (iLen > tmpPower) {
			for (int i = iLen; i > (iLen - tmpPower); i --) {
				strTmp[i] = strTmp[i-1];
			}
			strTmp[iLen-tmpPower] = TEXT('.');
			strTmp[iLen+1] = TEXT('\0');
		} else {
			TCHAR strTmp1[32];
			int i = 0;
			int j = 0;
			int k = 0;
			::memset(strTmp1, 0, sizeof(strTmp1));
			for (i = 0; i < (tmpPower - iLen + 1); i ++) {
				strTmp1[i] = TEXT('0');
			}
			for (j = i, k = 0; k < iLen;j ++, k ++) {
				strTmp1[j] = strTmp[k];
			}
			strTmp1[j++] = TEXT('\0');
			::memset(strTmp, 0, sizeof(strTmp));
			_tcscpy(strTmp, strTmp1);
			for (i = j; i > 1; i --) {
				strTmp[i] = strTmp[i-1];
			}
			strTmp[1] = TEXT('.');
			strTmp[j+1] = TEXT('\0');
		}

		if (bFlag) {
			iLen = _tcslen(strTmp);
			for (int i = iLen; i > 0; i --) {
				strTmp[i] = strTmp[i-1];
			}
			strTmp[0] = TEXT('-');
			strTmp[iLen+1] = TEXT('\0');
		}
	}

	// ���û�пո���ֱ�ӷ��������ַ���
	if (!bHasSpace)
	{
		_tcscpy(str, strTmp);
		return str;
	}

	if (nPower >= 0) {
		
		// PengJiLin, 2010-9-1, �������ö���λ��Ϊһ��, ��󲻳���8λ   
		CString s = CBcfFile::GetAppPath();
		CBcfFile f( s + "Order.bcf");
		BYTE byBits = f.GetKeyVal("VirtualMoney", "SpaceBits", 3);

		if(byBits < 1) byBits = 1;
		if(byBits > 8) byBits = 8;

		/// ��λ��ÿ��λ��һ����ʾ
		int nLen = _tcslen(strTmp);	// �ַ�������
		int nTimes = nLen % byBits/*3*/;		// ��λһ��ʱ�������λ��

		int nSrc = 0;
		int nDes = 0;

		// PengJiLin, 2010-8-3, ���ӿ�������ָ���
		TCHAR chUseSpace = 0;
		if(NULL == chSpace)
		{
			chUseSpace = ' ';
		}
		else
		{
			chUseSpace = chSpace[0];
		}

		for (int i=0; i<nLen; ++i)
		{
			/// ����ʱ����������ֵ�䲻�ӿո�
			if ((i>0) && (i%byBits==nTimes) && (!(i==1 && nNum<0)))
			{
				str[nDes++] = chUseSpace;
			}
			str[nDes++] = strTmp[nSrc++];
		}
		str[nDes] = TEXT('\0');
	} else {

		_tcscpy(str, strTmp);
	}

	return str;
}

///��ȡ�û�������ͼƬ����(����ʹ��)
/// @param[in] uLogoID �û���LogoID;
/// @param[in] i64Bank �û����;
/// @param[in] i64Money �û����Ͻ��
/// @return ��������ͼƬ����
extern "C" __declspec(dllexport) UINT GetAvatarIndexInMain(UINT uLogoID,__int64 i64Bank,__int64 i64Money)
{
// 	CString str;
// 	str.Format("DClient:GetAvatarIndexInMain(uLogoID = %d,i64Bank = %I64d,i64Money = %I64d)",uLogoID,i64Bank,i64Money);
// 	OutputDebugString(str);

	/*�û����������ʽ 
		0-�û�ʹ�ý�ҹ���ʽ 
		1-�û�ӵ�н�Ҿ��� 
		2-�û�ӵ�н�һ���־���*/
	int nChangeMethod = g_cfgModual.GetKeyVal("Avatar","ChangeMethod",0);

// 	if (nChangeMethod == 0) //�û�ʹ�ý�ҹ���ʽ 
// 	{
// 		return uLogoID;
// 	}
// 	else if (nChangeMethod == 1) //�û�ӵ�н�Ҿ��� 
// 	{
// 		return GetMoneyGrade(i64Money);
// 	}

	return uLogoID;
}

///��ȡ�û�������ͼƬ����(��Ϸ����ʹ��)
/// @param[in] uLogoID     �û���LogoID;
/// @param[in] i64Bank     �û����;
/// @param[in] i64Money    �û����Ͻ��;
/// @param[in] dwPoint     �û�����;
/// @param[in] uGameNameID ��ϷID;
/// @param[in] uComType    �������ͣ�1-Ϊ���ֳ���3-Ϊ��ҳ�
/// @return ��������ͼƬ����
extern "C" __declspec(dllexport) UINT GetAvatarIndexInRoom(UINT uLogoID,__int64 i64Bank,__int64 i64Money,long dwPoint,UINT uGameNameID,UINT uComType)
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

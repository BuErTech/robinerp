#include "Stdafx.h"
#include <afxdllx.h>
#include "Resource.h"
#include "ClientGameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE GameClientDLL = { NULL, NULL };

/**
 * DLL ��ں���
 */
extern "C" int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(GameClientDLL, hInstance)) return 0;
		new CDynLinkLibrary(GameClientDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(GameClientDLL);
	}
	return 1;
}

/********************************************************************************************/

/**
 * �ȼ��жϺ����������Ϸ��
 */
extern "C" __declspec(dllexport) TCHAR * GetGameOrderName(__int64 dwPoint)
{
	static __int64 uBasePoint[21]=
	{
		30,100,200,500,
		1000,1600,3000,5000,
		10000,20000,50000,80000,
		120000,200000,300000,400000,
		500000,600000,800000,1000000,
		1500000
	};
	static TCHAR * szOrderName[21]=
	{
		TEXT("����"),TEXT("���೤"),TEXT("���೤"),TEXT("���ų�"),
		TEXT("���ų�"),	TEXT("������"),	TEXT("������"),TEXT("��Ӫ��"),
		TEXT("��Ӫ��"),TEXT("���ų�"),TEXT("���ų�"),TEXT("���ó�"),
		TEXT("���ó�"),	TEXT("��ʦ��"),TEXT("��ʦ��"),TEXT("������"),
		TEXT("������"),TEXT("��˾��"),TEXT("˾��"),TEXT("����˾��"),
		TEXT("������˾��")
	};
	for (int i=0;i<21;i++)	
		if (dwPoint<uBasePoint[i]) 
			return szOrderName[i];
	return TEXT("��ί��ϯ");
}

/********************************************************************************************/
/**
 * ������Ϸ�ͻ���
 * �ͻ�����������
 */
GETCLIENTINFO;
GETGAMEICON(NAME_ID);
GETFRAMEINTERFACE(CClientGameDlg)
GETORDERFUNCTION(GetGameOrderName);

/********************************************************************************************/

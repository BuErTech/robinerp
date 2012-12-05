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
	
	static __int64 uBasePoint[13]=
	{
		0,50,150,350,				//30,100,200,500,
		750,1550,3150,6350,			//1000,1600,3000,5000,
		12750,20000,30000,40000,		//10000,20000,50000,80000,
		50000						//120000,200000,300000,400000,//500000,600000,800000,1000000,	//1500000
	};
	static TCHAR * szOrderName[13]=
	{
		TEXT("����"),TEXT("��ة"),TEXT("����"),TEXT("��ξ"),
		TEXT("Уξ"),TEXT("����"),TEXT("���ɽ�"),TEXT("̫��"),
		TEXT("��ʷ"),TEXT("����"),TEXT("̫ξ"),TEXT("�󽫾�"),
		TEXT("��")/*,TEXT("���Ⱥ��"),TEXT("���Ⱥ��"),TEXT("һ�Ⱥ��"),
		TEXT("���ȹ���"),TEXT("���ȹ���"),TEXT("һ�ȹ���"),TEXT("����"),
		TEXT("��������")*/
	};
	/*
	����=0
	��ة=50����=150��ξ=350Уξ=750����=1550���ɽ�=3150̫��=6350��ʷ=12750����=20000̫ξ=30000�󽫾�=40000��=50000
	*/
	for (int i=0;i<13;i++)	if (dwPoint<uBasePoint[i]) return szOrderName[i];
	return TEXT("��");
}

/********************************************************************************************/
/**
 * ������Ϸ�ͻ���
 * �ͻ�����������
 */
GETCLIENTINFO;
GETGAMEICON(NAME_ID);//JMod-CLIENT_DLL_NAME-20090213
GETFRAMEINTERFACE(CClientGameDlg)
GETORDERFUNCTION(GetGameOrderName);

/********************************************************************************************/

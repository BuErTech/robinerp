#include "Stdafx.h"
#include <afxdllx.h>
#include "Resource.h"
#include "ClientGameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE GameClientDLL = { NULL, NULL };

//DLL ��ں���
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

//�ȼ��жϺ�����ţţ��
extern "C" __declspec(dllexport) TCHAR * GetGameOrderName(__int64 dwPoint)
{
	static __int64 uBasePoint[15]=
	{
		-300,0,100,200,
		400,700,1100,1600,
		2200,2900,3700,4500,
		5500,6700,8000
	};
	static TCHAR * szOrderName[15]=
	{
		TEXT("����"),TEXT("���"),TEXT("����"),TEXT("��Ʒ֥���"),
		TEXT("��Ʒ��ة"),TEXT("��Ʒ֪��"),TEXT("��Ʒͨ��"),TEXT("��Ʒ֪��"),
		TEXT("��Ʒ֪��"),TEXT("��Ʒ����ʹ"),TEXT("��ƷѲ��"),TEXT("һƷ��ѧʿ"),
		TEXT("����"),TEXT("��ү"),TEXT("�ʵ�")
	};
	for (int i=0;i<15;i++)	if (dwPoint<uBasePoint[i]) return szOrderName[i];
	return TEXT("�ʵ�");
}

/********************************************************************************************/

//������Ϸ�ͻ���
//�ͻ�����������
GETCLIENTINFO;
GETGAMEICON(NAME_ID);
GETFRAMEINTERFACE(CClientGameDlg)
GETORDERFUNCTION(GetGameOrderName);

/********************************************************************************************/

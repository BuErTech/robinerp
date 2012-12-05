#pragma once

//��Ϣͷ�ļ�
#include "BaseMessage.h"
#include "GameRoomMessage.h"
//#include "..\..\���������\���.2003\ItemDateMessage.h"

//������ͷ�ļ�
#include "UserPower.h"
#include "ClientComStruct.h"

//������
#include "AFCWeb.h"
//#include "GameCard.h"
#include "AFCRegistry.h"

//������
#include "AFCEdit.h"
#include "AFCImage.h"
#include "AFCFunction.h"
#include "AFCResource.h"
#include "AFCButton.h"
#include "AFCDialog.h"
#include "AFCListCtrl.h"
#include "AFCTabCtrl.h"
#include "AFCWindow.h"
#include "AFCLogoResource.h"
#include "TCPClientSocket.h"
#include "AFCMenu.h"

//#include "SKinScrollWnd.h"
//#include "SKinScrollBar.h"
//������ 
#include "AFCExpressionWnd.h"
#include "MessageHandle.h"
#include "GameEndView.h"

 //wushuqun 2008.9.24
#include "ConfirmDlg.h"

//��Ϸ���
#include "PlayView.h"
#include "GameFrameDlg.h"
#include "GetPoint.h"
//#include "BroadcastMsgInputDlg.h"
#include "ShortMessage.h"
//���ֿռ�����
//using namespace AFC;
//using namespace AFC::AFCEdit;
//using namespace AFC::AFCSocket;
//using namespace AFC::AFCDialog;
//using namespace AFC::AFCButton;
//using namespace AFC::AFCWindow;
//using namespace AFC::AFCListCtrl;
//using namespace AFC::AFCResource;
//using namespace AFC::AFCImageGoGoGo;

#ifndef AFC_FACE_DLL
//��ȡ�ȼ���������
extern "C" __declspec(dllimport) TCHAR * GetMoneyOrderName(__int64 dwPoint,UINT uGameNameID);
extern "C" __declspec(dllexport) TCHAR * GetPointOrderName(__int64 dwPoint,UINT uGameNameID);
extern "C" __declspec(dllimport) void SetGameID(int iGameID);
extern "C" __declspec(dllimport) TCHAR * GetMatchOrderName(__int64 dwPoint,UINT uGameNameID);
extern "C" __declspec(dllimport) TCHAR * GetNormalOrderName(__int64 dwPoint,UINT uGameNameID);
extern "C" __declspec(dllimport) int GetNormalOrderBasePoint(__int64 dwPoint);
extern "C" __declspec(dllimport) int GetNormalOrderLevel(__int64 dwPoint);
extern "C" __declspec(dllimport) int  GetMoneyGrade(__int64 dwPoint);
//��������
extern "C" __declspec(dllimport) TCHAR * GetCharmGrade(__int64 dwCharm);

/// ��ȡָ����ֵ���ַ���
/// ������ͬʱ֧��UNICODE��ANSI
/// @param[out] str �ַ������飬���ɵ����߷���ռ�
/// @param[in] nNum ����ʾ��������ֵ
/// @param[in] nPower ��10�ļ�������ʾ����123��ʾ��123000����˲���Ϊ3
/// @param[in] bHasSpace �ַ����Ƿ�ֽ���ʾ��Ĭ��false��ʾ���ֲ��ֽڣ�true��ʾ�ÿո�ÿ��λ��һ��
/// @return ����str���ڵ�λ��
///
// PengJiLin, 2010-8-3, ���ӿ�������ָ���
extern "C" __declspec(dllexport) TCHAR *GetNumString(TCHAR str[], __int64 nNum, int nPower, bool bHasSpace, TCHAR* chSpace);

///��ȡ�û�������ͼƬ����(����ʹ��)
/// @param[in] uLogoID �û���LogoID;
/// @param[in] i64Bank �û����;
/// @param[in] i64Money �û����Ͻ��
/// @return ��������ͼƬ����
extern "C" __declspec(dllimport) UINT GetAvatarIndexInMain(UINT uLogoID,__int64 i64Bank,__int64 i64Money);

///��ȡ�û�������ͼƬ����(��Ϸ����ʹ��)
/// @param[in] uLogoID     �û���LogoID;
/// @param[in] i64Bank     �û����;
/// @param[in] i64Money    �û����Ͻ��;
/// @param[in] dwPoint     �û�����;
/// @param[in] uGameNameID ��ϷID;
/// @param[in] uComType    �������ͣ�1-Ϊ���ֳ���3-Ϊ��ҳ�
/// @return ��������ͼƬ����
extern "C" __declspec(dllimport) UINT GetAvatarIndexInRoom(UINT uLogoID,__int64 i64Bank,__int64 i64Money,long dwPoint,UINT uGameNameID,UINT uComType);

#else // AFC_FACE_DLL

extern "C" __declspec(dllexport) TCHAR * GetMoneyOrderName(__int64 dwPoint,UINT uGameNameID);
extern "C" __declspec(dllexport) TCHAR * GetPointOrderName(__int64 dwPoint,UINT uGameNameID);
extern "C" __declspec(dllexport) void SetGameID(int iGameID);
extern "C" __declspec(dllexport) TCHAR * GetMatchOrderName(__int64 dwPoint,UINT uGameNameID);
extern "C" __declspec(dllexport) TCHAR * GetNormalOrderName(__int64 dwPoint,UINT uGameNameID);
extern "C" __declspec(dllexport) int GetNormalOrderBasePoint(__int64 dwPoint);
extern "C" __declspec(dllexport) int GetNormalOrderLevel(__int64 dwPoint);
extern "C" __declspec(dllexport) int  GetMoneyGrade(__int64 dwPoint);
extern "C" __declspec(dllexport) TCHAR * GetCharmGrade(__int64 dwCharm);
extern "C" __declspec(dllexport) TCHAR *GetNumString(TCHAR str[], __int64 nNum, int nPower, bool bHasSpace, TCHAR* chSpace);

extern "C" __declspec(dllimport) UINT GetAvatarIndexInMain(UINT uLogoID,__int64 i64Bank,__int64 i64Money);
extern "C" __declspec(dllimport) UINT GetAvatarIndexInRoom(UINT uLogoID,__int64 i64Bank,__int64 i64Money,long dwPoint,UINT uGameNameID,UINT uComType);

#endif // AFC_FACE_DLL
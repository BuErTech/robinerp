#pragma once

//��Ϣͷ�ļ�
#include "../../common/basemessage.h"
#include "../../common/GameRoomMessage.h"
#include "../../common/commonuse.h"
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
#include "WndAnimate.h"
#include "AFCFunction.h"
#include "AFCResource.h"
#include "AFCButton.h"
#include "AFCDialog.h"
#include "AFCListCtrl.h"
//#include "AFCTabCtrl.h"
#include "AFCWindow.h"
#include "AFCLogoResource.h"
#include "AFCMenu.h"

//#include "TCPClientSocket.h"
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
//#include "GameFrameDlg.h"
#include "GetPoint.h"
//#include "BroadcastMsgInputDlg.h"
//#include "ShortMessage.h"
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

//��ȡ�ȼ���������
extern EXT_CLASS int GetMoneyOrderLevel(__int64 dwPoint);
extern EXT_CLASS TCHAR * GetMoneyOrderName(__int64 dwPoint);
extern EXT_CLASS TCHAR * GetPointOrderName(__int64 dwPoint);
extern EXT_CLASS TCHAR * GetMatchOrderName(__int64 dwPoint);
extern EXT_CLASS TCHAR * GetNormalOrderName(__int64 dwPoint);
extern EXT_CLASS int GetNormalOrderBasePoint(__int64 dwPoint);
extern EXT_CLASS int GetNormalOrderLevel(__int64 dwPoint);
extern EXT_CLASS int  GetMoneyGrade(__int64 dwPoint);
//��������
extern EXT_CLASS TCHAR * GetCharmGrade(__int64 dwCharm);


///��ȡ�û�������ͼƬ����(��Ϸ����ʹ��)
/// @param[in] uLogoID     �û���LogoID;
/// @param[in] i64Bank     �û����;
/// @param[in] i64Money    �û����Ͻ��;
/// @param[in] dwPoint     �û�����;
/// @param[in] uGameNameID ��ϷID;
/// @param[in] uComType    �������ͣ�1-Ϊ���ֳ���3-Ϊ��ҳ�
/// @return ��������ͼƬ����
UINT GetAvatarIndex(UINT uLogoID,__int64 i64Bank,__int64 i64Money,long dwPoint,UINT uGameNameID,UINT uComType);

///�Ƿ�VIP����Чʱ��
/// @param[in] lUserVipTime    VIP����Чʱ��;
/// @return ����lUserVipTime�Ƿ���
bool IsVipEffectTime(long lUserVipTime);

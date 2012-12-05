#if !defined(BRGAMEVIDEOSDK_H__INCLUDED_)
#define BRGAMEVIDEOSDK_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BRGameVideoSDK.h : header file
//
#include "brgamevideoclient.h"
#include "SDK/GVSDK.h"
#include "SDK/GVClientStyleDefine.h"
#include <list>

/**
 *	AnyChat������Ƶ����ƽ̨SDK�׼���װ��
 *	�����װ��SDK����ز��������ĳЩ����SDK�׼��У�������û�У������м���
 *	�ⲿ���Ը�����Ҫ�ʵ����޸ĸ���Ĳ��ִ��룬ʵ�����ϲ�ϵͳ���ں�
 *	��ϸ���ݿɲο�����BRGameVideoSDK��ʹ��ָ�ϡ�
 */

class CBRGameVideoSDK
{
// Construction
public:
	CBRGameVideoSDK();
	virtual ~CBRGameVideoSDK();

private:
	CBRGameVideoClient*		m_pBRGameVideoClient;		///< ��Ϸ��Ƶ�ͻ��˶���ָ��



private:
	// ������Ƶ������
	void SetVideoFrameStyle(void);


public:
	// �˳���ĳ�û���˽�ģ����߽�ĳ�û����Լ���˽���б������
	long PrivateChatExit(long userid);
	// �ظ��Է���˽������
	long PrivateChatEcho(long userid, long requestid, long bAccept);
	// ������Է�˽�ģ���Է�����˽������
	long PrivateChatRequest(long userid);
	// ��ȡָ���û���ǰ������ģʽ
	long GetUserChatMode(long userid);
	// ���ĵ�ǰ����ģʽ
	void ChangeChatMode(DWORD chatmode);

	// ��ָ���û���ӲƸ��˵���
	void AppendFortuneMenuItem(long userid, LPCTSTR menuText, LPCTSTR toolTip, short menuTag, short imageIndex);
	//���ã���գ�ָ���û��ĲƸ��˵�
	void ResetFortuneMenu(long userid);
	// ���òƸ��˵���ͼ��
	BOOL SetFortuneImage(LPCTSTR lpImageFile);

	// �õ�ָ��ID���û���
	CString GetUserNameById(int id);
	// ��ȡ��ǰ���������û��б�
	INT GetRoomUserList(std::list<INT>& userlist);
	/// ˢ��ָ�������
	void RefreshUserPanel(int id);
	// �����û��ĸ���ͼƬ
	void SetUserPicture(INT userid, LPCSTR pathname);
	// �����û�����������
	void SetUserTitle(INT userid, LPCTSTR lpTitleStr);
	// ��ʾ�����أ��û���Ƶ����
	void ShowUserVideo(INT userid, short bShow, CRect rc, CWnd* pParentWnd);
	// ��ʾ������Ƶ�������
	void ShowSettingPanel(void);
	// ��ȡ��������ͷ״̬
	INT GetLocalCameraState(void);
	// �رձ�������ͷ
	void CloseLocalCamera(void);
	// �򿪱�������ͷ
	void OpenLocalCamera(void);
	// �رձ���Mic
	void CloseLocalMic(void);
	// �򿪱���Mic
	void OpenLocalMic(void);
	// �뿪����
	void LeaveRoom(INT roomid=-1);
	// ���뷿��
	void EnterRoom(INT roomid, LPCSTR roompass=NULL);
	// �ӷ�����ע��
	void LogoutSystem(void);
	// ��¼������
	void LoginSystem(LPCSTR username=NULL, LPCSTR password=NULL);
	// ���ӵ�������
	void ConnectToServer(LPCSTR ipaddr, DWORD port=8902);
	// �ر�SDK
	void CloseGameVideoSDK(void);
	// ��ʼ��SDK
	BOOL InitGameVideoSDK(CWnd* pParent, HWND hMsgWnd=NULL);
};



#endif // !defined(BRGAMEVIDEOSDK_H__INCLUDED_)

#pragma once

//////////////////////////////////////////////////////////////////////////
///����֮�����Ϣ,�����������ڸ������ڽ��洫������
///2010.8.12 rongqiufen
//////////////////////////////////////////////////////////////////////////



#define MSG_CMD_GET_GAMEPARA				(WM_USER + 103)                              ///��ò���
#define MSG_CMD_CONNET_Z_SUCCEED			(WM_USER + 104)								 ///����Z�������ɹ�	
#define MSG_CMD_CONNET_Z_FAIL				(WM_USER + 129)								 ///����Z������ʧ��	
#define MSG_CMD_LONGIN						(WM_USER + 105)                              ///��¼	
#define MSG_CMD_GET_GAMEKIND				(WM_USER + 106)                              ///�����Ϸ����
#define MSG_CMD_GET_GAMENAME				(WM_USER + 107)								 ///�����Ϸ����
#define MSG_CMD_GET_GAMEROOM				(WM_USER + 108)								 ///�����Ϸ����
#define MSG_CMD_UPDATE_GAMEROOM				(WM_USER + 109)								 ///������Ϸ����
#define MSG_CMD_ENTER_GAMEROOM				(WM_USER + 110)								 ///������Ϸ����
#define MSG_CMD_SHOW_GAMEROOM				(WM_USER + 111)                              ///��ʾ��Ϸ����
#define MSG_CMD_SHOW_GAMEDESKSTATE			(WM_USER + 112)								 ///��ʾ���ӵ�״̬�������ˣ� ĩ��ʼ����Ϸ�е������µģ�
#define MSG_CMD_UPDATE_GAMEPEO				(WM_USER + 113)								 ///������Ϸ�������������£�

#define MSG_CMD_OPERATE						(WM_USER + 114)								 ///���û����������ԣ��߳����䣬��ֹ��¼��
#define MSG_CMD_BROADCAST					(WM_USER + 115)								 ///������


#define MSG_CMD_GM_ACT_AWORD				(WM_USER + 116)                              ///����
#define MSG_CMD_GM_ACT_NO_TALK				(WM_USER + 117)                              ///����
#define MSG_CMD_GM_ACT_KICK_OUT				(WM_USER + 118)                              ///�߳�����
#define MSG_CMD_GM_ACT_BANDON_ID			(WM_USER + 119)                              ///��ֹ��¼

#define MSG_CMD_CM_START_CHAT				(WM_USER + 121)                              ///����˽��


#define MSG_CMD_GM_TALK						(WM_USER + 122)                              ///�յ�˽����Ϣ 


#define MSG_CMD_GM_LOCK						(WM_USER + 123)                              ///����
#define MSG_CMD_GM_UNLOCK					(WM_USER + 124)                              ///���
#define MSG_CMD_GM_ALLLOCK					(WM_USER + 125)                              ///��������
#define MSG_CMD_GM_ALLUNLOCK				(WM_USER + 126)                              ///�������
#define MSG_CMD_GM_ALLLOCKOK				(WM_USER + 130)								 ///��������
#define MSG_CMD_GM_ALLUNLOCKOK				(WM_USER + 131)                              ///�������

#define	MSG_CMD_GM_FINDUSER					(WM_USER + 127)                              ///�����û�
#define	MSG_CMD_GM_GETUSER					(WM_USER + 128)								 ///ȡ���û�				

#define MSG_CMD_BACKWLOGIN					(WM_USER + 129)                              ///��̨��¼W 

#define MSG_CMD_GM_PRINTLOG					(WM_USER + 132)                              ///��ӡLogo��־
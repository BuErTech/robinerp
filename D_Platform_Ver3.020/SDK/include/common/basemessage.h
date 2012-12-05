#pragma once

#ifndef AFC_MESSAGE_HEAD_FILE
#define AFC_MESSAGE_HEAD_FILE

/********************************************************************************************/

///�˿ڶ��� 
#define SERVER_LOGON_PORT		3010							///�����½�˿�
#define GAME_LOGON_PORT			3015							///��Ϸ��½�˿�

#define CENTER_SERVER_PORT		13025							///���ķ���˿�
#define CENTER_DISTRI_PORT		13026							///���ķ�����ת�˿�

/********************************************************************************************/



///������汾
#define DEV_LIB_VER				1									///������汾
///ȫ�ְ�Ȩ���̶��ڳ�����޷��޸�
///#define TMLcopyright			TEXT("����:����ʦ����ѧ�������ѧѧԺwww.msdu.net")				///��Ȩ
///#define TMLcopyright			TEXT("����:���ӹ�����www.FengZi.net")				///��Ȩ
#define TMLcopyright			TEXT("����:�������������Ƽ����޹�˾www.kfgame.com")				///��Ȩ

///��������
#define MAX_PEOPLE				180									///�����Ϸ���� �ټ��� modify 8 -> 180 by wlr 20090714
#define MAX_SEND_SIZE			2044								///�����Ϣ��
#define MAX_TALK_LEN			500									///����������ݳ���
#define NORMAL_TALK_LEN			200									///��ͨ�������ݳ���
#define US_PLAY_GAME			20									///�û���Ϸ״̬
#define NET_HEAD_SIZE			sizeof(NetMessageHead)				///���ݰ�ͷ

///��Ϸ���Ͷ���
#define TY_NORMAL_GAME			1									///��ͨ��Ϸ
#define TY_MATCH_GAME			2									///������Ϸ
#define TY_MONEY_GAME			3									///�����Ϸ
#define TY_DRILLING_GAME		4									///��������Ϸ(������)
#define TY_FASCINATION_GAME		5									///��������Ϸ(������)
///
///Kylin 20090105 ������ҳ�
#define TY_SHABI_GAME		6									///���ҳ���Ϸ
///
///֧�����Ͷ���
#define SUP_NORMAL_GAME			0x01								///��ͨ��Ϸ
#define SUP_MATCH_GAME			0x02								///������Ϸ
#define SUP_MONEY_GAME			0x04								///�����Ϸ

///��ʾλ��
#define SHS_TALK_MESSAGE		0x01								///������Ϣ
#define SHS_MESSAGE_BOX			0x02								///������Ϣ
#define SHS_SYSTEM_SHOW			0x04								///ϵͳ��Ϣ

///�������ݰ��ṹͷ
struct NetMessageHead
{
	DWORD						uMessageSize;						///���ݰ���С
	DWORD						bMainID;							///����������
	DWORD						bAssistantID;						///������������ ID
	DWORD						bHandleCode;						///���ݰ��������
	DWORD						bReserve;							///�����ֶ�
};

//�˴�Ϊ��Կ�ɿͻ��Լ�����������������Χ
#define SECRET_KEY  150
///���ӳɹ���Ϣ 
struct MSG_S_ConnectSuccess
{
	BYTE						bMaxVer;							///���°汾����
	BYTE						bLessVer;							///��Ͱ汾����
	BYTE						bReserve[2];						///�����ֶ�
	__int64						i64CheckCode;						///���ܺ��У���룬�ɿͻ��˽����ڰ�ͷ�з���
};

/********************************************************************************************/

///	ϵͳ�ں�ʹ�� ��MAINID ʹ�� 0 - 49 ��
///	ϵͳ����ͨ�� ��MAINID ʹ�� 50 - 99 ��
///	��Ϸ�������� ��MAINID ʹ�� 100 - 149 ��
///	������Ϸͨ�� ��MAINID ʹ�� 151 - 199 ��
///	����ͨ�ű�־ ��MAINID ʹ�� 200 - 255 ��

/********************************************************************************************/

///������Ϣ
	#define MDM_NO_FAILE					0			///��ʧ����Ϣ

///������Ϣ
	#define MDM_CONNECT						1			///������Ϣ����
	///����������Ϣ��־
	#define ASS_NET_TEST					1			///�������
	#define ASS_CONNECT_SUCCESS 			3			///���ӳɹ�

/********************************************************************************************/

#endif
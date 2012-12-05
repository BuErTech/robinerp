#pragma once
#ifndef __UI_MESSAGE_H
#define  __UI_MESSAGE_H

#include <Windows.h>
#include <tchar.h>

///<UI��Ϣ����Ϣͷ
#define  WM_UI_MSG               WM_USER+1000
#define	 WM_USER_SOFTKEY		 WM_USER+1500

///UI��ϢС��Ϣͷ��TUIMessage��_uMessageType��Ա��ȡֵ
#define  UI_LOADFINISHED             200			///< UI�����������
#define  UI_LBUTTONDOWN              300       
#define  UI_LBUTTONUP                301
#define  UI_RBUTTONDOWN              302
#define  UI_RBUTTONUP                303
#define  UI_CATOONFINISH             304         ///<����������ɣ�Ҳ���ƿؼ�����/������Ϣ��_ch Ϊ 1 ���� _ch Ϊ 0 ����
#define  UI_KEYDOWN                  305
#define  UI_KEYUP                    306
#define  UI_MJCTRL_MOVEONMJ          307
#define  UI_MJCTRL_LBUTTON           308
#define  UI_DELETE_ITEM				 309		 /// ��Ͽ�ɾ�������б��е�Ԫ�أ���Ϣ�ṹ��_mouseX��ʾɾ���ĵڼ��0��
#define  UI_MOUSEENTER               310         /// �������/�Ƴ��ؼ�
#define  UI_LBUTTONDBDOWN            311       	 /// ������˫��
#define  UI_COMBOBOX_SELITEM         312         /// ��Ͽ�ѡ��ĳһ��
#define  UI_HUZICARD_DRAGUP          313         /// 
#define  UI_MOUSEMOVE                314         /// ����ƶ���Ϣ, ÿ���ؼ���Ҫ��������ƶ���Ϣ�Ż���Ӧ
#define  UI_COMBOBOX_CLKDROP         315		 /// ���������ť
#define  UI_TIME_RUN				  316			///ʱ��ؼ�������Ϣ���������ŵ���ʱ����
#define  UI_KEY_CHAR				  317			///�ַ���Ϣ����Ҫ�����ı���������Ϣ
#define  UI_BANK4GAME_CHECK			  318			///��Ϸ������Ϣ _ch Ϊ 1 ��ȡǮ�ɹ��� _ch Ϊ 0 ���Ǯ�ɹ�

typedef signed char         INT8, *PINT8;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
typedef signed __int64      INT64, *PINT64;
typedef unsigned char       UINT8, *PUINT8;
typedef unsigned short      UINT16, *PUINT16;
typedef unsigned int        UINT32, *PUINT32;
typedef unsigned __int64    UINT64, *PUINT64;


#pragma pack(4)
/// UIģ���������ڷ��͵���Ϣ��ʽ����
struct TUIMessage 
{
	TUIMessage(){memset(this, 0, sizeof(TUIMessage));}
	UINT32 _uControlID;			///< �ؼ�ID
	UINT32 _uMessageType;			///< ��Ϣ����
	UINT16 _mouseX, _mouseY;	///< ���У����Ĵ������꣬����Ļ����
	TCHAR _ch;					///< ���У�����ļ����ַ�
};
#pragma pack()

#endif// __UI_MESSAGE_H

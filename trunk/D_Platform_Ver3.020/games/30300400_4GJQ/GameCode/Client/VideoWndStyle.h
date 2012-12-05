#pragma once
/**
*  ��Ƶ��Ϣ����
*/
#define	WM_GV	WM_USER + 200

#define WM_GV_CONNECT				WM_GV + 1		///< �ͻ������ӷ�������wParam��BOOL����ʾ�Ƿ����ӳɹ�
#define WM_GV_LOGINSYSTEM			WM_GV + 2		///< �ͻ��˵�¼ϵͳ��wParam��INT����ʾ�Լ����û�ID�ţ�lParam��INT����ʾ��¼�����0 �ɹ�������Ϊ������룬�ο�������붨��
#define WM_GV_ENTERROOM				WM_GV + 3		///< �ͻ��˽��뷿�䣬wParam��INT����ʾ�����뷿���ID�ţ�lParam��INT����ʾ�Ƿ���뷿�䣺0�ɹ����룬����Ϊ�������
#define WM_GV_MICSTATECHANGE		WM_GV + 4		///< �û���Mic״̬�仯��Ϣ��wParam��INT����ʾ�û�ID�ţ�lParam��BOOL����ʾ���û��Ƿ����Mic
#define WM_GV_USERATROOM			WM_GV + 5		///< �û����루�뿪�����䣬wParam��INT����ʾ�û�ID�ţ�lParam��BOOL����ʾ���û��ǽ��루TRUE�����뿪��FALSE������
#define WM_GV_LINKCLOSE				WM_GV + 6		///< ���������ѹرգ�����Ϣֻ���ڿͻ������ӷ������ɹ�֮�������쳣�ж�֮ʱ����
#define WM_GV_ONLINEUSER			WM_GV + 7		///< �յ���ǰ����������û���Ϣ�����뷿��󴥷�һ�Σ�wParam��INT����ʾ�����û����������Լ�����lParam��INT����ʾ����ID
#define WM_GV_FORTUNEMENU			WM_GV + 8		///< �û�ѡ����һ��Ƹ��˵��wParam��INT����ʾ�û�ID�ţ�lParam��INT����ʾ�Ƹ��˵���ǣ�ָʾ��ѡ������һ��˵�

#pragma pack(push, 1)

/**
 *	��Ƶ���ڹ�������ť����
 */
#define GV_VFT_BTN_BTN_VIDEOCTRL	0x00000001L	///< ��Ƶ���ư�ť
#define GV_VFT_BTN_BTN_AUDIOCTRL	0x00000002L	///< ��Ƶ���ư�ť
#define GV_VFT_BTN_BTN_CLONEVIDEO	0x00000004L	///< ������Ƶ���ڰ�ť
#define GV_VFT_BTN_BTN_FORTUNE		0x00000008L	///< �Ƹ��˵���ť
#define GV_VFT_BTN_BTN_SNAPSHOT		0x00000010L	///< ���տ��ư�ť
#define GV_VFT_BTN_BTN_RECORDFILE	0x00000020L	///< ¼����ư�ť
#define GV_VFT_BTN_BTN_PLAYRECORD	0x00000040L	///< ����¼����ư�ť


/**
 *	��Ƶ���幤������ʾģʽ
 */
typedef enum{
	GV_VFTDM_AUTOSHOW = 0,						///< �Զ�ģʽ�������ʱ��ʾ����������
	GV_VFTDM_STILLSHOW,							///< ��ֹģʽ��ʼ����ʾ��������
	GV_VFTDM_VIDEOHIDE							///< ��Ƶ����ģʽ������Ƶ֮ǰʼ����ʾ����ֹģʽ��������Ƶ֮��Ϊ�Զ�ģʽ
}GV_VFT_DISPMODE;

/**
 *	��Ƶ���幤�������ṹ�嶨��
 */
typedef struct  
{
	DWORD		cbSize;							///< �ṹ���С
	COLORREF	bkColor;						///< ������������ɫ
	DWORD		maxTransPercent;				///< ���������͸���ȣ�0-!00��Ĭ��Ϊ70��ֵԽС��������Խ͸����
	DWORD		animateDuration;				///< �������仯�������ص���ʾ��������ʾ�����أ�ʱ������λ���룬Ĭ��ֵ3000
	DWORD		dwBtnMark;						///< �ڹ���������ʾ��Щ��ť����GV_VFT_BTN_BTN_XXXX��϶���
	DWORD		dwBtnSpacePix;					///< ��ť֮��ļ�������ص㣬Ĭ��Ϊ5��ֵԽС����ť����Խ��
	GV_VFT_DISPMODE		vftDispMode;			///< ��Ƶ���幤������ʾģʽ
}GV_VFT_STYLE,*LPGV_VFT_STYLE;


/**
 *	��Ƶ���ڱ߿�����
 */
typedef enum{
	GV_VF_STYLE_HIDEFRAME = 0,					///< �ޱ߿�
	GV_VF_STYLE_SMALLFRAME,						///< С�߿�
	GV_VF_STYLE_TOOLBAR							///< ������������ʾ�ı߿�
}GV_VFS;

/**
 *	��Ƶ����Ƥ����ʽ����
 */
typedef enum{
	GV_VF_SKIN_DEFAULT = 0,						///< Ĭ��Ƥ����ʽ
	GV_VF_SKIN_CRYSTALBLUE,						///< ˮ����Ƥ��
	GV_VF_SKIN_CLASSIC,							///< �ŵ�Ƥ��
}GV_VF_SKIN;
/**
 *	��Ƶ������ṹ�嶨��
 */
typedef struct  
{
	DWORD		cbSize;							///< �ṹ��Ĵ�С
	COLORREF	bkColor;						///< ��Ƶ���屳����ɫ
	COLORREF	selfTitleColor;					///< �Լ��ı�����ɫ
	COLORREF	otherTitleColor;				///< �����˵ı�����ɫ
	GV_VFS		vfStyle;						///< �߿���ʾ���
	BOOL		bMoveWindow;					///< �Ƿ�����ƶ���Ƶ����
	BOOL		bShowTitle;						///< �Ƿ���ʾ���⣬ͨ��Ϊ�û���
	BOOL		bShowVolume;					///< �Ƿ���ʾ����
	GV_VF_SKIN	vfSkin;							///< ��Ƶ����Ƥ����ʽ��ע�������߿�������Ϊ��GV_VF_STYLE_SMALLFRAME ʱ��Ч��
}GV_VW_STYLE,*LPGV_VW_STYLE;




#pragma pack(pop)
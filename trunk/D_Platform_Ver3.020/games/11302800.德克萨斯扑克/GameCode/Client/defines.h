// �����������༭���༭�����Ŀؼ��б��е�ID�ţ�����ֵӦ�������༭���б���һ��
// �����ɿ�ܴ����ID��Ӧ�����а��ţ����Ը��Ƶ�����д�����ȥ��Ϊ��֤��ܵ������
// �������ļ��ŵ�SDKĿ¼�£����Ƿŵ�������Ϸ��
#ifndef _DEFINES_H
#define _DEFINES_H
#include "../GameMessage/UpGradeMessage.h"

// ����Ϊ��ܴ�����
#define DDZ_EXIT					10001						// �˳�
#define DDZ_MIN_FRAME				10003						// ��С����Ϸ����	
#define DDZ_EXPRESSION				10008						// ����
#define DDZ_LIAOTIAN				10009						// ����
#define DDZ_LIAOTIAN_LIEBIAO		10010						// ��������ѡ���
#define DDZ_LIAOTIANJILU			10011						// �����¼
#define DDZ_FASONG					10013						// ���Ͱ�ť
#define DDZ_LABA					14003						// ������
#define DDZ_PLAY_MUSIC				154                         // ��������
#define DDZ_STOP_MUSIC				155                         // ֹͣ����
//#define DDZ_SOUND_CONFIG			155						    // ֹͣ����
#define DDZ_PHIZ					156							// ���鹦��
#define DDZ_CLOSE_PHIZ				156							// �رձ���
#define DDZ_NET_QUALITY				141                         // ����Ʒ��
#define DDZ_CAPTION					103							// ���ڱ���

#define SH_SINGLE_NOTE				501							// ��ע
#define SH_TOP_NOTE					502							// ��ע

// ����Ϊ��Ϸ�����д�����

// ��̬�ؼ�
#define SH_GAME_TITLE				103							// ��Ϸ����
//#define SH_NET_QUALITY			501							// �����������ɿ�ܴ���
#define SH_TOTAL_NOTE				504							// ��ע
#define SH_MY_NOTE					505							// ��ע�������ֵ

//��ţ��Ϸ

#define IDPIC_DLG_BG				800 //���õ�ע�Ի���ı���
#define IDB_CFG_DESK_BASE			801 //���õ�ע��ť

#define IDB_AGREE_DESK_CFG			808 //ͬ���ע��ť
#define IDB_DISAGREE_DESK_CFG		809 //��ͬ���ע��ť

#define IDC_DI_ZHU					1810//��ע
#define IDC_DING_ZHU				1811//��ע

#define IDT_SHOW_BASE_CARD			506	//��ʾ����
#define IDB_BURYCARD				507 //�۵���
#define IDB_REVOLU					508 //��Ҫ������ť

#define IDT_GRADES					811 //�Ƽ���ʾ
#define IDT_NT_NAME					812	//ׯ���������ʾ
#define IDT_RIOT_NT_NAME			813 //�����������ʾ
#define IDPIC_HT_HUAKIND_BASE		814 //��ǰ��Ļ�ɫ��ʾ
#define IDT_510K_POINTS				818 //��ǰ�÷�
#define IDPIC_510K_LIST_BASE		821 //��ǰ�÷ֱ�

#define IDT_DI_ZHU					850	//��ע
#define IDT_DING_ZHU				851	//��ע
#define IDPIC_WAIT_DIZHU			860 //�ȴ���ע����ʾ
#define IDPIC_WAIT_AGREE_DIZHU1		861 //�ȴ�ͬ���ע��ʾ
#define IDPIC_WAIT_AGREE_DIZHU2		862 //�ȴ�ͬ���ע��ʾ

#define IDVB_HIT_CARD				509 //�������ƿؼ�
#define IDB_OUT_CARD				510 //���ư�ť					
#define IDB_CUE						511 //��ʾ��ť
#define IDB_TUO_GUAN_ON				512 //��Ϸ�йܿ���ť
#define IDB_TUO_GUAN_OFF			513 //��Ϸ�йܹذ�ť
#define IDM_DLBCLICK				514 //����˫���¼�

#define IDPIC_SEL_HUA_KIND			527 //��ɫѡ��ť��
#define IDB_SEL_HUA_KIND			528 //��ɫѡ��ť
#define IDB_NEXT_ROUND_REQ			534 //��һ�غϿ�ʼ
#define IDC_SEND_CARD				535 //���ƶ�����ʾ���ƿؼ�

#define IDC_CLOCK_BASE				541 //ʱ��λ��
#define IDC_PLAYER_LOGO_BASE		551	//���Logo
#define IDT_PLAYER_ID_BASE			561	//���ID
#define IDPIC_PLAYER_BG				571 //��ҵ�ͼ
#define IDT_PLAYER_MONEY_BASE		581 //��ҵ���
#define IDC_OUTCARD_BASE			591 //������ƿؼ�
#define IDPIC_COINT_BASE			601 //���ͼ��
#define IDC_JETON_PRE_BASE			611	//δ������λ������
#define IDC_JETON_BASE				621	//��������λ������
#define IDC_CARD_POINT_VALUE_BASE	631 //����ĵ�ֵ
#define IDT_CARD_POINT_VALUE_BASE	1631//����ĵ�ֵ
#define IDPIC_TIPS_BASE				641 //��ʾ
#define IDC_HANDCARD_BASE			651 //�Լ����ϵ���
#define IDPIC_NT_FLAG_BASE			661 //ׯ�ұ��ͼ
#define IDT_PLAYER_IP				671	//���IP������
#define IDC_BASE_SHOW				119 //������ʾ�ؼ�
#define IDB_LAST_ROUND				135 //��һ����ʾ�ؼ�


/*********************************************************************************************************
*										�����ǵ¿���˹�˿˰�ť											 *
**********************************************************************************************************/
#define	IDB_BEGIN_GAME				200		// ��Ϸ��ʼ��ť
#define IDB_ALTER_STAKE				201		// �޸�ע�ť
#define IDB_BET						202		// ��ע��ť
#define	IDB_CALL					203		// ��ע��ť
#define IDB_RAISE					204		// ��ע��ť
#define IDB_CHECK					205		// ���ư�ť
#define IDB_FOLD					206		// ���ư�ť
#define IDB_ALL_IN					207		// ȫ��
#define IDB_BUTTON_MONEY			211		// ��ť
#define IDB_SLIDING_AXLE			220		// ������
#define IDB_ADDMONEY				221		// ��ע��+����ť
#define IDB_REDUCE					222		// ��ע��-����ť
#define IDB_CALLMANDATE				230		// ��ע�йܰ�ť
#define IDB_CHECKMANDATE			231		// �����йܰ�ť
#define IDB_CALLANYMANDATE			232		// ���κ�ע��ť
#define IDB_PASSABANDONMANDATE		233		// ����/�����йܰ�ť
#define IDB_CALLMANDATE1			750		// �򹳸�ע�йܰ�ť
#define IDB_CHECKMANDATE1			751		// �򹳹����йܰ�ť
#define IDB_CALLANYMANDATE1			752		// �򹳸��κ�ע��ť
#define IDB_PASSABANDONMANDATE1		753		// �򹳹���/�����йܰ�ť

#define IDC_REAL_MONEY				235		// ��ʵ�����ʾ
#define IDC_SHOW_BASE_CARD			236		// ���湫����
#define IDC_DINAR_MAP				237		// ��ʵ���LOGO
#define IDC_REAL_DINAR				238		// ��ʵ�����ʾ
#define IDC_ADDRESS					239		// ��ַ��ʾIP
#define IDC_USER_LOGO				241		// �û�LOGO * 8
#define IDC_USER_NAME				251		// �û����� * 8
#define IDC_UDER_ID					261		// �û�ID	* 8
#define IDC_USER_MONEY				271		// �û���� * 8
#define IDC_ZHUANG					304		// ׯ
#define IDC_SMALL_BLIND				305		// Сäע
#define IDC_BIG_BLIND				306		// ��äע
#define IDC_VICE_CARD				307		// һ����
#define IDC_SHOW_IMAGE				311		// ��ʾλ��
#define IDC_SHOW_CARD				331		// ��ʾ���� * 8
#define IDC_POT_DINAR				341		// ��ʾ�߳ؽ�� * 8
#define IDC_SIDE_POT				351		// ��ʾ�߳� * 8
#define IDC_JIN_BI					361		// ���LOGO��ʾ * 8
#define IDC_TABLE_BOARD_MONEY		371		// ������ * 8
#define IDC_MONEY					381		// �����ʾ * 8
#define IDC_CALL					391		// ��ע��ʾ * 8
#define IDC_RAISE					401		// ��ע��ʾ * 8
#define IDC_CHECK					411		// ������ʾ * 8
#define IDC_FOLD					421		// ������ʾ * 8
#define IDC_ALLIN					431		// ȫ����ʾ * 8
#define IDC_BET						441		// ��ע��ʾ * 8
#define IDC_BALANCE_POT				451		// �߳ؽ�����ʾ * 8
#define IDC_BALANCE_POT_MONEY		461		// �߳ؽ�����ʾ��� * 8
#define IDC_SHOW_SMALL_CARD			471		// С����ʾ * 8
#define IDC_CHAT_BUBBLE				481		// ����������ʾ * 8
#define IDC_CHAT_TYPE				491		// ��������������ʾ * 8
#define IDC_SHOW_IMAGE1				501		// Сäע��ʾ
#define IDC_USER_BASE_MAP			511		// ��ҵ�ͼ
#define IDC_USER_THIS_ID			520		// �������ID
#define IDC_USER_CHAIR				531		// �������
#define IDC_MONEY_MAP				541		// ������ͼƬ
#define IDC_SHOW_CARDTYPE_NAME      550     // ��������
#define IDPIC_BACK_CARD_BASE	    561     // ������ʾ���򣨷��Լ��ģ�
#define IDC_GAME_HELP				760		// ��������
#define IDC_READY_GO				780		// ��׼����Ϸ
#define IDC_READY					790		// ׼��ͼƬ
#define IDC_PROMPT					800		// ��ʾ�������
#define IDC_CLCKTIME				801		// ����ʱʱ��
#define IDC_MONEYLACK				830		// ��ұ������
#define IDT_MONEY_LACK				831		// ��ұ�����㶨ʱ��
#define IDC_SENDCARD_ANIM			841		// ����ID(841-860��ռ��)
#define IDC_YOU_WIN					840		// ��Ӯ��ͼƬ * 8
#define IDC_REFRESH_RECT            870     // ˢ������
/*********************************************************************************************************
*											�޸Ĵ�����Ի���											 *
**********************************************************************************************************/
#define IDB_OK_LOAD					600		// ȷ������
#define IDB_EXIT_GAME				601		// �˳���Ϸ
#define IDB_ADD						602		// ������
#define IDB_SUB						603		// �������
#define IDB_STOP					604		// �رհ�ť

#define IDC_TOTAL_MONEY				630		// �ܽ����
#define IDC_MAX_MONEY				631		// ��߽����
#define IDC_MIN_MONEY				632		// ��С�����
#define IDC_PLAN_MONEY				633		// ������ٽ����

#define IDD_LOADSELECTION_DLG       "resources\\config\\Confirm.xml"        //�޸�ע��Ի���
/*********************************************************************************************************
*											����Ի���													 *
**********************************************************************************************************/
#define IDC_VICTORY_ID				701		// Ӯ��ID
#define IDC_WIN_MONEY				711		// ��Ӯ�ö��ٽ��
#define IDC_INFO					721		// ��Ӯ����
#define IDC_SHUT					730		// �رս��㰴ť
#define IDC_DIBIAN                  740     // ��Ϸ���ڵױ�ͼ

#define IDD_FINISH_DLG				"resources\\config\\FinishDlg.xml"      //����Ի���
/*********************************************************************************************************
*											���öԻ���													 *
**********************************************************************************************************/
#define IDB_SET_BUTTON				820		// ���ð�ť
#define IDB_MUSIC_CLOSE				821		// ��Ч����
#define IDB_BACKDROP_MUSIC			822		// �������ֿ���
#define IDB_CHAT_SWITCH				823		// �������ݿ���
#define IDB_CLOSE					824		// �رս��㰴ť
#define IDB_MAKE_SURE				825		// ȷ�����ð�ť
#define IDB_CALL_OFF				826		// ȡ�����ð�ť

#define IDD_SOUND_DLG				"resources\\config\\SoundDlg.xml"      //�������öԻ���
#define IDB_SOUND_CFG_OK			1010	//��������OK
/*********************************************************************************************************
*																										 *
**********************************************************************************************************/


#define IDPIC_QIPAO_BASE			2044 //������ʾ����
#define IDT_QIPAO_BASE			    2048 //������ʾ

#define CN_FINISH_DLG				"resources\\config\\FinishDlg.xml"		// �����·��

#define IDT_FINISH_ID_BASE			301 //���ID
#define IDT_FINISH_WIN_LOSE_BASE	311 //�����Ӯ���
#define IDT_FINISH_POINTS_BASE		321 //��ҵ���
#define IDT_FINISH_COINT_BASE		1331 //��ҽ��
#define IDT_FINISH_RATE_INFO		901	//������Ϣ


// ��Ϣ��ʾ�Ի����� 
#define IDD_MSG_DLG_RES_CONFIRM		"resources\\config\\Confirm.xml"	// ȷ�϶Ի���·�� 
#define IDD_MSG_DLG_RES_SELECT		"resources\\config\\Select.xml"			// ѡ��Ի���·�� 
#define IDD_DLG_BTN_OK				10001	// ȷ����ť
#define IDD_DLG_BTN_CANCLE			10005	// ȡ����ť
#define IDD_DLG_TIP_LABLE			12345	// ��ǩ�ؼ�

#endif


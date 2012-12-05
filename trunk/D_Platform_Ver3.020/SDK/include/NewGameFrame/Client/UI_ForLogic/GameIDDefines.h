#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///�����Ϣ 50~120
#define     PLAYINFOSTART          50                           ///< �����Ϣ��ʼ���
#define     PLAYNUM                10                           ///< ��Ҹ���
#define		TEXT_LEV              PLAYINFOSTART                 ///< 50     �ȼ�  �൱��id0�ĵȼ�
#define		TEXT_ACCOUNT          (TEXT_LEV+PLAYNUM)            ///< 60     ID1�ʺ�
#define		TEXT_NICKNAME         (TEXT_ACCOUNT+PLAYNUM)        ///< 70     �ǳ�
#define     IMAGE_NICKNAME_BK     (TEXT_NICKNAME+PLAYNUM)       ///< 80     �ǳ���ʾ��ͼ   ������Щ�У���Щû��
#define		IMAGE_LOGO            (IMAGE_NICKNAME_BK+PLAYNUM)   ///< 90     ���ͼ���־
#define		TEXT_MONEY1           (IMAGE_LOGO+PLAYNUM)          ///< 100    ��Ǯ1   �����ʾ���ǽ��
#define		TEXT_MONEY2           (TEXT_MONEY1+PLAYNUM)         ///< 110    ��Ǯ2   �����ʾ����Ԫ��
#define		TEXT_MONEY3           (TEXT_MONEY2+PLAYNUM)         ///< 120    ��Ǯ3   ����
#define		IMAGE_LOGO_FRAME      (TEXT_MONEY3+PLAYNUM)         ///< 130    ���ͼ���ͼ

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///�ұ��� 700~800
#define     RIGHTBOARD_TEXT_NICKNAME        700        ///<�ǳ�
#define     RIGHTBOARD_TEXT_ID              701        ///<ID
#define     RIGHTBOARD_TEXT_JIFEN           702        ///<����
#define     RIGHTBOARD_TEXT_JINGBI          703        ///<���
#define     RIGHTBOARD_TEXT_MEILI           704        ///<����
#define     RIGHTBOARD_TEXT_DENJI           705        ///<�ȼ�

#define     RIGHTBOARD_USERLIST             710        ///<�û��б�
#define     RIGHTBOARD_USERMENU             711        ///<�û��˵�

#define     RIGHTBOARD_PROP_CTN             720        ///<��������
#define     RIGHTBOARD_PROP_PROP_1          721        ///<������������
#define     RIGHTBOARD_PROP_PROP_1_BTN_USE  722        ///<��������ʹ�ð�ť
#define     RIGHTBOARD_PROP_PROP_1_TEXT     723        ///<��������������ʾ
#define     RIGHTBOARD_PROP_BTN_NEXT1       724        ///<��������
#define     RIGHTBOARD_PROP_BTN_PRE1        725        ///<��������
#define     RIGHTBOARD_PROP_BTN_NEXT2       726        ///<��������
#define     RIGHTBOARD_PROP_BTN_PRE2        727        ///<��������

#define     RIGHTBOARD_BTN_SHOP             750        ///<�̳ǰ�ť
#define     RIGHTBOARD_BTN_VIP              751        ///<VIP��ť

#define     RIGHTBOARD_BTN_SET              760        ///<���ð�ť
#define     RIGHTBOARD_BTN_HOME             761        ///<��ҳ��ť
#define     RIGHTBOARD_BTN_CHARG            762        ///<��ֵ��ť 


#define     RIGHTBOARD_BTN_SHOW             798        ///<��ʾ�ұ�����ť
#define     RIGHTBOARD_BTN_HIDE             799        ///<�����ұ�����ť
#define     RIGHTBOARD_CONTAIN              800        ///<�ұ�������


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///�����   ID���� 200-249
#define		TALK_SHOW              200      ///<������Ϣ��ʾ��
#define		ETEXT_TALK_INPUT       201      ///<���������
#define		BN_TALK_SEND           202      ///<������Ͱ�Ť
#define		BN_TALK_EXPRESSIO      203      ///<������鰴Ť
#define		ETEXT_TALK_COMBO       204      ///<��������������
#define		TALK_MENU			   210      ///<�����Ҽ��˵�

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///��С������Ϣ�����
#define CTN_MsgInput		300	
#define CTN_MsgInput_OK		302	
#define CTN_MsgInput_Close  305 
#define CTN_MsgInput_Cancel 303 
#define CTN_MsgInput_Edit   301 
#define CTN_MsgInput_Tip    304 


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///�鿴��Ҳ˵� 400
#define		MEUN_LOOK			400



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///�Ի���  500~510
#define		CTN_DIALOG			500					///�Ի���
#define		BN_DLALOG_OK		501					///�Ի���ȷ��
#define		BN_DLALOG_CANCEL	504					///�Ի���ȡ��
#define     BN_DLALOG_CLOSE		505					///�Ի���ر�
#define		TX_DLALOG_MSG		506					///�Ի�����Ϣ


///���ÿ�  401~405
#define     BTN_SET             520                 ///���ð�ť
#define     SET_ID_BEGIN        401
#define     CTN_SET_DLG         401                 ///���ÿ�
#define     BTN_SET_CLOSE       402                 ///���ÿ�ر�
#define     BTN_SET_OK          403                 ///ȷ����ť
#define     BTN_SET_CANCEL      404                 ///ȡ����ť
#define     SLIDER_SET_VOL      408                 ///����
#define     CHECK_ENABLEWATCH   405                 ///�����Թ�
#define     SET_ID_END          420

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///����� 599~650
#define CTN_BIAOQING_KUANG 599  ///����
#define IMG_BIAOQING_0  600     ///��0������
#define IMG_BIAOQING_49 649     ///��49������
#define IMG_BIAOQING_BJ 650     ///���鱳��



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///���߹������� 801~850
#define     BUYPROP_CTN                     801     ///< ���߹�������
#define     BUYPROP_WEB_PROPINFO            802     ///< ������Ϣ��WEB�ؼ���
#define     BUYPROP_TEXT_OWNNUM             803     ///< ӵ������ 
#define     BUYPROP_TEXT_BUYNUM             804     ///< �������� 
#define     BUYPROP_BTN_USE                 805     ///< ʹ�ð�ť
#define     BUYPROP_BTN_BUY                 806     ///< ����ť
#define     BUYPROP_BTN_CLOSE               809     ///< �رհ�ť
#define     BUYPROP_TEXT_NEEDGOLD           807     ///< ����������
#define     BUYPROP_TEXT_HAVEGOLD           808     ///< ���н��
#define     BN_BUY_VIP                      810


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///��¡���߰�ť 1000��
#define     PROP_BTN_BEGIN                  1000
#define     PROP_BTN_END                    2000


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///��Ϸͨ��ID 1~50
#define     BN_START_GAME		   10001   ///< ��ʼ��Ϸ
#define     BN_RESET_FRAME         9       ///��ԭ��ť         
#define		BN_MIN_FRAME           10      ///< ��С������
#define		BN_SET_FRAME           12      ///< ������Ϸ��������Ϣ
#define		BN_CLOSE_FRAME         11      ///< �ر���Ϸ����

#define     BN_VOLUM_UP            13      ///< ������
#define     BN_VOLUM_DOWN          14      ///< ������
#define     BN_VOLUM_OFF           15      ///< ���ֹ�
#define     BN_VOLUM_ON            16      ///< ���ֿ�

#define     IMAGE_NET_SIGNAL       17      ///< �����ź�

#define     BN_SHOW_RIGHTFRAME     20      ///< ��ʾ�ɱ߿�

#define     USERPROP_ANIMAL_0      3500     ///�û����㶯��


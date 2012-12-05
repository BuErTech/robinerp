#include"stdafx.h"
//������������
#define SOUND_WIN					0x01				//ʤ��
#define SOUND_LOSE					0x02				//��
#define SOUND_WAITING				0x03				//�ȴ�
#define SOUND_ERROR					0x04				//���Ƴ�����
#define SOUND_READY					0x05				//��Ϸ��ʼ
#define SOUND_START					0x06				//׼������һ����
#define SOUND_START_OUT_CARD		0x07				//����һ����
#define SOUND_JIAO_FEN				0x08				//�з�
#define SOUND_OUT_CARD_FINISH		0x09				//ĳ��ҳ��ƽY��
#define SOUND_GAME_PASS				0x0A				//����
#define SOUND_FRIEND_APPEAR			0x0B				//���ѳ��F
#define SOUND_USER_RUNAWARY			0x0C				//��������
#define SOUND_USER_LEFT				0x0D				//�����뿪
#define SOUND_GIVE_BACK				0x0E				//���
#define SOUND_HIT_CARD              0x11                //����Ƶ�����
#define	SOUND_HIT_BEGIN             0x12                //�����ʼ��ť����

#define SOUND_OUT_CARD				0x13				//��������
#define SOUND_WARNING_5				0x14				//���� 5
#define SOUND_WARNING_4				0x15				//���� 4
#define SOUND_WARNING_3				0x16				//���� 3
#define SOUND_WARNING_2				0x17				//���� 2
#define SOUND_WARNING_1				0x18				//���� 1
#define SOUND_WARNING_0				0x19				//���� 0
#define SOUND_SEND_CARD				0x1A				//��������
#define SOUND_OUT_CARD_DEF			0x1B				//Ĭ�ϳ�������
#define SOUND_OUT_CARD_BOMB			0x1C				//��ը��
#define SOUND_OUT_CARD_PASS			0x1D				//����
#define SOUND_INVALIDATE			0x1E				//��ʾʧ��
#define SOUND_START_DEF				0x20				//�ĸ��˿�ʼ��Ϸ��
#define SOUND_GAME_OVER				0x21				//��Ϸ����


#define SOUND_NO_POINT				   0x22			   //�޵���
#define SOUND_BULL_ONE                 0x23            //ţһ
#define SOUND_BULL_TWO                 0x24            //ţ��
#define SOUND_BULL_THREE               0x25            //ţ��
#define SOUND_BULL_FOUR                0x26            //ţ��
#define SOUND_BULL_FIVE                0x27            //ţ��
#define SOUND_BULL_SIX                 0x28            //ţ��
#define SOUND_BULL_SEVEN               0x29            //ţ��
#define SOUND_BULL_EIGHT               0x2A            //ţ��
#define SOUND_BULL_NINE                0x2B            //ţ��
#define SOUND_BULL_BULL				   0x2C				//ţţ
#define SOUND_BULL_SILVER              0X2D            //��ţ
#define SOUND_BULL_GOLD                0X2E            //��ţ
#define SOUND_FIVE_SMALL               0X30            //��С
#define SOUND_BULL_BOMB                0X31            //ը��

//add by xyh 2011-12-13
#define SOUND_GIVEUP_NT				   0X40				//������ׯ
#define SOUND_CALL_NT				   0X41				//��ׯ
#define SOUND_USER_CHIP				   0X42				//�����ע
#define SOUND_CHONG_PAI				   0X43				//����
#define SOUND_TAN_PAI				   0X44				//̯��
#define SOUND_USER_LEAVE			   0X45				//�뿪
#define SOUND_USER_CUE				   0X46				//��ʾ


//��ע����(�������)
#define TYPE_GIVE_UP					0x00			//����
//#define TYPE_NOTE						0x01			//��ע
#define TYPE_ADD						0x02			//��ע
#define TYPE_FOLLOW						0x03			//��ע
#define TYPE_SHUTTLE					0x04			//���
#define TYPE_NN                         0x05            //ţţ

//������ע����(רΪ���׼��)
BOOL SetNoteSound(BYTE bNoteStyle,BOOL bIsBoy);
//����ͨ������
BOOL SetPlaySound(BYTE bSoundType);
//�������������O��
HWND SetOutCardTypeSound(BYTE iCardList[],int iCardCount,HWND hParent,BOOL IsBoy = true,BOOL IsFirest = TRUE);
//����mp3����
HWND PlayCardMusic(TCHAR FileName[],HWND hParent);
//���볡������
HWND SetSceneSound(BYTE iSceneSound,HWND hParent,BOOL IsBoy = TRUE,int iVal = -1);

#if !defined(PLAYERMANAGER_COMMON_H)
#define PLAYERMANAGER_COMMON_H


namespace MyGame
{

#ifndef IPC_VERSION
//////////////////////////////////////////////////////////////////////////
//���岿�֣�begin.

	#define MAX_NICK_NAME_LENGTH		20

//���岿�֣�end.
//////////////////////////////////////////////////////////////////////////

#else //define IPC_VERSION

//////////////////////////////////////////////////////////////////////////
//IPC���֣�begin.

	#define MAX_NICK_NAME_LENGTH		30

//IPC���֣�end.
//////////////////////////////////////////////////////////////////////////

#endif
	#define MAX_NUMBER_PLAYER	10
	#define MAX_NUMBER_WATCHER	16

	//��ҵ������Ϣ
	typedef enum enmPlayerTeam
	{
		EPT_TEAM_NULL,
		//���Ϊ�����˵�ʱ��ʹ�� EPT_TEAM_0��EPT_TEAM_9��Ӧ�ڴ�������λ��
		EPT_TEAM_0,
		EPT_TEAM_1,
		EPT_TEAM_2,
		EPT_TEAM_3,
		EPT_TEAM_4,
		EPT_TEAM_5,
		EPT_TEAM_6,
		EPT_TEAM_7,
		EPT_TEAM_8,
		EPT_TEAM_9,
		//���ѡ������ʹ�õĶ����ʶ
		EPT_TEAM_A,
		EPT_TEAM_B,
		EPT_TEAM_C,
		EPT_TEAM_D,
		EPT_TEAM_E,
		EPT_TEAM_F,
		EPT_TEAM_G,
		EPT_TEAM_H,
		EPT_TEAM_I,
		EPT_TEAM_END
	}ENM_PLAYER_TEAM;

	typedef enum enmUserStatus
	{
		EUS_GETOUT     = 0, // 0,�뿪��
		EUS_FREE       = 1, // 1,�ڷ���վ��
		EUS_SITDOWN    = 2, // 2,���������ϣ�û����ʼ
		EUS_READY      = 3, // 3,ͬ����Ϸ��ʼ
		EUS_PLAYING    = 4, // 4,������
		EUS_OFFLINE    = 5, // 5,���ߵȴ�����
		EUS_SPECTATING = 6, // 6,�Թ�
	} ENMUSERSTATUS;
	

	//�û����
	typedef enum enmUserIdentifier
	{
		EUI_PLAYER_ADMIN = 0x40, 
		EUI_VIP_PLAYER   = 0x80
	}ENM_USER_IDENTIFIER;
	
	//!��Ϸ���������
	typedef enum enmRoomType
	{
		ERT_NORMAL   = 0,		/*!< ��ͨ��*/
		ERT_MONEY,				/*!< ��Ϸ�ҳ�*/
		ERT_HAPPYBEAN			/*!< ���ֶ���*/
	}ENM_ROOM_TYPE;
}
#endif


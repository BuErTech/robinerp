
// ******************************************************************************************************
//  UpgradeMessage   version:  1.0   ��  date: 03/29/2011
//  --------------------------------------------------------------------------------------------------

//	�¿���˹�˿ˣ���Ϸ������-�ͻ���Э��\
NameID��51300280

//  --------------------------------------------------------------------------------------------------
//  Copyright (C) 2011 - All Rights Reserved
// ******************************************************************************************************
// 
// ******************************************************************************************************

#pragma once

#ifndef AFC_SERVER_DKSSPK_11302800_MESSAGE_HEAD_FILE
#define AFC_SERVER_DKSSPK_11302800_MESSAGE_HEAD_FILE

#define SUPER_PLAYER                    10				// �������

//�ļ����ֶ���
#define GAMENAME						TEXT("�¿���˹�˿�")

//�汾����
#define GAME_MAX_VER					1				// ������߰汾
#define GAME_LESS_VER					1				// ������Ͱ汾
#define GAME_CHANGE_VER					0				// �޸İ汾

//��Ϸ�����汾
#define DEV_HEIGHT_VERSION				10				// �ڲ������߰汾��
#define DEV_LOW_VERSION					1				// �ڲ������Ͱ汾��

//֧������
#define SUPPER_TYPE						SUP_NORMAL_GAME|SUP_MATCH_GAME|SUP_MONEY_GAME

//��Ϸ��Ϣ
#define NAME_ID							11302800		// ���� ID
#define PLAY_COUNT						8				// ��Ϸ����
#define GET_SERVER_INI_NAME(str)		sprintf(str,"%s%d_s.ini",CINIFile::GetAppPath(),NAME_ID);
#define MAX_NAME_INFO                   256

//��Ϸ���ݳ������
#define MAX_CARDS_NUM					216				// �ĸ��Ƶ���������Ҳ����֧����������
#define MAX_HANDCARDS_NUM				52				// ���ϳ��е�����������
#define MAX_510K_CARDS_NUM				48				// �ĸ���5/10/K����������
#define MAX_FIRST_CARDS					1				// ��������
#define MAX_DEAL_CARDS					2				// ��������
#define MAX_THREE_CARDS					3				// ��������
#define MAX_BACKCARDS_NUM				16				// ���ĵ�������

#define TYPE_PLAY_GAME					10000			// �����



#define TAG_INIT(tagName) \
    tagName(){Clear();} \
    void Clear()

//******************************************************************************************
//���ݰ���������ʶ
//******************************************************************************************

#define ASS_UG_USER_SET					50				//�û�������Ϸ
#define ASS_UG_USER_SET_CHANGE			51				//������ø���

#define ASS_GAME_BEGIN					120				//������Ϸ��ʼ
#define ASS_SYS_DESK_CFG				121				//ȷ����ע�����Ϣ
#define ASS_CFG_DESK					122				//������õ�ע
#define ASS_DESK_CFG					123				//ȷ����ע�����Ϣ
#define ASS_AGREE_DESK_CFG				124             //ͬ���ע
#define ASS_BET							125             //�����ע��Ϣ
#define ASS_SEND_A_CARD					130				//��1������Ϣ
#define ASS_SEND_3_CARD					131				//��3������Ϣ
#define ASS_SEND_4_5_CARD				132				//��4,5������Ϣ
#define ASS_SEND_CARD_FINISH			133				//���ƽ���
#define ASS_BETPOOL_UP					134				//�߳ظ���
#define ASS_BET_REQ						135				//��ע����
#define ASS_BET_INFO					136				//��ע��Ϣ
#define ASS_NEW_USER					137				//��һ����Ҳ���
#define ASS_PASS_USER					138				//���Ƶ���Ҳ���
#define ASS_SUB_MENOY                   139             //����������
#define ASS_COMPARE_CARD                140             //������Ϣ

#define ASS_CALLMANDATE				    141				//��ע�й�
#define ASS_CHECKMANDATE                142             //�����й�
#define ASS_CALLANYMANDATE              143				//���κ�ע�й�
#define ASS_PASSABANDONMANDATE          144             //���ƻ������й�

#define ASS_USER_LEFT_DESK              145             ///

#define ASS_SORT_OUT					170				//����ͼƬ

#define ASS_TOKEN						186				//������Ϣ
#define ASS_RESULT						191				//�غ����
#define ASS_SHOW_IN_MONEY				192				//��ʾ������
#define ASS_LOOK_CARD					193				//����
#define ASS_CAN_SHOW_CARD				194				//��չʾ�Ƶ���Ϣ
#define ASS_SHOW_CARD					195				//չʾ�Ƶ���Ϣ


//�쳣����Ϣ
#define ASS_NO_PLAYER					155				//û����ҽ�����Ϸ
#define ASS_AGREE						156				//����Ƿ�ͬ�����Ϣ
#define ASS_CFG_DESK_TIMEOUT			157				//���õ�ע��ʱ
#define ASS_NOT_ENOUGH_MONEY			158				//��ҽ�Ҳ������������Ϣ
#define ASS_MONEY_LACK					159				//��ҽ�Ҳ������ʾ

#define ASS_NEXT_ROUND_REQ				150				//�����һ�غ�����
#define ASS_NEW_ROUND_BEGIN				151				//��һ�غϿ�ʼ


//��������Ϣ
#define ASS_FINISH_ROUND_REQ			180				//����������Ϸ

//******************************************************************************************
// ��ע����
enum emType
{
    ET_UNKNOWN 		= 0,	//δ֪����
    ET_BET			= 1,	//��ע
    ET_CALL			= 2,	//��ע
    ET_ADDNOTE 		= 3,	//��ע
    ET_CHECK		= 4,	//����
    ET_FOLD    		= 5,    //����
    ET_ALLIN		= 6,	//ȫ��
    ET_AUTO			= 7,    //�Զ���ע
};

// ��������
enum emTypeCard
{
    UNKNOWN			= 0,	//δ֪����
    SEND_A_CAND		= 1,	//�·�һ������
    SEND_3_CAND		= 2,	//�·� 3 ������
    SEND_4_5_CAND   = 3,	//�·� 4, 5 ������
};

// ����������
enum emTypeMoney
{
	NT_UNKNOWN	= 0,	//δ֪����
	NT_LOAD		= 1,	//ȷ��������
	NT_EXIT		= 2,    //�˳���Ϸ
	NT_ADD		= 3,    //���ӽ��
	NT_SUB		= 4,    //���ٽ��
};

// �й�״̬
enum emToken
{
	TO_UNKNOWN					= 0,	// δ֪����
	TO_CALLMANDATE				= 1,	// ��ע�йܰ�ť
	TO_CHECKMANDATE				= 2,    // �����йܰ�ť
	TO_CALLANYMANDATE			= 3,    // ���κ�ע��ť
	TO_PASSABANDONMANDAT		= 4,    // ����/�����йܰ�ť
};


//֪ͨ������Ϸ��
struct TCfgDesk
{
    //��ע���
    struct TDizhu
    {
        bool	bCanSet;		//��ҿ����Ƿ�������ã��·�ʱ��
        int		iLowers[5];		//���5����ѡ�ĵ�עֵ���·�ʱ��
        bool    bSelects[5];	//��5��ֵ�Ŀ�ѡ����
        BYTE	bySelect;		//��Ҵ�iValuesѡ������һ����עֵ���Ϸ�ʱ��

        void Clear()
        {
            bCanSet = false;	//��ҿ����Ƿ��������
            memset(iLowers,0,sizeof(iLowers));		 //���������ѡ�ĵ�עֵ
            memset(bSelects,false,sizeof(bSelects)); //������ֵ�Ŀ�ѡ����
            bySelect = 255;	//��Ҵ�iValuesѡ������һ����עֵ
        }

    } dz;

    TAG_INIT(TCfgDesk)
    {
        dz.Clear();
    }

};

//�й���������
struct TDeskCfg
{
    //��ע���
    struct TDizhu
    {
        __int64	iLowers[5];						// ���5����ѡ�ĵ�עֵ
        __int64	iUppers[5];						// ���5����עֵ
        __int64 iRoomMultiple;                  // ���䱶��
        __int64	iLower;							// ϵͳ�Զ����õĵ�עֵ
        __int64	iUpper;							// ϵͳ�Զ����õĶ�עֵ
        __int64	iSmallBlindNote;				// Сäע���õĵ�עֵ
        __int64	iBigBlindNote;					// ��äע���õĵ�עֵ

        // ����������
        bool    bIsRoomSubMoney;                // �˷����Ƿ���Ҫ�����Ҳſ���
        __int64 iSubMinMoney;                   // ��������ʹ�����
        __int64 iSubMaxMoney;                   // ��������ߴ����� 
		__int64	iSubPlanMoney;					// ��ǰ׼��������
		__int64	iMinusMoney;					// �޸�ע��Ӽ����
		__int64	iMinMoney;						// ��С��ע���
		__int64	iUntenMoney;					// ���ڶ��ٽ��ʱ���� 

		bool iUserClose;						// һ�ֺ�10���ڽ����ǿ��
		int iUserCloseTime;						// �رտͻ���ʱ��
		int iUserInMoney;						// ��Ϸ������ʱ��

		emTypeMoney nType;						// ���������

        void Clear()
        {
            memset(iLowers,0,sizeof(iLowers));	// ���5����ѡ�ĵ�עֵ
            memset(iUppers,0,sizeof(iUppers));	// ���5����עֵ
            iLower = 0;							// ϵͳ�Զ����õĵ�עֵ
            iUpper = 0;							// ϵͳ�Զ����õĶ�עֵ
            iSmallBlindNote = 0;				// Сäע���õĵ�עֵ
            iBigBlindNote = 0;					// ��äע���õĵ�עֵ
			nType = NT_UNKNOWN;					// δ֪����
        }

    } dz;

    //ʱ�����
    struct TTime
    {
        BYTE   byOperate;    	 // ��Ҳ�����ʱ��

        void Clear()
        {
            byOperate = 0;	 	 // ��Ҳ�����ʱ��
        }
    }Time;

    //�������
    struct TRule
    {
        int  nPokeNum;			//ʹ�ö��ٷ��˿�
        void Clear()
        {
            nPokeNum = 1;		//ʹ�ö��ٷ��˿�
        }

    }Rule;

    //��������
    bool bShowUserInfo;			// �Ƿ���ʾ��Һ���ݣ������ڱ�����

    TAG_INIT(TDeskCfg)
    {
        dz.Clear();
        Time.Clear();
        Rule.Clear();
    }

};

//ĳ����Ƿ����ע����Ϣ��
struct TAgree 
{
    BYTE byUser; 				// ��Ϣ���������
    bool bAgree; 				// �Ƿ�ͬ��
    bool bAllAgree; 			// ��ǰ�Ƿ�������Ҷ�ͬ����Ϸ

    TAG_INIT(TAgree)
    {
        byUser = 255;			// ��Ϣ���������
        bAgree = true;			// �Ƿ�ͬ��
        bAllAgree = false;		// ��ǰ�Ƿ�������Ҷ�ͬ����Ϸ
    }
};

//������ע��
struct TBet
{
    emType	nType;				// ��ע����
    BYTE	byUser;				// ��ע�����
    BYTE	bUserman[8];			// �������
    __int64 nMoney;				// ���
	__int64	nBetMoney;			// ��ע�������

    TAG_INIT(TBet)
    { 
        nType = ET_UNKNOWN;		// ��ע����
        byUser = 255;			// ��ע�����
        //bUserman = 255;			// �������
		memset(bUserman,0,sizeof(bUserman));
        nMoney = 0;				// ��ҵ�ѡ��
		nBetMoney = 0;			// ��ע�������
    }
};

// ������
struct TSubMoney
{
	BYTE	bDeskStation;
	__int64 nMoney[PLAY_COUNT];
	bool	bIn[PLAY_COUNT];       // ׼��״̬
	
	TSubMoney()
	{
		memset(nMoney, 0, sizeof(nMoney));
		memset(bIn, 0, sizeof(bIn));
		bDeskStation = 255;
	}
}rmb;


// �������ݰ�
struct TCompareCard
{
    int  nCardKind[PLAY_COUNT];		 // �������
    BYTE bCards[PLAY_COUNT][5];		 // ���������ɵ�������
	BYTE bCardsEx[PLAY_COUNT][2];    // ʣ��2��û�õ���
	BYTE bHandCards[PLAY_COUNT][2];  // �������
	int nCardCount;                  // ���������
    int nHandCardCount[PLAY_COUNT];  // �����������

    TCompareCard()
    {
        memset(this, 0, sizeof(TCompareCard));
		memset(bHandCards, 255, sizeof(bHandCards));
    }
};

//�����·������ݸ��ͻ��˵����ݰ�
struct TCards
{
    emTypeCard		nTypeCard;					//��������
    BYTE   			byUser;						//���Ʒ��������
    BYTE			byCards[MAX_HANDCARDS_NUM];	//������
    int				iCardsNum;					//����������Ƶ�����

    TAG_INIT(TCards)
    {
        nTypeCard	= UNKNOWN;					//��������
        byUser		= 255;						//���Ʒ��������
        memset(byCards,0,sizeof(byCards));		//������
        iCardsNum = 0;							//��������Ƶ�����
    }
};

//���ư������������û����л������Ƶȵ�
struct TToken
{
    BYTE	  byUser;									// �������
    BYTE	  byVerbFlag;								// ��������־
    int		  iSelPoints[5];							// ��עֵѡ���
	emToken	  nemToken;									// �й�״̬����
    bool	  bNewTurn;                                 // �Ƿ�Ϊ�µ�һ�ֿ�ʼ
	bool	  bNewTurns;                                // �Ƿ�Ϊ�µ�һ�ֿ�ʼ
	int		  nTurnNums;								// ��ǰ��Ϸ���Ȧ��
	__int64   nCallMoney;                               // ��ǰ���Ը�ע�Ľ��

    TAG_INIT(TToken)
    {
		nemToken = TO_UNKNOWN;						// δ֪����
        byUser = 255;								// �������
        byVerbFlag = 0;								// ��������־
        bNewTurn = false;
		bNewTurns = false;
		nCallMoney = 0;
		nTurnNums = 0;
        memset(iSelPoints,0,sizeof(iSelPoints));	// ��עֵѡ���
    }
};

// �����ע�߳ظ��°�
struct TBetPool
{
    __int64 iBetPools[8];			//��ע�߳�

    TAG_INIT(TBetPool)
    {
        memset(iBetPools, 0, sizeof(iBetPools));  //�߳س�ʼ��
    }
};


//����ṹ��
struct TResult
{
	int  nbyUserID[PLAY_COUNT];				//Ӯ��ID
	char szName[8][61];						//Ӯ���ǳ�
    __int64  nBetPools[8];					//������ע��
    __int64  nUserBet[PLAY_COUNT][8];		//ÿ�������ע����
	bool bWinBetPools[PLAY_COUNT][8];		//Ӯ����ע��
	__int64  nWinPoolsMoney[PLAY_COUNT][8];	//Ӯ����ע�ؽ��
    __int64  nScore[PLAY_COUNT];     		//��Ӯ����
    __int64  nMoney[PLAY_COUNT];     		//ʵ����Ӯ���
	__int64  nMoneyEx[PLAY_COUNT];          //��Ӯ��� 
    __int64  nSubMoney[PLAY_COUNT];  		//��ǰÿ����Ҵ�����
    __int64  nSelfMoney[PLAY_COUNT]; 		//�Լ��Ľ�Ҹ���: �ͻ��˻�ȡ�Ľ���п��ܻ�û��ˢ��, ���������﷢��ȥ
	bool bWin[PLAY_COUNT];       			//��¼Ӯ��
	bool bExit;                  			//true:�����Ҫ�˳�, false:����Ҫ�˳�
	BYTE bOrder[PLAY_COUNT];                //�������, �����������

    TResult()
    {
        ::memset(this, 0, sizeof(TResult));
		::memset(nUserBet, 0, sizeof(nUserBet));
		::memset(nMoneyEx, 0, sizeof(nMoneyEx));
		::memset(bOrder, 255, sizeof(bOrder)); 
    }
};

//�µ�һ�غ������
struct TNextRoundReq
{
    enum emType
    {
        UNKNOWN		= 0,	//δ֪����
        READY		= 1,	//׼����
        EXIT		= 2,    //���˳���Ϸ
    };

    emType nType;			//���������

    TAG_INIT(TNextRoundReq)
    {
        nType = UNKNOWN;	//δ֪����
    }
};

//�µ�һ�غ���Ϣ��
struct TNextRoundBeginInfo
{
    BYTE byNTUser;		//ׯ��λ��
    BYTE bSmallBlind;	//Сäע
    BYTE bBigBlind;		//��äע

    TAG_INIT(TNextRoundBeginInfo)
    {
        ::memset(this,0,sizeof(TNextRoundBeginInfo));
        byNTUser = 255;
        bSmallBlind = 255;
        bBigBlind = 255;
    }
};

//û�����������֪ͨ
struct TNoPlayer
{
    bool bGameFinished;	//������ɢ��

    TAG_INIT(TNoPlayer)
    {
        bGameFinished = false;	//������ɢ��
    }
};

// ��ҽ���
struct TMoneyLack
{
	int nbyUser;			// �������

	TAG_INIT(TMoneyLack)
	{
		nbyUser = 255;
	}
};

//******************************************************************************************
// ��Ϸ״̬����
//******************************************************************************************

#define GS_WAIT_SETGAME					0	//�ȴ���������״̬
#define GS_WAIT_ARGEE					1	//�ȴ�ͬ������
#define GS_CONFIG_NOTE					20	//���õ�ע״̬
#define GS_AGREE_NOTE					21	//ͬ���ע״̬
#define GS_PLAY_GAME					25	//��Ϸ����״̬
#define GS_WAIT_NEXT_ROUND				28	//�ȴ���һ�غϽ���״̬

/******************************************************************************************
*									�¿���˹�˿�״̬����                                  *
*******************************************************************************************/

#define GS_USER_STAKE					30	// �����ע��״̬

/******************************************************************************************
*																						  *
*******************************************************************************************/
//��Ϸ״̬���Ļ���
struct TGSBase 
{
    /*-GSBase��������-*/
    int  iEvPassTime;			//��ǰ�¼������ĵ�ʱ�䣨�룩
    int	 iVerbTime;				//��Ϸ�и�������˼��ʱ��
	bool bBoy[PLAY_COUNT];  	//����Ա�
	bool bHaveUser[PLAY_COUNT]; //���ϸ�λ����û�����
	BYTE bGameStation;			//��Ϸ״̬

    /*-�����������-*/
    TDeskCfg tagDeskCfg;

    TAG_INIT(TGSBase)
    {
        ::memset(this,0,sizeof(TGSBase));
    }

};

//��Ϸ״̬�����ȴ�ͬ��/δ��ʼʱ
struct TGSWaitAgree : public TGSBase 
{
	bool	  bReady[PLAY_COUNT];               //�Ƿ�׼��
    __int64   nSubMoney[PLAY_COUNT];            //ÿ����Ҵ���Ľ��

    TAG_INIT(TGSWaitAgree)
    {
        ::memset(this,0,sizeof(TGSBase));
    }
};

//��Ϸ״̬������Ϸ��
struct TGSPlaying : public TGSBase 
{
    BYTE  byHandCard[2];	                //������ϵ��˿�
    int   nHandCardNums;		            //��������˿���Ŀ

    BYTE  byCards[5];                       //�����Ϲ�����
    int   nCardsCount;                      //���湫������
	int   nID[5];							//�����ť
    __int64   nBetMoney[PLAY_COUNT][8];     //��ע���
    bool  bNoBet[PLAY_COUNT];               //��¼������ע�����λ��
    bool  bGiveUp[PLAY_COUNT];              //��¼���������λ��

    BYTE  byTokenUser;		                //�õ����Ƶ��û�
    BYTE  byNTUser;					        //ׯ��
    BYTE  bySmallBlind;                     //Сä��
    BYTE  byBigBlind;                       //������
	__int64	  nCallMoney;					//��ע���

    BYTE  byVerbFlag;                       //��������
    bool  bNewTurn;                         //�µ�һ�ֲ���
	bool  bNewTurns;                        //�µ�һ�ֲ���

    __int64   nBetPools[8];			        //��ע�߳ؽ��
    __int64   nSubMoney[PLAY_COUNT];        //ÿ����Ҵ���Ľ��

	int   iUserStation ;
 
    TAG_INIT(TGSPlaying)
    {
        ::memset(this,0,sizeof(TGSPlaying));
    }
};

//��Ϸ״̬�����ȴ���һ�غ�
struct TGSWaitNextRound : public TGSBase 
{
    __int64   nSubMoney[PLAY_COUNT];            //ÿ����Ҵ���Ľ��

    TAG_INIT(TGSWaitNextRound)
    {
        ::memset(this,0,sizeof(TGSPlaying));
    }
};
struct UserLeftDeskStruct
{
	BYTE    bDeskStation ; 
	bool    bClearLogo ;
};

//����
struct UserLookCardStruct
{
	BYTE    bDeskStation; 

	UserLookCardStruct()
	{
		bDeskStation = 255;
	}
};

//չʾ��
struct ShowCardStruct
{
	BYTE	byDeskStation;
	BYTE    byCardList[2]; 
	int		iCardCount;

	ShowCardStruct()
	{
		::memset(this,0,sizeof(ShowCardStruct));
		byDeskStation = 255;
	}
	
};
#endif

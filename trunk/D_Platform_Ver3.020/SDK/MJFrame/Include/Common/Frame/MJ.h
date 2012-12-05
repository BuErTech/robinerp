/** @file	MJ.h
 *	@brief	�洢�Ϳ����齫���ݵ���

 *	����4����
 *	�����ࣺCMJHand
 *	�����ࣺCMJGive
 *	��ǽ�ࣺCMJWall
 *	3�ߵĺ������ݶ������ڸ��Ե�m_ucTile�����У������Ȳ�ͬ
 *	CMJHand ���� 18
 *	CMJGive ���� 36
 *	CMJWall ���� 144
 *	�����ࣺCMJSet
 *	�������ݱ����ڽṹTILE_SET�У�����4
 */

#ifndef MJ_H_
#define MJ_H_


#define PLAYER_NBR	4

#include <string.h>
#include <cassert>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "mjdef.h"

namespace MyGame
{
	/**
	* @ingroup MJFrame
	* @brief CMJHand
	*
	* <p>�� 
	*		CMJHand, �������ݴ洢��
	* </p>
	* @sa
	*/	
    class CMJSet;
	class /*SERAPI*/ CMJHand
	{
	public:
		CMJHand();
		virtual ~CMJHand();

		/**
		*  @brief ����ָ��λ���ϵ�����
		*  @param[in]  nPos ��Ҫ���õ�λ��
		*  @param[in]  ucTile ���õ�������
		*  @warning  ������õ�λ�ó�����ǰ���Ƶ�ʵ�ʳ��ȣ��������õ������ݳ������ֵTILE_END��������ʧ��
		*/	
		void SetTile( int nPos, TILE ucTile );

		/**
		*  @brief ��ȡָ��λ���ϵ���
		*  @param[in]  nPos ��Ҫ��ȡ��λ��
		*  @return  �����ȡ��λ�ó�����ǰ���Ƶ�ʵ�ʳ��ȣ��ͻ᷵��TILE_BEGIN
		*/	
		TILE GetTile( int nPos );

		/**
		*  @brief ���һ������
		*  @param[in]  ucTile ��ӵ�������
		*  @param[in]  nNbr ��ӵ�����
		*  @param[in]  bIncrease �Ƿ�Ҫ��������
		*  @return  ������õ�λ�ó������Ƶ�ʵ����󳤶ȣ��������õ������ݳ������ֵTILE_END�������ʧ��
		*/	
		BOOL AddTile( TILE ucTile, int nNbr = 1, int bIncrease = FALSE);

		/**
		*  @brief ɾ��һ������
		*  @param[in]  ucTile ָ��Ҫɾ������
		*  @param[in]  nNbr ɾ��������
		*  @param[in]  bIncrease �Ƿ�Ҫ����ɾ��
		*  @return  ���û�������ƾͻ᷵��FALSE
		*	@warning ɾ������������Ƶ�˳��
		*/	
		BOOL DelTile( TILE ucTile, int nNbr = 1, int bIncrease = FALSE );

		/**
		*  @brief ���һ������
		*  @param[in]  tiles ��������
		*  @return  ��������;���ȳ�����󳤶�,����FALSE
		*/	
		BOOL AddTiles( CMJHand &tiles );

		/**
		*  @brief ɾ��һ������
		*  @param[in]  tiles ��������
		*  @return  ����Ҳ��������е���,����FALSE
		*/	
		BOOL DelTiles( CMJHand &tiles );

		/**
		*  @brief �Ƿ�ָ�������ƶ������
		*  @param[in]  tiles ���ƶ���
		*  @return  �����������ȫ�������ڶ���tiles��,����TRUE,���򷵻�FALSE
		*/	
		BOOL IsSubSet( CMJHand &tiles );

		/**
		*  @brief �Ƿ����ָ��������
		*  @param[in]  ucTile    ָ������
		*  @param[in]  nNbr      ����������
		*  @param[in]  bIncrease �Ƿ���������߿ɲ�������Ϊ9��������һ��˳�ӣ����Դ������nNbr = 3, bIncrease = TRUE
		*  @return �������ָ����������,����TRUE,���򷵻�FALSE
		*/	
		BOOL IsHave( TILE ucTile, int nNbr = 1, int bIncrease = FALSE );

		/**
		*  @brief �Ƿ��л���
		*  @return ���Ƶ�����
		*/	
		int IsHaveFlower();

		/**
		*  @brief ����
		*  @param[in]  nMod 0:����, 1:����, 2:����������������, 3:����������������
		*  @param[in]  nGod 0:�����Ʋ�������������,1:�����Ʒ������е�ͷ��,2:�����Ʒ������е�β��
		*  @param[in]  tilesGod ��������
		*/	
		void Sort( int nMod = 0, int nGod = 0, CMJHand* tilesGod = NULL );

		/**
		*  @brief �������
		*/	
		void ReleaseAll( void );

		/**
		*  @brief �Ƿ�ɳ���
		*  @param[in]  ucTile ָ���Ե���
		*  @param[in]  bWind  �����Ƿ�ɳ�
        *  @param[in]  bArrow �����Ƿ�ɳ�
        *  @param[in/out] mjSet ���ؿ��ԳԵ�������
		*  @return  100:ͷ, 10:��, 1:β
		*
		*  ���ص���10��������,���ͬʱ������ͷ��β��,�ͻ᷵��101.
		*/	
		int IsCanCollect( TILE ucTile, int bWind, int bArrow, CMJSet & mjSet);

		/**
		*  @brief ����
		*  @param[in]  nPos1 ��һ��λ��
		*  @param[in]  nPos2 �ڶ���λ��
		*  @return  �����ɹ�����TRUE,���򷵻�FALSE
		*
		*  ����������ʵ�ʵĳ����н���,����ʧ��
		*/	
		BOOL Swap( int nPos1, int nPos2 );

		/**
		*  @brief ���õ�ǰ����
		*  @param[in]  nCurLength ����
		*/	
		void CurrentLength( const int& nCurLength );

		/**
		*  @brief ��ȡ����
		*  @return  ��ǰ���Ƶ�ʵ�ʳ���
		*/	
		int CurrentLength( void ) const { return m_nCurrentLength; }

		/**
		*  @brief ��ȡָ���Ƶ�����
		*  @param[in]  ucTile ������
		*  @return  ��:0��Ͳ:1����:2����:3����:4������:5
		*
		*  ����ֵ�к궨��ɲο�
		*/	
		int GetTileGenre( TILE ucTile );

		/**
		*  @brief ��ȡָ���Ƶ��Ƶ�
		*  @param[in]  ucTile ������
		*  @return  ȥ�����͵���ֵ
		*
		*  ����ֵ�к궨��ɲο�
		*/	
		int GetTileNumber( TILE ucTile );

        /**
         *  @brief ��ȡָ���Ƶ�ӵ�е�����
         *  @param[in]  ucTile ������
         *  @return  ���Ƶ�����
         */	
         int GetTileCount( TILE ucTile );

		/**
		*  @brief �������Ƶ����һ��
		*  @return  ���û���˷���TILE_BEGIN
		*/	
		TILE PopTile();

		/**
		*  @brief ���һ������
		*  @param[in]  ucTile ���ӵ�����
		*
		*  �൱��ִ����3�����ͬһ����
		*/	
		void AddTriplet( TILE ucTile );

		/**
		*  @brief ���һ��˳��
		*  @param[in]  ucTile ˳�ӵ�һ��������
		*  @warning ���Խ����Ȼ����ӽ�ȥ,�������9��,��ʵ�����9,10,11(һ��)��3������
		*/	
		void AddCollect( TILE ucTile );

		/**
		*  @brief ��TILE��������װ��ΪBYTE
		*  @param[in]  ucTile ������
		*  @return  BYTE���͵�����
		*/	
		BYTE GetTileToBYTE( TILE ucTile ) { return (BYTE)ucTile; }

		/**
		*  @brief ��ȡ����������
		*  @param[in] nHandTiles ������
		*  @return  �Ƴ���
		*/	
		int GetAllTile( int nHandTiles[] );

        /**
        *  @brief ����CMJHand������
        *  @param[in] mjHands ����������
        *	@return �����������
        *  @sa
        */		
        //const CMJHand operator = (const CMJHand& mjHands);
        CMJHand operator + (const CMJHand& mjHands);
        CMJHand operator - (const CMJHand& mjHands);
        CMJHand operator += (const CMJHand& mjHands);
        CMJHand operator -= (const CMJHand& mjHands);
	private:
		// ʵ������
		TILE m_ucTile[MAX_TOTAL_TILES];

		// ��ǰ�Ƴ�
		int  m_nCurrentLength;
	};

	/**
	* @ingroup MJFrame
	* @brief TILE_SET
	*
	* <p>�� 
	*		TILE_SET, ���Ƶ����ݽṹ
	* </p>
	*
	*/	
	struct TILE_SET
	{
		TILE m_ucTile[4]; // �����ܵ�������
		BYTE m_ucChair;   // ������˭��
		BYTE m_ucFlag;    // ��ǣ���¼�����ܵ����ã�

        TILE_SET()
        {
            memset(this, 0, sizeof(TILE_SET));
        }
	};

	/**
	* @ingroup MJFrame
	* @brief CMJSet
	*
	* <p>�� 
	*		CMJSet, ���Ƶ����ݴ洢�Ͳ���
	* </p>
	*
	*/	
	class /*SERAPI*/ CMJSet
	{
	public:
		CMJSet();
		virtual ~CMJSet();

		/**
		*  @brief ���һ������
		*  @param[in]  ucTile  �����ܵ�������
		*  @param[in]  ucFlag  ���Ʊ��(��,��,��)
		*  @param[in]  ucChair ����Ŀ�����(����ǳ��ƾͱ�ǵ��ǳԵ���һ��)
		*  @return �ɹ�TRUE,ʧ��FALSE
		*/	
		BOOL AddSet( TILE ucTile[4], BYTE ucFlag, BYTE ucChair );

		/**
		*  @brief ���һ������
		*  @param[in]  tagSet ��������
		*  @return �ɹ�TRUE,ʧ��FALSE
		*/	
		BOOL AddSet( TILE_SET tagSet );

		/**
		*  @brief ���һ������
		*  @param[in]  mjSet Ҫ��ӵ��������ݽṹ
		*  @return �ɹ�TRUE,ʧ��FALSE
		*/	
		BOOL AddSet( CMJSet& mjSet );

		/**
		*  @brief ��ȡһ������
		*  @param[in]  nPos Ҫ��ȡ��λ��
		*  @param[out]  tileSet ��ȡ�����ݽṹ
		*  @return �ɹ�TRUE,ʧ��FALSE
		*/	
		BOOL GetSet( int nPos, TILE_SET &tileSet );

		/**
		*  @brief �趨һ������
		*  @param[in]  nPos Ҫ�趨��λ��
		*  @param[in]  tileSet �趨�����ݽṹ
		*  @return �ɹ�TRUE,ʧ��FALSE
		* 
		*  λ�ò��ܳ���MAX_SET_TILES, �����趨ʧ��
		*/	
		void SetSet( int nPos, TILE_SET &tileSet );

		/**
		*  @brief �����������е���������������
		*  @param[in]  ucTile �������ܵ���
		*  @return ��������д�������Ƶ����ͳɹ�TRUE,����ʧ��FALSE
		*/	
		BOOL TripletToQuadruplet( TILE ucTile );

		/**
		*  @brief �������
		*/	
		void ReleaseAll( void );

		/**
		*  @brief ���õ�ǰ����
		*  @param[in]  nCurLength ����
		*/	
		void CurrentLength( const int& nCurLength );

		/**
		*  @brief ��ȡ����
		*  @return  ��ǰ���Ƶ�ʵ�ʳ���
		*/	
		int CurrentLength( void ) const { return m_nCurrentLength; }

		/**
		*  @brief ���������еĸ�����Ϣ
		*  @param[in]  nPos Ҫ�趨��λ��
		*  @param[in]  pParam Ҫ���õ����Ƹ�����Ϣ
		*  @return �ɹ�TRUE, ʧ��FALSE
		*/	
		BOOL SetParam( int nPos, void * pParam );

		/**
		*  @brief ��ȡ���������õĸ�����Ϣ
		*  @param[in]  nPos Ҫ��ȡ��λ��
		*  @return ʧ�ܷ���NULL
		*/	
		void * GetParam( int nPos);
	private:
		// ����
		TILE_SET m_TileSet[MAX_SET_TILES];

		// ��չ����(�ṩ���ط��齫�洢���õ���Ϣ, ������Ա�Լ�ת������), ��: ���ӱ��齫�ĸ��ϸܡ��ܺ�ܡ���������(�ܺ���׷����������ٸ�)
		void * m_pParam[MAX_SET_TILES];

		// ����
		int m_nCurrentLength;
	};

	/**
	* @ingroup MJFrame
	* @brief CMJGive
	*
	* <p>�� 
	*		CMJGive, ���Ƶ����ݴ洢�Ͳ���
	* </p>
	*
	* ���Ƶ����ݺ����Ƴ��˳���,��������һ��
	*/	
	class /*SERAPI*/ CMJGive
	{
	public:
		CMJGive();
		~CMJGive();

		/**
		*  @brief ���ó�������
		*  @sa CMJGive
		*/	
		void SetTile( int nPos, TILE ucTile );

		/**
		*  @brief ��ȡ��������
		*  @sa CMJGive
		*/	
		TILE GetTile( int nPos );

        /** 
        *  @brief ����Xλ�õ�������Ϣ
        *  @sa CMJGive
        */
        void SetBlock(int nPos, TILE_SET &tileSet);

        /** 
        *  @brief ��ȡXλ�õ�������Ϣ
        *  @sa CMJGive
        */
        void GetBlock(int nPos, TILE_SET &tileSet);

		/**
		*  @brief ���һ����
		*  @param[in]  ucTile ��ӵ�������
		*  @param[in]  nNbr ��ӵ�����
		*  @param[in]  bIncrease �Ƿ�Ҫ��������
		*  @return  ������õ�λ�ó����Ƶ�ʵ����󳤶ȣ��������õ������ݳ������ֵTILE_END�������ʧ��
		*/	
		BOOL AddTile( TILE ucTile, int nNbr = 1, int bIncrease = FALSE);

		/**
		*  @brief ɾ��һ������
		*  @param[in]  ucTile ָ��Ҫɾ������
		*  @param[in]  nNbr ɾ��������
		*  @param[in]  bIncrease �Ƿ�Ҫ����ɾ��
		*  @return  ���û�������ƾͻ᷵��FALSE
		*  @warning ɾ������������Ƶ�˳��
		*/	
		BOOL DelTile( TILE ucTile, int nNbr = 1, int bIncrease = FALSE );

		/**
		*  @brief ���һ�����
		*  @sa CMJGive
		*/	
		BOOL AddTiles( CMJGive &tiles );

		/**
		*  @brief ɾ��һ���������
		*  @sa CMJGive
		*/	
		BOOL DelTiles( CMJGive &tiles );

		/**
		*  @brief �Ƿ����ָ������
		*  @param[in]  ucTile    ָ������
		*  @param[in]  nNbr      ����������
		*  @param[in]  bIncrease �Ƿ���������߿ɲ�������Ϊ9��������һ��˳�ӣ����Դ������nNbr = 3, bIncrease = TRUE
		*  @return �������ָ����������,����TRUE,���򷵻�FALSE
		*/	
		BOOL IsHave( TILE ucTile, int nNbr = 1, int bIncrease = FALSE );

		/**
		*  @brief �������
		*  @sa CMJGive
		*/	
		void ReleaseAll( void );

		/**
		*  @brief ���ó��Ƶĳ���
		*  @sa CMJGive
		*/	
		void CurrentLength( const int& nCurLength );

		/**
		*  @brief ��ȡ�������ݵĳ���
		*  @sa CMJGive
		*/	
		int CurrentLength( void ) const { return m_nCurrentLength; }

		/**
		*  @brief ����ָ��2��λ���ϵ�������
		*  @sa CMJGive
		*/	
		BOOL Swap( int nPos1, int nPos2 );

		/**
		*  @brief ��ȡ����������
		*  @param[in] nGiveTiles ������
		*  @return  �Ƴ���
		*/	
		int GetAllTile( int nGiveTiles[] );

	private:
		// ʵ������
		TILE m_ucTile[MAX_GIVE_TILES];

        // ��������, ���3D����
        TILE_SET m_TileSet[MAX_GIVE_TILES];

		// ��ǰ�Ƴ�
		int  m_nCurrentLength;
	};

	/**
	* @ingroup MJFrame
	* @brief CMJWall
	*
	* <p>�� 
	*		CMJWall, ��ǽ�����ݴ洢�Ͳ���
	* </p>
	*
	* ��ǽ�ĳ��ȴ�144����,�����Լ���ϴ�ƹ���,������������������һ��
	*/	
	class /*SERAPI*/ CMJWall
	{
	public:
		CMJWall();
		virtual ~CMJWall();

		/**
		*  @brief �������
		*  @sa CMJWall
		*/	
		void ReleaseAll();

		/**
		*  @brief �������
		*  @param[in]  nLength Ҫ���ĳ���
		*  @param[in]  ucClear �Ƿ�Ҫȫ������ָ������,Ϊ0��ʾ�Զ���˳�����
		*
		*  ���ucClear��Ĭ��Ϊ0�ͻὫָ�����ȵ��ư���,��,Ͳ,��,����˳�����144��,����ǰ���Ȼ�����Ϊָ����nLength
		*/	
		void FillTiles( int nLength = 0, BYTE ucClear = 0 );

		/**
		*  @brief ϴ��
		*
		*  ���ҵ�ǰ���ȵ��Ƶ�˳��
		*/	
		void RandomTiles();

		/**
		*  @brief ����ָ��2��λ�õ���
		*  @sa CMJWall
		*/	
		BOOL Swap( int nPos1, int nPos2 );

		/**
		*  @brief ���һ����
		*  @param[in]  ucTile ��ӵ�������
		*  @param[in]  nNbr ��ӵ�����
		*  @param[in]  bIncrease �Ƿ�Ҫ��������
		*  @return  ������õ�λ�ó����Ƶ�ʵ����󳤶ȣ��������õ������ݳ������ֵTILE_END�������ʧ��
		*/	
		BOOL AddTile( TILE ucTile, int nNbr = 1, int bIncrease = FALSE);

		/**
		*  @brief ɾ��һ������
		*  @param[in]  ucTile ָ��Ҫɾ������
		*  @param[in]  nNbr ɾ��������
		*  @param[in]  bIncrease �Ƿ�Ҫ����ɾ��
		*  @return  ���û�������ƾͻ᷵��FALSE
		*  @warning ɾ������������Ƶ�˳��
		*/	
		BOOL DelTile( TILE ucTile, int nNbr = 1, int bIncrease = FALSE );

		/**
		*  @brief ���һ����
		*  @sa CMJWall
		*/	
		BOOL AddTiles( CMJWall &tiles );

		/**
		*  @brief ɾ��һ����
		*  @sa CMJWall
		*/	
		BOOL DelTiles( CMJWall &tiles );

		/**
		*  @brief �趨ָ��λ���ϵ���
		*  @sa CMJWall
		*/	
		void SetTile( int nPos, TILE ucTile );

		/**
		*  @brief ��ȡָ��λ���ϵ���
		*  @sa CMJWall
		*/	
		TILE GetTile( int nPos );

		/**
		*  @brief �Ƿ����ָ������
		*  @param[in]  ucTile    ָ������
		*  @param[in]  nNbr      ����������
		*  @param[in]  bIncrease �Ƿ���������߿ɲ�������Ϊ9��������һ��˳�ӣ����Դ������nNbr = 3, bIncrease = TRUE
		*  @return �������ָ����������,����TRUE,���򷵻�FALSE
		*/	
		BOOL IsHave( TILE ucTile, int nNbr = 1, int bIncrease = FALSE );

		/**
		*  @brief �������һ����
		*  @sa CMJWall
		*/	
		TILE PopTile();

        /**
        *  @brief ������ǰһ����
		*  @param[in] nPos ���Ƶ�λ��
        *  @sa CMJWall
        */	
        TILE PopFrontTile(int nPos = -1);

		/**
		*  @brief ���ó�ǽ����
		*  @sa CMJWall
		*/	
		void CurrentLength( const int &nCurLength );

		/**
		*  @brief ��ȡ��ǰ��ǽ����
		*  @sa CMJWall
		*/	
		//int CurrentLength( void ) const { return m_nCurrentLength; }
		int CurrentLength();

		/**
		*  @brief ����ץ���벹�Ƶ�λ��
		*  @param[in] nStartPos ץ��λ��
		*  @param[in] nEndPos   ����λ��
		*  @sa CMJWall
		*/	
		void SetPos(int nStartPos, int nEndPos);

		/**
		*  @brief ����ץ��λ��
		*  @sa CMJWall
		*/	
		void SetStartPos(int nPos);

		/**
		*  @brief ���ò���λ��
		*  @sa CMJWall
		*/	
		void SetEndPos(int nPos);

		/**
		*  @brief ��ȡץ��λ��
		*  @sa CMJWall
		*/	
		int GetStartPos();

		/**
		*  @brief ���ò���λ��
		*  @param[in] nOffset ��ȡ���Ƶ�ƫ��λ��
		*  @sa CMJWall
		*/	
		int GetEndPos(int nOffset = 0);
	private:
		// ����
		TILE m_ucTile[MAX_TOTAL_TILES];

		// ����
		int m_nCurrentLength;

		// ��ǰץ��λ��
		int m_nStartPos;

		// ����λ��(����λ��)
		int m_nEndPos;
	};
}

#endif

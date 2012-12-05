/** @file	MJRule.h
 *	@brief	�齫�Ĺ�����
 *	@author duanxiaohui
 *	@date 10.07.15
 *
 *  �㷬�õĺ����������˴�������з��ͣ�������ͨ��������϶��γ��µķ���
 */
#ifndef AFX_MJRULE_H__F30D5373_AF06_4AAD_BA58_89EB2AA5C183__INCLUDED_
#define AFX_MJRULE_H__F30D5373_AF06_4AAD_BA58_89EB2AA5C183__INCLUDED_

#include "MJ.h"
#include "MJFanCounter.h"

namespace MyGame
{
	/**
	* @ingroup MJFrame
	* @brief CMJRule
	*
	* <p>�� 
	*		CMJRule, �齫�Ĺ�����
	* </p>
	* @sa
	*/	
	class /*SERAPI*/ CMJRule  
	{
	public:
		CMJRule();
		virtual ~CMJRule();

		/**
		*  @brief �ж��ܲ���������Ƶ�����
		*  @param[in]  mjTilesNoGod ȥ���������Ƶ�����
		*  @param[in]  mjSet ��ҵ�����
		*  @param[in]  nLaiziCount ����������
		*  @return  ������Ժ�����TRUE,���򷵻�FALSE
		*
		*  �����Ƿ�
		*/	
		virtual BOOL CheckWin( CMJHand mjTilesNoGod, CMJSet mjSet, int nLaiziCount = 0 );

		/**
		*  @brief �Ƿ��ܺ�
		*  @param[in]  mjTiles ��ҵ�����
		*  @param[in]  mjSet ��ҵ�����
		*  @param[in]  nLaiziAdd ���ӵ�����������
		*  @param[in]  bCheckGod �Ƿ���������
		*  @return  ������Ժ�����TRUE,���򷵻�FALSE
		*
		*  ���Ƿ�������,���жϺ��Ƶ����ĺ���
		*/	
		virtual BOOL IsWin( CMJHand mjTiles, CMJSet mjSet, int nLaiziAdd = 0, int bCheckGod = TRUE );

		/**
		*  @brief �Ƿ�����
		*  @param[in]  mjTiles ��ҵ�����
		*  @param[in]  mjSet ��ҵ�����
		*  @param[out]  mjGive ����������������ƿ��Ա����ȥ���Ǽ���
		*  @return  �������������TRUE,���򷵻�FALSE
		*/	
		virtual BOOL IsTing( CMJHand mjTiles, CMJSet mjSet, CMJHand& mjGive );

		/**
		*  @brief �Ƿ�����,����ȡ����ʲô
		*  @param[in]  mjTiles ��ҵ�����
		*  @param[in]  mjSet ��ҵ�����
		*  @param[out]  mjTing ������������ؿ��Ժ�����
		*  @return  �������������TRUE,���򷵻�FALSE
		*
		*  ��IsTing���������ڣ�IsTing��mjTiles�ǰ�����1�Ų��ܺ����ơ��������mjTiles���Ѿ������˵�����
		*/	
		virtual BOOL GetTingTiles( CMJHand mjTiles, CMJSet mjSet, CMJHand& mjTing );

		/**
		*  @brief �Ƿ��ܺ�ָ����
		*  @param[in]  mjTiles ��ҵ�����
		*  @param[in]  mjSet ��ҵ�����
		*  @param[in]  ucTile ָ������
		*  @return  ������Ժ�����TRUE,���򷵻�FALSE
		*/	
		virtual BOOL IsCanWin( CMJHand mjTiles, CMJSet mjSet, TILE ucTile );

		/**
		*  @brief �Ƿ��ܳ���
		*  @param[in]  mjTiles ��ҵ�����
		*  @param[in]  ucTile ָ��Ҫ�Ե���
		*  @param[in]  nChairGive ���Ƶ����
		*  @param[in]  nChairCheck �жϳ��Ƶ����
		*  @param[in]  nPlayerNbr �ܵ���Ϸ����(�����жϳԵ��ǲ����ϼ�)
		*  @return  ������ԳԷ���TRUE,���򷵻�FALSE
		*/	
		virtual BOOL IsCanCollect( CMJHand& mjTiles, TILE ucTile, int nChairGive, int nChairCheck, int nPlayerNbr = 4 );

		/**
		*  @brief �Ƿ�������
		*  @param[in]  mjTiles ��ҵ�����
		*  @param[in]  ucTile ָ��Ҫ������
		*  @return  �������������TRUE,���򷵻�FALSE
		*/	
		virtual BOOL IsCanTriplet( CMJHand& mjTiles, TILE ucTile );

		/**
		*  @brief �ܷ��ָ������
		*  @param[in]  mjTiles ��ҵ�����
		*  @param[in]  ucTile ָ��Ҫ�ܵ���
		*  @return  ������Ըܷ���TRUE,���򷵻�FALSE
		*/	
		virtual BOOL IsCanQuadruplet( CMJHand& mjTiles, TILE ucTile );

		/**
		*  @brief �����Ƿ����ƿɸ�
		*  @param[in]  mjTiles ��ҵ�����
		*  @param[in]  mjSet ��ҵ�����
		*  @return  ������Ըܷ���TRUE,���򷵻�FALSE
		*/	
		virtual BOOL IsCanQuadrupletSelf( CMJHand& mjTiles, CMJSet &mjSet );

		/**
		*  @brief ��ȡ���ٷ�
		*  @param[in]  mjTiles ��ҵ�����
		*  @param[in]  mjSet ��ҵ�����
		*  @param[in]  env �����ƵĻ�����Ϣ
		*  @return ���ط�����Ŀ
		*/	
		virtual int GetFanCount( CMJHand mjTiles, CMJSet mjSet, ENVIRONMENT env );

		/**
		*  @brief �ܷ���ͨ�ĺ�
		*  @param[in]  mjTiles ��ҵ�����
		*  @param[in]  nLaiziCount �����Ƶ�����
		*  @param[in]  bNoJiang û�н���
		*  @return ���������ͨ����,����TRUE,���򷵻�FALSE
		*/	
		virtual BOOL CheckWinNormal( CMJHand mjTiles, int nLaiziCount, int bNoJiang = 0 );

		/**
		*  @brief �ܷ�7��
		*  @param[in]  mjTiles ��ҵ�����
		*  @param[in]  nLaiziCount �����Ƶ�����
		*  @return ������Ժ���,����TRUE,���򷵻�FALSE
		*/	
		virtual BOOL CheckWinAllPair( CMJHand mjTiles, int nLaiziCount );

		/**
		*  @brief �ܷ��ʮ����
		*  @param[in]  mjTiles ��ҵ�����
		*  @param[in]  nLaiziCount �����Ƶ�����
		*  @return ������Ժ���,����TRUE,���򷵻�FALSE
		*/	
		virtual BOOL CheckWin131( CMJHand mjTiles, int nLaiziCount );

		/**
		*  @brief �ܷ������
		*  @param[in]  mjTiles ��ҵ�����
		*  @param[in]  mjSet   ��ҵ���������
		*  @return ������Ժ���,����TRUE,���򷵻�FALSE
		*/	
		virtual BOOL CheckWinAllGreen( CMJHand mjTiles, CMJSet mjSet );

		/**
		*  @brief �ܷ���ҷ�
		*  @param[in]  mjTiles ��ҵ�����
		*  @param[in]  mjSet   ��ҵ���������
		*  @return ������Ժ���,����TRUE,���򷵻�FALSE
		*/	
		virtual BOOL CheckWinAllWind( CMJHand mjTiles, CMJSet mjSet );

		/**
		*  @brief �ܷ�������
		*  @param[in]  mjTiles ��ҵ�����
		*  @param[in]  nLaiziCount �����Ƶ�����
		*  @return ������Ժ���,����TRUE,���򷵻�FALSE
		*/	
		virtual BOOL CheckWinZuhelong( CMJHand mjTiles, int nLaiziCount );

		/**
		*  @brief �ܷ��ȫ����
		*  @param[in]  mjTiles ��ҵ�����
		*  @param[in]  nLaiziCount �����Ƶ�����
		*  @return ������Ժ���,����TRUE,���򷵻�FALSE
		*/	
		virtual BOOL CheckWinQuanbukao( CMJHand mjTiles, int nLaiziCount );

		/**
		*  @brief �����㷬ָ��
		*  @param[in]  pFanCount �������ָ��
		*/	
		virtual void SetFanCounter( CMJFanCounter* pFanCount );

		/**
		*  @brief ����������
		*  @return ����0
		*/	
		virtual int SetGodTiles();

		/**
		*  @brief �����������Ϣ
		*/	
		virtual void ClearGodTiles();

		/**
		*  @brief ������Ҳ���뵽������ȥ
		*  @param[out]  mjAll ���Ϻ����������
		*  @param[in]  mjTiles ��ҵ�����
		*  @param[in]  mjSet ��ҵ�����
		*/	
		virtual void GetAllTiles( CMJHand &mjAll, CMJHand mjTiles, CMJSet mjSet );

		/**
		*  @brief ��ȡ������
		*  @return ���������Ƶ�����
		*/	
		CMJHand GetTilesGod() { return m_TilesGod; }

		/**
		*  @brief ��ȡһ��ȡ���������Ƶ�����
		*  @param[in]  mjTiles ��ҵ�����
		*  @param[out]  mjTilesNoGod ȥ�������ƺ������
		*  @param[out]  mjTilesGod ȥ��������ʣ�µ�����
		*  @return ���������Ƶ�����
		*/	
		int GetTilesNoGod( CMJHand& mjTiles, CMJHand& mjTilesNoGod, CMJHand& mjTilesGod );

		/**
		*  @brief ��ȡ��СҪ��
		*  @return ������СҪ��ķ�
		*/	
		virtual int GetMinFan() { return m_tagMJSetting.nMinFan; }

		/**
		*  @brief �������Ƶ����������ʵ�ʵ���
		*  @param[out]  mjTilesNoGod ȥ�������ƺ������
		*  @param[in]  mjSet ��ҵ�����
		*  @param[in]  nLaiziCount �����Ƶ�����
		*  @param[out]  mjLaizi �滻�����
		*  @param[in]  mjLaiziTiles �����Ʊ������
		*  @return �������ļ��㷬��
		*/	
		virtual int ConfigGodTiles( CMJHand &mjTilesNoGod, CMJSet mjSet, int nLaiziCount, CMJHand &mjLaizi, CMJHand mjLaiziTiles );

		/**
		*  @brief �������Ƶ����������ʵ�ʵ���
		*  @param[out]  mjTilesNoGod ȥ�������ƺ������
		*  @param[in]  mjSet ��ҵ�����
		*  @param[in]  nLaiziCount �����Ƶ�����
		*  @param[out]  mjLaizi �滻�����
		*  @param[in]  mjLaiziTiles �����Ʊ������
		*  @return ����һ������������ֵ
		*
		*  ����������ȥ�����ҳ���󷬣�ֻҪ���������ķ��Ϳ�����
		*/	
		virtual int ConfigGodTilesFast( CMJHand &mjTilesNoGod, CMJSet mjSet, int nLaiziCount, CMJHand &mjLaizi, CMJHand mjLaiziTiles );

		/**
		*  @brief �������Ƶ����������ʵ�ʵ���
		*  @param[in]  mjTilesNoGod ȥ�������ƺ������
		*  @param[out]  mjTileoGod ȥ�������������飬�������Ǳ��滻�ص���
		*  @param[in]  mjSet ��ҵ�����
		*  @param[in]  nLaiziCount �����Ƶ�����
		*  @param[in]  bNeedFanCount �Ƿ���Ҫ���Ʒ���,�������Ҫ,ֱ�ӷ���1
		*  @return  �����������,����0,���򷵻������˵ķ���ֵ
		*/	
		virtual int CheckNeedFan( CMJHand mjTilesNoGod, CMJHand& mjTilesGod, CMJSet mjSet, int nLaiziCount = 0, int bNeedFanCount = FALSE );

		/**
		*  @brief ���������ļ�
		*  @param[in]  mjdata ���õ�����
		*/	
		virtual void SetMJSetting( MJ_SETTING_DATA mjdata );

		/**
		*  @brief ��ȡ�����ļ�
		*  @return �����ļ���Ϣ
		*/	
		virtual MJ_SETTING_DATA GetMJSetting() { return m_tagMJSetting; }

		/**
		*  @brief �����㷬��������
		*  @param[in]  env ���������ṹ
		*  @param[in]  nChair Ҫ���㷬����
		*  @param[in]  bSetChairTile Ҫ���÷����˵���
		*/	
		virtual void SetFanEvn( ENVIRONMENT &env, int nChair, int bSetChairTile = TRUE );

		/**
		*  @brief ��ȡ���������������
		*  @return �����������
		*/	
		virtual CMJHand GetTilesGodTrans() { return m_TilesGodTrans; }

		/**
		*  @brief ��ȡ���Ƶ�����
		*  @return ���ؽ������ 100 �γ�ͷ 10 ���м� 1�Ժ��棬0ʲô�����ܳ�
		*/	
		virtual int GetCollectRes( CMJHand mjHand, TILE ucTile, int nChair, int nGiveChair, int bWind = FALSE );

		/**
		*  @brief ��ȡָ���Ƶ�����
		*  @param[in]  ucTile ������
		*  @return  ��:0��Ͳ:1����:2����:3����:4������:5
		*
		*  ����ֵ�к궨��ɲο�
		*/	
		virtual int GetTileGenre( TILE ucTile );

	public:
		// �㷬ָ��
		CMJFanCounter* m_pFanCount;

	protected:
		// ���������У�һ��ֻ��һ��
		// ���жϺ��Ƶ�ʱ�������ƿ��Գ䵱�κ�����
		CMJHand m_TilesGod;
		// ���������滻���ƣ���ÿ��CONFIG�󶼻ᱻˢ��
		CMJHand m_TilesGodTrans;
        // ���Ʋ���, ���Բ����ĳ���������
        CMJSet  m_tmpBlockSet;
        // ���Ʋ���, ���ƺ���Դ����������
        CMJHand m_tmpTingGive;


		/************************************************************************/
		/* 
		��Ϸ��������Ϣ
		*/
		/************************************************************************/
		MJ_SETTING_DATA m_tagMJSetting;

	};

}

#endif // !defined(AFX_MJRULE_H__F30D5373_AF06_4AAD_BA58_89EB2AA5C183__INCLUDED_)

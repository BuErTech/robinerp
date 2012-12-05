///// �����ṩ��Ч��ʵ��ģ��Ľӿڶ���
#pragma once
#ifndef  _IUSERINTERFACE_H
#define  _IUSERINTERFACE_H
#include <iostream>

#ifdef UI_EXPORTS
#define UI_API __declspec(dllexport)
#else
#define UI_API __declspec(dllimport)
#endif UI_EXPORTS
#include <Windows.h>
enum BLONGED{NONE=1,CONTAIN=2};          ///<�ؼ��������࣬none��ʾ�������κοؼ���contain��ʾ���ڰ������ؼ�
enum FONTSHOWSTYLE{AlignmentLEFT,AlignmentCenter,AlignmentFar, AlignmentVerticalCenter};    ///<�������ģʽ
enum FONTSHOWDIRECTION {FormatFlagsLevel,FormatFlagsVertical};
enum ControlKind{C_Contain,C_Button,C_Image,C_Card,C_Cartoon,C_Text,C_EditorText,C_ComBox,C_Progress,C_ScrollBar,
	C_ImageFlash,C_Web,C_RadioButton,C_ListControl,C_Beacon,C_Talk,C_Num,C_NoMoveCard,C_LightCard,C_CanOperateCard,
    C_Slider,C_InputUserName,C_Menu,C_Sudoku,C_Time,C_SiTuanOperateCard,C_SiTuanNoMoveCard,C_SiTuanLightCard,C_ExAnimate, 
    C_MjCtrl, C_MjHandCtrl, C_MjGiveCtrl, C_MjWallCtrl, C_MjBlockCtrl,C_MoveAction,C_ShCtrl, C_FlashPlay, C_RSLamp, C_HuziCard,
	C_TopPaper}; ///<�ؼ�����
enum PLAYSTATE{START,PLAYING,PAUSE,CONTINUE,STOP};///<����״̬
enum CARD_SHOW_DIR{MYSELF,LEFT,PARTNER,RIGHT};   ///<����ʾ���� 
typedef enum enmCardShape
{
	ECS_SHAPE_SPECIAL =5,	/*!< �ر��ƣ���lym�ã� */
	ECS_SHAPE_NONE = 4,		/*!< ��С�����޻�ɫ */
	ECS_SHAPE_SPADE=3,		/*!< ���� */
	ECS_SHAPE_HEARTS=2,		/*!< ���� */
	ECS_SHAPE_CLUBS=1,		/*!< ÷�� */
	ECS_SHAPE_DIAMONDS=0		/*!< ���� */
}ENM_CARD_SHAPE;

//! �Ƶĵ���
typedef enum enmCardNumber
{
	ECN_NUM_NONE = 0,		/*!< �Ʊ� */				
	ECN_NUM_2,
	ECN_NUM_3,
	ECN_NUM_4,
	ECN_NUM_5,
	ECN_NUM_6,
	ECN_NUM_7,
	ECN_NUM_8,
	ECN_NUM_9,
	ECN_NUM_10,
	ECN_NUM_J,
	ECN_NUM_Q,
	ECN_NUM_K,
	ECN_NUM_A,
	ECN_NUM_Joker,			/*!< С�� */
	ECN_NUM_JOKER			/*!< ���� */
}ENM_CARD_NUMBER;


typedef enum enuCurveType
{
    CT_NONE     = 0,
    CT_BEZIER   = 1,
    CT_STRAIGHT = 2
} CURVE_TYPE;

//����ʽ���ְ�(���ֿؼ���0~9+-,)
typedef struct newNumStruct
{
	bool	bHavePlusSign;			//�Ӻ�
	bool	bHaveMinusSign;			//����
	bool	bHaveComma;				//����

	bool	bShowPlusSignOrMinusSign;		//��ʾ�ӺŻ����
	bool	bShowComma;						//��ʾ����
	BYTE	iShowCommaIndex;				//����λ��������һ���ֺ�
	BYTE	iShowZeroType;					//��ʾ����Ϊ0ʱ��ʾ�ӺŻ��Ǽ��ţ�����ֻ��ʾ0��0ʱ�Ӽ�������ʾ��1ʱ��ʾ�Ӻţ���2��ʾ���ţ�
	newNumStruct()
	{
		bHavePlusSign =true;
		bHaveMinusSign =true;
		bHaveComma =true;

		bShowPlusSignOrMinusSign =false;
		bShowComma =false;
		iShowCommaIndex =3;
		iShowZeroType =0;
	}
}NEW_NUM_STRUCT;

/**
* @brief ���ߺ����ӿ�
*/
class ICurveLine
{
public:
    /**
    * @brief �������������
    * @param[in]       vPoints         ��һ������㣬���һ�����յ㣬����ȫ���м�ؼ���
    * @return  int����������0
    */
    virtual int SetCurvePoints(POINT vPoints[], int nSize) = 0;
    /**
    * @brief �õ������ɼ��������
    */
    virtual int GetPointsCount() = 0;
    /**
    * @brief �õ�ĳһ�̵ĵ�����
    * @param[in]       nFinishedPersent    ��ɵİٷֱ�
    * @param[in]       bSymmetrical        �Ƿ�Ϊ�Գ�����
    * @return  int����������0
    */
    virtual int GetCurrrentPoint(int nFinishedPersent, POINT *pPointOut, bool bSymmetrical = false) = 0;    
    /**
    * @brief �õ���������
    */
    virtual CURVE_TYPE GetCurveType() = 0;
};

/** @brief ��Ҫ�ֶ����õĿؼ�
*/
interface IManual
{
public:
    /** @brief ��ʼ��
    *  �ɿͻ����Լ����ã�����OLE�̲߳���ȫ
    */
    virtual void Init() = 0;
    /** @brief ����
    *  �ɿͻ����Լ����ã�����OLE�̲߳���ȫ
    */
    virtual void Draw() = 0;
    /** @brief ����ʼ��
    *  �ɿͻ����Լ����ã�����OLE�̲߳���ȫ
    */
    virtual void UnInit() = 0;
};


interface IBCInterFace
{
public:
	virtual void SetControlVisible(bool bflag)=0;
	virtual bool    GetVisible()=0;           ///<��ÿؼ��Ƿ���ʾ
	virtual int GetZorder()=0;///<��ȡ�ؼ����
	virtual void SetZorder(INT16 zorder)=0;///<���ÿؼ����
	virtual void SetGameXY(int x,int y)=0;   ///<�����Ҫ����Ϸ�õ�
	
	virtual int GetGameX()=0;
	virtual int GetGameY()=0;
	virtual int GetGameW()=0;
	virtual int GetGameH()=0;
	virtual void SetGameW(int w)=0;
	virtual void SetGameH(int h)=0;
	virtual void SetTooltip(wchar_t * pchar)=0;   ///<���ÿؼ������������ʾ�ַ���
	virtual wchar_t * GetToolTip()=0;             ///<��ȡ�ؼ��������ʾ�ַ���
	virtual void SetIsMove(bool bFlag)=0;
	virtual bool GetIsMove()=0;

	virtual int GetGameControlID()=0; ///<��ȡ�ؼ�DI
	virtual void SetTipDrawAtMouse(bool bDrawAtMouse = true)=0;
	virtual void SetTipPath(wchar_t *p)=0;//�Զ���TIP����ͼƬ
	virtual wchar_t * GetTipPath()=0;

	virtual void SetHaveShrinkArea(bool bFlag)=0;


    ///** @brief ÿ���ؼ�����������������ؼ��洢���ڴ���
    // *  ��ؼ�ʱ�����ô˷�������
    // *  �ڴ˺����ڲ�ʵ�ְ汾����
    // *  @param[in]          *pstream          Ҫд��stream����������ofstream��Ҳ������ostringstream
    // *  @return bool �ɹ����
    // */ 
    //virtual bool ToMemory(std::ostream *pstream) = 0;
    ///** @brief ÿ���ؼ�����������������ڴ��ж�ȡ�ؼ�
    // *  �ڴ˺����ڲ�ʵ�ְ汾����
    // *  @param[in]          *pstream          Ҫ����stream����������ofstream��Ҳ������ostringstream
    // *  @return bool �ɹ����
    // */ 
    //virtual bool FromMemory(std::istream *pstream) = 0;
    
};

/// ͼ��ؼ��ӿ�
interface  IImage :  virtual public IBCInterFace
{
public:
    virtual IBCInterFace * Clone(int nId)=0;
    virtual int LoadPic(wchar_t *szFileName)=0;

    ///��ȡIMG��Ϣ,��ȡ���ص�ͼƬ���
    virtual int GetPicWidth()=0;
    ///��ȡͼƬ�߶�
    virtual int GetPicHeight()=0;
    virtual void SetSrcX(int x)=0; ///<���ô�ͼ��ԭ�����е�λ��
    virtual void SetSrcY(int y)=0; ///<���ô�ͼ��ԭ�����е�λ��
    virtual void SetSrcW(int w)=0; ///<���ô�ͼ��ԭ�����е�λ��
    virtual void SetSrcH(int h)=0; ///<���ô�ͼ��ԭ�����е�λ��

    /** @brief �����Ƿ����Ҿ�����ʾ
     *	@param[in]      bCrossMirror
     *	@return ��
     */
    virtual void SetCrossMirror(bool bCrossMirror)=0;
    virtual void SetDir(int degree, POINT pt)=0;

    /** 
     * @brief ����������ͼƬ��Χ���Ƿ���ʾΪ���ι��
     * @param[in] bShow true:��ʾ, false:����ʾ
     */
    virtual void SetIsHandCursor(bool bShow) = 0;

	///<�Ƿ���괩͸
	virtual void SetMouseThough(bool bThough)=0;    

	///< �Ƿ���Ӧ����ƶ���Ϣ
	virtual void SetMouseMove(bool bMouseMove) = 0;

	/// ����ͼƬ�ĸ��������ɫ�������ϣ����ϣ����¡����µĴ���
	virtual void SetColor(DWORD c1, DWORD c2, DWORD c3, DWORD c4) = 0;

	//��ȡͼƬ��ת�Ƕ�
	virtual int GetDir() = 0;
};

/// ��ť�ؼ��ӿ�
interface  IButton : virtual public IImage
{
public:
    ///���еĺ��� ���û��ӿ�
    virtual void SetEnable(bool flag)=0;              ///<���ÿؼ��Ƿ����
    virtual bool GetEnable()=0;;                      ///<��ȡ�ؼ��Ƿ����
    virtual void SetClickState(bool bFlag)=0;         ///<�����Ƿ��ǵ��״̬ 
    virtual void SetSelectState(bool bFlag)=0;        ///<�����Ƿ���ѡ��״̬
    virtual void SetText(wchar_t *pchar)=0;			  ///<��������ڰ�ť��ʾ��̬����
    virtual IBCInterFace * Clone(int nId)=0;
	virtual void CreateRgn() = 0;                     ///<��������
    virtual void SetParameter(void * pParam) = 0;     ///<���ð�ť�еĲ���
    virtual void * GetParameter() = 0;                ///<��ȡ��ť�еĲ���
};


///�ı��ؼ�
interface  IText: virtual public IBCInterFace
{
public:
	virtual wchar_t * GetText()=0;                      ///<��ȡ�ؼ���ʾ����
	virtual void SetText(wchar_t * pText)=0;///<���ÿؼ���ʾ����
	virtual void SetFontColor(COLORREF crFontColor)=0; ///<���ÿؼ�������ʾ��ɫ
	virtual IBCInterFace * Clone(int nId)=0;
	virtual void SetFontShowStyle(FONTSHOWSTYLE showStyle)=0;   ///<���ÿؼ�������ʾģʽ�����󣬾��У�����
    virtual void SetMutiShow(bool bFlag)=0;             ///<�����Ƿ������ʾ
    virtual void SetMouseThough(bool bThough)=0;    ///<�Ƿ���괩͸
	virtual int GetFontSize()=0; 
	virtual void SetFontSelectColor(DWORD color) = 0;	 ///��������ѡ����ɫ
	virtual bool GetIsShowOutline()=0;	///�Ƿ���ʾ��ߡ���LCF
	virtual DWORD GetOutlineColor()=0;	///�����ɫ����LCF
	virtual void SetFontOutlineInfo(bool isOutline, COLORREF color) = 0;	 ///���������Ƿ���߼������ɫ����LCF
};
///�ı��༭��ӿ�
interface  IEditorText : virtual public IText
{
	virtual void SecondInit()=0;
	virtual void Clear()=0;
	virtual IBCInterFace * Clone(int nId)=0;

    /** @brief ������������ĳ��Χ��ASCII
     *  ���ֻ��������'0'~'9'������� SetEnableASCII('0', '9');
     *  @param[in]      nType       ����ģʽ��0��ȫ������1�������֣�2��������
     *	@return
     */
    virtual void SetEnableASCII(int nType, wchar_t cStart=' ', wchar_t cEnd=' ') = 0;
    /** @brief ������������ַ�
     *	@param[in]      nMaxChar    ��������ַ�
     *	@return ��
     */
    virtual void SetMaxTypeIn(int nMaxChar) = 0;
    /// �����Ƿ����
    virtual void SetEnable(bool bEnable) = 0;

    /** 
    * @brief ���ô˱༭���������ֵ��Χ
    * @param[in] bEnable �˹����Ƿ�����
    * @param[in] nMinNum Ҫ���Ƶ���С��ֵ��Χ
    * @param[in] nMaxNum Ҫ���Ƶ������ֵ��Χ
    */
    virtual void SetNumberLimit(bool bEnable, int nMinNum = 0, int nMaxNum = 0) = 0;
	virtual bool GetIsPassWord()=0;
	virtual void SetSendKeyCharMessage(bool bSendKeyCharMessage) = 0;///<�Ƿ����ַ���Ϣ
	virtual bool GetSendKeyCharMessage() = 0;///<��ȡ�Ƿ����ַ���Ϣ
};
///����ؼ��ӿ�
interface  ICard: virtual public IBCInterFace
{
public:
	virtual void SetShowNumFlag(bool bFlag)=0; ///<�����Ƿ���ʾ����
	virtual bool GetShowNumFlag()=0;           ///<��ȡ�Ƿ���ʾ����

	virtual void SetCardSortFlag(bool bFlag)=0; ///<��������ʽ
	virtual bool GetCardSortFlag()=0;           ///<��ȡ����ʽ
	///�������ϵ���
	virtual bool SetCard(BYTE bCardList[], BYTE bUpList[], BYTE bCardCount)=0;
	///���ô��ȥ����
	virtual void SetMoveCard(BYTE bMoveList[], BYTE bMoveCardCount)=0;
	///��ȡ��һ�ִ��ȥ������Ϣ������������
	virtual byte GetMoveCard(BYTE bCardList[])=0;
	///��ȡ������˿�
	virtual BYTE GetUpCard(BYTE bCardList[])=0;
	///��ȡ����ʣ���˿���Ϣ ��������Ŀ
	virtual BYTE GetCard(BYTE bCardList[], BYTE bUpList[])=0;
	///��ȡ�˿���Ŀ
	virtual BYTE GetCardCount()=0;
	///��ȡ�����˿���ֵ
	virtual BYTE GetCardValue(BYTE bIndex)=0 ;
	///��ȡ�˿�λ��
	virtual POINT GetCardPos(BYTE bIndex)=0;
	///��ѡ����ƴ��ȥ����ʣ�������ʾ��  ��ɾ��������˿�,��ʾʣ����ƣ�
	virtual int OutCard()=0;
	
};

interface  ICombox: virtual public IBCInterFace
{
public:
	///�û��ӿ�
	virtual wchar_t * GetCurSelValue()=0;                   ///<��õ�ǰѡȡ�����ֵ
	virtual void AddString(wchar_t * pchar)=0;             ///<����Ԫ��
	virtual wchar_t * SelectString(int vindex)=0;              ///<ѡ�������Ԫ��
	virtual int DeleteString(int nIndex)=0;                    ///<ɾ���ڼ���
	virtual int FindString(wchar_t * pchar) const=0;           ///<�����������Ŀ
	virtual int GetCount() const=0;                        ///<��ȡԪ�ظ���
	virtual int GetCurSel() const=0;                       ///<��õ�ǰѡȡ����
	virtual int SetCurSel(int nSelect)=0;                  ///<���õ�ǰѡȡ����
	virtual int GetTopIndex() const=0;                     ///<��ȡĬ����ʾ����
	virtual int SetTopIndex(int nIndex)=0;                 ///<����Ĭ����ʾ����
	virtual void Clear()=0;                                ///<���������
	virtual int SetItemHeight( int cyItemHeight)=0;         ///<���õ�����Ŀ�ĸ߶ȣ���ȺͿؼ����һ��
	virtual void SetAddItemWidth(int iAddWidth)=0;			///<���ӿ��
	virtual int GetAddItemWidth()=0;						///<��ȡ���ӿ��

	virtual void SetControlMoveDirect(BYTE bDir)=0;  ///<������������ʾ�ķ��� Ĭ�� Ϊ0���£� 1����
	virtual IBCInterFace * Clone(int nId)=0;

    virtual void SetEditable(bool bCanEdit)=0;
	virtual wchar_t * GetText()=0;							///<��ȡ�ؼ���ʾ����
	virtual void SetText(wchar_t * pText)=0;				///<���ÿؼ���ʾ����   
	virtual void SetMaxTypeIn(int nMaxChar) = 0;
	virtual void TextClear()=0;								///<���Combox��Edit���������
	virtual void SetFontShowStyle(FONTSHOWSTYLE style)=0;
	virtual void SetFontSelectColor(DWORD color) = 0;					///��������ѡ����ɫ
	virtual DWORD GetFontSelectColor() = 0;								///��ȡ����ѡ����ɫ

    virtual void SetComboxFontSize(int fontsize) = 0;       ///<���������б���е������С
    virtual void SetComboxFontColor(COLORREF crColor) = 0;  ///<���������б���е�������ɫ
    virtual void SetTooltipEnable(bool bEnable) = 0;        ///<�����Ƿ�����������ʾ
};

interface  IInputUserName: virtual public ICombox
{
public:
    virtual void SetMaxNum(int nMaxNum)=0;
	virtual void SetDeleteButton(bool bFlag) =0;			///<�����Ƿ���ʾɾ����ť
	virtual bool GetDeleteButton() =0;						///<��ȡɾ����ť��ʾ״̬
	virtual void SetAddSelectW(int iAddSelect) =0;			///<����������ӳ����
	virtual int GetAddSelectW() =0;							///<��ȡ������ӳ����
	virtual void SetFontSelectColor(DWORD color) = 0;		///<���ñ༭��ѡ�������ɫ
};

////////////////////////////////////////////////////////////////////////////////
///����
// �����ķ�ҳ����
interface IContainPaginator
{
public:
    virtual void SetIsPagiStyle(bool bIs) = 0;
    virtual bool GetIsPagiStyle() = 0;
    virtual void InsertPagecontent(IBCInterFace * pcontrol)=0;  ///< ��������
    virtual void InsertNormalcontent(IBCInterFace * pcontrol)=0;///< ���븨����ť
    virtual bool SetNumPerPage(BYTE nNum) = 0;                  ///< ���ô�����һҳ�Ŷ���
    virtual int GetNumPerPage() = 0;                            ///< ���ô�����һҳ�Ŷ���
    virtual bool NextPage() = 0;                                ///< ���·�ҳ
    virtual bool PrevPage() = 0;                                ///< ���Ϸ�ҳ
    virtual void FirstPage() = 0;                               ///< ��һҳ
    virtual void LastPage() = 0;                                ///< ���һҳ
    virtual bool SetCurrentPage(int nCup) = 0;                  ///< �ƶ���ĳһҳ
    virtual int GetCurrentPage() = 0;                           ///< ��ǰҳ
    virtual int GetTotalPage() = 0;                             ///< ͳ��ҳ��
    virtual void SetNoPagecontentID(int nIDs[], int nSize)=0;   ///< ����10������Ҫ��ҳ�Ŀؼ�ID

    /**
    * @brief ��������ߵļ������������֮��ļ��
    * @param[in]   nLSpace     ����ߵļ��
    * @param[in]   nRSpace     ����ߵļ��
    * @param[in]   nTSpace     ����ߵļ��
    * @param[in]   nBSpace     ����ߵļ��
    * @param[in]   nHBetweenSpace  ����Ԫ���ں�����
    * @param[in]   nVBetweenSpace  ����Ԫ����������
    */
    virtual void SetSpaces(int nLSpace, int nRSpace, int nTSpace, int nBSpace,
        int nHBetweenSpace, int nVBetweenSpace)=0;     
    /**
    * @brief �õ������߾�
    */
    virtual void GetSpaces(int &nLSpace, int &nRSpace, int &nTSpace, int &nBSpace,
        int &nHBetweenSpace, int &nVBetweenSpace)=0;   

    /**
    * @brief �Ƴ�����
    */
    virtual void RemovePagecontent(IBCInterFace * pcontrol) = 0;
};
///����
interface  IContain: virtual public IBCInterFace
{
public:
	virtual void MoveControl(int DistanceX,int DistanceY)=0;///<�ƶ����������е����пؼ�ҲҪһ���ƶ�
	virtual void SetChildVisable(IBCInterFace *pChild, bool bVisable)=0;
	virtual IBCInterFace * Clone(int nId)=0;
	virtual int SetPic(int nindex,wchar_t * pwhcar)=0;    ///<�������пؼ�����ͼƬ�����ﴫ���ǿؼ�ID���ɹ�����0 ʧ�ܷ���-1

	virtual int GetControlSize()=0;  ///<�����ؼ�����
	virtual int GetControlBlongContainID(int id)=0;  ///<��ȡָ��ID���ϲ������ؼ�ID
	virtual void IInsertControl(IBCInterFace * bControl)=0;///<�򴰿�����ӿؼ�
	virtual void IDeleteControl(IBCInterFace * pcontrol)=0;    ///< �Ƴ�ĳ���ؼ�
	///�û��ӿ�
//	virtual BControlInterface * GetControl(POINT point)=0; ///<����λ�úͿؼ������ȷ����ȡ�Ŀؼ�ָ��
//	virtual BControlInterface * GetControl(int id)=0; ///<����λ�úͿؼ������ȷ����ȡ�Ŀؼ�ָ��
    virtual IContainPaginator *GetContainPaginator() = 0;   ///<��ȡ��ҳ����	

    virtual void SortAllSubControlByZord() = 0;
	virtual bool GetControlShowFlag(IBCInterFace *pBcInterface)=0;
};

///<��̬����
interface ICartoon:virtual public IBCInterFace
{
public:
	virtual void SetSpeed(int speed)=0;  ///<���ö����ٶ�
	virtual void SetState(PLAYSTATE bstate)=0; ///<���ö���״̬
	virtual void SetPlayNum(int iplaynum)=0; ///<���ò��Ŵ���
	virtual byte GetFrameNum()=0;             ///<��ȡ֡��
	//virtual bool GetVisble()=0;              ///<��ȡ�ؼ��Ƿ���ʾ
	virtual int GetSpeed()=0;                ///<��ȡ�����ٶ�
	virtual PLAYSTATE GetState()=0;                ///<��ȡ����״̬
	virtual int GetPlayNum()=0;             ///<��ȡ���Ŵ���
	virtual byte GetCurrentFrame()=0;  ///<��ȡ��ǰ���ŵ�֡����

	virtual void SetShowIndex(int nIndex)=0; ///<����Ҫ��ʾ��֡������
	virtual IBCInterFace * Clone(int nId)=0;

	virtual void SetPlayStarFrameNum(int nindex)=0;
	virtual void SetPlayEndNum(int nNum)=0;
	
	virtual void SetCurrentShowFrameNum(int nNum)=0;
	virtual int GetPlayEndNum()=0;
	virtual int GetCurrentShowFrameNum()=0;
	virtual int GetPlayStartNum()=0;
};

///������
interface  IProgress: virtual public IBCInterFace
{
public:
	///�û��ӿ�
	virtual void SetMaxValue(int MaxValue)=0;///<���ý�������������ʾֵ
	virtual void SetCurValue(int CurValue)=0;///<���ý��������ڵ�ֵ

	virtual int GetMaxValue()=0;             ///<��ȡ��������������ʾֵ
	virtual int GetCurValue()=0;             ///<��ȡ���������ڵ���ʾֵ
	virtual IBCInterFace * Clone(int nId)=0;
};

interface  IScrollBar:virtual public IBCInterFace
{
public:
	///�û��ӿ�
	virtual void SetMaxValue(float maxValue)=0;         ///<���������ֵ
	virtual float GetMaxValue()=0;                      ///<��ȡ���ֵ

	virtual void SetCurValue(float curValue)=0;         ///<���õ�ǰֵ
	virtual float GetCurValue()=0;                      ///<��ȡ��ǰֵ
	virtual IBCInterFace * Clone(int nId)=0;
	virtual void SetBarHeightFixed(bool bFlag)=0;
};

///IMAGE�ƶ�����
interface  IImageFlash: virtual public IBCInterFace
{
public:
	virtual void SetPlayNum(int num)=0;                         ///<���ò��Ŵ���
	virtual int GetPlayNum()=0;                                 ///<��ȡ���Ŵ���
	virtual void SetPlayState(bool flag)=0;                   ///<�����Ƿ�ʼ����
	virtual bool GetPlayState()=0;                              ///<��ȡ����״̬

	virtual void SetStartP(POINT point)=0;                                 ///<���ÿ�ʼX
	virtual void SetEnd(POINT point)=0;                                   ///<�����յ�X
	virtual void SetControlPoint(POINT point)=0;                ///<�����м���Ƶ�����
	virtual POINT GetControlPoint()=0;                          ///<��ȡ�м���Ƶ�����
	virtual POINT GetStartP()=0;                                  ///<��ȡ��ʼx
	virtual POINT GetEnd()=0;                                    ///<��ȡ�յ�X
	virtual IBCInterFace * Clone(int nId)=0;
};

interface IWeb:virtual public IBCInterFace, virtual public IManual
{
public:
	///<��������
	virtual void SetShowWebAddress(wchar_t* pchar)=0;
    /// ʹ�ô���
    /// ʹ�ô��ڼ�����ҳ�ؼ����Ӵ��ڵ���ʽ������֧�ָ����������
    /// �����óɽ�����ƺ󣬴˺�����Ч
    virtual void SetUseWindow(bool bUse = true) = 0;
    /// ���ý������
    /// �н������ʱ�����ڻ�ý���ʱ�л��ɴ���ģʽ����ס���пؼ�������SetUseWindow������Ч��
    /// ���޽���ʱ�л�����ͨģʽ���ɱ������ؼ���ס
    virtual void SetFocusSwitching(bool bSwitch) = 0;
};

interface IRadioButton:virtual public IBCInterFace
{
public:
	virtual void SetText(wchar_t * pchar)=0;                 ///<������ʾ�ַ�
	virtual void SetTextFontSize(int nSize)=0;               ///<������ʾ�����С
	virtual int GetTextFontSize()=0;                         ///<��ȡ��ʾ�����С
	virtual void SetTextFontColor(COLORREF crColor)=0;       ///<����������ɫ
	virtual COLORREF GetTextFontColor()=0;                   ///<��ȡ������ɫ
	virtual bool GetIsSelect()=0;                             ///<��ȡ�Ƿ���ѡ����
	virtual void SetIsSelect(bool bFlag)=0;                  ///<�����Ƿ���ѡȡ״̬
	virtual IBCInterFace * Clone(int nId)=0;
	virtual void SetBkPic(wchar_t * pwhcar)=0;
	virtual void SetSelPic(wchar_t * pwhcar)=0;
};
interface ITalk:virtual public IBCInterFace
{
	virtual  void SetText(wchar_t *  wchar)=0; ///<������ʾ������
	virtual int  GetRowCount()=0;            ///<��ȡ��ǰ��ʾ������
	virtual wchar_t * GetText(int nIndex)=0;   ///<��ȡĳһ�е���ʾ�ַ���
	virtual IBCInterFace * Clone(int nId)=0;
	virtual void ClearText()=0;
	virtual bool GetIsAutoScroll()=0;
	virtual void SetIsAutoScroll(bool bAuto)=0;
	virtual void CopyAllText()=0;
};
interface IBeacon:virtual public IBCInterFace
{

};

interface IListControl:virtual public IBCInterFace
{
public:
	///<��������
	virtual void SetRowHeight(int nHeight)=0;                  ///<����ÿ�еĸ߶�
	virtual int  GetRowHeight()=0;							   ///<��ȡÿ�еļ��
	virtual void SetImageWH(int nWidth,int nHeight)=0;                      ///<����ÿ�������У�ͼƬ�Ŀ��
	virtual void SetMinColWidth(int nWidth)=0;                    ///<����ÿ�е���С���
	virtual void InserHeadNode(int nindex)=0; ///<��ӱ�ͷ�ڵ�
	virtual void SetHeadWH(int nIndex,int nWidth,int nHeight)=0;///<���ñ�ͷ����Ŀ��
	virtual void SetHeadSpaceW(int nWidth)=0;                   ///<���ñ�ͷ���ͼƬ�Ŀ�

	virtual void InsertObjNode(int nRowIndwx,int nColIndex)=0;           ///<����һ������ ��ֻ�������в���
	virtual void SetRowFontText(int nRowIndwx,int nColIndex,wchar_t * pFontStyle)=0;///<����ÿ��ÿ�е���ʾ������

	virtual void SetObjImagePath(int nObjRowIndex,int nColIndex,int nImageIndex,wchar_t * path)=0;  ///<���ö�������ӦIMAGE�ؼ���ͼƬ·��
	//virtual void LoadObjImage(int nObjRowIndex,int nColIndex,int nImageIndex,wchar_t *szFileName)=0;///<Ϊ��������ӦIMAGE�ؼ�����ͼƬ

	virtual void LoadBkImage(wchar_t * Path)=0;     ///<���ؿؼ���ͼ 
	virtual wchar_t * GetBkImage()=0;               ///<��ÿؼ���ͼ

	virtual void InsertColumn(int nIndex,wchar_t * pText,int nWidth,int nHeight)=0;              ///<�����ͷ

    /** 
     * @brief ����ڵ�
     * @param [in] nRow     �������
     * @param [in] nCol     �������
     * @param [in] pText    ������ı�����
     * @param [in] pContain ���������(��Ҫ���ڵ�Ԫ����Ƕ��һ������, �����е�����������), ��������ʱ, ��3������pText��Ч(���: L"")
     * @return void
     */
	virtual void InsertInItem(int nRow, int nCol, wchar_t * pText, IContain * pContain = NULL) = 0;

	///�Ա�ͷ�����ķ���
	virtual void SetFontStyle(wchar_t * pFontStyle)=0;  ///<���ñ�ͷ������ʽ
	virtual void SetFontSize(int fontsize)=0;       ///<���ñ�ͷ�����С
	virtual void SetHeadFontText(int nIndex,wchar_t * pFontStyle)=0;///<���ñ�ͷ��ʾ������

	virtual wchar_t * GetFontStyle()=0;                       
	virtual int  GetFontSize()=0;  ///<��ȡ�����С
	virtual COLORREF GetFontColor()=0;               ///<��ȡ�ؼ�������ʾ��ɫ
	virtual void SetFontColor(COLORREF crFontColor)=0; ///<���ÿؼ�������ʾ��ɫ

	virtual void LoadHeadBkImage(wchar_t * path)=0;     ///<���ر�ͷ��ͼ
	virtual void LoadHeadSpaceImage(wchar_t * path)=0;///<���ر�ͷ���ͼƬ

	virtual wchar_t * GetHeadBkImagePath()=0;           ///<��ȡ��ͷͼƬ·��
	virtual wchar_t * GetHeadSpaceImagePath()=0;           ///<��ȡ��ͷͼƬ·��

	virtual void SetHaveScroll(bool bFalg)=0;   ///<�����Ƿ���Զ�й�����
	virtual bool GetHaveScroll()=0;             ///<��ȡ�Ƿ�����Զ�й���������

	
	//virtual void LoadScrollBkImage(wchar_t * path)=0;     ///<���ֵ�ͼ
	//virtual wchar_t * GetScrollBkImage()=0;

	//virtual void LoadScrollTImage(wchar_t * path)=0;   ///<���������İ�ŤͼƬ
	//virtual wchar_t * GetScrollTImage()=0;

	//virtual void LoadScrollBImage(wchar_t * path)=0;   ///<���������İ�ŤͼƬ
	//virtual wchar_t * GetScrollBImage()=0;

	//virtual void LoadScrollBarImage(wchar_t * path)=0;  ///<�м���ֵ�ͼƬ
	//virtual wchar_t * GetScrollBarImage()=0;
	
	virtual void SetHeadObjHeight(int nH)=0;  ///<���ñ�ͷ����ĸ�
	virtual int GetHeadObjHeight()=0;    ///<��ȡ��ͷ����ĸ߶�

	virtual void Clear()=0;               ///<��������У�������ձ�ͷ
	virtual void SetHeadFontStyle(wchar_t * pFontStyle)=0;
	virtual void SetHeadFontSize(int fontsize)=0;
	virtual void SetHeadFontColor(COLORREF crColor)=0;
	virtual void SetRowFontStyle(int nRowIndex,wchar_t * pFontStyle)=0;
	virtual void SetRowFontSize(int nRowIndex,int fontsize)=0;
	virtual void SetRowFontColor(int nRowIndex,COLORREF crColor)=0;
	virtual void SetRowFontColor(int nRowIndex,int nColIndex,COLORREF crColor)=0;


	virtual void SetSelectRectColor(COLORREF crSelect)=0;            ///<����ѡ���е���ɫ
	virtual COLORREF GetSelectRectColor()=0; ///<����ѡ���е���ɫ

	virtual void SetRowFontIsBold(int nRowIndex,bool bFlag)=0;
	virtual void SetHeadFontIsBold(bool bFlag)=0;
	virtual IBCInterFace * Clone(int nId)=0;

	virtual wchar_t * GetItemText(int nRow,int nCol)=0;  ///<�����ͷ���±궼��0��ʼ��
	virtual void ModifyItemText(int nRow,int nCol,wchar_t * pchar)=0; ///<�޸�ĳ��ֵ
	virtual int GetRowNum()=0;
	virtual int GetColNum()=0;
	virtual void SetTextShowStyle(int nCol,FONTSHOWSTYLE style)=0; ///<����ĳ�е���ʾ��ʽ
	virtual void ModifyHeadFontText(int nIndex,wchar_t * pFontText)=0;///<���ñ�ͷ��ʾ������
	virtual int GetSelectRowIndex()=0;                                ///<��ȡѡ�е����±꣬��0��ʼ��ûѡ���򷵻�-1

	virtual void  DeleteRow(int nIndex)=0;        ///<ɾ��ĳһ��  ����ɾ����ͷ��ֻ���޸�

    virtual void SetIsMouseSelectColor(bool bFlag) = 0;       ///< �Ƿ���������ƶ�ѡ����ɫ
    virtual void SetMouseSelectColor(COLORREF crSelect) = 0;  ///< ������굱ǰ�����е���ɫ
    virtual COLORREF GetMouseSelectColor() = 0;               ///< ������굱ǰ�����е���ɫ

    virtual void SetCellButton(bool bShow) = 0;               ///< �����Ƿ���ʾ��Ԫ��ť
    virtual void SetButtonImage(wchar_t * path) = 0;          ///< ���õ�Ԫ�����ұ�ָ�밴ťͼƬ

    virtual bool IsBtnRect() = 0;                             ///< �ж��Ƿ���ڰ�ť����
	virtual void SetVerBarHeightFixed(bool bFlag) = 0;		  ///< ���ô�ֱ�������Ƿ�������ݶ����Զ������߶�

	virtual void SetColumnMultiLine(int nRowIndex,int nColIndex,bool bEnable = true) = 0;
};

interface ICardBase:virtual public IBCInterFace
{
	//virtual void SetCardWH(int nW,int nH)=0;  ///<�����ƵĿ��
	//virtual void GetCardWH(int & nW,int & nH)=0; ///<��ȡ�ƵĿ��
	//virtual int GetCardHSpace()=0;           ///<��ȡ�Ƶ�ˮƽ���
	//virtual void SetCardFlushStyle(int nIndex)=0; ///<��������ʽ
	//virtual int GetCardFlushStyle()=0;        ///<��ȡ����ʽ
	virtual void SetCardList(BYTE BList[] ,int nNum)=0;     ///<������ʾ����
	//virtual void SetCardPic(BYTE nIndex,wchar_t * pchar)=0;  ///<�����Ƶ���Դ.˳����ؽ�ȥ
	virtual int GetCardList(BYTE BCardList[])=0;             ///<��ȡ��ʾ���� 
	//virtual wchar_t * GetCardPic(int nIndex)=0;              ///<��ȡ���ʵ�·��
	virtual int GetCardNum()=0;                          ///<��ȡ�Ƶ�����
	virtual void RemoveCard(BYTE BCardList[],int nNum)=0;                   ///<�Ƴ����е���
	//virtual void SetCardAlignStyle(BYTE bStyle)=0;    ///<���ö���ģʽ 0-2  0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ���
	//virtual BYTE GetCardAlignStyle()=0;

	//virtual ENM_CARD_NUMBER GetCardPointByIndex(int nIndex)=0;           ///<��ȡָ���������Ƶĵ���
	//virtual ENM_CARD_SHAPE GetCardFigureByIndex(int nIndex)=0;          ///<��ȡָ���������ƵĻ�ɫ
	//virtual void RemoveCard(BYTE BCardList[],int nNum)=0;                   ///<�Ƴ����е���
    virtual void SetCardHSpace(int nSpace)=0;  ///<�����Ƶ�ˮƽ���

    /** 
     * @brief ����������ͼƬ
     * @param ��ͼƬ��Դ·��
     */
    virtual void SetLaiZiImagePath(wchar_t * pchar) = 0;

    /** 
     * @brief ����������
     * @param bCards       ����������
     * @param byLaiziCount �����С
     */
    virtual void SetLaiZiCard(BYTE bCards[], BYTE byLaiziCount) = 0;

	/** 
	* @brief �����ر���ͼƬ
	* @param ��ͼƬ��Դ·��
	*/
	virtual void SetSpacialImagePath(wchar_t * pchar) = 0;

	//�����Ƿ�������ƶ���Ϣ
	virtual void SetMoveCardMessage(bool bSetMoveCardMessage) = 0;

	//�����Ƿ�������ƶ���Ϣ
	virtual bool GetMoveCardMessage() = 0;

	//��ȡ��ǰ�ƶ���ֵ
	virtual BYTE GetMoveCardNum() = 0;

};

interface INoMoveCard:virtual public ICardBase
{
	virtual void SetIsShowCardNum(bool bFlag)=0;     ///<�����Ƿ���ʾʣ���Ƶ�����
	//virtual bool GetIsShowCardNum()=0;     ///<��ȡ�Ƿ���ʾ�Ƶ�����
	//virtual void SetCardShowDir(CARD_SHOW_DIR cardDir)=0;       ///<�����Ƶ���ʾ����
	//virtual CARD_SHOW_DIR GetCardShowDir()=0;       ///<��ȡ�Ƶ���ʾ����
	virtual IBCInterFace * Clone(int nId)=0;
    /** @brief ����ĳֵ���Ʊ��
     *  @param[in]      ucDarkCard      ����
     *  @param[in]      ucCount         �����С
     */
    virtual void SetDarkCard(BYTE ucDarkCard[], BYTE ucCount, COLORREF clr = 0x66FFFFFF)=0;
    /** @brief ����ĳֵ���Ƶ���
     *  @param[in]      ucUpCard        ����
     *  @param[in]      ucCount         �����С
     *  @param[in]      ucFlag          ��Ӧ�����������ϻ������� 0 ���� 1���� 2����
     */
    virtual void SetUpCard(BYTE ucUpCard[], BYTE ucCount, BYTE ucFlag[])=0;

    virtual void SetCardShowMode(bool bVertical) = 0;  ///< �Ƶİڷŷ�ʽ true: ����,  false:���
    virtual bool GetCardShowMode() = 0;                ///< ��ȡ�Ƶİڷŷ�ʽ true: ����,  false:���
};

interface ILightCard:virtual public INoMoveCard
{
	//virtual void SetShowCardWH(int nW,int nH)=0;
	//virtual void GetShowCardWH(int & nW,int & nH)=0;
	//virtual void SetShowCardRowNum(int nNum)=0;
	//virtual int  GetShowCardRowNum()=0;
	//virtual void SetRowSpace(int nSpace)=0;
	//virtual int  GetRowSpace()=0;
	virtual IBCInterFace * Clone(int nId)=0;
};

interface IOperateCard:virtual public ICardBase
{
	//virtual void SetUpSpace(int nSpace)=0; ///<����������ĸ߶�
	//virtual int  GetUpSpace()=0;           ///<��ȡ������ĸ߶�
	
	virtual void SetUpCardList(BYTE BCardList[],int nNum)=0;
	virtual int GetUpCardList(BYTE BCardList[])=0 ;   ///<��ȡ������Ƶ�ֵ


	//virtual void SetSelectImagePath(wchar_t * pchar)=0; ///<����ѡ���ǵ�ͼƬ
	//virtual wchar_t * GetSelectImagePath()=0;           ///<��ȡѡ���ǵ�ͼƬ·��
	virtual IBCInterFace * Clone(int nId)=0;

    virtual void SetIsDragble(bool bDragble) = 0;

	virtual int GetClickStartIndex(void) = 0;  ///<��ʼ���µ��Ƶ��±�

	virtual const BYTE *GetUpCardSign(void) = 0;	///<��ü�¼�ؼ�������Ƶı�־

};

interface INum:virtual public IBCInterFace
{
	virtual void SetNum(__int64 nNum)=0;    ///<��������
	virtual __int64  GetNum()=0;            ///<��ȡ����
    /** @brief ����ͼƬ�Ƿ��з���
     *  �з���ͼƬ��ʽΪ +0123456789/-0123456789��
     *  ע�⣺ֻ��һ�����Ż�һ�����ţ���������ʱ��Ȼ����������Ĭ����ʾ0ǰ��ķ���
     */
    virtual void SetHaveSymbol(bool bHave)=0; 
    virtual void SetImagePath(wchar_t * pchar)=0;
	virtual void SetZeroImagePath(wchar_t* pchar)=0;
	virtual wchar_t* GetZeroImagePath()=0;
	virtual void SetNumUint(int iNum)=0;
	virtual int GetNumUint()=0;
	virtual void SetRankType(int iRankType)=0;
	virtual int GetRankType()=0;
	//��������ʽ���ְ�
	virtual void SetNewNumStruct(NEW_NUM_STRUCT NumStruct) = 0;
	//��ȡ����ʽ���ְ�
	virtual NEW_NUM_STRUCT GetNewNumStruct() = 0;
	//��������ʽ����
	virtual void SetNewNum(__int64 nNum) = 0;
	//��ȡ����ʽ����
	virtual __int64 GetNewNum() = 0;
	virtual IBCInterFace * Clone(int nId)=0;
};

interface ITime:virtual public IBCInterFace
{
	virtual void SetShowMaxNum(int nNum)=0;         ///<������ʾ���ʱ��
	virtual int  GetShowMaxNum()=0;                 ///<��ȡ��ʾ���ʱ��
	virtual void SetShowMinNum(int nNum)=0;        ///<������ʾ��Сʱ�� 
	virtual int  GetShowMinNum()=0;              ///<��ȡ��ʾ��Сʱ��
	virtual void SetShowAllTime(DWORD dtTime)=0; ///<������ʾ��ʱ��
	virtual DWORD GetShowAllTime()=0;    ///<��ȡ��ʾ��ʱ��
	virtual int  GetCurrentTimeNum()=0;     ///<��ȡ��ǰʱ��
	virtual void SetPLay(bool bFlag)=0;
	virtual void SetPlaySoundNum(int iLessTimeNum) = 0;
	virtual int  GetPlaySoundNum() = 0;
};


interface ISlider:virtual public IBCInterFace
{
	virtual void SetMaxValue(int nMax)=0;
	virtual int GetMaxValue()=0;
	virtual void SetCurrentValue(int nCurrentValue)=0;
	virtual int GetCurrentValue()=0;
	virtual IBCInterFace * Clone(int nId)=0;
	virtual void SetFillPath(wchar_t * pchar)=0;
	virtual void SetButtonSliderPath(wchar_t * pchar)=0;
};
interface IMenu:virtual public IBCInterFace
{
	virtual void AddString(wchar_t * pchar,int nKind=0)=0; ///<����ӵ�TEXT���Ƿָ��ߣ�0��ʾTEXT��1��ʾ�Ƿָ���

	virtual void SetItemHeight(int nH)=0;
	virtual void SetLineColor(COLORREF crColor)=0;


	virtual void SetFontStyle(wchar_t * pchar)=0;
	virtual void SetFontSize(int nSize)=0;                    ///<��ȡ�ؼ������С
	virtual void SetFontColor(COLORREF crColor)=0;
	virtual void SetFontShowStyle(FONTSHOWSTYLE style)=0;

	virtual void SetBKImagePath(wchar_t * pchar)=0;

	virtual void SetMoveColor(COLORREF crColor)=0;
	virtual int GetSelectIndex()=0;
	virtual int GetItemHeight()=0;

	virtual void SetMoveFramColor(COLORREF crColor)=0;
	virtual void Clear()=0;
	virtual void DeleteItem(int index)=0;
	virtual wchar_t * GetItemText(int index)=0;
};

interface ISudoku:virtual public IBCInterFace
{
	virtual RECT GetSrcRect(int nIdx)=0;	///< ��ȡָ������ͼƬ��С
	virtual int SetPicByIndex(int nIdx,wchar_t *szFileName,RECT& rc) = 0; ///<���þŹ���ͼ
};

interface ISiTuanOperateCard:virtual public IOperateCard
{
    ///// ���ô�С������һ��
    //virtual void SetJoJOEqual(bool bEqual) = 0;
};
interface ISiTuanNomoveCard:virtual public INoMoveCard
{
	
};
interface ISiTuanLightCard:virtual public ILightCard
{

};

///IMAGE�ƶ�����
interface  IExAnimate: virtual public IBCInterFace
{
public:
    virtual void SetPlayNum(int num)=0;                         // ���ò��Ŵ���
    virtual int GetPlayNum()=0;                                 // ��ȡ���Ŵ���
    virtual void SetPlayState(bool flag)=0;                     // �����Ƿ�ʼ����
    virtual bool GetPlayState()=0;                              // ��ȡ����״̬

    virtual void SetStartP(POINT point)=0;                      // ���ÿ�ʼX
    virtual void SetEnd(POINT point)=0;                         // �����յ�X
    virtual void SetControlPoint(POINT point)=0;                // �����м���Ƶ�����
    virtual POINT GetControlPoint()=0;                          // ��ȡ�м���Ƶ�����
    virtual POINT GetStartP()=0;                                // ��ȡ��ʼx
    virtual POINT GetEnd()=0;                                   // ��ȡ�յ�X

    
    virtual void SetMoveTotalTime(int atime)=0;                 // �����ƶ�ʱ��
    virtual int GetMoveTotalTime()=0;                           // ��ȡ�ƶ�ʱ��

    
    virtual void SetFrameNum(int nNum)=0;                       // ���ö���֡��
    virtual int GetFrameNum()=0;                                // ��ȡ����֡��

    virtual void SetUseMouse(bool bFlag) = 0;                   // �Ƿ���괩͸
    virtual void SetDisplayColor(COLORREF clr) = 0;             // ��ʾ����ɫ

    virtual void SetPlayTotalTime(int atime)=0;                 // ���ò�����ʱ��
    virtual int GetPlayTotalTime()=0;

    virtual IBCInterFace * Clone(int nId)=0;

    virtual const wchar_t * GetExtFilePath() = 0;
    virtual void SetExtFilePath(wchar_t *pwszFileName) = 0;
    virtual bool LoadFromFile() = 0;

    virtual void SetCurveInstance(CURVE_TYPE curType) = 0;      // ������������
    virtual CURVE_TYPE GetCurveType() = 0;                      // �����������

    virtual void SetIsSymmetrical(bool bIsSymmetrical) = 0;     // �Ƿ�Գ�
    virtual bool GetIsSymmetrical() = 0;                        // �Ƿ�Գ�

    virtual void SetIsLoop(bool bIsLoop = true) = 0;            // �Ƿ�ѭ������
    virtual bool GetIsLoop() = 0;                               // �Ƿ�ѭ������

    virtual void SetShowFrame(int nCount) = 0;                  // ��ʾĳһ֡

    /** @brief �����Ƿ����Ҿ�����ʾ
     *	@param[in]      bCrossMirror
     *	@return ��
     */
    virtual void SetCrossMirror(bool bCrossMirror)=0;

	virtual void SetMouseThough(bool bThough) =0;    ///<�Ƿ���괩͸
};


////////////////////////////////////////////////////////////////////////////////
// �齫�ؼ��ӿ�begin{{

const int MAX_TILES_INCTRL = 200;

const int MAX_GOD_TILE = 16;
const int MAX_GIVE_TILE = 100;

const int MAX_HAND_TILE = 20;

const int MAX_BLOCK_COUNT = 10;

// UI�������� 
enum MJ_UI_TYPE
{
    UT_NONE=-1          ,   // ���ƣ�û�ƣ���������
    UT_SELF=0           ,   // �Լ������С��
    UT_FOLLOW=1         ,   // �¼ҷŵ���
    UT_OPPOSITE=2       ,   // �Լҷŵ���
    UT_AHEAD=3          ,   // �ϼҷŵ���
    UT_WALL_VERL=4      ,   // �������ǽ
    UT_WALL_VERR=5      ,   // �����ҳ�ǽ
    UT_WALL_NEAR=6      ,   // ��Ľ���ǽ
    UT_WALL_FAR=7       ,   // ���Զ��ǽ
    UT_FOLLOW_BACK=8    ,   // �¼ҵ��Ʊ�
    UT_OPPOSITE_BACK=9  ,   // �Լҵ��Ʊ�
    UT_AHEAD_BACK=10    ,   // �ϼҵ��Ʊ�
    UT_SELF_LAY=11      ,   // �Լ��ŵ��Ĵ���
    UT_FOLLOW_LAY=12    ,
    UT_OPPOSITE_LAY=13  ,
    UT_AHEAD_LAY=14     ,
    UT_SELF_STAND=15    ,   // �Լ�վ������
};

enum MJ_LOGIC_TYPE
{
    LT_EMPTY               ,    // û����Ϊ���������ƣ�
    LT_COLLECT             ,    // ����
    LT_TRIPLET             ,    // ��
    LT_QUADRUPLET          ,    // ��
    LT_QUADRUPLET_CONCEALED,    // ����
    LT_QUADRUPLET_REVEALED ,    // ����
    LT_QUANDUPLET_SPECIAL  ,    // �����齫����ܣ��ĸ���һ���
    LT_WIN                 ,    // ����
    LT_TING                ,    // ����
    LT_FLOWER              ,    // ����
    LT_GIVE                     // Ҫ�����ƣ����ұߵ�һ�ţ�
};


/**
* @brief �齫�ؼ�֧�ֵ����ݽṹ
* ע�⣺
* ��Ϊ����ʱһ��MJ_TILE_UIINFOֻ��ʾһ���齫��, pucTilesֻ��һ����Ч
* ��Ϊ����ʱһ��MJ_TILE_UIINFO��ʾһ�����ƣ�pucTiles�����ж��
*/	
struct MJ_TILE_UIINFO
{
    BYTE            ucTiles[4]; /// �齫��ֵ 1~9�� 11~29�� 21~29Ͳ 31~37���������з��� 41~48�����ﶬ÷�����  

    BOOL            bSelect;    /// �Ƿ񱻵�����Ƿ���
    MJ_LOGIC_TYPE   ucActFlag;  /// ��ǣ���¼�����ܵ����ã�
    MJ_UI_TYPE      ucUiType;   /// ���ͣ���DT_XXXXX��
    BYTE            ucChair;    /// ������˭��
    DWORD           dwReserve1; /// ��ռ��λ	0λ��0:����ѡ���ƣ�1:����ѡ����     
                                ///             1λ��0:��ͨ��    ��1:������
    DWORD           dwReserve2; ///     

    MJ_TILE_UIINFO()
    {
        memset(this, 0, sizeof(MJ_TILE_UIINFO));
    }
};

/////////////////////////////////////////////����ؼ�
interface IShCtrl : virtual public IBCInterFace
{
public:
    /**
    * @brief �������ͼƬ��Դ
    * Ŀǰֻ��һ��
    */
    virtual void SetShPic(const wchar_t * pchar) = 0;
    /**
    * @brief ��������ӿ�
    */
    virtual void SetShTileWidth(int nWidth) = 0;
    /**
    * @brief ��������Ӹ�
    */
    virtual void SetShTileHeight(int nHeight) = 0;
    /**
    * @brief �����Ƿ�ʹ�����
    */
    virtual void SetIsUseMouse(bool bUse = true) = 0;
    /**
    /* @brief ��ʾ�������ֵ
    */
    virtual void SetShNote(int iNote) =0;
	/**
	/* @brief ��ʾ������з�ʽ
	*/
	virtual void SetShNoteType(int iNoteType)=0;
	/**
    /* @brief ��ʾ������ų���ֵ
    */
    virtual void SetShStackNote(int iStackNote)=0;

    /// ��Ӧ��get����
    virtual int GetTileCount() = 0;
    virtual bool GetShPic(wchar_t *pszPicpath, int nSrcSize) = 0;
    virtual int GetShTileWidth() = 0;
    virtual int GetShTileHeight() = 0;
    virtual bool GetIsUseMouse() = 0;
    virtual int GetShNote() =0;//�����ʾ�������ֵ
	virtual int GetShNoteType()=0;//��ó���ڷŷ�ʽ
	virtual int GetShStackNote()=0;//�����ʾ������ų���ֵ

    virtual void SetTrimNoteMode(int nTrimNote)=0;
	/** @brief ���õ�ǰ�Ǽ����˵ĳ���ڷŷ�ʽ
	 *  @param[in]    ������Ϊ4 �ڷŷ�ʽm_iShNoteType:0�Լң�1�Ҽң�2�Լ���3���
	*/
	virtual void SetShNumber(int iNumber)=0;
	virtual int GetShNumber()=0;//��ȡ��ǰ�Ǽ����˵ĳ���ڷŷ�ʽ
};
/////////////////////////////////////////////����ؼ�

interface IMjCtrl : virtual public IBCInterFace
{
public:
    /**
    * @brief ��������Ϣ
    * ע�⣺
    * ��Ϊ����ʱһ�� MJ_TILE_UIINFOֻ��ʾһ���齫��, pucTilesֻ��һ����Ч
    * ��Ϊ����ʱһ��MJ_TILE_UIINFO��ʾһ�����ƣ�pucTiles�����ж��
    */
    virtual bool SetTilesInfo(MJ_TILE_UIINFO pTilesUIInfo[], int nLen) = 0;

    /**
    * @brief �����齫�ӿ�
    */
    virtual void SetMjTileWidth(int nWidth) = 0;
    /**
    * @brief �����齫�Ӹ�
    */
    virtual void SetMjTileHeight(int nHeight) = 0;
    /**
    * @brief �����Ƿ�ʹ�����
    */
    virtual void SetIsUseMouse(bool bUse = true) = 0;

    /// ��Ӧ��get����
    virtual bool GetTilesInfo(MJ_TILE_UIINFO pTilesUIInfo[], int nSrcLen) = 0;
    virtual int GetTileCount() = 0;
    virtual int GetMjTileWidth() = 0;
    virtual int GetMjTileHeight() = 0;
    virtual bool GetIsUseMouse() = 0;
};


interface IMjHandCtrl : virtual public IMjCtrl
{
public:
    /**
    * @brief ��������
    */
    virtual void SetHandInfo(int nHandTiles[], int nCount) = 0;
    /**
    * @brief ����������
    */
    virtual void SetGodTile(int nGodTiles[], int nCount) = 0;    
    /**
    * @brief ������ͼ��λ
    */
    virtual void SetViewDirection(BYTE nViewStation) = 0;
    /**
    * @brief �õ��������
    */
    virtual int GetUpTile() = 0;    
    /**
    * @brief �����ļ����Ʋ���ѡ
    */
    virtual void SetInvalidTile(int nInvalidTiles[], int nCount) = 0;
    /**
    * @brief �����Ƿ����ұ�һ�ŷֿ�
    */
    virtual void SetGiveTileMode(bool bIsGiving = false) = 0;
    /**
     * @brief �����Ƿ�����
     */
    virtual void SetIsLaydown(bool bIsLaydown = false) = 0;
    /**
    * @brief ����һ��������Ϣ
    */
    virtual void AddOneBlock(MJ_LOGIC_TYPE type, BYTE ucTiles[4], BYTE ucChair) = 0;
    /**
    * @brief ����һ�������
    */
    virtual void AddOneSpecQuad(BYTE ucTiles[4], BYTE ucCount[4]) = 0;
    /**
    * @brief �������������ʾ
    */
    virtual void ClearAllBlock() = 0;

    /**
    * @brief �õ���������
    */
    virtual void GetHandInfo(int nHandTiles[], int nCount) = 0;
    /**
    * @brief �õ���λ
    */
    virtual BYTE GetViewDirection() = 0;
    /**
     * @brief �õ�ĳ�����ƵĻ��ƾ���
     */
    virtual RECT GetTileRectByIndex(int nIndex) = 0;
    /** @brief ����ĳ���Ʊ�������Ƶ���ɫ
     *  ����������ϵ����������ƣ���˽ӿڶ�����Ч
     */
    virtual void SetGodColor(int nIndex, bool bIs=true) = 0;

    /** @brief ����������С
    */
    virtual void SetLockSize(bool bLock)=0;

    /** 
     * @brief �����ֶ�ʹ�����ѡ���Ƶ���, ��ʱ����ƶ�Ч����Ч
     * @param [in] bSelect �Ƿ��ֶ�ѡ��
     */
    virtual void SetHandSelect(bool bSelect) = 0;

    /**  
     * @brief �����ֶ�ʹ�����ѡ���Ƶ���, ��ʱ����ƶ�Ч����Ч
     * @param [in] nUpTiles �ؼ��е����������
     * @param [in] nUpCount ����������鳤��
     * @return ����ʵ�ʵĵ����Ƶ�����
     */
    virtual int GetAllUpTile(int nUpTiles[], int nUpCount) = 0;
};

interface IMjGiveCtrl : virtual public IMjCtrl
{
public:
    /**
    * @brief ���ó���
    */
    virtual void SetGiveInfo(int nHandTiles[], int nCount) = 0;
    /**
    * @brief ����������
    */
    virtual void SetGodTile(int nGodTiles[], int nCount) = 0;    
    /**
    * @brief ������ͼ��λ
    */
    virtual void SetViewDirection(BYTE nViewStation) = 0;
	/**
	* @brief ����ÿ����ʾ����, ����������Χ������һ����ʾ
	*/
	virtual void SetGiveLineCnt(int nCount) = 0;

    /**
     * @brief �õ�ĳ���齫�ӵĻ��ƾ���
     */
    virtual RECT GetTileRectByIndex(int nIndex) = 0;

    /**
    * @brief �õ���������
    */
    virtual void GetGiveInfo(int nHandTiles[], int nCount) = 0;
    /**
    * @brief �õ���λ
    */
    virtual BYTE GetViewDirection() = 0;
	/**
	* @brief ��ȡÿ����ʾ����
	*/
	virtual int GetGiveLineCnt() = 0;
};

interface IMjWallCtrl : virtual public IMjCtrl
{
public:
    /**
     * @brief ���Ʊ������ǽ
     * @param[in]        nCount  ���    
     */
    virtual void FillWallBack(int nCount) = 0;  
    /**
     * @brief ������ǽ��ĳ����ʾΪʲô��
     * @param[in]       nIndex  �Ӹ���λ����ҿ���0���ұߵ�һ������һ�� 1���ұߵ�һ������һ�� 2���ұߵڶ�������һ�� ����������
     * @param[in]       nTile   -1Ϊ����ʾ
     */
    virtual void SetWallTile(int nTile, int nIndex) = 0;
    /**
    * @brief ������ͼ��λ
    */
    virtual void SetViewDirection(BYTE nViewStation) = 0;
    /**
    * @brief �õ���λ
    */
    virtual BYTE GetViewDirection() = 0;
    /**
     * @brief �õ�ĳ���齫�ӵĻ��ƾ���
     * @param[in] nIndex  �Ӹ���λ����ҿ���0���ұߵ�һ������һ�� 1���ұߵ�һ������һ�� 2���ұߵڶ�������һ�� ����������
     */
    virtual RECT GetTileRectByIndex(int nIndex) = 0;
    /** @brief ������ǽ��󳤶�
     *  @param[in]      nMaxLength
     */
    virtual void SetMaxLength(int nMaxLength) = 0;
};

interface IMjBlockCtrl : virtual public IMjCtrl
{
    // �ӿڷ������ã���IMjHandCtrl�ϲ���
//public:
//    /**
//    * @brief ����һ��������Ϣ
//    */
//    virtual void AddOneBlock(MJ_LOGIC_TYPE type, BYTE ucTiles[4], BYTE ucChair) = 0;
//    /**
//    * @brief �������������ʾ
//    */
//    virtual void ClearAllDis() = 0;
//    /**
//    * @brief ������ͼ��λ
//    */
//    virtual void SetViewDirection(BYTE nViewStation) = 0;
//
//    /**
//    * @brief �õ���λ
//    */
//    virtual BYTE GetViewDirection() = 0;
};

// �齫�ؼ��ӿ�end}}
////////////////////////////////////////////////////////////////////////////////

interface IMoveAction : virtual public IBCInterFace
{
public:
    /** @brief ����Ҫ���ƵĿؼ�ID
    *	@param[in]      nID   Ҫ���ƵĿؼ�ID��
    *	@return ��������
    */
    virtual bool SetControlingCtrlID(int nID) = 0;
    /** @name ��������
    */
    //@{
    virtual void SetStartP(POINT point) = 0;
    virtual void SetEndP(POINT point) = 0;
    virtual void SetControlP(POINT point) = 0;
    //@}    
    /** @brief ������������
    *	@param[in]      curType         �������ͣ�����μ�enum CURVE_TYPE(IUserInterface.h)
    *	@return ��
    */
    virtual void SetCurveInstance(CURVE_TYPE curType) = 0;
    /** @brief �����ƶ���ʱ��
    *	@param[in]      uTime           �ƶ���ʱ��
    *	@return ��
    */
    virtual void SetTotalMoveTime(unsigned int uTime) = 0;    
    /** @brief �����Ƿ񲥷�
    *	@param[in]      bool            ���Ż�ֹͣ�����Ŵ�0��ʼ����ֹͣ��ѿؼ��Ż�ԭʼλ�ã�
    *	@return ��
    */
    virtual bool SetPlayState(bool bPlay) = 0;    
    /** @brief ���ò���״̬
    *	@param[in]      bool            ���Ż���ͣ
    *	@return �������ͣ����ز���״̬
    */
    virtual bool SetPauseState(bool bPlay) = 0;    

    virtual void SetIsSymmetrical(bool bIsSymmetrical) = 0;     // �Ƿ�Գ�
    virtual bool GetIsSymmetrical() = 0;                                 // �Ƿ�Գ�
    /** @name һЩget����
    */
    //@{
    virtual CURVE_TYPE GetCurveType() = 0;
    virtual int GetControlingCtrlID() = 0;
    virtual IBCInterFace *GetControlingCtrl() = 0;
    virtual POINT GetControlP() = 0;
    virtual POINT GetStartP() = 0;
    virtual POINT GetEndP() = 0;    
    virtual unsigned int GetTotalMoveTime() = 0;
    virtual bool GetPlayState() = 0;
    //@}
};


/** @brief FLASH���ſؼ�
 */
interface IFlashPlay : virtual public IBCInterFace,virtual public IManual
{
public:
    /** @brief ����flash�ļ�����ʼ����
     *	@param[in]      pszSwfFile
     *	@return ��������
     */
    virtual bool LoadFlashFile(const wchar_t *pszSwfFile) = 0;
    /** @brief ���ò���
     *	@param[in]      pszName ������
     *	@param[in]      pszVal  ����ֵ
     *	@return ��������
     */
    virtual bool SetVariable(const wchar_t *pszName, const wchar_t *pszVal) = 0;

    /// ����callfunction
    virtual bool CallFunction(const char *pszFunctionXml) = 0;
    /** @brief ����͸���ؼ�ɫ
     */
    virtual void SetColorKey(COLORREF rgb) = 0;
    /// ʹ�ô���
    /// ʹ�ô��ڼ�����ҳ�ؼ����Ӵ��ڵ���ʽ������֧�ָ����������
    virtual void SetUseWindow(bool bUse = true) = 0;
};

/** @brief ����ƿؼ�
 *  revolving scenic lamp
 */
interface IRSLamp : virtual public IText
{
    /** @brief ���ù����ٶȣ�ÿ���������
    *	@param[in]      nPixelPerSec        ���<����ֹ����
    *	@param[in]      nFreshTime          ˢ��ʱ��
    *	@return ������
    */
    virtual bool SetPixelPerTime(int nPixelPerSec, int nFreshTime) = 0;
    /** @brief ���ù�������
    *	@param[in]      nDir                0����1����
    *	@return ������
    */
    virtual bool SetRevoDir(int nDir) = 0;
    /** @brief ���ı��ļ����������������
     *	@param[in]      *pwszFileName
     *	@return ��������0��Ҫ�ǲ�����0....
     */
    virtual int LoadContentFromFile(wchar_t *pwszFileName) = 0;
    /** @brief ��������֮��ļ��(ms)     
     */
    virtual void SetRowInterval(DWORD dwInterval) = 0;

};

/** @brief ������ؼ��ӿ�
 */
/// ������ؼ��ӿ�
interface ITopPaper : virtual public IBCInterFace
{
	/** @brief ����/��ȡ���ϵ��ı�
	*/
	virtual void SetPaperText(int nRow,int nCol,wchar_t* wszText) = 0;
	virtual wchar_t* GetPaperText(int nRow,int nCol) = 0;

	/** @brief //���������ı�����
	*/
	virtual void ClearAllText() = 0;

};

/** @brief ���
*/
interface IXUICursor
{
public:
    /** @brief ���ù��ͼƬ 
    */
    virtual int SetCursorImage(const wchar_t *pCurImg) = 0;
    /** @brief ��ȡ���ͼƬ·��
    */
    virtual const wchar_t *GetCursorImage() = 0;
    /** @brief ������ͼ����
    */    
    //@{{
    virtual void SetSrcX(int x) = 0; 
    virtual void SetSrcY(int y) = 0; 
    virtual void SetSrcW(int w) = 0; 
    virtual void SetSrcH(int h) = 0; 
    //@}}
    /** @brief ��ȡͼƬ��С
    */
    //@{{
    virtual int GetCursorImageWidth() = 0;
    virtual int GetCursorImageHeight() = 0;
    //@}}
    /** @brief ������ʾʱ�Ƿ��������
    */
    virtual void SetHideCursor(bool bHide) = 0;
    /** @brief ��ȡ��ʾʱ�Ƿ�������ʾ��־
    */
    virtual bool GetHideCursor() = 0;
    /** @brief ��������ģʽ
    *         0�����Ͻ�Ϊ���
    *         1������Ϊ���
    *         2�����Ͻ�Ϊ���     
    */
    virtual int SetPointMode(int nMode) = 0;
    virtual int GetPointMode() = 0;

    /** @brief ���ÿɼ�
    */
    virtual void SetControlVisible(bool bVisible) = 0;
    /** @brief �Ƿ�ɼ�
     */
    virtual bool GetControlVisible() = 0;
};

////////////////////////////////////////////////////////////////////////////
/// ����UI����Ľӿڣ�ͨ�����ӿڣ����Ի�ȡָ���ؼ�ID�Ŀؼ��ӿ�
interface  IUserInterface
{
public:
	///����ȫ���Ǹ��û��Ľӿں���
	virtual BLONGED GetControlBlonged(int id)=0;  ///<����ID�жϿؼ����ں��������������κ������򷵻�none
	virtual int GetControlBlongedId(int id)=0;   ///<����ID��ÿؼ�����������ID������������κ��������򷵻�ID=0
	virtual IBCInterFace * GetIControlbyID(int Id)=0; ///<�õ��ؼ�ָ��
	virtual IBCInterFace * GetIControlbyName(wchar_t * pName)=0;///<�õ��ؼ�ָ��
	virtual int GetOppId(POINT point)=0;
	virtual int GetSelectID()=0;///<��ȡ������ʱ�Ŀؼ�ID
	virtual void DeleteAllControl()=0;///<ɾ���������пؼ�
	virtual int LoadData(wchar_t *szFileName)=0;
	virtual HWND GetWindowsHwnd()=0;///<��ȡ��ǰ���ھ��
	virtual void IInsertControl(IBCInterFace * bControl)=0; ///<�򴰿�����ӿؼ�
	virtual void IDeleteControl(IBCInterFace * bControl)=0;      ///<ɾ�������еĿؼ�
	virtual void InsertControlID(int nId)=0;          ///<��¼��ӵĿؼ�ID  
	virtual void DeleteControlID(int nId)=0;          ///<ɾ����ӵĿؼ�ID
    /** @brief ���ļ������ӿؼ���ID�Ų��䣬�����ID���ظ��ˣ�������
     */
    virtual int AddControlFromFile(wchar_t *szFileName, IContain *pParent = NULL)=0;
    /** @brief ���ù��ID     
     *  ����������ֵ
     *  IDC_ARROW           32512
     *  IDC_IBEAM           32513
     *  IDC_WAIT            32514
     *  IDC_CROSS           32515
     *  IDC_UPARROW         32516
     *  IDC_HAND            32649
     *  IDC_HELP            32651
     */
    virtual bool SetUICursor(int nID)=0;
    /** @brief ��ȡ���ؼ�
    */
    virtual IXUICursor *GetICursor() = 0;
    /** @brief ����ѡ�еĿؼ�
     */
    virtual void SetFocousID(int id) = 0;

    /// duanxiaohui ���ô����Ƿ���ʾ/����(��Ҫ���ȫ������UI��������)
    virtual void SetShow(bool bShow) = 0;
    /// end duanxiaohui

	/*���ÿؼ��Ķ���߽�
	  @param rect ��Դ����ı߿������ֵ
	        ={0,0,0,0} ��ʾ��Դ����ı߿�
	        ={10,20,30,40}��ʾ��Դ�����߿�+10px,����ϱ߿�+20px,����ұ߿�-30px,����±߿�-40px
	  @author:jianguakun 2012.8.28
	*/
	virtual void SetControlsShrinkArea(RECT& rect) = 0;
};

/** @brief �ܺ��ӿؼ��ӿ�
*/
interface IHuziCard : virtual public IBCInterFace
{
    enum 
    {
        MAX_VER_COUNT=4
    };
    /// �༭���ӿ�
public:
    /** @brief ���ô�ֱ���
    */
    virtual void SetVerInterval(int nInterval)=0;
    /** @brief ���ú���ͼƬ
    */
    virtual void SetImagePath(const wchar_t *pPath)=0;
    /** @brief ������ֵ
    *  �����ά����
    */     
    virtual void SetCardList(BYTE ucCard[][MAX_VER_COUNT], int nLen)=0;
    /** @brief ���ô�ֱ��ͼ��Ŀ
    */
    virtual void SetVerCutCount(int nCount)=0;
    /** @brief �����Ƿ��ѡ��
    */
    virtual void SetCanSelectCard(bool bCan)=0;
    /** @brief ��ȡ��ǰѡ�е���
     */
    virtual BYTE GetSelectCard()=0;
    /** @brief ��ȡ��ǰѡ�е�������
    *  ��ά��  [x][y]
    */
    virtual POINT GetSelectIndex()=0;
    /** @brief ���þ�����ʾ
    */
    virtual void SetAlignCenter(bool bCenter = true)=0;
    /** @brief �����Ƿ���϶�
    */
    virtual void SetCanDrag(bool bCan)=0;
    /** @brief ����ĳ�б��
    */
    virtual void SetDarkRow(int nIndex)=0;
	/** ���ô����Ʊ߿�Ϊ��ɫ
    */
	virtual void SetActiveBorder(BYTE iCard[], BYTE iCardCount[],int iCardLength ,DWORD iColor[]) = 0;
	/** @brief �����ƶ�ͼƬ·��
	*/
	virtual void SetMoveImagePath(TCHAR *path) = 0;
	/** @brief ���ö���·��
	*/
	virtual void SetAnimatePath(TCHAR *path) = 0;
	/** @brief �����ƶ��������Ƿ���ɫ
	*/
	virtual void SetMoveDarkIndex(bool bMoveDark) = 0;
public:
    /** @brief Getter����
    */
    //@{{
    virtual int GetVerInterval()=0;
    virtual const wchar_t *GetImagePath()=0;
    virtual void GetCardList(BYTE ucCard[][MAX_VER_COUNT], int nSize, int &nReadSize)=0;
    virtual int GetVerCutCount()=0;
    virtual bool GetCanSelectCard()=0;
    virtual bool GetCanDrag()=0;
	virtual const TCHAR *GetMoveImagePath()=0;
	virtual const TCHAR *GetAnimatePath()=0;
	virtual bool GetMoveDarkIndex()=0;
    //@}}
}; 
 #endif

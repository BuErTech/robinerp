#ifndef I_USERINTERFACEFOREDITOR_H
#define  I_USERINTERFACEFOREDITOR_H
#include <vector>
#include "IUICommands.h"


#define WM_BF_CHANGPROP    WM_USER+487
#define WM_BF_RBUTTONDOWN  WM_USER+491
#define WM_BF_KEYDOWN      WM_USER+492

enum BLONGED{NONE=1,CONTAIN=2};          ///<�ؼ��������࣬none��ʾ�������κοؼ���contain��ʾ���ڰ������ؼ�
enum FONTSHOWSTYLE{AlignmentLEFT,AlignmentCenter,AlignmentFar,AlignmentVerticalCenter};    ///<�������ģʽ
enum FONTSHOWDIRECTION {FormatFlagsLevel,FormatFlagsVertical};
enum ControlKind{C_Contain,C_Button,C_Image,C_Card,C_Cartoon,C_Text,C_EditorText,C_ComBox,C_Progress,C_ScrollBar,
C_ImageFlash,C_Web,C_RadioButton,C_ListControl,C_Beacon,C_Talk,C_Num,C_NoMoveCard,C_LightCard,C_CanOperateCard,C_Slider,
C_InputUserName,C_Menu,C_Sudoku,C_Time,C_SiTuanOperateCard,C_SiTuanNoMoveCard,C_SiTuanLightCard, C_ExAnimate, 
C_MjCtrl, C_MjHandCtrl, C_MjGiveCtrl, C_MjWallCtrl, C_MjBlockCtrl, C_MoveAction, C_ShCtrl, C_FlashPlay,C_RSLamp,C_HuziCard,
C_TopPaper}; ///<�ؼ�����
enum PLAYSTATE{START,PLAYING,PAUSE,CONTINUE,STOP};///<����״̬
enum CARD_SHOW_DIR{MYSELF,LEFT,PARTNER,RIGHT};   ///<����ʾ���� 
typedef enum enmCardShape
{
	ECS_SHAPE_SPECIAL = 5,	/*!< �ر��� */
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


typedef signed char         INT8, *PINT8;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
typedef signed __int64      INT64, *PINT64;
typedef unsigned char       UINT8, *PUINT8;
typedef unsigned short      UINT16, *PUINT16;
typedef unsigned int        UINT32, *PUINT32;
typedef unsigned __int64    UINT64, *PUINT64; 


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
	///�ؼ�˽����������
	virtual int    GetW()=0;                   ///<��ȡ�ؼ���ʾ����	
	virtual int    GetH()=0;                    ///<��ȡ�ؼ���ʾ����
	virtual int	GetId()=0;                      ///<��ÿؼ�ID	
	virtual	byte    GetMode()=0;                ///<�ؼ���ʾģʽ�����У����ң��ϣ��¡�������
	virtual ControlKind    GetControlKind()=0;       ///<��ȡ�ؼ�����
	virtual void    SetControlKind(ControlKind kind)=0;  ///<���ÿؼ�����
	virtual void    SetId(int m_id)=0;                    ///<���ÿؼ�ID
	virtual void    SetName(wchar_t * pchar)=0;            ///<���ÿؼ�����
	virtual void    SetMode(byte bmode)=0;			   ///<������ʾģʽ�����У����ң��ϣ��¡�������
	virtual void    SetVisible(bool Flag)=0;      ///<���ÿؼ��Ƿ���ʾ
	virtual void	SetControlVisible(bool bflag)=0;///<�����Ƿ���ʾ
	virtual void    SetModeXY(int x,int y)=0;///<���ÿؼ���ʾ����				                            
	virtual void    SetW(int w)=0;					   ///<���ÿؼ���ʾ����
	virtual void    SetH(int h)=0;                     ///<���ÿؼ���ʾ����
	virtual int     GetModeX()=0;                     ///<��ȡ�ؼ���ʾ����	���ǻ�ȡ�����ģʽ������
	virtual int     GetModeY()=0;                     ///<��ȡ�ؼ���ʾ����	���ǻ�ȡ�����ģʽ������
	virtual wchar_t *  GetName()=0;                ///<��ÿؼ�����
	virtual bool    GetVisible()=0;           ///<��ÿؼ��Ƿ���ʾ
	virtual int GetZorder()=0;///<��ȡ�ؼ����
	virtual void SetZorder(INT16 zorder)=0;///<���ÿؼ����
	virtual void SetAbsXY(int x,int y)=0;///<���ÿؼ�ʵ������
	virtual int GetAbsX()=0; ///<��ȡ���Ե�Xֵ
	virtual int GetAbsY()=0; ///<��ȡ���Ե�Yֵ
	virtual void SetControlFocus(bool flag)=0;///<���ÿؼ�����

	virtual POINT CoordinateOpToAbs()=0; ///<������������ģʽ���ɾ������꣬������ʾ
	virtual POINT CoordinateAbsToOp()=0;///<���������껻���������

	virtual void SetIsMove(bool bFlag)=0;
	virtual bool GetIsMove()=0;

	virtual void SetTableIndex(int nIndex)=0;   ///<�������ÿؼ��Ľ���˳��
	virtual int  GetTableIndex()=0;
	virtual IBCInterFace* GetParentControl()=0;

	virtual void SetHaveShrinkArea(bool bFlag)=0;
};

/// ͼ��ؼ��ӿ�
interface IImage : virtual public IBCInterFace
{
public:
	virtual wchar_t * GetPath()=0;                   ///<�ؼ�������ͼƬ·��
	virtual void  SetPath(wchar_t * pchar)=0;         ///<���ÿؼ�����ͼƬ·��
	///��ȡIMG��Ϣ,��ȡ���ص�ͼƬ���
	virtual int GetPicWidth()=0;
	///��ȡͼƬ�߶�
	virtual int GetPicHeight()=0;

	/// ����ͼƬ
	virtual int LoadPic(wchar_t *szFileName)=0;
	/// ����ͼƬ��ʾ�ķ���
	virtual void SetDir(int degree, POINT pt)=0;
	virtual void SetSrcPos(int x,int y,int w,int h)=0;

	/// ����ͼƬ�ĸ��������ɫ�������ϣ����ϣ����¡����µĴ���
	virtual void SetColor(DWORD c1, DWORD c2, DWORD c3, DWORD c4)=0;

	virtual int GetSrcX()=0;///<��ȡ��ͼ��ԭ�����е�λ��
	virtual int GetSrcY()=0;///<��ȡ��ͼ��ԭ�����е�λ��
	virtual int GetSrcW()=0;///<��ȡ��ͼ��ԭ�����е�λ��
	virtual int GetSrcH()=0;///<��ȡ��ͼ��ԭ�����е�λ��
	virtual void SetSrcX(int x)=0; ///<���ô�ͼ��ԭ�����е�λ��
	virtual void SetSrcY(int y)=0; ///<���ô�ͼ��ԭ�����е�λ��
	virtual void SetSrcW(int w)=0; ///<���ô�ͼ��ԭ�����е�λ��
	virtual void SetSrcH(int h)=0; ///<���ô�ͼ��ԭ�����е�λ��
	
	virtual int GetShowX()=0;
	virtual int GetShowY()=0;
	virtual int GetShowW()=0;
	virtual int GetShowH()=0;

	virtual IBCInterFace * Clone(int nId)=0;

    /** @brief �����Ƿ����Ҿ�����ʾ
     *	@param[in]      bCrossMirror
     *	@return ��
     */
    virtual void SetCrossMirror(bool bCrossMirror)=0;

};

/// ��ť�ؼ��ӿ�
interface  IButton : virtual public IImage
{
public:
	///���еĺ��� ���û��ӿ�
	virtual void SetEnable(bool flag)=0;                ///<���ÿؼ��Ƿ����
	virtual bool GetEnable()=0;                         ///<��ȡ�ؼ��Ƿ����

	virtual void SetText(wchar_t *pchar)=0;				///<��������ڰ�ť��ʾ��̬����
	virtual wchar_t* GetText()=0;						///<����ڰ�ť��ʾ��̬����

	virtual void SetFontColor(COLORREF crFontColor)=0;	///<���ÿؼ�������ʾ��ɫ
	virtual COLORREF GetFontColor()=0;					///<��ȡ�ؼ�������ʾ��ɫ

	virtual void SetFontStyle(wchar_t * pFontStyle)=0;  ///<���ÿؼ�������ʽ
	virtual wchar_t * GetFontStyle()=0;                 ///<��ȡ�ؼ�������ʽ

	virtual void SetFontSize(int fontsize)=0;			///<���ÿؼ������С
	virtual int GetFontSize()=0;						///<��ȡ�ؼ������С

	virtual void SetFontShowStyle(FONTSHOWSTYLE showStyle)=0;   ///<���ÿؼ�������ʾģʽ�����󣬾��У�����
	virtual FONTSHOWSTYLE GetFontShowStyle()=0;					///<��ȡ�ؼ�������ʾģʽ�����󣬾��У�����

	virtual void SetItalicFlag(bool bFlag)=0;       ///<�����Ƿ���б��
	virtual bool GetItalicFlag()=0;                 ///<��ȡ�����Ƿ���б��

	virtual void SetBoldFlag(bool bFlag)=0;         ///<�����Ƿ��Ǵ���
	virtual bool GetBoldFlag()=0;                   ///<��ȡ�Ƿ��Ǵ���

	virtual void SetTextTop(int px) = 0;			///<�����ı��Ĵ�ֱλ��
	virtual int GetTextTop() = 0;					///<��ȡ�ı��Ĵ�ֱλ��

	virtual IBCInterFace * Clone(int nId)=0;
};

///�ı��ؼ�
interface  IText: virtual public IBCInterFace
{
public:
	virtual wchar_t * GetText()=0;                      ///<��ȡ�ؼ���ʾ����
	virtual void SetText(wchar_t * pText)=0;///<���ÿؼ���ʾ����
	virtual void SetFontColor(COLORREF crFontColor)=0; ///<���ÿؼ�������ʾ��ɫ

	virtual wchar_t * GetFontStyle()=0;                 ///<��ȡ�ؼ�������ʽ
	virtual bool GetMutiShow()=0;                       ///<��ȡ�Ƿ������ʾ��־
	virtual int GetFontSize()=0;                    ///<��ȡ�ؼ������С
	virtual COLORREF GetFontColor()=0;               ///<��ȡ�ؼ�������ʾ��ɫ
	virtual FONTSHOWSTYLE   GetFontShowStyle()=0;             ///<��ȡ�ؼ�������ʾģʽ�����󣬾��У�����
	virtual FONTSHOWDIRECTION   GetFontShowDirection()=0;         ///<��ȡ�ؼ�������ʾ���򣬺�������
	
	virtual void SetFontStyle(wchar_t * pFontStyle)=0;  ///<���ÿؼ�������ʽ
	virtual void SetFontSize(int fontsize)=0;       ///<���ÿؼ������С
	virtual void SetMutiShow(bool bFlag)=0;             ///<�����Ƿ������ʾ
	
	virtual void SetFontShowStyle(FONTSHOWSTYLE showStyle)=0;   ///<���ÿؼ�������ʾģʽ�����󣬾��У�����
	virtual void SetFontShowDirection(FONTSHOWDIRECTION showDirection)=0; ///<���ÿؼ�������ʾ���򣬺�������

	virtual void SetItalicFlag(bool bFlag)=0;                 ///<�����Ƿ���б��
	virtual bool GetItalicFlag()=0;                 ///<��ȡ�����Ƿ���б��
	virtual void SetBoldFlag(bool bFlag)=0;                   ///<�����Ƿ��Ǵ���
	virtual bool GetBoldFlag()=0;                   ///<��ȡ�Ƿ��Ǵ���
	virtual void SetUnderLineFlag(bool bFlag)=0;              ///<�����������»���
	virtual bool GetUnderLineFlag()=0;              ///<��ȡ�»��߱�־
	virtual void SetIsManyChar(bool bFlag)=0;                 ///<�����Ƿ��кܶ����
	virtual bool GetIsManyChar()=0;                 ///<��ȡ�������������Ƿ��кܶ������
	
	virtual IBCInterFace * Clone(int nId)=0;
    virtual void SetMouseThough(bool bThough)=0;    ///<�Ƿ���괩͸

	virtual bool GetIsShowOutline()=0;	///�Ƿ���ʾ��ߡ���LCF
	virtual COLORREF GetOutlineColor()=0;	///�����ɫ����LCF
	virtual void SetFontOutlineInfo(bool isOutline, COLORREF color) = 0;	 ///���������Ƿ���߼������ɫ����LCF
};
///�ı��༭��ӿ�
interface  IEditorText : virtual public IText
{
	virtual void SetRectColor(COLORREF crColor)=0;///<���ô�����ɫ
	virtual void SetFillColor(COLORREF crColor)=0;///<���ô����ɫ
	virtual void SetSorColor(COLORREF crColor)=0;///<���ù����ɫ

	virtual COLORREF GetRectColor()=0;///<��ȡ������ɫ
	virtual COLORREF GetFillColor()=0;///<��ȡ�����ɫ
	virtual COLORREF GetSorColor()=0;///<��ȡ�����ɫ

	virtual void SetIsPassWord(bool bFlag)=0; ///<�����Ƿ��������
	virtual bool GetIsPassWord()=0;
	virtual IBCInterFace * Clone(int nId)=0;
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

	virtual void SetCardWH(int w,int h)=0;               ///<�����ƿ��
	virtual int GetCardWH(int &w,int &h)=0;              ///<��ȡ�ƵĿ���
	virtual void SetCardPrama(int hSpace,int vSpace,int uSpace)=0; ///<�����ƺ�������������������߶�
	virtual int GetCarddPrama(int & hSpace,int &vSpace,int &uSpace)=0;///<��ȡ�ƻ�������

	virtual void SetCardDirLook(int dir)=0;///<��������ʾ�������ĸ�����
	virtual int GetCardDirLook()=0;        ///<��ȡ�Ƶ���ʾ����
	virtual void SetOwnerFlag(bool bFlag)=0 ;///<�����Ƿ����Լ������ƿؼ�
	virtual bool GetOwnerFlag()=0;           ///<��ȡ�Ƿ����Լ������ƿؼ���־
	
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

	virtual int SetItemHeight( int cyItemHeight)=0;       ///<���õ�����Ŀ�ĸ߶ȣ���ȺͿؼ����һ��
	virtual int GetItemHeight() const=0;                   ///<��ȡ������Ŀ�ĸ߶�

	virtual void SetBoldFlag(bool bFlag)=0;				///< �����Ƿ����
	virtual bool GetBoldFlag() const=0;						///< ��ȡ�Ƿ����
	virtual void SetItalicFlag(bool bFlag)=0;				///< �����Ƿ�б��
	virtual bool GetItalicFlag() const=0;					///< ��ȡ�Ƿ�б��

	virtual void SetFontStyle(wchar_t * pFontStyle)=0;  ///<���ÿؼ�������ʽ
	virtual void SetFontSize(int fontsize)=0;       ///<���ÿؼ������С

	virtual wchar_t * GetFontStyle()=0;             ///<��ȡ������ʽ
	virtual int  GetFontSize()=0;                  ///<��ȡ�����С
	virtual void SetButtonPicPath(wchar_t * pchar)=0;  ///<���ð�ŤͼƬ·��
	virtual void LoadButtonPic(wchar_t * pchar)=0;     ///<���ذ�ŤͼƬ
	virtual wchar_t * GetButtonPicPath()=0;            //<��ȡ��ŤͼƬ·��

	
	virtual void SetFrameColor(COLORREF crColor)=0; ///<���ÿؼ������ɫ
	virtual COLORREF GetFrameColor()=0;             ///<��ȡ�ؼ������ɫ

	virtual void SetEditorFrameColor(COLORREF crColor)=0; ///<���ñ༭�������ɫ
	virtual COLORREF GetEditorFramColor()=0;              ///<��ȡ�༭�������ɫ
	virtual void SetEditorFillColor(COLORREF crColor)=0;  ///<���ñ༭�����ɫ
	virtual COLORREF GetEditorFillColor()=0;              ///<��ȡ�༭�����ɫ
	
	virtual void SetBkRectFillColor(COLORREF crColor)=0;           ///<������������ɫ
	virtual COLORREF GetBkRectFillColor()=0;                       ///<��ȡ��������ɫ
	virtual void SetBkRectColor(COLORREF crColor)=0;               ///<�����������߿���ɫ
	virtual COLORREF GetBkRectColor()=0;                           ///<��ȡ�������߿���ɫ
	virtual void SetMoveRectFillColor(COLORREF crColor)=0;         ///<��������ƶ�ѡ��ʱ����ɫ
	virtual COLORREF GetMoveRectFillColor()=0;                     ///<��ȡ����ƶ�ѡ��ʱ����ɫ
	virtual void SetSorColor(COLORREF crColor)=0;                  ///<���ù����ɫ
	virtual COLORREF GetSorColor()=0;                              ///<��ȡ�����ɫ

	virtual void SetFontColor(COLORREF crColor)=0;                 ///<����������ɫ
	virtual COLORREF GetFontColor()=0;                             ///<��ȡ������ɫ

	virtual IBCInterFace * Clone(int nId)=0;

    virtual void SetEditable(bool bCanEdit)=0;

	virtual void SetFontSelectColor(DWORD color) = 0;					///��������ѡ����ɫ
	virtual DWORD GetFontSelectColor() = 0;								///��ȡ����ѡ����ɫ
	virtual bool GetIsShowOutline()=0;	///�Ƿ���ʾ��ߡ���LCF
	virtual COLORREF GetOutlineColor()=0;	///�����ɫ����LCF
	virtual void SetFontOutlineInfo(bool isOutline, COLORREF color) = 0;	 ///���������Ƿ���߼������ɫ����LCF
};

interface  IInputUserName: virtual public ICombox
{
public:
	virtual void SetDeleteButtonPic(wchar_t * pchar)=0;
	virtual wchar_t * GetDeleteButtonPicPath()=0;
	virtual void SetDeleteButtonPicPos(int x,int y,int w,int h)=0;
	virtual void SetDeleteButtonW(int nw)=0;
	virtual int GetDeleteButtonW()=0;

    ///���ع�����ͼƬ
    virtual void LoadScrollBkImage(wchar_t * pchar)=0;
    virtual void LoadScrollTButton(wchar_t * pchar)=0;
    virtual void LoadScrollBButton(wchar_t * pchar)=0;
    virtual void LoadScrollBar(wchar_t * pchar)=0;
    virtual wchar_t * GetScrollBkPath()=0;
    virtual wchar_t * GetScrollTButtonPath()=0;
    virtual wchar_t * GetScrollBButtonPath()=0;
    virtual wchar_t * GetScrollBarPath()=0;
	virtual void SetFontSelectColor(DWORD color) = 0;					///��������ѡ����ɫ
	virtual DWORD GetFontSelectColor() = 0;								///��ȡ����ѡ����ɫ
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
    virtual void GetNoPagiID(int nIDs[], int nSize)=0;          ///< ��ȡ����ҳ��ID
    virtual void SetNoPagecontentID(int nIDs[], int nSize)=0;

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
};

interface  IContain: virtual public IBCInterFace
{
public:
	virtual void SetBKImagePath(wchar_t * pchar)=0;           ///<����������ͼ·��
	virtual void LoadBkImage(wchar_t * pchar)=0;              ///<����������ͼ·��
	virtual wchar_t * GetBkImagePath()=0;                     ///<��ȡ������ͼ·��	
	virtual void MoveControl(int DistanceX,int DistanceY)=0;///<�ƶ����������е����пؼ�ҲҪһ���ƶ�
	virtual int GetControlSize()=0;  ///<�����ؼ�����
	virtual int GetControlBlongContainID(int id)=0;  ///<��ȡָ��ID���ϲ������ؼ�ID
	virtual void IInsertControl(IBCInterFace * bControl)=0;///<�򴰿�����ӿؼ�
	virtual IBCInterFace *IGetControlbyIndex(int listindex, bool &bVisable)=0;///<����list����ָ����ŵĶ���ָ�룬����ʾ����
	virtual void SetChildVisable(IBCInterFace *pChild, bool bVisable)=0;
	virtual void IDeleteControl(IBCInterFace * pcontrol)=0;    ///< �Ƴ�ĳ���ؼ�
    /** @brief ��ĳ���ؼ��Ӹ������ƶ�����һ��������
     *  @param[in]          pcontrol        ���ƶ��Ŀؼ�
     *  @param[in]          pOther          Ŀ�����������Ϊ�գ����ƶ����κ������������û��Լ�����
     *  @return ���Ҫ�ƶ��Ŀؼ������ڴ������У��ͷ���true�����򷵻�false
     */
    virtual bool IMoveControl(IBCInterFace * pcontrol, IContain *pOtherContain = NULL)=0;
	virtual IBCInterFace * Clone(int nId)=0;
	virtual bool GetControlShowFlag(int nIndex)=0;   ///<�ؼ��Ƿ�����������ʾ��ʾ����Ϊ��Щ�ؼ�����Ҫ��ʾ����Ҫ�ض�ʱ�ڲ���ʾ
    virtual bool GetControlShowFlag(IBCInterFace *pBcInterface)=0;

	virtual void SortAllSubControlByZord()=0;  ///<����ȫ������
	virtual void SetAllZorder(INT16 zorder)=0;///<���ÿؼ���� �����ͽ������е����в�ζ���Ӧ�����޸ģ��Ա�༭ editor by 2010.11.12

    virtual IContainPaginator *GetContainPaginator() = 0;   ///<��ȡ��ҳ����

	//virtual void SetControlShowFlag(BControlInterface * pBcInterface,bool bFlag)=0;
};

////////////////////////////////////////////////////////////////////////////////



///<��̬����
interface ICartoon:virtual public IBCInterFace
{
public:
	virtual void SetFrameNum(byte framenum)=0;///<���ò���֡��
	//virtual void SetVisble(bool bflag)=0;///<�����Ƿ���ʾ����
	virtual void SetSpeed(int speed)=0;  ///<���ö����ٶ�
	virtual void SetState(PLAYSTATE bstate)=0; ///<���ö���״̬
	virtual void SetPlayNum(int iplaynum)=0; ///<���ò��Ŵ���
	virtual byte GetFrameNum()=0;             ///<��ȡ֡��
	//virtual bool GetVisble()=0;              ///<��ȡ�ؼ��Ƿ���ʾ
	virtual int GetSpeed()=0;                ///<��ȡ�����ٶ�
	virtual PLAYSTATE GetState()=0;                ///<��ȡ����״̬
	virtual int GetPlayNum()=0;             ///<��ȡ���Ŵ���
	virtual byte GetCurrentFrame()=0;  ///<��ȡ��ǰ���ŵ�֡����
	virtual bool SetImagePath(int frameindex,wchar_t * path)=0;    ///<������Ӧ֡��·��
	virtual bool LoadPic(int frameindex,wchar_t * path)=0;         ///<��������֡������m_vImage
	virtual void SetPicABSXY(int frameindex,int x,int y,int w,int h)=0;     ///<����ÿ֡������͸߿�
	virtual wchar_t * GetPicPath(int frameindex)=0;///<��ȡÿ֡·��

	virtual IBCInterFace * Clone(int nId)=0;
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
	virtual void LoadBkImage(wchar_t * pchar)=0;            ///<���ص�ͼ
 	virtual void SetBkPath(wchar_t * pchar)=0;              ///<���õ�ͼ·��
	virtual void LoadProgressImage(wchar_t * pchar)=0;      ///<���ؽ�����ʾ
	virtual void SetProgressImage(wchar_t * pchar)=0;       ///<���ý�����ʾ·��
	virtual wchar_t * GetBkImagePath()=0;                   ///<��ȡ��ͼ·��
	virtual wchar_t * GetProgressImagePath()=0;             ///<��ȡ��������ʾ·��
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
	virtual void LoadBkImage(wchar_t * pchar)=0;      ///<���ص�ͼ
	virtual wchar_t * GetBkImagePath()=0;                 ///<��ȡ��ͼ·��
	virtual void LoadTButton(wchar_t * pchar)=0;      ///<���ض��˰�Ť��ͼ
	virtual wchar_t * GetTButtonPath()=0;                 ///<��ȡ���˰�ŤͼƬ·��
	virtual void LoadBButton(wchar_t * pchar)=0;      ///<���ص׶˰�ŤͼƬ
	virtual wchar_t * GetBButtonPath()=0;                 ///<��ȡ�׶˰�ŤͼƬ
	virtual void LoadScrollButton(wchar_t * pchar)=0; ///<�����м���ֵ�ͼƬ
	virtual wchar_t * GetScrollButtonPath()=0;            ///<��ȡ�м���ֵ�ͼƬ·��
	virtual void SetBkImagePath(wchar_t * pchar)=0;///<���õ�ͼ·��
	virtual void SetTButtonPath(wchar_t * pchar)=0;///<���õ�ͼ·��
	virtual void SetBButtonPath(wchar_t * pchar)=0;///<���õ�ͼ·��
	virtual void SetScrollButtonPath(wchar_t * pchar)=0;///<���õ�ͼ·��
	virtual void SetVerticalFlag(bool flag)=0;         ///<�����Ǻ���������������������
	virtual bool GetVerticalFlag()=0;                  ///<��ȡ�����������־
	virtual IBCInterFace * Clone(int nId)=0;
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

	virtual void SetPicPath(int nIndex,wchar_t * pchar)=0;            ///<���ö���ͼƬ·��
	//virtual void LoadPic(wchar_t * pchar)=0;               ///<���ض���ͼƬ
	virtual wchar_t * GetPicPath(int nIndex)=0;                      ///<��ȡ����ͼƬ��·��

	virtual void SetMoveTotalTime(int atime)=0;                                  ///<�����ƶ�ʱ��
	virtual int GetMoveTotalTime()=0;                                   ///<��ȡ�ƶ�ʱ��
	virtual void SetLineKind(bool flag)=0;                      ///<�����Ƿ���ֱ������
	virtual bool GetLineKind()=0;                               ///<��ȡ�Ƿ���ֱ������

	virtual void SetMoveFlag(bool bFlag)=0;  ///<�����Ƿ����ƶ�����
	virtual bool GetMoveFlag()=0;  ///<��ȡ�Ƿ����ƶ�����

	virtual void SetFrameNum(int nNum)=0;       ///<���ö���֡��
	virtual int GetFrameNum()=0;              ///<��ȡ����֡��

    virtual void SetPlayTotalTime(int atime)=0;      ///<���ò�����ʱ��
	virtual int GetPlayTotalTime()=0;

	virtual void ClearAllPath()=0;        ///<������������ͼƬ·��
	virtual IBCInterFace * Clone(int nId)=0;
};

interface IWeb:virtual public IBCInterFace, virtual public IManual
{
public:
	///<��������
	virtual void SetShowWebAddress(wchar_t * pchar)=0;
    /// ʹ�ô���
    /// ʹ�ô��ڼ�����ҳ�ؼ����Ӵ��ڵ���ʽ������֧�ָ����������
    virtual void SetUseWindow(bool bUse = true) = 0;
};



interface IListControl:virtual public IBCInterFace
{
public:
	///<��������
	virtual void SetRowHeight(int nHeight)=0;                  ///<����ÿ�еĸ߶�
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
	
	virtual void InsertColumn(int nIndex,wchar_t * pText,int nWidth,int nHeight)=0;   ///<�����ͷ
	virtual void InsertInItem(int nRow,int nCol,wchar_t * pText)=0;                   ///<����ڵ�
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

	virtual void Clear()=0;               ///<��������У�������ձ�ͷ

	virtual void LoadVerScrollBkImage(wchar_t * path)=0;
	virtual wchar_t * GetVerScrollBkImage()=0;
	virtual void LoadVerScrollTImage(wchar_t * path)=0;
	virtual wchar_t * GetVerScrollTImage()=0;
	virtual void LoadVerScrollBImage(wchar_t * path)=0;
	virtual wchar_t * GetVerScrollBImage()=0;
	virtual void LoadVerScrollBarImage(wchar_t * path)=0;
	virtual wchar_t * GetVerScrollBarImage()=0;
	//���
	virtual void LoadHorizScrollBkImage(wchar_t * path)=0;
	virtual wchar_t * GetHorizScrollBkImage()=0;
	virtual void LoadHorizScrollTImage(wchar_t * path)=0;
	virtual wchar_t * GetHorizScrollTImage()=0;
	virtual void LoadHorizScrollBImage(wchar_t * path)=0;
	virtual wchar_t * GetHorizScrollBImage()=0;
	virtual void LoadHorizScrollBarImage(wchar_t * path)=0;
	virtual wchar_t * GetHorizScrollBarImage()=0;

	virtual void SetHeadObjHeight(int nH)=0;  ///<���ñ�ͷ����ĸ�
	virtual int GetHeadObjHeight()=0;  ///<��ȡ��ͷ����ĸ߶�

	virtual void SetSelectRectColor(COLORREF crSelect)=0;            ///<����ѡ���е���ɫ
	virtual COLORREF GetSelectRectColor()=0; ///<����ѡ���е���ɫ
	virtual IBCInterFace * Clone(int nId)=0;
	virtual bool GetIsShowOutline()=0;	///�Ƿ���ʾ��ߡ���LCF
	virtual COLORREF GetOutlineColor()=0;	///�����ɫ����LCF
	virtual void SetFontOutlineInfo(bool isOutline, COLORREF color) = 0;	 ///���������Ƿ���߼������ɫ����LCF
};

interface ITalk:virtual public IBCInterFace
{
	virtual  void SetText(wchar_t *  wchar)=0; ///<������ʾ������

	virtual void LoadScrollBkImage(wchar_t * pchar)=0;
	virtual void LoadScrollTButton(wchar_t * pchar)=0;
	virtual void LoadScrollBButton(wchar_t * pchar)=0;
	virtual void LoadScrollBar(wchar_t * pchar)=0;

	virtual wchar_t * GetScrollBkPath()=0;
	virtual wchar_t * GetScrollTButtonPath()=0;
	virtual wchar_t * GetScrollBButtonPath()=0;
	virtual wchar_t * GetScrollBarPath()=0;

	virtual void LoadBkPath(wchar_t * pchar)=0;
	virtual wchar_t * GetBkPath()=0;
	virtual IBCInterFace * Clone(int nId)=0;

	virtual void SetFaceImagePath(wchar_t * pchar)=0;  ///<���ر���ͼƬ
	virtual wchar_t * GetFaceImagePath()=0;
};


interface IBeacon:virtual public IBCInterFace
{

};

interface IRadioButton:virtual public IBCInterFace
{
public:
	virtual void SetBkImagePath(wchar_t * pchar)=0;          ///<���ص�ͼ·��
	virtual void LoadBkImage(wchar_t * pchar)=0;             ///<���ص�ͼ
	virtual void SetSelectPath(wchar_t * pchar)=0;           ///<��¼ѡ����·��
	virtual void LoadSelectImage(wchar_t * pchar)=0;         ///<����ѡ����ͼƬ
	virtual wchar_t * GetBkImage()=0;                        ///<��ȡ��ͼ·��
	virtual wchar_t * GetSelectImage()=0;                    ///<��ȡѡ���·��

	virtual void SetText(wchar_t * pchar)=0;                 ///<������ʾ�ַ�
	virtual wchar_t * GetText()=0;                           ///<��ȡ��ʾ�ַ�

	virtual void SetTextFontStyle(wchar_t * pchar)=0;        ///<������ʾ��������
	virtual wchar_t  * GetTextFontStyle()=0;                 ///<��ȡ��ʾ��������

	virtual void SetTextFontSize(int nSize)=0;               ///<������ʾ�����С
	virtual int GetTextFontSize()=0;                         ///<��ȡ��ʾ�����С

	virtual void SetTextFontColor(COLORREF crColor)=0;       ///<����������ɫ
	virtual COLORREF GetTextFontColor()=0;                   ///<��ȡ������ɫ
	
	virtual void SetBkWith(int w)=0;                         ///<���õ�ͼ���
	virtual int GetBkWith()=0;                               ///<��ȡ��ͼ���

	virtual bool GetIsSelect()=0;                             ///<��ȡ�Ƿ���ѡ����
	virtual IBCInterFace * Clone(int nId)=0;
};

interface ICardBase:virtual public IBCInterFace
{
	virtual void SetCardWH(int nW,int nH)=0;  ///<�����ƵĿ��
	virtual void SetCardHSpace(int nSpace)=0;  ///<�����Ƶ�ˮƽ���
	virtual void GetCardWH(int & nW,int & nH)=0; ///<��ȡ�ƵĿ��
	virtual int GetCardHSpace()=0;           ///<��ȡ�Ƶ�ˮƽ���
	virtual void SetCardFlushStyle(int nIndex)=0; ///<��������ʽ
	virtual int GetCardFlushStyle()=0;        ///<��ȡ����ʽ
	virtual void SetCardList(BYTE BList[] ,int nNum)=0;     ///<������ʾ����
	virtual void SetCardPic(BYTE nIndex,wchar_t * pchar)=0;  ///<�����Ƶ���Դ.˳����ؽ�ȥ
	virtual int GetCardList(BYTE BCardList[])=0;             ///<��ȡ��ʾ���� 
	virtual wchar_t * GetCardPic(int nIndex)=0;              ///<��ȡ���ʵ�·��
	virtual int GetCardNum()=0;                          ///<��ȡ�Ƶ�����

	virtual void SetCardAlignStyle(BYTE bStyle)=0;    ///<���ö���ģʽ 0-2  0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ���
	virtual BYTE GetCardAlignStyle()=0;
	virtual IBCInterFace * Clone(int nId)=0;

	//virtual ENM_CARD_NUMBER GetCardPointByIndex(int nIndex)=0;           ///<��ȡָ���������Ƶĵ���
	//virtual ENM_CARD_SHAPE GetCardFigureByIndex(int nIndex)=0;          ///<��ȡָ���������ƵĻ�ɫ
	//virtual void RemoveCard(BYTE BCardList[],int nNum)=0;                   ///<�Ƴ����е���
};

interface INoMoveCard:virtual public ICardBase
{
	virtual void SetIsShowCardNum(bool bFlag)=0;      ///<�����Ƿ���ʾʣ���Ƶ�����
	virtual bool GetIsShowCardNum()=0;                ///<��ȡ�Ƿ���ʾ�Ƶ�����
	virtual void SetCardShowDir(int cardDir)=0;       ///<�����Ƶ���ʾ����
	virtual int GetCardShowDir()=0;                   ///<��ȡ�Ƶ���ʾ����
    virtual void SetCardShowMode(bool bVertical) = 0; ///<�Ƶİڷŷ�ʽ true: ����,  false:���
    virtual bool GetCardShowMode() = 0;               ///<��ȡ�Ƶİڷŷ�ʽ true: ����,  false:���
	virtual IBCInterFace * Clone(int nId)=0;

};

interface ILightCard:virtual public INoMoveCard
{
	virtual void SetShowCardWH(int nW,int nH)=0;
	virtual void GetShowCardWH(int & nW,int & nH)=0;
	virtual void SetShowCardRowNum(int nNum)=0;
	virtual int  GetShowCardRowNum()=0;
	virtual void SetRowSpace(int nSpace)=0;
	virtual int  GetRowSpace()=0;
	virtual IBCInterFace * Clone(int nId)=0;
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
    virtual bool GetHaveSymbol()=0;
	virtual void SetImagePath(wchar_t * pchar)=0;
	virtual wchar_t * GetImagePath()=0;
};
interface ITime:virtual public IBCInterFace
{
	virtual void SetImageNumPath(wchar_t * pchar)=0; ///<��������ͼƬ
	virtual wchar_t * GetImageNumPath()=0;     ///<��ȡ����ͼƬ
	virtual void SetBkImagePath(wchar_t * pchar)=0;   ///<���õ�ɫͼƬ
	virtual wchar_t * GetBkImagePath()=0;            ///<��ȡ��ɫͼƬ
	virtual void SetShowMaxNum(int nNum)=0;         ///<������ʾ���ʱ��
	virtual int  GetShowMaxNum()=0;                 ///<��ȡ��ʾ���ʱ��
	virtual void SetShowMinNum(int nNum)=0;        ///<������ʾ��Сʱ�� 
	virtual int  GetShowMinNum()=0;              ///<��ȡ��ʾ��Сʱ��
	virtual void SetShowAllTime(DWORD dtTime)=0; ///<������ʾ��ʱ��
	virtual DWORD GetShowAllTime()=0;    ///<��ȡ��ʾ��ʱ��
	virtual int  GetCurrentTimeNum()=0;     ///<��ȡ��ǰʱ��
	virtual void SetPLay(bool bFlag)=0;
	virtual bool GetPLay()=0;
};
interface IOperateCard:virtual public ICardBase
{
	virtual void SetUpSpace(int nSpace)=0; ///<����������ĸ߶�
	virtual int  GetUpSpace()=0;           ///<��ȡ������ĸ߶�

	virtual void SetSelectImagePath(wchar_t * pchar)=0; ///<����ѡ���ǵ�ͼƬ
	virtual wchar_t * GetSelectImagePath()=0;           ///<��ȡѡ���ǵ�ͼƬ·��
	virtual IBCInterFace * Clone(int nId)=0;
};
interface ISlider:virtual public IBCInterFace
{
	virtual void SetFillPath(wchar_t * pchar)=0;
	virtual wchar_t * GetFillPath()=0;
	virtual void SetButtonSliderPath(wchar_t * pchar)=0;
	virtual wchar_t * GetButtonSliderPath()=0;
	virtual void SetSliderStyle(BYTE bStyle)=0;
	virtual BYTE GetSliderStyle()=0;
	virtual IBCInterFace * Clone(int nId)=0;
};
interface IMenu:virtual public IBCInterFace
{
	virtual void SetItemHeight(int nH)=0;
	virtual int GetItemHeight()=0;
	virtual void SetLineColor(COLORREF crColor)=0;
	virtual COLORREF GetLineColor()=0;

	virtual void SetBKImagePath(wchar_t * pchar)=0;;
	virtual wchar_t * GetBkImagePath()=0;

	virtual void SetMoveColor(COLORREF crColor)=0;
	virtual COLORREF GetMoveColor()=0;

	virtual void SetMoveFramColor(COLORREF crColor)=0;
	virtual COLORREF GetMoveFramColor()=0;

	virtual void SetFontStyle(wchar_t * pchar)=0;
	virtual wchar_t * GetFontStyle()=0;                 ///<��ȡ�ؼ�������ʽ
	virtual void SetFontSize(int nSize)=0;
	virtual int GetFontSize()=0;                    ///<��ȡ�ؼ������С
	virtual void SetFontColor(COLORREF crColor)=0;
	virtual COLORREF GetFontColor()=0;               ///<��ȡ�ؼ�������ʾ��ɫ
	virtual void SetFontShowStyle(FONTSHOWSTYLE style)=0;
	virtual FONTSHOWSTYLE   GetFontShowStyle()=0;;             ///<��ȡ�ؼ�������ʾģʽ�����󣬾��У�����
	virtual bool GetIsShowOutline()=0;	///�Ƿ���ʾ��ߡ���LCF
	virtual COLORREF GetOutlineColor()=0;	///�����ɫ����LCF
	virtual void SetFontOutlineInfo(bool isOutline, COLORREF color) = 0;	 ///���������Ƿ���߼������ɫ����LCF
};

struct SudokuInfo_1;
interface ISudoku:virtual public IBCInterFace
{	
    virtual SudokuInfo_1 *GetSudokuAttribute()=0;                ///< ��ȡsudoku�ؼ�����
    virtual int UpdateAttribute()=0;                             ///< ����sudoku�ؼ�����
	virtual int SetPicByIndex(int nIdx,wchar_t *szFileName,RECT& rc) = 0; ///<���þŹ���ͼ
};

interface ISiTuanOperateCard:virtual public IOperateCard
{
	virtual void SetFoldSpace(int nSpace)=0; ///<�����ص��Ƶļ��
	virtual void SetThreeSpace(int nSpace)=0; ///<���������ƻ������Ƽ��
	virtual void SetFoldToOtherSpace(int nSpace)=0; ///<�����ص��Ƶ�������֮��ļ��
	virtual void SetThreeToOtherSpace(int nSpace)=0; ///<���������Ƶ�������֮��ļ��
	virtual void SetTwoToOtherSpace(int nSpace)=0; ///<���������Ƶ�������֮��ļ��
	virtual void SetThreeToThreeSpace(int nSpace)=0;        ///<����һ����֮��ļ��

	virtual int GetFoldSpace()=0; ///<�����ص��Ƶļ��
	virtual int GetThreeSpace()=0; ///<���������ƻ������Ƽ��
	virtual int GetFoldToOtherSpace()=0; ///<�����ص��Ƶ�������֮��ļ��
	virtual int GetThreeToOtherSpace()=0; ///<���������Ƶ�������֮��ļ��
	virtual int GetTwoToOtherSpace()=0; ///<���������Ƶ�������֮��ļ��
	virtual int GetThreeToThreeSpace()=0;        ///<����һ����֮��ļ��
	virtual void SetNumPic(wchar_t * Pchar)=0; ///<��������ͼƬ��·��
	virtual wchar_t * GetNumPic()=0;///<��ȡ����ͼƬ��·��
};
interface ISiTuanNomoveCard:virtual public INoMoveCard
{
	virtual void SetNumPic(wchar_t * pchar )=0; ///<�������ֿؼ���ͼƬ·��
	virtual wchar_t * GetNumPic()=0;            ///<��ȡ���ֿؼ���ͼƬ·��
};
interface ISiTuanLightCard:virtual public ILightCard
{
	virtual void SetNumPic(wchar_t * pchar)=0; ///<��������ͼƬ��·��
	virtual wchar_t *GetNumPic()=0;  ///<��ȡ����ͼƬ��·��
	virtual void SetFoldSpace(int nSpace)=0;
	virtual void SetFoldToOtherSpace(int nSpace)=0;
	virtual int GetFoldSpace()=0;
	virtual int GetFoldToOtherSpace()=0;
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

    virtual void SetPicPath(int nIndex,wchar_t * pchar)=0;            ///<���ö���ͼƬ·��(���·��)    
    virtual void SetAbsPicPath(int nIndex,wchar_t * pchar)=0;         ///<���ö���ͼƬ·��(����·��)  
    virtual wchar_t * GetPicPath(int nIndex)=0;                 // ��ȡ����ͼƬ��·��
    virtual void ClearAllPath()=0;                              // ������������ͼƬ·��

    virtual void SetMoveTotalTime(int atime)=0;                 // �����ƶ�ʱ��
    virtual int GetMoveTotalTime()=0;                           // ��ȡ�ƶ�ʱ��


    virtual void SetFrameNum(int nNum)=0;                       // ���ö���֡��
    virtual int GetFrameNum()=0;                                // ��ȡ����֡��

    virtual void SetPlayTotalTime(int atime)=0;                 // ���ò�����ʱ��
    virtual int GetPlayTotalTime()=0;

    
    virtual IBCInterFace * Clone(int nId)=0;

    virtual const wchar_t * GetExtFilePath() = 0;
    virtual void SetExtFilePath(wchar_t *pwszFileName) = 0;
    virtual bool LoadFromFile() = 0;
    virtual bool WriteToFile() = 0;

    virtual void SetCurveInstance(CURVE_TYPE curType) = 0;      // ������������
    virtual CURVE_TYPE GetCurveType() = 0;                      // �����������
    virtual void SetIsSymmetrical(bool bIsSymmetrical) = 0;     // �Ƿ�Գ�
    virtual bool GetIsSymmetrical() = 0;                        // �Ƿ�Գ�
 

    virtual void SetUseMouse(bool bFlag) = 0;

    virtual void SetShowCurveLine(bool bFlag) = 0;   
    virtual bool GetIsShowCurveLine() = 0;

    virtual void SetIsLoop(bool bIsLoop = true) = 0;            // �Ƿ�ѭ������
    virtual bool GetIsLoop() = 0;                               // �Ƿ�ѭ������
    virtual void SetShowFrame(int nCount) = 0;                  // ��ʾĳһ֡

    /** @brief �����Ƿ����Ҿ�����ʾ
     *	@param[in]      bCrossMirror
     *	@return ��
     */
    virtual void SetCrossMirror(bool bCrossMirror) = 0;
};


////////////////////////////////////////////////////////////////////////////////
// �齫�ؼ��ӿ�begin{{

const int MAX_TILES_INCTRL = 200;

const int MAX_GOD_TILE = 8;
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
    DWORD           dwReserve2; /// ��ռ��λ    0λ��0:��ͨ��    ��1:������

    MJ_TILE_UIINFO()
    {
        memset(this, 0, sizeof(MJ_TILE_UIINFO));
    }
};


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
     * @brief �����齫ͼƬ��Դ
     * Ŀǰֻ��һ��
     */
    virtual void SetMjPic(const wchar_t * pchar) = 0;
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
    virtual bool GetMjPic(wchar_t *pszPicpath, int nSrcSize) = 0;
    virtual int GetMjTileWidth() = 0;
    virtual int GetMjTileHeight() = 0;
    virtual bool GetIsUseMouse() = 0;
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

    /// ��Ӧ��get����
    virtual int GetTileCount() = 0;
    virtual bool GetShPic(wchar_t *pszPicpath, int nSrcSize) = 0;
    virtual int GetShTileWidth() = 0;
    virtual int GetShTileHeight() = 0;
    virtual bool GetIsUseMouse() = 0;
	virtual int GetShNote()=0;//�����ʾ�������ֵ
	virtual int GetShNoteType()=0;//��ó���ڷŷ�ʽ

    /** @brief �л�������ʾ��ʽ
     *  @param[in]      nTrimNote       ��Ϊ0��Ϊ����ģʽ
     */
    virtual void SetTrimNoteMode(int nTrimNote)=0;    
    /** @brief ��ȡ���뷽ʽ
     */
    virtual int GetTrimNote()=0;
};
/////////////////////////////////////////////����ؼ�

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
};


interface IMjBlockCtrl : virtual public IMjCtrl
{
    // �ӿڷ������ã���IMjHandCtrl�ϲ���
public:
    /**
     * @brief ����һ��������Ϣ
     */
    virtual void AddOneBlock(MJ_LOGIC_TYPE type, BYTE ucTiles[4], BYTE ucChair) = 0;
    /**
     * @brief �������������ʾ
     */
    virtual void ClearAllDis() = 0;
    /**
    * @brief ������ͼ��λ
    */
    virtual void SetViewDirection(BYTE nViewStation) = 0;

    /**
    * @brief �õ���λ
    */
    virtual BYTE GetViewDirection() = 0;
    /**
     * @brief �õ�����������Ϣ
     */
    virtual void GetAllBlockInfo(MJ_LOGIC_TYPE mtType[], BYTE ucTiles[][4], BYTE ucChairs[], BYTE size) = 0;
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
///�ɷ�ҳ������
//interface  IPagContain: virtual public IContain
//{
//public:
//    virtual void IInsertControl(IBCInterFace * bControl)=0;     ///< �򴰿�����ӿؼ�    
//    virtual void IDeleteControl(IBCInterFace * pcontrol)=0;     ///< �Ƴ�ĳ���ؼ�
//    virtual bool SetNumPerPage(BYTE nNum) = 0;                  ///< ���ô�����һҳ�Ŷ���
//    virtual bool NextPage() = 0;                                ///< ���·�ҳ
//    virtual bool PrevPage() = 0;                                ///< ���Ϸ�ҳ
//    virtual void FirstPage() = 0;                               ///< ��һҳ
//    virtual void LastPage() = 0;                                ///< ���һҳ
//};

/** @brief FLASH���ſؼ�
*/
interface IFlashPlay : virtual public IBCInterFace, virtual public IManual
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
    /** @brief �õ�FLASH�ļ���
     *	@return �ļ���
     */
    virtual bool GetSwfFileName(wchar_t *pFileName, int nSize) = 0;
    /// ʹ�ô���
    /// ʹ�ô��ڼ�����ҳ�ؼ����Ӵ��ڵ���ʽ������֧�ָ����������
    virtual void SetUseWindow(bool bUse = true) = 0;
};

/** @brief ����ƿؼ�
*  revolving scenic lamp
*/
interface IRSLamp : virtual public IText
{
public:
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
    /// ���õ�ͼ·��
    virtual void SetBKImage(wchar_t * pchar) = 0;
    /** @brief ���ı��ļ����������������
    *	@param[in]      *pwszFileName
    *	@return ��������0��Ҫ�ǲ�����0....
    */
    virtual int LoadContentFromFile(wchar_t *pwszFileName) = 0;
    /** @brief ��������֮��ļ��(ms)     
     */
    virtual void SetRowInterval(DWORD dwInterval) = 0;

    /** @brief Get����
     */
    virtual int GetPixelPerSec() = 0;
    virtual int GetRefreshTime() = 0;
    virtual int GetRevoDir() = 0;
    virtual DWORD GetRowInterval() = 0;
    /// ��ȡ������ͼ·��
    virtual wchar_t * GetBkImagePath() = 0;
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
	virtual int GetControlSize()=0;                              ///<��ȡ�ؼ�����
	virtual void SetFocousID(int id)=0;///<���û�ȡ����ID
	virtual int GetFocous()=0;              ///<��ȡ��ǰ��ȡ����ID
	virtual void SortControl()=0;    ///<���ؼ���Լ����������
	virtual void SortControlByID()=0;  ///<��ID˳��������
	virtual void IInsertControl(IBCInterFace * bControl)=0; ///<�򴰿�����ӿؼ�
	virtual void IDeleteControl(IBCInterFace * bControl)=0;      ///<ɾ�������еĿؼ�
	virtual void IRemoveControl(IBCInterFace * bControl)=0;      ///<�Ƴ������еĿؼ���ֻ���ڸö������Ƴ����ÿؼ�ʵ�ʻ�����
	virtual IBCInterFace * IGetControlByVectorIndex(int index)=0;///<��vector���±귵��ָ�ö���ָ��
	virtual IBCInterFace * Create(int index)=0;                   ///<���������ؼ�����
	virtual void InsertControlID(int nId)=0;          ///<��¼��ӵĿؼ�ID
	virtual void DeleteControlID(int nId)=0;          ///<ɾ����ӵĿؼ�ID
	virtual void DeleteOneControlId(int nId)=0;       ///<ɾ��һ���ؼ���ID�����������Ҳֻ����ID�б���ɾ���Լ��ģ�����ֻ���޸Ĳ��ʱ�õ��ˡ�
	virtual void SortControlByZord()=0;  ///<�����пؼ���ID���ؼ��Ĳ����������
	virtual void LButtonDown(POINT point)=0;       ///<����������
	virtual void LButtonUp(POINT point)=0;         ///<�ɿ�������
	virtual void MouseMove(POINT point)=0;         ///<����ƶ�
	virtual void RButtonDown(POINT point)=0;       ///<����Ҽ�����
	virtual void KeyDown(int nChar)=0;            ///<���̰����¼�
	virtual void KeyUp(int nChar)=0;            ///<���̰����¼�
	virtual void KeyChar(wchar_t nChar)=0;            ///<���������¼�
	virtual void OnSize()=0;                       ///<�����С�ı��¼�
	virtual int LoadData(wchar_t *szFileName)=0;
	virtual void SetParentHwnd(HWND hwnd)=0; ///<�������Դ��ڵľ��
	virtual int GetIDInBox()=0;
	virtual void SetIdInBox(int id)=0;

	virtual void FreeUI()=0;           ///<�ͷŽ���ؼ�


	virtual void SetGameBoxLeft(int x)=0;                     ///<���ñ༭�������
	virtual void SetGameBoxTop(int y)=0;                     ///<���ñ༭�������
	virtual void SetGameBoxWidth(int w)=0;                     ///<���ñ༭�������
	virtual void SetGameBoxHeight(int h)=0;                     ///<���ñ༭�������
	virtual int GetGameBoxLeft()=0;                     ///<���ñ༭�������
	virtual int GetGameBoxTop()=0;                     ///<���ñ༭�������
	virtual int GetGameBoxWidth()=0;                     ///<���ñ༭�������
	virtual int GetGameBoxHeight()=0;                     ///<���ñ༭�������

    /** @brief ���ļ������ӿؼ�
    *	@param[in]          nStartID             ��ʼID������ڴ��ж���ؼ��������ش�ID
    *	@return ���Ӻ��nStartIDֵ
    */
    virtual int AddControlFromFile(int nStartID, wchar_t *szFileName, IContain *pParent = NULL, CMDCallBack CallBack = NULL) = 0;

    /** @brief ���ļ������ӿؼ���ID�Ų��䣬�����ID���ظ��ˣ�������
    */
    virtual int AddControlFromFile(wchar_t *szFileName, IContain *pParent = NULL, CMDCallBack CallBack = NULL) = 0;
    /** @brief ȡ������ӿ�
     */
    virtual ICmdInterface *GetCmdInterface() = 0;
    /** @brief д�ļ�
     */
    virtual bool WriteFile(wchar_t * pFilePath) = 0;
    /** @brief д�ļ�
     */
    virtual bool WriteFile(wchar_t * pFilePath, IBCInterFace *pInterface) = 0;
    /** @brief ��ȡ���ؼ�
     */
    virtual IXUICursor *GetICursor() = 0;
    /** @brief ���ÿͻ������С
     */
    virtual void SetClientSize(SIZE size) = 0;
    /** @brief ��ȡ�ͻ������С
    */
    virtual SIZE GetClientSize() = 0;
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
    /** @brief ���ÿؼ�����
     */
    virtual void SetControlRotate(int nDegree)=0;
    /** @brief ���ú�����
     */
    virtual void SetHorInterval(int nInterval)=0;

public:
    /** @brief Getter����
     */
    //@{{
    virtual int GetVerInterval()=0;
    virtual int GetHorInterval()=0;
    virtual const wchar_t *GetImagePath()=0;
    virtual void GetCardList(BYTE ucCard[][MAX_VER_COUNT], int nSize, int &nReadSize)=0;
    virtual int GetVerCutCount()=0;
    virtual bool GetCanSelectCard()=0;
    virtual int GetControlRotate()=0;
    //@}}
};

/// ������ؼ��ӿ�
interface ITopPaper : virtual public IBCInterFace
{

public:
	///���еĺ��� ���û��ӿ�

	virtual int LoadBkPic(wchar_t *szFileName) = 0;   ///<ͨ��·�����ر���ͼ
	virtual wchar_t* GetBkPicPath() = 0;			  ///<��ȡ����ͼ��·��

	virtual void SetFontStyle(wchar_t * pFontStyle) = 0;  ///<���ÿؼ�������ʽ
	virtual wchar_t * GetFontStyle() = 0;                 ///<��ȡ�ؼ�������ʽ

	virtual void SetFontSize(int fontsize) = 0;		///<���ÿؼ������С
	virtual int GetFontSize() = 0;					///<��ȡ�ؼ������С

	virtual void SetLeanSide(int Side) = 0;			///���ÿؼ���ͣ�������� ���ң��ϣ���
	virtual int GetLeanSide() = 0;					///��ÿؼ���ͣ��������

	virtual void SetDefaultFontColor(COLORREF crFontColor) = 0; ///<���ÿؼ�Ĭ�ϵ�������ʾ��ɫ
	virtual COLORREF GetDefaultFontColor() = 0; ///<���û�ȡ�ؼ�Ĭ�ϵ�������ʾ��ɫ

	virtual void SetRowNums(int num) = 0;	///��������
	virtual int GetRowNums() = 0;			///�������

	virtual void SetColNums(int num) = 0;	///��������
	virtual int GetColNums() = 0;			///�������

	virtual void SetRowH(int px) = 0;	//�����и�
	virtual int GetRowH() = 0;			//��ȡ�и�

	virtual void SetRowSpace(int px) = 0;	///�����о�
	virtual int GetRowSpace() = 0;			///����о�

	virtual void SetColSpace(wchar_t* pColSpace) = 0;	///�����о�
	virtual wchar_t* GetColSpace() = 0;			///����о�

	virtual void SetColW(int px) = 0;	///�����п�
	virtual int GetColW() = 0;			///����п�

	virtual void SetTextBeginXY(int x,int y) = 0; //���õ�һ���ı����Ͻǵ�xy����
	virtual POINT GetTextBeginXY() = 0;		//��ȡ��һ���ı����Ͻǵ�xy����

	virtual void SetCornerSpace(int px) = 0;	//����¶���߽ǵľ���
	virtual int GetCornerSpace() = 0;	//���¶���߽ǵľ���

	virtual void SetRowColors(wchar_t* pColorDef) = 0;	///��������ɫ
	virtual wchar_t* GetRowColors() = 0;				///�������ɫ

	virtual void SetTestMode(bool bTest) = 0; //���ò���ģʽ
	virtual bool GetTestMode() = 0;  //���ÿؼ��Ƿ��ڲ���ģʽ

	virtual void SetPaperText(int nRow,int nCol,wchar_t* wszText) = 0; //���ð��ϵ��ı�
	virtual wchar_t* GetPaperText(int nRow,int nCol) = 0; //�õ����ϵ��ı�

	virtual void ClearAllText() = 0;	//���������ı�����
	
	virtual bool GetIsShowOutline()=0;	///�Ƿ���ʾ��ߡ���LCF
	virtual COLORREF GetOutlineColor()=0;	///�����ɫ����LCF
	virtual void SetFontOutlineInfo(bool isOutline, COLORREF color) = 0;	 ///���������Ƿ���߼������ɫ����LCF
};

#endif




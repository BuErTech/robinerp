#pragma once

#define  MAX_INCONTAIN_NUM    500                 ///<������������ɿؼ�����
#define  MAX_CARTOONFRAMNUM   50                  ///<�������֡��
using namespace std;

/// ��ǰ�汾��
//const double g_dbCurVersion = 8.3;
//const double g_dbCurVersion = 8.4;
//const double g_dbCurVersion = 8.5;      // ��datͷ�м����ʼ����Ϣ�����봰�ڴ�С�ı�
//const double g_dbCurVersion = 8.6;        // ���o�ؼ�����ı�������Ϣ
//const double g_dbCurVersion = 8.7;        // �û�����ؼ�����ѡ��������ɫ
//const double g_dbCurVersion = 8.8;        // ������ʾģʽ����
const double g_dbCurVersion = 8.9;        // �������

typedef struct _XUIStructInit
{
    int    nWndWidth;
    int    nWndHeight;
} XUIStructInit;

/// ����ʶ��汾�����ò�ͬ����
struct Type_8_1
{
};

	struct MyRect_t
	{
		WORD x;
		WORD y;
		WORD w;
		WORD h;
		MyRect_t(): x(0), y(0), w(0), h(0){}
		MyRect_t(int _x, int _y, int _w, int _h) : x(_x), y(_y), w(_w), h(_h)
		{
		}
	};
	///�����ļ���
	struct ControlCommonInfo 
	{
		int    Id;                    ///<�ؼ�ID
		wchar_t pName[MAX_PATH];                   ///<�ؼ�����
		byte    Mode;                  ///<�ؼ���ʾģʽ�����У����ң��ϣ��¡�������
		bool    Visible;               ///<�ؼ��Ƿ���ʾ
		int     X;                     ///<�ؼ���ʾ����
		int     Y;                     ///<�ؼ���ʾ����
		int    W;                     ///<�ؼ���ʾ����
		int    H;                     ///<�ؼ���ʾ����
		ControlKind   controlKind;                      ///<�ؼ�����-------���б��е�һ��
		int  Zorder;                            ///<�ؼ���ʾ���
		ControlCommonInfo()
		{
			ZeroMemory(pName,MAX_PATH);
		}
	};

	struct ControlCommonInfo4
	{
		int    Id;                    ///<�ؼ�ID
		wchar_t pName[MAX_PATH];                   ///<�ؼ�����
		byte    Mode;                  ///<�ؼ���ʾģʽ�����У����ң��ϣ��¡�������
		bool    Visible;               ///<�ؼ��Ƿ���ʾ
		int     X;                     ///<�ؼ���ʾ����
		int     Y;                     ///<�ؼ���ʾ����
		int    W;                     ///<�ؼ���ʾ����
		int    H;                     ///<�ؼ���ʾ����
		ControlKind   controlKind;                      ///<�ؼ�����-------���б��е�һ��
		int  Zorder;                            ///<�ؼ���ʾ���
		int  TableIndex;             ///<��������
		ControlCommonInfo4()
		{
			ZeroMemory(pName,MAX_PATH);
		}
				
	};
	
	///��Ť��ͼƬ�洢��Ϣ
	struct ImageButtonInfo //UI.dat8.5�����°汾
	{
		int   SrcX;                    ///<Ҫ���Ƶ�Դͼ�񲿷ֵ����Ͻǵ� x ����
		int   SrcY;					   ///<Ҫ���Ƶ�Դͼ�񲿷ֵ����Ͻǵ� y ����
		int   SrcW;                    ///<Ҫ���Ƶ�Դͼ�񲿷ֵĿ�
		int   SrcH;                    ///<Ҫ���Ƶ�Դͼ�񲿷ֵĸ�
		wchar_t  pPath[MAX_PATH];      ///<�ؼ�����ͼ�Ƶ�ַ

		ImageButtonInfo()
		{
			ZeroMemory(this, sizeof(ImageButtonInfo));
		}
	};

	struct ImageButtonInfo_8_6			//UI.dat8.6�汾
	{
		int   SrcX;                    ///<Ҫ���Ƶ�Դͼ�񲿷ֵ����Ͻǵ� x ����
		int   SrcY;					   ///<Ҫ���Ƶ�Դͼ�񲿷ֵ����Ͻǵ� y ����
		int   SrcW;                    ///<Ҫ���Ƶ�Դͼ�񲿷ֵĿ�
		int   SrcH;                    ///<Ҫ���Ƶ�Դͼ�񲿷ֵĸ�
		wchar_t  pPath[MAX_PATH];      ///<�ؼ�����ͼ�Ƶ�ַ

		wchar_t   Text[MAX_PATH];		///< Text����ʾ������
		wchar_t   FontStyle[MAX_PATH];	///<��������    ���壬��Բ������
		int		  nFontSize;			///<��ʾ����Ĵ�С
		COLORREF  crFontColor;			///<������ɫ
		FONTSHOWSTYLE bFontShowStyle;	///<����ʾ��ʽ��0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ���
		bool      bItalicFlag;          ///<�Ƿ���б��
		bool      bBoldFlag;            ///<�Ƿ�Ӵ�
		int       iTextTop;				///<�ı��Ĵ�ֱλ��

		ImageButtonInfo_8_6()
		{
			ZeroMemory(this, sizeof(ImageButtonInfo_8_6));
		}
	};

	struct CardSpeciaInfo
	{
		CardSpeciaInfo(){ZeroMemory(this, sizeof(CardSpeciaInfo));}
		int								CardHight;					///<�˿˸߶�
		int								CardWidth;					///<�˿˿��
		int								CardHSpace;					///<�˿˼��
		int                             CardVSpace;                 ///<�����Ƽ��
		int								CardUpSpace;				///<����߶�
		byte							DirLook;                 ///<�˿˷��÷���
		bool                            OwnerFlag;               ///<�Ƿ����Լ������ƿؼ�
		wchar_t                         path[2][MAX_PATH];           ///<���ڴ��8��IMAGE��·��
	};

	struct CartoonInfo 
	{
		CartoonInfo(){ZeroMemory(this, sizeof(CartoonInfo));}
		byte       bFrameNum;     ///<����֡��
		int       nSpeed;        ///<�����ٶ�
		int        iPlayNum;      ///<�������Ŵ���
		wchar_t      vPath[20][MAX_PATH];     ///<��������֡��·��
	};
     ///<������ṹ��ͬ����Ҫ�Ǳ�������֡���Ӵ���ǰΪ20֡�����ܲ����á�
	struct CartoonNewInfo 
	{
		CartoonNewInfo(){ZeroMemory(this, sizeof(CartoonNewInfo));}
		byte       bFrameNum;     ///<����֡��
		int       nSpeed;        ///<�����ٶ�
		int        iPlayNum;      ///<�������Ŵ���
		wchar_t      vPath[MAX_CARTOONFRAMNUM][MAX_PATH];     ///<��������֡��·��
	};

	struct TextInfo 
	{
		TextInfo(){ZeroMemory(this, sizeof(TextInfo));}
		wchar_t    Text[MAX_PATH];  ///< Text����ʾ������
		wchar_t    FontStyle[MAX_PATH]; ///<��������    ���壬��Բ������
		bool      bMutiShow;    ///<�Ƿ������ʾ
		int      nFontSize;///<��ʾ����Ĵ�С
		COLORREF  crFontColor; ///<������ɫ
		bool      bItalicFlag;             ///<�Ƿ���б��
		bool      bBoldFlag;               ///<�Ƿ�Ӵ�
		bool      bUnderLineFlag;          ///<�Ƿ����»���
		bool      bIsManyChar;             ///<�Ƿ���кܶ���������ĺ���
		//FONTSHOWSTYLE      bFontShowStyle; ///<����ʾ��ʽ��0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ���
		///FONTSHOWDIRECTION      bFontShowDirection; ///<������ʾ���� 0��ʾ����1��ʾ����
	};
	
#pragma pack(1)
	struct TextInfo88 
	{
		TextInfo88(){ZeroMemory(this, sizeof(TextInfo88));}
		wchar_t    Text[MAX_PATH];  ///< Text����ʾ������
		wchar_t    FontStyle[MAX_PATH]; ///<��������    ���壬��Բ������
		bool      bMutiShow;    ///<�Ƿ������ʾ
		int      nFontSize;///<��ʾ����Ĵ�С
		COLORREF  crFontColor; ///<������ɫ
		bool      bItalicFlag;             ///<�Ƿ���б��
		bool      bBoldFlag;               ///<�Ƿ�Ӵ�
		bool      bUnderLineFlag;          ///<�Ƿ����»���
		bool      bIsManyChar;             ///<�Ƿ���кܶ���������ĺ���
		FONTSHOWSTYLE      bFontShowStyle; ///<����ʾ��ʽ��0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ���
		///FONTSHOWDIRECTION      bFontShowDirection; ///<������ʾ���� 0��ʾ����1��ʾ����
	};
#pragma pack()

//����LCF
#pragma pack(1)
	struct TextInfo89 
	{
		TextInfo89(){ZeroMemory(this, sizeof(TextInfo89));}
		wchar_t    Text[MAX_PATH];  ///< Text����ʾ������
		wchar_t    FontStyle[MAX_PATH]; ///<��������    ���壬��Բ������
		bool      bMutiShow;    ///<�Ƿ������ʾ
		int      nFontSize;///<��ʾ����Ĵ�С
		COLORREF  crFontColor; ///<������ɫ
		bool      bItalicFlag;             ///<�Ƿ���б��
		bool      bBoldFlag;               ///<�Ƿ�Ӵ�
		bool      bUnderLineFlag;          ///<�Ƿ����»���
		bool      bIsManyChar;             ///<�Ƿ���кܶ���������ĺ���
		FONTSHOWSTYLE      bFontShowStyle; ///<����ʾ��ʽ��0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ���
		bool bIsOutline;				   ///�����Ƿ���ߡ���LCF
		COLORREF  crFontOutlineColor;      ///<���������ɫ����LCF
	};
#pragma pack()
	struct EditorTextInfo
	{
		EditorTextInfo(){ZeroMemory(this, sizeof(EditorTextInfo));}
		wchar_t    _Text[MAX_PATH];  ///< Text����ʾ������
		wchar_t    _FontStyle[MAX_PATH]; ///<��������    ���壬��Բ������
		bool      _bMutiShow;    ///<�Ƿ������ʾ
		int      _nFontSize;///<��ʾ����Ĵ�С
		COLORREF  _crFontColor; ///<������ɫ
		bool      _bItalicFlag;             ///<�Ƿ���б��
		bool      _bBoldFlag;               ///<�Ƿ�Ӵ�
		bool      _bUnderLineFlag;          ///<�Ƿ����»���
		bool      _bIsManyChar;             ///<�Ƿ���кܶ���������ĺ���
		COLORREF  _crSorColor;                ///<�����ɫ
		COLORREF  _crRectFrame;             ///<���߿���ɫ
		COLORREF  _crRectBk;                ///<��ܵ�ɫ
		bool      _bIsPassWord;             ///<�Ƿ��������
	};

	struct EditorTextInfo89
	{
		EditorTextInfo89(){ZeroMemory(this, sizeof(EditorTextInfo89));}
		wchar_t    _Text[MAX_PATH];  ///< Text����ʾ������
		wchar_t    _FontStyle[MAX_PATH]; ///<��������    ���壬��Բ������
		bool      _bMutiShow;    ///<�Ƿ������ʾ
		int      _nFontSize;///<��ʾ����Ĵ�С
		COLORREF  _crFontColor; ///<������ɫ
		bool      _bItalicFlag;             ///<�Ƿ���б��
		bool      _bBoldFlag;               ///<�Ƿ�Ӵ�
		bool      _bUnderLineFlag;          ///<�Ƿ����»���
		bool      _bIsManyChar;             ///<�Ƿ���кܶ���������ĺ���
		COLORREF  _crSorColor;                ///<�����ɫ
		COLORREF  _crRectFrame;             ///<���߿���ɫ
		COLORREF  _crRectBk;                ///<��ܵ�ɫ
		bool      _bIsPassWord;             ///<�Ƿ��������
		bool bIsOutline;				   ///�����Ƿ���ߡ���LCF
		COLORREF  crFontOutlineColor;      ///<���������ɫ����LCF
	};

	///�������洢�ṹ
	struct ContainInfo
	{
		ContainInfo(){ZeroMemory(this, sizeof(ContainInfo));}
		int  controlSize;                             ///�����ؼ�����
		ControlKind controlKind[MAX_INCONTAIN_NUM];             ///�����ؼ�����
		wchar_t _vPath[MAX_PATH];                     ///<������ͼ·��
	};

	///�������洢�ṹ
	struct ContainInfoNew 
	{
		ContainInfoNew(){ZeroMemory(this, sizeof(ContainInfoNew));}
		int  controlSize;                             ///�����ؼ�����
		ControlKind controlKind[MAX_INCONTAIN_NUM];             ///�����ؼ�����
		wchar_t _vPath[MAX_PATH];                     ///<������ͼ·��
		bool   _bIsMove;                              ///<�����Ƿ���ƶ�
	};
	///�������洢�ṹ ///����4�汾��
	struct ContainInfo4
	{
		ContainInfo4(){ZeroMemory(this, sizeof(ContainInfo4));}
		int  controlSize;                             ///�����ؼ�����
		ControlKind controlKind[MAX_INCONTAIN_NUM];             ///�����ؼ�����
		bool        controlShowFlag[MAX_INCONTAIN_NUM];         ///<�Ƿ���ؼ���ʾ����ʾ
		wchar_t _vPath[MAX_PATH];                     ///<������ͼ·��
		bool   _bIsMove;                              ///<�����Ƿ���ƶ�
	};
    ///�������洢�ṹ ///����7�汾��
    struct ContainInfo7
    {
        ContainInfo7(){ZeroMemory(this, sizeof(ContainInfo7));}
        int  controlSize;                             ///�����ؼ�����
        ControlKind controlKind[MAX_INCONTAIN_NUM];             ///�����ؼ�����
        bool        controlShowFlag[MAX_INCONTAIN_NUM];         ///<�Ƿ���ؼ���ʾ����ʾ
        wchar_t _vPath[MAX_PATH];                     ///<������ͼ·��
        bool   _bIsMove;                              ///<�����Ƿ���ƶ�
        bool   _bIsPagiStyle;                         ///<�Ƿ��ҳ
        int    _nNumperPage;                          ///<һҳ����
        int    _nCurrentPage;                         ///<��ǰҳ
        int    _nVecNoPagiID[10];                     ///<10������ҳ��ID��
    };

	///�������洢�ṹ ///����8�汾�ģ����°汾Ϊ8��
	struct ContainInfo8
	{
		ContainInfo8(){ZeroMemory(this, sizeof(ContainInfo8));}
		int  controlSize;                             ///�����ؼ�����
		ControlKind controlKind[MAX_INCONTAIN_NUM];             ///�����ؼ�����
		//bool        controlShowFlag[MAX_INCONTAIN_NUM];         ///<�Ƿ���ؼ���ʾ����ʾ
		wchar_t _vPath[MAX_PATH];                     ///<������ͼ·��
		bool   _bIsMove;                              ///<�����Ƿ���ƶ�
		bool   _bIsVisable;							  ///< �Ƿ�ɼ�
		bool   _bIsPagiStyle;                         ///<�Ƿ��ҳ
		int    _nNumperPage;                          ///<һҳ����
		int    _nCurrentPage;                         ///<��ǰҳ
		int    _nVecNoPagiID[10];                     ///<10������ҳ��ID��
	};

	///������
	struct ComboxInfo
	{
		ComboxInfo(){ZeroMemory(this, sizeof(ComboxInfo));}
		wchar_t    _fontStyle[MAX_PATH]; ///<��������    ���壬��Բ������
		int      _nFontSize;///<��ʾ����Ĵ�С
		wchar_t      _vPath[MAX_PATH];     ///<������Ť��ͼƬ·��
		
		COLORREF  _crFontColor;                  ///<������ɫ
		COLORREF  _crMoveRectColor;              ///<�ƶ������ɫ
		COLORREF  _crBkRectFillColor;                ///<�ؼ�������ɫ
		COLORREF  _crBkFrameColor;               ///<�ؼ������߿���ɫ

		COLORREF  _crSor;                        ///<�����ɫ
		COLORREF  _crEditorFrame;                ///<�༭��߿���ɫ
		COLORREF  _crEditorFill;                 ///<�༭�������ɫ
		COLORREF  _crFrame;                      ///<�ؼ����߿���ɫ

	
	};

	///������
	struct ComboxInfo89
	{
		ComboxInfo89(){ZeroMemory(this, sizeof(ComboxInfo89));}
		wchar_t    _fontStyle[MAX_PATH]; ///<��������    ���壬��Բ������
		int      _nFontSize;///<��ʾ����Ĵ�С
		wchar_t      _vPath[MAX_PATH];     ///<������Ť��ͼƬ·��

		COLORREF  _crFontColor;                  ///<������ɫ
		COLORREF  _crMoveRectColor;              ///<�ƶ������ɫ
		COLORREF  _crBkRectFillColor;                ///<�ؼ�������ɫ
		COLORREF  _crBkFrameColor;               ///<�ؼ������߿���ɫ

		COLORREF  _crSor;                        ///<�����ɫ
		COLORREF  _crEditorFrame;                ///<�༭��߿���ɫ
		COLORREF  _crEditorFill;                 ///<�༭�������ɫ
		COLORREF  _crFrame;                      ///<�ؼ����߿���ɫ
		bool bIsOutline;				   ///�����Ƿ���ߡ���LCF
		COLORREF  crFontOutlineColor;      ///<���������ɫ����LCF

	};
	
	struct InputeUserNameINfo
	{
		InputeUserNameINfo(){ZeroMemory(this, sizeof(InputeUserNameINfo));}
		wchar_t    _fontStyle[MAX_PATH]; ///<��������    ���壬��Բ������
		int      _nFontSize;///<��ʾ����Ĵ�С
		wchar_t      _vPath[MAX_PATH];     ///<������Ť��ͼƬ·��

		COLORREF  _crFontColor;                  ///<������ɫ
		COLORREF  _crMoveRectColor;              ///<�ƶ������ɫ
		COLORREF  _crBkRectFillColor;                ///<�ؼ�������ɫ
		COLORREF  _crBkFrameColor;               ///<�ؼ������߿���ɫ

		COLORREF  _crSor;                        ///<�����ɫ
		COLORREF  _crEditorFrame;                ///<�༭��߿���ɫ
		COLORREF  _crEditorFill;                 ///<�༭�������ɫ
		COLORREF  _crFrame;                      ///<�ؼ����߿���ɫ
		wchar_t   _vDeleteButtonPath[MAX_PATH];  ///<ɾ����Ť��ͼƬ·��
		int       _nDeleteButtonW;               ///<ɾ����Ť�Ŀ�
	};
#pragma pack(1)
	struct InputUserNameInfo_8_1
	{
		InputUserNameInfo_8_1(){ZeroMemory(this, sizeof(InputUserNameInfo_8_1));}
		wchar_t    _fontStyle[MAX_PATH]; ///<��������    ���壬��Բ������
		int      _nFontSize;///<��ʾ����Ĵ�С
		wchar_t      _vPath[MAX_PATH];     ///<������Ť��ͼƬ·��

		BYTE	  _BoldAndItalic;			///<�Ƿ�Ӵֺ�б�� 0x01��ʾ���壬0x02��ʾб��

		COLORREF  _crFontColor;                  ///<������ɫ
		COLORREF  _crMoveRectColor;              ///<�ƶ������ɫ
		COLORREF  _crBkRectFillColor;                ///<�ؼ�������ɫ
		COLORREF  _crBkFrameColor;               ///<�ؼ������߿���ɫ

		COLORREF  _crSor;                        ///<�����ɫ
		COLORREF  _crEditorFrame;                ///<�༭��߿���ɫ
		COLORREF  _crEditorFill;                 ///<�༭�������ɫ
		COLORREF  _crFrame;                      ///<�ؼ����߿���ɫ
		wchar_t   _vDeleteButtonPath[MAX_PATH];  ///<ɾ����Ť��ͼƬ·��
		int       _nDeleteButtonW;               ///<ɾ����Ť�Ŀ�
	};
#pragma pack()

#pragma pack(1)
    struct InputUserNameInfo_8_2
    {
        InputUserNameInfo_8_2(){ZeroMemory(this, sizeof(InputUserNameInfo_8_2));}
        wchar_t    _fontStyle[MAX_PATH]; ///<��������    ���壬��Բ������
        int      _nFontSize;///<��ʾ����Ĵ�С
        wchar_t      _vPath[MAX_PATH];     ///<������Ť��ͼƬ·��

        BYTE	  _BoldAndItalic;			///<�Ƿ�Ӵֺ�б�� 0x01��ʾ���壬0x02��ʾб��

        COLORREF  _crFontColor;                  ///<������ɫ
        COLORREF  _crMoveRectColor;              ///<�ƶ������ɫ
        COLORREF  _crBkRectFillColor;                ///<�ؼ�������ɫ
        COLORREF  _crBkFrameColor;               ///<�ؼ������߿���ɫ

        COLORREF  _crSor;                        ///<�����ɫ
        COLORREF  _crEditorFrame;                ///<�༭��߿���ɫ
        COLORREF  _crEditorFill;                 ///<�༭�������ɫ
        COLORREF  _crFrame;                      ///<�ؼ����߿���ɫ
        wchar_t   _vDeleteButtonPath[MAX_PATH];  ///<ɾ����Ť��ͼƬ·��
        int       _nDeleteButtonW;               ///<ɾ����Ť�Ŀ�


        wchar_t	  _vScrollBkPath[MAX_PATH];		///<��������ͼ
        wchar_t   _vScrollTPath[MAX_PATH];		///<�������ϰ�Ť
        wchar_t   _vScrollBPath[MAX_PATH];		///<�������°�Ť
        wchar_t   _vScrollBarPath[MAX_PATH];	///<����ͼƬ
    };
#pragma pack()

#pragma pack(1)
    struct InputUserNameInfo_8_7
    {
        InputUserNameInfo_8_7(){ZeroMemory(this, sizeof(InputUserNameInfo_8_7));}
        wchar_t    _fontStyle[MAX_PATH]; ///<��������    ���壬��Բ������
        int      _nFontSize;///<��ʾ����Ĵ�С
        wchar_t      _vPath[MAX_PATH];     ///<������Ť��ͼƬ·��

        BYTE	  _BoldAndItalic;			///<�Ƿ�Ӵֺ�б�� 0x01��ʾ���壬0x02��ʾб��

        COLORREF  _crFontColor;                  ///<������ɫ
        COLORREF  _crMoveRectColor;              ///<�ƶ������ɫ
        COLORREF  _crBkRectFillColor;                ///<�ؼ�������ɫ
        COLORREF  _crBkFrameColor;               ///<�ؼ������߿���ɫ

        COLORREF  _crSor;                        ///<�����ɫ
        COLORREF  _crEditorFrame;                ///<�༭��߿���ɫ
        COLORREF  _crEditorFill;                 ///<�༭�������ɫ
        COLORREF  _crFrame;                      ///<�ؼ����߿���ɫ
        wchar_t   _vDeleteButtonPath[MAX_PATH];  ///<ɾ����Ť��ͼƬ·��
        int       _nDeleteButtonW;               ///<ɾ����Ť�Ŀ�


        wchar_t	  _vScrollBkPath[MAX_PATH];		///<��������ͼ
        wchar_t   _vScrollTPath[MAX_PATH];		///<�������ϰ�Ť
        wchar_t   _vScrollBPath[MAX_PATH];		///<�������°�Ť
        wchar_t   _vScrollBarPath[MAX_PATH];	///<����ͼƬ
		DWORD	  _crFontSelectColor;			///<����ѡ����ɫ
    };
#pragma pack()

#pragma pack(1)
	struct InputUserNameInfo89
	{
		InputUserNameInfo89(){ZeroMemory(this, sizeof(InputUserNameInfo89));}
		wchar_t    _fontStyle[MAX_PATH]; ///<��������    ���壬��Բ������
		int      _nFontSize;///<��ʾ����Ĵ�С
		wchar_t      _vPath[MAX_PATH];     ///<������Ť��ͼƬ·��

		BYTE	  _BoldAndItalic;			///<�Ƿ�Ӵֺ�б�� 0x01��ʾ���壬0x02��ʾб��

		COLORREF  _crFontColor;                  ///<������ɫ
		COLORREF  _crMoveRectColor;              ///<�ƶ������ɫ
		COLORREF  _crBkRectFillColor;                ///<�ؼ�������ɫ
		COLORREF  _crBkFrameColor;               ///<�ؼ������߿���ɫ

		COLORREF  _crSor;                        ///<�����ɫ
		COLORREF  _crEditorFrame;                ///<�༭��߿���ɫ
		COLORREF  _crEditorFill;                 ///<�༭�������ɫ
		COLORREF  _crFrame;                      ///<�ؼ����߿���ɫ
		wchar_t   _vDeleteButtonPath[MAX_PATH];  ///<ɾ����Ť��ͼƬ·��
		int       _nDeleteButtonW;               ///<ɾ����Ť�Ŀ�


		wchar_t	  _vScrollBkPath[MAX_PATH];		///<��������ͼ
		wchar_t   _vScrollTPath[MAX_PATH];		///<�������ϰ�Ť
		wchar_t   _vScrollBPath[MAX_PATH];		///<�������°�Ť
		wchar_t   _vScrollBarPath[MAX_PATH];	///<����ͼƬ
		DWORD	  _crFontSelectColor;			///<����ѡ����ɫ
		bool bIsOutline;				   ///�����Ƿ���ߡ���LCF
		COLORREF  crFontOutlineColor;      ///<���������ɫ����LCF
	};
#pragma pack()
	///������
	struct ProgressInfo
	{
		ProgressInfo(){ZeroMemory(this, sizeof(ProgressInfo));}
		//COLORREF  _crBk;        ///<��������ɫ
		//COLORREF  _crFrame;     ///<�����������ɫ
		//COLORREF  _crProgress;  ///<�м�Ľ���������ɫ
		wchar_t    _vBkPath[MAX_PATH]; ///<��������ɫ·��
		wchar_t    _vProgressPath[MAX_PATH]; ///<��������ʾͼƬ·��
	};
	///������
	struct ScrollBarInfo
	{
		ScrollBarInfo(){ZeroMemory(this, sizeof(ScrollBarInfo));}
		wchar_t      _bkPath[MAX_PATH];     ///<��������ɫͼƬ·��
		wchar_t      _tPath[MAX_PATH];     ///<����������ͼƬ·��
		wchar_t      _bPath[MAX_PATH];     ///<�������׶�ͼƬ·��
		wchar_t      _sPath[MAX_PATH];     ///<����������ͼƬ·��
		bool         _verticalFlag;        ///<���������򣬺�������ͣ�trueΪ����
	};
	///IMAGE�ƶ�����
	struct ImageFlashInfo
	{
		ImageFlashInfo(){ZeroMemory(this, sizeof(ImageFlashInfo));}
		//wchar_t    _vpath[MAX_PATH];       ///<IMAGEͼƬ·��
		int        _nFrameNum;               ///<������֡��
		POINT      _ptStart;                 ///<�������ŵ���ʼ����
		POINT      _ptEnd;                   ///<�������ŵ��յ�����
		POINT      _ptControl;               ///<�м���Ƶ�����
		int        _nMoveTotalTime;          ///<�����ƶ���ʱ��
		int        _nPlayTotalTime;          ///<����������ʱ��
		int        _nPlayNum;                ///<�������Ŵ���
		bool       _bLineKind;               ///<�Ƿ���ֱ�����͵�
		bool       _bMoveFlag;               ///<�����Ƿ�����ƶ�
		wchar_t      _vPath[20][MAX_PATH];     ///<��������֡��·��
	};

	///��ѡ��
	struct RadioButtonInfo 
	{
		RadioButtonInfo(){ZeroMemory(this, sizeof(RadioButtonInfo));}
		wchar_t _vBkPath[MAX_PATH];       ///<�洢��ͼ·��
		wchar_t _vSelectPath[MAX_PATH];   ///<ѡ���·��
		wchar_t    _Text[MAX_PATH];  ///< Text����ʾ������
		wchar_t    _FontStyle[MAX_PATH]; ///<��������    ���壬��Բ������
		int      _nFontSize;///<��ʾ����Ĵ�С
		COLORREF  _crFontColor; ///<������ɫ
		int       _nBkWith;    ///<��ͼ���
	};
	///�ؼ��б�
	struct ListControlInfo
	{
		ListControlInfo(){ZeroMemory(this, sizeof(ListControlInfo));}
		wchar_t _vBkPath[MAX_PATH];       ///<�洢��ͼ·��
		wchar_t _vHeadPath[MAX_PATH];     ///<��ͷͼƬ
		wchar_t _vHeadSpacePath[MAX_PATH]; ///<��ͷ���ͼƬ
		wchar_t _vVerScrollBkPath[MAX_PATH];  ///<��������ͼ
		wchar_t _vVerScrollTPath[MAX_PATH];   ///<�������ϰ�Ť
		wchar_t _vVerScrollBPath[MAX_PATH];   ///<�������°�Ť
		wchar_t _vVerScrollBarPath[MAX_PATH]; ///<����ͼƬ

		wchar_t _vHorizScrollBkPath[MAX_PATH];  ///<��������ͼ
		wchar_t _vHorizScrollTPath[MAX_PATH];   ///<�������ϰ�Ť
		wchar_t _vHorizScrollBPath[MAX_PATH];   ///<�������°�Ť
		wchar_t _vHorizScrollBarPath[MAX_PATH]; ///<����ͼƬ

		bool    _bHaveScroll;              ///<�Ƿ�һֱ�й�����
		wchar_t    _FontStyle[MAX_PATH]; ///<��������    ���壬��Բ������
		int      _nFontSize;///<��ʾ����Ĵ�С
		COLORREF  _crFontColor; ///<������ɫ
		int       _nHeadHeight; ///<��ͷ����ĸ�
	};
	///�ؼ��б�
	struct ListControlInfo89
	{
		ListControlInfo89(){ZeroMemory(this, sizeof(ListControlInfo89));}
		wchar_t _vBkPath[MAX_PATH];       ///<�洢��ͼ·��
		wchar_t _vHeadPath[MAX_PATH];     ///<��ͷͼƬ
		wchar_t _vHeadSpacePath[MAX_PATH]; ///<��ͷ���ͼƬ
		wchar_t _vVerScrollBkPath[MAX_PATH];  ///<��������ͼ
		wchar_t _vVerScrollTPath[MAX_PATH];   ///<�������ϰ�Ť
		wchar_t _vVerScrollBPath[MAX_PATH];   ///<�������°�Ť
		wchar_t _vVerScrollBarPath[MAX_PATH]; ///<����ͼƬ

		wchar_t _vHorizScrollBkPath[MAX_PATH];  ///<��������ͼ
		wchar_t _vHorizScrollTPath[MAX_PATH];   ///<�������ϰ�Ť
		wchar_t _vHorizScrollBPath[MAX_PATH];   ///<�������°�Ť
		wchar_t _vHorizScrollBarPath[MAX_PATH]; ///<����ͼƬ

		bool    _bHaveScroll;              ///<�Ƿ�һֱ�й�����
		wchar_t    _FontStyle[MAX_PATH]; ///<��������    ���壬��Բ������
		int      _nFontSize;///<��ʾ����Ĵ�С
		COLORREF  _crFontColor; ///<������ɫ
		int       _nHeadHeight; ///<��ͷ����ĸ�
		bool bIsOutline;				   ///�����Ƿ���ߡ���LCF
		COLORREF  crFontOutlineColor;      ///<���������ɫ����LCF
	};
	///�����
	struct TalkControlInfo
	{
		TalkControlInfo(){ZeroMemory(this, sizeof(TalkControlInfo));}
		wchar_t _vBkPath[MAX_PATH];       ///<�洢��ͼ·��
		wchar_t _vScrollBkPath[MAX_PATH];  ///<��������ͼ
		wchar_t _vScrollTPath[MAX_PATH];   ///<�������ϰ�Ť
		wchar_t _vScrollBPath[MAX_PATH];   ///<�������°�Ť
		wchar_t _vScrollBarPath[MAX_PATH]; ///<����ͼƬ
	};

	///�����
	struct TalkControlInfo6
	{
		TalkControlInfo6(){ZeroMemory(this, sizeof(TalkControlInfo6));}
		wchar_t _vBkPath[MAX_PATH];       ///<�洢��ͼ·��
		wchar_t _vScrollBkPath[MAX_PATH];  ///<��������ͼ
		wchar_t _vScrollTPath[MAX_PATH];   ///<�������ϰ�Ť
		wchar_t _vScrollBPath[MAX_PATH];   ///<�������°�Ť
		wchar_t _vScrollBarPath[MAX_PATH]; ///<����ͼƬ
		wchar_t _vFaceImagePath[MAX_PATH]; ///<�����ļ�
	};
	///�����ƶ��ƿؼ�
	struct NoMoveCardControlInfo
	{
		NoMoveCardControlInfo(){ZeroMemory(this, sizeof(NoMoveCardControlInfo));}
		wchar_t _vCardJoke[MAX_PATH];    ///<��С��ͼƬ·��
		wchar_t _vCard[MAX_PATH];        ///<�Ƶ�����ͼƬ·��
		int _nCardH;                     ///<�����Ƶĸ�
		int _nCardW;                     ///<�����ƵĿ�
		int _nCardHSpace;                ///<�Ƶĺ�����
		BYTE  _bAlignStyle;              ///<���뷽ʽ  0-2  0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ���
		int _bShowStyle;                 ///<��ʾ�Ƶķ���0-3 ˳ʱ����ʾ
		bool _bIsShowNum;                ///<�Ƿ���ʾʣ���Ƶ�����
		bool _bVertical;                 ///<�Ƶİڷ�ģʽ: true:���Ű�, false:���Ű�
	};

	///�����ƿؼ�
	struct LightCardControlInfo
	{
		LightCardControlInfo(){ZeroMemory(this, sizeof(LightCardControlInfo));}
		wchar_t _vCardJoke[MAX_PATH];       ///<��С��ͼƬ·��
		wchar_t _vCard[MAX_PATH];  ///<�Ƶ�����ͼƬ·��
		int _nShowCardW;    ///<����ʾ�Ŀ�
		int _nShowCardH;    ///<����ʾ�ĸ�
		int _nShowCardRowNum;  ///<һ����ʾ��������
		int _nRowSpace;     ///<������֮��ľ���
		int _nHSpace;       ///<���������Ƶļ��
	};
	struct OperateCardInfo
	{
		OperateCardInfo(){ZeroMemory(this, sizeof(OperateCardInfo));}
		wchar_t _vCardJoke[MAX_PATH];       ///<��С��ͼƬ·��
		wchar_t _vCard[MAX_PATH];  ///<�Ƶ�����ͼƬ·��
		wchar_t _vCardSelect[MAX_PATH];  ///<�Ƶ�ѡ��ͼƬ
		int _nCardH;                     ///<�����Ƶĸ�
		int _nCardW;                     ///<�����ƵĿ�
		int _nCardHSpace;                ///<�Ƶĺ�����
		BYTE  _bAlignStyle;             ///<���뷽ʽ  0-2  0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ���
		int _nUpSpace;                  ///<�Ƶ�����߶�
	};
	///���ֿؼ�
	struct NumControlInfo
	{
		NumControlInfo(){ZeroMemory(this, sizeof(NumControlInfo));}
		wchar_t _vNumPath[MAX_PATH];       ///<����ͼƬ·��
	};
	///�����ؼ�
	struct SliderControlInfo
	{	
		SliderControlInfo(){ZeroMemory(this, sizeof(SliderControlInfo));}
		wchar_t _vFillPath[MAX_PATH];       ///<���ͼƬ
		wchar_t _vButtonPath[MAX_PATH];  ///<�϶�ͼƬ
		BYTE    _bSliderStyle;           ///<������ʽ  ��������
	};
	///�˵���
	struct MenuControlInfo
	{
		MenuControlInfo(){ZeroMemory(this, sizeof(MenuControlInfo));}
		wchar_t _vBkImagePath[MAX_PATH];       ///<��ͼ
		wchar_t   _vFontStyle[MAX_PATH]; ///<��������    ���壬��Բ������
		int      _nFontSize;///<��ʾ����Ĵ�С
		COLORREF  _crFontColor; ///<������ɫ
		
		int     _nTextHeight;         ///<����TEXT�ĸ߶�
		FONTSHOWSTYLE _bFontShowStyle;  ///<����ģʽ

		COLORREF  _crLineColor;        ///<�ָ��ߵ���ɫ
		COLORREF  _crMoveColor;        ///<�ƶ���ʾ����ɫ
		COLORREF  _crMoveFramColor;
	};

	///�˵���
	struct MenuControlInfo89
	{
		MenuControlInfo89(){ZeroMemory(this, sizeof(MenuControlInfo89));}
		wchar_t _vBkImagePath[MAX_PATH];       ///<��ͼ
		wchar_t   _vFontStyle[MAX_PATH]; ///<��������    ���壬��Բ������
		int      _nFontSize;///<��ʾ����Ĵ�С
		COLORREF  _crFontColor; ///<������ɫ

		int     _nTextHeight;         ///<����TEXT�ĸ߶�
		FONTSHOWSTYLE _bFontShowStyle;  ///<����ģʽ

		COLORREF  _crLineColor;        ///<�ָ��ߵ���ɫ
		COLORREF  _crMoveColor;        ///<�ƶ���ʾ����ɫ
		COLORREF  _crMoveFramColor;
		bool bIsOutline;				   ///�����Ƿ���ߡ���LCF
		COLORREF  crFontOutlineColor;      ///<���������ɫ����LCF
	};

	struct SudokuInfo
	{
		SudokuInfo(){ZeroMemory(this, sizeof(SudokuInfo));}
		wchar_t      _vPath[MAX_PATH];
		MyRect_t     _vRectSrcPos[8];
	};
#pragma pack(1)
    struct SudokuInfo_1
    {
        SudokuInfo_1(){ZeroMemory(this, sizeof(SudokuInfo_1));}

        wchar_t      _vPath[MAX_PATH];  ///< ͼƬ·��
        MyRect_t     _vRectSrcPos[9];   ///< 9��PosInfo
        bool         bUseCenter;        ///< �Ƿ���ʾ�м�ƽ��ͼƬ   
        bool         bIsFrame;            ///< �Ƿ��Ǳ߿�
        bool         m_bAlighRight;
        bool         m_bAlighBottom;
    };
#pragma pack()

	struct TimeInfo
	{
		TimeInfo(){ZeroMemory(this, sizeof(TimeInfo));}
		wchar_t  _vBKImagePath[MAX_PATH]; ///<��ͼ·��
		wchar_t  _vNumPath[MAX_PATH];   ///<ʱ��ͼƬ
		int        _nMaxNum;           ///<��ʾ�������
		int        _nMinNum;           ///<��ʾ��С����
	};
	struct SiTuanOperateCardInfo
	{
		SiTuanOperateCardInfo(){ZeroMemory(this, sizeof(SiTuanOperateCardInfo));}
		wchar_t _vCardJoke[MAX_PATH];       ///<��С��ͼƬ·��
		wchar_t _vCard[MAX_PATH];  ///<�Ƶ�����ͼƬ·��
		wchar_t _vCardSelect[MAX_PATH];  ///<�Ƶ�ѡ��ͼƬ
		int _nCardH;                     ///<�����Ƶĸ�
		int _nCardW;                     ///<�����ƵĿ�
		int _nCardHSpace;                ///<�Ƶĺ�����
		BYTE  _bAlignStyle;             ///<���뷽ʽ  0-2  0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ���
		int _nUpSpace;                  ///<�Ƶ�����߶�
		int    _nCardFoldSpace;      ///<�ص����
		int    _nCardThreeSpace;     ///<���Ż������˿�֮��ļ��
		int    _nCardThreeToThree;   ///<���ŵ����ţ����ŵ����ŵļ��
		int    _nCardFoldToOtherSpace;    ///<�ص��Ƶ���ͨ�Ƶļ��
		int    _nCardThreeToOtherSpace; ///<�����Ƶ������Ƶļ��
		int    _nCardTwoToOtherSpace;   ///<���ŵ�������֮��ļ��
		wchar_t _vNum[MAX_PATH];        ///<�Ƶ������ֵ�·��
	};
	struct SiTuanNoMoveCardInfo
	{
		SiTuanNoMoveCardInfo(){ZeroMemory(this, sizeof(SiTuanNoMoveCardInfo));}
		wchar_t _vCardJoke[MAX_PATH];       ///<��С��ͼƬ·��
		wchar_t _vCard[MAX_PATH];  ///<�Ƶ�����ͼƬ·��
		wchar_t _vNum[MAX_PATH]; ///<��ʾ������ͼƬ
		int _nCardH;                     ///<�����Ƶĸ�
		int _nCardW;                     ///<�����ƵĿ�
		int _nCardHSpace;                ///<�Ƶĺ�����
		BYTE  _bAlignStyle;             ///<���뷽ʽ  0-2  0��ʾ����룬1��ʾ���У�2��ʾ�Ҷ���
		int _bShowStyle;      ///<��ʾ�Ƶķ���0-3 ˳ʱ����ʾ
		bool _bIsShowNum;               ///<�Ƿ���ʾʣ���Ƶ�����
	};
	struct SiTuanLightCardInfo
	{
		SiTuanLightCardInfo(){ZeroMemory(this, sizeof(SiTuanLightCardInfo));}
		wchar_t _vCardJoke[MAX_PATH];       ///<��С��ͼƬ·��
		wchar_t _vCard[MAX_PATH];  ///<�Ƶ�����ͼƬ·��
		int _nShowCardW;    ///<����ʾ�Ŀ�
		int _nShowCardH;    ///<����ʾ�ĸ�
		int _nShowCardRowNum;  ///<һ����ʾ��������
		int _nRowSpace;     ///<������֮��ľ���
		int _nHSpace;       ///<���������Ƶļ��
		wchar_t  _pNumPath[MAX_PATH];   ///<����ͼƬ��·��
		int    _nFoldSpace;///���Ƽ��
		int    _nFoldToOther; ///<���Ƶ������Ƶļ��
	};

    ///IMAGE�ƶ�����
    struct ExAnimateInfo
    {
        ExAnimateInfo(){ZeroMemory(this, sizeof(ExAnimateInfo));}
        //wchar_t    _vpath[MAX_PATH];       ///<IMAGEͼƬ·��
        int        _nFrameNum;               ///<������֡��
        POINT      _ptStart;                 ///<�������ŵ���ʼ����
        POINT      _ptEnd;                   ///<�������ŵ��յ�����
        POINT      _ptControl;               ///<�м���Ƶ�����
        int        _nMoveTotalTime;          ///<�����ƶ���ʱ��
        int        _nPlayTotalTime;          ///<����������ʱ��
        int        _nPlayNum;                ///<�������Ŵ���
        bool       _bLineKind;               ///<�Ƿ���ֱ�����͵�
        bool       _bMoveFlag;               ///<�����Ƿ�����ƶ�
        wchar_t      _vPath[50][MAX_PATH];   ///<��������֡��·��        

        wchar_t     wszExtFilePath[MAX_PATH];     
    };

    ///�齫�ؼ���Ϣ
    struct MjCtrlInfo
    {
        MjCtrlInfo(){ZeroMemory(this, sizeof(MjCtrlInfo));}
        int                         m_iMjTileWid;                   /// �齫�ӿ�
        int                         m_iMjTileHei;                   /// �齫�Ӹ�
        MJ_TILE_UIINFO              m_TilesInfo[MAX_TILES_INCTRL];  /// �齫��Ϣ
        int                         m_nTilesCount;                  /// �齫����
        wchar_t                     m_MjPicPath[MAX_PATH];          /// �齫ͼƬ·��
    };   


    ///�齫���ƿؼ���Ϣ
    struct MjHandCtrlInfo
    {
        MjHandCtrlInfo(){ZeroMemory(this, sizeof(MjHandCtrlInfo));}

        int                         m_iMjTileWid;                   /// �齫�ӿ�
        int                         m_iMjTileHei;                   /// �齫�Ӹ�
        int                         m_nTilesHand[MAX_TILES_INCTRL]; /// �齫��Ϣ
        int                         m_nTilesCount;                  /// �齫����
        BYTE                        m_bViewStation;                 /// ��λ
        wchar_t                     m_MjPicPath[MAX_PATH];          /// �齫ͼƬ·��
    };   


    ///�齫���ƿؼ���Ϣ
    struct MjGiveCtrlInfo
    {
        MjGiveCtrlInfo(){ZeroMemory(this, sizeof(MjGiveCtrlInfo));}

        int                         m_iMjTileWid;                   /// �齫�ӿ�
        int                         m_iMjTileHei;                   /// �齫�Ӹ�
        int                         m_nTilesGive[MAX_TILES_INCTRL]; /// �齫��Ϣ
        int                         m_nTilesCount;                  /// �齫����
        BYTE                        m_bViewStation;                 /// ��λ
        wchar_t                     m_MjPicPath[MAX_PATH];          /// �齫ͼƬ·��
    };   

    ///�齫���ƿؼ���Ϣ
    struct MjWallCtrlInfo
    {
        MjWallCtrlInfo(){ZeroMemory(this, sizeof(MjWallCtrlInfo ));}

        int                         m_iMjTileWid;                   /// �齫�ӿ�
        int                         m_iMjTileHei;                   /// �齫�Ӹ�
        int                         m_nTilesCount;                  /// �齫����
        BYTE                        m_bViewStation;                 /// ��λ
        wchar_t                     m_MjPicPath[MAX_PATH];          /// �齫ͼƬ·��
    };   

    ///�齫���ƿؼ�
    struct MjBlockCtrlInfo
    {
        MjBlockCtrlInfo(){ZeroMemory(this, sizeof(MjBlockCtrlInfo ));}

        int                         m_iMjTileWid;                   /// �齫�ӿ�
        int                         m_iMjTileHei;                   /// �齫�Ӹ�
        int                         m_nTilesCount;                  /// �齫����
        BYTE                        m_bViewStation;                 /// ��λ
        MJ_LOGIC_TYPE               m_mtTypes[MAX_BLOCK_COUNT];
        BYTE                        m_ucTiles[MAX_BLOCK_COUNT][4];
        BYTE                        m_ucChairs[MAX_BLOCK_COUNT];
        wchar_t                     m_MjPicPath[MAX_PATH];          /// �齫ͼƬ·��
    };  
	///�������ؼ���Ϣ
	struct ShCtrlInfo
	{
		ShCtrlInfo(){ZeroMemory(this, sizeof(ShCtrlInfo));}
		int                         iShTileWid;                   /// ��������ӿ�
		int                         iShTileHei;                   /// ��������Ӹ�
		int                         nTilesCount;                  /// ����������
		wchar_t                     ShPicPath[MAX_PATH];          /// �������ͼƬ·��
		int							iShNote;                      /// �����ʾ����ֵ
		int							iShNoteType;				  /// �����ʾ��ʽ
	};  

#pragma pack(1)
    ///�ƶ������ؼ�
    struct MoveActionCtrlInfo
    {
        MoveActionCtrlInfo(){ZeroMemory(this, sizeof(MoveActionCtrlInfo ));}

        POINT                       m_ptStart;                      /// �������ŵ���ʼ����
        POINT                       m_ptControl;                    /// �м���Ƶ�����
        POINT                       m_ptEnd;                        /// �������ŵ��յ�����  
        CURVE_TYPE                  m_eCurveType;                   /// ��������
        unsigned int                m_uTotalMoveTime;               /// �ƶ���ʱ��
        bool                        m_bIsSymmetrical;               /// �Ƿ�Գ�
        int                         m_nControlingID;                /// ���Ƶ�ID
    };
#pragma pack()

#pragma pack(1)
    ///�ƶ������ؼ�
    struct FlashPlayCtrlInfo
    {
        FlashPlayCtrlInfo(){ZeroMemory(this, sizeof(FlashPlayCtrlInfo ));}

        wchar_t                     m_szSwfFile[MAX_PATH];
    };
#pragma pack()

#pragma pack(1)
    ///����ƿؼ�
    struct RSLampCtrlInfo : public TextInfo
    {
        RSLampCtrlInfo(){ZeroMemory(this, sizeof(RSLampCtrlInfo));}

        wchar_t                     m_szBkImg[MAX_PATH];
        int                         m_nRefreshTime;     // ˢ��ʱ��
        int                         m_nPixelPerSec;     // ���ù����ٶȣ�ÿm_nFreshTime�����������
        int                         m_nDir;             // 0����1����
    };

	///����ƿؼ�
    struct RSLampCtrlInfo88 : public TextInfo88
    {
        RSLampCtrlInfo88(){ZeroMemory(this, sizeof(RSLampCtrlInfo88));}

        wchar_t                     m_szBkImg[MAX_PATH];
        int                         m_nRefreshTime;     // ˢ��ʱ��
        int                         m_nPixelPerSec;     // ���ù����ٶȣ�ÿm_nFreshTime�����������
        int                         m_nDir;             // 0����1����
    };

	///����ƿؼ�89����LCF
	struct RSLampCtrlInfo89 : public TextInfo89
	{
		RSLampCtrlInfo89(){ZeroMemory(this, sizeof(RSLampCtrlInfo89));}

		wchar_t                     m_szBkImg[MAX_PATH];
		int                         m_nRefreshTime;     // ˢ��ʱ��
		int                         m_nPixelPerSec;     // ���ù����ٶȣ�ÿm_nFreshTime�����������
		int                         m_nDir;             // 0����1����
	};
#pragma pack()

#pragma pack(1)
    ///���ӿؼ���Ϣ
    struct HuziCardCtrlInfo
    {
        wchar_t  pPath[MAX_PATH];                   // �ؼ�����ͼƬ��ַ
        int      nVerInterval;                      // ��ֱ���
        int      nVerCutCount;                      // ��ֱ��Ƭ��
        bool     bCanSelect;                        // �Ƿ��ѡ��
        int      nRotate;                           // ����


        HuziCardCtrlInfo()
        {
            ZeroMemory(this, sizeof(HuziCardCtrlInfo));
        }
    };

    ///���ӿؼ���Ϣ
    struct HuziCardCtrlInfo2
    {
        wchar_t  pPath[MAX_PATH];                   // �ؼ�����ͼƬ��ַ
        int      nVerInterval;                      // ��ֱ���
        int      nHorInterval;                      // ������
        int      nVerCutCount;                      // ��ֱ��Ƭ��
        bool     bCanSelect;                        // �Ƿ��ѡ��
        int      nRotate;                           // ����

        int      nSpare[4];                         // ��4���ֽڳ���

        HuziCardCtrlInfo2()
        {
            ZeroMemory(this, sizeof(HuziCardCtrlInfo2));
        }
    };
#pragma pack()

#pragma pack(1)

	///������ؼ���Ϣ
	struct TopPaperInfo
	{
		// �ؼ�����ͼƬ��ַ
		wchar_t  wszBkPath[MAX_PATH];                 

		//���弰��С
		wchar_t wszFontStyle[MAX_PATH];
		int nFontSize;

		//��������
		int m_nLeanSide;

		//Ĭ���������ɫ
		COLORREF crDefFontColor;

		//�񵥵�����������
		int iRowNums;
		int iColNums;

		//�񵥵��и��п�
		int iRowH;
		int iColW;

		//�񵥵��о����о�
		int iRowSpace;
		wchar_t wszColSpace[64];

		//���ű�����
		int iBegTextX;
		int iBegTextY;

		//¶�Ǿ���
		int m_iCornerSpace;

		//ÿһ��������ɫ����
		wchar_t wszRowColor[MAX_PATH];

		TopPaperInfo()
		{
			ZeroMemory(this, sizeof(TopPaperInfo));
		}
	};

#pragma pack()

///������ؼ���Ϣ
struct TopPaperInfo89
{
	// �ؼ�����ͼƬ��ַ
	wchar_t  wszBkPath[MAX_PATH];                 

	//���弰��С
	wchar_t wszFontStyle[MAX_PATH];
	int nFontSize;

	//��������
	int m_nLeanSide;

	//Ĭ���������ɫ
	COLORREF crDefFontColor;

	//�񵥵�����������
	int iRowNums;
	int iColNums;

	//�񵥵��и��п�
	int iRowH;
	int iColW;

	//�񵥵��о����о�
	int iRowSpace;
	wchar_t wszColSpace[64];

	//���ű�����
	int iBegTextX;
	int iBegTextY;

	//¶�Ǿ���
	int m_iCornerSpace;

	//ÿһ��������ɫ����
	wchar_t wszRowColor[MAX_PATH];

	bool bIsOutline;				   ///�����Ƿ���ߡ���LCF
	COLORREF  crFontOutlineColor;      ///<���������ɫ����LCF

	TopPaperInfo89()
	{
		ZeroMemory(this, sizeof(TopPaperInfo89));
	}
};

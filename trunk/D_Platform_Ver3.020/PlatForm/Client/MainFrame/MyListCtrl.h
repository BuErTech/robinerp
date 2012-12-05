#ifndef _MY_LIST_CTRL_H_INCLUDE
#define _MY_LIST_CTRL_H_INCLUDE

//�궨��
#define USER_STATE_COUNT						6						//״̬��Ŀ
#define USER_STATE_WIDTH						27						//״̬���
#define	USER_STATE_HEIGHT						17						//״̬�߶�

#include <string>
#include <vector>
#include <map>

/**
 * �б�ͷ
 */
class CComBodyClassT : public CHeaderCtrl
{
public:
	/// ���캯��
	CComBodyClassT();
	/// ��������
	virtual ~CComBodyClassT();

protected:
	int			m_Type;		//��������,����
	CImage  m_bkimg;
	CImage  m_bkimg2;
	COLORREF    m_color;
	COLORREF    m_textcolor;
	CFont                       m_Font;                         /**< ��ǰ��ǩʹ�õ����� */

public:
	void SetBkImage(HINSTANCE hInstance,const char *path,const char *path2,COLORREF color,COLORREF tcolor, int type=0);

protected:
	/// �ػ�����
	afx_msg void OnPaint();
	/// ��������
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);

	DECLARE_MESSAGE_MAP()
};

/**
 * �б���ͼ������
 */
class CHappyGoDlgT : public CListCtrl
{
public:
	/**
	 * �б�ͷ�ṹ
	 */
	struct ListHeader
	{
		std::string m_Name;              // ������
		int m_Fmt;                       // �е���ʾλ��
		int m_Width;                     // �е���ʾ���
	};

	/**
	 * ÿ�е�ͷ�ṹ
	 */
	struct ItemHeader
	{
		std::string m_Name;              // ��ͷ������
		int m_ImageId;                   // ����ʹ�õ�ͼƬID
	};

	static CImageList           m_UserStateList;

protected:
	CComBodyClassT               m_ListHeader;
	COLORREF                    m_bkColor;
	COLORREF                    m_TextColor;
	COLORREF                    m_FocusbkColor;
	COLORREF                    m_FocusTextColor;
	std::vector<ListHeader>     m_ListHeaderData;
	std::vector<ItemHeader>     m_ItemHeaderData;
	CFont                       m_Font;                         /**< ��ǰ��ǩʹ�õ����� */
	CImage                      m_bgImage;                      /**< ����ͼƬ */
	CImage                      m_probgImage,m_proImage;        /**< ������ͼƬ */
	std::map<int,int> m_ProgressList;
	int                         m_processpos;
	int                         m_sudu;
	int                         m_curSel;

public:
	/// ���캯��
	CHappyGoDlgT();
	/// ��������
	virtual ~CHappyGoDlgT();
	/// ����Ҫ��ʾ�ı���ͼƬ
	void SetBgImage(HINSTANCE hInstance,const char *path);
	/// ���ù�����λ��
	inline void setProcessPos(int pos) { m_processpos = pos; }
	/// ���ù�������Դ�ļ�
	void SetProcessImage(HINSTANCE hInstance,const char *path,const char *path2);
	/// ��ʼ�б�ؼ�ͷ����
	void InitListHeader(HINSTANCE hInstance,const char *path,const char *path2,COLORREF color,COLORREF tcolor, int type=0);
	/// ��ʼ�б�����
	void InitListCtrl(COLORREF bkColor,COLORREF TextColor,COLORREF FocusbkColor,COLORREF FocusTextColor,
		              std::string resourceimage);
	/// �����б��ͷ
	void ParseListHeader(std::string mHeader);
	/// ���һ����ͷ
	void AddItemHeader(int pImageId,std::string pName);
	/// ������ͷ
	void UpdateItemHeader(int pIndex,int pImageId,std::string pName);
	/// ����ָ���еĽ���
	void SetProcess(int row,float pro);
	/// ���������ļ��Ĵ�С
	void SetFileSize(int row,int size);
	/// ���������ļ����ٶ�
	void SetFileDownload(int row,int size);
	/// ˢ��ѡ��Ŀؼ�
	void RefreshSelectItem(void);

protected:
	//��ȡ��ɫ
	virtual bool GetDrawColor(COLORREF & crTextColor, COLORREF & crBackColor, UINT uRow, bool bSelect);
	//�ؼ���
	virtual void PreSubclassWindow();
	//�滭����
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	//��������
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

#endif

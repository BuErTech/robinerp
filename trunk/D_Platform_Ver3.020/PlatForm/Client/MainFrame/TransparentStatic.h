#ifndef _TRANSPARENTSTATIC_H_INCLUDE
#define _TRANSPARENTSTATIC_H_INCLUDE

#include <string>
#include <vector>

/**
 * ͸����ǩ��
 */
class CTransparentStaticT : public CWnd
{
public:
	/// ���캯��
    CTransparentStaticT();
	/// ��������
	virtual ~CTransparentStaticT();
	
	/// ����������ʾ��ɫ
	void SetTextColor(COLORREF clrFont);
	/// �õ�������ɫ
	COLORREF GetTextColor(void) const { return m_clrFontColor; }
	/// ���ñ�ǩ��ʾ����
	void SetMyText(std::string pText); 
	/// ���ñ�ǩ��ʾ����
	void SetMyNumber(int pNum);

	/// ��ձ���ͼ
	void ClearBackground();

	/// �õ���ǩ��ʾ����
	std::string GetMyText() { return m_Text; }
	/// ���ñ�ǩ�Ķ��뷽ʽ
	void SetHSnapType(int htype) { m_hSnapType = htype; }
	/// �õ���ǩ�Ķ��뷽ʽ
	int GetHSnapType(void) { return m_hSnapType; }
	/// ���ÿؼ��ڸ������е���ʾλ��
	void SetPositonByParents(int sx,int sy) { m_StartX = sx; m_StartY = sy; }
	/// �����Ƿ������ʾ
	inline void SetMultiList(bool isMulti) { m_IsMulitList = isMulti; }
	/// �õ��Ƿ������ʾ
	inline bool GetMultiList(void) { return m_IsMulitList; }

	CFont      m_Font;                         /**< ��ǰ��ǩʹ�õ����� */
private:
	int m_StartX,m_StartY;
	COLORREF  m_clrFontColor;                  /**< ������ʾ��ɫ */
	std::string m_Text;                        /**< Ҫ��ʾ������ */
	std::vector<std::string> m_TextList;       /**< ��Ҫ���ڶ�����ʾ */
	int FontWidth;                             /**< ������ */
	CDC      m_memDC;
	bool m_bDCStored,m_isRgn;//�Ƿ��Ѿ����汳��ͼ
	int   m_hSnapType;
	bool  m_IsMulitList;                      /**< �Ƿ������ʾ */

protected:
	/// �����ָ��λ�ÿ�ʼ�������ַ��ĸ���
	int GetChineseCharCount(std::string str,int startpos);
	/// ����ַ����Ա��ڶ�����ʾ
	void SplitStringToMulitList(void);
	/// ��λͼ�н�������������
	HRGN CreateRgnFromFile( HBITMAP hBmp, COLORREF color );

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()
};

#endif

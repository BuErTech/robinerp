#ifndef _MY_BUTTON_H_INCLUDE
#define _MY_BUTTON_H_INCLUDE

#include <string>

class BzButton : public CButton
{
public:
	typedef enum state
	{
		NORMAL,
		HOVER,
		DOWN,
		DISABLE
	}state;

	/// ���캯��
	BzButton();
	/// ��������
	virtual ~BzButton();

	/// ��������Ҫ��ʾ��ͼƬ
	void SetImage(CString path);
	/// ����Դ�ļ��е���ͼƬ
	void LoadImageFromeResource(HINSTANCE hInstance,UINT nIDResource );
	/// �ı䰴ť����ʾλ��
	void SetPosition(CPoint pos);
	void SetOwnerDraw(bool IsDraw);
	/// ��������Ҫ��ʾ������
	void SetText(std::string pText) { m_Text = pText; }
	/// �õ�����Ҫ��ʾ������
	std::string GetText() { return m_Text; }
	/// ����������ɫ
	void SetTextColor(COLORREF pColor) { m_TextColor = pColor; }
    /// �õ����ֵ���ɫ
	DWORD GetTextColor()  { return m_TextColor; }
    /// ��⵱ǰ�����Ƿ��ڰ�ť�Ŀɼ���Χ��
	bool PointInRect(CPoint pPoint);
	/// ��ձ���ͼ
	void ClearBackground();

private:
	/// ����ť
	void DrawButton(void);

protected:
	CImage m_image;            /**< �����ťʹ�õ�ͼƬ */
	state    m_state;                  /**< ��ǰ��ť��״̬ */
	bool     m_bMouseOver;             /**< ����Ƿ񾭹� */   
	CDC      m_memDC;
	int      m_Mousex,m_Mousey;        /**< ���X��Y */
	int      m_Width,m_Height;         /**< ��ť�߿� */
	bool m_bDCStored,m_isRgn;//�Ƿ��Ѿ����汳��ͼ
	std::string m_Text;
	COLORREF m_TextColor;

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
};

#endif

#pragma once


// CCardShape

class CCardShape : public CWnd
{
	DECLARE_DYNAMIC(CCardShape)

private:
	BYTE			m_byShape;   /**< ���� */
	CGameImage      m_imgShape;  /**< ��ͼƬ */
	CPoint          m_pt;        /**< ����λ�� */
	Image         * m_pImage;
public:
	CCardShape();
	virtual ~CCardShape();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

public:
	/// ������ʾλ��
	void SetPoint(int x, int y);

	/// ��ʾ����
	void ShowCardShape(BYTE byShape);

	//<��������
	void CreateRgn();
};



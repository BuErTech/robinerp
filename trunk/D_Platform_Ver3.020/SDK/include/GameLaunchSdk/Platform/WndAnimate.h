#ifndef _100125_PNG_ANIMAL_H
#define _100125_PNG_ANIMAL_H

#include <vector>
using std::vector;

const int TIME_RENDER = 1001;		/// ��Ⱦ��ʱ��
const int TIME_FRAME = 1002;		/// ��֡��ʱ��
const int RENDER_FLIP = 5;			/// ����ms��Ⱦһ��

/// Memdc��
/// ע�⣺	��������� Create()�� ��һ��Ҫ���� Delete()
/// ע�⣺	����һ��	CMemdc A;
///			�ٴ���һ��	CMemdc B;
///			Ȼ��		B = A;
///			��󣬵���	B.Delete();�Ϳ����ˡ�
class CMemdc
{
public:
	///
	/// ���캯��
	CMemdc(void);
	///
	/// ��������
	~CMemdc(void);
	///
	/// �����ڴ�DC
	/// @param[in]	HDC		hSrcDC		ԴȪ
	/// @param[in]	int		iWidth		�����Ĵ�С
	/// @param[in]	int		iHeight		�����ĸ߶�
	/// @return void
	void Create(HDC hSrcDC, int iWidth, int iHeight);
	///
	/// �õ�����DC
	/// @param	void
	/// @return	const	HDC		����DC
	const HDC GetCompatibleDC(void) const;
	/// 
	/// �õ����
	/// @param	void
	/// @return	int		���ؿ��
	int GetWidth();
	/// �õ��߶�
	/// @param	void
	/// @return	int		���ظ߶�
	int GetHeight();
	///
	/// ɾ���ڴ�DC
	/// param	void
	/// return	void
	void Delete(void);
	///
	/// ��ֵ����
	CMemdc	&operator = (const CMemdc &other);
	///
	/// ���ƹ��캯��
	CMemdc(const CMemdc &other);


private:
	HBITMAP				m_hCompatibleBmp;				///< ����λͼ
	HDC					m_hCompatibleDC;				///< ����DC
	int					m_iWidth;						///< ����λͼ�Ŀ�
	int					m_iHeight;						///< ����λͼ�ĸ�

	void				*m_pData;						///< λͼ���� 
	BITMAPINFO			m_BmpInfo;						///< λͼͼ��
};


/// ������
class EXT_CLASS CWndAnimate : public CWnd
{
public:
	///
	/// ���캯��
	CWndAnimate(void);
	///
	/// ��������
	virtual ~CWndAnimate(void);
private:
	///
	/// ���ƹ��캯��
	CWndAnimate(const CWndAnimate &other);
	///
	/// ��ֵ����
	CWndAnimate &operator = (const CWndAnimate &other);

public:

	/// �Ӱ��ж�ȡImage��ע��Ҫ�ֶ��ͷ� 
	static Image *InitImageFromPkg(const wchar_t *pszFileName);

	///
	/// ����
	///@param[in] CWnd *pParent ������
	///@return bool true �ɹ� false ʧ��
	bool Create(CWnd *pParent);

	///
	/// ����һ��PNGͼƬ
	/// @param[in]		const wchar_t pszFileName*			ͼƬ�ļ���(Unicode)
	void AppendPNG(const wchar_t *pszFileName);	

	///���ʱ������λ�ÿ��ܻ�䣬�˺������е���
	void SetAnimalPos(int iX,int iY) ; 
	/// 
	/// ����һ��PNGͼƬ���ᶨ��С��λ��
	/// @param[in]		const wchar_t	pszFileName*		ͼƬ�ļ���(Unicode)
	/// @param[in]		int				nSrcX
	/// @param[in]		int				nSrcY
	/// @param[in]		int				nWidth
	/// @param[in]		int				nHeight
	void AppendPNG(const wchar_t *pszFileName, int nSrcX, int nSrcY, int nWidth, int nHeight);

	/// ����һ��ͼƬ��ͼƬ����ֳɼ�֡
	/// @param[in]		const wchar_t	pszFileName*		ͼƬ�ļ���(Unicode)
	/// @param[in]		int				nCutInHor			����ֳɼ�֡
	void AppendPNG(const wchar_t *pszFileName, int nCutInHor);

	/// ����һ��ͼƬ��ͼƬ����ֳɼ�֡�����ؼ�ɫ
	/// @param[in]		const wchar_t	pszFileName*		ͼƬ�ļ���(Unicode)
	/// @param[in]		int				nCutInHor			����ֳɼ�֡
	void AppendPNG(const wchar_t *pszFileName, COLORREF clrKey, int nCutInHor);

	///
	/// ��ʼ����
	///@param[in] iX, iY ����
	///@param[in] iWidth, iHeight ��ȸ߶�
	///@param[in] nRound ���Ŷ��ٴ�
	///@param[in] iFlip ÿ֡���
	///@return void
	void BeginAnimate(int iX, int iY, int iWidth, int iHeight, int nRound = 1, int iFlip = 100);

	/// ֹͣ����
	void StopAnimate(void);

	/// ��ʾ���
	void SetShowWindow(bool bShow)
	{
		m_bShowWindow = bShow;
	}

	/// �õ���ʾ���״̬
	bool GetIsShow()
	{
		return m_bShowWindow;
	}

	/// �õ�ĳһ֡��DC�������Լ���
	HDC GetOneFrameDC(int nPos)
	{
		if (nPos>m_FrameList.size())
		{
			return NULL;
		}

		return m_FrameList.at(nPos).GetCompatibleDC();		
	}

	/// �����֡
	int InsertBlankFrame(int iWidth, int iHeight)
	{
		HDC hScreendc = GetDC()->m_hDC;

		CMemdc dc;
		dc.Create(hScreendc, iWidth, iHeight);
		m_FrameList.push_back(dc);
		m_nFrameCount ++;

		return m_FrameList.size();
	}

	/// ����λ��
	void CordinatePos()
	{		
		if (NULL==m_pParent)
		{
			return;
		}
		/// ��������
		POINT dst;
		dst.x = m_iX;//rct.left;
		dst.y = m_iY;//rct.top;
		m_pParent->ClientToScreen(&dst);

		SetWindowPos(m_pParent,dst.x,dst.y,0,0,SWP_NOSIZE);
	}

	/// �Ƿ�������
	bool GetIsPlaying()
	{
		return m_bPlaying;
	}
protected:
	/**
	 * @brief ����������
	 */
	virtual afx_msg void OnPaint();
	/**
	 * @brief ��������ƶ�����λ�á�������һ֡�ȵ�
	 */
	virtual void OnFrameMove();	

	/// 
	/// ��ʱ����Ӧ��Ϣ
	/// @param[in]		UINT_PTR nIDEvent		��ʱ��ID
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	/// ��Ⱦ����
	void RenderAnimate(void);

	/// 
	/// �滭���
	///
	/// �����߳�
	//static DWORD WINAPI AnimateThread(LPVOID lpParam);

protected:
	int							m_iX;
	int							m_iY;
	int							m_iWidth;
	int							m_iHeight;
	int							m_nPos;							///< ������һ֡
	bool						m_bShowWindow;					///< �Ƿ���ʾ


	bool						m_bPlaying;						/// �Ƿ����ڲ���

	CWnd						*m_pParent;						///< ������

private:
	vector<CMemdc>				m_FrameList;					///< ֡����

	int							m_iFlip;						///< ֡����	
	int							m_nFrameCount;					///< ���м�֡
	int							m_nTime;						///< ���ż���
	int							m_nTotalRound;					///< �ܹ����Ŷ��ٴ�

	DECLARE_MESSAGE_MAP()
};

#endif
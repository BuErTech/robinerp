#ifndef _VIDEO_CTRL_CLASS_H_
#define _VIDEO_CTRL_CLASS_H_

#include "VideoWndStyle.h"
#include "VideoWnd.h"

struct ST_INITVIDEO
{
	CWnd		*pParentWnd;
	CString		strUserName;
	CString		strUserPass;
	UINT		nRoomID;
	CString		strRoomPass;
	int			iRoomPassType;
};

interface I_VIDEO
{
public:
	virtual  bool	Video_Initialize(ST_INITVIDEO& stInitVideo)=0;

	virtual  void	Video_Release()=0;
};

class  CVideoCtrl  : public CWnd	 , public I_VIDEO
{
public:
	CVideoCtrl(void);
	virtual ~CVideoCtrl(void);
	virtual  bool	Video_Initialize( ST_INITVIDEO& stInitVideo); 
	/*****************************************
	(IN)  name, ����ǳ�;
	(OUT) rect, �������Ƶ��������;
	���ɹ������rectʱ������true,����false;
	******************************************/
	virtual	 bool	GetVideoRectByName( CString name, RECT& rect, CString& pic)=0;
	virtual  void	Video_Release();

public:
	/// init video
	bool Init(CWnd *pParentWnd, CString strUserName,CString strUserPass,UINT nRoomID,CString strRoomPass,int iRoomPassType); 	
	/// ������Ƶ������
	void SetVideoFrameStyle(GV_VW_STYLE *FrameStyle, GV_VFT_STYLE *ToolbarStyle);
	/// ��ȡ����ͷ��״̬
	int GetCameraState();

	///���ӷ�����
	bool		Connect();
	bool		Login();
	bool		Enterroom();
	bool		OpenLocalVedio();		///< �򿪱�����Ƶ
	void		SetUserPos();			///< ���������Ƶ���ڵ�λ��
	void        Logout();
	void		ShowUserPanelEx(long userid, short bShow, short style, long* pParentWnd);

	/// �û����루�뿪������
	LRESULT     UserAtRoom(WPARAM wParam, LPARAM lParam);
	LRESULT     OnlineUser(WPARAM wParam, LPARAM lParam);
	/// ������Ƶ��Ϣ
	void VedioMsgProc(UINT msg,WPARAM wParam, LPARAM lParam);   

public:
	CString		m_strVedioServerIP;
	UINT		m_nVedioServerPort;
	UINT		m_nRoomID;
	CString     m_strRoomPass;
	CString		m_strUserName;
	CString		m_strUserPass;			 ///< ��½��Ƶ������������
	CString		m_strActiveCode;         ///< ������,����ؼ�
	int			m_iActiveCodeType;       ///< ���������ͣ�0=���ģ�
	int			m_iRoomPassType;		 ///< ������������

	CVideoWnd   *m_pVideoWnd;
	short		m_PanelStyle;            ///< ��Ƶ�����Ƿ����Ӵ���1�ǣ�0����
	long*		m_pParentWnd;

	GV_VW_STYLE	 m_VideoWindowStyle;	 ///< ��Ƶ������
	GV_VFT_STYLE  m_VideoToolbarStyle;	 ///< ��Ƶ��������� 
	int			m_iUserID[MAX_PEOPLE];   ///< ����Լ���id[0];id[n]==-1��ʾ��λ������

	CString		m_strSnapShotSavePath;	 ///< ���ÿ��ձ���·��
	CString		m_strRecordFileSavePath; ///< ����¼���ļ�����·��

	int			m_widthVideo;
	int			m_heightVideo;

protected:
	DECLARE_MESSAGE_MAP() 
	afx_msg LRESULT OnGVClientConnect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGVClientLogin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGVClientEnterRoom(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGVClientMicStateChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGVClientUserAtRoom(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGVClientLinkClose(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGVClientOnlineUser(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGVFortuneMenu(WPARAM wParam, LPARAM lParam);
};

#endif
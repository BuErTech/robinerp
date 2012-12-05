#pragma once

#include "Stdafx.h"
//#include "afcdevhead.h" 
#include "AFCImage.h"
#include "AFCResource.h"

/**
���ߣ�yyf
�������ڣ�2012.10.15
��������������mfc���ĶԻ��򻭵�ͼ��ֻҪ�������׵������ļ���
          ���԰�һ�ŵ�ͼ��9����ķ�ʽ���쵽��Ҫ�Ĵ�С��
		  ���ҶԻ���Ĵ�С�������ļ���Ҳ�������á�

----------�����ļ� configDlg.bcf����-----------

[ConfirmDlg]
;�Ƿ��þŹ���ͼ 0:���ã�1��.
b_UseConfig=1
;Դ��ͼ�Ź����У����ķ��� ���Ͻ� x ƫ��
i_corner_x1=10
;Դ��ͼ�Ź����У����ķ��� ���½� x ƫ��
i_corner_x2=254
;Դ��ͼ�Ź����У����ķ��� ���Ͻ� y ƫ��
i_corner_y1=40
;Դ��ͼ�Ź����У����ķ��� ���½� y ƫ��
i_corner_y2=140
;�Ի���ĸ�
;410
i_Dlg_High=194
;�Ի���Ŀ� 
;250
i_Dlg_Wide=282
;����ͼƬx����
i_title_x=10
;����ͼƬy����
i_title_y=5
;��ͼ (ͼƬ·��)
path_bk=dialog\dialog_bk.png
;����ͼƬ
path_title=dialog\addfrend_title.png

--------------------------------------

ʹ�÷�����
         
(1) ���� #include "..\UILibrary2003\CconfigDlg.h" 
(2) �ڶԻ������� ���� 
	  CconfigDlg  m_configDlg;
	  
(3) �ڶԻ����г�ʼ�� m_configDlg
    ���磺
	 
	// -------��ʼ�� m_configDlg---------  
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%s%sdialog\\configDlg\\XXXXXXXXXX\\configDlg.bcf",s,skinfolder);// �����ļ���·��
	wsprintf(PathDefault,"%s%s",s,skinfolder);//ͼƬ��·��dialogĿ¼���ϼ�Ŀ¼
	m_configDlg.LoadConfig(pathConfig,PathDefault);	

	int cx,cy; 
	cx=m_configDlg.GetDlgWide();
	cy=m_configDlg.GetDlgHigh(); 
	SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOZORDER); 
	//----------------------------------------------------- 

(4) �� OnPaint() �����е��û滭��ͼ.
void XXXX::OnPaint()
{
	CPaintDC dc(this); // device context for painting  

	//�滭��ܵ�ͼ  
	m_configDlg.DrawViewFrame(&dc);
}

(5) �� �Ի�����ԭ���õ� ���磺CGameImage	m_bk; �����ͼ��
    �ں��������г��� m_bk.GetPixel(xxx,yyy) �ĵط�����m_configDlg.GetBKImageEX().GetPixel()�������
*/
class EXT_CLASS CconfigDlg
{
public:
	//�Ƿ��þŹ���ͼ 0:���ã�1��.
	bool m_bUseConfig;

	//�Ի���ĸ�
	int m_iDlgHigh;

	//�Ի���Ŀ�
	int m_iDlgWide;

	// ͼƬĬ���� image\\client\\skinX\\ Ŀ¼�¿�ʼ 
	//�����ļ�·��
	TCHAR m_Str_path_Config[MAX_PATH];
	TCHAR m_Str_PathDefault[MAX_PATH];// ͼƬĬ���� image\\client\\skinX\\ Ŀ¼�¿�ʼ 
	TCHAR m_Str_path_Title[MAX_PATH];//����ͼƬ·��

	//;���þŹ���ͼʱ�����������ͼ  
	TCHAR m_Str_path_9Speed[MAX_PATH];
	CGameImageEx m_img_Defaultbk;//Ĭ�ϵ�ͼ CGameImage
	CGameImageEx m_img_Title;//����ͼ  
	//CGameImage m_img_Defaultbk;//Ĭ�ϵ�ͼ CGameImage
	 
	////��ͼ�Ź����У����Ͻ�
	int m_iCorner_x1 ;//x ƫ��
	int m_iCorner_y1;// y ƫ��
	
	//��ͼ�Ź����У����½� 
	int m_iCorner_x2;//x ƫ��
	int m_iCorner_y2;// y ƫ��

	//����ͼ ��λ��
	int m_iTitle_x;
	int m_iTitle_y;

public:
	CconfigDlg(void);
	virtual ~CconfigDlg(void);
	// ���������ļ���
	void LoadConfig(TCHAR * Path,TCHAR * PathDefault);
	// ����ͼ
	void DrawViewFrame(CDC * pDC, int iWidth=0, int iHeight=0);
	// ���ضԻ���Ŀ�
	inline int GetDlgWide(void)
	{ 
		return m_iDlgWide; 
	};
	// ���ضԻ���ĸ�
	inline int GetDlgHigh(void)
	{ 
		return m_iDlgHigh; 
	};
	////���ر��� image
	inline CGameImageEx& GetBKImageEX(void)
	{ 
		return m_img_Defaultbk;
	};
	//���ر��� image
	inline CGameImage& GetBKImage(void)
	{ 
		return m_img_Defaultbk;
	};
};
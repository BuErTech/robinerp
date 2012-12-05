#include "StdAfx.h"
#include "CconfigDlg.h"


CconfigDlg::CconfigDlg(void)
{
	//�Ƿ��þŹ���ͼ 0:���ã�1��.
	bool b_Use = 0;

	//�Ի���ĸ�
	int i_Dlg_High = 0;

	//�Ի���Ŀ�
	int i_Dlg_Wide = 0;

}


CconfigDlg::~CconfigDlg(void)
{
}


// ���������ļ���
//Path �����ļ�·��
//PathDefault Ĭ��·��
void CconfigDlg::LoadConfig(TCHAR * Path,TCHAR * PathDefault)
{ 
	wsprintf(m_Str_PathDefault,"%s",PathDefault); // ͼƬĬ���� image\\client\\skinX\\ Ŀ¼�¿�ʼ
	 
	//�����ļ�·�� 
	wsprintf(m_Str_path_Config,"%s",Path); 

	CBcfFile forder(m_Str_path_Config);
	 
	//��ͼ (ͼƬ·��) 
	wsprintf(m_Str_path_9Speed,"%s%s",PathDefault,forder.GetKeyVal("ConfirmDlg", "path_bk", "dialog\dialog_bk.png")); 
	m_img_Defaultbk.SetLoadInfo(m_Str_path_9Speed,CGameImageLink::m_bAutoLock);
	
	//����ͼ   
	wsprintf(m_Str_path_Title,"%s%s",PathDefault,forder.GetKeyVal("ConfirmDlg", "path_title", "dialog\title.png")); // ����ͼƬ·��
	m_img_Title.SetLoadInfo(m_Str_path_Title,CGameImageLink::m_bAutoLock); 

	//�Ƿ��þŹ���ͼ 0:���ã�1��.
	 m_bUseConfig = forder.GetKeyVal("ConfirmDlg", "b_UseConfig", 0);

	//�Ի���ĸ�
	 m_iDlgHigh = forder.GetKeyVal("ConfirmDlg", "i_Dlg_High", 0);

	//�Ի���Ŀ�
	 m_iDlgWide = forder.GetKeyVal("ConfirmDlg", "i_Dlg_Wide", 0);
	
	////��ͼ�Ź����У����Ͻ�
	 m_iCorner_x1  = forder.GetKeyVal("ConfirmDlg", "i_corner_x1", 0);//x ƫ��
	 m_iCorner_y1 = forder.GetKeyVal("ConfirmDlg", "i_corner_y1", 0);// y ƫ��
	
	//��ͼ�Ź����У����½� 
	 m_iCorner_x2 = forder.GetKeyVal("ConfirmDlg", "i_corner_x2", 0);//x ƫ��
	 m_iCorner_y2 = forder.GetKeyVal("ConfirmDlg", "i_corner_y2", 0);// y ƫ��

	//����ͼ ��λ��
	 m_iTitle_x = forder.GetKeyVal("ConfirmDlg", "i_title_x", 0);//x ƫ��
	 m_iTitle_y = forder.GetKeyVal("ConfirmDlg", "i_title_y", 0);// y ƫ��
	  
}


// ����ͼ
void CconfigDlg::DrawViewFrame(CDC * pDC, int iWidth, int iHeight)
{
	if(1 == m_bUseConfig)
	{//�þŹ���ͼ

		CGameImageHelper ImageHandle(&m_img_Defaultbk); 

		int iSrcBkHeight = ImageHandle.GetHeight();//Դ����ͼ��
		int iSrcBkWidth = ImageHandle.GetWidth();//Դ����ͼ��

		if( iSrcBkWidth <=0  || iSrcBkHeight <=0 || NULL == pDC)
		{
			return;
		}
		 
		//	
		////�Ի���ĸ�
		// m_iDlgHigh;

		////�Ի���Ŀ�
		// m_iDlgWide;

		//////��ͼ�Ź����У����Ͻ�
		//  m_iCorner_x1 ;//x ƫ��
		//  m_iCorner_y1;// y ƫ��
		//
		////��ͼ�Ź����У����½� 
		//  m_iCorner_x2;//x ƫ��
		//  m_iCorner_y2;// y ƫ��

		//�� �Ź����У����Ͻǡ�
		ImageHandle.BitBlt(pDC->GetSafeHdc(),0,0,m_iCorner_x1-0,m_iCorner_y1-0,0,0);
		//�� �Ź����У����Ͻǡ�
		ImageHandle.BitBlt(pDC->GetSafeHdc(),m_iDlgWide-(iSrcBkWidth - m_iCorner_x2) ,0,iSrcBkWidth - m_iCorner_x2,m_iCorner_y1-0,m_iCorner_x2 ,0);
		//�� �Ź����У����½ǡ�
		ImageHandle.BitBlt(pDC->GetSafeHdc(),0,m_iDlgHigh-(iSrcBkHeight-m_iCorner_y2),m_iCorner_x1,iSrcBkHeight-m_iCorner_y2,0,m_iCorner_y2);
		//�� �Ź����У����½ǡ�
		ImageHandle.BitBlt(pDC->GetSafeHdc(),m_iDlgWide-(iSrcBkWidth - m_iCorner_x2),m_iDlgHigh-(iSrcBkHeight-m_iCorner_y2),iSrcBkWidth - m_iCorner_x2,iSrcBkHeight-m_iCorner_y2,m_iCorner_x2,m_iCorner_y2);
		//�� �Ź����У����к�����
		ImageHandle.StretchBlt(pDC->GetSafeHdc(),m_iCorner_x1,0,m_iDlgWide-(m_iCorner_x1+iSrcBkWidth-m_iCorner_x2),m_iCorner_y1-0,m_iCorner_x1,0,m_iCorner_x2-m_iCorner_x1,m_iCorner_y1-0 );
		//�� �Ź����У����к�����
		ImageHandle.StretchBlt(pDC->GetSafeHdc(),m_iCorner_x1,m_iDlgHigh-(iSrcBkHeight-m_iCorner_y2),m_iDlgWide-(m_iCorner_x1+iSrcBkWidth-m_iCorner_x2),iSrcBkHeight-m_iCorner_y2 ,m_iCorner_x1,m_iCorner_y2,m_iCorner_x2-m_iCorner_x1,iSrcBkHeight-m_iCorner_y2);
		//�� �Ź����У�����������
		ImageHandle.StretchBlt(pDC->GetSafeHdc(),0,m_iCorner_y1,m_iCorner_x1-0,m_iDlgHigh-(m_iCorner_y1+(iSrcBkHeight-m_iCorner_y2)),0,m_iCorner_y1,m_iCorner_x1-0,m_iCorner_y2-m_iCorner_y1 );//�� �Ź����У�����������
		//�� �Ź����У�����������
		ImageHandle.StretchBlt(pDC->GetSafeHdc(),m_iDlgWide-(iSrcBkWidth - m_iCorner_x2),m_iCorner_y1,iSrcBkWidth-m_iCorner_x2,m_iDlgHigh-(m_iCorner_y1+(iSrcBkHeight-m_iCorner_y2)),m_iCorner_x2,m_iCorner_y1,iSrcBkWidth-m_iCorner_x2,m_iCorner_y2-m_iCorner_y1 );
		//�� �Ź����У�����λ������
		ImageHandle.StretchBlt(pDC->GetSafeHdc(),m_iCorner_x1 ,m_iCorner_y1,m_iDlgWide-(m_iCorner_x1+iSrcBkWidth-m_iCorner_x2),m_iDlgHigh-(m_iCorner_y1+(iSrcBkHeight-m_iCorner_y2)),m_iCorner_x1,m_iCorner_y1,m_iCorner_x2-m_iCorner_x1,m_iCorner_y2-m_iCorner_y1 );
		
		
	    ImageHandle.OpenGDIHandle(&m_img_Title);//����ͼ
		//������ͼ
	    ImageHandle.BitBlt(pDC->GetSafeHdc(),m_iTitle_x ,m_iTitle_y ,m_img_Title.GetWidth(),m_img_Title.GetHeight(),0,0);
		
	}
	else
	{//���þŹ���ͼ

		CGameImageHelper ImageHandle(&m_img_Defaultbk);
		ImageHandle.BitBlt(pDC->GetSafeHdc(),0,0,m_iDlgWide,m_iDlgHigh,0,0);

	}
}

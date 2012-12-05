/*
  �ļ���:bz2wui.h
  ���ܣ�BZ2wui SDK �ӿ�����
  ���ߣ�Fred Huang
  ���ڣ�2009.2.2

  �޸���ʷ ��
      1.�޸��ˣ�wushuqun
        ���ڣ�  2009.2.3
        �޸Ĵ���1>Ϊ���нӿں������ӱ�׼ע��;
                2>�ӿں��� wuiPaintControl() ����CDC * pDC ������������ȷ���ؼ�����ȷ��ͼ�������ڶԻ���
                  ����ͼ����ʱ����ֻ��HWND�������ᵼ�¶Ի����ͼ�����贫��CPaintDCָ�롣
      2.wushuqun 2009.2.5
        �޸Ĵ���1>Ϊ���� wuiPaintControl() �¼Ӿ��л�ͼ����
      3.wuhsuqun 2009.2.6
         1>�¼�wuiInit ��wuiUnInit�ӿڣ���Ϊ�����ĳ���ʹ�øÿ�
      4. wushuqun 2009.02.25
		   ��д�ÿ�ʵ�ַ�ʽ��ԭ������CList ����̫�������� map
     5. wushuqun 2009.2.27
	   ȡ���ⲿ wuiSetButtonStatus �ӿڣ��޸�  wuiPaintControl �ӿ�

		 ������   : wuiPaintControl
		 ˵��     : ���ƿؼ�
					���пؼ��Ļ�ͼ������ֻҪָ���ؼ���buc�ļ��е�ID�ţ������Զ�ʶ��ؼ�����
		 �������� : void
		 ����     :  hWnd,���ڿؼ����
				  :  pDC�����ڵ�CDCָ��
				  :  hHandle,�Ѵ򿪵�buc�ļ����
				  :  uID,�ÿؼ���buc�ļ��е�ID��
				  :  exParam,���Ӳ���
						��ť�ؼ�����ʾ��ť��ǰ״̬,ֻ���ǰ�ť�ؼ�ʱ��������
						���ڿؼ�����Ϊ�Ź����ͼ��ʽ
							0,��ʾ��ȫ���վŹ����ͼ��ʽ����Ի���
							1,��ʾ�м乬���ƽ�̲��ֲ����ƣ�����Ϸ��ܣ���߿�
							2,���ԾŹ����ͼ����ͼƽ�̴���
				  :  bForceVisible,�Ƿ�ǿ����ʾ,Ĭ��Ϊfalse,��Ϊtrue��ǿ��ʹ�ؼ��ػ�

		 extern BZ2WUI_API void wuiPaintControl(HWND hWnd,CDC * pDC,DWORD hHandle,UINT uID,BYTE exParam = 0,bool bForceVisible=false,bool bCenterDraw=false);
	 
	 6.wushuqun 2009.3.4
	   �¼�wuiDrawImageById��ͼ�ӿ�
		 
		 ������   : wuiDrawImageById
		 ����     : wushuqun 
		 ����     : 2009.3.4
		 ˵��     : ֱ�ӻ�ͼ����ͼ��СΪuID����ͼ�Ĵ�С,��͸����ͼ
		 �������� : void 
		 ����     : hHandle,�Ѵ򿪵�buc�ļ����
				  : uID,�ÿؼ���buc�ļ��е�ID��
				  : desX,Ŀ��X��ʼ����
				  : desY,Ŀ��Y��ʼ����
				  : pDC,Ŀ��DC
				  : transColor,ָ��͸��ɫ��Ĭ��ΪRGB(255,0,255);
		extern BZ2WUI_API void wuiDrawImageById(DWORD hHandle,UINT uID,CDC * pDC,int desX,int desY,COLORREF transColor = 0xFF00FF);

		 ������   : wuiDrawImageById
		 ����     : wushuqun 
		 ����     : 2009.3.4
		 ˵��     : ��pDC�ϻ�ͼ���������ţ��趨͸��ɫ
		 �������� : void 
		 ����     : hHandle,�Ѵ򿪵�buc�ļ����
				  : uID,�ÿؼ���buc�ļ��е�ID��
				  : desX,Ŀ��X��ʼ����
				  : desY,Ŀ��Y��ʼ����
				  : desWidth,Ԥ���ƿ��
				  : desHeight,Ԥ���Ƹ߶�
				  : pDC,Ŀ��DC
				  : transColor,ָ��͸��ɫ��Ĭ��ΪRGB(255,0,255);
		extern BZ2WUI_API void wuiDrawImageById(DWORD hHandle,UINT uID,CDC * pDC,int desX,int desY,int desWidth,int desHeight,COLORREF transColor = 0xFF00FF);

*/
#pragma once

#ifdef BZ2WUI_EXPORTS
#define BZ2WUI_API __declspec(dllexport)
#else
#define BZ2WUI_API __declspec(dllimport)
#endif

#define WUI_ERR_SUCCEED		0x00
#define WUI_ERR_NOTFOUNDWUI	0x01
#define WUI_ERR_ADDNULLCONTROL	0x02
#define WUI_ERR_NOCONTROLDEFINE	0x03
#define WUI_ERR_CONTROLEXIST	0x04
#define WUI_ERR_CONTROLNOMATCH	0x05

////////////////////////////////////////
// BZ2wuiSDK  �ⲿ�ӿ�

/*
 ������   : wuiInit
 ˵��     : ��ʼ��GDI+����,�ڲ�����Ϸ�иò����������������ٴε��ã�
            ��Ϊ������������Ҫ��Ӧ�ó����InitInstance�����е���Щ����,�����޷����BZ2wui SDK
			ָ���Ĺ���
 �������� : bool
             = true ʱ����ʼ���ɹ�
			 = false ʱ����ʼ��ʧ��
 */
extern BZ2WUI_API bool wuiInit();

/*
 ������   : wuiUnInit
 ˵��     : �ر�GDI+��ͼ����
            ������Ϸ���Ѵ���ò��裬�����ٴε���
			��Ϊ��������������Ӧ�ó�������˳�ʱ���ô˺���
 �������� : void 
 */
extern BZ2WUI_API void wuiUnInit();

/*
 ������   : wuiOpen
 ˵��     : ��һ��buc�ļ�
 �������� : DWORD 
            != 0 ʱ��Ϊ���ļ��ɹ�ʱ���ļ����
			 = 0 ʱ�����ļ�ʧ��
 ����     : 
           szFilename,buc�ļ���
 */
extern BZ2WUI_API DWORD wuiOpen(LPCTSTR szFilename);

/*
 ������   : wuiOpen
 ˵��     : �ر�һ���ļ����ΪhHandle��ָ��buc�ļ�
 �������� : void 
 ����     : 
           hHandle,buc�ļ����
 */
extern BZ2WUI_API void wuiClose(DWORD hHandle);

/*
 ������   : wuiSetControlRgn
 ˵��     : ���ÿؼ���������,�Զ����ε���ɫֵΪ0xFF00FF ����ɫ 
 �������� : void 
 ����     : hWnd,�ؼ����ھ��
          : hHandle,�Ѵ򿪵�buc�ļ����
          : uID,�ÿؼ���buc�ļ���ָ����ID��
 */
extern BZ2WUI_API void wuiSetControlRgn(HWND hWnd,DWORD hHandle,UINT uID);


/*
 ������   : wuiPaintControl
 ˵��     : ���ƿؼ�
            ���пؼ��Ļ�ͼ������ֻҪָ���ؼ���buc�ļ��е�ID�ţ������Զ�ʶ��ؼ�����
 �������� : void
 ����     :  hWnd,���ڿؼ����
          :  pDC�����ڵ�CDCָ��
          :  hHandle,�Ѵ򿪵�buc�ļ����
	  :  uID,�ÿؼ���buc�ļ��е�ID��
	  :  exParam,���Ӳ���
		��ť�ؼ�����ʾ��ť��ǰ״̬,ֻ���ǰ�ť�ؼ�ʱ��������
		���ڿؼ�����Ϊ�Ź����ͼ��ʽ
			0,��ʾ��ȫ���վŹ����ͼ��ʽ����Ի���
			1,��ʾ�м乬���ƽ�̲��ֲ����ƣ�����Ϸ��ܣ���߿�
			2,���ԾŹ����ͼ����ͼƽ�̴���
          :  bForceVisible,�Ƿ�ǿ����ʾ,Ĭ��Ϊfalse,��Ϊtrue��ǿ��ʹ�ؼ��ػ�
*/
extern BZ2WUI_API void wuiPaintControl(HWND hWnd,CDC * pDC,DWORD hHandle,UINT uID,BYTE exParam = 0,bool bForceVisible=false,bool bCenterDraw=false);


/*
 ������   : wuiSetVisiable
 ˵��     : ���ÿؼ��Ƿ������ʾ���������ı���Ч
 �������� : void 
 ����     :  hHandle,�Ѵ򿪵�buc�ļ����
          :  uID,�ÿؼ���buc�ļ��е�ID��
          :  bVisiable,�Ƿ���ʾ,Ĭ��trueΪ��ʾ,falseΪ����ʾ
 */
extern BZ2WUI_API void wuiSetVisiable(DWORD hHandle,UINT uID,BOOL bVisiable=TRUE);

/*
 ������   : wuiSetText
 ˵��     : ���ÿؼ����ı���ֻ���ı��ؼ���Ч
 �������� : void 
 ����     :  hHandle,�Ѵ򿪵�buc�ļ����
          :  uID,�ÿؼ���buc�ļ��е�ID��
          :  strText,�ؼ��ϵ��ı�
*/
extern BZ2WUI_API void wuiSetText(DWORD hHandle,UINT uID,CString strText);

/*
 ������   : wuiSetButtonStatus
 ˵��     : ���ÿؼ��İ�ť״̬��ֻ�԰�ť�ؼ���Ч
            ��ť״̬�밴ťͼƬ�Ķ�Ӧ˳���Ըð�ťͼƬΪ׼		                       
 �������� : void 
 ����     :  hHandle,�Ѵ򿪵�buc�ļ����
          :  uID,�ÿؼ���buc�ļ��е�ID��
          :  uStatus,״̬���֣����������ĸ�ֵ:
			    0�������һ��״̬
				1������ڶ���״̬
				2�����������״̬
				3��������ĸ�״̬	
 */
extern BZ2WUI_API void wuiSetButtonStatus(DWORD hHandle,UINT uID,int uStatus);

/*
 ������   : wuiSetRuntimePath
 ˵��     : ���������ļ�·�������ڵ�ǰĿ¼���Ҳ�����Ӧ����Դ�ļ�ʱ���Դ�sPath��ָ
            ��Ŀ¼��ȥ��
 �������� : void 
 ����     :  hHandle,�Ѵ򿪵�buc�ļ����
          :  sPath,��ָ��Ŀ¼�ַ���
*/
extern BZ2WUI_API void wuiSetRuntimePath(DWORD hHandle,CString sPath);

/*
 ������   : wuiGetControlImageSizeById
 ����     : wushuqun 
 ����     : 2009.2.4
 ˵��     : �õ��ؼ�ͼƬ�����Ͻ�λ�ü��ߴ�
 �������� : void 
 ����     : hHandle,�Ѵ򿪵�buc�ļ����
          : uID,�ÿؼ���buc�ļ��е�ID��
          : leftPt,�����CPoint �������ã����ÿؼ�ͼƬ������ͼ�е����Ͻ�λ��д��ñ���
          : size,�����CSize�������ã����ÿؼ�ͼƬ�ĳߴ�(����)д��ñ���
*/
extern BZ2WUI_API void wuiGetControlImageSizeById(DWORD hHandle,UINT uID,CPoint & leftPt,CSize & size);

/*
 ������   : wuiDrawImageById
 ����     : wushuqun 
 ����     : 2009.3.4
 ˵��     : ֱ�ӻ�ͼ����ͼ��СΪuID����ͼ�Ĵ�С,��͸����ͼ
 �������� : BOOL 
 ����     : hHandle,�Ѵ򿪵�buc�ļ����
          : uID,�ÿؼ���buc�ļ��е�ID��
          : desX,Ŀ��X��ʼ����
          : desY,Ŀ��Y��ʼ����
		  : pDC,Ŀ��DC
		  : transColor,ָ��͸��ɫ��Ĭ��ΪRGB(255,0,255);
*/
extern BZ2WUI_API BOOL wuiDrawImageById(DWORD hHandle,UINT uID,CDC * pDC,int desX,int desY,COLORREF transColor = 0xFF00FF);

/*
 ������   : wuiDrawImageById
 ����     : wushuqun 
 ����     : 2009.3.4
 ˵��     : ��pDC�ϻ�ͼ���������ţ��趨͸��ɫ
 �������� : BOOL 
 ����     : hHandle,�Ѵ򿪵�buc�ļ����
          : uID,�ÿؼ���buc�ļ��е�ID��
          : desX,Ŀ��X��ʼ����
          : desY,Ŀ��Y��ʼ����
		  : desWidth,Ԥ���ƿ��
		  : desHeight,Ԥ���Ƹ߶�
		  : pDC,Ŀ��DC
		  : transColor,ָ��͸��ɫ��Ĭ��ΪRGB(255,0,255);
*/
extern BZ2WUI_API BOOL wuiDrawImageById(DWORD hHandle,UINT uID,CDC * pDC,int desX,int desY,int desWidth,int desHeight,COLORREF transColor = 0xFF00FF);


#define WM_MOUSEPOSITION WM_USER+5000

class AFX_EXT_CLASS CMouseHook:public CObject  
{  
public:  
	CMouseHook();	//������Ĺ��캯��  
	~CMouseHook();	//���������������  
	BOOL StartHook(HWND hWnd);	//��װ���Ӻ���  
	BOOL StopHook();	//ж�ع��Ӻ���  
};  

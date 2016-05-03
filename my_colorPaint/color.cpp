#include <windows.h>
#include <windowsx.h>
#include "resource.h"

//��������
BOOL InitWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK MainWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ColorDlgProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

static int iCurrentColor[5]  = {IDC_WHITE, IDC_BLACK};// ������ɫ���ı���ɫ
BOOL Bg = FALSE;

COLORREF GetMyColor(HWND hWnd)
{
	// �����Զ���Ի���

	COLORREF	 cr = 0;
	CHOOSECOLOR  cc;
	COLORREF     crBackColors[16];

	//��ʼ��CHOOSECOLOR�ṹ���� cc
	cc.lStructSize    = sizeof (CHOOSECOLOR);
	cc.hwndOwner      = hWnd;
	cc.hInstance      = NULL;
	cc.rgbResult      = RGB (0, 0, 0);
	cc.lpCustColors   = crBackColors;
	cc.Flags          = CC_RGBINIT | CC_FULLOPEN;
	cc.lCustData      = 0;
	cc.lpfnHook       = NULL;
	cc.lpTemplateName = NULL;

	if (ChooseColor (&cc))
		cr = cc.rgbResult;
	return cr;
};

COLORREF g_Color;
/*******************************************************************
������WinMain ()
���ܣ�Win32 Ӧ�ó�����ں��������������ڣ�������Ϣѭ��
********************************************************************/
int WINAPI WinMain(HINSTANCE hInstance,         //��ǰʵ�����
	HINSTANCE hPrevInstance,     //ǰһ��ʵ�����
	PSTR szCmdLine,              //�������ַ�
	int iCmdShow)                //������ʾ��ʽ
{
	MSG msg;

	//����������
	if (!InitWindow(hInstance, iCmdShow))
		return FALSE;

	//������Ϣѭ�����Ӹ�Ӧ�ó������Ϣ�����м�ȡ��Ϣ��
	//�͵���Ϣ������̣�����ȡ��WM_QUIT��Ϣʱ���˳���Ϣѭ����
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

/******************************************************************
������InitWindow ()
���ܣ��������ڡ�
*******************************************************************/
BOOL InitWindow(HINSTANCE hInstance, int iCmdShow)
{
	TCHAR szAppName[] = TEXT("dialog");		  //Ӧ�ó�������
	HWND hwnd;                                    //���ھ��
	WNDCLASS wcMainWnd;                           //������ṹ
	ATOM a;

	//��䴰����ṹ
	wcMainWnd.style = CS_VREDRAW | CS_HREDRAW;
	wcMainWnd.lpfnWndProc = MainWndProc ;
	wcMainWnd.cbClsExtra = 0;
	wcMainWnd.cbWndExtra = 0;
	wcMainWnd.hInstance = hInstance;
	wcMainWnd.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcMainWnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcMainWnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcMainWnd.lpszMenuName = NULL;
	wcMainWnd.lpszClassName = szAppName;

	//ע�ᴰ����
	a = RegisterClass(&wcMainWnd);

	if (!a)
	{
		MessageBox(NULL, TEXT("ע�ᴰ����ʧ�ܣ�"), szAppName,
			MB_ICONERROR);
		return 0;
	}

	//����������
	hwnd = CreateWindow(szAppName, 						//����������
		TEXT("Windows �Ի���"),  //���ڱ���
		WS_OVERLAPPEDWINDOW,				//���ڷ��
		100,								//����λ�õ�x����
		100,								//����λ�õ�y����
		400,								//���ڵĿ��
		300,								//���ڵĸ߶�
		NULL,								//�����ھ��
		LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1)),								//�˵����
		hInstance,							//Ӧ�ó���ʵ�����
		NULL);								//���ڴ�������ָ��

	if (!hwnd) return FALSE;

	//��ʾ�����´���
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	return TRUE;
}

/******************************************************************
������MainWndProc  ()
���ܣ�������������Ϣ
*******************************************************************/
LRESULT CALLBACK MainWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND  hWndEdit;
	HDC         hdc;
	PAINTSTRUCT ps;
	RECT        rect;
	static HINSTANCE    hInstance;
	static COLORREF     crBackColors[8] ={RGB(255, 0, 0), RGB(0, 255, 0),
		RGB(0, 0, 255), RGB(255, 255, 0),
		RGB(255, 0, 255), RGB(0, 255, 255),
		RGB(0, 0, 0), RGB(255, 255, 255)};
	// static int dwColor;
	HBRUSH hNewBrush = WHITE_BRUSH, hOldBrush;;
	static CHOOSECOLOR  cc;
	static COLORREF     crCustomColors[16];
	static COLORREF     crText = RGB (0, 0, 0);

	switch (message)
	{
		case WM_CREATE:
			//��ʼ���ṹ cc
			cc.lStructSize    = sizeof (CHOOSECOLOR);
			cc.hwndOwner      = hWnd;
			cc.hInstance      = NULL;
			cc.rgbResult      = RGB (0, 0, 0);
			cc.lpCustColors   = crCustomColors;
			cc.Flags          = CC_RGBINIT | CC_FULLOPEN;
			cc.lCustData      = 0;
			cc.lpfnHook       = NULL;
			cc.lpTemplateName = NULL;

			return 0;

	case  WM_COMMAND:
		if (lParam == 0) // �˵�
		{
			switch(wParam)
			{
			case ID_changeBackground:
				Bg = FALSE;
				if (DialogBox (hInstance, MAKEINTRESOURCE (ColorDlg), hWnd, ColorDlgProc))
					InvalidateRect (hWnd, NULL, TRUE);
				return 0;

			case ID_changeText:
				Bg = TRUE;
				if (DialogBox (hInstance, MAKEINTRESOURCE (ColorDlg), hWnd, ColorDlgProc))
					InvalidateRect (hWnd, NULL, TRUE);
				return 0;

			case ID_changeBBk:
				if (ChooseColor (&cc))
				{
					crText = crCustomColors[0];
					hNewBrush = CreateSolidBrush(crText);
					SetClassLong(hWnd, GCL_HBRBACKGROUND, (LONG) hNewBrush);
					InvalidateRect (hWnd, NULL, TRUE);
				}
				return 0;
			}
		}
		return 0;

	case WM_PAINT: 
		hdc = BeginPaint (hWnd, &ps); 
		GetClientRect (hWnd, &rect);
		SetBkColor (hdc, crBackColors[iCurrentColor[0]-IDC_RED]);
		SetTextColor(hdc, crBackColors[iCurrentColor[1]-IDC_RED]);

		DrawText (hdc, TEXT ("�ı���ɫ���ؼ�����ɫ���ͻ�������ɫ�ĸı�"),
			-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint (hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	//����ȱʡ��Ϣ�������
	return DefWindowProc(hWnd, message, wParam, lParam);

} //���� WinProc ����

BOOL CALLBACK ColorDlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int  iColor, iFigure;

	switch (message)
	{
	case WM_INITDIALOG:
		iColor  = iCurrentColor[Bg];
		CheckRadioButton (hDlg, IDC_RED, IDC_BLACK, iColor);
		SetFocus (GetDlgItem (hDlg, iColor));
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			iCurrentColor[Bg]  = iColor;
			EndDialog (hDlg, TRUE);
			return TRUE;

		case IDCANCEL:
			EndDialog (hDlg, FALSE);
			return TRUE;

		case IDC_RED:
		case IDC_GREEN:
		case IDC_BLUE:
		case IDC_YELLOW:
		case IDC_MAGENTA:
		case IDC_CYAN:
		case IDC_WHITE:
			iColor = LOWORD (wParam);
			CheckRadioButton (hDlg, IDC_RED, IDC_WHITE, LOWORD (wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;

}//���� ColorDlgProc

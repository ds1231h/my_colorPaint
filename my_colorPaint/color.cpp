#include <windows.h>
#include <windowsx.h>
#include "resource.h"

//函数声明
BOOL InitWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK MainWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ColorDlgProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

static int iCurrentColor[5]  = {IDC_WHITE, IDC_BLACK};// 背景颜色与文本颜色
BOOL Bg = FALSE;

COLORREF GetMyColor(HWND hWnd)
{
	// 启动自定义对话框

	COLORREF	 cr = 0;
	CHOOSECOLOR  cc;
	COLORREF     crBackColors[16];

	//初始化CHOOSECOLOR结构变量 cc
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
函数：WinMain ()
功能：Win32 应用程序入口函数。创建主窗口，处理消息循环
********************************************************************/
int WINAPI WinMain(HINSTANCE hInstance,         //当前实例句柄
	HINSTANCE hPrevInstance,     //前一个实例句柄
	PSTR szCmdLine,              //命令行字符
	int iCmdShow)                //窗口显示方式
{
	MSG msg;

	//创建主窗口
	if (!InitWindow(hInstance, iCmdShow))
		return FALSE;

	//进入消息循环：从该应用程序的消息队列中检取消息，
	//送到消息处理过程，当检取到WM_QUIT消息时，退出消息循环。
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

/******************************************************************
函数：InitWindow ()
功能：创建窗口。
*******************************************************************/
BOOL InitWindow(HINSTANCE hInstance, int iCmdShow)
{
	TCHAR szAppName[] = TEXT("dialog");		  //应用程序名称
	HWND hwnd;                                    //窗口句柄
	WNDCLASS wcMainWnd;                           //窗口类结构
	ATOM a;

	//填充窗口类结构
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

	//注册窗口类
	a = RegisterClass(&wcMainWnd);

	if (!a)
	{
		MessageBox(NULL, TEXT("注册窗口类失败！"), szAppName,
			MB_ICONERROR);
		return 0;
	}

	//创建主窗口
	hwnd = CreateWindow(szAppName, 						//窗口类名称
		TEXT("Windows 对话框"),  //窗口标题
		WS_OVERLAPPEDWINDOW,				//窗口风格
		100,								//窗口位置的x坐标
		100,								//窗口位置的y坐标
		400,								//窗口的宽度
		300,								//窗口的高度
		NULL,								//父窗口句柄
		LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1)),								//菜单句柄
		hInstance,							//应用程序实例句柄
		NULL);								//窗口创建数据指针

	if (!hwnd) return FALSE;

	//显示并更新窗口
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	return TRUE;
}

/******************************************************************
函数：MainWndProc  ()
功能：处理主窗口消息
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
			//初始化结构 cc
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
		if (lParam == 0) // 菜单
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

		DrawText (hdc, TEXT ("文本颜色、控件背景色、客户区背景色的改变"),
			-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint (hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	//调用缺省消息处理过程
	return DefWindowProc(hWnd, message, wParam, lParam);

} //函数 WinProc 结束

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

}//结束 ColorDlgProc

#include "boardGUI.h"
#include "sharedMemory.h"
#include "GUI.h"
#include "resource.h"
#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK TrataEventos(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK TrataEventosConfig(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
LRESULT	CALLBACK TrataEventosSobre(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);

TCHAR szProgName[] = TEXT("Base");

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpCmdLine, int nCmdShow) {
	HWND hWnd;
	MSG lpMsg;
	WNDCLASSEX wcApp;
	BOARD board;
	
	wcApp.cbSize = sizeof(WNDCLASSEX);
	wcApp.hInstance = hInst;
	wcApp.lpszClassName = szProgName;
	wcApp.lpfnWndProc = TrataEventos;
	wcApp.style = CS_HREDRAW | CS_VREDRAW;
	wcApp.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcApp.hIconSm = LoadIcon(NULL, IDI_INFORMATION);
	wcApp.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcApp.lpszMenuName = MAKEINTRESOURCE(IDR_MENU_PRINCIPAL);
	wcApp.cbClsExtra = 0;
	wcApp.cbWndExtra = 0;
	wcApp.hbrBackground = CreateSolidBrush(RGB(220, 220, 220));

	if (!RegisterClassEx(&wcApp))
		return(0);

	hWnd = CreateWindow(szProgName,
		TEXT("BoardGui"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		500,
		500,
		(HWND)HWND_DESKTOP,
		(HMENU)NULL,
		(HINSTANCE)hInst,
		0
	);

	try {
		SharedMemory::connect(board);
	} catch (std::TSTRING e) {
		MessageBox(hWnd, (LPWSTR)&e, TEXT("Erro"), MB_OK | MB_ICONERROR);
		return 0;
	}
	//TODO: check if board is connected

	LONG_PTR ptr = SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)&board);

	ShowWindow(hWnd, nCmdShow);	
	UpdateWindow(hWnd);

	while (GetMessage(&lpMsg, NULL, 0, 0) > 0) {
		TranslateMessage(&lpMsg);
		DispatchMessage(&lpMsg);
	}

	return (int)lpMsg.wParam;
}

LRESULT CALLBACK TrataEventos(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
	BOARD* board = (BOARD*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	HDC hdc;
	RECT rect;
	PAINTSTRUCT ps;

	switch (messg) {
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case ID_CONFIG:
					DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG_CONFIG), hWnd, TrataEventosConfig);
					break;

				case ID_SOBRE:
					DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG_SOBRE), hWnd, TrataEventosSobre);
					break;
			}

			break;

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rect);

			//FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

			//int barWidth = (rect.right - rect.left) / MAX_COMPANIES;
			//int maxBarHeight = rect.bottom - 100; // 100 pixels for margins and labels

			//double maxValue = 0;
			//for (int i = 0; i < MAX_COMPANIES; ++i) {
			//	if (board->sharedMemory->companies[i].pricePerStock > maxValue) {
			//		maxValue = board->sharedMemory->companies[i].pricePerStock;
			//	}
			//}

			//for (int i = 0; i < MAX_COMPANIES; ++i) {
			//	int barHeight = (int)(board->sharedMemory->companies[i].pricePerStock / maxValue * maxBarHeight);
			//	RECT barRect = {
			//		rect.left + i * barWidth,
			//		rect.bottom - barHeight,
			//		rect.left + (i + 1) * barWidth,
			//		rect.bottom
			//	};

			//	FillRect(hdc, &barRect, (HBRUSH)(COLOR_ACTIVECAPTION + 1));

			//	std::TSTRING companyName = board->sharedMemory->companies[i].name;
			//	DrawText(hdc, companyName.c_str(), -1, &barRect, DT_CENTER | DT_BOTTOM);
			//}

			EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return(DefWindowProc(hWnd, messg, wParam, lParam));
			break;
	}
	return 0;
}

LRESULT CALLBACK TrataEventosConfig(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
	switch (messg) {
		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return TRUE;
	}

	return FALSE;
}

LRESULT	CALLBACK TrataEventosSobre(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
	switch (messg) {
		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return TRUE;
	}

	return FALSE;
}

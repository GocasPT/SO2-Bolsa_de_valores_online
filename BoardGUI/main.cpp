#include "boardGUI.h"
#include "sharedMemory.h"
#include "resource.h"
#include <windows.h>
#include <tchar.h>
#include <stdexcept>

LRESULT CALLBACK TrataEventos(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK TrataEventosConfig(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK TrataEventosSobre(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);

TCHAR szProgName[] = TEXT("Base");

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpCmdLine, int nCmdShow) {
    HWND hWnd;
    MSG lpMsg;
    WNDCLASSEX wcApp;

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

    if (!RegisterClassEx(&wcApp)) return 0;

    hWnd = CreateWindow(szProgName, TEXT("BoardGui"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 500, HWND_DESKTOP, NULL, hInst, NULL);
    if (!hWnd) return 0;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&lpMsg, NULL, 0, 0) > 0) {
        TranslateMessage(&lpMsg);
        DispatchMessage(&lpMsg);
    }

    return (int)lpMsg.wParam;
}

DWORD WINAPI updateBoard(LPVOID lpParam) {
    BOARD* board = (BOARD*)lpParam;

    while (board->isRunning) {
        WaitForSingleObject(board->hMutex, INFINITE);
        SharedMemory::read(*board);
        ReleaseMutex(board->hMutex);
        InvalidateRect(board->hWnd, NULL, TRUE);
    }

    return 0;
}

void DrawBoard(HWND hWnd, BOARD& board) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    RECT rect;
    GetClientRect(hWnd, &rect);

    WaitForSingleObject(board.hMutex, INFINITE);

    FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));

    int barHeight = (rect.bottom - rect.top) / (board.N + 1);
    int maxBarWidth = (rect.right - 150) * board.scale / 100;

    double maxValue = 0;
    for (int i = 0; i < board.N; ++i) {
        if (board.sharedMemory->companies[i].pricePerStock > maxValue) {
            maxValue = board.sharedMemory->companies[i].pricePerStock;
        }
    }

    for (int i = 0; i < board.N; ++i) {
        int barWidth = static_cast<int>(board.sharedMemory->companies[i].pricePerStock / maxValue * maxBarWidth);
        RECT barRect = {
            rect.left + 100,
            rect.top + (i + 1) * barHeight,
            rect.left + 100 + barWidth,
            rect.top + (i + 1) * barHeight + barHeight - 10
        };

        FillRect(hdc, &barRect, (HBRUSH)(COLOR_ACTIVECAPTION + 1));

        std::wstring companyName = board.sharedMemory->companies[i].name;
        RECT textRect = { rect.left, rect.top + (i + 1) * barHeight, rect.left + 90, rect.top + (i + 1) * barHeight + barHeight - 10 };
        DrawText(hdc, companyName.c_str(), -1, &textRect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

        std::wstring priceText = std::to_wstring(board.sharedMemory->companies[i].pricePerStock);
        RECT priceRect = { rect.left + 100 + barWidth + 5, rect.top + (i + 1) * barHeight, rect.right, rect.top + (i + 1) * barHeight + barHeight - 10 };
        DrawText(hdc, priceText.c_str(), -1, &priceRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    }

    ReleaseMutex(board.hMutex);
    EndPaint(hWnd, &ps);
}


LRESULT CALLBACK TrataEventos(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
    static BOARD board;

    switch (messg) {
    case WM_CREATE:
        try {
            board.isRunning = true;
            board.N = 10;
			board.scale = 100;
            board.hWnd = hWnd;
            board.hMutex = CreateMutex(NULL, FALSE, NULL);
            if (!board.hMutex) throw _T("CreateMutex failed");

            SharedMemory::connect(board);

            HANDLE hThread = CreateThread(NULL, 0, updateBoard, &board, 0, NULL);
            if (!hThread) throw _T("CreateThread failed");
            CloseHandle(hThread);
        } catch (std::TSTRING e) {
            MessageBox(hWnd, (LPCWSTR)&e, TEXT("Error"), MB_OK | MB_ICONERROR);
            PostQuitMessage(0);
            return 0;
        }
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_CONFIG:
            DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_DIALOG_CONFIG), hWnd, TrataEventosConfig, (LPARAM)&board);
            break;
        case ID_SOBRE:
            DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG_SOBRE), hWnd, TrataEventosSobre);
            break;
        }
        break;

    case WM_PAINT:
        DrawBoard(hWnd, board);
        break;

    case WM_DESTROY:
        board.isRunning = false;
        CloseHandle(board.hMutex);
		SharedMemory::close(board);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, messg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK TrataEventosConfig(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
    static BOARD* pBoard;

    switch (messg) {
    case WM_INITDIALOG:
        pBoard = (BOARD*)lParam;
        SetDlgItemInt(hWnd, IDC_EDIT_N, pBoard->N, FALSE);
        SetDlgItemInt(hWnd, IDC_EDIT_SCALE, pBoard->scale, FALSE);
        return TRUE;

    case WM_COMMAND:
        if (HIWORD(wParam) == EN_CHANGE) {
            int id = LOWORD(wParam);
            if (id == IDC_EDIT_N || id == IDC_EDIT_SCALE) {
                BOOL success;
                int value = GetDlgItemInt(hWnd, id, &success, FALSE);
                if (success) {
                    if (id == IDC_EDIT_N && (value < 0 || value > 10)) {
                        SetDlgItemInt(hWnd, IDC_EDIT_N, (value < 0) ? 0 : 10, FALSE);
                    } else if (id == IDC_EDIT_SCALE && (value < 0 || value > 100)) {
                        SetDlgItemInt(hWnd, IDC_EDIT_SCALE, (value < 0) ? 0 : 100, FALSE);
                    }
                }
            }
        }
        switch (LOWORD(wParam)) {
        case IDOK:
        {
            BOOL success;
            int newN = GetDlgItemInt(hWnd, IDC_EDIT_N, &success, FALSE);
            int newScale = GetDlgItemInt(hWnd, IDC_EDIT_SCALE, &success, FALSE);
            if (success && newN >= 0 && newN <= 10 && newScale >= 0 && newScale <= 100) {
                WaitForSingleObject(pBoard->hMutex, INFINITE);
                pBoard->N = newN;
                pBoard->scale = newScale;
                ReleaseMutex(pBoard->hMutex);
                InvalidateRect(pBoard->hWnd, NULL, TRUE);
            }
            EndDialog(hWnd, IDOK);
            return TRUE;
        }
        case IDCANCEL:
            EndDialog(hWnd, IDCANCEL);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

LRESULT CALLBACK TrataEventosSobre(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
    if (messg == WM_CLOSE) {
        EndDialog(hWnd, 0);
        return TRUE;
    }
    return FALSE;
}

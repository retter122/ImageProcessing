#include <iostream>
#include <windows.h>
#include <ctime>
#include <cmath>

#include "./interface/interface.h"


// FUNCTIONS PROTOTYPE
static LRESULT WProc(HWND hWnd, UINT Mess, WPARAM WPar, LPARAM LPar);


// WINDOW CONSTANT
static const char WName[] = "Image Processing";

#define WSizeScreenAsp 3 / 4
#define WPosScreenAsp 1 / 6


// WINDOW DATA
static WNDCLASSA WClass = { CS_HREDRAW | CS_VREDRAW, WProc, 0, 0, 0, 0, 0, 0, 0, "Image Processing" };
static MSG Msg = { 0 };

static HDC Dc = 0, MemDC = 0;
static HBITMAP MemBm = 0;
static PAINTSTRUCT PStruct = { 0 };

static RECT WSize = { 0 };


// MAIN FUNCTION
int main() {
    WSize.right = GetSystemMetrics(SM_CXSCREEN) * WSizeScreenAsp, WSize.bottom = GetSystemMetrics(SM_CYSCREEN) * WSizeScreenAsp;

    // WINDOW INIT
    if (!RegisterClassA(&WClass));
    ShowWindow(CreateWindowExA(WS_EX_COMPOSITED, WClass.lpszClassName, WName, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, WSize.right * WPosScreenAsp, WSize.bottom * WPosScreenAsp, WSize.right, WSize.bottom, 0, 0, 0, 0), SW_SHOWNORMAL);

    // APP MAIN CYCLE
    int32_t last_time = clock();
    while (GetMessageA(&Msg, 0, 0, 0)) {
        DispatchMessageA(&Msg);
        TranslateMessage(&Msg);

        if (Msg.message == WM_QUIT) break;
    } DeleteInterface();

    return 0;
}


// MESSAGE PROCESSING FUNCTION
static LRESULT WProc(HWND hWnd, UINT Mess, WPARAM WPar, LPARAM LPar) {
    switch (Mess) {
        // WINDOW DESTROY MESSAGE
        case (WM_DESTROY):
            PostQuitMessage(0);
            break;

        // COMMAND EVENT
        case (WM_COMMAND):
            InvalidateRect(hWnd, &WSize, FALSE);

            if ((WPar & 0xFFFF) == UPPANEL_NEW) NewPageEvent(hWnd, WSize.bottom);

            for (uint32_t i = 0; i < PagesNum; ++i) {
                if (ImagePages[i].get_close_id() == (WPar & 0xFFFF)) {
                    ClosePage(hWnd, WSize.bottom, i);
                    break;
                } else if (ImagePages[i].get_button_id() == (WPar & 0xFFFF)) PageChosed = i;
            }

            break;

        // WINDOW CREATE EVENT
        case (WM_CREATE):
            ShowInterface(WSize.bottom, hWnd);
            break;

        // WINDOW RESIZE EVENT
        case (WM_SIZE):
            WSize.right = LPar & 0xFFFF, WSize.bottom = ((LPar >> 16) & 0xFFFF);
            UpdateInterface(WSize.bottom);
            break;

        // PAINT EVENT
        case (WM_PAINT):
            Dc = BeginPaint(hWnd, &PStruct);
            MemDC = CreateCompatibleDC(Dc);
            MemBm = CreateCompatibleBitmap(Dc, WSize.right, WSize.bottom);
            SelectObject(MemDC, MemBm);

            DrawBackground(MemDC, WSize.right, WSize.bottom);

            BitBlt(Dc, 0, 0, WSize.right, WSize.bottom, MemDC, 0, 0, SRCCOPY);
            DeleteDC(MemDC), DeleteObject(MemBm);
            EndPaint(hWnd, &PStruct);
            break;

        // OTHER MESSAGES
        default: return DefWindowProcA(hWnd, Mess, WPar, LPar);
    } return 0;
}
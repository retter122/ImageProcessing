#include <iostream>
#include <windows.h>
#include <ctime>

#include "./interface/interface.h"


// FUNCTIONS PROTOTYPE
static LRESULT WProc(HWND hWnd, UINT Mess, WPARAM WPar, LPARAM LPar);


// WINDOW CONSTANT
static const char WName[] = "Image Processing";

#define WSizeScreenAsp 3 / 4
#define WPosScreenAsp 1 / 6

#define FPS_SLEEP 10


// WINDOW DATA
static WNDCLASSA WClass = { CS_HREDRAW | CS_VREDRAW, WProc, 0, 0, 0, 0, 0, (HBRUSH)NULL_BRUSH, 0, "Image Processing" };
static MSG Msg = { 0 };

static HDC Dc = 0;
static PAINTSTRUCT PStruct = { 0 };

static RECT WSize = { 0 };

// MAIN FUNCTION
int main() {
    WSize.right = GetSystemMetrics(SM_CXSCREEN) * WSizeScreenAsp, WSize.bottom = GetSystemMetrics(SM_CYSCREEN) * WSizeScreenAsp;

    // WINDOW INIT
    if (!RegisterClassA(&WClass));
    ShowWindow(CreateWindowExA(0, WClass.lpszClassName, WName, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, WSize.right * WPosScreenAsp, WSize.bottom * WPosScreenAsp, WSize.right, WSize.bottom, 0, 0, 0, 0), SW_SHOWNORMAL);

    // APP MAIN CYCLE
    int32_t last_time = clock();
    while (1) {
        if (PeekMessageA(&Msg, 0, 0, 0, PM_REMOVE)) {
            DispatchMessageA(&Msg);
            TranslateMessage(&Msg);

            if (Msg.message == WM_QUIT) break;
        } else {
            Sleep(std::max(0, FPS_SLEEP - ((int32_t)clock() - last_time)));
            last_time = clock();
        }
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
            break;

        // WINDOW CREATE EVENT
        case (WM_CREATE):
            ShowInterface(WSize.bottom, hWnd);
            break;

        // WINDOW RESIZE EVENT
        case (WM_SIZE):
            WSize.right = LPar & 0xFFFF, WSize.bottom = ((LPar >> 16) & 0xFFFF);
            break;

        // PAINT EVENT
        case (WM_PAINT):
            UpdateInterface(WSize.bottom);

            Dc = BeginPaint(hWnd, &PStruct);
            DrawBackground(Dc, WSize.right, WSize.bottom);

            EndPaint(hWnd, &PStruct);
            break;

        // OTHER MESSAGES
        default: return DefWindowProcA(hWnd, Mess, WPar, LPar);
    } return 0;
}
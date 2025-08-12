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
static WNDCLASSA WClass = { CS_HREDRAW | CS_VREDRAW, WProc, 0, 0, 0, 0, 0, 0, 0, "Image Processing" };
static MSG Msg = { 0 };

static HDC Dc = 0;
static PAINTSTRUCT PStruct = { 0 };

static HBRUSH BgrBrushes[2] = { 0, 0 };
static HPEN BgrPens[1] = { 0 };

static POINT WSize = { 0 };

// MAIN FUNCTION
int main() {
    WSize.x = GetSystemMetrics(SM_CXSCREEN) * WSizeScreenAsp, WSize.y = GetSystemMetrics(SM_CYSCREEN) * WSizeScreenAsp;

    BgrBrushes[0] = CreateSolidBrush(RGB(128, 128, 128)), BgrBrushes[1] = CreateSolidBrush(RGB(80, 80, 80));
    BgrPens[0] = CreatePen(PS_NULL, 0, RGB(0, 0, 0));

    // WINDOW INIT
    if (!RegisterClassA(&WClass));
    ShowWindow(CreateWindowExA(0, WClass.lpszClassName, WName, WS_OVERLAPPEDWINDOW, WSize.x * WPosScreenAsp, WSize.y * WPosScreenAsp, WSize.x, WSize.y, 0, 0, 0, 0), SW_SHOWNORMAL);

    // APP MAIN CYCLE
    int32_t last_time = clock();
    while (1) {
        if (PeekMessageA(&Msg, 0, 0, 0, PM_REMOVE)) {
            DispatchMessageA(&Msg);
            if (Msg.message == WM_QUIT) break;
        } else {
            Sleep(std::max(0, FPS_SLEEP - ((int32_t)clock() - last_time)));
            last_time = clock();
        }
    }

    return 0;
}


// DRAW BACKGROUND FUNCTION
static void DrawBackground() {
    SelectObject(Dc, BgrBrushes[0]), SelectObject(Dc, BgrPens[0]);
    Rectangle(Dc, 0, 0, WSize.x + 1, WSize.y + 1);

    SelectObject(Dc, BgrBrushes[1]);
    Rectangle(Dc, 0, 0, WSize.y * LEFTPANEL_BTN_SIZE, WSize.y);
}


// MESSAGE PROCESSING FUNCTION
static LRESULT WProc(HWND hWnd, UINT Mess, WPARAM WPar, LPARAM LPar) {
    switch (Mess) {
        // WINDOW DESTROY MESSAGE
        case (WM_DESTROY):
            PostQuitMessage(0);
            break;

        // WINDOW CREATE EVENT
        case (WM_CREATE):
            ShowInterface(WSize.y, hWnd);
            break;

        // WINDOW RESIZE EVENT
        case (WM_SIZE):
            WSize.x = LPar & 0xFFFF, WSize.y = ((LPar >> 16) & 0xFFFF);
            UpdateInterface(WSize.y);
            break;

        // PAINT EVENT
        case (WM_PAINT):
            Dc = BeginPaint(hWnd, &PStruct);
            DrawBackground();

            EndPaint(hWnd, &PStruct);
            break;

        // OTHER MESSAGES
        default: return DefWindowProcA(hWnd, Mess, WPar, LPar);
    } return 0;
}
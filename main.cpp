#include <iostream>
#include <windows.h>

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

static HDC Dc = 0;
static PAINTSTRUCT PStruct = { 0 };

static HBRUSH BgrBrushes[1] = { 0 };
static HPEN BgrPens[1] = { 0 };

static POINT WSize = { 0 };

// MAIN FUNCTION
int main() {
    WSize.x = GetSystemMetrics(SM_CXSCREEN) * WSizeScreenAsp, WSize.y = GetSystemMetrics(SM_CYSCREEN) * WSizeScreenAsp;

    BgrBrushes[0] = CreateSolidBrush(RGB(128, 128, 128));
    BgrPens[0] = CreatePen(PS_NULL, 0, RGB(0, 0, 0));

    // WINDOW INIT
    if (!RegisterClassA(&WClass));
    ShowWindow(CreateWindowExA(0, WClass.lpszClassName, WName, WS_OVERLAPPEDWINDOW, WSize.x * WPosScreenAsp, WSize.y * WPosScreenAsp, WSize.x, WSize.y, 0, 0, 0, 0), SW_SHOWNORMAL);

    // APP MAIN CYCLE
    while (1) {
        if (PeekMessageA(&Msg, 0, 0, 0, PM_REMOVE)) {
            DispatchMessageA(&Msg);
            if (Msg.message == WM_QUIT) break;
        }
    }

    return 0;
}


// DRAW BACKGROUND FUNCTION
static void DrawBackground() {
    SelectObject(Dc, BgrBrushes[0]), SelectObject(Dc, BgrPens[0]);
    Rectangle(Dc, 0, 0, WSize.x + 1, WSize.y + 1);

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
            break;

        // PAINT EVENT
        case (WM_PAINT):
            Dc = BeginPaint(hWnd, &PStruct);
            DrawBackground();
            UpdateInterface(WSize.y);

            EndPaint(hWnd, &PStruct);
            break;

        // OTHER MESSAGES
        default: return DefWindowProcA(hWnd, Mess, WPar, LPar);
    } return 0;
}
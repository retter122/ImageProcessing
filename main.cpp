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

#define MOUSEWHELL_SENSIVITY (1.f / 360.f)


// WINDOW DATA
static WNDCLASSA WClass = { CS_HREDRAW | CS_VREDRAW, WProc, 0, 0, 0, 0, 0, 0, 0, "Image Processing" };
static MSG Msg = { 0 };

static HDC Dc = 0, MemDC = 0;
static HBITMAP MemBm = 0;
static PAINTSTRUCT PStruct = { 0 };

static RECT WSize = { 0 };

static uint8_t NowInstrument = LEFTPANEL_CURSOR;

static uint8_t CtrlPressed = 0;


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

        // KEYDOWN COMMAND
        case (WM_KEYDOWN):
            if ((WPar & 0xFF) == VK_CONTROL) CtrlPressed = 1;
            else if ((WPar & 0xFF) == 'Z' && CtrlPressed && PageChosed < PagesNum) ImagePages[PageChosed].undo();
            else if ((WPar & 0xFF) == 'Y' && CtrlPressed && PageChosed < PagesNum) ImagePages[PageChosed].redo();
            InvalidateRect(hWnd, &WSize, FALSE);
            break;

        // COMMAND EVENT
        case (WM_COMMAND):
            InvalidateRect(hWnd, &WSize, FALSE);

            switch((WPar & 0xFFFF)) {
                case(UPPANEL_NEW):
                    NewPageEvent(hWnd, WSize.bottom);
                    break;

                case(LEFTPANEL_CURSOR):
                case(LEFTPANEL_PEN):
                case(LEFTPANEL_RECTANGLE):
                case(LEFTPANEL_FILLED_RECTANGLE):
                case(LEFTPANEL_ELLIPSE):
                case(LEFTPANEL_FILLED_ELLIPSE):
                case(LEFTPANEL_LINE):
                case(LEFTPANEL_PIPETTE):
                    NowInstrument = (WPar & 0xFFFF);
                    break;
            }

            for (uint32_t i = 0; i < PagesNum; ++i) {
                if (ImagePages[i].get_close_id() == (WPar & 0xFFFF)) {
                    ClosePage(hWnd, WSize.bottom, i);
                    break;
                } else if (ImagePages[i].get_button_id() == (WPar & 0xFFFF)) PageChosed = i;
            }

            break;

        // MOUSEDOWN EVENT
        case (WM_LBUTTONDOWN):
            LastMousePX = LPar & 0xFFFF, LastMousePY = (LPar >> 16) & 0xFFFF;
            if ((NowInstrument >= LEFTPANEL_PEN || NowInstrument <= LEFTPANEL_BLUR) && NowInstrument != LEFTPANEL_PIPETTE && PagesNum > PageChosed) ImagePages[PageChosed].new_action();
            
            if (NowInstrument == LEFTPANEL_PIPETTE && PagesNum > PageChosed) PipetteInstrument(LastMousePX, LastMousePY, WSize.right, WSize.bottom);
            else if (NowInstrument == LEFTPANEL_PEN && PagesNum > PageChosed) PenInstrument(LastMousePX, LastMousePY, WSize.right, WSize.bottom);
            
            InvalidateRect(hWnd, &WSize, FALSE);
            break;

        // MOSEMOVE EVENT
        case (WM_MOUSEMOVE):
            InvalidateRect(hWnd, &WSize, FALSE);
            if (WPar == MK_LBUTTON) {
                int16_t NowMX = (LPar & 0xFFFF), NowMY = ((LPar >> 16) & 0xFFFF);

                if (NowInstrument == LEFTPANEL_CURSOR) CursorInstrument(NowMX, NowMY);
                else if (NowInstrument == LEFTPANEL_PEN && PagesNum > PageChosed) PenInstrument(NowMX, NowMY, WSize.right, WSize.bottom);
                else if (NowInstrument == LEFTPANEL_RECTANGLE && PagesNum > PageChosed) RectangleInstrument(NowMX, NowMY, WSize.right, WSize.bottom);
                else if (NowInstrument == LEFTPANEL_FILLED_RECTANGLE && PagesNum > PageChosed) FilledRectangleInstrument(NowMX, NowMY, WSize.right, WSize.bottom);
                else if (NowInstrument == LEFTPANEL_ELLIPSE && PagesNum > PageChosed) EllipseInstrument(NowMX, NowMY, WSize.right, WSize.bottom);
                else if (NowInstrument == LEFTPANEL_FILLED_ELLIPSE && PagesNum > PageChosed) FilledEllipseInstrument(NowMX, NowMY, WSize.right, WSize.bottom);
                else if (NowInstrument == LEFTPANEL_LINE && PagesNum > PageChosed) LineInstrument(NowMX, NowMY, WSize.right, WSize.bottom);
            }
            
            break;

        // MOUSE WHEEL MOVE
        case (WM_MOUSEWHEEL):
            PageImageScale *= pow(1.5, (float)GET_WHEEL_DELTA_WPARAM(WPar) * MOUSEWHELL_SENSIVITY);
            InvalidateRect(hWnd, &WSize, FALSE);
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
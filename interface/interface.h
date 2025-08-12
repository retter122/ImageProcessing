#pragma once

#include <windows.h>
#include <iostream>
#include <stdint.h>

#include "./button.h"
#include "./edit.h"

#include "../image/loader.h"


// BACKGROUND DATA
static HBRUSH BgrBrushes[2] = { 0, 0 };
static HPEN BgrPens[1] = { 0 };

static uint32_t PALETTE_R = 0, PALETTE_G = 0, PALETTE_B = 0;


// INTERFACE PARAMS
#define LEFTPANEL_BTN_SIZE 0.08
#define LEFTPANEL_START 0.05

#define LEFTPANEL_PALETTE_WIDTH 0.16
#define LEFTPANEL_PALETTE_HEIGHT 0.04
#define LEFTPANEL_RGB_MARGIN 0.02
#define LEFTPANEL_RGB_FONTSIZE 0.038
#define LEFTPANEL_RGB_HEIGHT (LEFTPANEL_PALETTE_HEIGHT + LEFTPANEL_BTN_SIZE * 2)
#define LEFTPANEL_RGB_WIDTH 0.04

#define LEFTPANEL_CURSOR 1
#define LEFTPANEL_PEN 2
#define LEFTPANEL_RECTANGLE 3
#define LEFTPANEL_FILLED_RECTANGLE 4
#define LEFTPANEL_ELLIPSE 5
#define LEFTPANEL_FILLED_ELLIPSE 6
#define LEFTPANEL_LINE 7
#define LEFTPANEL_PIPETTE 8
#define LEFTPANEL_BLUR 9


// LEFT PANEL INTERFACE DATA
static Button LeftPanel[] = { 
    Button(0, LEFTPANEL_START, LEFTPANEL_BTN_SIZE, LEFTPANEL_BTN_SIZE, BmpBitmapLoad("./textures/left_panel/cursor.bmp"), LEFTPANEL_CURSOR),
    Button(0, LEFTPANEL_START + LEFTPANEL_BTN_SIZE, LEFTPANEL_BTN_SIZE, LEFTPANEL_BTN_SIZE, BmpBitmapLoad("./textures/left_panel/pen.bmp"), LEFTPANEL_PEN),
    Button(0, LEFTPANEL_START + LEFTPANEL_BTN_SIZE * 2, LEFTPANEL_BTN_SIZE, LEFTPANEL_BTN_SIZE, BmpBitmapLoad("./textures/left_panel/rectangle.bmp"), LEFTPANEL_RECTANGLE),
    Button(0, LEFTPANEL_START + LEFTPANEL_BTN_SIZE * 3, LEFTPANEL_BTN_SIZE, LEFTPANEL_BTN_SIZE, BmpBitmapLoad("./textures/left_panel/filled_rectangle.bmp"), LEFTPANEL_FILLED_RECTANGLE),
    Button(0, LEFTPANEL_START + LEFTPANEL_BTN_SIZE * 4, LEFTPANEL_BTN_SIZE, LEFTPANEL_BTN_SIZE, BmpBitmapLoad("./textures/left_panel/ellipse.bmp"), LEFTPANEL_ELLIPSE),
    Button(0, LEFTPANEL_START + LEFTPANEL_BTN_SIZE * 5, LEFTPANEL_BTN_SIZE, LEFTPANEL_BTN_SIZE, BmpBitmapLoad("./textures/left_panel/filled_ellipse.bmp"), LEFTPANEL_FILLED_ELLIPSE),
    Button(0, LEFTPANEL_START + LEFTPANEL_BTN_SIZE * 6, LEFTPANEL_BTN_SIZE, LEFTPANEL_BTN_SIZE, BmpBitmapLoad("./textures/left_panel/line.bmp"), LEFTPANEL_PEN),
    Button(0, LEFTPANEL_START + LEFTPANEL_BTN_SIZE * 7, LEFTPANEL_BTN_SIZE, LEFTPANEL_BTN_SIZE, BmpBitmapLoad("./textures/left_panel/pipette.bmp"), LEFTPANEL_PIPETTE),
    Button(0, LEFTPANEL_START + LEFTPANEL_BTN_SIZE * 8, LEFTPANEL_BTN_SIZE, LEFTPANEL_BTN_SIZE, BmpBitmapLoad("./textures/left_panel/blur.bmp"), LEFTPANEL_BLUR)
};
#define LeftPanelSize (sizeof(LeftPanel) / sizeof(Button))

static Edit RGBPalete[] = {
    Edit(0, LEFTPANEL_START + LEFTPANEL_BTN_SIZE * 9 + LEFTPANEL_RGB_MARGIN, LEFTPANEL_PALETTE_WIDTH, LEFTPANEL_PALETTE_HEIGHT, LEFTPANEL_RGB_FONTSIZE),
    Edit(0, LEFTPANEL_START + LEFTPANEL_BTN_SIZE * 10 + LEFTPANEL_RGB_MARGIN, LEFTPANEL_PALETTE_WIDTH, LEFTPANEL_PALETTE_HEIGHT, LEFTPANEL_RGB_FONTSIZE),
    Edit(0, LEFTPANEL_START + LEFTPANEL_BTN_SIZE * 11 + LEFTPANEL_RGB_MARGIN, LEFTPANEL_PALETTE_WIDTH, LEFTPANEL_PALETTE_HEIGHT, LEFTPANEL_RGB_FONTSIZE),
};
#define RGBPaleleteSize (sizeof(RGBPalete) / sizeof(Edit))


// SHOW INTERFACE FUNCTION
static void ShowInterface(float Scale, HWND Parent) {
    // BACKGROUND INIT
    BgrBrushes[0] = CreateSolidBrush(RGB(128, 128, 128)), BgrBrushes[1] = CreateSolidBrush(RGB(80, 80, 80));
    BgrPens[0] = CreatePen(PS_NULL, 0, RGB(0, 0, 0));

    // SHOW LEFT PANEL
    for (uint32_t i = 0; i < LeftPanelSize; ++i) LeftPanel[i].Show(Scale, Parent);
    for (uint32_t i = 0; i < RGBPaleleteSize; ++i) {
        RGBPalete[i].Show(Scale, Parent);
        RGBPalete[i].NumberOnly();
    }
}


// UPDATE INTERFACE
static void UpdateInterface(float Scale) {
    // UPDATE LEFT PANEL
    for(uint32_t i = 0; i < LeftPanelSize; ++i) LeftPanel[i].Update(Scale);
    for(uint32_t i = 0; i < RGBPaleleteSize; ++i) RGBPalete[i].Update(Scale);
}


// DRAW BACKGROUND FUNCTION
static void DrawBackground(HDC Dc, uint32_t WSizeX, uint32_t WSizeY) {
    SelectObject(Dc, BgrBrushes[0]), SelectObject(Dc, BgrPens[0]);
    Rectangle(Dc, 0, 0, WSizeX + 1, WSizeY + 1);

    SelectObject(Dc, BgrBrushes[1]);
    Rectangle(Dc, 0, 0, WSizeY * LEFTPANEL_BTN_SIZE, WSizeY);
    Rectangle(Dc, 0, (LEFTPANEL_START + LEFTPANEL_BTN_SIZE * 9) * WSizeY, (LEFTPANEL_PALETTE_WIDTH + LEFTPANEL_RGB_MARGIN * 2 + LEFTPANEL_RGB_WIDTH) * WSizeY, WSizeY);

    PALETTE_R = RGBPalete[0].get_number(), PALETTE_G = RGBPalete[1].get_number(), PALETTE_B = RGBPalete[2].get_number();
    HBRUSH NowRGB = CreateSolidBrush(RGB( PALETTE_R,  PALETTE_G,  PALETTE_B));
    SelectObject(Dc, NowRGB);

    Rectangle(Dc, (LEFTPANEL_PALETTE_WIDTH + LEFTPANEL_RGB_MARGIN) * WSizeY, (LEFTPANEL_START + LEFTPANEL_BTN_SIZE * 9 + LEFTPANEL_RGB_MARGIN) * WSizeY, (LEFTPANEL_PALETTE_WIDTH + LEFTPANEL_RGB_MARGIN + LEFTPANEL_RGB_WIDTH) * WSizeY, (LEFTPANEL_START + LEFTPANEL_BTN_SIZE * 9 + LEFTPANEL_RGB_MARGIN + LEFTPANEL_RGB_HEIGHT) * WSizeY);
    DeleteObject(NowRGB);
}
#pragma once

#include <windows.h>
#include <iostream>
#include <stdint.h>

#include "./button.h"
#include "./edit.h"
#include "./page.h"

#include "../image/loader.h"


// BACKGROUND DATA
static HBRUSH BgrBrushes[2] = { 0, 0 };
static HPEN BgrPens[1] = { 0 };

static uint32_t PALETTE_R = 0, PALETTE_G = 0, PALETTE_B = 0;

static uint32_t PageChosed = 0;
static float PageImageScale = 1;


// LEFTPANEL PARAMS
#define LEFTPANEL_BTN_SIZE 0.08
#define LEFTPANEL_START 0.09

#define LEFTPANEL_PALETTE_WIDTH 0.10
#define LEFTPANEL_PALETTE_HEIGHT 0.035
#define LEFTPANEL_RGB_MARGIN 0.02
#define LEFTPANEL_RGB_FONTSIZE 0.038
#define LEFTPANEL_RGB_HEIGHT (LEFTPANEL_PALETTE_HEIGHT * 3 + LEFTPANEL_RGB_MARGIN * 2)
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


// UPPANEL PARAMS
#define UPPANEL_BTN_WIDTH 0.2
#define UPPANEL_BTN_HEIGHT 0.05
#define UPPANEL_FONTSIZE 0.048

#define UPPANEL_NEW 10
#define UPPANEL_SAVE 11
#define UPPANEL_LOAD 12
#define UPPANEL_PARAMS 13


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
    Edit(0, LEFTPANEL_START + LEFTPANEL_BTN_SIZE * 9 + LEFTPANEL_RGB_MARGIN * 2 + LEFTPANEL_PALETTE_HEIGHT, LEFTPANEL_PALETTE_WIDTH, LEFTPANEL_PALETTE_HEIGHT, LEFTPANEL_RGB_FONTSIZE),
    Edit(0, LEFTPANEL_START + LEFTPANEL_BTN_SIZE * 9 + LEFTPANEL_RGB_MARGIN * 3 + LEFTPANEL_PALETTE_HEIGHT * 2, LEFTPANEL_PALETTE_WIDTH, LEFTPANEL_PALETTE_HEIGHT, LEFTPANEL_RGB_FONTSIZE),
};
#define RGBPaleleteSize (sizeof(RGBPalete) / sizeof(Edit))


// UPPANEL INTERFACE DATA
static Button UpPanel[] = {
    Button(0, 0, UPPANEL_BTN_WIDTH, UPPANEL_BTN_HEIGHT, UPPANEL_FONTSIZE, "New", UPPANEL_NEW),
    Button(UPPANEL_BTN_WIDTH, 0, UPPANEL_BTN_WIDTH, UPPANEL_BTN_HEIGHT, UPPANEL_FONTSIZE, "Save", UPPANEL_SAVE),
    Button(UPPANEL_BTN_WIDTH * 2, 0, UPPANEL_BTN_WIDTH, UPPANEL_BTN_HEIGHT, UPPANEL_FONTSIZE, "Load", UPPANEL_LOAD),
    Button(UPPANEL_BTN_WIDTH * 3, 0, UPPANEL_BTN_WIDTH, UPPANEL_BTN_HEIGHT, UPPANEL_FONTSIZE, "Params", UPPANEL_PARAMS),
};
#define UpPanelSize (sizeof(UpPanel) / sizeof(Button))


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

    // SHOW UP PANEL
    for (uint32_t i = 0; i < UpPanelSize; ++i) UpPanel[i].Show(Scale, Parent);
}


// UPDATE INTERFACE
static void UpdateInterface(float Scale) {
    // UPDATE LEFT PANEL
    for(uint32_t i = 0; i < LeftPanelSize; ++i) LeftPanel[i].Update(Scale);
    for(uint32_t i = 0; i < RGBPaleleteSize; ++i) RGBPalete[i].Update(Scale);

    // UPDATE UP PANEL
    for (uint32_t i = 0; i < UpPanelSize; ++i) UpPanel[i].Update(Scale);

    // UPDATE PAGES
    UpdatePages(Scale);
}


// DRAW PALETE COLOR
static void DrawPaleteColor(HDC Dc, uint32_t WSizeX, uint32_t WSizeY) {
    HPEN RGBPen = CreatePen(PS_SOLID, 0.01 * WSizeY, 0);

    PALETTE_R = RGBPalete[0].get_number(), PALETTE_G = RGBPalete[1].get_number(), PALETTE_B = RGBPalete[2].get_number();
    HBRUSH NowRGB = CreateSolidBrush(RGB( PALETTE_R,  PALETTE_G,  PALETTE_B));

    SelectObject(Dc, NowRGB), SelectObject(Dc, RGBPen);

    Rectangle(Dc, (LEFTPANEL_PALETTE_WIDTH + LEFTPANEL_RGB_MARGIN) * WSizeY, (LEFTPANEL_START + LEFTPANEL_BTN_SIZE * 9 + LEFTPANEL_RGB_MARGIN) * WSizeY, (LEFTPANEL_PALETTE_WIDTH + LEFTPANEL_RGB_MARGIN + LEFTPANEL_RGB_WIDTH) * WSizeY + 1, (LEFTPANEL_START + LEFTPANEL_BTN_SIZE * 9 + LEFTPANEL_RGB_MARGIN + LEFTPANEL_RGB_HEIGHT) * WSizeY + 1);
    DeleteObject(NowRGB), DeleteObject(RGBPen);
}


// DRAW PAGE FUNCTION
static void DrawPageImage(HDC Dc, uint32_t WSizeX, uint32_t WSizeY, float Scale, const Image& Img) {
    uint32_t ImgSX = (float)Img.get_width() * Scale, ImgSY = (float)Img.get_height() * Scale;

    BitmapImage DrawImg = ImageToBitmap(Img);
    DrawImg.resize(ImgSX, ImgSY);

    HDC MDc = CreateCompatibleDC(Dc);
    DrawImg.update_bitmap(0);
    SelectObject(MDc, DrawImg.get_bitmap());

    BitBlt(Dc, (WSizeX - ImgSX) / 2, (WSizeY - ImgSY) / 2, (WSizeX - ImgSX) / 2 + ImgSX, (WSizeY - ImgSY) / 2 + ImgSY, MDc, 0, 0, SRCCOPY);
    DeleteDC(MDc);
}


// DRAW BACKGROUND FUNCTION
static void DrawBackground(HDC Dc, uint32_t WSizeX, uint32_t WSizeY) {
    SelectObject(Dc, BgrBrushes[0]), SelectObject(Dc, BgrPens[0]);
    Rectangle(Dc, 0, 0, WSizeX + 1, WSizeY + 1);

    SelectObject(Dc, BgrBrushes[1]);
    Rectangle(Dc, 0, 0, LEFTPANEL_BTN_SIZE * WSizeY + 1, WSizeY + 1);
    Rectangle(Dc, 0, (LEFTPANEL_START + LEFTPANEL_BTN_SIZE * 9) * WSizeY, (LEFTPANEL_PALETTE_WIDTH + LEFTPANEL_RGB_MARGIN * 2 + LEFTPANEL_RGB_WIDTH) * WSizeY, WSizeY + 1);
    Rectangle(Dc, 0, 0, WSizeX + 1, WSizeY * (LEFTPANEL_START) + 1);

    DrawPaleteColor(Dc, WSizeX, WSizeY);

    if (PagesNum > PageChosed) DrawPageImage(Dc, WSizeX, WSizeY, PageImageScale, ImagePages[PageChosed].PgImage);
}


// RESOURCES DELETE
static void DeleteInterface() {
    DeleteObject(BgrBrushes[0]), DeleteObject(BgrBrushes[1]);
    DeleteObject(BgrPens[0]);

    PagesDelete();
}


// NEW BUTTON FUNCTION
static void NewPageEvent(HWND Handle, float Scale) {
    AddNewPage(Handle, Scale, 100, 100);
}
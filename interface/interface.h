#pragma once

#include <windows.h>
#include <iostream>
#include <stdint.h>

#include "./button.h"

#include "../image/loader.h"


// INTERFACE PARAMS
#define LEFTPANEL_BTN_SIZE 0.08
#define LEFTPANEL_START 0.05

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


// SHOW INTERFACE
static void ShowInterface(float Scale, HWND Parent) {
    // SHOW LEFT PANEL
    for (uint32_t i = 0; i < LeftPanelSize; ++i) LeftPanel[i].Show(Scale, Parent);
}


// UPDATE INTERFACE
static void UpdateInterface(float Scale) {
    // UPDATE LEFT PANEL
    for(uint32_t i = 0; i < LeftPanelSize; ++i) LeftPanel[i].Update(Scale);
}
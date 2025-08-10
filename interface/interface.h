#pragma once

#include <windows.h>
#include <iostream>
#include <stdint.h>

#include "./button.h"


// LEFT PANEL INTERFACE DATA
static Button LeftPanel[1] = { Button(0, 0, 0.3, 0.3, 0.1, "Test", 1) };
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
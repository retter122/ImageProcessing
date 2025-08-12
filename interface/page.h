#pragma once

#include <iostream>
#include <windows.h>
#include <stdint.h>

#include "../image/image.h"

#include "./button.h"


// PAGES PARAMS
#define PAGES_BTN_START_ID 100


// PAGE STRUCT
struct Page {
    Button PgButton;
    Image PgImage;

    Page(uint32_t width, uint32_t height, uint32_t btn_id) {}
};


// PAGES ARRAY
static Page *ImagePages = 0;
static uint32_t PagesNum = 0;

static uint32_t NowPageBtnId = PAGES_BTN_START_ID;


// ADD NEW PAGE FUNCTION
static void AddNewPage(uint32_t width, uint32_t height) {
    Page* NewPages = new Page[PagesNum + 1];
    for (uint32_t i = 0; i < PagesNum; ++i) NewPages[i] = ImagePages[i];
    NewPages[PagesNum++] = Page(width, height, NowPageBtnId);

    if (ImagePages) delete[] ImagePages;
    ImagePages = NewPages;
}
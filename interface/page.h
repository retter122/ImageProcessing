#pragma once

#include <iostream>
#include <windows.h>
#include <stdint.h>

#include "../image/image.h"

#include "./button.h"


// PAGES PARAMS
#define PAGES_BTN_START_ID 100

#define PAGES_BTN_WIDTH 0.2
#define PAGES_CLOSE_WIDTH 0.04
#define PAGES_HEIGHT 0.04
#define PAGES_FONTSIZE 0.038

#define PAGES_START 0.05


// PAGE CLASS
struct Page {
    private:
        Button PgButton, PgClose;

    public:
        Image PgImage;

        Page(): PgButton(), PgClose(), PgImage() {}
        Page(uint32_t width, uint32_t height, const std::string& Name, uint32_t id) : PgImage(width, height, 0), PgButton((PAGES_BTN_WIDTH + PAGES_CLOSE_WIDTH) * id, PAGES_START, PAGES_BTN_WIDTH, PAGES_HEIGHT, PAGES_FONTSIZE, Name, id * 2 + PAGES_BTN_START_ID), PgClose((PAGES_BTN_WIDTH + PAGES_CLOSE_WIDTH) * id + PAGES_BTN_WIDTH, PAGES_START, PAGES_CLOSE_WIDTH, PAGES_HEIGHT, PAGES_FONTSIZE, "x", id * 2 + 1 + PAGES_BTN_START_ID) {}
        Page(const Page& _obj) : PgButton(_obj.PgButton), PgImage(_obj.PgImage) {}

        std::string get_name() { return this->PgButton.Text; }
        uint32_t get_button_id() { return this->PgButton.get_id(); }
        uint32_t get_close_id() { return this->PgClose.get_id(); }

        void set_name(const std::string& _name) { this->PgButton.Text = _name; }

        void Show(float Scale, HWND Parent) { this->PgButton.Show(Scale, Parent), this->PgClose.Show(Scale, Parent); }
        void Update(float Scale) { this->PgButton.Update(Scale), this->PgClose.Update(Scale); }

        void Destroy() { this->PgButton.Destroy(), this->PgClose.Destroy(); }
        ~Page() { this->Destroy(); }
};


// PAGES ARRAY DATA
static uint32_t PagesNum = 0;
static Page *ImagePages = 0;


// UPDATE PAGES FUNCTION
static void UpdatePages(float Scale) {
    for (uint32_t i = 0; i < PagesNum; ++i) ImagePages[i].Update(Scale);
}


// ADD NEW PAGE FUNCTION
static void AddNewPage(HWND Parent, float Scale, uint32_t width, uint32_t height) {
    Page* NewPages = new Page[PagesNum + 1];
    for (uint32_t i = 0; i < PagesNum; ++i) {
        NewPages[i] = ImagePages[i];
        NewPages[i].Show(Scale, Parent);
    } NewPages[PagesNum] = Page(width, height, "New image", PagesNum);

    if (ImagePages) delete[] ImagePages;
    ImagePages = NewPages;
    ImagePages[PagesNum].PgImage.clear();
    ImagePages[PagesNum].Show(Scale, Parent);
    
    ++PagesNum;
}


// CLOSE PAGE FUNCTION
static void ClosePage(HWND Parent, float Scale, uint32_t PageIdx) {
    if (PagesNum == 1) {
        delete[] ImagePages;
        ImagePages = 0;
    } else {
        Page *NewPages = new Page[PagesNum - 1];

        for (uint32_t i = 0; i < PageIdx; ++i) {
            NewPages[i] = ImagePages[i];
            NewPages[i].Show(Scale, Parent);
        } for (uint32_t i = PageIdx + 1; i < PagesNum; ++i) {
            NewPages[i - 1] = Page(ImagePages[i].PgImage.get_width(), ImagePages[i].PgImage.get_height(), ImagePages[i].get_name(), i - 1);
            NewPages[i - 1].Show(Scale, Parent);
        }

        delete[] ImagePages;
        ImagePages = NewPages;
    } --PagesNum;
}


// PAGES DELETE
static void PagesDelete() {
    PagesNum = 0;
    delete[] ImagePages;
}
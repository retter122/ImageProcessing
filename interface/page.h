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

        Image* PgImage;
        uint32_t ImagesNum, ActualImage;

        void copy_images(Image* _imgs) {
            if (this->PgImage) delete[] this->PgImage;
            this->PgImage = new Image[this->ImagesNum];

            for (uint32_t i = 0; i < this->ImagesNum; ++i) this->PgImage[i] = _imgs[i];
        }

    public:

        Page(): PgButton(), ImagesNum(0), ActualImage(0), PgClose(), PgImage(0) {}
        Page(uint32_t width, uint32_t height, const std::string& Name, uint32_t id) : PgImage(new Image[1]), ImagesNum(1), ActualImage(0), PgButton((PAGES_BTN_WIDTH + PAGES_CLOSE_WIDTH) * id, PAGES_START, PAGES_BTN_WIDTH, PAGES_HEIGHT, PAGES_FONTSIZE, Name, id * 2 + PAGES_BTN_START_ID), PgClose((PAGES_BTN_WIDTH + PAGES_CLOSE_WIDTH) * id + PAGES_BTN_WIDTH, PAGES_START, PAGES_CLOSE_WIDTH, PAGES_HEIGHT, PAGES_FONTSIZE, "x", id * 2 + 1 + PAGES_BTN_START_ID) { this->PgImage[0] = Image(width, height, 0); }
        Page(const Page& _obj) : PgButton(_obj.PgButton), ImagesNum(_obj.ImagesNum), ActualImage(_obj.ActualImage), PgImage(0) { this->copy_images(_obj.PgImage); }

        std::string get_name() const { return this->PgButton.Text; }
        uint32_t get_button_id() const { return this->PgButton.get_id(); }
        uint32_t get_close_id() const { return this->PgClose.get_id(); }
        Image& get_actual_img() const {
            Image nul;
            return (this->ActualImage < this->ImagesNum) ? this->PgImage[ActualImage] : nul;
        }

        void set_name(const std::string& _name) { this->PgButton.Text = _name; }
        void new_action() {
            Image* NewImage = new Image[this->ImagesNum + 1];
            for (uint32_t i = 0; i < this->ImagesNum; ++i) NewImage[i] = this->PgImage[i];

            if (this->PgImage) {
                NewImage[this->ImagesNum] = this->PgImage[this->ImagesNum - 1];
                delete[] this->PgImage;
            } this->PgImage = NewImage;
            ++this->ImagesNum, ++this->ActualImage;
        }
        void set_id(uint32_t id) {
            //std::cout << this->PgButton.Text << std::endl;
            this->PgButton = Button((PAGES_BTN_WIDTH + PAGES_CLOSE_WIDTH) * id, PAGES_START, PAGES_BTN_WIDTH, PAGES_HEIGHT, PAGES_FONTSIZE, this->PgButton.Text, id * 2 + PAGES_BTN_START_ID);
            this->PgClose = Button((PAGES_BTN_WIDTH + PAGES_CLOSE_WIDTH) * id + PAGES_BTN_WIDTH, PAGES_START, PAGES_CLOSE_WIDTH, PAGES_HEIGHT, PAGES_FONTSIZE, "x", id * 2 + 1 + PAGES_BTN_START_ID);
        }

        void Show(float Scale, HWND Parent) { this->PgButton.Show(Scale, Parent), this->PgClose.Show(Scale, Parent); }
        void Update(float Scale) { this->PgButton.Update(Scale), this->PgClose.Update(Scale); }

        void operator=(const Page& _obj) {
            this->PgButton = _obj.PgButton, this->PgClose = _obj.PgClose;
            this->ImagesNum = _obj.ImagesNum, this->ActualImage = _obj.ActualImage;
            this->copy_images(_obj.PgImage);
        }

        void Destroy() { this->PgButton.Destroy(), this->PgClose.Destroy(); }
        ~Page() { 
            if (this->PgImage) delete[] this->PgImage;
            this->Destroy();
        }
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
    ImagePages[PagesNum].get_actual_img().clear();
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
            NewPages[i - 1] = ImagePages[i];
            NewPages[i - 1].set_id(i - 1);
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
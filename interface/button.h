#pragma once

#include <iostream>
#include <windows.h>

#include "../image/bitmap.h"


// BUTTONS DATA
static uint32_t next_unique_id = 1;


// BUTTON CONTAINER
class Button {
    private:
        HWND BtnHandle;
        HFONT FontHandle;

        BitmapImage TempImage;
    public:
        float XPos, YPos, XSize, YSize, FontSize;
        std::string Text;
        BitmapImage ImageBtn;

        uint32_t unique_id;

        Button() : FontSize(0), XPos(0), YPos(0), XSize(0), YSize(0), BtnHandle(0), Text(""), ImageBtn(), unique_id(0), FontHandle(0) {}
        Button(float _XP, float _YP, float _XS, float _YS, float _FS, const std::string& _Txt, uint32_t id) : FontSize(_FS), XPos(_XP), YPos(_YP), XSize(_XS), YSize(_YS), BtnHandle(0), Text(_Txt), ImageBtn(), unique_id(id), FontHandle(0) {}
        Button(float _XP, float _YP, float _XS, float _YS, const BitmapImage& _Img, uint32_t id) : FontSize(0), XPos(_XP), YPos(_YP), XSize(_XS), YSize(_YS), BtnHandle(0), Text(""), ImageBtn(_Img), unique_id(id), FontHandle(0) {  }
        Button(const Button& _obj) : FontSize(0), XPos(_obj.XPos), YPos(_obj.YPos), XSize(_obj.XSize), YSize(_obj.YSize), BtnHandle(0), Text(""), ImageBtn(), unique_id(_obj.unique_id), FontHandle(0) {  }

        void Show(float Scale, HWND Parent) {
            this->BtnHandle = CreateWindowExA(0, "button", this->Text.c_str(), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | (this->ImageBtn.get_bytes() ? BS_BITMAP : 0) | BS_FLAT, 0, 0, 0, 0, Parent, 0, (HINSTANCE)this->unique_id, 0);
            this->Update(Scale);
        }

        void Update(float Scale) {
            uint32_t PixWidth = this->XSize * Scale, PixHeight = this->YSize * Scale;

            SetWindowLongPtrA(this->BtnHandle, GWL_STYLE, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | (this->ImageBtn.get_bytes() ? BS_BITMAP : 0) & ~(WS_BORDER) | BS_FLAT);
            SetWindowPos(this->BtnHandle, 0, this->XPos * Scale, this->YPos * Scale, PixWidth, PixHeight, 0);

            if (this->ImageBtn.get_bytes()) {

                //SendMessageA(this->BtnHandle, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)this->TempImage);
            } else {
                DeleteObject(this->FontHandle);
                this->FontHandle = CreateFontA(this->FontSize * Scale, 0, 0, 0, 400, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, 0);
                SendMessage(this->BtnHandle, WM_SETFONT, (WPARAM)this->FontHandle, TRUE);
            }
        }

        void operator=(const Button& _obj) {
            this->XPos = _obj.XPos, this->YPos = _obj.YPos, this->XSize = _obj.XSize, this->YSize = _obj.YSize;
            this->BtnHandle = _obj.BtnHandle, this->unique_id = _obj.unique_id, this->FontSize = _obj.FontSize, this->FontHandle = 0;
            this->Text = _obj.Text, this->ImageBtn = _obj.ImageBtn;
        }

        ~Button() { DeleteObject(this->FontHandle); }
};
#pragma once

#include <iostream>
#include <windows.h>

#include "../image/image.h"


// BUTTONS DATA
static uint32_t next_unique_id = 1;


// BUTTON CONTAINER
class Button {
    private:
        HWND BtnHandle;
        HFONT FontHandle;

        BitmapImage TempImage;
        uint64_t unique_id;
    public:
        float XPos, YPos, XSize, YSize, FontSize;
        std::string Text;
        BitmapImage ImageBtn;

        Button() : FontSize(0), XPos(0), YPos(0), XSize(0), YSize(0), BtnHandle(0), Text(""), ImageBtn(), unique_id(0), FontHandle(0) {}
        Button(float _XP, float _YP, float _XS, float _YS, float _FS, const std::string& _Txt, uint64_t id) : FontSize(_FS), XPos(_XP), YPos(_YP), XSize(_XS), YSize(_YS), BtnHandle(0), Text(_Txt), ImageBtn(), unique_id(id), FontHandle(0) {}
        Button(float _XP, float _YP, float _XS, float _YS, const BitmapImage& _Img, uint64_t id) : FontSize(0), XPos(_XP), YPos(_YP), XSize(_XS), YSize(_YS), BtnHandle(0), Text(""), ImageBtn(_Img), unique_id(id), FontHandle(0) {  }
        Button(const Button& _obj) : FontSize(0), XPos(_obj.XPos), YPos(_obj.YPos), XSize(_obj.XSize), YSize(_obj.YSize), BtnHandle(0), Text(""), ImageBtn(), unique_id(_obj.unique_id), FontHandle(0) {  }

        uint32_t get_id() const { return this->unique_id; }

        void Show(float Scale, HWND Parent) {
            this->BtnHandle = CreateWindowExA(0, "button", this->Text.c_str(), WS_VISIBLE | BS_DEFPUSHBUTTON | WS_CHILD | (this->ImageBtn.get_bytes() ? BS_BITMAP : 0), 0, 0, 0, 0, Parent, (HMENU)this->unique_id, (HINSTANCE)GetWindowLongPtr(Parent, GWLP_HINSTANCE), 0);
            this->Update(Scale);
        }

        void Update(float Scale) {
            uint32_t PixWidth = this->XSize * Scale, PixHeight = this->YSize * Scale;

            SetWindowLongPtrA(this->BtnHandle, GWL_STYLE, WS_VISIBLE | BS_DEFPUSHBUTTON | WS_CHILD | (this->ImageBtn.get_bytes() ? BS_BITMAP : 0));
            SetWindowPos(this->BtnHandle, 0, this->XPos * Scale, this->YPos * Scale, PixWidth, PixHeight, 0);

            if (this->ImageBtn.get_bytes()) {
                this->TempImage = this->ImageBtn;
                this->TempImage.resize(PixWidth, PixHeight);
                this->TempImage.update_bitmap(0);

                SendMessageA(this->BtnHandle, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)this->TempImage.get_bitmap());
            } else {
                DeleteObject(this->FontHandle);
                this->FontHandle = CreateFontA(this->FontSize * Scale, 0, 0, 0, 400, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, 0);
                SendMessage(this->BtnHandle, WM_SETFONT, (WPARAM)this->FontHandle, TRUE);
            }
        }

        void operator=(const Button& _obj) {
            this->XPos = _obj.XPos, this->YPos = _obj.YPos, this->XSize = _obj.XSize, this->YSize = _obj.YSize;
            this->BtnHandle = 0, this->unique_id = _obj.unique_id, this->FontSize = _obj.FontSize, this->FontHandle = 0;
            this->Text = _obj.Text, this->ImageBtn = _obj.ImageBtn;
        }

        void Destroy() { DestroyWindow(this->BtnHandle); }
        ~Button() { DeleteObject(this->FontHandle), this->Destroy(); }
};
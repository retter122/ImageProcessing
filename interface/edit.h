#pragma once

#include <iostream>
#include <windows.h>
#include <stdint.h>
#include <string>


// EDIT CLASS
class Edit {
    private:
        HWND Handle;
        HFONT HFont;

    public:
        float XPos, YPos, XSize, YSize;
        float FontSize;

        Edit(): XPos(0), YPos(0), XSize(0), YSize(0), Handle(0), FontSize(0), HFont(0) {}
        Edit(const Edit& _obj): XPos(_obj.XPos), YPos(_obj.YPos), XSize(_obj.XSize), YSize(_obj.YSize), Handle(_obj.Handle), FontSize(_obj.FontSize), HFont(0) {}
        Edit(float _XP, float _YP, float _XS, float _YS, float _FS): XPos(_XP), YPos(_YP), XSize(_XS), YSize(_YS), Handle(0), FontSize(_FS), HFont(0) {}

        int32_t get_number() const { 
            std::string str = this->get_text();
            if (str.size() == 0) return 0;

            return std::stoi(str);
        }
        std::string get_text() const {
            if (this->Handle == 0) return "";

            uint32_t TxtLen = GetWindowTextLengthA(this->Handle);
            if (TxtLen == 0) return "";

            char* text = new char[TxtLen + 2];

            GetWindowTextA(this->Handle, text, TxtLen + 1);    
            text[TxtLen + 1] = 0;     

            std::string str(text);
            return str;
        }

        void set_text(const std::string _text) { SetWindowTextA(this->Handle, _text.c_str()); }

        void Show(float Scale, HWND Parent) {
            this->Handle = CreateWindowExA(0, "EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 0, 0, 0, 0, Parent, 0, 0, 0);
            this->Update(Scale);
        }

        void NumberOnly() { SetWindowLongPtrA(this->Handle, GWL_STYLE, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER); }

        void Update(float Scale) {
            DeleteObject(this->HFont);
            this->HFont = CreateFontA(this->FontSize * Scale, 0, 0, 0, 400, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, 0);

            SetWindowPos(this->Handle, 0, this->XPos * Scale, this->YPos * Scale, this->XSize * Scale, this->YSize * Scale, 0);
            SendMessage(this->Handle, WM_SETFONT, (WPARAM)HFont, true);
        }

        void Destroy() { DestroyWindow(this->Handle); }
        ~Edit() { DeleteObject(this->HFont), this->Destroy(); }
};
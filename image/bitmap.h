#pragma once

#include <iostream>
#include <stdint.h>
#include <windows.h>


// BITMAP IMAGE CLASS
class BitmapImage {
    private:
        uint32_t *bytes;
        BITMAPINFO BMI;
        HBITMAP HBm;

    public:
        BitmapImage() : bytes{0}, BMI{ {sizeof(BITMAPINFOHEADER), 0, 0, 1, 32, BI_RGB, 0, 0, 0, 0, 0}, { 0, 0, 0, 0 } }, HBm(0) {}
        BitmapImage(LONG _wid, LONG _hei, uint32_t* _byt) : BMI{ {sizeof(BITMAPINFOHEADER), _wid, _hei, 1, 32, BI_RGB, 0, 0, 0, 0, 0}, { 0, 0, 0, 0 } }, bytes(0), HBm(0) { this->copy_bytes(_byt); }
        BitmapImage(const BitmapImage& _obj) : BMI(_obj.BMI), bytes(0), HBm(0) { this->copy_bytes(_obj.bytes); }

        uint32_t get_width() { return this->BMI.bmiHeader.biWidth; }
        uint32_t get_height() { return this->BMI.bmiHeader.biHeight; }
        uint32_t *get_bytes() { return this->bytes; }
        HBITMAP get_bitmap() { return this->HBm; }

        void update_bitmap(HDC Dc) { 
            if (!this->bytes) return;
            DeleteObject(this->HBm);

            void* temp = (this->bytes);
            this->HBm = CreateDIBSection(Dc, &this->BMI, DIB_RGB_COLORS, &temp, 0, 0);
            
            memcpy(temp, this->bytes, this->BMI.bmiHeader.biHeight * this->BMI.bmiHeader.biWidth * sizeof(uint32_t));
        }

        void resize(uint32_t new_width, uint32_t new_heigth) {
            if (this->bytes) {
                uint32_t new_bytes_num = new_width * new_heigth;
                uint32_t *new_bytes = new uint32_t[new_bytes_num];

                float ypos = 0, yasp = (float)this->BMI.bmiHeader.biHeight / (float)new_heigth, xasp = (float)this->BMI.bmiHeader.biWidth / (float)new_width;
                for (uint32_t i = 0; i < new_heigth; ++i, ypos += yasp) {
                    float xpos = 0;
                    for (uint32_t j = 0; j < new_width; ++j, xpos += xasp) new_bytes[i * new_width + j] = this->bytes[(uint32_t)ypos * this->BMI.bmiHeader.biWidth + (uint32_t)xpos];
                }

                delete[] this->bytes;
                this->bytes = new_bytes;
            } this->BMI.bmiHeader.biWidth = new_width, this->BMI.bmiHeader.biHeight = new_heigth;
        }

        void copy_bytes(uint32_t* _bytes) {
            if (!_bytes) return;

            uint32_t bytes_num = this->BMI.bmiHeader.biWidth * this->BMI.bmiHeader.biHeight;
            if (this->bytes) delete[] this->bytes;
            this->bytes = new uint32_t[bytes_num];

            memcpy(this->bytes, _bytes, bytes_num * sizeof(uint32_t));
        }

        void operator=(const BitmapImage& _obj) {
            if (this->bytes) delete[] this->bytes;
            DeleteObject(this->HBm);

            this->BMI = _obj.BMI;
            this->HBm = 0, this->bytes = 0;
            this->copy_bytes(_obj.bytes);
        }

        ~BitmapImage() { 
            if (this->bytes) delete[] this->bytes;
            DeleteObject(this->HBm);
        }
};
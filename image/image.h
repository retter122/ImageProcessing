#pragma once

#include <iostream>
#include <stdint.h>

#include "./mask.h"


// IMAGE MACROS
#define CopyImagePixel(Dst, Src) (Dst[0] = Src[0], Dst[1] = Src[1], Dst[2] = Src[2], Dst[3] = Src[3])

#define GetU32R(Color) ((Color >> 16) & 0xFF)
#define GetU32G(Color) ((Color >> 8) & 0xFF)
#define GetU32B(Color) (Color & 0xFF)

#define ToU32RGB(R, G, B) (((uint32_t)R << 16) | ((uint32_t)G << 8) | (uint32_t)B)
#define ToF32RGB(U32, F32) (F32[0] = GetU32R(U32), F32[1] = GetU32G(U32), F32[2] = GetU32B(U32))


// IMAGE CLASS
class Image {
    private:
        float (*bytes)[4] = 0;
        uint32_t width = 0, height = 0;
    
    public:
        Image() : width(0), height(0), bytes(0) {}
        Image(const Image& _obj) : width(_obj.width), height(_obj.height), bytes(0) { this->copy_bytes(_obj.bytes); }
        Image(uint32_t _wid, uint32_t _hei, float (*_byt)[4]) : width(_wid), height(_hei), bytes(0) { this->copy_bytes(_byt); }

        uint32_t get_width() const { return this->width; }
        uint32_t get_height() const { return this->height; }
        float (*get_bytes() const)[4] { return this->bytes; }

        void clear() {
            uint32_t bytes_num = this->width * this->height;
            if (!this->bytes) this->bytes = new float[bytes_num][4];

            for (uint32_t i = 0; i < bytes_num; ++i) ToF32RGB(0, this->bytes[i]), this->bytes[i][3] = 0;
        }

        void copy_bytes(float (*_bytes)[4]) {
            if (!_bytes) return;

            uint32_t bytes_num = this->width * this->height;
            if (this->bytes) delete[] this->bytes;
            this->bytes = new float[bytes_num][4];

            memcpy(this->bytes, _bytes, bytes_num * sizeof(float[4]));
        }

        void resize(uint32_t new_width, uint32_t new_height) {
            if (this->bytes) {
                uint32_t num_bytes = new_width * new_height;
                float (*new_bytes)[4] = new float[num_bytes][4];

                float ypos = 0, yasp = (float)this->height / (float)new_height, xasp = (float)this->width / new_width;
                for (uint32_t i = 0; i < new_height; ++i, ypos += yasp) {
                    float xpos = 0;
                    for (uint32_t j = 0; j < new_width; ++j, xpos += xasp) CopyImagePixel(new_bytes[i * new_width + j], this->bytes[(uint32_t)ypos * this->width + (uint32_t)xpos]);
                }

                delete[] this->bytes;
                this->bytes = new_bytes;
            } this->width = new_width, this->height = new_height;
        }

        void mask(const Mask& _mask) {
            if (!this->bytes || !_mask.get_bytes() || this->width != _mask.get_width() || this->height != _mask.get_height()) return;
            
            uint32_t bytes_num = this->width * this->height;
            float *mask_bytes = _mask.get_bytes();
            for (uint32_t i = 0; i < bytes_num; ++i) this->bytes[i][0] *= mask_bytes[i], this->bytes[i][1] *= mask_bytes[i], this->bytes[i][2] *= mask_bytes[i], this->bytes[i][3] *= mask_bytes[i];
        }

        void draw_elipse(int32_t PX, int32_t PY, int32_t SX, int32_t SY, float R, float G, float B) {
            PY = this->height - PY;

            for (int32_t i = std::max(PY - SX, 0); i < std::min(PY + SX, (int32_t)this->height); ++i) {
                int32_t rd = sqrt(SX * SX - (PY - i) * (PY - i));
                for (int32_t j = std::max(PX - rd, 0); j < std::min(PX + rd, (int32_t)this->width); ++j) this->bytes[i * this->width + j][0] = R, this->bytes[i * this->width + j][1] = G, this->bytes[i * this->width + j][2] = B;
            }
        }

        void draw_circle(int32_t PX, int32_t PY, int32_t SX, int32_t SY, int32_t Wid, float R, float G, float B) {
            PY = this->height - PY;

            for (int32_t i = std::max(PY - SX, 0); i < std::min(PY + SX, (int32_t)this->height); ++i) {
                int32_t rd1 = sqrt(SX * SX - (PY - i) * (PY - i)), rd2 = (SX - Wid) > abs(PY - i) ? (sqrt((SX - Wid) * (SX - Wid) - (PY - i) * (PY - i))) : 0;

                for (int32_t j = std::max(PX - rd1, 0); j < std::min(PX - rd2, (int32_t)this->width); ++j) this->bytes[i * this->width + j][0] = R, this->bytes[i * this->width + j][1] = G, this->bytes[i * this->width + j][2] = B;
                for (int32_t j = std::max(PX + rd2, 0); j < std::min(PX + rd1, (int32_t)this->width); ++j) this->bytes[i * this->width + j][0] = R, this->bytes[i * this->width + j][1] = G, this->bytes[i * this->width + j][2] = B;
            }
        }

        void draw_rect(int32_t X1, int32_t Y1, int32_t X2, int32_t Y2, uint32_t Width, float R, float G, float B) {
            for (uint32_t i = std::min(X1, X2); i < std::max(X2, X1); ++i) {
                this->draw_elipse(i, Y1, Width, Width, R, G, B);
                this->draw_elipse(i, Y2, Width, Width, R, G, B);
            } for (uint32_t i = std::min(Y1, Y2); i < std::max(Y2, Y1); ++i) {
                this->draw_elipse(X1, i, Width, Width, R, G, B);
                this->draw_elipse(X2, i, Width, Width, R, G, B);
            }
        }

        void draw_filled_rect(int32_t X1, int32_t Y1, int32_t X2, int32_t Y2, float R, float G, float B) {
            Y1 = this->height - Y1, Y2 = this->height - Y2;

            for (int32_t i = std::max(std::min(Y1, Y2), 0); i < std::min(std::max(Y2, Y1), (int32_t)this->height); ++i) {
                for (int32_t j = std::max(std::min(X1, X2), 0); j < std::min(std::max(X2, X1), (int32_t)this->width); ++j) this->bytes[j + i * this->width][0] = R, this->bytes[j + i * this->width][1] = G, this->bytes[j + i * this->width][2] = B; 
            }
        }

        void operator=(const Image& _obj) {
            this->width = _obj.width, this->height = _obj.height;
            this->copy_bytes(_obj.bytes);
        }

        ~Image() { if (this->bytes) delete[] this->bytes; }
};


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

        uint32_t get_width() const { return this->BMI.bmiHeader.biWidth; }
        uint32_t get_height() const { return this->BMI.bmiHeader.biHeight; }
        uint32_t *get_bytes() const { return this->bytes; }
        HBITMAP get_bitmap() const { return this->HBm; }
        BITMAPINFO get_bmi() const { return this->BMI; }

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


// IMAGE CONVERT FUNCTIONS
static BitmapImage ImageToBitmap(const Image& _Img) {
    float (*img_bytes)[4] = _Img.get_bytes();
    if (!img_bytes) return BitmapImage();

    uint32_t bytes_num = _Img.get_width() * _Img.get_height();
    uint32_t* bitmap_bytes = new uint32_t[bytes_num];
    for (uint32_t i = 0; i < bytes_num; ++i) bitmap_bytes[i] = ToU32RGB(img_bytes[i][0] * 255, img_bytes[i][1] * 255, img_bytes[i][2] * 255);

    BitmapImage out(_Img.get_width(), _Img.get_height(), bitmap_bytes);
    delete[] bitmap_bytes;
    return out;
}
#pragma once

#include <iostream>
#include <stdint.h>


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

        void operator=(const Image& _obj) {
            this->width = _obj.width, this->height = _obj.height;
            this->copy_bytes(_obj.bytes);
        }

        ~Image() { if (this->bytes) delete[] this->bytes; }
};


// CONVERT BUFFER FORMAT FUNCTIONS
static uint32_t *PixelsToUint32(const Image& _obj) {
    uint32_t NumBytes = _obj.get_width() * _obj.get_height();
    uint32_t* bytes = new uint32_t[NumBytes];
    float (*bytes_float)[4] = _obj.get_bytes();

    for (uint32_t i = 0; i < NumBytes; ++i) bytes[i] = ToU32RGB(bytes_float[i][0], bytes_float[i][1], bytes_float[i][2]);
    return bytes;
}


static Image Uint32ToPixels(uint32_t *buffer, uint32_t _wid, uint32_t _hei) {
    uint32_t buf_size = _wid * _hei;
    float (*bytes)[4] = new float[buf_size][4];

    for (uint32_t i = 0; i < buf_size; ++i) ToF32RGB(buffer[i], bytes[i]), bytes[i][3] = 1;

    Image _out(_wid, _hei, bytes);
    delete[] bytes;

    return _out;
}
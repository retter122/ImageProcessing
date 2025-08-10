#pragma once

#include <iostream>
#include <stdint.h>
#include <windows.h>


// BITMAP IMAGE CLASS
class BitmapImage {
    private:
        uint32_t* bytes;
        uint32_t width, heigth;

    public:
        BitmapImage() : bytes(0), width(0), heigth(0) {}
        BitmapImage(uint32_t _wid, uint32_t _hei, uint32_t* _byt) : width(_wid), heigth(_hei), bytes(0) { this->copy_bytes(_byt); }
        BitmapImage(const BitmapImage& _obj) : width(_obj.width), heigth(_obj.heigth), bytes(0) { this->copy_bytes(_obj.bytes); }

        uint32_t get_width() { return this->width; }
        uint32_t get_height() { return this->heigth; }
        uint32_t *get_bytes() { return this->bytes; }

        void copy_bytes(uint32_t* _bytes) {
            if (!_bytes) return;

            uint32_t bytes_num = this->width * this->heigth;
            if (this->bytes) delete[] this->bytes;
            this->bytes = new uint32_t[bytes_num];

            memcpy(this->bytes, _bytes, bytes_num * sizeof(uint32_t));
        }

        ~BitmapImage() { if (this->bytes) delete[] this->bytes; }
};
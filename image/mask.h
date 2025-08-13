#pragma once

#include <iostream>
#include <windows.h>
#include <stdint.h>


// MASK CLASS
class Mask {
    private:
        float* bytes;
        uint32_t width, height;

    public:
        Mask(): bytes(0), width(0), height(0) {}
        Mask(uint32_t _wid, uint32_t _hei, float *_byt): width(_wid), height(_hei), bytes(0) { this->copy_bytes(_byt); }
        Mask(const Mask& _obj): width(_obj.width), height(_obj.height), bytes(0) { this->copy_bytes(_obj.bytes); }

        uint32_t get_width() const { return this->width; }
        uint32_t get_height() const { return this->height; }
        float* get_bytes() const { return this->bytes; }

        void copy_bytes(float *_bytes) {
            if (!_bytes) return;

            uint32_t bytes_num = this->width * this->height;
            if (this->bytes) delete[] this->bytes;
            this->bytes = new float[bytes_num];

            for (uint32_t i = 0; i < bytes_num; ++i) this->bytes[i] = _bytes[i];
        }

        void resize(uint32_t new_width, uint32_t new_height) {
            if (this->bytes) {
                uint32_t new_bytes_num = new_width * new_height;
                float *new_bytes = new float[new_bytes_num];

                float ypos = 0, yasp = (float)this->height / (float)new_height, xasp = (float)this->width / (float)new_width;
                for (uint32_t i = 0; i < new_height; ++i, ypos += yasp) {
                    float xpos = 0;
                    for (uint32_t j = 0; j < new_width; ++j, xpos += xasp) new_bytes[i * new_width + j] = this->bytes[(uint32_t)ypos * this->width + (uint32_t)xpos];
                }

                delete[] this->bytes;
                this->bytes = new_bytes;
            } this->width = new_width, this->height = new_height;
        } 

        ~Mask() { if (this->bytes) delete[] this->bytes; }
};
#pragma once

#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <windows.h>

#include "./image.h"


// BMP LOAD FUNCTION
static Image BmpLoad(const std::string& _path) {
    FILE* InpFile = fopen(_path.c_str(), "rb");
    if (!InpFile) return Image();

    BITMAPFILEHEADER BFile = { 0 };
    BITMAPINFOHEADER BInfo = { 0 };

    fread(&BFile, sizeof(BITMAPFILEHEADER), 1, InpFile);
    fread(&BInfo, sizeof(BITMAPINFOHEADER), 1, InpFile);
    if (BFile.bfType != 0x4d42) return Image();

    uint32_t bytes_num = BInfo.biHeight * BInfo.biWidth, inp_dword = 0;
    float (*bytes)[4] = new float[bytes_num][4];

    fseek(InpFile, BFile.bfOffBits, SEEK_SET);
    for (uint32_t i = 0; i < bytes_num; ++i) {
        fread(&inp_dword, BInfo.biBitCount / 8, 1, InpFile);
        ToF32RGB(inp_dword, bytes[i]);
    }

    Image _out(BInfo.biWidth, BInfo.biHeight, bytes);
    delete[] bytes;
    return _out;
}
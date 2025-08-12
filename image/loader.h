#pragma once

#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <windows.h>

#include "./image.h"
#include "./bitmap.h"


// BMP LOAD FUNCTIONS
static FILE* LoadBMPHeader(const std::string _path, BITMAPFILEHEADER* BFile, BITMAPINFOHEADER* BInfo) {
    FILE* Inp = fopen(_path.c_str(), "rb");
    if (!Inp) return 0;

    fread(BFile, sizeof(BITMAPFILEHEADER), 1, Inp);
    fread(BInfo, sizeof(BITMAPINFOHEADER), 1, Inp);

    return Inp;
}


static Image BmpLoad(const std::string& _path) {
    BITMAPFILEHEADER BFile = { 0 };
    BITMAPINFOHEADER BInfo = { 0 };

    FILE* InpFile = LoadBMPHeader(_path, &BFile, &BInfo);
    if (!InpFile || BFile.bfType != 0x4d42) return Image();

    uint32_t bytes_num = BInfo.biHeight * BInfo.biWidth, inp_dword = 0;
    float (*bytes)[4] = new float[bytes_num][4];

    fseek(InpFile, BFile.bfOffBits, SEEK_SET);
    for (uint32_t i = 0; i < bytes_num; ++i) {
        fread(&inp_dword, BInfo.biBitCount / 8, 1, InpFile);
        ToF32RGB(inp_dword, bytes[i]), bytes[i][3] = 1;
    }

    fclose(InpFile);
    Image _out(BInfo.biWidth, BInfo.biHeight, bytes);
    delete[] bytes;
    return _out;
}


static BitmapImage BmpBitmapLoad(const std::string& _path) {
    BITMAPFILEHEADER BFile = { 0 };
    BITMAPINFOHEADER BInfo = { 0 };

    FILE* InpFile = LoadBMPHeader(_path, &BFile, &BInfo);
    if (!InpFile || BFile.bfType != 0x4d42) return BitmapImage();

    uint32_t bytes_num = BInfo.biWidth * BInfo.biHeight;
    uint32_t* bytes = new uint32_t[bytes_num];

    fseek(InpFile, BFile.bfOffBits, SEEK_SET);
    for (uint32_t i = 0; i < bytes_num; ++i) fread(bytes + i, BInfo.biBitCount / 8, 1, InpFile);

    fclose(InpFile);
    BitmapImage _out(BInfo.biWidth, BInfo.biHeight, bytes);
    delete[] bytes;
    return _out;
}
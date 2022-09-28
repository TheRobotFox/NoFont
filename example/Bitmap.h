#pragma once
#include <stdio.h>
#include <stdlib.h>
#pragma pack(push,1)

struct Color{
    unsigned char B,G,R;
};


struct BitmapHeader{

    unsigned short Signature; // 0x4D42
    unsigned int FileSize; // img size+54
    unsigned char empty0[4];
    unsigned int options; // 54
    unsigned int InfoSignature; // 40
    unsigned int width;
    unsigned int height;
    unsigned int options2; // (1<<24)+24<<16
    unsigned char empty1[4];
    unsigned int imgsize; // pixels*4
    unsigned int ppm; // DPI*39
    unsigned int ppm2; //ppm
    unsigned char empty2[8];
};
#pragma pack (pop)

struct BitmapFile{
    struct BitmapHeader;
    struct Color data[1];
};

struct Color makeRGB(unsigned char R, unsigned char G, unsigned char B);
struct BitmapHeader CreateBitmapHeader(int width,int height);
char CreateBitmapFile(const char* Filename, int width,int height, struct Color Color);
char saveBitmap(const char* Filename, unsigned int width, unsigned int height , struct Color* data);
struct BitmapFile* ReadBitmap(const char* Filename);

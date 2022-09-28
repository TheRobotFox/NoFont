#include "Bitmap.h"

struct Color makeRGB(unsigned char R, unsigned char G, unsigned char B){
	struct Color Color = {B,G,R};
	return Color;
}

struct BitmapHeader CreateBitmapHeader(int width,int height){
	struct BitmapHeader Header = {0};
	unsigned int DPI = 72;
	unsigned int amountOfPixels = width*height;
	Header.Signature = 0x4D42;
	Header.FileSize = amountOfPixels*4+54;
	Header.imgsize = amountOfPixels*4;
	Header.options = 54;
	Header.InfoSignature = 40;
	Header.width = width;
	Header.height = height;
	Header.ppm = 39*DPI;
	Header.ppm2 = 39*DPI;
	Header.options2 = 24<<16;
	return Header;
}

char CreateBitmapFile(const char* Filename, int width,int height, struct Color Color){
	struct Color* data = malloc(sizeof(struct Color)*width*height);
    for(int i=0; i<width*height; i++){
        data[i]=Color;
    }
	char ret = saveBitmap(Filename,width,height,data);
	free(data);
	return ret;

}

char saveBitmap(const char* Filename, unsigned int width, unsigned int height , struct Color* data){

	struct BitmapHeader Header = CreateBitmapHeader(width,height);
	unsigned int amountOfPixels = width*height;
	FILE* Bitmap = 0;
	Bitmap = fopen(Filename, "wb");
	if(Bitmap){

		fwrite(&Header, 1, sizeof(struct BitmapHeader), Bitmap);
		for (unsigned int i = 0; i < amountOfPixels; i++) {

			fwrite(data+i, 1, 3, Bitmap);
		}

		fclose(Bitmap);
		return 0;
	}
	return 1;
}

struct BitmapFile* ReadBitmap(const char* Filename){

	FILE* Bitmap = 0;
	Bitmap = fopen(Filename, "rb");
	if(Bitmap){

		fseek(Bitmap,0,SEEK_END);
		unsigned int size = ftell(Bitmap);
		struct BitmapFile* BitmapFile = malloc(size);
		fseek(Bitmap,0,SEEK_SET);
		fread(BitmapFile,1,size,Bitmap);
		return BitmapFile;
	}
	return 0;
}

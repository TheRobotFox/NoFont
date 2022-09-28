#include "Bitmap.h"
#include "Font.font.h"
#include <stdio.h>
#include <string.h>

#define PIX_X 1024
#define PIX_Y 128
struct Color array[PIX_Y*PIX_X];

int Rect_fill(Font_Rect *rect, __attribute__((unused))void *ptr)
{
	int x_s=rect->x, y_s=rect->y, width=rect->width, heigth=rect->height, ret=0,
	x_shrik = PIX_X-x_s-width,
	y_shrik = PIX_Y-y_s-heigth;
	if(x_shrik<0){
		width-=x_shrik;
		ret = 1;
	}
	if(y_shrik<0){
		heigth-=y_shrik;
		ret = 1;
	}

	for(int y=0; y<heigth; y++)
	{
		for(int x=0; x<width; x++)
		{
			array[PIX_X*(PIX_Y-y-y_s-1)+x+x_s]=makeRGB(255,255,255);
		}
	}
	return ret;
}

int main(int argc, const char **argv)
{
	if(argc<2){
		printf("Usage: %s [TEXT]", argv[0]);
		exit(0);
	}
	puts(argv[1]);
	printf("%g | %g\n", font.h_kern, font.v_kern);
	Font_render_string_rect(&font, argv[1], strlen(argv[1]), 0,0,30,Rect_fill, NULL);
	const char *str = "MADE WITH NOFONT";
	const size_t len = strlen(str);
	int size = 9;
	Font_Rect rect = Font_string_dimensions(&font, str, len, size);
	printf("%d | %d\n", rect.width, rect.height);
	Font_render_string_rect(&font, str, len, PIX_X-rect.width,PIX_Y-rect.height,size,Rect_fill, NULL);
	saveBitmap("out.bmp", PIX_X, PIX_Y, array);
	return 0;
}

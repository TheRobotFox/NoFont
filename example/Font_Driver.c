#include "Font_Driver.h"

// get width of Printed Character
// the heigth is equal to size
int Font_Char_width(const struct Font_Char *Char, int size)
{
	if(!Char)
		return size/1.5;
	return (float)size*Char->width/Char->height+.5;
}

// Caluclate bounding box of a string
Font_Rect Font_string_dimensions(const struct Font *font, const char *str, size_t len, int size)
{
	Font_Rect rect = {0};
	int x = 0,
		y = 1;

	for(size_t i=0; i<len; i++)
	{
		if(!x)
			x-=size/font->height*font->h_kern;
		switch(str[i])
		{
		case '\n':

			rect.width= x>rect.width ? x : rect.width;
			x=0;
			y++;
			break;

		case ' ':
			x+=size*font->space_width/font->height+size/font->height*font->h_kern;
			break;

		default:
			x+=Font_Char_width(font->map[(size_t)str[i]], size)+size/font->height*font->h_kern;
		}
	}
	rect.width= x>rect.width ? x : rect.width;
	rect.height=size+(y-1)*size*font->v_kern;
	return rect;
}

// Render single Character using a Rect_Draw function provided by user
int Font_Char_render_rect(const struct Font_Char *Char, int x, int y, int size, f_Font_Driver_draw_rect callback, void *arg)
{
	Font_Rect rect;

	if(!Char){
		rect.x=x;
		rect.y=y;
		rect.width=Font_Char_width(Char, size);
		rect.height=size;
		return callback(&rect, arg);
	}

	float pixel_size = (float)size/Char->height;

	int index;
	for(int c_y=0; c_y<Char->height; c_y++)
	{
		for(int c_x=0; c_x<Char->width; c_x++)
		{
			index=Char->width*c_y+c_x;
			if(Char->data[index/8] & 0x80 >> (index % 8)){

				int s_x = c_x*pixel_size,
					s_y = c_y*pixel_size;

				rect.x=x+s_x;
				rect.y=y+s_y;

				rect.width=(c_x+1)*pixel_size-s_x;
				rect.height=(c_y+1)*pixel_size-s_y;

				int ret = callback(&rect, arg);
				if(ret)
					return ret;
			}
		}
	}
	return 0;
}

// Render whole string using a Rect_Draw function provided by user
int Font_render_string_rect(struct Font *font, const char *str, size_t len, int x, int y, int size, f_Font_Driver_draw_rect callback, void *arg)
{
	int x_start=x;

	for(size_t i=0; i<len; i++)
	{
		switch(str[i])
		{
		case '\n':
			x=x_start;
			y+=size*font->v_kern;
			break;

		case ' ':
			x+=size*font->space_width/font->height+size/font->height*font->h_kern;
			break;

		default:
			{
				int ret = Font_render_char_rect(font, str[i], x, y, size, callback, arg);
				if(ret)
					return i;
				x+=Font_Char_width(font->map[(size_t)str[i]], size)+size/font->height*font->h_kern;
			}
		}
	}
	return len;
}


// Pixel

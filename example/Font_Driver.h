#include <stddef.h>
struct Font_Char
{
	const unsigned short width, height;
	const unsigned char *data;
};

struct Font
{
	const struct Font_Char *charset;
	const struct Font_Char *map[256];
	unsigned short height;

	//Settings
	float h_kern;
	float v_kern;
	unsigned short space_width;
};

typedef struct{int x, y, width, height;} Font_Rect;

typedef int (*f_Font_Driver_draw_rect)(Font_Rect*, void*);
typedef int (*f_Font_Driver_draw_pixel)(int, int, void*);

int Font_Char_width(const struct Font_Char *Char, int size);
Font_Rect Font_string_dimensions(const struct Font *font, const char *str, size_t len, int size);

// Rect Render
int Font_Char_render_rect(const struct Font_Char *Char, int x, int y, int size, f_Font_Driver_draw_rect allback, void *arg);
#define Font_render_char_rect(Font, Char, x, y, size, callback, arg) Font_Char_render_rect((Font)->map[(size_t)Char], x, y, size, callback, arg)

int Font_render_string_rect(struct Font *font, const char *str, size_t len, int x, int y, int size, f_Font_Driver_draw_rect allback, void *arg);

// Pixel
int Font_Char_render_pixel(const struct Font_Char *Char, int x, int y, int size, f_Font_Driver_draw_pixel callback, void *arg);
#define Font_render_char_pixel(Font, Char, x, y, size, callback, arg) Font_Char_render_pixel(Font->map[(size_t)Char], x, y, size, callback, arg)

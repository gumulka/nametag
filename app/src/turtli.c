#include "nametag.h"
#include <zephyr/display/cfb.h>

#define turtli_width  43
#define turtli_height 19

static char turtli_black_bits[] = {
	0x00, 0x00, 0xf8, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x27, 0xe8, 0x01, 0x00, 0x00, 0xe0, 0x14,
	0x10, 0x03, 0x00, 0xf8, 0x11, 0x3c, 0xf0, 0x0e, 0x00, 0x04, 0x2e, 0xc2, 0x1f, 0x19, 0x00,
	0x22, 0x64, 0x81, 0x20, 0x36, 0x00, 0x21, 0x98, 0x83, 0xc0, 0xe9, 0x01, 0x03, 0x32, 0x9d,
	0x38, 0x11, 0x06, 0xfc, 0xc7, 0xe8, 0x27, 0x1e, 0x04, 0x00, 0x9c, 0x47, 0xc4, 0x0f, 0x02,
	0x00, 0x38, 0x5c, 0x74, 0xf8, 0x01, 0x00, 0x48, 0xe8, 0xdf, 0x08, 0x00, 0x00, 0x48, 0x70,
	0x00, 0x0f, 0x00, 0x00, 0x98, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x90, 0x80, 0x01, 0x00, 0x00,
	0x00, 0x30, 0x03, 0x03, 0x00, 0x00, 0x00, 0xa0, 0x04, 0x06, 0x00, 0x00, 0x00, 0xe0, 0x18,
	0x04, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00};
static char turtli_dark_grey_bits[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x17, 0x00, 0x00, 0x00, 0x00, 0xeb,
	0xef, 0x00, 0x00, 0x00, 0xe0, 0xc3, 0x0f, 0x01, 0x00, 0x00, 0xd0, 0x3d, 0xe0, 0x06, 0x00,
	0x00, 0x98, 0x7e, 0xdf, 0x09, 0x00, 0x00, 0x60, 0x7c, 0x3f, 0x16, 0x00, 0x00, 0xc0, 0x62,
	0xc7, 0x0e, 0x00, 0x00, 0x00, 0x17, 0xd8, 0x01, 0x00, 0x00, 0x00, 0xb8, 0x3b, 0x00, 0x00,
	0x00, 0x00, 0xa0, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static char turtli_light_grey_bits[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x01, 0x00, 0x00, 0x00, 0x00,
	0xdc, 0x03, 0x00, 0x00, 0x00, 0x00, 0xde, 0x07, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0d, 0x00,
	0x00, 0xe0, 0x01, 0x00, 0x38, 0x00, 0x00, 0xe0, 0x03, 0x00, 0x60, 0x00, 0x00, 0xf0, 0x01,
	0x00, 0xc0, 0x03, 0x80, 0x07, 0x00, 0x00, 0xb0, 0x07, 0x00, 0x07, 0x00, 0x00, 0xb0, 0x0f,
	0x00, 0x00, 0x00, 0x00, 0x60, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x60, 0x7f, 0x00, 0x00, 0x00,
	0x00, 0xc0, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x40, 0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0xe0,
	0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static int draw_pixel(const struct device *const display, int x, int y, int factor)
{
	struct cfb_position pixel;
	for (int i_x = 0; i_x < factor; i_x++) {
		for (int i_y = 0; i_y < factor; i_y++) {
			pixel.x = x + i_x;
			pixel.y = y + i_y;
			cfb_draw_point(display, &pixel);
		}
	}
	return 0;
}

static int draw_image(const struct device *const display, int x_pos, int y_pos, int width,
		      int height, int factor, char *data)
{
	int width_offset = (width + 7) / 8;
	width_offset *= 8;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {

			int pos = y * width_offset + x;
			int bit = pos % 8;
			pos /= 8;
			if (data[pos] >> bit & 1) {
				draw_pixel(display, x * factor + x_pos, y * factor + y_pos, factor);
			}
		}
	}
	return 0;
}

int draw_turtli(const struct device *const display, int x, int y, int factor)
{
	int err;
	draw_image(display, x, y, turtli_width, turtli_height, factor, turtli_light_grey_bits);
	cfb_framebuffer_finalize(display);
	err = cfb_framebuffer_clear(display, true);
	err = cfb_framebuffer_clear(display, true);
	draw_image(display, x, y, turtli_width, turtli_height, factor, turtli_dark_grey_bits);
	cfb_framebuffer_finalize(display);
	err = cfb_framebuffer_clear(display, true);
	err = cfb_framebuffer_clear(display, true);
	draw_image(display, x, y, turtli_width, turtli_height, factor, turtli_black_bits);
	return err;
}

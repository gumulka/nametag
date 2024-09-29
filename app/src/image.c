
#include <zephyr/display/cfb.h>

int draw_pixel(const struct device *const display, int x, int y, int factor)
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

int draw_image(const struct device *const display, int x_pos, int y_pos, int width, int height,
	       int factor, char *data)
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

#include <zephyr/kernel.h>

int draw_pixel(const struct device *const display, int x, int y, int factor);

int draw_image(const struct device *const display, int x_pos, int y_pos, int width, int height,
	       int factor, char *data);

#include <zephyr/kernel.h>

int clear_display(const struct device *const display);

int serious_name(const struct device *const display);
int lgbtq_name(const struct device *const display);

int draw_turtli(const struct device *const display, int x, int y, int factor);

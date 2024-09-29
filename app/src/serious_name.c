#include "nametag.h"

#include <zephyr/display/cfb.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(serious_name, CONFIG_APP_LOG_LEVEL);

static int serious_name_show(const struct device *const display, char *name)
{
	int err;

	uint8_t width, heigth;

	err = cfb_set_kerning(display, 0);
	if (err) {
		return err;
	}
	err = cfb_framebuffer_set_font(display, 2);
	if (err) {
		return err;
	}
	cfb_get_font_size(display, 2, &width, &heigth);
	int num_chr = strlen(name);
	int position = (296 - num_chr * width) / 2;

	err = cfb_draw_text(display, name, position, 40);
	if (err) {
		return err;
	}
	err = cfb_framebuffer_set_font(display, 1);
	if (err) {
		return err;
	}
	err = cfb_draw_text(display, "er/ihm", 110, 70);
	if (err) {
		return err;
	}

	err = cfb_framebuffer_finalize(display);
	if (err) {
		return err;
	}

	return 0;
}

int serious_name(const struct device *const display)
{
	return serious_name_show(display, "Fabian");
}

int serious_nick(const struct device *const display)
{
	return serious_name_show(display, "Fabian/Gumulka");
}

int serious_full_name(const struct device *const display)
{
	return serious_name_show(display, "Fabian Pflug");
}

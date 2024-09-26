#include "nametag.h"

#include <zephyr/display/cfb.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(serious_name, CONFIG_APP_LOG_LEVEL);

int serious_name(const struct device *const display)
{
	int err;

	err = cfb_set_kerning(display, 0);
	if (err) {
		return err;
	}
	err = cfb_framebuffer_set_font(display, 2);
	if (err) {
		return err;
	}
	err = cfb_draw_text(display, "Fabian Pflug", 25, 40);
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

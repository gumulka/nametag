#include "nametag.h"

#include <zephyr/display/cfb.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(lgbtq_name, CONFIG_APP_LOG_LEVEL);

int lgbtq_name(const struct device *const display)
{
	int err;

	display_blanking_on(display);

	err = cfb_framebuffer_clear(display, true);
	if (err) {
		LOG_WRN("Framebuffer clear error=%d", err);
		return err;
	}
	display_blanking_off(display);

	err = cfb_set_kerning(display, 0);
	if (err) {
		return err;
	}
	err = cfb_framebuffer_set_font(display, 2);
	if (err) {
		return err;
	}
	err = cfb_draw_text(display, "Fabian Pflug", 25, 10);
	if (err) {
		return err;
	}

	err = cfb_framebuffer_set_font(display, 1);
	if (err) {
		return err;
	}
	err = cfb_draw_text(display, "er/ihm", 110, 45);
	if (err) {
		return err;
	}

	err = cfb_framebuffer_set_font(display, 0);
	if (err) {
		return err;
	}
	err = cfb_set_kerning(display, 4);
	if (err) {
		return err;
	}
	err = cfb_draw_text(display, "Kinky/Poly/Allo-Aro", 15, 80);
	if (err) {
		return err;
	}
	err = cfb_draw_text(display, "Ich liebe Umarmungen", 10, 110);
	if (err) {
		return err;
	}

	err = cfb_framebuffer_finalize(display);
	if (err) {
		return err;
	}

	return 0;
}

#include "nametag.h"

#include <zephyr/display/cfb.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(lgbtq_name, CONFIG_APP_LOG_LEVEL);

int lgbtq_name_display(const struct device *const display, bool with_umarmungen)
{
	int err;
	int y_pos = 10;

	err = cfb_set_kerning(display, 0);
	if (err) {
		return err;
	}
	err = cfb_framebuffer_set_font(display, 2);
	if (err) {
		return err;
	}
	if (!with_umarmungen) {
		y_pos += 10;
	}
	err = cfb_draw_text(display, "Fabian Pflug", 25, y_pos);
	if (err) {
		return err;
	}

	err = cfb_framebuffer_set_font(display, 1);
	if (err) {
		return err;
	}
	y_pos += 35;
	err = cfb_draw_text(display, "er/ihm", 110, y_pos);
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

	y_pos += 35;
	if (!with_umarmungen) {
		y_pos += 10;
	}
	err = cfb_draw_text(display, "Kinky/Poly/Allo-Aro", 15, y_pos);
	if (err) {
		return err;
	}

	if (with_umarmungen) {
		y_pos += 30;
		err = cfb_draw_text(display, "Ich liebe Umarmungen", 10, y_pos);
		if (err) {
			return err;
		}
	}

	err = cfb_framebuffer_finalize(display);
	if (err) {
		return err;
	}

	return 0;
}

int lgbtq_name(const struct device *const display)
{
	return lgbtq_name_display(display, false);
}

int lgbtq_name_extra(const struct device *const display)
{
	return lgbtq_name_display(display, true);
}

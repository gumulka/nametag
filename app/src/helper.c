#include "nametag.h"

#include <zephyr/display/cfb.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(helper, CONFIG_APP_LOG_LEVEL);

int clear_display(const struct device *const display)
{
	int err;
	display_blanking_on(display);

	err = cfb_framebuffer_clear(display, false);
	if (err) {
		LOG_WRN("Framebuffer clear error=%d", err);
		return err;
	}
	return display_blanking_off(display);
}

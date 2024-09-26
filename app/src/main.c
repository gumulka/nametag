#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/display/cfb.h>

#include "nametag.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

static const struct device *const display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

int main(void)
{
	int ret;

	if (!gpio_is_ready_dt(&led)) {
		LOG_ERR("No LED defined");
		return -ENODEV;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		LOG_ERR("Could not configure LED.");
		return 0;
	}

	if (!device_is_ready(display)) {
		LOG_WRN("Display device not ready");
		return -ENODEV;
	}

	ret = cfb_framebuffer_init(display);
	if (ret) {
		LOG_WRN("Framebuffer initialization failed!");
		return ret;
	}

	clear_display(display);
	serious_name(display);

	k_msleep(1000 * 20);

	clear_display(display);
	lgbtq_name(display);

	k_msleep(1000 * 20);

	clear_display(display);

	draw_turtli(display, 150, 50, 3);
	cfb_set_kerning(display, 0);
	cfb_framebuffer_set_font(display, 2);
	cfb_draw_text(display, "Fabilicious", 25, 10);
	cfb_framebuffer_set_font(display, 1);
	cfb_draw_text(display, "Ich mag", 25, 55);
	cfb_draw_text(display, "kuscheln", 25, 80);
	cfb_framebuffer_finalize(display);

	while (true) {
		gpio_pin_toggle_dt(&led);
		k_msleep(500);
	}

	return 0;
}

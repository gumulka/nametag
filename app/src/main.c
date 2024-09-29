#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/display/cfb.h>

#include "nametag.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(DT_ALIAS(sw0), gpios, {0});
static struct gpio_callback button_cb_data;

static const struct device *const display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

static int current_image = 0;

typedef int (*image_display)(const struct device *const display);

image_display images[10] = {
	serious_name, serious_nick,     serious_full_name,
	lgbtq_name,   lgbtq_name_extra, fabilicious_turtli,
};

static void set_display(struct k_work *_work)
{
	clear_display(display);
	if (current_image < ARRAY_SIZE(images) && images[current_image]) {
		images[current_image](display);
	} else {
		current_image = -1;
	}
}
K_WORK_DELAYABLE_DEFINE(set_display_work, set_display);

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
	current_image++;
	k_work_reschedule(&set_display_work, K_MSEC(1000));
}

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

	if (!gpio_is_ready_dt(&button)) {
		printk("Error: button device %s is not ready\n", button.port->name);
		return -ENODEV;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n", ret, button.port->name,
		       button.pin);
		return 0;
	}

	ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n", ret,
		       button.port->name, button.pin);
		return 0;
	}

	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);

	k_work_reschedule(&set_display_work, K_MSEC(1000));

	return 0;
}

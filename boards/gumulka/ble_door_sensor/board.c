
#include <zephyr/drivers/gpio.h>
#include <zephyr/init.h>

static int board_nametag_init(void)
{
	const struct gpio_dt_spec enable = GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), enable_gpios);

	if (!gpio_is_ready_dt(&enable)) {
		return -ENODEV;
	}

	(void)gpio_pin_configure_dt(&enable, GPIO_OUTPUT_ACTIVE);

	return 0;
}

SYS_INIT(board_nametag_init, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEVICE);

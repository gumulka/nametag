#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci_types.h>
#include <zephyr/bluetooth/uuid.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, CONFIG_APP_LOG_LEVEL);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

static struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR),
	BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME) - 1)};

static bool error = false;
static bool done_bt_init = false;

static void bt_ready(int err)
{
	if (err) {
		LOG_ERR("Bluetooth init failed (err %d)", err);
		error = true;
		return;
	}

	/* Start advertising */
	err = bt_le_adv_start(
		BT_LE_ADV_PARAM(BT_LE_ADV_OPT_CONNECTABLE | BT_LE_ADV_OPT_USE_IDENTITY,
				BT_LE_ADV_INTERVAL_MAX / 2, BT_LE_ADV_INTERVAL_MAX, NULL),
		ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		LOG_ERR("Advertising failed to start (err %d)", err);
		error = true;
		return;
	}
	LOG_INF("Started Advertising");
	done_bt_init = true;
}

int main(void)
{
	error = false;
	int ret;

	if (!gpio_is_ready_dt(&led)) {
		LOG_ERR("No LED defined");
		error = true;
		return -ENODEV;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		LOG_ERR("Could not configure LED.");
		error = true;
		return 0;
	}

	/* Initialize the Bluetooth Subsystem */
	ret = bt_enable(bt_ready);
	if (ret) {
		LOG_ERR("Bluetooth init failed (err %d)", ret);
		error = true;
		return ret;
	}

	LOG_INF("Toggling LED");
	while (!done_bt_init) {
		gpio_pin_toggle_dt(&led);
		k_msleep(100);
	}

	while (!error) {
		gpio_pin_toggle_dt(&led);
		k_msleep(500);
	}

	return 0;
}

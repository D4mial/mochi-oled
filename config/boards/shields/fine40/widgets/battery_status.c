#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/battery.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/event_manager.h>

#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
#include <zmk/usb.h>
#include <zmk/events/usb_conn_state_changed.h>
#endif

#include "battery_status.h"

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct battery_state {
    uint8_t level;
    bool usb_present;
};

static void set_battery_symbol(lv_obj_t *label, struct battery_state state) {
    const char *symbol;

    if (state.usb_present) {
        symbol = LV_SYMBOL_CHARGE;
    } else if (state.level >= 90) {
        symbol = LV_SYMBOL_BATTERY_FULL;
    } else if (state.level >= 65) {
        symbol = LV_SYMBOL_BATTERY_3;
    } else if (state.level >= 40) {
        symbol = LV_SYMBOL_BATTERY_2;
    } else if (state.level >= 15) {
        symbol = LV_SYMBOL_BATTERY_1;
    } else {
        symbol = LV_SYMBOL_BATTERY_EMPTY;
    }

    lv_label_set_text(label, symbol);
}

static void battery_status_update_cb(struct battery_state state) {
    struct zmk_widget_battery_status *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        set_battery_symbol(widget->obj, state);
    }
}

static struct battery_state battery_status_get_state(const zmk_event_t *eh) {
    return (struct battery_state){
        .level = zmk_battery_state_of_charge(),
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
        .usb_present = zmk_usb_is_powered(),
#endif
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_status, struct battery_state, battery_status_update_cb,
                             battery_status_get_state)

ZMK_SUBSCRIPTION(widget_battery_status, zmk_battery_state_changed);
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
ZMK_SUBSCRIPTION(widget_battery_status, zmk_usb_conn_state_changed);
#endif

int zmk_widget_battery_status_init(struct zmk_widget_battery_status *widget, lv_obj_t *parent) {
    widget->obj = lv_label_create(parent);

    lv_label_set_text(widget->obj, LV_SYMBOL_BATTERY_FULL);

    sys_slist_append(&widgets, &widget->node);

    widget_battery_status_init();

    return 0;
}

lv_obj_t *zmk_widget_battery_status_obj(struct zmk_widget_battery_status *widget) {
    return widget->obj;
}

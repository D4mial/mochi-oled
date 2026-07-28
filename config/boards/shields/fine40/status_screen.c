#include <zephyr/kernel.h>
#include <lvgl.h>

#include "widgets/output_status.h"
#include "widgets/battery_status.h"

static struct zmk_widget_output_status output_status_widget;
static struct zmk_widget_battery_status battery_status_widget;

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen = lv_obj_create(NULL);

    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(screen, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(screen, 8, 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(screen, lv_color_black(), 0);

    zmk_widget_output_status_init(&output_status_widget, screen);
    lv_obj_set_style_text_align(zmk_widget_output_status_obj(&output_status_widget), LV_TEXT_ALIGN_CENTER, 0);

    zmk_widget_battery_status_init(&battery_status_widget, screen);
    lv_obj_set_style_text_align(zmk_widget_battery_status_obj(&battery_status_widget), LV_TEXT_ALIGN_CENTER, 0);

    return screen;
}

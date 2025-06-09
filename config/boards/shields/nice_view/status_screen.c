#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/display/cfb.h>
#include <zmk/display.h>
#include <zmk/display/status_screen.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/events/wpm_state_changed.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/events/endpoint_changed.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/events/split_peripheral_status_changed.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if IS_ENABLED(CONFIG_ZMK_WIDGET_BATTERY_STATUS)
#include <zmk/display/widgets/battery_status.h>
static struct zmk_widget_battery_status battery_status_widget;
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_OUTPUT_STATUS)
#include <zmk/display/widgets/output_status.h>
static struct zmk_widget_output_status output_status_widget;
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_LAYER_STATUS)
#include <zmk/display/widgets/layer_status.h>
static struct zmk_widget_layer_status layer_status_widget;
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_WPM_STATUS)
#include <zmk/display/widgets/wpm_status.h>
static struct zmk_widget_wpm_status wpm_status_widget;
#endif

// Include company logo image
#include "images/company_logo.h"

static void zmk_display_status_screen_init(struct device *display) {
    lv_obj_t *screen;
    
    screen = lv_obj_create(NULL);

    lv_obj_t *centered_container = lv_obj_create(screen);
    lv_obj_set_size(centered_container, 128, 32);
    lv_obj_align(centered_container, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_layout(centered_container, LV_LAYOUT_GRID);
    lv_obj_set_style_grid_cell_column_pos(centered_container, 0, 0);
    lv_obj_set_style_grid_cell_column_span(centered_container, 2, 0);
    lv_obj_set_style_grid_column_dsc_array(centered_container, (const lv_coord_t[]){60, 60, LV_GRID_TEMPLATE_LAST}, 0);
    lv_obj_set_style_grid_row_dsc_array(centered_container, (const lv_coord_t[]){32, LV_GRID_TEMPLATE_LAST}, 0);

    uint8_t screen_index = zmk_display_is_pred_left_hand() ? 0 : 1;
    
    if (screen_index == 0) {
        // Left screen - Small output status and other widgets
#if IS_ENABLED(CONFIG_ZMK_WIDGET_BATTERY_STATUS)
        zmk_widget_battery_status_init(&battery_status_widget, centered_container);
        lv_obj_align(zmk_widget_battery_status_obj(&battery_status_widget), LV_ALIGN_TOP_LEFT, 0, 0);
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_OUTPUT_STATUS)
        zmk_widget_output_status_init(&output_status_widget, centered_container);
        lv_obj_align(zmk_widget_output_status_obj(&output_status_widget), LV_ALIGN_TOP_RIGHT, 0, 0);
        lv_obj_set_size(zmk_widget_output_status_obj(&output_status_widget), 16, 16); // Smaller output status
#endif

        // Small logo in the center of the left screen
        lv_obj_t *left_logo = lv_img_create(centered_container);
        lv_img_set_src(left_logo, &company_logo);
        lv_obj_align(left_logo, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_size(left_logo, 32, 16); // Smaller than the one on the right

#if IS_ENABLED(CONFIG_ZMK_WIDGET_LAYER_STATUS)
        zmk_widget_layer_status_init(&layer_status_widget, centered_container);
        lv_obj_align(zmk_widget_layer_status_obj(&layer_status_widget), LV_ALIGN_BOTTOM_LEFT, 0, 0);
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_WPM_STATUS)
        zmk_widget_wpm_status_init(&wpm_status_widget, centered_container);
        lv_obj_align(zmk_widget_wpm_status_obj(&wpm_status_widget), LV_ALIGN_BOTTOM_RIGHT, 0, 0);
#endif
    } else {
        // Right screen - Company logo in the middle
#if IS_ENABLED(CONFIG_ZMK_WIDGET_BATTERY_STATUS)
        zmk_widget_battery_status_init(&battery_status_widget, centered_container);
        lv_obj_align(zmk_widget_battery_status_obj(&battery_status_widget), LV_ALIGN_TOP_LEFT, 0, 0);
#endif

        // Company logo in the center
        lv_obj_t *logo = lv_img_create(centered_container);
        lv_img_set_src(logo, &company_logo);
        lv_obj_align(logo, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_size(logo, 64, 32);

#if IS_ENABLED(CONFIG_ZMK_WIDGET_LAYER_STATUS)
        zmk_widget_layer_status_init(&layer_status_widget, centered_container);
        lv_obj_align(zmk_widget_layer_status_obj(&layer_status_widget), LV_ALIGN_BOTTOM_LEFT, 0, 0);
#endif

#if IS_ENABLED(CONFIG_ZMK_WIDGET_WPM_STATUS)
        zmk_widget_wpm_status_init(&wpm_status_widget, centered_container);
        lv_obj_align(zmk_widget_wpm_status_obj(&wpm_status_widget), LV_ALIGN_BOTTOM_RIGHT, 0, 0);
#endif
    }

    zmk_display_set_status_screen(screen);
}

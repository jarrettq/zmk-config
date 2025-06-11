/*
 * Custom status screen for Corne right half
 * Shows custom image and status information
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zmk/display.h>
#include <zmk/display/widgets/output_status.h>
#include <zmk/display/widgets/battery_status.h>
#include <zmk/display/widgets/layer_status.h>
#include "custom_image.h"

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if IS_ENABLED(CONFIG_ZMK_DISPLAY_STATUS_SCREEN_CUSTOM)

static struct zmk_widget_layer_status layer_status_widget;
static struct zmk_widget_battery_status battery_status_widget;
static struct zmk_widget_output_status output_status_widget;

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen = lv_obj_create(NULL);
    
    // Set screen background
    lv_obj_set_style_bg_color(screen, lv_color_black(), LV_PART_MAIN);
    
    // Create main container for the image
    lv_obj_t *img_container = lv_obj_create(screen);
    lv_obj_set_size(img_container, 128, 64);
    lv_obj_align(img_container, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(img_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(img_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(img_container, 0, 0);
    
    // Add custom image
    lv_obj_t *img = lv_img_create(img_container);
    lv_img_set_src(img, &niceview_oled_img);
    lv_obj_center(img);
    
    // Create status container at bottom
    lv_obj_t *status_container = lv_obj_create(screen);
    lv_obj_set_size(status_container, 128, 16);
    lv_obj_align(status_container, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_opa(status_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(status_container, LV_OPA_TRANSP, 0);
    lv_obj_set_flex_flow(status_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(status_container, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    // Initialize status widgets
    zmk_widget_layer_status_init(&layer_status_widget, status_container);
    zmk_widget_battery_status_init(&battery_status_widget, status_container);
    zmk_widget_output_status_init(&output_status_widget, status_container);
    
    return screen;
}

#endif // CONFIG_ZMK_DISPLAY_STATUS_SCREEN_CUSTOM

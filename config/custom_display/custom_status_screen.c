/*
 * Custom status screen for Corne keyboard
 * Shows custom image on right half, simple status on left half
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zmk/display.h>
#include "custom_image.h"

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if IS_ENABLED(CONFIG_ZMK_DISPLAY_STATUS_SCREEN_CUSTOM)

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen = lv_obj_create(NULL);
    
    // Set screen background
    lv_obj_set_style_bg_color(screen, lv_color_black(), LV_PART_MAIN);
    
#if CONFIG_ZMK_SPLIT_ROLE_PERIPHERAL
    // RIGHT HALF: Show custom image
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
    
    // Add "R" indicator for right half
    lv_obj_t *status_label = lv_label_create(screen);
    lv_label_set_text(status_label, "R");
    lv_obj_set_style_text_color(status_label, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_font(status_label, &lv_font_montserrat_10, LV_PART_MAIN);
    lv_obj_align(status_label, LV_ALIGN_BOTTOM_RIGHT, -2, -2);
#else
    // LEFT HALF: Show simple status
    lv_obj_t *container = lv_obj_create(screen);
    lv_obj_set_size(container, 128, 64);
    lv_obj_align(container, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(container, LV_OPA_TRANSP, 0);
    
    // Simple status display for left half
    lv_obj_t *status_label = lv_label_create(container);
    lv_label_set_text(status_label, "CORNE\nKEYBOARD\n\nLeft Half");
    lv_obj_set_style_text_color(status_label, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_style_text_align(status_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(status_label);
#endif
    
    return screen;
}

#endif // CONFIG_ZMK_DISPLAY_STATUS_SCREEN_CUSTOM

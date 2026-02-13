/*
 * Idle screen for Prospector dongle display.
 * Shows a centered logo image when the keyboard is idle.
 */

#include "idle_screen.h"

LV_IMG_DECLARE(logo_dongle);

lv_obj_t *create_idle_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(screen, 255, LV_PART_MAIN);

    lv_obj_t *img = lv_img_create(screen);
    lv_img_set_src(img, &logo_dongle);
    lv_obj_center(img);

    return screen;
}

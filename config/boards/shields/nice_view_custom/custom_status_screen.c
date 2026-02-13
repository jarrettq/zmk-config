/*
 * Copyright (c) 2023 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 *
 * Custom status screen entry point for nice_view_custom shield.
 * Provides zmk_display_status_screen() which ZMK calls to obtain
 * the root LVGL screen object for the display.
 */

#include "widgets/peripheral_screen.h"

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if IS_ENABLED(CONFIG_NICE_VIEW_CUSTOM_WIDGET)
static struct zmk_widget_peripheral_screen screen_widget;
#endif

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen;
    screen = lv_obj_create(NULL);

#if IS_ENABLED(CONFIG_NICE_VIEW_CUSTOM_WIDGET)
    zmk_widget_peripheral_screen_init(&screen_widget, screen);
    lv_obj_align(zmk_widget_peripheral_screen_obj(&screen_widget), LV_ALIGN_TOP_LEFT, 0, 0);
#endif

    return screen;
}

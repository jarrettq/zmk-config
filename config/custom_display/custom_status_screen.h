/*
 * Custom status screen header
 */

#pragma once

#include <lvgl.h>

#if IS_ENABLED(CONFIG_ZMK_DISPLAY_STATUS_SCREEN_CUSTOM)
lv_obj_t *zmk_display_status_screen();
#endif

/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zmk/display.h>
#include <zmk/display/widgets/output_status.h>
#include <zmk/display/widgets/battery_status.h>
#include <zmk/display/widgets/layer_status.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/events/endpoint_selection_changed.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/usb.h>
#include <zmk/ble.h>
#include <zmk/endpoints.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static struct zmk_widget_layer_status layer_status_widget;
static struct zmk_widget_battery_status battery_status_widget;
static struct zmk_widget_output_status output_status_widget;

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen;
    screen = lv_obj_create(NULL);

    // Create a container for the custom art
    lv_obj_t *art_container = lv_obj_create(screen);
    lv_obj_set_size(art_container, 128, 32);
    lv_obj_align(art_container, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_opa(art_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(art_container, LV_OPA_TRANSP, 0);

    // Add custom ASCII art
    lv_obj_t *art_label = lv_label_create(art_container);
    lv_label_set_text(art_label, 
        "      /\\_/\\  \n"
        "     ( o.o ) \n"
        "      > ^ <  \n"
        "    Corne KB ");
    lv_obj_align(art_label, LV_ALIGN_CENTER, 0, -5);
    lv_obj_set_style_text_align(art_label, LV_TEXT_ALIGN_CENTER, 0);

    // Add status widgets at the bottom
    lv_obj_t *status_container = lv_obj_create(screen);
    lv_obj_set_size(status_container, 128, 32);
    lv_obj_align(status_container, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_opa(status_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(status_container, LV_OPA_TRANSP, 0);

    // Initialize widgets
    zmk_widget_layer_status_init(&layer_status_widget, status_container);
    lv_obj_align(zmk_widget_layer_status_obj(&layer_status_widget), LV_ALIGN_TOP_LEFT, 0, 0);

    zmk_widget_battery_status_init(&battery_status_widget, status_container);
    lv_obj_align(zmk_widget_battery_status_obj(&battery_status_widget), LV_ALIGN_TOP_RIGHT, 0, 0);

    zmk_widget_output_status_init(&output_status_widget, status_container);
    lv_obj_align(zmk_widget_output_status_obj(&output_status_widget), LV_ALIGN_BOTTOM_LEFT, 0, 0);

    return screen;
}

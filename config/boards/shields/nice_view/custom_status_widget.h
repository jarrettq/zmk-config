/*
 * Copyright (c) 2023 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <lvgl.h>
#include <zmk/display/widgets/output_status.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/ble.h>
#include <zmk/endpoints.h>

LV_IMG_DECLARE(bluetooth_rune);

struct zmk_widget_output_status {
    sys_slist_t children;
    lv_obj_t *container;
    lv_obj_t *icon;
    lv_obj_t *prof_num;
};

static void set_status_symbol(lv_obj_t *icon, lv_obj_t *prof_num, struct zmk_widget_output_status *widget) {
    enum zmk_endpoint endpoint = zmk_endpoints_selected();
    
    switch (endpoint) {
    case ZMK_ENDPOINT_USB:
        lv_obj_add_flag(prof_num, LV_OBJ_FLAG_HIDDEN);
        lv_img_set_src(icon, LV_SYMBOL_USB);
        break;
    case ZMK_ENDPOINT_BLE: {
        int active_profile = zmk_ble_active_profile_index();
        if (active_profile >= 0) {
            char text[4] = {};
            snprintf(text, sizeof(text), "%d", active_profile + 1);
            lv_label_set_text(prof_num, text);
            lv_obj_clear_flag(prof_num, LV_OBJ_FLAG_HIDDEN);
            lv_img_set_src(icon, &bluetooth_rune);
        } else {
            lv_obj_add_flag(prof_num, LV_OBJ_FLAG_HIDDEN);
            lv_img_set_src(icon, &bluetooth_rune); // Show Bluetooth symbol without profile
        }
        break;
    }
    default:
        lv_obj_add_flag(prof_num, LV_OBJ_FLAG_HIDDEN);
        lv_img_set_src(icon, LV_SYMBOL_CLOSE);
        break;
    }
}

static void output_status_update_cb(struct zmk_widget_output_status *widget) {
    set_status_symbol(widget->icon, widget->prof_num, widget);
}

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct zmk_widget_output_status *zmk_widget_output_status_create(lv_obj_t *parent) {
    struct zmk_widget_output_status *widget = zmk_display_alloc(sizeof(struct zmk_widget_output_status));

    if (widget == NULL) {
        return NULL;
    }

    widget->container = lv_obj_create(parent);
    lv_obj_set_size(widget->container, 20, 24);
    lv_obj_clear_flag(widget->container, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(widget->container, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Create the Bluetooth icon and make it smaller than default
    widget->icon = lv_img_create(widget->container);
    lv_obj_align(widget->icon, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_size(widget->icon, 16, 16); // Smaller icon size
    
    // Create a label to display the profile number
    widget->prof_num = lv_label_create(widget->container);
    lv_obj_align(widget->prof_num, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_label_set_text(widget->prof_num, "");
    lv_obj_set_style_text_color(widget->prof_num, lv_color_white(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(widget->prof_num, &lv_font_unscii_8, LV_PART_MAIN | LV_STATE_DEFAULT);

    sys_slist_append(&widgets, &widget->children);
    output_status_update_cb(widget);

    return widget;
}

lv_obj_t *zmk_widget_output_status_obj(struct zmk_widget_output_status *widget) {
    return widget->container;
}

int zmk_widget_output_status_init(struct zmk_display_status *status) {
    struct zmk_widget_output_status *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, children) {
        output_status_update_cb(widget);
    }
    return 0;
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_output_status, struct zmk_widget_output_status,
                           zmk_widget_output_status_init, output_status_update_cb)

ZMK_SUBSCRIPTION(widget_output_status, zmk_endpoint_changed);
ZMK_SUBSCRIPTION(widget_output_status, zmk_ble_active_profile_changed);
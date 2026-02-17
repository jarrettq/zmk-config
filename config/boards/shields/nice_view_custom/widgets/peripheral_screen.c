/*
 * Copyright (c) 2023 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 *
 * Peripheral screen widget for nice_view_custom.
 * Displays: battery level, connection status, and a per-half logo.
 *
 * The nice!view Sharp LCD is physically mounted in portrait but driven
 * as 160x68 landscape in LVGL. Canvas content is drawn in portrait
 * orientation (68x68) and rotated 270° before display. The logo image
 * is placed as a standalone image widget (pre-rotated in C data).
 */

#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/split_peripheral_status_changed.h>
#include <zmk/split/bluetooth/peripheral.h>
#include <zmk/battery.h>

#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
#include <zmk/usb.h>
#include <zmk/events/usb_conn_state_changed.h>
#endif

#include "peripheral_screen.h"

/* Select logo image based on which corne half we're building for */
#if defined(CONFIG_SHIELD_CORNE_LEFT)
LV_IMG_DECLARE(logo_left);
#define LOGO_IMG (&logo_left)
#elif defined(CONFIG_SHIELD_CORNE_RIGHT)
LV_IMG_DECLARE(logo_right);
#define LOGO_IMG (&logo_right)
#else
/* Fallback for non-corne builds or testing */
LV_IMG_DECLARE(logo_left);
#define LOGO_IMG (&logo_left)
#endif

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

/*
 * Draw the status canvas (top-right 68x68 area).
 * Content is drawn in "portrait" (y = reading direction)
 * and then rotated 270° to match the physical display orientation.
 */
static void draw_top(lv_obj_t *widget_obj, lv_color_t cbuf[], const struct peripheral_state *state) {
    lv_obj_t *canvas = lv_obj_get_child(widget_obj, 0);

    /* Fill background */
    lv_draw_rect_dsc_t rect_bg_dsc;
    init_rect_dsc(&rect_bg_dsc, BACKGROUND);
    lv_canvas_draw_rect(canvas, 0, 0, CANVAS_SIZE, CANVAS_SIZE, &rect_bg_dsc);

    /* Draw battery percentage */
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, FOREGROUND, &lv_font_montserrat_16, LV_TEXT_ALIGN_RIGHT);

    char bat_text[8];
    snprintf(bat_text, sizeof(bat_text), "%d%%", state->battery);
    lv_canvas_draw_text(canvas, 2, 4, 64, &label_dsc, bat_text);

    /* Draw connection status */
    lv_canvas_draw_text(canvas, 2, 28, 64, &label_dsc,
                        state->connected ? LV_SYMBOL_WIFI : LV_SYMBOL_CLOSE);

    /* Rotate canvas for physical display orientation */
    rotate_canvas(canvas, cbuf);
}

/**
 * Battery status
 */
struct battery_status_state {
    uint8_t level;
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
    bool usb_present;
#endif
};

static void set_battery_status(struct zmk_widget_peripheral_screen *widget,
                               struct battery_status_state state) {
    widget->state.battery = state.level;
    draw_top(widget->obj, widget->cbuf, &widget->state);
}

static void battery_status_update_cb(struct battery_status_state state) {
    struct zmk_widget_peripheral_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        set_battery_status(widget, state);
    }
}

static struct battery_status_state battery_status_get_state(const zmk_event_t *eh) {
    return (struct battery_status_state){
        .level = zmk_battery_state_of_charge(),
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
        .usb_present = zmk_usb_is_powered(),
#endif
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_status, struct battery_status_state,
                            battery_status_update_cb, battery_status_get_state);
ZMK_SUBSCRIPTION(widget_battery_status, zmk_battery_state_changed);
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
ZMK_SUBSCRIPTION(widget_battery_status, zmk_usb_conn_state_changed);
#endif

/**
 * Peripheral connection status
 */
struct peripheral_conn_state {
    bool connected;
};

static void set_conn_status(struct zmk_widget_peripheral_screen *widget,
                            struct peripheral_conn_state state) {
    widget->state.connected = state.connected;
    draw_top(widget->obj, widget->cbuf, &widget->state);
}

static void conn_status_update_cb(struct peripheral_conn_state state) {
    struct zmk_widget_peripheral_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        set_conn_status(widget, state);
    }
}

static struct peripheral_conn_state conn_get_state(const zmk_event_t *eh) {
    return (struct peripheral_conn_state){
        .connected = zmk_split_bt_peripheral_is_connected(),
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_conn_status, struct peripheral_conn_state,
                            conn_status_update_cb, conn_get_state);
ZMK_SUBSCRIPTION(widget_conn_status, zmk_split_peripheral_status_changed);

/**
 * Initialization
 */
int zmk_widget_peripheral_screen_init(struct zmk_widget_peripheral_screen *widget,
                                      lv_obj_t *parent) {
    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, SCREEN_HEIGHT, SCREEN_WIDTH); /* 160 x 68 */
    lv_obj_set_style_bg_color(widget->obj, BACKGROUND, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(widget->obj, LV_OPA_COVER, LV_PART_MAIN);

    /* Status canvas (68x68) at top-right — battery & connection info */
    lv_obj_t *top = lv_canvas_create(widget->obj);
    lv_obj_align(top, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_canvas_set_buffer(top, widget->cbuf, CANVAS_SIZE, CANVAS_SIZE,
                         LV_IMG_CF_TRUE_COLOR);

    /* Logo image — fills remaining area (left portion of display) */
    lv_obj_t *art = lv_img_create(widget->obj);
    lv_img_set_src(art, LOGO_IMG);
    lv_obj_align(art, LV_ALIGN_BOTTOM_LEFT, -2, 2);

    sys_slist_append(&widgets, &widget->node);
    widget_battery_status_init();
    widget_conn_status_init();

    return 0;
}

lv_obj_t *zmk_widget_peripheral_screen_obj(struct zmk_widget_peripheral_screen *widget) {
    return widget->obj;
}

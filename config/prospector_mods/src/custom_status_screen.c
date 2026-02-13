/*
 * Modified custom_status_screen.c for Prospector ZMK module.
 *
 * CHANGES:
 *   - Added idle screen: when activity state transitions to IDLE or SLEEP,
 *     the display swaps from the status screen to a logo. Any keypress
 *     (ZMK_ACTIVITY_ACTIVE) swaps back.
 *
 * Original: https://github.com/carrefinho/prospector-zmk-module
 */

#include <lvgl.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>

#include "widgets/layer_roller.h"
#include "widgets/battery_bar.h"
#include "widgets/caps_word_indicator.h"
#include "idle_screen.h"

#include <fonts.h>
#include <sf_symbols.h>

#include <zmk/keymap.h>
#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/activity_state_changed.h>
#include <zmk/activity.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static struct zmk_widget_layer_roller layer_roller_widget;
static struct zmk_widget_battery_bar battery_bar_widget;
static struct zmk_widget_caps_word_indicator caps_word_indicator_widget;

static lv_obj_t *status_screen;
static lv_obj_t *idle_screen_obj;

/* ---------- Activity state listener ---------- */

struct activity_state {
    enum zmk_activity_state state;
};

static void activity_update_cb(struct activity_state state) {
    if (state.state == ZMK_ACTIVITY_IDLE || state.state == ZMK_ACTIVITY_SLEEP) {
        lv_scr_load(idle_screen_obj);
    } else {
        lv_scr_load(status_screen);
    }
}

static struct activity_state activity_get_state(const zmk_event_t *eh) {
    return (struct activity_state){
        .state = zmk_activity_get_state(),
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_activity, struct activity_state, activity_update_cb,
                            activity_get_state);
ZMK_SUBSCRIPTION(widget_activity, zmk_activity_state_changed);

/* ---------- Screen setup ---------- */

lv_obj_t *zmk_display_status_screen() {
    /* --- Status screen (normal operation) --- */
    status_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(status_screen, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(status_screen, 255, LV_PART_MAIN);

#ifdef CONFIG_DT_HAS_ZMK_BEHAVIOR_CAPS_WORD_ENABLED
    zmk_widget_caps_word_indicator_init(&caps_word_indicator_widget, status_screen);
    lv_obj_align(zmk_widget_caps_word_indicator_obj(&caps_word_indicator_widget),
                 LV_ALIGN_RIGHT_MID, -10, 46);
#endif

    zmk_widget_battery_bar_init(&battery_bar_widget, status_screen);
    lv_obj_set_size(zmk_widget_battery_bar_obj(&battery_bar_widget), lv_pct(100), 48);
    lv_obj_align(zmk_widget_battery_bar_obj(&battery_bar_widget), LV_ALIGN_BOTTOM_MID, 0, 0);

    zmk_widget_layer_roller_init(&layer_roller_widget, status_screen);
    lv_obj_set_size(zmk_widget_layer_roller_obj(&layer_roller_widget), 224, 140);
    lv_obj_align(zmk_widget_layer_roller_obj(&layer_roller_widget), LV_ALIGN_LEFT_MID, 0, -20);

    /* --- Idle screen (shows logo when idle) --- */
    idle_screen_obj = create_idle_screen();

    /* Start listening for activity changes */
    widget_activity_init();

    return status_screen;
}

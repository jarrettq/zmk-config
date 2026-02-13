/*
 * Copyright (c) 2023 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <lvgl.h>
#include <zephyr/kernel.h>
#include "util.h"

struct peripheral_state {
    uint8_t battery;
    bool connected;
};

struct zmk_widget_peripheral_screen {
    sys_snode_t node;
    lv_obj_t *obj;
    uint8_t cbuf[CANVAS_BUF_SIZE];
    struct peripheral_state state;
};

int zmk_widget_peripheral_screen_init(struct zmk_widget_peripheral_screen *widget,
                                      lv_obj_t *parent);
lv_obj_t *zmk_widget_peripheral_screen_obj(struct zmk_widget_peripheral_screen *widget);

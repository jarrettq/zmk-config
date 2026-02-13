/*
 * Copyright (c) 2023 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <lvgl.h>

#define SCREEN_WIDTH 68
#define SCREEN_HEIGHT 160

#define CANVAS_SIZE 68

/* Canvas buffer: TRUE_COLOR at LV_COLOR_DEPTH_1 = 1 byte per pixel */
#define CANVAS_BUF_SIZE (CANVAS_SIZE * CANVAS_SIZE)

#define BACKGROUND lv_color_white()
#define FOREGROUND lv_color_black()

void rotate_canvas(lv_obj_t *canvas, lv_color_t *buf);
void init_label_dsc(lv_draw_label_dsc_t *dsc, lv_color_t color,
                    const lv_font_t *font, lv_text_align_t align);

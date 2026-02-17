/*
 * Copyright (c) 2023 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <lvgl.h>

#define SCREEN_WIDTH 68
#define SCREEN_HEIGHT 160

#define CANVAS_SIZE 68

#define BACKGROUND lv_color_white()
#define FOREGROUND lv_color_black()

void rotate_canvas(lv_obj_t *canvas, lv_color_t cbuf[]);
void init_label_dsc(lv_draw_label_dsc_t *dsc, lv_color_t color,
                    const lv_font_t *font, lv_text_align_t align);
void init_rect_dsc(lv_draw_rect_dsc_t *rect_dsc, lv_color_t bg_color);

/*
 * Copyright (c) 2023 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 *
 * Utility functions for nice_view_custom display rendering.
 * Provides canvas helpers and rotation for the Sharp Memory LCD.
 */

#include "util.h"
#include <string.h>

static uint8_t cbuf_tmp[CANVAS_SIZE * CANVAS_SIZE];

void rotate_canvas(lv_obj_t *canvas, lv_color_t *buf) {
    memcpy(cbuf_tmp, buf, sizeof(cbuf_tmp));
    lv_canvas_fill_bg(canvas, BACKGROUND, LV_OPA_COVER);

    /* 270° rotation of a CANVAS_SIZE x CANVAS_SIZE buffer.
     * Maps (x, y) → (y, CANVAS_SIZE - 1 - x)
     * This converts portrait-drawn content to landscape for the
     * physically portrait-mounted Sharp LCD. */
    for (int y = 0; y < CANVAS_SIZE; y++) {
        for (int x = 0; x < CANVAS_SIZE; x++) {
            ((uint8_t *)buf)[(CANVAS_SIZE - 1 - x) * CANVAS_SIZE + y] =
                cbuf_tmp[y * CANVAS_SIZE + x];
        }
    }
}

void init_label_dsc(lv_draw_label_dsc_t *dsc, lv_color_t color,
                    const lv_font_t *font, lv_text_align_t align) {
    lv_draw_label_dsc_init(dsc);
    dsc->color = color;
    dsc->font = font;
    dsc->align = align;
}

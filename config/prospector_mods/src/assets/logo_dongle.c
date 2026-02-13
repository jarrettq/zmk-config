/*
 * Placeholder logo for the Prospector dongle idle screen.
 *
 * ===== IMAGE FORMAT =====
 * Format: LV_IMG_CF_INDEXED_1BIT (1-bit indexed, 2-color palette)
 * Size:   60 × 60 pixels
 * Palette: Index 0 = black (background), Index 1 = white (foreground)
 *
 * ===== DISPLAY CONTEXT =====
 * The Prospector's ST7789V TFT is 240×280, rotated 270° at the driver level.
 * After rotation LVGL sees it as 280×240 (landscape). This image is centered
 * on that display via lv_obj_center().
 *
 * ===== REPLACING WITH YOUR LOGO =====
 * Option A — Monochrome (small file):
 *   1. Design your logo for the 280×240 display
 *   2. Use LVGL's image converter: CF_INDEXED_1_BIT, C array output
 *   3. Replace logo_dongle_map[] and update the lv_img_dsc_t dimensions
 *
 * Option B — Full color RGB565 (richer visuals):
 *   1. Design your logo (up to 280×240)
 *   2. Use LVGL's image converter: CF_TRUE_COLOR (RGB565), C array output
 *   3. Replace the entire file with the converter output
 *   4. The lv_img_dsc_t should use .header.cf = LV_IMG_CF_TRUE_COLOR
 *      and .data_size = width * height * 2
 *
 * ===== PIXEL ENCODING (1-bit indexed) =====
 * Each byte = 8 horizontal pixels (MSB first)
 * 0 = palette index 0 (black), 1 = palette index 1 (white)
 * 8 bytes per row for 60px width (last 4 bits are padding)
 */

#include <lvgl.h>

/* Image data: palette (8 bytes) + pixel data (480 bytes) = 488 bytes */
static const uint8_t logo_dongle_map[488] = {
    /* Palette (BGRA format, 4 bytes each) */
    0x00, 0x00, 0x00, 0xff, /* Index 0: black (background) */
    0xff, 0xff, 0xff, 0xff, /* Index 1: white (foreground) */
    /* Pixel data: 8 bytes per row × 60 rows = 480 bytes */
    /* All zero = blank (all black). Replace with your logo data. */
    /* ---- REPLACE FROM HERE WITH YOUR LOGO DATA ---- */
};

const lv_img_dsc_t logo_dongle = {
    .header.always_zero = 0,
    .header.w = 60,
    .header.h = 60,
    .header.cf = LV_IMG_CF_INDEXED_1BIT,
    .data_size = sizeof(logo_dongle_map),
    .data = logo_dongle_map,
};

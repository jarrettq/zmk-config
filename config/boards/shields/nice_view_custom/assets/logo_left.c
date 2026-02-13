/*
 * Placeholder logo for the LEFT half of the Corne split keyboard.
 *
 * ===== IMAGE FORMAT =====
 * Format: LV_IMG_CF_INDEXED_1BIT (1-bit indexed, 2-color palette)
 * Size:   140 × 68 pixels
 * Palette: 2 entries × 4 bytes (BGRA) = 8 bytes
 * Pixels:  ceil(140/8) × 68 = 18 × 68 = 1224 bytes (MSB-first)
 * Total:   8 + 1224 = 1232 bytes
 *
 * ===== ORIENTATION =====
 * The nice!view Sharp LCD is physically in portrait (68 wide × 160 tall)
 * but LVGL drives it as landscape (160 wide × 68 tall).
 * This image occupies the left 140×68 region of the LVGL framebuffer,
 * which maps to the LOWER 68×140 region on the physical display.
 *
 * To get your logo to display correctly:
 *   1. Design your logo at 68×140 px (as it should appear on the display)
 *   2. Rotate it 90° CLOCKWISE
 *   3. Export as 140×68 1-bit indexed BMP/PNG
 *   4. Convert to C array using LVGL's image converter or your pipeline
 *   5. Replace logo_left_map[] pixel data below (starting at offset 8)
 *
 * ===== PIXEL ENCODING =====
 * Each byte encodes 8 horizontal pixels, MSB first:
 *   bit 7 = leftmost pixel, bit 0 = rightmost
 *   0 = palette index 0 (background/white)
 *   1 = palette index 1 (foreground/black)
 * 18 bytes per row (last byte's low 4 bits are padding for 140px width)
 */

#include <lvgl.h>

/* Image data: palette (8 bytes) + pixel data (1224 bytes) */
static const uint8_t logo_left_map[1232] = {
    /* Palette */
    0xff, 0xff, 0xff, 0xff,  /* Index 0: white (background) */
    0x00, 0x00, 0x00, 0xff,  /* Index 1: black (foreground) */
    /* Pixel data: 1224 bytes, all zero = blank white placeholder */
    /* ---- REPLACE FROM HERE WITH YOUR LOGO DATA ---- */
};

const lv_img_dsc_t logo_left = {
    .header.always_zero = 0,
    .header.w = 140,
    .header.h = 68,
    .header.cf = LV_IMG_CF_INDEXED_1BIT,
    .data_size = sizeof(logo_left_map),
    .data = logo_left_map,
};

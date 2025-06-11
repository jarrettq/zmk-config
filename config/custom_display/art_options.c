/*
 * Custom Display Options for Corne Right Half
 * 
 * Instructions:
 * 1. OPTION A: Use the Binary Image (current setup)
 *    - The custom_image.c contains your binary image data
 *    - The image will be displayed automatically
 *    - Image size: 180x64 pixels, 1-bit indexed
 * 
 * 2. OPTION B: Use ASCII Art instead
 *    - Replace the image code in custom_status_screen.c
 *    - Copy one of the art arrays below
 *    - Follow the instructions at the bottom
 */

// Option 1: Cat Face
static const char *cat_art[] = {
    "      /\\_/\\    ",
    "     ( ^.^ )   ",
    "      > ^ <    ",
    "   Right Half  ",
    NULL
};

// Option 2: Simple Corne Logo
static const char *corne_art[] = {
    "   ╭─────────╮  ",
    "   │  CORNE  │  ",
    "   │    KB   │  ",
    "   ╰─────────╯  ",
    NULL
};

// Option 3: Keyboard ASCII
static const char *keyboard_art[] = {
    " ┌─┬─┬─┬─┬─┐   ",
    " │Q│W│E│R│T│   ",
    " ├─┼─┼─┼─┼─┤   ",
    " │A│S│D│F│G│   ",
    " └─┴─┴─┴─┴─┘   ",
    NULL
};

// Option 4: Heart
static const char *heart_art[] = {
    "   ♥ ZMK ♥     ",
    "              ",
    "   Right      ",
    "   Half       ",
    NULL
};

// Option 5: Minimalist
static const char *minimal_art[] = {
    "              ",
    "   [ R ]      ",
    "              ",
    "              ",
    NULL
};

// Option 6: Gaming Theme
static const char *gaming_art[] = {
    "    ▲ ▲ ▲     ",
    "   ◄ ● ►     ",
    "    ▼ ▼ ▼     ",
    "   GAME ON    ",
    NULL
};

// Option 7: Tech Theme
static const char *tech_art[] = {
    "  ┌─[●]─┐     ",
    "  │ 0x42 │     ",
    "  └─────┘     ",
    "   TECH       ",
    NULL
};

// Option 8: Coffee Theme
static const char *coffee_art[] = {
    "      ___     ",
    "     |___|    ",
    "      ___     ",
    "   COFFEE?    ",
    NULL
};

/*
 * TO SWITCH FROM IMAGE TO ASCII ART:
 * 
 * 1. Open custom_status_screen.c
 * 2. Remove the line: #include "custom_image.h"
 * 3. Replace the image display code with ASCII art code
 * 4. Add your chosen art array from above
 * 5. Update CMakeLists.txt to remove custom_image.c if desired
 * 
 * TO USE ASCII ART:
 * 1. Copy one of the art arrays above
 * 2. Replace the image display section in custom_status_screen.c
 * 3. Each line should be exactly 14 characters for best display
 * 4. Use 4 lines maximum for the nice!view screen size
 * 
 * Example ASCII Art Display Code:
 * ```c
 * // Add chosen art array here
 * static const char *custom_art[] = {
 *     "   ╭─────────╮  ",
 *     "   │  CORNE  │  ",
 *     "   │    KB   │  ",
 *     "   ╰─────────╯  ",
 *     NULL
 * };
 * 
 * // Replace image code with:
 * lv_obj_t *art_label = lv_label_create(img_container);
 * lv_obj_set_style_text_color(art_label, lv_color_white(), LV_PART_MAIN);
 * char art_text[256] = "";
 * for (int i = 0; custom_art[i] != NULL; i++) {
 *     strcat(art_text, custom_art[i]);
 *     if (custom_art[i + 1] != NULL) {
 *         strcat(art_text, "\n");
 *     }
 * }
 * lv_label_set_text(art_label, art_text);
 * lv_obj_center(art_label);
 * ```
 */

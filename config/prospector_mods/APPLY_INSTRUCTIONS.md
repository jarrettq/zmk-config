# Prospector Module Modifications

These files modify the [Prospector ZMK module](https://github.com/carrefinho/prospector-zmk-module)
to add:

1. **Dark yellow active layer text** — changes the selected layer color from white
   (`0xffffff`) to dark yellow (`0xCCA000`) in the layer roller widget.
2. **Idle screen with dongle logo** — when the keyboard goes idle, the dongle display
   swaps from the status screen to a logo image. Pressing any key swaps back.

---

## How to Apply

### Step 1: Fork the Prospector Module

1. Go to <https://github.com/carrefinho/prospector-zmk-module>
2. Click **Fork** to create your own copy (e.g. `your-username/prospector-zmk-module`)
3. Clone your fork locally

### Step 2: Replace/Add Files

Copy the files from this directory into your fork's
`boards/shields/prospector_adapter/` directory:

```
# REPLACE existing files:
src/custom_status_screen.c    →  boards/shields/prospector_adapter/src/custom_status_screen.c
src/widgets/layer_roller.c    →  boards/shields/prospector_adapter/src/widgets/layer_roller.c

# ADD new files:
src/idle_screen.c             →  boards/shields/prospector_adapter/src/idle_screen.c
src/idle_screen.h             →  boards/shields/prospector_adapter/src/idle_screen.h
src/assets/logo_dongle.c      →  boards/shields/prospector_adapter/src/assets/logo_dongle.c
```

### Step 3: Update CMakeLists.txt

In your fork, edit `boards/shields/prospector_adapter/CMakeLists.txt`.
Add these two lines (after the existing `zephyr_library_sources` lines):

```cmake
  zephyr_library_sources(src/idle_screen.c)
  zephyr_library_sources(src/assets/logo_dongle.c)
```

### Step 4: Update west.yml

In your `config/west.yml`, change the Prospector remote and project to point at your fork:

```yaml
remotes:
  - name: my-prospector              # your fork
    url-base: https://github.com/YOUR_USERNAME
projects:
  - name: prospector-zmk-module
    remote: my-prospector             # point to your fork
    revision: main
```

### Step 5: Commit & Push

Commit all changes to your fork and push. Your GitHub Actions build will now
use the modified Prospector module.

---

## What Changed (Summary)

| File | Change |
|------|--------|
| `layer_roller.c` | Line ~145: `lv_color_hex(0xffffff)` → `lv_color_hex(0xCCA000)` for `LV_PART_SELECTED` |
| `custom_status_screen.c` | Added `idle_screen.h` include, activity state listener, `lv_scr_load()` to swap between status and idle screens |
| `idle_screen.c/h` | New: creates an LVGL screen with a centered logo image |
| `logo_dongle.c` | New: placeholder 60×60 monochrome logo (replace pixel data with your own) |

---

## Dongle Logo Image

The placeholder in `logo_dongle.c` is a blank 60×60 1-bit indexed image.
To add your actual logo:

1. Design your logo for the Prospector's **280×240** display (landscape after 270° rotation)
2. Export as a C array using [LVGL's image converter](https://lvgl.io/tools/imageconverter)
   - For monochrome: use `CF_INDEXED_1_BIT`, output format C array
   - For color: use `CF_TRUE_COLOR` (RGB565), output format C array
3. Replace the contents of `logo_dongle.c` with the converter output
4. Adjust the image dimensions in the `lv_img_dsc_t` header

The idle timeout defaults to 30 seconds (`CONFIG_ZMK_IDLE_TIMEOUT=30000`).
You can adjust this in your `corne_dongle.conf` if desired.

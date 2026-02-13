# Display Customization — Implementation Guide

## Overview

This config implements four display customizations for the Corne split keyboard
with dongle setup:

| # | Feature | Target Hardware | Implementation |
|---|---------|-----------------|----------------|
| A | Per-half custom logos | nice!view (Sharp LCD, 160×68, mono) | `nice_view_custom` local shield |
| B | Battery + connection status | nice!view | `nice_view_custom` local shield |
| C | Dark yellow active layer text | Prospector dongle (ST7789V TFT, 280×240, color) | Fork of Prospector module |
| D | Idle screen with dongle logo | Prospector dongle | Fork of Prospector module |

---

## What's Already Done (Local Config)

### A+B: nice_view_custom Shield

A self-contained replacement for the stock `nice_view` shield. Located at:

```
config/boards/shields/nice_view_custom/
├── Kconfig.shield            # Shield definition
├── Kconfig.defconfig          # LVGL/display config defaults
├── nice_view_custom.overlay   # Sharp LCD device tree
├── CMakeLists.txt             # Build — conditional left/right logo
├── custom_status_screen.c     # Entry point (zmk_display_status_screen)
├── widgets/
│   ├── peripheral_screen.c    # Main widget: battery, connection, logo
│   ├── peripheral_screen.h    # Widget struct definition
│   ├── util.c                 # Canvas rotation (270°) helper
│   └── util.h                 # Display constants, macros
└── assets/
    ├── logo_left.c            # ← PLACEHOLDER — replace pixel data
    └── logo_right.c           # ← PLACEHOLDER — replace pixel data
```

**build.yaml** has been updated: `nice_view` → `nice_view_custom` for both halves.

### Screen Layout (nice!view)

The nice!view is physically portrait (68 wide × 160 tall). LVGL drives it
as landscape (160×68). Content is drawn to 68×68 canvases, rotated 270°,
then displayed.

```
Physical display (portrait):        LVGL framebuffer (landscape):
┌────────┐  68px                    ┌─────────────────────────────────┐
│        │                          │ logo (140×68)   │ status (68×68)│ 68px
│  logo  │  140px                   │                 │  battery %    │
│        │                          │                 │  conn status  │
├────────┤                          └─────────────────────────────────┘
│ status │  68px                                    160px
│ bat %  │
│ conn   │
└────────┘
```

### Adding Your Logos

Each logo file is at `config/boards/shields/nice_view_custom/assets/`:

- [logo_left.c](config/boards/shields/nice_view_custom/assets/logo_left.c) — left half
- [logo_right.c](config/boards/shields/nice_view_custom/assets/logo_right.c) — right half

**Image specs:**
- Format: `LV_IMG_CF_INDEXED_1BIT` (1-bit monochrome)
- Dimensions: 140 × 68 pixels (in LVGL coordinates)
- The image data is **pre-rotated** — design at 68×140 physical, rotate 90° CW before export
- Use [LVGL's image converter](https://lvgl.io/tools/imageconverter): select `CF_INDEXED_1_BIT`, output C array
- Replace `logo_left_map[]` / `logo_right_map[]` pixel data (starts at byte offset 8, after the 8-byte palette)

---

## What Requires a Fork (Prospector Module)

### C+D: Prospector Module Modifications

The modified source files are in `config/prospector_mods/`. See
[APPLY_INSTRUCTIONS.md](config/prospector_mods/APPLY_INSTRUCTIONS.md) for
step-by-step fork instructions.

**Summary of changes:**

| File | What Changed |
|------|-------------|
| `src/widgets/layer_roller.c` | Line ~145: selected text color `0xffffff` → `0xCCA000` (dark yellow) |
| `src/custom_status_screen.c` | Added activity state listener + idle screen swap via `lv_scr_load()` |
| `src/idle_screen.c` + `.h` | New: creates LVGL screen with centered dongle logo |
| `src/assets/logo_dongle.c` | New: placeholder 60×60 monochrome logo |

**After forking**, update `config/west.yml`:

```yaml
remotes:
  - name: my-prospector                   # ← your fork
    url-base: https://github.com/YOUR_USERNAME
projects:
  - name: prospector-zmk-module
    remote: my-prospector                  # ← point here
    revision: main
```

### Dongle Logo

The placeholder is at `config/prospector_mods/src/assets/logo_dongle.c`.

**Image specs:**
- Display: 280×240 pixels (landscape, after 270° driver rotation)
- Format: `LV_IMG_CF_INDEXED_1BIT` (monochrome) or `LV_IMG_CF_TRUE_COLOR` (RGB565 color)
- For color logos, use LVGL converter with `CF_TRUE_COLOR` — the `.data_size` = width × height × 2
- The idle screen appears after 30s of inactivity (configurable via `CONFIG_ZMK_IDLE_TIMEOUT` in `corne_dongle.conf`)

---

## Build Checklist

- [x] `build.yaml` updated (`nice_view` → `nice_view_custom`)
- [x] `corne_dongle.conf` has `CONFIG_ZMK_IDLE_TIMEOUT=30000`
- [ ] Replace placeholder logo data in `logo_left.c` and `logo_right.c`
- [ ] Fork Prospector module and apply files from `config/prospector_mods/`
- [ ] Update `config/west.yml` to point to your Prospector fork
- [ ] Replace placeholder `logo_dongle.c` with your dongle logo
- [ ] Push and build via GitHub Actions

---

## Technical Notes

- **LVGL version:** v8 (ZMK v0.3 uses Zephyr 3.5)
- **Canvas rotation:** 270° manual pixel remapping in `util.c` — each 68×68 tile is
  drawn in portrait, rotated, then placed in the landscape framebuffer
- **Per-half selection:** `CONFIG_SHIELD_CORNE_LEFT` / `CONFIG_SHIELD_CORNE_RIGHT`
  Kconfig symbols control which logo file is compiled (set in `CMakeLists.txt`)
- **Idle screen:** Uses `zmk_activity_state_changed` event; `lv_scr_load()` swaps
  between status and idle LVGL screens. Widget listeners continue updating in
  the background.

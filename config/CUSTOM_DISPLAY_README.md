# Custom Corne Display Setup

This setup completely replaces the default ZMK status screen with custom displays for both halves of your Corne keyboard.

## Current Setup: Custom Display for Both Halves

**Right Half:** Your custom 180x64 pixel binary image with "R" indicator  
**Left Half:** Simple "CORNE KEYBOARD" text display with "Left Half" indicator

## Files Created

- `custom_display/custom_status_screen.c` - Main display logic with half detection
- `custom_display/custom_status_screen.h` - Header file  
- `custom_display/custom_image.c` - Binary image data (180x64, 1-bit indexed)
- `custom_display/custom_image.h` - Image header
- `custom_display/art_options.c` - ASCII art alternatives and instructions
- `CMakeLists.txt` - Build configuration

## Display Layout

**Right Half:**
- **Main Area**: Your custom 180x64 pixel image
- **Bottom Right**: Small "R" indicator

**Left Half:**
- **Center**: "CORNE KEYBOARD" text
- **Bottom**: "Left Half" indicator

## Build and Flash

1. **Commit & Push** your changes to trigger GitHub Actions build
2. **Download** both firmware files:
   - `corne_left-nice_nano_v2-zmk.uf2` (for left half)
   - `corne_right-nice_nano_v2-zmk.uf2` (for right half)  
3. **Flash** each firmware to the corresponding half
4. **Enjoy** your custom displays!

## Key Changes Made

- ✅ **Disabled default widgets** to prevent conflicts
- ✅ **Half detection** shows different content on each side
- ✅ **Clean custom display** without stock widget interference
- ✅ **No symbol conflicts** with simplified approach

## Switching to ASCII Art

If you prefer ASCII art instead, see instructions in `custom_display/art_options.c`

## Creating Your Own Images

To replace the current image with your own:

1. **Create** a 180x64 pixel image (black and white)
2. **Convert** to 1-bit indexed LVGL format using online converters
3. **Replace** the data in `custom_image.c`
4. **Update** the image descriptor if needed

## Troubleshooting

- **Image not showing**: Check that CONFIG_ZMK_DISPLAY_STATUS_SCREEN_CUSTOM=y in corne.conf
- **Build errors**: Ensure all files are properly included in CMakeLists.txt  
- **Symbol conflicts**: This version avoids widget conflicts by using a simplified approach

## Technical Details

- **Image Format**: 1-bit indexed (black/white)
- **Display Size**: 128x64 pixels (image scales automatically)
- **File Size**: ~1.5KB for the image data
- **Memory**: Uses LVGL image widget for efficient display
- **Compatibility**: Works with nice!view displays without conflicting with built-in widgets

The setup has been simplified to avoid symbol conflicts with nice!view's built-in status widgets while still providing your custom image display.

Enjoy your personalized Corne keyboard display! 🎯

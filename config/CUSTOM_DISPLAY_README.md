# Custom Corne Display Setup

This setup displays a **custom 180x64 pixel binary image** on the right half of your Corne keyboard's nice!view display.

## Current Setup: Binary Image Display

Your right half will show your custom image with a simple "R" indicator in the bottom-right corner to identify it as the right half.

## Files Created

- `custom_display/custom_status_screen.c` - Main display logic
- `custom_display/custom_status_screen.h` - Header file  
- `custom_display/custom_image.c` - Binary image data (180x64, 1-bit indexed)
- `custom_display/custom_image.h` - Image header
- `custom_display/art_options.c` - ASCII art alternatives and instructions
- `CMakeLists.txt` - Build configuration

## Display Layout

- **Main Area**: Your custom 180x64 pixel image
- **Bottom Right**: Simple "R" indicator for right half
- **Left Half**: Default ZMK status screen with battery/layer/connection info

## Build and Flash

1. **Commit & Push** your changes to trigger GitHub Actions build
2. **Download** the new `corne_right-nice_nano_v2-zmk.uf2` firmware
3. **Flash** it to your right half
4. **Enjoy** your custom image display!

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

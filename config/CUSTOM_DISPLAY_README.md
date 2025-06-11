# Custom Corne Display Setup

This setup allows you to display custom art on the right half of your Corne keyboard's nice!view display. You can choose between a **binary image** or **ASCII art**.

## Current Setup: Binary Image Display

Your setup is currently configured to display a **180x64 pixel binary image** on the right half.

## Files Created

- `custom_display/custom_status_screen.c` - Main display logic
- `custom_display/custom_status_screen.h` - Header file  
- `custom_display/custom_image.c` - Binary image data (180x64, 1-bit indexed)
- `custom_display/custom_image.h` - Image header
- `custom_display/art_options.c` - ASCII art alternatives and instructions
- `CMakeLists.txt` - Build configuration

## Display Options

### Option A: Binary Image (Current)
- Shows your custom 180x64 pixel image
- Full display resolution usage
- Professional appearance
- Status widgets at bottom

### Option B: ASCII Art Alternative  
- Text-based art using monospace characters
- 4 lines maximum, 14 characters per line
- Multiple pre-made designs available
- Easy to customize

## How to Use Your Image

The binary image from your file is already integrated and will display automatically when you build and flash the firmware.

## How to Switch to ASCII Art

If you prefer ASCII art instead:

1. **Open** `custom_display/art_options.c`
2. **Choose** one of the 8 pre-made designs
3. **Follow** the detailed switching instructions in that file
4. **Modify** `custom_status_screen.c` as instructed

## Available ASCII Art Options

- Cat Face
- Simple Corne Logo  
- Keyboard ASCII
- Heart Theme
- Minimalist
- Gaming Theme
- Tech Theme
- Coffee Theme

## Build and Flash

After the setup is complete:

1. **Commit** your changes to your zmk-config repository
2. **Wait** for GitHub Actions to build your firmware
3. **Download** the new firmware files
4. **Flash** the `corne_right-nice_nano_v2-zmk.uf2` file to your right half
5. **Enjoy** your custom display!

## Display Layout

The display shows:
- **Main Area**: Your custom binary image (or ASCII art if switched)
- **Bottom**: Status widgets (layer, battery, connection status)
- **Left Half**: Continues showing the default ZMK status screen

## Creating Your Own Images

To replace the current image with your own:

1. **Create** a 180x64 pixel image (black and white)
2. **Convert** to 1-bit indexed LVGL format using online converters
3. **Replace** the data in `custom_image.c`
4. **Update** the image descriptor if needed

## Troubleshooting

- **Image not showing**: Check that CONFIG_ZMK_DISPLAY_STATUS_SCREEN_CUSTOM=y in corne.conf
- **Build errors**: Ensure all files are properly included in CMakeLists.txt  
- **Wrong size**: Verify image dimensions are exactly 180x64 pixels

## Technical Details

- **Image Format**: 1-bit indexed (black/white)
- **Display Size**: 128x64 pixels (image scales automatically)
- **File Size**: ~1.5KB for the image data
- **Memory**: Uses LVGL image widget for efficient display

Enjoy your personalized Corne keyboard display!

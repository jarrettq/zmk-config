# Instructions for Building and Flashing the Custom Display Firmware

## What we've created:

1. **Custom Status Screen (status_screen.c)**
   - Left display: Shows a smaller output status with the connection number
   - Right display: Shows your custom company logo

2. **Company Logo (images/company_logo.c)**
   - A simple logo that will be displayed on the right half

3. **Configuration Files**
   - Modified corne.conf to enable custom widgets
   - Created proper directory structure for ZMK to find our custom files

## Building Your Firmware

To build your custom firmware:

1. Make sure you have the ZMK development environment set up
2. Run the following command in your zmk-config directory:

```bash
west build -p
```

Or if you're using GitHub Actions for building:

```bash
git add .
git commit -m "Added custom nice!view display configuration"
git push
```

Then download the firmware files from the GitHub Actions workflow after it completes.

## Flashing Your Firmware

1. Connect your nice!nano to your computer while holding the RESET button to put it in bootloader mode
2. The board should appear as a USB drive
3. Copy the appropriate UF2 file for each half:
   - `corne_left-nice_nano_v2-zmk.uf2` for the left half
   - `corne_right-nice_nano_v2-zmk.uf2` for the right half

4. The device will automatically reset and disconnect after flashing

## Customizing Further

If you want to change the logo:
1. Convert your logo to a 1-bit monochrome image
2. Use LVGL's image converter tool to convert it to a C array
3. Replace the contents of `company_logo_map` in `company_logo.c`

For more advanced customization of the output status widget:
1. Study the ZMK source code for `zmk/display/widgets/output_status.c`
2. Create a more detailed custom version based on the ZMK implementation

## Troubleshooting

If your display doesn't show the customizations:
1. Check that all files are in the right location
2. Make sure CONFIG_ZMK_DISPLAY_CUSTOM_WIDGET=y is in your corne.conf
3. Try rebuilding with a clean build directory: `west build -p -b nice_nano_v2 -- -DSHIELD="corne_left nice_view_adapter nice_view" -DZMK_CONFIG="/Users/tkl-jarrettq/zmk-config/config"`

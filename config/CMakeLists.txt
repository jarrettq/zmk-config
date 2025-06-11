# Add custom display source files
if(CONFIG_ZMK_DISPLAY_STATUS_SCREEN_CUSTOM)
    target_sources(app PRIVATE 
        custom_display/custom_status_screen.c
        custom_display/custom_image.c
    )
endif()

if(CONFIG_ZMK_DISPLAY AND CONFIG_NICE_VIEW_CUSTOM_WIDGET)
  zephyr_library_include_directories(${CMAKE_SOURCE_DIR}/include)
  zephyr_library_sources(custom_status_screen.c)
  zephyr_library_sources(widgets/util.c)
  zephyr_library_sources(widgets/peripheral_screen.c)

  # Select logo based on which corne half is being built
  if(CONFIG_SHIELD_CORNE_LEFT)
    zephyr_library_sources(assets/logo_left.c)
  elseif(CONFIG_SHIELD_CORNE_RIGHT)
    zephyr_library_sources(assets/logo_right.c)
  else()
    # Fallback: compile left logo
    zephyr_library_sources(assets/logo_left.c)
  endif()
endif()

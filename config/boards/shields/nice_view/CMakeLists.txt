# Copyright (c) 2023 The ZMK Contributors
# SPDX-License-Identifier: MIT

if(CONFIG_ZMK_DISPLAY)
  zephyr_library()
  
  # Include our custom status screen if defined
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/status_screen.c)
    zephyr_library_sources(status_screen.c)
  endif()
  
  # Include company logo image if available
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/images/company_logo.c)
    zephyr_library_sources(images/company_logo.c)
  endif()
endif()

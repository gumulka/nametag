# SPDX-License-Identifier: Apache-2.0

if(NOT CONFIG_MCUBOOT)
zephyr_library()
zephyr_library_sources(board.c)
endif()

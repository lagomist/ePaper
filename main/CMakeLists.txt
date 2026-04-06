list(APPEND inc_list
    ${COMPONENT_DIR}/driver/include
    ${COMPONENT_DIR}/bsp/include
    ${COMPONENT_DIR}/monitor/include
    ${COMPONENT_DIR}/ui/include
)

list (APPEND src_list
    ${COMPONENT_DIR}/driver/epaper.cpp
    ${COMPONENT_DIR}/driver/shtc3.cpp
    ${COMPONENT_DIR}/driver/multi_button.cpp
    ${COMPONENT_DIR}/driver/lvgl_display.cpp
    ${COMPONENT_DIR}/driver/board_codec.cpp
    ${COMPONENT_DIR}/driver/vbat_adc.cpp
    ${COMPONENT_DIR}/bsp/board_power.cpp
    ${COMPONENT_DIR}/bsp/board_audio.cpp
    ${COMPONENT_DIR}/bsp/board_rtc.cpp
    ${COMPONENT_DIR}/bsp/bsp.cpp
    ${COMPONENT_DIR}/monitor/led_heartbeat.cpp
    ${COMPONENT_DIR}/monitor/button_handler.cpp
    ${COMPONENT_DIR}/monitor/time_sync.cpp
    ${COMPONENT_DIR}/monitor/music_player.cpp
    ${COMPONENT_DIR}/ui/gui.cpp
    ${COMPONENT_DIR}/ui/dashboard_model.cpp
    ${COMPONENT_DIR}/ui/dashboard_page.cpp
    ${COMPONENT_DIR}/main.cpp
)

idf_component_register(
    SRCS
    ${src_list}
    INCLUDE_DIRS
    ${inc_list}
    )

fatfs_create_spiflash_image(storage ${CMAKE_CURRENT_SOURCE_DIR}/storage FLASH_IN_PROJECT)


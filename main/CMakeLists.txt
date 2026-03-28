list(APPEND inc_list
    ${COMPONENT_DIR}/driver/include
    ${COMPONENT_DIR}/bsp/include
    ${COMPONENT_DIR}/monitor/include
)

list (APPEND src_list
    ${COMPONENT_DIR}/driver/epaper.cpp
    ${COMPONENT_DIR}/driver/shtc3.cpp
    ${COMPONENT_DIR}/driver/multi_button.cpp
    ${COMPONENT_DIR}/bsp/board_power.cpp
    ${COMPONENT_DIR}/bsp/bsp.cpp
    ${COMPONENT_DIR}/monitor/button_monitor.cpp
    ${COMPONENT_DIR}/main.cpp
)

idf_component_register(
    SRCS
    ${src_list}
    INCLUDE_DIRS
    ${inc_list}
    )

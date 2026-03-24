list(APPEND inc_list
)

list (APPEND src_list
    ${COMPONENT_DIR}/main.cpp
)

idf_component_register(
    SRCS
    ${src_list}
    INCLUDE_DIRS
    ${inc_list}
    )

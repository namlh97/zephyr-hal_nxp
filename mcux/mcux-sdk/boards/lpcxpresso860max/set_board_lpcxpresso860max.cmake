include_guard(GLOBAL)


if (CONFIG_USE_BOARD_Project_Template_lpcxpresso860max)
# Add set(CONFIG_USE_BOARD_Project_Template_lpcxpresso860max true) in config.cmake to use this component

message("BOARD_Project_Template_lpcxpresso860max component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if((CONFIG_BOARD STREQUAL lpcxpresso860max) AND CONFIG_USE_component_miniusart_adapter AND (CONFIG_DEVICE_ID STREQUAL LPC865) AND CONFIG_USE_device_LPC865_startup AND CONFIG_USE_driver_clock AND CONFIG_USE_driver_lpc_gpio AND CONFIG_USE_driver_lpc_iocon_lite AND CONFIG_USE_driver_lpc_miniusart AND CONFIG_USE_driver_power_no_lib AND CONFIG_USE_driver_swm AND CONFIG_USE_utility_assert_lite AND CONFIG_USE_utility_debug_console_lite)

add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/board.h "" BOARD_Project_Template_lpcxpresso860max)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/board.c "" BOARD_Project_Template_lpcxpresso860max)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/clock_config.h ${CMAKE_CURRENT_LIST_DIR}/project_template/. BOARD_Project_Template_lpcxpresso860max)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/clock_config.c "" BOARD_Project_Template_lpcxpresso860max)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/pin_mux.h ${CMAKE_CURRENT_LIST_DIR}/project_template/. BOARD_Project_Template_lpcxpresso860max)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/pin_mux.c "" BOARD_Project_Template_lpcxpresso860max)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/peripherals.h ${CMAKE_CURRENT_LIST_DIR}/project_template/. BOARD_Project_Template_lpcxpresso860max)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/peripherals.c "" BOARD_Project_Template_lpcxpresso860max)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/project_template/.
)

else()

message(SEND_ERROR "BOARD_Project_Template_lpcxpresso860max dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()

project(main C CXX)

set(GUI_PATH "${CMAKE_CURRENT_SOURCE_DIR}/gui")
set(FRAMES_PATH "${GUI_PATH}/frames")
set(WINDOWS_PATH "${GUI_PATH}/windows")
set(HARDWARE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/hardware")
set(MAIN_PATH "${CMAKE_CURRENT_SOURCE_DIR}/main")
set(LWIP_CONF_PATH "${CMAKE_CURRENT_SOURCES_DIR}/lwip_conf")
set(TESTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../tests")


set(FRAMES_SOURCES
	"${FRAMES_PATH}/frame.cpp"
	"${FRAMES_PATH}/clk_frame.cpp"
	"${FRAMES_PATH}/interval_frame.cpp"
	"${FRAMES_PATH}/main_frame.cpp"
	"${FRAMES_PATH}/overview_interval_frame.cpp"
	"${FRAMES_PATH}/set_interval_frame.cpp"
	"${FRAMES_PATH}/connect_frame.cpp"
)

set(WINDOWS_SOURCES
	"${WINDOWS_PATH}/button.cpp"
	"${WINDOWS_PATH}/control_window.cpp"
	"${WINDOWS_PATH}/static_measure_temp_window.cpp"
	"${WINDOWS_PATH}/static_preset_temp_window.cpp"
	"${WINDOWS_PATH}/static_temp_window.cpp"
	"${WINDOWS_PATH}/static_time_window.cpp"
	"${WINDOWS_PATH}/static_window.cpp"
	"${WINDOWS_PATH}/temp_window.cpp"
	"${WINDOWS_PATH}/time_window.cpp"
	"${WINDOWS_PATH}/window.cpp"
)

set(GUI_SOURCES
	${FRAMES_SOURCES}
	${WINDOWS_SOURCES}
	"${GUI_PATH}/window_system.cpp"
)

set(HARDWARE_SOURCES
	"${HARDWARE_PATH}/eeprom.cpp"
	"${HARDWARE_PATH}/io.cpp"
	"${HARDWARE_PATH}/lcd.cpp"
	"${HARDWARE_PATH}/one_wire.cpp"
	"${HARDWARE_PATH}/relay.cpp"
	"${HARDWARE_PATH}/syscalls.c"
)

set(MAIN_SOURCES
	"${MAIN_PATH}/interval_frame_data.cpp"
	"${MAIN_PATH}/main.cpp"
	"${MAIN_PATH}/application.cpp"
	"${MAIN_PATH}/rtc_controller.cpp"
	"${MAIN_PATH}/temp_controller.cpp"
	"${MAIN_PATH}/temp_sensor.cpp"
	"${MAIN_PATH}/time.cpp"
	"${MAIN_PATH}/timer_events.cpp"
	"${MAIN_PATH}/intervals_changed_event.cpp"
	"${MAIN_PATH}/des_key.cpp"
)

# TODO: TEST_SOURCES - these sources do not have to be in release binary
set(TEST_SOURCES
	"${TESTS_PATH}/char_stream_test.cpp"
	"${TESTS_PATH}/response_buffer_test.cpp"
)

set(SOURCES
	${FRAMES_SOURCES}
	${WINDOWS_SOURCES}
	${GUI_SOURCES}
	${HARDWARE_SOURCES}
	${MAIN_SOURCES}
	${TEST_SOURCES}
)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../cube" "${CMAKE_BINARY_DIR}/cube")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../communication" "${CMAKE_BINARY_DIR}/communication")

add_executable(${PROJECT_NAME}
	"${SOURCES}"
    $<TARGET_OBJECTS:cube>
)

target_link_libraries(${PROJECT_NAME}
	"communication"  # static library
	"${ROOT_DIR}/lib/libSTM32CryptographicV3.0.0_CM3_GCC_ot.a"
)

include_directories(
	$<TARGET_PROPERTY:cube,INCLUDE_DIRECTORIES>
	${GUI_PATH}
	${FRAMES_PATH}
	${WINDOWS_PATH}
	${HARDWARE_PATH}
	${MAIN_PATH}
	${LWIP_CONF_PATH}
	# TODO: tests are optional - do not include them in release
	${TESTS_PATH}
)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}
	COMMENT "Size of the binary:"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME} ${PROJECT_NAME}.bin
	COMMENT "Generating raw objcopy"
) 

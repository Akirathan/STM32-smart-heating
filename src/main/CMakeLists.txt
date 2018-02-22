project(main C CXX)

set(SOURCES
	"impl/button.cpp"
	"impl/clk_frame.cpp"
	"impl/control_window.cpp"
	"impl/eeprom.cpp"
	"impl/interval_frame.cpp"
	"impl/interval_frame_data.cpp"
	"impl/io.cpp"
	"impl/lcd.cpp"
	"impl/main.cpp"
	"impl/main_frame.cpp"
	"impl/one_wire.cpp"
	"impl/overview_interval_frame.cpp"
	"impl/relay.cpp"
	"impl/rtc_controller.cpp"
	"impl/set_interval_frame.cpp"
	"impl/static_measure_temp_window.cpp"
	"impl/static_preset_temp_window.cpp"
	"impl/static_temp_window.cpp"
	"impl/static_time_window.cpp"
	"impl/static_window.cpp"
	"impl/syscalls.c"
	"impl/temp_controller.cpp"
	"impl/temp_sensor.cpp"
	"impl/temp_window.cpp"
	"impl/time.cpp"
	"impl/time_window.cpp"
	"impl/window.cpp"
	"impl/window_system.cpp"
)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../cube" "${CMAKE_BINARY_DIR}/cube")

add_executable(${PROJECT_NAME}
	"${SOURCES}"
    $<TARGET_OBJECTS:cube>
)

include_directories(
	$<TARGET_PROPERTY:cube,INCLUDE_DIRECTORIES>
	"impl"
)
project(communication CXX)

set(COMMUNICATION_SOURCES
	"char_stream.cpp"
	"tcp_driver.cpp"
	"interval.cpp"
	"interval_list.cpp"
	"communication_device.cpp"
	"client.cpp"
	"client_timer.cpp"
	"client_error_timer.cpp"
	"client_receive_timeout_timer.cpp"
	"des.cpp"
)

set(HTTP_SOURCES 
	"http/response_buffer.cpp"
	"http/response.cpp"
	"http/request.cpp"
	"http/header.cpp"
	"http/header_option.cpp"
)

set(SOURCES ${COMMUNICATION_SOURCES} ${HTTP_SOURCES})

include_directories(
	"${MAIN_DIR}/main"  # For settings.h
	# These directories are needed to include application.hpp and its dependencies
	"${MAIN_DIR}/gui"
	"${MAIN_DIR}/gui/frames"
	"${MAIN_DIR}/gui/windows"
	"${MAIN_DIR}/hardware"

	$<TARGET_PROPERTY:cube,INCLUDE_DIRECTORIES>  # For LwIP includes
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
	"."
	"http"
)

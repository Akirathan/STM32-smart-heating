project(communication CXX)

set(COMMUNICATION_SOURCES
	"tcp_driver.cpp"
	"interval.cpp"
	"interval_list.cpp"
	"communication_device.cpp"
	"client.cpp"
)

set(HTTP_SOURCES 
	"http/response.cpp"
	"http/request.cpp"
	"http/header.cpp"
	"http/header_option.cpp"
)

set(SOURCES ${COMMUNICATION_SOURCES} ${HTTP_SOURCES})

include_directories(
	"${MAIN_DIR}/main"  # For settings.h
	$<TARGET_PROPERTY:cube,INCLUDE_DIRECTORIES>  # For LwIP includes
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
	"."
	"http"
)

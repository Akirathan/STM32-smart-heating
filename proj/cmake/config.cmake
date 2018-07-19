
set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(BUILD_DIR "${ROOT_DIR}/build")
set(TOOLCHAIN_FILE "toolchain.cmake")

###########            CONFIG    ####################################
# Set these variables according to your system.
set(CMAKE "cmake")
set(VERBOSE_MAKEFILE FALSE)
set(GENERATOR "Unix Makefiles")

# If true, sources from cube-mx will be linked, otherwise sources from
# hardware directory will be linked.
set(USE_CUBE_MX TRUE)

######################################################################

if (VERBOSE_MAKEFILE)
  set(VERBOSE_MAKEFILE_OPT "-DCMAKE_VERBOSE_MAKEFILE=TRUE")
else()
  set(VERBOSE_MAKEFILE_OPT "")
endif()

execute_process(COMMAND
  "${CMAKE}"
  "-G${GENERATOR}"
  "-DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN_FILE}"
  "-DUSE_CUBE_MX=${USE_CUBE_MX}"
  "${VERBOSE_MAKEFILE_OPT}"
  "-B${BUILD_DIR}"
  "-H."
)

# Enable parallel build
if (GENERATOR STREQUAL "Unix Makefiles")
	set(BUILD_CMD "${CMAKE} --build ${BUILD_DIR} -- -j4")
else()
	set(BUILD_CMD "${CMAKE} --build ${BUILD_DIR}")
endif()

# Create build script
file(WRITE "build.bat"
  ${BUILD_CMD}
)

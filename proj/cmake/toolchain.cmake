# Modify these to match your needs.
set(TARGET_TRIPLET "arm-none-eabi")
set(TOOLCHAIN_PREFIX "/home/mayfa/gcc-arm-none-eabi-6-2017-q2-update")
set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/../../STM32F107XC_FLASH.ld")

message("Processing toolchain file ...")

set(TOOLCHAIN_BIN_DIR "${TOOLCHAIN_PREFIX}/bin")
set(TOOLCHAIN_INC_DIR "${TOOLCHAIN_PREFIX}/${TARGET_TRIPLET}/include")
set(TOOLCHAIN_LIB_DIR "${TOOLCHAIN_PREFIX}/${TARGET_TRIPLET}/lib")

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

if(WIN32)
    set(TOOL_EXECUTABLE_SUFFIX ".exe")
else()
    set(TOOL_EXECUTABLE_SUFFIX "")
endif()

if(${CMAKE_VERSION} VERSION_LESS 3.6.0)
    INCLUDE(CMakeForceCompiler)
    CMAKE_FORCE_C_COMPILER("${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-gcc${TOOL_EXECUTABLE_SUFFIX}" GNU)
    CMAKE_FORCE_CXX_COMPILER("${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-g++${TOOL_EXECUTABLE_SUFFIX}" GNU)
else()
    SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
    SET(CMAKE_C_COMPILER "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-gcc${TOOL_EXECUTABLE_SUFFIX}")
    SET(CMAKE_CXX_COMPILER "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-g++${TOOL_EXECUTABLE_SUFFIX}")
endif()
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-gcc${TOOL_EXECUTABLE_SUFFIX}")

set(CMAKE_OBJCOPY "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-objcopy${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "objcopy tool")
set(CMAKE_OBJDUMP "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-objdump${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "objdump tool")
set(CMAKE_SIZE "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-size${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "size tool")
set(CMAKE_DEBUGER "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-gdb${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "debuger")
set(CMAKE_CPPFILT "${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-c++filt${TOOL_EXECUTABLE_SUFFIX}" CACHE INTERNAL "C++filt")


# Generic defines and flags (for both C and CXX)
add_definitions(
  -DSTM32F107VCTx
  -DUSE_HAL_DRIVER
  -DUSE_STM3210C_EVAL
  -DSTM3210C_EVAL
)

# Generic options for C and CXX compilers
add_compile_options(
  -mcpu=cortex-m3
  -mthumb
  -mfloat-abi=soft
  -specs=nano.specs
  -ffunction-sections
  -fdata-sections
  -Wall
)

# Specific options for CXX
string(CONCAT CMAKE_CXX_FLAGS
  "-std=c++14 "
  "-fno-exceptions "
  "-fno-threadsafe-statics "
  "-fno-rtti "
  "-fpermissive "             # For FAT library
)

# Specific options for linker
string(CONCAT CMAKE_EXE_LINKER_FLAGS
  "-mcpu=cortex-m3 "
  "-Wl,-Map=output.map "
  "-Wl,--gc-sections "
  "-Wl,--start-group -lc -lm -Wl,--end-group "
  "-specs=nano.specs "
  "-static "
  "-T${LINKER_SCRIPT} "
  "-Wl,-u,Reset_Handler "
  "-Wl,--defsym=malloc_getpagesize_P=0x1000 "
)



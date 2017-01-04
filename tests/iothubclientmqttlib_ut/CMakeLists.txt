if(NOT ${use_mqtt})
    message(FATAL_ERROR "iothubclientmqttlib being generated without mqtt support")
endif()

compileAsC99()

IF(WIN32)
    #windows needs this define
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-DGB_MEASURE_MEMORY_FOR_THIS -DGB_DEBUG_ALLOC)
ENDIF(WIN32)

include_directories(${IOTHUB_CLIENT_INC_FOLDER})
include_directories(${SHARED_UTIL_INC_FOLDER})

# add the executable
add_executable(iothubclientmqttlib_ut iothubclientmqttlib_ut.c)
target_link_libraries (iothubclientmqttlib_ut duktapert iothubclientmqttlib helper)

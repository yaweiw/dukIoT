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

add_library(iothubclientmqttlib iothubclientmqttlib.c)

target_link_libraries(iothubclientmqttlib
#iothubclient is here only because locking... in gballoc no less.
    iothub_client
    iothub_client_mqtt_transport
)

linkSharedUtil(iothubclientmqttlib)
linkMqttLibrary(iothubclientmqttlib)

#install (TARGETS iothubclientmqttlib DESTINATION lib)
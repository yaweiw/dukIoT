include_directories(${IOTHUB_CLIENT_INC_FOLDER})
include_directories(${SHARED_UTIL_INC_FOLDER})

add_library(iothubclientlib iothubclientlib.c)

target_link_libraries(iothubclientlib 
#iothubclient is here only because locking... in gballoc no less.
	iothub_client
	iothub_client_http_transport
)

linkSharedUtil(iothubclientlib)
linkHttp(iothubclientlib)

install (TARGETS iothubclientlib DESTINATION lib)
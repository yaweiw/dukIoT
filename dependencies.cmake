#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

if (NOT DEFINED AZURE_C_SHARED_UTILITY_INCLUDES OR AZURE_C_SHARED_UTILITY_INCLUDES STREQUAL "")
    set(AZURE_C_SHARED_UTILITY_INCLUDES "${PROJECT_SOURCE_DIR}/azure-iot-sdk-c/c-utility/inc/azure_c_shared_utility")
endif()

if (NOT DEFINED SHARED_UTIL_INC_FOLDER OR SHARED_UTIL_INC_FOLDER STREQUAL "")
    set(SHARED_UTIL_INC_FOLDER "${PROJECT_SOURCE_DIR}/azure-iot-sdk-c/c-utility/inc")
endif()

message(status "#################")
message(status "{AZURE_C_SHARED_UTILITY_INCLUDES}: ${AZURE_C_SHARED_UTILITY_INCLUDES}")
message(status "{azure_c_shared_utility}: ${azure_c_shared_utility}")
message(status "{SHARED_UTIL_INC_FOLDER}: ${SHARED_UTIL_INC_FOLDER}")
message(status "#################")

if(${use_installed_dependencies})
    if(NOT azure_c_shared_utility_FOUND)
        find_package(azure_c_shared_utility REQUIRED CONFIG)
    endif()
    if(NOT uamqp_FOUND)
        find_package(uamqp REQUIRED CONFIG)
    endif()
    if(NOT umqtt_FOUND)
        find_package(umqtt REQUIRED CONFIG)
    endif()
    if(${use_wsio})
        if(NOT libwebsockets_FOUND)
            find_package(libwebsockets REQUIRED CONFIG)
        endif()
    endif()

    include_directories(${AZURE_C_SHARED_UTILITY_INCLUDES}/azure_c_shared_utility)


else()
    add_subdirectory(azure-iot-sdk-c/c-utility)
    add_subdirectory(azure-iot-sdk-c/uamqp)
    add_subdirectory(azure-iot-sdk-c/umqtt)
    include_directories(${SHARED_UTIL_INC_FOLDER}/azure_c_shared_utility)
endif()
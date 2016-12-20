#Copyright (c) Microsoft. All rights reserved.
#Licensed under the MIT license. See LICENSE file in the project root for full license information.

#this is CMakeLists.txt for app
compileAsC99()

set(cmdlet_js_file ${cmdlet_js_file}
    ./cmdlet.js
    )

# add the binary tree to the search path for include files
include_directories (.)
add_subdirectory(cmdlets)
add_subdirectory(duktapert)

# add the executable
add_executable(App app.c)
target_link_libraries (App duktapert)

# add the install targets
install (TARGETS App DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file (COPY ${cmdlet_js_file} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
file (COPY ${cmdlet_js_file} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)



set(module_js_files ${module_js_files}
    ./azure-iot-device.js
    ./connectionstring.js
    ./test.js
    ./protocol.js
    ./message.js
    ./client.js
    ./modulesearch.js
    )

file (COPY ${module_js_files} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
file (COPY ${module_js_files} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
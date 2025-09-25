# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

# Pick which main.cpp to use based on the PIO environment
if(PRODUCT STREQUAL "example_led_builtin")
    # led_builtin example main
    set(MAIN_SRC ${CMAKE_SOURCE_DIR}/components/led_builtin/examples/blink/src/main.cpp)
elseif(PRODUCT STREQUAL "example_status_led")
    # status_led example main
    set(MAIN_SRC ${CMAKE_SOURCE_DIR}/components/led_builtin/examples/status/src/main.cpp)
else()
    # Default main
    set(MAIN_SRC ${CMAKE_SOURCE_DIR}/src/main.cpp)
endif()

# Final source list for this component
set(APP_SRCS ${COMMON_SRCS} ${MAIN_SRC})

idf_component_register(SRCS ${APP_SRCS} INCLUDE_DIRS ${EXTRA_INCLUDE_DIRS})


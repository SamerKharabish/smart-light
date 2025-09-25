FILE(GLOB_RECURSE TEMPLATE_SRCS src/*.*)

idf_component_register(SRCS ${TEMPLATE_SRCS} INCLUDE_DIRS "include")
include(../QxOrm/QxOrm.cmake)
set(target "QxOrmHelperMacros")

add_definitions(-D_BUILDING_QX_ORM_HELPER_MACROS)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC
    "*.cpp"
)
file(GLOB HEADER
    "*.h"
    "*.hpp"
)
add_library(${target} INTERFACE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories(${target} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
install(TARGETS ${target}
LIBRARY DESTINATION "${LIB_FOLDER}"
    ARCHIVE DESTINATION "${LIB_FOLDER}"
    RUNTIME DESTINATION "${EXEC_FOLDER}"
    INCLUDES DESTINATION "${INCLUDE_FOLDER}")
install(FILES ${HEADER} DESTINATION "${INCLUDE_FOLDER}")

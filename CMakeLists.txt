project(Interface)

set(HEADER_FILES
    BackendBase.hpp
    ToolBase.hpp
)

set(CMAKE_CXX_STANDARD 14)
add_library(Interface_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
add_executable(Example_Run main.cpp)

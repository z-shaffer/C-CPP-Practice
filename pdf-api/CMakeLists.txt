cmake_minimum_required(VERSION 3.10)
project(PDF-Core-SDK)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "src/*.cpp")

find_package( CURL REQUIRED )

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty
        ${CMAKE_SOURCE_DIR}/src
)

add_executable(pdfapp ${SOURCES}
        src/main.cpp
        src/pdftools_api.cpp
        src/pdftools_api.h
        src/pdf_processor.cpp
        src/pdf_processor.h
        src/cli_parser.cpp
        src/cli_parser.h)

target_link_libraries(pdfapp curl)

file(GLOB TEST_SRC_FILES "test/*.cpp")
add_executable(pdftest ${TEST_SRC_FILES} test/pdftest.cpp src/cli_parser.cpp)
target_link_libraries(pdftest PRIVATE curl)




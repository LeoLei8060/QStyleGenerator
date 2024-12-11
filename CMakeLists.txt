cmake_minimum_required(VERSION 3.16)
project(QStyleGenerator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/out)

# Find Qt packages
find_package(Qt5 COMPONENTS 
    Core
    Gui
    Widgets
    REQUIRED
)

# Source files
set(SOURCES
    main.cpp
    mainwindow.cpp
)

# Header files
set(HEADERS
    mainwindow.h
)

# QScintilla library
set(QSCINTILLA_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty")
set(QSCINTILLA_INCLUDE_DIR "${QSCINTILLA_ROOT}/include")
set(QSCINTILLA_LIBRARY_DIR "${QSCINTILLA_ROOT}/lib")
set(QSCINTILLA_DLL_DIR "${QSCINTILLA_ROOT}/dll")

# Add QScintilla include directory
include_directories(${QSCINTILLA_INCLUDE_DIR})

link_directories(${QSCINTILLA_LIBRARY_DIR})

# Create executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    "qscintilla2_qt5"
)

# Set include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${QSCINTILLA_INCLUDE_DIR}
)

# Copy DLL files
file(GLOB DLL_FILES "${QSCINTILLA_DLL_DIR}/*.dll")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DLL_FILES}
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    COMMENT "Copying DLL files to output directory"
)

# Install rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

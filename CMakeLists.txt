cmake_minimum_required(VERSION 3.16)

project(memtest86
    VERSION 1.1.0
    DESCRIPTION "Memory diagnostic tool for deepin 25, inspired by memtest86"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets LinguistTools)
find_package(Dtk6 REQUIRED COMPONENTS Widget)

set(TS_FILES
    translations/memtest86_zh_CN.ts
    translations/memtest86_en_US.ts
)

qt6_add_translations(memtest86
    TS_FILES ${TS_FILES}
    QM_FILES_OUTPUT_VARIABLE QM_FILES
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/translations
)

add_executable(memtest86
    src/main.cpp
    src/core/memtester.cpp
    src/core/memtester.h
    src/cli/cli.cpp
    src/cli/cli.h
    src/gui/mainwindow.cpp
    src/gui/mainwindow.h
    src/resources.qrc
    ${QM_FILES}
)

target_link_libraries(memtest86 PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Dtk6::Widget
)

target_include_directories(memtest86 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Development-only core test: verifies stop responsiveness and that
# finished() reports non-zero tested bytes when stopped mid-run.
add_executable(stop_test
    tests/stop_test.cpp
    src/core/memtester.cpp
    src/core/memtester.h
)
target_link_libraries(stop_test PRIVATE Qt6::Core)
target_include_directories(stop_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Install: binary + translations
install(TARGETS memtest86 RUNTIME DESTINATION bin)

install(FILES ${QM_FILES}
    DESTINATION share/memtest86/translations)

# Desktop entry & icon
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/misc/memtest86.desktop.in
    ${CMAKE_CURRENT_BINARY_DIR}/memtest86.desktop
    @ONLY
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/memtest86.desktop
    DESTINATION share/applications)

# App icon (SVG + multi-size PNG, hicolor theme).
# Each PNG is named memtest86.png inside its size directory; source files in
# misc/icons/ are bare <size>.png and are installed with the app name.
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/misc/memtest86.svg
    DESTINATION share/icons/hicolor/scalable/apps
    RENAME memtest86.svg)
foreach(size 16 24 32 48 64 128 256 512)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/misc/icons/${size}.png
        DESTINATION share/icons/hicolor/${size}x${size}/apps
        RENAME memtest86.png)
endforeach()

# Man page
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/misc/memtest86.1
    DESTINATION share/man/man1)

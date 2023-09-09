cmake_minimum_required(VERSION 3.24)
project(wave_function_collapse_ui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.5.1/mingw_64/lib/cmake")

find_package(Qt6 COMPONENTS
        Core
        Xml
        Gui
        Widgets
        Concurrent
        REQUIRED)

add_executable(wave_function_collapse_ui main.cpp
        src/ui_forms/wavefunctioncollapseapp.cpp
        src/ui_forms/wavefunctioncollapseapp.h
        src/ui_forms/wavefunctioncollapseapp.ui
        src/ui_forms/graphicsworkspace.cpp
        src/ui_forms/graphicsworkspace.h
        src/ui_forms/graphicsworkspace.ui
        src/scenes/AbstractScene.h
        src/ui_forms/sudokuscenesidemenu.cpp
        src/ui_forms/sudokuscenesidemenu.h
        src/ui_forms/sudokuscenesidemenu.ui
        src/AbstractSideMenu.h
        src/scenes/SudokuScene.cpp
        src/scenes/SudokuScene.h
        src/graphics_items/GridTile.cpp
        src/graphics_items/GridTile.h
        src/models/SudokuBoard.cpp
        src/models/SudokuBlock.cpp
        src/models/BlockState.cpp
        src/graphics_items/BoardContainer.cpp
        src/graphics_items/BoardContainer.h
        src/utils.cpp
        src/utils.h
        src/widgets/Button.cpp
        src/widgets/Button.h
        src/widgets/ButtonGroup.cpp
        src/widgets/ButtonGroup.h
        src/scenes/ImageGenerationScene.cpp
        src/scenes/ImageGenerationScene.h
        src/models/Cell.cpp
        src/models/Cell.h
        src/models/TiledModel_ImageGrid.cpp
        src/models/TiledModel_ImageGrid.h
        src/models/CellState.cpp
        src/models/CellState.h
        src/graphics_items/CellGraphicsItem.cpp
        src/graphics_items/CellGraphicsItem.h
        src/graphics_items/ImageGenerationCanvas.cpp
        src/graphics_items/ImageGenerationCanvas.h
        src/ui_forms/ImageGenerationSideMenu.cpp
        src/ui_forms/ImageGenerationSideMenu.h
        src/models/TiledRuleModel.cpp
        src/models/TiledRuleModel.h
        src/widgets/CheckBox.cpp
        src/widgets/CheckBox.h
        src/widgets/Slider.cpp
        src/widgets/Slider.h
        src/widgets/SliderGroup.cpp
        src/widgets/SliderGroup.h
)
target_link_libraries(wave_function_collapse_ui
        Qt::Core
        Qt6::Xml
        Qt::Gui
        Qt::Widgets
        Qt::Concurrent
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()

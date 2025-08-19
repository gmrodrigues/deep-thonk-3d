# Library for core logic
add_library(deepThonk3d_lib
    resources.qrc

    # Core
    core/rogerian/Engine.h
    core/rogerian/Engine.cpp

    # UI
    ui/bridge/Bridge.h
    ui/bridge/Bridge.cpp
    ui/model/TreeItem.h
    ui/model/TreeItem.cpp
    ui/model/RuleModel.h
    ui/model/RuleModel.cpp
)

# Link library to Qt
target_link_libraries(deepThonk3d_lib
    PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick
        Qt::QuickControls2
)

# Include directories for library
target_include_directories(deepThonk3d_lib
    PUBLIC # PUBLIC so targets linking to this lib get the include dirs
        ${CMAKE_CURRENT_SOURCE_DIR}
)


# Executable
add_executable(deepThonk3d_app
    main.cpp
)

# Link executable to our library and Qt
target_link_libraries(deepThonk3d_app
    PRIVATE
        deepThonk3d_lib
        Qt::Core
        Qt::Gui
        Qt::Qml
)


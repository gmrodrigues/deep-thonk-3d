# Executable
add_executable(deepThonk3d_app
    main.cpp
    resources.qrc

    # UI
    ui/bridge/Bridge.h
    ui/bridge/Bridge.cpp
)

# Link libraries
target_link_libraries(deepThonk3d_app
    PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick
        Qt::QuickControls2
)

# Include directories
target_include_directories(deepThonk3d_app
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Changelog

All notable changes to this project will be documented in this file.

## [0.2.0] - 2025-08-18

### Added
- Implemented the core C++ Rogerian Engine (`Engine.h`, `Engine.cpp`) for processing user input.
- Added `nlohmann/json` as a third-party dependency for parsing rule files.
- Integrated the `Engine` with the `Bridge` to connect the C++ backend to the QML frontend.
- The application now loads rule packs from JSON files (`en-US.json`, `pt-BR.json`) via Qt resources.
- The chatbot now generates dynamic responses using regex matching, pronoun reflection, and template substitution.

### Fixed
- Corrected multiple C++ compilation errors related to namespaces, member variable access, and data type mismatches between the `Engine` and `Rules.h` definitions.
- Refactored the `Engine` to load rules from a string instead of a file path, simplifying integration with Qt's resource system.

## [0.1.0] - 2025-08-18

### Added
- Initial project structure with CMake build system for a Qt 6 C++/QML application.
- Basic QML UI with a chat log, text input, and a `TableView` for therapist rule structure.
- C++/QML `Bridge` to facilitate communication between the backend and frontend.
- `install_qt6.sh` script to automate the installation of Qt 6 development and QML module dependencies on Debian-based systems.
- `run.sh` script to automate the build and execution of the application.
- `Mvp-0.md`, `docs/Project.md`, and `docs/Architecture.md` to document project goals and design.

### Fixed
- Resolved multiple CMake configuration issues, including Qt6 component naming and target name conflicts.
- Fixed QML resource loading by switching from `qt_add_qml_module` to a manual `resources.qrc` file.
- Systematically identified and fixed runtime errors for missing QML modules by adding necessary packages to `install_qt6.sh` (`qml6-module-qtquick`, `qml6-module-qtquick-controls`, `qml6-module-qtquick-layouts`, `qml6-module-qtqml-workerscript`, `qml6-module-qtquick-templates`, `qml6-module-qtquick-window`).
- Corrected several QML runtime errors, including `TableViewColumn is not a type`, binding loops, and null reference errors, to achieve a stable UI.

### Changed
- Replaced `TreeView` with `TableView` in `Main.qml` to align with available components and fix UI errors.

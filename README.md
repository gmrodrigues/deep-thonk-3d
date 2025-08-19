# Deep Thonk 3D

Deep Thonk 3D is a desktop and web application that simulates a Rogerian psychotherapist, allowing users to engage in text-based conversations to explore their thoughts and feelings.

## Building and Running

This project uses CMake presets for building. Make sure you have CMake, Ninja, and the required Qt 6 dependencies installed.

### Linux Desktop

To configure and build for Linux, run the following commands from the project's root directory:

```bash
# Configure the project
cmake --preset linux-debug

# Build the project
cmake --build --preset linux-debug

# Run the application
./build/linux-debug/src/deepThonk3d_app
```

### WebAssembly (WASM)

To build for WebAssembly, you need to have the Qt for WASM SDK installed. You must also set the `QT6_INSTALL_DIR` environment variable to point to your Qt installation directory (e.g., `/path/to/Qt`).

```bash
# Configure the project
cmake --preset wasm-debug

# Build the project
cmake --build --preset wasm-debug
```

After building, you can serve the contents of the `build/wasm-debug` directory with a local web server to run the application in a browser.

## Switching Locales

The therapist's language can be switched at runtime. To change the locale, simply type `locale:` followed by the desired language code into the chat input field and press Enter. For example:

- `locale:en-US` for English
- `locale:pt-BR` for Brazilian Portuguese

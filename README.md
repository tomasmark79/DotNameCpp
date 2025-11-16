<img src="assets/DotNameCppLogo.svg" width="100%" alt="DotNameCpp Logo">  

<span id="linux-badge"><a href="https://github.com/tomasmark79/DotNameCpp/actions/workflows/linux.yml"><img src="https://github.com/tomasmark79/DotNameCpp/actions/workflows/linux.yml/badge.svg?branch=main" alt="" onerror="document.getElementById('linux-badge').remove()" style="border:0;"></a></span> <span id="linux-clang-badge"><a href="https://github.com/tomasmark79/DotNameCpp/actions/workflows/linux-clang.yml"><img src="https://github.com/tomasmark79/DotNameCpp/actions/workflows/linux-clang.yml/badge.svg?branch=main" alt="" onerror="document.getElementById('linux-clang-badge').remove()" style="border:0;"></a></span> <span id="linux-mingw-badge"><a href="https://github.com/tomasmark79/DotNameCpp/actions/workflows/linux-cross-mingw.yml"><img src="https://github.com/tomasmark79/DotNameCpp/actions/workflows/linux-cross-mingw.yml/badge.svg?branch=main" alt="" onerror="document.getElementById('linux-mingw-badge').remove()" style="border:0;"></a></span> <span id="linux-aarch64-badge"><a href="https://github.com/tomasmark79/DotNameCpp/actions/workflows/linux-cross-aarch64.yml"><img src="https://github.com/tomasmark79/DotNameCpp/actions/workflows/linux-cross-aarch64.yml/badge.svg?branch=main" alt="" onerror="document.getElementById('linux-aarch64-badge').remove()" style="border:0;"></a></span> <span id="macos-badge"><a href="https://github.com/tomasmark79/DotNameCpp/actions/workflows/macos.yml"><img src="https://github.com/tomasmark79/DotNameCpp/actions/workflows/macos.yml/badge.svg?branch=main" alt="" onerror="document.getElementById('macos-badge').remove()" style="border:0;"></a></span> <span id="windows-badge"><a href="https://github.com/tomasmark79/DotNameCpp/actions/workflows/windows.yml"><img src="https://github.com/tomasmark79/DotNameCpp/actions/workflows/windows.yml/badge.svg?branch=main" alt="" onerror="document.getElementById('windows-badge').remove()" style="border:0;"></a></span> <span id="emscripten-badge"><a href="https://github.com/tomasmark79/DotNameCpp/actions/workflows/emscripten.yml"><img src="https://github.com/tomasmark79/DotNameCpp/actions/workflows/emscripten.yml/badge.svg?branch=main" alt="" onerror="document.getElementById('emscripten-badge').remove()" style="border:0;"></a></span> [![discord](https://img.shields.io/discord/1220657359572172810?color=5865F2&logo=discord&logoColor=white)](https://discord.gg/4vkSjvvqSW)

# DotNameCpp — Advanced C++ Development Template

## Table of Contents

- [Overview](#overview)
- [Quick Start](#quick-start)
- [VSCode Keyboard Workflow](#vscode-keyboard-workflow)
- [Build Methods](#build-methods)
- [Prerequisites](#prerequisites)
- [Development Tools](#development-tools)
- [Maintenance Scripts](#maintenance-scripts)
- [License](#license)

---

## Overview

**Keyboard-driven C++ template** for VSCode: configure, build, test, format, coverage, docs, and package—all via shortcuts. Works across Linux/macOS/Windows/Emscripten with deterministic automation via `SolutionController.py`.

[QuickStart Video](https://www.youtube.com/watch?v=0WoN8i8f7-o)  

### Why?

- **Keyboard-first**: One shortcut → structured task chain
- **Modern stack**: CMake presets + Conan 2 + cross/wasm support
- **Built-in quality**: clang-format, clang-tidy, coverage, Doxygen
- **Dual target**: Reusable library + application

---

## Quick Start


### Clone Template

```bash
git clone git@github.com:tomasmark79/DotNameCpp.git
```

**Quick clone with auto-rename** - add to your `.bashrc` / `.zshrc`:

```bash
clonedotname() {
    local PN="${1:-App}"
    git clone git@github.com:tomasmark79/DotNameCpp.git "$PN" && \
    rm -rf "$PN/.git" && \
    cd "$PN" && \
    python SolutionRenamer.py DotNameLib "$PN"Lib DotNameApplication "$PN" && \
    code .
}
```

Then use: `clonedotname MyProject`

### Recommended: VSCode Workflow

Open in VSCode and use keyboard shortcuts:

```text
Shift+F7 → "Zero to Hero"    # Complete setup, build, test, package
F7                              # Quick rebuild
F5                              # Start debugging
```


### GitHub Codespaces

Click **Code** → **Codespaces** → **Create codespace** on GitHub for instant cloud environment.

---

## VSCode Keyboard Workflow

Keyboard-first development: every major action = shortcut → menu → deterministic scripted sequence.

### Core Development Shortcuts

| Shortcut | Action | Description |
|----------|--------|-------------|
| **F5** | Quick Debug | Start debugging application |
| **F7** | Quick Build | Build application (default Debug) |
| **Shift+F7** | Build Menu | **PRIMARY BUILD WORKFLOW** - Access all build tasks |
| **Ctrl+F7** | Other Tasks | Utilities, formatting, documentation, coverage |
| **Ctrl+Alt+R** | Run Application | Execute the built application binary |
| **Ctrl+Alt+E** | Launch Emscripten | Build and serve WebAssembly version |
| **Ctrl+Alt+P** | Build All Presets | Build all CMake presets |

### Build Tasks Menu (Shift+F7)

The **primary development workflow** - this menu contains all essential build operations:

- **Zero to Build**: Complete clean build workflow
- **Zero to Hero**: Full development cycle (build, test, package)
- **Conan Install**: Install/update dependencies
- **CMake Configure**: Configure build system
- **CMake Configure with Coverage**: Configure with code coverage
- **Build**: Compile the project
- **Run Tests**: Execute test suite
- **Clean selected folder**: Clean build artifacts
- **Install built components**: Install built components
- **Create Tarballs**: Package for distribution

### Utility Tasks Menu (Ctrl+F7)

Secondary workflow for code quality and maintenance:

- **Format Code**: Apply clang-format to all C++ files
- **Format CMake**: Format all CMake files
- **clang-tidy linting**: Run static analysis
- **Generate Documentation**: Create Doxygen docs
- **Coverage Tasks from Other Menu**: Quick access to coverage analysis
- **Build All CMakeUserPresets.json**: Build all presets

### Code Quality Shortcuts

| Shortcut | Action | Description |
|----------|--------|-------------|
| **Ctrl+Alt+F** | Format Code | Run clang-format recursively on whole project |
| **Ctrl+Alt+L** | Lint Code | Run clang-tidy analysis |
| **Ctrl+Alt+M** | Format CMake | Run cmake-format recursively on CMake files |

### Coverage Analysis Shortcuts

| Shortcut | Action | Description |
|----------|--------|-------------|
| **Ctrl+Alt+Shift+C** | Configure with Coverage | Configure build with coverage enabled |
| **Ctrl+Alt+H** | Coverage HTML Report | Generate and open HTML coverage report |
| **Ctrl+Alt+Shift+H** | Coverage Full Report | Generate both HTML and XML coverage reports |
| **Ctrl+Alt+T** | Coverage Tasks from Other Menu | Show detailed coverage task options |

### Recommended Development Flow

1. First run: `Shift+F7` → "Zero to Hero" (installs, configures, builds, tests, packages)
2. Iterate: `F7` build → `F5` debug → edit → repeat
3. Quality: `Ctrl+F7` utilities (format / tidy / docs)
4. Tests: `Shift+F7` → "Run Tests"
5. Coverage (native default only): `Ctrl+Alt+H`

### Pro Tips

- Shortcuts configurable via `.vscode/keybindings.json`
- Works identically across Linux / macOS / Windows (and wasm where applicable)

---

## Build Methods

### Method 1: VSCode Shortcuts (Recommended)

Use the keyboard shortcuts detailed in the [VSCode Keyboard Workflow](#vscode-keyboard-workflow) section for a seamless development experience.

> **💡 Note:** All build operations are orchestrated by `SolutionController.py`, which handles Conan dependencies, CMake configuration, and build execution automatically. The raw commands shown in Method 2 are provided for reference and advanced users only - in normal workflow, you simply use VSCode shortcuts and the controller manages everything for you.

### Method 2: CMake + Conan (Raw Commands)

> **⚠️ Advanced Users Only:** These raw commands are typically **not needed** - `SolutionController.py` (invoked via VSCode shortcuts) handles all of this automatically. Use these only for manual control, CI/CD pipelines, or understanding the build process internals.

#### Native Builds

```bash
# Linux/macOS - Complete workflow
conan install . --output-folder=build/application/default/debug \
  -s build_type=Debug -s compiler.cppstd=20 -pr default -b=missing

cmake -B build/application/default/debug \
  -DCMAKE_TOOLCHAIN_FILE=build/application/default/debug/conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Debug \
  -DBUILD_LIBRARY=ON -DBUILD_APPLICATION=ON

cmake --build build/application/default/debug --config Debug -j $(nproc)
ctest --test-dir build/application/default/debug
```

```powershell
# Windows (PowerShell)
conan install . --output-folder=build/application/default/debug `
  -s build_type=Debug -s compiler.cppstd=20 -pr default -b=missing

cmake -B build/application/default/debug `
  -DCMAKE_TOOLCHAIN_FILE=build/application/default/debug/conan_toolchain.cmake `
  -DCMAKE_BUILD_TYPE=Debug `
  -DBUILD_LIBRARY=ON -DBUILD_APPLICATION=ON

cmake --build build/application/default/debug --config Debug
ctest --test-dir build/application/default/debug
```

#### Cross-Compilation Examples

```bash
# Emscripten (WebAssembly)
conan install . --output-folder=build/application/emscripten/debug \
  -s build_type=Debug -s compiler.cppstd=20 \
  -pr:host=emscripten -pr:build=default -b=missing

source build/application/emscripten/debug/conanbuild.sh
cmake -B build/application/emscripten/debug \
  -DCMAKE_TOOLCHAIN_FILE=build/application/emscripten/debug/conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Debug \
  -DBUILD_LIBRARY=ON -DBUILD_APPLICATION=ON \
  -DPLATFORM=Web

cmake --build build/application/emscripten/debug

# MinGW (Windows cross-compile from Linux)
conan install . --output-folder=build/application/x86_64_w64_mingw32/debug \
  -s build_type=Debug -s compiler.cppstd=20 \
  -pr:host=x86_64_w64_mingw32 -pr:build=default -b=missing

source build/application/x86_64_w64_mingw32/debug/conanbuild.sh
cmake -B build/application/x86_64_w64_mingw32/debug \
  -DCMAKE_TOOLCHAIN_FILE=build/application/x86_64_w64_mingw32/debug/conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Debug \
  -DBUILD_LIBRARY=ON -DBUILD_APPLICATION=ON

cmake --build build/application/x86_64_w64_mingw32/debug

# ARM64/aarch64 (Raspberry Pi cross-compile from Linux)
conan install . --output-folder=build/application/rpi4_glibc2.36_gcc12.4/debug \
  -s build_type=Debug -s compiler.cppstd=20 \
  -pr:host=rpi4_glibc2.36_gcc12.4 -pr:build=default -b=missing

source build/application/rpi4_glibc2.36_gcc12.4/debug/conanbuild.sh
cmake -B build/application/rpi4_glibc2.36_gcc12.4/debug \
  -DCMAKE_TOOLCHAIN_FILE=build/application/rpi4_glibc2.36_gcc12.4/debug/conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Debug \
  -DBUILD_LIBRARY=ON -DBUILD_APPLICATION=ON

cmake --build build/application/rpi4_glibc2.36_gcc12.4/debug
```

#### Build Type Variations

```bash
# Release build
conan install . --output-folder=build/application/default/release \
  -s build_type=Release -s compiler.cppstd=20 -pr default -b=missing

cmake -B build/application/default/release \
  -DCMAKE_TOOLCHAIN_FILE=build/application/default/release/conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_LIBRARY=ON -DBUILD_APPLICATION=ON

cmake --build build/application/default/release --config Release

# Release with Debug Info
conan install . --output-folder=build/application/default/relwithdebinfo \
  -s build_type=RelWithDebInfo -s compiler.cppstd=20 -pr default -b=missing

cmake -B build/application/default/relwithdebinfo \
  -DCMAKE_TOOLCHAIN_FILE=build/application/default/relwithdebinfo/conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DBUILD_LIBRARY=ON -DBUILD_APPLICATION=ON

cmake --build build/application/default/relwithdebinfo

# Minimum Size Release
conan install . --output-folder=build/application/default/minsizerel \
  -s build_type=MinSizeRel -s compiler.cppstd=20 -pr default -b=missing

cmake -B build/application/default/minsizerel \
  -DCMAKE_TOOLCHAIN_FILE=build/application/default/minsizerel/conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=MinSizeRel \
  -DBUILD_LIBRARY=ON -DBUILD_APPLICATION=ON

cmake --build build/application/default/minsizerel
```

#### Advanced Options

```bash
# Code coverage (native Debug builds only)
conan install . --output-folder=build/application/default/debug \
  -s build_type=Debug -s compiler.cppstd=20 -pr default -b=missing

cmake -B build/application/default/debug \
  -DCMAKE_TOOLCHAIN_FILE=build/application/default/debug/conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Debug \
  -DBUILD_LIBRARY=ON -DBUILD_APPLICATION=ON \
  -DENABLE_COVERAGE=ON

cmake --build build/application/default/debug
ctest --test-dir build/application/default/debug
gcovr --html-details coverage.html --root .

# With sanitizers (Debug only)
cmake -B build/application/default/debug \
  -DCMAKE_TOOLCHAIN_FILE=build/application/default/debug/conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Debug \
  -DBUILD_LIBRARY=ON -DBUILD_APPLICATION=ON \
  -DSANITIZE_ADDRESS=ON

# With Link-Time Optimization (Release builds)
cmake -B build/application/default/release \
  -DCMAKE_TOOLCHAIN_FILE=build/application/default/release/conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_LIBRARY=ON -DBUILD_APPLICATION=ON \
  -DENABLE_IPO=ON

# With hardening enabled
cmake -B build/application/default/release \
  -DCMAKE_TOOLCHAIN_FILE=build/application/default/release/conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_LIBRARY=ON -DBUILD_APPLICATION=ON \
  -DENABLE_HARDENING=ON

# Library only build
conan install . --output-folder=build/library/default/debug \
  -s build_type=Debug -s compiler.cppstd=20 -pr default -b=missing

cmake -B build/library/default/debug \
  -DCMAKE_TOOLCHAIN_FILE=build/library/default/debug/conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=Debug \
  -DBUILD_LIBRARY=ON -DBUILD_APPLICATION=OFF

cmake --build build/library/default/debug
```

#### Utility Commands

```bash
# List available CMake presets
cmake --list-presets

# Clean build
rm -rf build/application/default/debug
cmake --build build/application/default/debug --target clean

# Install built artifacts
cmake --install build/application/default/debug --prefix install

# Create distribution package
cd build/application/default/debug
cpack -G TGZ

# Run specific test
ctest --test-dir build/application/default/debug -R TestName

# Verbose build output
cmake --build build/application/default/debug --verbose

# Show Conan profile
conan profile show --profile default
conan profile show --profile x86_64_w64_mingw32
```

### Build Options

```cmake
option(ENABLE_GTESTS "Build and run unit tests" ON)
option(ENABLE_CCACHE "Use ccache compiler cache" ON)
option(ENABLE_COVERAGE "Enable code coverage analysis" OFF)
option(ENABLE_IPO "Enable link-time optimization" OFF)
option(ENABLE_HARDENING "Enable security hardening" OFF)
option(SANITIZE_ADDRESS "Enable address sanitizer" OFF)
```

---

## Prerequisites

### Essential Tools

| Tool | Minimum Version | Purpose | Installation |
|------|----------------|---------|--------------|
| **Git** | 2.25+ | Version control | [Download](https://git-scm.com/downloads) |
| **CMake** | 3.31+ | Build system | [Download](https://cmake.org/download/) |
| **Conan** | 2.0+ | Package manager | `pip install conan` |
| **Python** | 3.8+ | Automation scripts | [Download](https://python.org) |
| **Ninja** | 1.10+ | Build tool | [Download](https://ninja-build.org/) |
| **VSCode** | Latest | IDE | [Download](https://code.visualstudio.com/) |


### Recommended Tools

| Tool | Purpose | Installation |
|------|---------|--------------|
| **ccache** | Compilation cache | [Download](https://ccache.dev/download.html) |
| **clang-tidy** | Static analysis | Part of LLVM |
| **clang-format** | Code formatting | Part of LLVM |
| **cmake-format** | CMake formatting | `pip install cmake-format` |
| **Doxygen** | Documentation | [Download](https://www.doxygen.nl/download.html) |
| **gcovr** | Code coverage | `pip install gcovr` |
| **Emscripten** | WebAssembly compilation | [Download](https://emscripten.org/docs/getting_started/downloads.html) |


### Supported Compilers

| Platform | Compilers | Versions |
|----------|-----------|----------|
| **Linux** | GCC, Clang | GCC 11+, Clang 14+ |
| **macOS** | Clang, GCC | Xcode 14+, GCC 11+ |
| **Windows** | MSVC, Clang, GCC | VS 2022, Clang 14+, MinGW 11+ |
| **Emscripten** | emcc | 3.1.0+ |

---

## Development Tools

**Static Analysis**: `Ctrl+Alt+L` (clang-tidy - requires build artifacts)  
**Code Formatting**: `Ctrl+Alt+F` (C++), `Ctrl+Alt+M` (CMake)  
**Documentation**: `Ctrl+F7` → "Generate Documentation" (Doxygen)  
**Code Coverage**: `Ctrl+Alt+H` (native builds only)

---

## Maintenance Scripts

#### SolutionRenamer.py
Rename project from template defaults:

```bash
python3 SolutionRenamer.py DotNameLib NewAwesomeLib DotNameApplication NewAwesomeApplication
```

#### SolutionUpgrader.py
Sync files from upstream template:

```bash
python SolutionUpgrader.py -i    # Interactive mode
python SolutionUpgrader.py -c    # Check for updates
python SolutionUpgrader.py -u README.md  # Update specific file
```

#### SolutionController.py
Central automation hub (normally accessed via VSCode shortcuts):

```bash
python SolutionController.py <product> "<task>" <arch> [build_type]

# Examples:
python SolutionController.py application "Build" default Debug
python SolutionController.py both "Zero to Hero" default Release
```

---

## License

**MIT License**  
Copyright (c) 2024–2025 Tomáš Mark

---

## Important Notes

- **Coverage**: Native `default` Debug builds only
- **Emscripten**: Requires appropriate Conan profile (not auto-installed)
- **Cross-compilation**: Conan profiles must be created manually
- **CI/CD**: See badges at top for current build status

For support: [Open an issue](https://github.com/tomasmark79/DotNameCpp/issues) · [Discussions](https://github.com/tomasmark79/DotNameCpp/discussions)

---

**Happy coding!**

[Back to top](#dotnamecpp--advanced-c-development-template)

<img src="assets/DotNameCppLogo.svg" width="100%" alt="DotNameCpp Logo">

# DotNameCpp - Advanced C++ Development Template

Production-ready C++ starter focused on fast iteration, keyboard‑driven workflows in VSCode, cross‑platform + WebAssembly targets, and automated, reproducible build tooling (CMake + Conan 2 + presets + scripted orchestration). Built to eliminate friction from ad‑hoc shell commands and fragile IDE integrations.

<span id="linux-badge"><a href="https://github.com/tomasmark79/DotNameCpp/actions/workflows/linux.yml"><img src="https://github.com/tomasmark79/DotNameCpp/actions/workflows/linux.yml/badge.svg?branch=main" alt="" onerror="document.getElementById('linux-badge').remove()" style="border:0;"></a></span> <span id="macos-badge"><a href="https://github.com/tomasmark79/DotNameCpp/actions/workflows/macos.yml"><img src="https://github.com/tomasmark79/DotNameCpp/actions/workflows/macos.yml/badge.svg?branch=main" alt="" onerror="document.getElementById('macos-badge').remove()" style="border:0;"></a></span> <span id="windows-badge"><a href="https://github.com/tomasmark79/DotNameCpp/actions/workflows/windows.yml"><img src="https://github.com/tomasmark79/DotNameCpp/actions/workflows/windows.yml/badge.svg?branch=main" alt="" onerror="document.getElementById('windows-badge').remove()" style="border:0;"></a></span><span id="emscripten-badge"> <a href="https://github.com/tomasmark79/DotNameCpp/actions/workflows/emscripten.yml"><img src="https://github.com/tomasmark79/DotNameCpp/actions/workflows/emscripten.yml/badge.svg?branch=main" alt="" onerror="document.getElementById('emscripten-badge').remove()" style="border:0;"></a></span>

> Quick start: `Shift+F7` → "🦸 Zero to Hero" (configure, build, test, package). Then use `F7` (rebuild) and `F5` (debug).

[YouTube Video DotNameCpp QuickStart](https://www.youtube.com/watch?v=0WoN8i8f7-o)  
[YouTube Video DotNameCpp C++ Structure](https://www.youtube.com/watch?v=24snI1QVlIA)
[Discord](https://discord.gg/4vkSjvvqSW)

---

## 📚 Table of Contents

- [🚀 Overview](#-overview)
- [⌨️ VSCode Keyboard Workflow (Primary)](#️-vscode-keyboard-workflow-primary)
- [✨ Key Features](#-key-features)
- [⚡ Quick Start](#-quick-start)
- [🔧 Build System & Configuration (Comprehensive Guide)](#-build-system--configuration-comprehensive-guide)
- [🛠️ Prerequisites & Dependencies](#️-prerequisites--dependencies)
- [🏗️ Project Structure](#️-project-structure)
- [📦 Package Management](#-package-management)
- [🧰 Development Tools](#-development-tools)
- [🌍 Cross-Platform Development](#-cross-platform-development)
- [🌐 WebAssembly & Emscripten](#-webassembly--emscripten)
- [🧪 Testing Framework](#-testing-framework)
- [📚 Library Development](#-library-development)
- [🔄 CI/CD Pipeline](#-cicd-pipeline)
- [🛠️ Maintenance & Utilities](#️-maintenance--utilities)
- [📖 Documentation](#-documentation)
- [📄 License](#-license)

---

## 🚀 Overview

DotNameCpp is an **opinionated template**: everything (configure, build, test, format, coverage, docs, package, wasm serve) is triggered by **VSCode keyboard shortcuts and task menus**—no dependency on the official CMake VSCode extensions. Btw, CMake extensions is still supported. This yields a deterministic experience across Linux / macOS / Windows / Emscripten and reduces cognitive load.

### Why this template?

- Keyboard‑first workflow (no remembering long commands)
- Unified automation via `SolutionController.py` (CI mirrors local)
- Modern CMake presets + Conan 2 + optional cross / wasm profiles
- Built‑in quality: clang-format, clang-tidy, coverage, Doxygen
- Dual target: reusable library + standalone app
- Cross‑platform (native + wasm) with consistent task semantics

### Philosophy
Prefer **predictable scripted orchestration** over fragile IDE state. Keep the developer in flow: one shortcut → structured task chain. Advanced details (toolchain flags, dependency install, coverage setup) live in automation so day‑to‑day work stays simple.

### 🎯 Primary Development Workflow (Essentials)

Copy `.vscode/keybindings.json` into your user settings once (if not already). Then:

1. `Shift+F7` → Build Menu (first: "🦸 Zero to Hero")
2. `F7` Rebuild · `F5` Debug · `Ctrl+Alt+R` Run
3. `Ctrl+F7` Utilities (format / lint / docs / coverage)
4. `Ctrl+Alt+E` Emscripten serve · Coverage: `Ctrl+Alt+H`

---

## ⌨️ VSCode Keyboard Workflow (Primary)

Keyboard-first development: every major action = shortcut → menu → deterministic scripted sequence.

### 🎯 Core Development Shortcuts

| Shortcut | Action | Description |
|----------|--------|-------------|
| **F5** | Quick Debug | Start debugging standalone application |
| **F7** | Quick Build | Build standalone application (default Debug) |
| **Shift+F7** | 🔨 Build Menu | **PRIMARY BUILD WORKFLOW** - Access all build tasks |
| **Ctrl+F7** | 🛠️ Other Tasks | Utilities, formatting, documentation, coverage |
| **Ctrl+Alt+R** | Run Standalone | Execute the built standalone binary |
| **Ctrl+Alt+E** | Launch Emscripten | Build and serve WebAssembly version |
| **Ctrl+Alt+P** | Build All Presets | Build all CMake presets |

### 🔨 Build Tasks Menu (Shift+F7)

The **primary development workflow** - this menu contains all essential build operations:

- **🚀 Zero to Build**: Complete clean build workflow
- **🦸 Zero to Hero**: Full development cycle (build, test, package)
- **🗡️ Conan Install**: Install/update dependencies
- **🔧 CMake Configure**: Configure build system
- **📊 CMake configure with Coverage**: Configure with code coverage
- **🔨 Build**: Compile the project
- **🧪 Run CTest**: Execute test suite
- **🧹 Clean selected folder**: Clean build artifacts
- **📌 Install built components**: Install built components
- **🗜️ Create Tarballs**: Package for distribution

### 🛠️ Utility Tasks Menu (Ctrl+F7)

Secondary workflow for code quality and maintenance:

- **📐 Format Code**: Apply clang-format to all C++ files
- **📏 Format CMake**: Format all CMake files
- **🔍 clang-tidy linting**: Run static analysis
- **📖 Generate Documentation**: Create Doxygen docs
- **📊 Coverage Tasks**: Quick access to coverage analysis
- **🔧 Build All CMakeUserPresets.json**: Build all presets

### 🔬 Code Quality Shortcuts

| Shortcut | Action | Description |
|----------|--------|-------------|
| **Ctrl+Alt+F** | Format Code | Run clang-format on current file |
| **Ctrl+Alt+L** | Lint Code | Run clang-tidy analysis |
| **Ctrl+Alt+M** | Format CMake | Run cmake-format on CMake files |

### 📊 Coverage Analysis Shortcuts

| Shortcut | Action | Description |
|----------|--------|-------------|
| **Ctrl+Alt+Shift+C** | Configure with Coverage | Configure build with coverage enabled |
| **Ctrl+Alt+H** | Coverage HTML Report | Generate and open HTML coverage report |
| **Ctrl+Alt+Shift+H** | Coverage Full Report | Generate both HTML and XML coverage reports |
| **Ctrl+Alt+T** | Coverage Tasks Menu | Show detailed coverage task options |

### 🚀 Recommended Development Flow

1. First run: `Shift+F7` → "🦸 Zero to Hero" (installs, configures, builds, tests, packages)
2. Iterate: `F7` build → `F5` debug → edit → repeat
3. Quality: `Ctrl+F7` utilities (format / tidy / docs)
4. Tests: `Shift+F7` → "🧪 Run CTest"
5. Coverage (native default only): `Ctrl+Alt+H`

### 💡 Pro Tips

- Shortcuts configurable via `.vscode/keybindings.json`
- Works identically across Linux / macOS / Windows (and wasm where applicable)

---

## ✨ Key Features

### 🏗️ **Advanced Build System**
- **CMake 3.31+** with modern preset system
- **Conan 2.0** integration for dependency management
- **Ninja** build system for fast compilation
- **ccache** support for accelerated rebuilds
- **Cross-compilation** ready with toolchain support

### 🌐 **Multi-Platform Support**
- **Native Development**: Linux, macOS, Windows
- **WebAssembly**: Emscripten integration for web deployment
- **Cross-Compilation**: ARM, x86, x64 architectures, etc.

### 🧰 **Development Tools Integration**
- **Static Analysis**: clang-tidy with comprehensive rule sets
- **Code Formatting**: clang-format and cmake-format
- **Documentation**: Doxygen with automated generation
- **Testing**: Google Test framework integration
- **Debugging**: GDB/LLDB/CMake support with VSCode integration
- **Parallel Computing**: TBB - Threading Building Blocks

### 📦 **Project Flexibility**
- **Dual Structure**: Library and standalone application support
- **Modular Architecture**: Reusable components and utilities
- **Asset Management**: Integrated resource handling system
- **Licensing**: Built-in license collection and management

### 🚀 **Automation & CI/CD**
- **GitHub Actions**: Multi-platform CI/CD pipelines
- **Automated Testing**: Unit tests, integration tests, and benchmarks
- **Release Management**: Automated packaging and distribution
- **Quality Gates**: Static analysis and code coverage integration

---

## ⚡ Quick Start

### 🎯 Recommended: VSCode Keyboard Workflow

**Open in VSCode and use keyboard shortcuts** for the best development experience:

```text
Shift+F7 → "🦸 Zero to Hero"    # Complete setup, build, test, and package
F7                              # Quick rebuild
F5                              # Start debugging
```

### 🛠️ Clone the Repository

```bash
git clone git@github.com:tomasmark79/DotNameCpp.git
```

### 🛠️ Clone the Repository with removal of .git with simple command
add this function to your `.bashrc` or `.zshrc`:

```bash
clonedotname() {
    local PN="${1:-AppName}"
    git clone git@github.com:tomasmark79/DotNameCpp.git "$PN" && rm -rf "$PN/.git" && cd "$PN"
    python SolutionRenamer.py DotNameLib "$PN"Lib DotNameStandalone "$PN"Standalone
}
```

Then simply use:
```bash
clonedotname # Clone with default name "AppName"
clonedotname MyCustomAppName # Clone with custom name
```

### ☁️ GitHub Codespaces

```bash
pip install conan
conan profile detect
# Use the Python controller for advanced options - callable from VSCode tasks
python SolutionController.py standalone "🗡️ Conan install" default Debug
python SolutionController.py standalone "🔧 CMake configure" default Debug
python SolutionController.py standalone "🔨 Build" default Debug
```

Click the **Code** button on GitHub → **Codespaces** → **Create codespace** for instant cloud development environment.

---

## 🛠️ Prerequisites & Dependencies

### 🔧 Essential Tools

| Tool | Minimum Version | Purpose | Installation |
|------|----------------|---------|--------------|
| **Git** | 2.25+ | Version control | [Download](https://git-scm.com/downloads) |
| **CMake** | 3.31+ | Build system | [Download](https://cmake.org/download/) |
| **Conan** | 2.0+ | Package manager | `pip install conan` |
| **Python** | 3.8+ | Automation scripts | [Download](https://python.org) |
| **Ninja** | 1.10+ | Build tool | [Download](https://ninja-build.org/) |

### 🛡️ Recommended Tools

| Tool | Purpose | Installation |
|------|---------|--------------|
| **ccache** | Compilation cache | [Download](https://ccache.dev/download.html) |
| **clang-tidy** | Static analysis | Part of LLVM |
| **clang-format** | Code formatting | Part of LLVM |
| **cmake-format** | CMake formatting | `pip install cmake-format` |
| **Doxygen** | Documentation | [Download](https://www.doxygen.nl/download.html) |
| **gcovr** | Code coverage | `pip install gcovr` |
| **TBB** | Parallel computing (Linux) | `sudo dnf install tbb-devel` (Fedora), `sudo apt install libtbb-dev` (Ubuntu) |

### 💻 Supported Compilers

| Platform | Compilers | Versions |
|----------|-----------|----------|
| **Linux** | GCC, Clang | GCC 11+, Clang 14+ |
| **macOS** | Clang, GCC | Xcode 14+, GCC 11+ |
| **Windows** | MSVC, Clang, GCC | VS 2022, Clang 14+, MinGW 11+ |
| **Emscripten** | emcc | 3.1.0+ |

---

## 🏗️ Project Structure

```
📁 DotNameCpp/
├── 📁 .github/                     # GitHub workflow files
├── 📁 .vscode/                     # Visual Studio Code configuration
├── 📁 assets/                      # Project resources
│   ├── customstrings.json          # Localization strings
│   ├── DotNameCppLogo.svg          # Project logo
│   └── ems-mini.html               # Emscripten template
├── 📁 build/                       # Build outputs (auto-generated)
│   ├── 📁 installation/            # Installation artifacts
│   ├── 📁 standalone/              # Standalone builds
│   └── 📁 tarballs/                # Distribution packages
├── 📁 cmake/                       # CMake modules and utilities
│   ├── project-common.cmake        # Common project settings
│   ├── project-library.cmake       # Library-specific configuration
│   ├── project-standalone.cmake    # Standalone app configuration
│   ├── tmplt-*.cmake               # Feature modules (sanitizers, hardening, etc.)
│   └── 📁 modules/                 # Custom CMake modules
├── 📁 doc/                         # Documentation (auto-generated)
├── 📁 include/DotNameLib/          # Public library headers
├── 📁 src/                         # Library source code
│   ├── DotNameLib.cpp              # Main library implementation
│   ├── 📁 Assets/                  # Asset management utilities
│   ├── 📁 Logger/                  # Logging functionality
│   └── 📁 Utils/                   # Utility classes
├── 📁 standalone/                  # Standalone application
│   ├── 📁 src/                     # Application source
│   └── 📁 tests/                   # Application tests
├── 📄 CMakeLists.txt               # Root CMake configuration
├── 📄 CMakeUserPresets.json        # CMake presets for different configurations
├── 📄 conantools.py                # Conan integration utilities  
├── 📄 conanfile.py                 # Conan dependency specification
├── 📄 SolutionController.py        # Main Template automation script
├── 📄 SolutionRenamer.py           # Template renaming utility
└── 📄 SolutionUpgrader.py          # Template update utility
```

---

## 🔧 Build System & Configuration (Comprehensive Guide)

This section provides complete build configuration options for all development scenarios, from beginner-friendly VSCode workflow to advanced command-line usage.

### 🎯 Method 1: VSCode Keyboard Workflow (Recommended)

**Primary approach** using the keyboard shortcuts described above:

```text
Shift+F7 → "🦸 Zero to Hero"    # Complete setup, build, test, package
F7                              # Quick rebuild
F5                              # Debug mode
Ctrl+Alt+R                      # Run application
```

### 🎯 Method 2: Python SolutionController

**Automated workflow** using the Python controller script:

> Normally, you won't need to run these commands manually, since all actions are available through keyboard shortcuts in VSCode.

```bash
# Complete development cycle
python SolutionController.py both "🦸 Zero to Hero" default Debug

# Step-by-step build process
python SolutionController.py standalone "🗡️ Conan install" default Debug
python SolutionController.py standalone "🔧 CMake configure" default Debug
python SolutionController.py standalone "🔨 Build" default Debug
python SolutionController.py standalone "🧪 Run CTest" default Debug

# Cross-platform builds
python SolutionController.py standalone "🔨 Build" emscripten Debug
python SolutionController.py standalone "🔨 Build" x86_64_w64_mingw32 Debug

# Code coverage workflow
python SolutionController.py standalone "📊 Configure with Coverage" default Debug
python SolutionController.py standalone "🔨 Build" default Debug
python SolutionController.py standalone "🧪 Run CTest" default Debug
python SolutionController.py standalone "📊 Coverage HTML Report" default Debug

# Development tools
python SolutionController.py both "🔍 clang-tidy linting" default Debug
python SolutionController.py both "📐 Format Code" noNeedArch
python SolutionController.py both "📏 Format CMake" noNeedArch
python SolutionController.py both "📖 Generate Documentation" noNeedArch
```

### 🎯 Method 3: Pure CMake Workflow

**Direct CMake** for integration with existing build systems:

```bash
# Quick start (recommended for simple cases)
conan install . --output-folder="build/" --build=missing --deployer=full_deploy -s build_type=Debug
cmake --preset generic-linux-x86_64-gcc-15-debug -DBUILD_LIBRARY=ON -DBUILD_STANDALONE=ON
cmake --build build/ -j $(nproc)

# Advanced CMake with presets
cmake --list-presets                                   # List available presets
cmake --preset debug-linux-x86_64-gcc-15               # Configure with preset
cmake --build --preset debug-linux-x86_64-gcc-15       # Build with preset
ctest --preset debug-linux-x86_64-gcc-15               # Test with preset

# Manual CMake configuration (not recommended)
mkdir build && cd build
conan install .. --build=missing
cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_LIBRARY=ON -DBUILD_STANDALONE=ON
make -j $(nproc)
```

**Important:** Always specify the build type (`-s build_type=Debug` or `-s build_type=Release`) with Conan install, and prefer generated CMake presets over manual configuration.

### 🔧 Build Configuration Options

Configure your build with these CMake options:

```cmake
option(ENABLE_GTESTS "Build and run unit tests" ON)
option(ENABLE_CCACHE "Use ccache compiler cache" ON)
option(BUILD_SHARED_LIBS "Build shared (.so) libraries" OFF)
option(USE_STATIC_RUNTIME "Link C++ runtime statically" OFF)
option(ENABLE_IPO "Enable link-time optimization" OFF)
option(ENABLE_HARDENING "Enable security hardening" OFF)
option(ENABLE_COVERAGE "Enable code coverage analysis" OFF)
option(SANITIZE_ADDRESS "Enable address sanitizer" OFF)
option(SANITIZE_UNDEFINED "Enable undefined behavior sanitizer" OFF)
option(SANITIZE_THREAD "Enable thread sanitizer" OFF)
option(SANITIZE_MEMORY "Enable memory sanitizer" OFF)
```

> **⚠️ Coverage Limitation**: Coverage is supported **only for native `default` builds** (locally runnable). Cross targets (Emscripten, MinGW, ARM, etc.) are build‑only in this workflow; tests & coverage skipped.

### 💡 Build System Tips

- **Start with VSCode shortcuts** for the best development experience
- **Use Python SolutionController** for automation and CI/CD
- **Fall back to pure CMake** for integration with existing systems
- **Always specify build types** with Conan to ensure proper linking
- **Use presets** instead of manual CMake configuration
- **Native builds only** for code coverage analysis

---

## 📦 Package Management

### 🔗 Conan 2.0 Integration

This template uses Conan 2.0 for modern, efficient dependency management:

```python
# conanfile.py excerpt
def requirements(self):
    # Core dependencies
    self.requires("fmt/10.1.1")
```

### 📋 Dependency Management Commands

> The only thing you need to do to use Conan in your environment is to install Conan and create a profile for your compiler.

> The template automates everything you need. However, it's useful to know the basic commands for working with Conan. Find the official documentation at [Conan Documentation](https://docs.conan.io/en/latest/).

---

## 🧰 Development Tools

### 🔍 Static Analysis with clang-tidy

> For clang-tidy linting must exist build artifacts due to the nature of the analysis.

Keyboard shortcut: `Ctrl+Alt+L`

### 🎨 Code Formatting

> For clang-format and cmake-format, no build artifacts are needed.
Keyboard shortcuts: `Ctrl+Alt+F` (C++), `Ctrl+Alt+M` (CMake)

---

## 🌍 Cross-Platform Development

### 🖥️ Platform Support Matrix

| Feature | Linux | macOS | Windows | WebAssembly |
|---------|-------|-------|---------|-------------|
| **Native Build** | ✅ | ✅ | ✅ | N/A |
| **Cross-Compilation** | ✅ | ✅ | ✅ | ✅ |
| **Package Management** | ✅ | ✅ | ✅ | ✅ |
| **IDE Integration** | ✅ | ✅ | ✅ | ✅ |
| **CI/CD** | ✅ | ✅ | ✅ | ✅ |

> **📝 parallel computing support note**: For full parallel computing support, install TBB development package: `sudo apt install libtbb-dev` (Ubuntu/Debian) or `sudo dnf install tbb-devel` (Fedora/RHEL)

### 🔧 Cross-Compilation Setup

>Just like with native compilation, the best approach is to use VSCode keyboard shortcuts. However, if you need to use the command line, here are the basic commands:

```bash
# ARM64 Linux from x86_64
conan install . -pr:h=profiles/arm64-linux -pr:b=default

# Windows from Linux
conan install . -pr:h=profiles/windows-x64 -pr:b=default

# Configure cross-compilation
cmake --preset cross-arm64-release
```

---

## 🌐 WebAssembly & Emscripten

### 🚀 Emscripten Integration

> This template provides seamless WebAssembly development with Emscripten. Simply use a Conan profile configured for your environment.

### 🌐 Live Demo

Experience the template in action:
- **Debug Version**: [DotNameStandalone.html (Debug)](https://digitalspace.name/dotnamecpp/preview/debug/DotNameStandalone.html)
- **Release Version**: [DotNameStandalone.html (Release)](https://digitalspace.name/dotnamecpp/preview/release/DotNameStandalone.html)

Experience with more complex scenarios: **GPU required**
- **Debug Version**: [DotNameStandalone.html (Debug)](https://digitalspace.name/dotnamecpp/i2/singularity/debug/index2.html)
- **Release Version**: [DotNameStandalone.html (Release)](https://digitalspace.name/dotnamecpp/i2/singularity/release/index2.html)

### 🎯 Web Deployment

Use the keyboard shortcut to build and serve the WebAssembly application locally:  
Keyboard shortcut: `Ctrl+Alt+E`  

---

## 🧪 Testing Framework

### 🔬 Google Test Integration

Comprehensive testing framework with Google Test:

```cpp
// Example test (standalone/tests/)
#include <gtest/gtest.h>
#include "DotNameLib/DotNameLib.hpp"

TEST(DotNameLibTest, Initialization) {
    EXPECT_TRUE(DotNameLib::isInitialized());
    EXPECT_EQ(DotNameLib::getVersion(), "0.0.1"); // sync with cmake/project-library.cmake
}
```

---

## 📚 Library Development

### 📦 Library Structure

The template supports both header-only and compiled library development:

```cpp
// include/DotNameLib/DotNameLib.hpp
#pragma once

namespace DotNameLib {
    // Public API
    bool initialize();
    void shutdown();
    std::string getVersion();
}

// src/DotNameLib.cpp
#include "DotNameLib/DotNameLib.hpp"

namespace DotNameLib {
    bool initialize() {
        // Implementation
        return true;
    }
}
```

### 🔗 Integration Methods Example

1. project called **Wheels** *using DotNameCpp* and pushed to GitHub
2. Then, in next project named **Car**, using the **Wheels** library as follows

#### Method 1: CPM.cmake (Recommended)

In project Car in file project-standalone.cmake:
```cmake
CPMAddPackage("gh:tomasmark79/Wheels#main")
target_link_libraries(${STANDALONE_NAME} PRIVATE WheelsLib)
```

In project Car in file project-tests.cmake:

```cmake
target_link_libraries(${TEST_NAME} PRIVATE WheelsLib)
```

In project Car in file AppCore.hpp create instance of the library:

```cpp
#include <WheelsLib/WheelsLib.hpp>

std::unique_ptr<dotname::WheelsLib> uniqueWheelsLib;
uniqueWheelsLib = std::make_unique<dotname::WheelsLib> (AppContext::assetsPath);
LOG_I_STREAM << "Wheels count: " << uniqueWheelsLib->getWheelsCount() << std::endl;
```

#### Method 2: FetchContent similar to CPM.cmake

```cmake
include(FetchContent)
FetchContent_Declare(
    Wheels
    GIT_REPOSITORY https://github.com/tomasmark79/Wheels.git
    GIT_TAG v1.0.0
)
FetchContent_MakeAvailable(Wheels)

target_link_libraries(your_target Wheels::Wheels)
```

#### Method 3: Conan Package (Planned)

```python
# In consumer project's conanfile.py
def requirements(self):
    self.requires("dotnamelib/1.0.0")
```

#### Method 4: find_package

```cmake
find_package(Wheels REQUIRED)
target_link_libraries(your_target Wheels::Wheels)
```

#### Method 5: Subdirectory

```cmake
add_subdirectory(path/to/Wheels)
target_link_libraries(your_target Wheels::Wheels)
```

### 🔄 Versioning Strategy

The main versioning for libraries is maintained in the project definition file at `cmake/project-library.cmake`.

```cmake
project(
    ${LIBRARY_NAME}
    VERSION 0.0.1
    LANGUAGES C CXX
    DESCRIPTION "template Copyright (c) 2024 TomasMark [at] digitalspace.name"
    HOMEPAGE_URL "https://github.com/tomasmark79/DotNameCpp")
```

---

## 🔄 CI/CD Pipeline

### 🚀 GitHub Actions Workflows

Comprehensive CI/CD with multi-platform support:

**Implemented Workflows:**
- **✅ Linux CI**: Ubuntu with GCC and Clang
- **✅ macOS CI**: Latest macOS with Xcode
- **✅ Windows CI**: Windows Server with MSVC
- **✅ Emscripten CI**: WebAssembly builds and tests

**Badge Status**: See repository badges at the top (linux.yml · macos.yml · windows.yml · emscripten.yml)

---

## 🛠️ Maintenance & Utilities

### 🔄 Project Maintenance Scripts

#### SolutionRenamer.py
Rename your project from the template:

```bash
python SolutionRenamer.py DotNameLib MyAwesomeLib DotNameStandalone MyAwesomeApp
python SolutionRenamer.py MyAwesomeLib DotNameLib MyAwesomeApp DotNameStandalone
```

Affected files: Check source code for the complete list.

#### SolutionUpgrader.py
Update template files from the upstream repository:

```bash
python SolutionUpgrader.py
```

#### SolutionController.py
Central automation hub for all build and development tasks:

```bash
# Interactive usage - Use VSCode tasks for full functionality
python SolutionController.py

# Command line usage
python SolutionController.py <product> "<task>" <architecture> [build_type]

# Examples:
python SolutionController.py standalone "🔨 Build" default Debug
python SolutionController.py both "🦸 Zero to Hero" default Release
python SolutionController.py emscripten "🚀 Launch Emscripten Server" noNeedArch

# Note: Help system documentation is in development
```

---

## 📖 Documentation

### 📚 Doxygen Integration

Automatic documentation generation with Doxygen:  
Keyboard shortcut: `Ctrl+F7` → "📖 Generate Documentation"

---

## 📄 License

**MIT License**  
Copyright (c) 2024–2025 Tomáš Mark

---

## ⚠️ Important Disclaimer

This template is provided "as is" without warranties of any kind. While extensively tested across multiple platforms and continuously maintained, users must validate that the template meets their specific requirements before production use. The template undergoes regular updates to maintain compatibility with evolving toolchains and dependencies.

**Key Considerations:**
- Verify all dependencies and tools are compatible with your target environment
- Test thoroughly in your specific development and deployment scenarios
- Some features (marked as TODO) are planned but not yet implemented
- Cross-platform builds may require additional configuration on certain systems

---

**Happy coding! 🚀**

For support, questions, or suggestions, please [open an issue](https://github.com/tomasmark79/DotNameCpp/issues) or [start a discussion](https://github.com/tomasmark79/DotNameCpp/discussions).

---

## ⚠️ Limitations & Notes

- Coverage tasks manages native `default` `Debug` only.
- Emscripten requires an appropriate Conan profile (not auto-installed).
- Cross-compilation Conan profiles aren't auto-generated; users must create them.

[🔝 Back to top](#dotnamecpp---advanced-c-development-template)

# 🎯 Dual Language Server Setup (clangd + ccls) - Detailed Guide

This document provides comprehensive setup and configuration for the dual LSP approach used in DotNameCpp.

## 📋 Configuration Overview
This setup combines **clangd** for IntelliSense and diagnostics with **ccls** for CodeLens functionality on Linux systems.

## 🎯 Core Components

### 1. **Dual Language Server Architecture**
- **clangd** - Primary LSP for IntelliSense, code completion, diagnostics
- **ccls** - Secondary LSP solely for CodeLens (reference counting)

### 2. **Required VS Code Extensions**
```bash
# System packages (Fedora/RHEL)
sudo dnf install ccls clang-tools-extra

# System packages (Ubuntu/Debian) 
sudo apt install ccls clangd clang-tools

# VS Code extensions:
# - clangd (llvm-vs-code-extensions.vscode-clangd)
# - ccls (ccls-project.ccls)
# IMPORTANT: Disable MS C/C++ IntelliSense
```

## 📁 Configuration Files

### **`.ccls`** (project root)
```bash
# File type definitions
%h -x c++-header        # .h files as C++ headers
%hpp -x c++-header      # .hpp files
%hxx -x c++-header      # .hxx files
%objc -x objective-c    # Objective-C support
%objcpp -x objective-c++
%objcxx -x objective-c++
%cuda -x cuda          # CUDA support
%cuda_h -x cuda-header

# Project include paths
-I./include
-I./src
-I./standalone/
```

### **`.clangd`** (project root)
```yaml
# Background indexing
Index:
  Background: Build

# Diagnostics and clang-tidy rules
Diagnostics:
  ClangTidy:
    Add: [performance-*, readability-*, modernize-*]
    Remove: [modernize-use-trailing-return-type]
    CheckOptions:
      # Allow short variable names (i, j, k, etc.)
      readability-identifier-length.MinimumVariableNameLength: 1
      readability-identifier-length.MinimumParameterNameLength: 1
      readability-identifier-length.MinimumLoopCounterNameLength: 1
      readability-identifier-length.IgnoredVariableNames: "^[a-z]$"
      readability-identifier-length.IgnoredParameterNames: "^[a-z]$"
  UnusedIncludes: Strict

# Inline hints in editor
InlayHints:
  Enabled: true
  ParameterNames: true
  DeducedTypes: true

# Compilation flags
CompileFlags:
  Add: [-Wall, -Wextra, -std=c++17]
  Remove: [-W*]
  CompilationDatabase: build/standalone/default/debug/

# Code completion settings
Completion:
  AllScopes: true
```

### **VS Code Global Settings** (settings.json)
```json
{
    // === C++ LANGUAGE SERVERS ===
    
    // Disable MS C/C++ IntelliSense (CRITICAL!)
    "C_Cpp.intelliSenseEngine": "disabled",
    
    // CLANGD - Main LSP for IntelliSense
    "clangd.enable": true,
    "clangd.arguments": [
        "--background-index",           // Background indexing
        "--completion-style=detailed",  // Detailed code completion
        "--header-insertion=never",     // Don't auto-insert #include
        "--clang-tidy"                  // Enable clang-tidy checks
    ],
    
    // CCLS - CodeLens only
    "ccls.launch.command": "ccls",
    "ccls.cache.directory": "/home/user/.cache/ccls-cache",
    "ccls.codeLens.enabled": true,
    "ccls.codeLens.localVariables": false,
    "ccls.diagnostics.onChange": 0,    // Disabled - using clangd
    "ccls.diagnostics.onOpen": 0,      // Disabled - using clangd
    "ccls.diagnostics.onSave": 1       // Minimal check on save
}
```

## 🚀 Installation Steps for New Environment

### 1. **System Package Installation**
```bash
# Fedora/RHEL
sudo dnf install ccls clang-tools-extra gcc g++ cmake

# Ubuntu/Debian
sudo apt install ccls clangd clang-tools gcc g++ cmake
```

### 2. **VS Code Extension Installation**
```bash
# Via command line
code --install-extension llvm-vs-code-extensions.vscode-clangd
code --install-extension ccls-project.ccls

# Or in VS Code: Ctrl+Shift+X → search "clangd" and "ccls"
```

### 3. **Configuration Files Setup**
The template already includes `.ccls` and `.clangd` files. For new projects:
```bash
# In project root
touch .ccls .clangd
# Copy content from template
```

### 4. **Generate compile_commands.json**
```bash
# For CMake project (template handles this automatically)
mkdir -p build/standalone/default/debug
cd build/standalone/default/debug
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../../../../
cd -

# Optional symlink in root
ln -s build/standalone/default/debug/compile_commands.json .
```

### 5. **First Launch**
1. Open VS Code in project
2. Wait for indexing (check status bar)
3. Restart language servers: `Ctrl+Shift+P` → "Reload Window"

## 🔧 Troubleshooting

### CodeLens Issues
```bash
# Clear ccls cache
rm -rf ~/.cache/ccls-cache
# Restart: Ctrl+Shift+P → "ccls: Restart"
```

### IntelliSense Issues
```bash
# Clear clangd cache
rm -rf .cache/clangd
# Restart: Ctrl+Shift+P → "clangd: Restart language server"
```

### Verification
- **IntelliSense**: Ctrl+Space for code completion
- **CodeLens**: Reference counts should appear above functions
- **Diagnostics**: Errors underlined in red in real-time

## 📌 Key Benefits

1. **Dual LSP setup** - Best of both worlds
2. **CodeLens references** - Visible reference counts
3. **Fast diagnostics** - Immediate error display
4. **Short variable names** - Allows i, j, k for loops
5. **Header file support** - Proper diagnostics in .hpp files

## 🔄 Updates

Update clangd:
```bash
# In VS Code
Ctrl+Shift+P → "clangd: Download language server"
```

Update ccls:
```bash
sudo dnf update ccls  # Fedora
sudo apt update && sudo apt upgrade ccls  # Ubuntu
```

---
*This configuration has been tested with VS Code and provides optimal C++ development environment with full modern tooling support.*
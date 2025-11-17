# DO NOT use Conan cmake_layout(self) HERE!
import os
import json
from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps
from conan.tools.files import copy
from conan.tools.system import package_manager

# Optional utility script "conantools.py" for common tasks
try:
    import sys
    import os
    sys.path.insert(0, os.path.dirname(__file__))
    from conantools import (
        generate_cmake_with_custom_presets,
        apply_cmake_post_processing,
        copy_additional_files
    )
    UTILITIES_AVAILABLE = True
except ImportError:
    UTILITIES_AVAILABLE = False

class DotNameCppTemplate(ConanFile):
    # name = "dotnamecpptemplate"
    # version = "0.0.3"
    # description = "DotNameCpp - Advanced C++ Development Template"
    # topics = ("cpp", "template", "cmake")
    # url = "https://github.com/tomasmark79/DotNameCpp"
    # license = "MIT"
    # exports_sources = "patches/*", "include/*", "src/*", "CMakeLists.txt"

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"
    
    options = {"fPIC": [True, False]}
    default_options = {"fPIC": True}

    def imports(self):
        self.copy("license*", dst="licenses", folder=True, ignore_case=True)

    def generate(self):
        """Generate CMake files and apply customizations"""
        if UTILITIES_AVAILABLE:
            # Unique CMake toolchain and presets generation
            generate_cmake_with_custom_presets(self)
            # Update preset names, etc.
            apply_cmake_post_processing(self)
            # ImGui binding, etc.
            copy_additional_files(self)
        else:
            # Fallback to basic CMake toolchain generation
            self.output.info("Utility functions not available - using basic CMake generation")
            tc = CMakeToolchain(self)
            tc.variables["CMAKE_BUILD_TYPE"] = str(self.settings.build_type)
            tc.generate()
        
    def configure(self):
        self.options["*"].shared = False

    #def requirements(self):
        #self.requires("fmt/[~11.2]")                    # Formatting library
        #self.requires("nlohmann_json/[~3.12]")          # JSON library
        #self.requires("zlib/[~1.3]")                    # Compression library

    def build_requirements(self):
        # self.tool_requires("cmake/[>=3.15]")          # Minimum CMake version
        # self.tool_requires("ninja/[>=1.10]")          # Ninja build system
        pass

    def system_requirements(self):
        # apt = package_manager.Apt(self)
        # apt.install(["libsdl2-dev", "libgl1-mesa-dev"])
        # dnf = package_manager.Dnf(self)  
        # dnf.install(["SDL2-devel", "mesa-libGL-devel"])
        # pacman = package_manager.PacMan(self)
        # pacman.install(["sdl2", "mesa"])
        pass
        
    def validate(self):
        """Validate configuration (optional)"""
        # Add validation logic here
        # Example: Check for incompatible settings
        # if self.settings.os == "Windows" and self.settings.compiler.libcxx == "libstdc++11":
        #     raise ConanInvalidConfiguration("libstdc++11 not supported on Windows")
        pass
"""
Unified Conan tools for DotName project utilities.
This module combines all conan_tools functionality into a single file.
"""
import os
import json
import re
from pathlib import Path
from conan.tools.files import copy
from conan.tools.cmake import CMakeToolchain

class ConanTools:
    """
    Unified class containing all Conan utilities for the DotName project.
    """
    
    def __init__(self, conan_instance):
        """Initialize with Conan instance for access to settings and output"""
        self.conan = conan_instance
    
    # ========================================================================
    # Main generator functions
    # ========================================================================
    
    def generate_cmake_with_custom_presets(self):
        """
        Generate CMake toolchain with custom preset configuration.
        
        Returns:
            str: Generated preset name
        """
        tc = CMakeToolchain(self.conan)
        tc.variables["CMAKE_BUILD_TYPE"] = str(self.conan.settings.build_type)
        
        # Customize preset name to avoid conflicts
        preset_name = self._generate_unique_preset_name()
        tc.presets_prefix = ""
        tc.presets_build_type_suffix = ""
        tc.preset_name = preset_name
        tc.user_presets_path = "CMakeUserPresets.json"
        self.conan.output.info(f"Setting custom preset name: {preset_name}")
        
        # Generate the toolchain
        tc.generate()
        
        return preset_name
    
    def apply_cmake_post_processing(self):
        """Apply post-processing to generated CMake files"""
        self.conan.output.info("Applying post-processing to preset files...")
        
        # Fix preset names
        self.update_presets(os.getcwd())
        
        # Apply CMake patches
        self.remove_stdcpp_from_system_libs()
    
    def copy_additional_files(self):
        """Copy additional files from dependencies (customize as needed)"""
        # Example: Copy ImGui bindings
        if "imgui" in self.conan.dependencies:
            try:
                imgui_dep = self.conan.dependencies["imgui"]
                copy(self.conan, "*opengl3*", 
                     os.path.join(imgui_dep.package_folder, "res", "bindings"), 
                     os.path.join(self.conan.source_folder, "src/bindings"))
                copy(self.conan, "*sdl2*", 
                     os.path.join(imgui_dep.package_folder, "res", "bindings"), 
                     os.path.join(self.conan.source_folder, "src/bindings"))
            except Exception as e:
                self.conan.output.warning(f"Could not copy ImGui bindings: {e}")
        pass
    
    # ========================================================================
    # CMake presets management
    # ========================================================================
    
    def update_presets(self, working_dir=None):
        """
        Dynamic change of names in CMakePresets.json to avoid name conflicts
        
        This method generates unique preset names based on current Conan settings
        to prevent conflicts when using multiple build configurations.
        
        Args:
            working_dir: Working directory path to detect build type
        """
        preset_file = "CMakePresets.json"
        if not os.path.exists(preset_file):
            self.conan.output.warn(f"Preset file {preset_file} not found in {os.getcwd()}")
            return
            
        try:
            self.conan.output.info(f"Reading preset file: {os.path.abspath(preset_file)}")
            # Read existing presets
            with open(preset_file, "r", encoding="utf-8") as f:
                content = f.read().strip()
                if not content:
                    self.conan.output.warn(f"Preset file {preset_file} is empty, skipping preset updates")
                    return
                data = json.loads(content)
                
            # Generate unique preset name based on current settings and build type
            preset_name = self._generate_preset_name(working_dir or "")
            
            # Update all preset types with the new name
            self._update_all_presets(data, preset_name)
                        
            # Write updated presets back to file
            with open(preset_file, "w", encoding="utf-8") as f:
                json.dump(data, f, indent=4)
                
            self.conan.output.info(f"Updated CMake presets with name: {preset_name}")
                
        except (json.JSONDecodeError, IOError) as e:
            self.conan.output.warn(f"Failed to update CMake presets: {e}")
    
    def _generate_preset_name(self, working_dir_path=""):
        """Generate preset name based on Conan settings and build type"""
        # Detect build type from working directory path
        build_target = "generic"
        if "library" in working_dir_path:
            build_target = "lib"
        elif "application" in working_dir_path:
            build_target = "app"
        
        # Create descriptive name: target-os-arch-compiler-version-buildtype
        return (f"{build_target}-"
               f"{str(self.conan.settings.os).lower()}-"
               f"{self.conan.settings.arch}-"
               f"{str(self.conan.settings.compiler)}-"
               f"{self.conan.settings.compiler.version}-"
               f"{str(self.conan.settings.build_type).lower()}")
    
    def _update_all_presets(self, data, preset_name):
        """Update all preset types with the new name"""
        # Process each preset type (configure, build, test)
        for preset_type in ["configurePresets", "buildPresets", "testPresets"]:
            for preset in data.get(preset_type, []):
                # Update name and display name
                preset["name"] = preset_name
                if "displayName" in preset:
                    preset["displayName"] = preset_name
                # Update reference to configure preset for build/test presets
                if "configurePreset" in preset:
                    preset["configurePreset"] = preset_name
    
    # ========================================================================
    # CMake file patching utilities
    # ========================================================================
    
    def remove_stdcpp_from_system_libs(self):
        """
        Remove stdc++ and fs from SYSTEM_LIBS in generated Conan CMake files
        
        This fixes linking issues that can occur when stdc++ or fs (filesystem) 
        is automatically added to system libraries by Conan generators.
        Modern C++17/20 compilers include filesystem support without needing -lfs.
        """
        generators_path = Path(getattr(self.conan, 'generators_folder', None) or ".")
        
        cmake_files = self._find_cmake_data_files(generators_path)
        
        if not cmake_files:
            self.conan.output.info("No CMake data files found to patch")
            return
            
        patched_count = self._patch_files(cmake_files)
        
        if patched_count > 0:
            self.conan.output.info(f"Successfully patched {patched_count} CMake files")
    
    def _find_cmake_data_files(self, generators_path):
        """Find all *-data.cmake files"""
        cmake_files = list(generators_path.glob("*-data.cmake"))
        cmake_files.extend(generators_path.glob("*-*-*-data.cmake"))
        return list(set(cmake_files))  # Remove duplicates
    
    def _patch_files(self, cmake_files):
        """
        Patch the found CMake files to remove stdc++ and fs
        
        Modern C++17/20 compilers have std::filesystem support built directly 
        into the standard library, so separate linking of -lfs or -lstdc++fs 
        is no longer needed. This patch removes these unnecessary library 
        dependencies to prevent linker errors.
        """
        # Compile regex patterns once for better performance
        # Pattern to match and remove stdc++ from SYSTEM_LIBS
        stdc_pattern = re.compile(
            r'(set\([^_]*_SYSTEM_LIBS(?:_[A-Z]+)?\s+[^)]*?)'
            r'stdc\+\+([^)]*\))', 
            re.MULTILINE
        )
        # Pattern to match and remove fs (filesystem) from SYSTEM_LIBS
        fs_pattern = re.compile(
            r'(set\([^_]*_SYSTEM_LIBS(?:_[A-Z]+)?\s+[^)]*?)'
            r'\bfs\b\s*([^)]*\))', 
            re.MULTILINE
        )
        
        patched_count = 0
        for cmake_file in cmake_files:
            try:
                content = cmake_file.read_text(encoding='utf-8')
                original_content = content
                
                # Remove stdc++ from SYSTEM_LIBS
                content = stdc_pattern.sub(r'\1\2', content)
                
                # Remove fs from SYSTEM_LIBS
                content = fs_pattern.sub(r'\1\2', content)
                
                if content != original_content:
                    cmake_file.write_text(content, encoding='utf-8')
                    self.conan.output.info(f"Patched {cmake_file.name} - removed stdc++/fs from SYSTEM_LIBS")
                    patched_count += 1
                    
            except (IOError, OSError) as e:
                self.conan.output.warn(f"Could not patch {cmake_file}: {e}")
        
        return patched_count
    
    # ========================================================================
    # Helper methods
    # ========================================================================
    
    def _generate_unique_preset_name(self):
        """Generate unique preset name based on settings and build type"""
        # Detect build type from working directory path
        cwd = os.getcwd()
        build_target = "generic"
        if "library" in cwd:
            build_target = "lib"
        elif "application" in cwd:
            build_target = "app"
        
        # Create descriptive name: target-os-arch-compiler-version-buildtype
        return (f"{build_target}-"
               f"{str(self.conan.settings.os).lower()}-"
               f"{self.conan.settings.arch}-"
               f"{str(self.conan.settings.compiler)}-"
               f"{self.conan.settings.compiler.version}-"
               f"{str(self.conan.settings.build_type).lower()}")


# ============================================================================
# Convenience functions for backward compatibility
# ============================================================================

def generate_cmake_with_custom_presets(conan_instance):
    """
    Generate CMake toolchain with custom preset configuration.
    
    Args:
        conan_instance: Conan recipe instance
        
    Returns:
        str: Generated preset name
    """
    tools = ConanTools(conan_instance)
    return tools.generate_cmake_with_custom_presets()


def apply_cmake_post_processing(conan_instance):
    """Apply post-processing to generated CMake files"""
    tools = ConanTools(conan_instance)
    tools.apply_cmake_post_processing()


def copy_additional_files(conan_instance):
    """Copy additional files from dependencies (customize as needed)"""
    tools = ConanTools(conan_instance)
    tools.copy_additional_files()


# Legacy class aliases for backward compatibility
class CMakePresetsManager:
    """Legacy class - use ConanTools instead"""
    def __init__(self, conan_instance):
        self._tools = ConanTools(conan_instance)
    
    def update_presets(self, working_dir=None):
        return self._tools.update_presets(working_dir)


class CMakePatches:
    """Legacy class - use ConanTools instead"""
    def __init__(self, conan_instance):
        self._tools = ConanTools(conan_instance)
    
    def remove_stdcpp_from_system_libs(self):
        return self._tools.remove_stdcpp_from_system_libs()
    
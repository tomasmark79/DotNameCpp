import os
import sys
import re 

# Renamer is respecting existing Uper and Lower letters and keep them in the new name
source_dir = "src"
include_dir = "include"
application_dir = "application"
test_dir = "tests"
asset_dir = "assets"
cmake_dir = "cmake"

# forbidden words that cannot be used in the project name
FORBIDDEN_WORDS = [
    'build', 'app', 'application', 'library', 'default', 'debug', 'release', 'relwithdebinfo',
    'minsizerel', "appcontext", "index", "main", "test", "tests", "example", "examples",
    'logger', 'asset', 'assets', 'utils', 'logging', 'factory', 'manager', 'loader',
    'reader', 'writer', 'formatter', 'platform', 'filesystem', 'json', 'strings',
    'serializer', 'resolver', 'console', 'windows', 'unix', 'emscripten', 'interface',
    'mock', 'error', 'namespace', 'dotnamecpp', 'v1'
]

def check_forbidden_words(name):
    """
    Check if the name contains any forbidden words (case-insensitive).
    Only matches whole words, not partial matches.
    Returns False if a forbidden word is found, True otherwise.
    """
    name_lower = name.lower()
    for word in FORBIDDEN_WORDS:
        # Create pattern that matches word boundaries
        pattern = r'\b' + re.escape(word.lower()) + r'\b'
        if re.search(pattern, name_lower):
            print(f"Error: The name '{name}' contains forbidden word '{word}'")
            return False
    return True

def rename_project(old_lib_name, new_lib_name, old_application_name, new_application_name):
    # Add validation at the start of the function
    if not check_forbidden_words(new_lib_name):
        sys.exit(1)
    if not check_forbidden_words(new_application_name):
        sys.exit(1)

    # Convert to lowercase and uppercase
    old_lib_name_lower = old_lib_name.lower()
    new_lib_name_lower = new_lib_name.lower()
    old_lib_name_upper = old_lib_name.upper()
    new_lib_name_upper = new_lib_name.upper()
    old_application_name_lower = old_application_name.lower()
    new_application_name_lower = new_application_name.lower()
    old_application_name_upper = old_application_name.upper()
    new_application_name_upper = new_application_name.upper()

    # Library can't have the same name as the application project
    if new_lib_name == new_application_name:
        print("Error: new_lib_name and new_application_name must be different")
        sys.exit(1)

    # List of files where the project names should be changed
    files = [
        "CMakeLists.txt",
        f"{cmake_dir}/project-common.cmake",
        f"{cmake_dir}/project-application.cmake",
        f"{cmake_dir}/project-library.cmake",
        f"{cmake_dir}/project-tests.cmake",
        f"{application_dir}/{source_dir}/Application.cpp",
        f"{application_dir}/{test_dir}/CMakeLists.txt",
        f"{application_dir}/{test_dir}/AssetManagerTest.cpp",
        f"{include_dir}/{old_lib_name}/{old_lib_name}.hpp",
        f"{source_dir}/Utils/UtilsFactory.hpp",
        f"{source_dir}/Utils/Logger/ConsoleLogger.hpp",
        ".vscode/launch.json",
        ".vscode/launch-windows.json",
        ".vscode/tasks.json",
        "Doxyfile",
        "conanfile.py",
        "LICENSE",
        "README.md",
        f"{asset_dir}/ems-mini.html"
        # Add more files as needed
    ]

    # 1. FIRST: Update content in files (before renaming paths)
    print("=== Updating file contents ===")
    for file in files:
        if os.path.isfile(file):
            with open(file, 'r', encoding='utf-8') as f:
                content = f.read()

            # Replace all variants using regex with word boundaries for safer replacement
            # Order matters - do longer strings first to avoid partial matches
            patterns = [
                (r'\b' + re.escape(old_application_name_upper) + r'\b', new_application_name_upper),
                (r'\b' + re.escape(old_application_name_lower) + r'\b', new_application_name_lower),
                (r'\b' + re.escape(old_application_name) + r'\b', new_application_name),
                # Special pattern for constants with underscores (must come before general lib name patterns)
                (re.escape(old_lib_name_upper) + r'_(\w+)', new_lib_name_upper + r'_\1'),
                (r'\b' + re.escape(old_lib_name_upper) + r'\b', new_lib_name_upper),
                (r'\b' + re.escape(old_lib_name_lower) + r'\b', new_lib_name_lower),
                (r'\b' + re.escape(old_lib_name) + r'\b', new_lib_name),
            ]
            
            for pattern, replacement in patterns:
                content = re.sub(pattern, replacement, content)
            
            with open(file, 'w', encoding='utf-8') as f:
                f.write(content)
            print(f"✓ Updated content in: {file}")
        else:
            print(f"\033[93m⚠ Skipping (not found): {file}\033[0m")

    # 2. SECOND: Rename individual files (but NOT directories yet)
    print("\n=== Renaming files ===")
    # Note: src/{old_lib_name}.cpp doesn't exist - this is a header-only library
    
    if os.path.isfile(f"{include_dir}/{old_lib_name}/{old_lib_name}.hpp"):
        os.rename(f"{include_dir}/{old_lib_name}/{old_lib_name}.hpp", 
                  f"{include_dir}/{old_lib_name}/{new_lib_name}.hpp")
        print(f"✓ Renamed: {include_dir}/{old_lib_name}/{old_lib_name}.hpp → {include_dir}/{old_lib_name}/{new_lib_name}.hpp")

    # 3. LAST: Rename directories
    print("\n=== Renaming directories ===")
    if os.path.isdir(f"{include_dir}/{old_lib_name}"):
        os.rename(f"{include_dir}/{old_lib_name}", f"{include_dir}/{new_lib_name}")
        print(f"✓ Renamed: {include_dir}/{old_lib_name} → {include_dir}/{new_lib_name}")

    print("\n\033[92m🎉 Project renaming completed successfully!\033[0m")

if __name__ == "__main__":
    if len(sys.argv) != 5:
        print("Usage: python3 SolutionRenamer.py <DotNameLib> <NewAwesomeLib> <DotNameApplication> <NewAwesomeApplication>")
        sys.exit(1)

    old_lib_name = sys.argv[1]
    new_lib_name = sys.argv[2]
    old_application_name = sys.argv[3]
    new_application_name = sys.argv[4]

    rename_project(old_lib_name, new_lib_name, old_application_name, new_application_name)
import os
import sys
import subprocess
import shutil
import platform
import re
import tarfile
import uuid
import json
import webbrowser

# MIT License Copyright (c) 2024-2025 Tomáš Mark

controllerVersion = "v20250919"

systemPlatform = platform.system().lower()
pythonVersion = sys.version.split()[0]
baseName = os.path.basename(__file__)
workSpaceDir = os.path.dirname(os.path.abspath(__file__))

# CMake user presets file name just for [read only] here
user_presets_file = "CMakeUserPresets.json"

# Build folder name
buildFolderName = "build"

# Output directories
installationOutputDir = os.path.join(workSpaceDir, buildFolderName, "installation")
tarrballsOutputDir = os.path.join(workSpaceDir, buildFolderName, "tarballs")

# CMake files to format with cmake-format
cmake_files = [
    os.path.join(workSpaceDir, "CMakeLists.txt"),
    os.path.join(workSpaceDir, "cmake", "project-library.cmake"),
    os.path.join(workSpaceDir, "cmake", "project-standalone.cmake"),
    os.path.join(workSpaceDir, "cmake", "project-common.cmake"),
    os.path.join(workSpaceDir, "standalone", "tests", "CMakeLists.txt")
]

GREEN = "\033[0;32m"
YELLOW = "\033[0;33m"
RED = "\033[0;31m"
LIGHTBLUE = "\033[1;34m"
LIGHTGREEN = "\033[1;32m"
LIGHTRED = "\033[1;31m"
GREY = "\033[1;30m"
NC = "\033[0m"

def exit_ok(msg):
    print(f"{GREEN}{msg}{NC}")
    sys.exit(0)

def exit_with_error(msg):
    print(f"{RED}{msg}{NC}")
    sys.exit(1)

# Get the task name and other parameters from the command line
buildProduct = sys.argv[1] if len(sys.argv) > 1 else None
taskName = sys.argv[2] if len(sys.argv) > 2 else None
buildArch = sys.argv[3] if len(sys.argv) > 3 else None
buildType = sys.argv[4] if len(sys.argv) > 4 else "not defined"

# Calculate the flags for library and standalone
lib_flag = buildProduct in ["both", "library"]
st_flag = buildProduct in ["both", "standalone"]

# Cross compilation flag
isCrossCompilation = False

# Unique ID for debugging CMake
unique_id = str(uuid.uuid4())

# Check if the task name is not empty
if not taskName:
    exit_with_error("Task name is missing. Exiting.")

# Remove comments and trailing commas from JSON
def remove_comments(json_str):
    import re
    # Remove // comments
    json_str = re.sub(r'//.*', '', json_str)
    # Remove /* ... */ block comments
    json_str = re.sub(r'/\*.*?\*/', '', json_str, flags=re.DOTALL)
    # Remove trailing commas in objects and arrays
    json_str = re.sub(r',\s*([}\]])', r'\1', json_str)
    return json_str

# Load tasks.json and get the valid archs and build types
def get_tasks_config():
    tasks_path = os.path.join(workSpaceDir, ".vscode", "tasks.json")
    if os.path.isfile(tasks_path):
        with open(tasks_path, "r", encoding="utf-8") as f:
            raw_content = f.read()
        # Remove comments and trailing commas from JSON
        clean_content = remove_comments(raw_content)
        tasks_config = json.loads(clean_content)
        valid_archs = []
        valid_build_types = []
        for inp in tasks_config.get("inputs", []):
            if inp.get("id") == "buildArch":
                valid_archs = inp.get("options", [])
            if inp.get("id") == "buildType":
                valid_build_types = inp.get("options", [])
        if not valid_archs:
            exit_with_error("Architecture definitions (buildArch) are missing in tasks.json")
        if not valid_build_types:
            exit_with_error("Build type definitions (buildType) are missing in tasks.json")
    else:
        exit_with_error(f"File tasks.json not found: {tasks_path}")
    return valid_archs, valid_build_types

# call the function to get the valid archs and build types
valid_archs, valid_build_types = get_tasks_config()

# debug print all available tasks
# print(f"{YELLOW}Available archs: {valid_archs}{NC}")
print(f"{YELLOW}Available build types: {valid_build_types}{NC}")

# Formatting tasks don't need to set the build architecture
isCrossCompilation = False
if not buildArch == "noNeedArch":
    if buildArch in valid_archs:
        isCrossCompilation = (buildArch != "default")
    else:
        if "darwin" in systemPlatform:
            isCrossCompilation = False
        else:
            exit_with_error("Undefined build architecture. Exiting.")

def print_header():
    print(f"{GREEN}DotNameCpp Solution Controller (c) 2024-2025 Tomáš Mark - {controllerVersion}{NC}")
    print(f"{GREY}Python runtime\t: {pythonVersion}{NC}")
    print(f"{LIGHTBLUE}taskName\t: {taskName}{NC}")
    print(f"Build Product\t: {LIGHTRED}{buildProduct}{NC}")
    print(f"Build Arch\t: {buildArch}")
    print(f"Build Type\t: {buildType}")
    print(f"Work Space\t: {workSpaceDir}{NC}")
    print(f"Install Artef.\t: {installationOutputDir}{NC}")
    print(f"Release Tarballs: {tarrballsOutputDir}{NC}")
    print(f"{GREEN}Cross\t\t: {isCrossCompilation}{NC}")

print_header()

def get_version_and_names_from_cmake_lists():
    # Read root CMakeLists.txt for version
    with open('CMakeLists.txt', 'r') as file:
        cmake_content = file.read()
    
    # Read common cmake file for names (both LIBRARY_NAME and STANDALONE_NAME are defined there)
    with open('cmake/project-common.cmake', 'r') as file:
        common_content = file.read()
    
    # Extract version from root project
    lib_ver = re.search(r'VERSION\s+(\d+\.\d+\.\d+)', cmake_content).group(1)
    
    # Extract names from common cmake file
    lib_name = re.search(r'set\(LIBRARY_NAME\s+(\w+)', common_content).group(1)
    st_name = re.search(r'set\(STANDALONE_NAME\s+(\w+)', common_content).group(1)
    
    return lib_ver, lib_name, st_name

def log2file(message):
    with open(os.path.join(workSpaceDir, "SolutionController.log"), "a") as f:
        f.write(message + "\n")

def execute_command(cmd):
    print(f"{LIGHTBLUE}> Executed: {cmd}{NC}")
    log2file(cmd)
    if systemPlatform == "windows":
        result = subprocess.run(cmd, shell=True)
    else:
        result = subprocess.run(cmd, shell=True, executable="/bin/bash")
    if result.returncode != 0:
        exit_with_error(f"Command failed: {cmd}")

def execute_subprocess(cmd, executable):
    print(f"{LIGHTBLUE}> Executed: {cmd}{NC}")
    if systemPlatform == "windows":
        executable = "C:\\Windows\\System32\\cmd.exe"
    log2file(cmd)
    result = subprocess.run(cmd, shell=True, executable=executable)
    if result.returncode != 0:
        exit_with_error(f"Command failed: {cmd}")

def get_build_dir(kind):
    return os.path.join(buildFolderName, kind, buildArch, buildType.lower())

# https://docs.conan.io/2/tutorial/consuming_packages/cross_building_with_conan.html
def conan_install(bdir):
    with open("CMakeLists.txt") as f:
        cmake_content = f.read()
    profile = "default" if not isCrossCompilation else buildArch
    exeCmd = (
    f'conan install "{workSpaceDir}" '
    f'--output-folder="{os.path.join(workSpaceDir, bdir)}" '
    f'--deployer=full_deploy --build=missing '
    f'--profile:host={profile} --profile:build=default '
    f'--settings build_type={buildType}'
    )
    execute_command(exeCmd)

def cmake_configure(src, bdir, isCMakeDebugger=False, enable_coverage=False):
    conan_toolchain_file_path = os.path.join(workSpaceDir, bdir, "conan_toolchain.cmake")
    
    # Determine build flags based on build directory
    is_library_build = "library" in bdir
    is_standalone_build = "standalone" in bdir
    
    # Set CMake options for orchestrator
    BUILD_OPTIONS = f'-DBUILD_LIBRARY={"ON" if is_library_build else "OFF"} -DBUILD_STANDALONE={"ON" if is_standalone_build else "OFF"}'
    
    # Add coverage option if requested
    if enable_coverage:
        BUILD_OPTIONS += ' -DENABLE_COVERAGE=ON'
    
    # Conan
    # ---------------------------------------------------------------------------------
    if os.path.isfile(conan_toolchain_file_path):
        print(f"{LIGHTBLUE} using file: conan_toolchain.cmake {NC}")
        print(f"{LIGHTBLUE} using file:", conan_toolchain_file_path, NC)
        DCMAKE_TOOLCHAIN_FILE_CMD = f'-DCMAKE_TOOLCHAIN_FILE="{conan_toolchain_file_path}"'
        # Linux and MacOS
        if systemPlatform in ["linux", "darwin"]:
            # CMake configuration for Linux and MacOS with Conan toolchain
            conan_build_sh_file = os.path.join(workSpaceDir, bdir, "conanbuild.sh")
            if (not isCMakeDebugger):
                if (buildArch == "emscripten"):
                    bashCmd = (
                        f'source "{conan_build_sh_file}" && '
                        f'cmake -S "{src}" '
                        f'-B "{os.path.join(workSpaceDir, bdir)}" '
                        f'{DCMAKE_TOOLCHAIN_FILE_CMD} '
                        f'{BUILD_OPTIONS} '
                        f'-DPLATFORM=Web '
                        f'-DCMAKE_BUILD_TYPE={buildType} '
                        f'-DCMAKE_INSTALL_PREFIX="{os.path.join(installationOutputDir, buildArch, buildType.lower())}"'
                    )
                else:
                    bashCmd = (
                        f'source "{conan_build_sh_file}" && '
                        f'cmake -S "{src}" '
                        f'-B "{os.path.join(workSpaceDir, bdir)}" '
                        f'{DCMAKE_TOOLCHAIN_FILE_CMD} '
                        f'{BUILD_OPTIONS} '
                        f'-DCMAKE_BUILD_TYPE={buildType} '
                        f'-DCMAKE_INSTALL_PREFIX="{os.path.join(installationOutputDir, buildArch, buildType.lower())}"'
                    )
            else:
                # CMake configuration debugger
                print (f"uuid: {unique_id}")
                launch_json_path = os.path.join(workSpaceDir, ".vscode", "launch.json")
                try:
                    with open(launch_json_path, 'r') as file:
                        launch_data = json.load(file)
                    for config in launch_data.get("configurations", []):
                        if "pipeName" in config:
                            config["pipeName"] = f"/tmp/cmake-debugger-pipe-{unique_id}"
                    with open(launch_json_path, 'w') as file:
                        json.dump(launch_data, file, indent=4)
                except json.JSONDecodeError as e:
                    print(f"Error decoding JSON: {e}")
                    exit(1)
                print("If you want to debug CMake, please put a breakpoint in your CMakeLists.txt and start debugging in Visual Studio Code.")
                bashCmd = (
                    f'source "{conan_build_sh_file}" && '
                    f'cmake -S "{src}" '
                    f'-B "{os.path.join(workSpaceDir, bdir)}" '
                    f'{DCMAKE_TOOLCHAIN_FILE_CMD} '
                    f'{BUILD_OPTIONS} '
                    f'-DCMAKE_BUILD_TYPE={buildType} '
                    f'-DCMAKE_INSTALL_PREFIX="{os.path.join(installationOutputDir, buildArch, buildType.lower())}" '
                    f'--debugger '
                    f'--debugger-pipe /tmp/cmake-debugger-pipe-{unique_id}'
                )
            # Execute comfigure bash command
            execute_subprocess(bashCmd, "/bin/bash")
        # Windows
        if systemPlatform == "windows":
            # CMake configuration for Windows x64 with Conan toolchain
            conan_build_bat_file = os.path.join(workSpaceDir, bdir, "conanbuild.bat")
            if (not isCMakeDebugger):
                winCmd = (
                    f'call "{conan_build_bat_file}" && '
                    f'cmake -S "{src}" '
                    f'-B "{os.path.join(workSpaceDir, bdir)}" '
                    f'{DCMAKE_TOOLCHAIN_FILE_CMD} '
                    f'{BUILD_OPTIONS} '
                    f'-G "Visual Studio 17 2022" '
                    f'-DCMAKE_BUILD_TYPE={buildType} '
                    f'-DCMAKE_INSTALL_PREFIX="{os.path.join(installationOutputDir, buildArch, buildType.lower())}"'
                )
            else:
                # CMake configuration debugger 
                print (f"uuid: {unique_id}")
                launch_json_path = os.path.join(workSpaceDir, ".vscode", "launch.json")                 
                try:
                    with open(launch_json_path, 'r') as file:
                        launch_data = json.load(file)

                    for config in launch_data.get("configurations", []):
                        if "pipeName" in config:
                            config["pipeName"] = f"\\\\.\\pipe\\cmake-debugger-pipe-{unique_id}"
                    with open(launch_json_path, 'w') as file:
                        json.dump(launch_data, file, indent=4)
                except json.JSONDecodeError as e:
                    print(f"Error decoding JSON: {e}")
                    exit(1)
                print("If you want to debug CMake, please put a breakpoint in your CMakeLists.txt and start debugging in Visual Studio Code.")
                winCmd = (
                    f'call "{conan_build_bat_file}" && '
                    f'cmake -S "{src}" '
                    f'-B "{os.path.join(workSpaceDir, bdir)}" '
                    f'{DCMAKE_TOOLCHAIN_FILE_CMD} '
                    f'{BUILD_OPTIONS} '
                    f'-G "MinGW Makefiles" '
                    f'-DCMAKE_BUILD_TYPE={buildType} '
                    f'-DCMAKE_INSTALL_PREFIX="{os.path.join(installationOutputDir, buildArch, buildType.lower())}" '
                    f'--debugger '
                    f'--debugger-pipe \\\\.\\pipe\\cmake-debugger-pipe-{unique_id}'
                )
            # Execute comfigure windows command
            execute_subprocess(winCmd, "cmd.exe")

    # CMake solo
    # This command condition will miss find_package(Conan's packages) in CMakeLists.txt
    # But it is useful for CMake projects without Conan
    # ---------------------------------------------------------------------------------
    if not os.path.isfile(conan_toolchain_file_path):
        if buildArch in valid_archs and buildArch != "default":
            # CMake configuration for cross-compilation with solo toolchain
            cmake_toolchain_file = os.path.join(workSpaceDir, "Utilities", "CMakeToolChains", buildArch + ".cmake")
            DCMAKE_TOOLCHAIN_FILE_CMD = f"-DCMAKE_TOOLCHAIN_FILE=" + cmake_toolchain_file
            print(f"{LIGHTBLUE} using file:", cmake_toolchain_file, NC)
        else:
            # CMake native
            DCMAKE_TOOLCHAIN_FILE_CMD = ""
        # CMake solo command
        cmd = (
            f'cmake -S "{src}" '
            f'-B "{os.path.join(workSpaceDir, bdir)}" '
            f'{DCMAKE_TOOLCHAIN_FILE_CMD} '
            f'{BUILD_OPTIONS} '
            f'-DCMAKE_BUILD_TYPE={buildType} '
            f'-DCMAKE_INSTALL_PREFIX="{os.path.join(installationOutputDir, buildArch, buildType.lower())}"'
        )
        execute_command(cmd)

def cmake_build(bdir, target=None):

    # --target is optional
    if target is None:
        target = ""
    else:
        target = f"--target {target}"

    conan_build_sh_file = os.path.join(workSpaceDir, bdir, 'conanbuild.sh')
    if os.path.exists(conan_build_sh_file):
        bashCmd = f'source "{conan_build_sh_file}" && cmake --build "{os.path.abspath(bdir)}" {target} -j {os.cpu_count()}'
    else:
        bashCmd = f'cmake --build "{os.path.abspath(bdir)}" {target} -j {os.cpu_count()}'
    execute_subprocess(bashCmd, "/bin/bash")


def cmake_build_presets():
    # Verify that the user presets file exists
    if not os.path.isfile(user_presets_file):
        exit_with_error(f"Error: {user_presets_file} does not exist.")

    # Verify that jq is installed
    if shutil.which("jq") is None:
        exit_with_error("Error: jq is not installed. Install it using 'dnf install jq' (Fedora) or 'apt install jq' (Debian).")

    # Load the list of included preset files
    cmd = f'jq -r \'.include[]\' "{user_presets_file}"'
    result = subprocess.run(cmd, shell=True, capture_output=True, text=True, executable="/bin/bash")
    if result.returncode != 0:
        exit_with_error(f"Error reading {user_presets_file}: {result.stderr.strip()}")

    preset_files = result.stdout.strip().splitlines()

    # Iterate through all preset files
    for file in preset_files:
        if os.path.isfile(file):
            # Extract build presets
            cmd = f'jq -r \'.buildPresets[].name\' "{file}"'
            result = subprocess.run(cmd, shell=True, capture_output=True, text=True, executable="/bin/bash")
            if result.returncode != 0:
                print(f"Error reading {file}: {result.stderr.strip()}")
                continue

            presets = result.stdout.strip().splitlines()

            # Perform build for each preset
            for preset in presets:
                print(f"Building preset: {preset} from {file}")
                build_cmd = f'cmake --build --preset "{preset}"'
                build_result = subprocess.run(build_cmd, shell=True, executable="/bin/bash")
                if build_result.returncode != 0:
                    print(f"Error building preset {preset} from {file}")
        else:
            print(f"File {file} does not exist, skipped.")

def clean_build_folder(bdir):
    print(f"{LIGHTBLUE}> Removing build directory: {bdir}{NC}")
    log2file(f"Remove: {bdir}")
    shutil.rmtree(bdir, ignore_errors=True)

def build_spltr():
    if lib_flag:
        cmake_build(get_build_dir("library"))
    if st_flag:
        cmake_build(get_build_dir("standalone"))

def configure_spltr(enable_coverage=False):
    if lib_flag:
        cmake_configure(".", get_build_dir("library"), False, enable_coverage)
    if st_flag:
        cmake_configure(".", get_build_dir("standalone"), False, enable_coverage)

def configure_spltr_cmake_debugger():
    if lib_flag:
        cmake_configure(".", get_build_dir("library"), True)
    if st_flag:
        cmake_configure(".", get_build_dir("standalone"), True)

def cmake_install(bdir):
    cmake_build(bdir, target="install")

def conan_spltr():
    if lib_flag:
        conan_install(get_build_dir("library"))
    if st_flag:
        conan_install(get_build_dir("standalone"))

def clean_spltr():
    if lib_flag:
        clean_build_folder(get_build_dir("library"))
    if st_flag:
        clean_build_folder(get_build_dir("standalone"))

def installation_spltr():
    if lib_flag:
        cmake_install(get_build_dir("library"))
    if st_flag:
        cmake_install(get_build_dir("standalone"))

def license_spltr():
    lib_ver, lib_name, st_name = get_version_and_names_from_cmake_lists()
    if lib_flag:
        cmake_build(get_build_dir("library"), f"write-licenses-{lib_name}")
    if st_flag:
        cmake_build(get_build_dir("standalone"), f"write-licenses-{st_name}")

def reorder_build_type_to_end(preset_name, build_type):
    """
    Reorder preset name to move build type to the end.
    Example: 'debug-linux-x86_64-gcc-15' -> 'linux-x86_64-gcc-15-debug'
    """
    build_type_lower = build_type.lower()
    if preset_name.startswith(f"{build_type_lower}-"):
        # Remove build type from beginning and add to end
        return preset_name[len(f"{build_type_lower}-"):] + f"-{build_type_lower}"
    else:
        # If build type is not at the beginning, return as is
        return preset_name


def release_tarballs_spltr():
    os.makedirs(tarrballsOutputDir, exist_ok=True)
    lib_ver, lib_name, st_name = get_version_and_names_from_cmake_lists()

    if buildArch in valid_archs:
        if lib_flag:
            # Determine the suffix for the library tarball
            bdir = get_build_dir("library")
            preset_file = os.path.join(workSpaceDir, bdir, "CMakePresets.json")
            # fallback suffix - buildType at the end
            suffix = f"{buildArch}-{buildType.lower()}"
            if os.path.isfile(preset_file):
                try:
                    with open(preset_file, "r", encoding="utf-8") as pf:
                        pd = json.load(pf)
                    cp = pd.get("configurePresets", [])
                    if cp and "name" in cp[0]:
                        preset_name = cp[0]["name"]
                        suffix = reorder_build_type_to_end(preset_name, buildType)
                except Exception:
                    pass
            archive_name = f"{lib_name}-{lib_ver}-{suffix}.tar.gz"
            # Create the source directory path
            source_dir = os.path.join(installationOutputDir, buildArch, buildType.lower())
            if os.listdir(source_dir):
                print(f"Creating library tarball from: {source_dir}")
                out_path = os.path.join(tarrballsOutputDir, archive_name)
                with tarfile.open(out_path, "w:gz") as tar:
                    # exclude bin folder from library tarball
                    tar.add(source_dir, arcname=".", filter=lambda x: None if "bin" in x.name else x)
                print(f"Created tarball: {out_path}")
            else:
                print(f"No content found in {source_dir} for library.")

        if st_flag:
            # Determine the suffix for the standalone tarball
            bdir = get_build_dir("standalone")
            preset_file = os.path.join(workSpaceDir, bdir, "CMakePresets.json")
            # fallback suffix - buildType at the end
            suffix = f"{buildArch}-{buildType.lower()}"
            if os.path.isfile(preset_file):
                try:
                    with open(preset_file, "r", encoding="utf-8") as pf:
                        pd = json.load(pf)
                    cp = pd.get("configurePresets", [])
                    if cp and "name" in cp[0]:
                        preset_name = cp[0]["name"]
                        suffix = reorder_build_type_to_end(preset_name, buildType)
                except Exception:
                    pass
            st_archive_name = f"{st_name}-{lib_ver}-{suffix}.tar.gz"
            source_dir = os.path.join(installationOutputDir, buildArch, buildType.lower())

            if os.listdir(source_dir):
                print(f"Creating standalone tarball from: {source_dir}")
                out_path = os.path.join(tarrballsOutputDir, st_archive_name)
                with tarfile.open(out_path, "w:gz") as tar:
                    tar.add(source_dir, arcname=".")
                print(f"Created tarball: {out_path}")

            else:
                print(f"No content found in {source_dir} for standalone.")

def run_ctest():
    st_build_dir = get_build_dir("standalone") + "/tests"
    if os.path.exists(st_build_dir):
        os.chdir(st_build_dir)
        print(f"Running CTest in {st_build_dir}")
        execute_command("ctest --verbose")
        # execute_command("ctest --output-on-failure")
    else:
        print(f"Build directory {st_build_dir} does not exist. Skipping CTest.")

def run_cpack():
    if lib_flag:
        cmake_build(get_build_dir("library"), target="package")
    if st_flag:
        cmake_build(get_build_dir("standalone"), target="package")

def find_clang_tidy():
    for version in range(20, 0, -1):
        clang_tidy_cmd = f"clang-tidy-{version}"
        if shutil.which(clang_tidy_cmd):
            return clang_tidy_cmd
    return "clang-tidy"  # Fallback to default clang-tidy if no versioned one is found

def launch_emrun_server():
    """Launch Emscripten emrun server for standalone application."""
    """Serving whole workspace enabling load and debugging C++ in browser DevTools."""

    # Get the standalone name and version from CMakeLists.txt
    try:
        lib_ver, lib_name, st_name = get_version_and_names_from_cmake_lists()
    except Exception as e:
        exit_with_error(f"Failed to read project names from CMakeLists.txt: {e}")
    
    # Kill any existing emrun processes first
    try:
        print(f"{YELLOW}Stopping any existing emrun processes...{NC}")
        if systemPlatform == "windows":
            subprocess.run("taskkill /F /IM emrun.exe", shell=True, capture_output=True)
        else:
            # Unix-like
            subprocess.run(f"pkill -f 'emrun.*'", shell=True, capture_output=True)
    except Exception:
        print(f"{YELLOW}No existing emrun processes found or failed to stop them.{NC}")
    
    # Build the path to the emscripten build directory
    emscripten_build_dir_relative = os.path.join(buildFolderName, "standalone", "emscripten", "debug", "bin")
    html_file = os.path.join(workSpaceDir, emscripten_build_dir_relative, f"{st_name}.html")
    
    # Check if the HTML file exists
    if not os.path.exists(html_file):
        exit_with_error(f"HTML file not found: {html_file}\nPlease build the Emscripten target first.")
    
    # Change to the build directory
    os.chdir(workSpaceDir)  # Serve whole workspace due to relative paths in HTML
    print(f"{GREEN}Starting emrun server in: {workSpaceDir}{NC}")
    print(f"{GREEN}Serving relative path: {emscripten_build_dir_relative}/{st_name}.html{NC}")

    # Start emrun server
    try:
        if systemPlatform == "windows":
            subprocess.Popen(f"emrun {st_name}.html", shell=True, 
                           creationflags=subprocess.CREATE_NEW_CONSOLE)
        else:
            # Unix-like
            subprocess.Popen(f"nohup emrun {workSpaceDir} > /dev/null 2>&1 &", shell=True, executable="/bin/bash")
       
        print(f"{GREEN}Emrun server started successfully!{NC}")
        print(f"{LIGHTBLUE}Server should be available at: http://localhost:6931/{NC}")

        # Open the URL in the default web browser
        url = f"http://localhost:6931/{emscripten_build_dir_relative}/{st_name}.html"
        print("Opening default browser...")
        webbrowser.open(url)
            
    except Exception as e:
        exit_with_error(f"Failed to start emrun server: {e}")

def clang_tidy_spltr():
    clang_tidy_cmd = find_clang_tidy()

    def run_clang_tidy(bdir):
        for root, _, files in os.walk(workSpaceDir):
            if buildFolderName in root:
                continue
            for file in files:
                if file.endswith((".c", ".cpp", ".h", ".hpp")):
                    full_path = os.path.join(root, file)
                    cmd = f'{clang_tidy_cmd} -p "{bdir}" "{full_path}"'
                    print(f"Analyzing: {full_path}")
                    execute_command(cmd)
                    print(f"Done: {full_path}")

    if lib_flag:
        bdir = get_build_dir("library")
        run_clang_tidy(bdir)
    if st_flag:
        bdir = get_build_dir("standalone")
        run_clang_tidy(bdir)

def find_clang_format():
    for version in range(99, 0, -1):
        clang_format_cmd = f"clang-format-{version}"
        if shutil.which(clang_format_cmd):
            return clang_format_cmd
    return "clang-format"  # Fallback to default clang-format if no versioned one is found

def clang_format():
    clang_format_cmd = find_clang_format()

    for root, _, files in os.walk(workSpaceDir):
        if buildFolderName in root:
            continue
        for file in files:
            if file.endswith((".c", ".cpp", ".h", ".hpp")):
                full_path = os.path.join(root, file)
                cmd = f'{clang_format_cmd} -i "{full_path}"'
                print(f"Processing: {full_path}")
                execute_command(cmd)
                print(f"Done: {full_path}")

def cmake_format():
    for cmake_file in cmake_files:
        if os.path.isfile(cmake_file):
            cmd = f'cmake-format --enable-markup -i "{cmake_file}"'
            print(f"Processing: {cmake_file}")
            execute_command(cmd)
            print(f"Done: {cmake_file}")

def conan_create():
    cmd = f'conan create "{workSpaceDir}"'
    execute_command(cmd)

def conan_graph():
    cmd = f'conan graph info "{workSpaceDir}" --format=html > graph.html'
    execute_command(cmd)

    # Open the generated graph in the default web browser
    graph_file = os.path.join(workSpaceDir, "graph.html")
    webbrowser.open(graph_file)

# Function to generate Doxygen documentation
def doxygen_documentation():
    if not shutil.which("doxygen"):
        exit_with_error("Doxygen is not installed. Please install it to generate documentation.")
    
    doxygen_config_file = "Doxyfile"
    if not os.path.isfile(doxygen_config_file):
        exit_with_error(f"Doxygen configuration file '{doxygen_config_file}' not found.")
    
    cmd = f'doxygen "{doxygen_config_file}"'
    execute_command(cmd)
    print(f"Doxygen documentation generated in the 'html' directory.")

    # Open the generated documentation in the default web browser
    html_dir = os.path.join(workSpaceDir, "doc/html")
    index_file = os.path.join(html_dir, "index.html")

    if not webbrowser.open(index_file):
        print(f"Documentation index file '{index_file}' does not exist. Please check the Doxygen configuration.")

# ------ coverage functions -----------------------------------

def run_coverage_html():
    """Generate HTML coverage report"""
    build_dir = get_build_dir("standalone")
    if os.path.exists(build_dir):
        os.chdir(build_dir)
        print(f"Generating HTML coverage report in {build_dir}")
        execute_command("cmake --build . --target coverage-html")
        
        # Try to open coverage report in browser
        coverage_file = os.path.join(os.getcwd(), "coverage.html")
        if os.path.exists(coverage_file):
            webbrowser.open(coverage_file)
            print(f"Coverage HTML report opened in browser: {coverage_file}")
        else:
            print(f"Coverage report not found at {coverage_file}")
    else:
        print(f"Build directory {build_dir} does not exist. Run build with -DENABLE_COVERAGE=ON first.")

def run_coverage_xml():
    """Generate XML coverage report (for CI/CD)"""
    build_dir = get_build_dir("standalone")
    if os.path.exists(build_dir):
        os.chdir(build_dir)
        print(f"Generating XML coverage report in {build_dir}")
        execute_command("cmake --build . --target coverage-xml")
        
        coverage_file = os.path.join(os.getcwd(), "coverage.xml")
        if os.path.exists(coverage_file):
            print(f"Coverage XML report generated: {coverage_file}")
        else:
            print(f"Coverage XML report not found at {coverage_file}")
    else:
        print(f"Build directory {build_dir} does not exist. Run build with -DENABLE_COVERAGE=ON first.")

def run_coverage_summary():
    """Display coverage summary in console"""
    build_dir = get_build_dir("standalone")
    if os.path.exists(build_dir):
        os.chdir(build_dir)
        print(f"Displaying coverage summary from {build_dir}")
        execute_command("cmake --build . --target coverage-summary")
    else:
        print(f"Build directory {build_dir} does not exist. Run build with -DENABLE_COVERAGE=ON first.")

def run_coverage_full():
    """Generate both HTML and XML coverage reports"""
    build_dir = get_build_dir("standalone")
    if os.path.exists(build_dir):
        os.chdir(build_dir)
        print(f"Generating full coverage report in {build_dir}")
        execute_command("cmake --build . --target coverage")
        
        # Try to open HTML coverage report in browser
        coverage_file = os.path.join(os.getcwd(), "coverage.html")
        if os.path.exists(coverage_file):
            webbrowser.open(coverage_file)
            print(f"Coverage HTML report opened in browser: {coverage_file}")
        
        # Check XML report
        xml_file = os.path.join(os.getcwd(), "coverage.xml")
        if os.path.exists(xml_file):
            print(f"Coverage XML report generated: {xml_file}")
    else:
        print(f"Build directory {build_dir} does not exist. Run build with -DENABLE_COVERAGE=ON first.")

def run_coverage_reset():
    """Reset coverage counters"""
    build_dir = get_build_dir("standalone")
    if os.path.exists(build_dir):
        os.chdir(build_dir)
        print(f"Resetting coverage counters in {build_dir}")
        execute_command("cmake --build . --target coverage-reset")
    else:
        print(f"Build directory {build_dir} does not exist.")

def configure_with_coverage():
    """Configure build with coverage enabled"""
    print("Configuring build with code coverage enabled...")
    
    original_dir = os.getcwd()
    
    try:
        # Install dependencies first using Conan
        conan_spltr()
        # Configure with coverage enabled using the unified function
        configure_spltr(enable_coverage=True)
        print("Build configured with coverage enabled. You can now build and run tests.")
        print("After running tests, use coverage tasks to generate reports.")
    finally:
        os.chdir(original_dir)

# ------ help functions for task map --------------------------

def zero_to_build():
    clean_spltr()
    conan_spltr()
    configure_spltr()
    build_spltr()

def zero_to_hero():
    zero_to_build()
    installation_spltr()
    release_tarballs_spltr()

# ------ task map ---------------------------------------------

task_map = {
    "🚀 Zero to Build": zero_to_build,
    "🦸 Zero to Hero": zero_to_hero,
    "🧹 Clean Build": clean_spltr,
    "🗡️ Conan Install": conan_spltr,
    "🔧 CMake Configure": configure_spltr,
    "📊 CMake Configure with Coverage": configure_with_coverage,
    "🪲 CMake Configure with CMake-debugger": configure_spltr_cmake_debugger,
    "🔨 Build": build_spltr,
    "🧪 Run Tests": run_ctest,
    "📜 Gather dependency licenses": license_spltr,
    "📌 Install built components": installation_spltr,
    "🗜️ Create Tarballs for distribution": release_tarballs_spltr,
    "🛸 Run CPack": run_cpack,
    "🔍 clang-tidy linting": clang_tidy_spltr,
    "📐 Format Code": clang_format,
    "📏 Format CMake": cmake_format,
    "🔨 Build All CMakeUserPresets.json": cmake_build_presets,
    "⚔️ Create Conan Recipe" : conan_create,
    "📊 Dependency Graph": conan_graph,
    "📖 Generate Documentation": doxygen_documentation,
    "🚀 Launch Emscripten Server": launch_emrun_server,
    "📊 Coverage HTML Report": run_coverage_html,
    "📊 Coverage XML Report": run_coverage_xml,
    "📊 Coverage Summary": run_coverage_summary,
    "📊 Coverage Full Report": run_coverage_full,
    "📊 Coverage Reset": run_coverage_reset,
    "": lambda: exit_ok("")
}

if taskName in task_map:
    task_map[taskName]()
else:
    print(f"Received unknown task: {taskName}")
    exit_with_error("Task name is missing. Exiting.")

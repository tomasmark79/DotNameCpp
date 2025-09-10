import os
import requests
import shutil
from datetime import datetime
import sys
import subprocess
import tempfile
import logging
import hashlib
from dateutil import parser as date_parser

# MIT License Copyright (c) 2024-2025 Tomáš Mark

# Configure logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s'
)

# URL of the repository with the updated files
repo_url = "https://raw.githubusercontent.com/tomasmark79/DotNameCpp/main/"
token = os.environ.get("GITHUB_TOKEN", "")  # Set this environment variable with your token

def check_write_permissions(path):
    """Check if we have write permissions for the file path or its parent directory."""
    dir_path = os.path.dirname(path) or '.'
    
    # Check if directory exists
    if not os.path.exists(dir_path):
        try:
            # Try to create the directory first
            os.makedirs(dir_path, exist_ok=True)
        except (IOError, OSError) as e:
            logging.error(f"Failed to create directory {dir_path}: {str(e)}")
            return False
    
    # Check if we can write to the directory
    try:
        test_file = os.path.join(dir_path, '.write_test')
        with open(test_file, 'w') as f:
            f.write('test')
        os.remove(test_file)
        return True
    except (IOError, OSError) as e:
        logging.error(f"No write permissions for {dir_path}: {str(e)}")
        return False

def create_backup_dir():
    timestamp = datetime.now().strftime("%Y%m%d%H%M%S")
    backup_dir = os.path.join("dotnamebackup", timestamp)
    os.makedirs(backup_dir, exist_ok=True)
    return backup_dir

def is_binary_file(file_path):
    """Determine if a file is binary based on extension or content."""
    # Check based on extension
    binary_extensions = ['.png', '.jpg', '.jpeg', '.gif', '.bmp', '.ico', '.pdf', 
                         '.exe', '.dll', '.so', '.dylib', '.bin', '.dat']
    
    ext = os.path.splitext(file_path)[1].lower()
    if ext in binary_extensions:
        return True
    
    # If not determined by extension, check file content
    try:
        with open(file_path, 'rb') as f:
            chunk = f.read(1024)
            # Check for null bytes which typically indicate binary content
            if b'\x00' in chunk:
                return True
            # Try to decode as text
            try:
                chunk.decode('utf-8')
                return False
            except UnicodeDecodeError:
                return True
    except (IOError, OSError):
        # If can't open, assume it's not binary
        return False

def can_update_file(file_path):
    # For SolutionUpgrader.py: allow self-update only in clones, not in main template
    if file_path == "SolutionUpgrader.py":
        if is_main_template():
            # We're in the main template - don't allow self-update to prevent losing improvements
            return False
        else:
            # We're in a clone - allow self-update to get latest version from template
            return True
        
    # Skip checks for binary files
    if os.path.exists(file_path) and is_binary_file(file_path):
        return True
        
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            for line in file:
                if "<DOTNAME_NO_UPDATE>" in line:
                    return False
    except UnicodeDecodeError:
        try:
            # Fallback to binary mode and decode line by line
            with open(file_path, 'rb') as file:
                for line in file:
                    try:
                        decoded_line = line.decode('utf-8', errors='ignore')
                        if "<DOTNAME_NO_UPDATE>" in decoded_line:
                            return False
                    except:
                        pass
        except:
            # If all else fails, allow update
            pass
    except (IOError, OSError):
        # File doesn't exist yet, so it can be updated
        pass
    return True

def ensure_directory_exists(path):
    """Ensure that the directory for the given file path exists."""
    dir_path = os.path.dirname(path)
    if dir_path and not os.path.exists(dir_path):
        try:
            os.makedirs(dir_path, exist_ok=True)
            logging.info(f"Created directory structure: {dir_path}")
            return True
        except (IOError, OSError) as e:
            logging.error(f"Failed to create directory {dir_path}: {str(e)}")
            return False
    return True

def update_file(file_path, backup_dir):
    # Ensure directory structure exists
    if not ensure_directory_exists(file_path):
        return False

    if not check_write_permissions(file_path):
        logging.error(f"No write permissions for {file_path}")
        return False

    try:
        url = repo_url + file_path
        headers = {"Authorization": f"token {token}"} if token else {}
        
        # Use binary mode for all requests to handle both text and binary files
        response = requests.get(url, timeout=30, verify=True, headers=headers)
        response.raise_for_status()

        # Backup existing file
        if os.path.exists(file_path) and file_path != "SolutionUpgrader.py":
            backup_path = os.path.join(backup_dir, file_path)
            # Ensure backup directory structure exists
            os.makedirs(os.path.dirname(backup_path), exist_ok=True)
            shutil.copy2(file_path, backup_path)
            logging.info(f"Backed up: {file_path}")

        # Handle binary files
        if is_binary_file(file_path) or any(file_path.endswith(ext) for ext in ['.png', '.jpg', '.jpeg', '.gif']):
            with open(file_path, 'wb') as file:
                file.write(response.content)
            logging.info(f"Updated binary file: {file_path}")
        else:
            # Try UTF-8 first, fallback to detected encoding
            try:
                with open(file_path, 'w', encoding='utf-8') as file:
                    file.write(response.text)
                logging.info(f"Updated: {file_path}")
            except UnicodeEncodeError:
                with open(file_path, 'w', encoding=response.encoding) as file:
                    file.write(response.text)
                logging.info(f"Updated with {response.encoding} encoding: {file_path}")
        return True

    except requests.RequestException as e:
        logging.error(f"Failed to update {file_path}: {str(e)}")
    except OSError as e:
        logging.error(f"File system error for {file_path}: {str(e)}")
    return False

def get_all_files_from_repo():
    """Get all files from the GitHub repository using GitHub API."""
    try:
        # GitHub API URL for repository contents
        api_url = "https://api.github.com/repos/tomasmark79/DotNameCpp/git/trees/main?recursive=1"
        headers = {"Authorization": f"token {token}"} if token else {}
        
        response = requests.get(api_url, timeout=30, verify=True, headers=headers)
        response.raise_for_status()
        
        data = response.json()
        files = []
        
        # Filter only files (not directories) and exclude certain patterns
        exclude_patterns = [
            '.git/',
            'build/',
            'dotnamebackup/',
            '__pycache__/',
            '.pytest_cache/',
            'doc/html/',
            'doc/latex/',
            '.vscode/ipch/',
            'build_*/build'
        ]
        
        for item in data.get('tree', []):
            if item['type'] == 'blob':  # blob = file, tree = directory
                file_path = item['path']
                
                # Skip files matching exclude patterns
                should_exclude = False
                for pattern in exclude_patterns:
                    if pattern in file_path:
                        should_exclude = True
                        break
                
                if not should_exclude:
                    files.append(file_path)
        
        logging.info(f"Found {len(files)} files in repository")
        return files
        
    except requests.RequestException as e:
        logging.error(f"Failed to get repository files: {str(e)}")
        return []

def get_files_to_check():
    """Get list of files to check - auto-discovered from repository."""
    logging.info("Auto-discovering files from repository...")
    return get_all_files_from_repo()

def is_main_template():
    """Check if we're in the main template repository (not a clone)."""
    try:
        # Primary method: check git remote URL
        result = subprocess.run(['git', 'remote', 'get-url', 'origin'], 
                              capture_output=True, text=True, timeout=5)
        if result.returncode == 0:
            remote_url = result.stdout.strip()
            # If remote points to the main template repository
            if 'tomasmark79/DotNameCpp' in remote_url:
                return True
    except (subprocess.TimeoutExpired, subprocess.CalledProcessError, FileNotFoundError):
        pass
    
    # Fallback method: check directory name
    try:
        current_dir = os.path.basename(os.getcwd())
        return current_dir == 'DotNameCpp'
    except:
        pass
    
    # If all methods fail, assume it's a clone (safer for self-update protection)
    return False

def get_file_hash(file_path):
    """Calculate SHA256 hash of a local file."""
    if not os.path.exists(file_path):
        return None
    
    try:
        hasher = hashlib.sha256()
        if is_binary_file(file_path):
            with open(file_path, 'rb') as f:
                for chunk in iter(lambda: f.read(4096), b""):
                    hasher.update(chunk)
        else:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                hasher.update(f.read().encode('utf-8'))
        return hasher.hexdigest()
    except (IOError, OSError) as e:
        logging.error(f"Failed to calculate hash for {file_path}: {str(e)}")
        return None

def get_remote_file_hash(file_path):
    """Get SHA256 hash of remote file."""
    try:
        url = repo_url + file_path
        headers = {"Authorization": f"token {token}"} if token else {}
        
        response = requests.get(url, timeout=30, verify=True, headers=headers)
        response.raise_for_status()
        
        hasher = hashlib.sha256()
        hasher.update(response.content)
        return hasher.hexdigest()
    except requests.RequestException as e:
        logging.error(f"Failed to get remote hash for {file_path}: {str(e)}")
        return None

def get_file_last_commit_date(file_path):
    """Get the date of the last commit for a file from GitHub."""
    try:
        # GitHub API URL for file commits
        api_url = f"https://api.github.com/repos/tomasmark79/DotNameCpp/commits"
        headers = {"Authorization": f"token {token}"} if token else {}
        
        params = {
            "path": file_path,
            "per_page": 1  # We only need the latest commit
        }
        
        response = requests.get(api_url, params=params, timeout=30, verify=True, headers=headers)
        response.raise_for_status()
        
        commits = response.json()
        if commits and len(commits) > 0:
            commit_date_str = commits[0]['commit']['committer']['date']
            # Parse ISO 8601 date string to datetime object
            commit_date = date_parser.parse(commit_date_str)
            return commit_date
        
        return None
        
    except requests.RequestException as e:
        logging.error(f"Failed to get commit date for {file_path}: {str(e)}")
        return None
    except Exception as e:
        logging.error(f"Failed to parse commit date for {file_path}: {str(e)}")
        return None

def get_local_file_modification_date(file_path):
    """Get the local file modification date."""
    try:
        if os.path.exists(file_path):
            timestamp = os.path.getmtime(file_path)
            return datetime.fromtimestamp(timestamp)
        return None
    except OSError as e:
        logging.error(f"Failed to get modification date for {file_path}: {str(e)}")
        return None

def check_file_status(file_path):
    """Check file status compared to remote version.
    Returns:
    - 'missing': File doesn't exist locally
    - 'protected': File is protected from updates
    - 'up_to_date': Files are identical
    - 'outdated': Local file is older than remote
    - 'locally_modified': Local file is newer than remote
    - 'differs': Files differ but can't determine which is newer
    - 'error': Unable to determine status
    """
    if not os.path.exists(file_path):
        return 'missing'
    
    if not can_update_file(file_path):
        return 'protected'
    
    local_hash = get_file_hash(file_path)
    remote_hash = get_remote_file_hash(file_path)
    
    if local_hash is None or remote_hash is None:
        return 'error'
    
    # If hashes are the same, files are identical
    if local_hash == remote_hash:
        return 'up_to_date'
    
    # Files are different, now check dates to determine which is newer
    local_date = get_local_file_modification_date(file_path)
    remote_date = get_file_last_commit_date(file_path)
    
    if local_date is None or remote_date is None:
        return 'differs'  # Can't determine which is newer
    
    # Compare dates (remove timezone info for comparison)
    if hasattr(remote_date, 'replace'):
        remote_date = remote_date.replace(tzinfo=None)
    
    if local_date < remote_date:
        return 'outdated'  # Local file is older
    elif local_date > remote_date:
        return 'locally_modified'  # Local file is newer
    else:
        return 'differs'  # Same date but different content

def check_outdated_files():
    """Check all files and return list categorized by status."""
    outdated_files = []
    locally_modified_files = []
    up_to_date_files = []
    protected_files = []
    missing_files = []
    differs_files = []
    error_files = []
    
    logging.info("Checking file status...")
    
    # Get files to check (either predefined or auto-discovered)
    files_to_check = get_files_to_check()
    
    if not files_to_check:
        logging.error("No files to check found!")
        return {
            'outdated': [],
            'locally_modified': [],
            'missing': [],
            'up_to_date': [],
            'protected': [],
            'differs': [],
            'errors': []
        }
    
    for file_path in files_to_check:
        status = check_file_status(file_path)
        
        if status == 'missing':
            missing_files.append(file_path)
            logging.info(f"Missing: {file_path}")
        elif status == 'protected':
            protected_files.append(file_path)
            logging.info(f"Protected: {file_path}")
        elif status == 'up_to_date':
            up_to_date_files.append(file_path)
            logging.info(f"Up-to-date: {file_path}")
        elif status == 'outdated':
            outdated_files.append(file_path)
            logging.info(f"Outdated: {file_path}")
        elif status == 'locally_modified':
            locally_modified_files.append(file_path)
            logging.info(f"Locally modified: {file_path}")
        elif status == 'differs':
            differs_files.append(file_path)
            logging.info(f"Differs from template: {file_path}")
        else:  # error
            error_files.append(file_path)
            logging.warning(f"Error checking: {file_path}")
    
    # Print summary
    print("\n" + "="*50)
    print("FILE STATUS SUMMARY")
    print("="*50)
    
    if outdated_files:
        print(f"\n📋 OUTDATED FILES (local older than GitHub) ({len(outdated_files)}):")
        for file in outdated_files:
            print(f"  - {file}")
    
    if locally_modified_files:
        print(f"\n🔄 LOCALLY MODIFIED FILES (local newer than GitHub) ({len(locally_modified_files)}):")
        for file in locally_modified_files:
            print(f"  - {file}")
    
    if differs_files:
        print(f"\n❓ FILES DIFFERENT FROM TEMPLATE (cannot determine age) ({len(differs_files)}):")
        for file in differs_files:
            print(f"  - {file}")
    
    if missing_files:
        print(f"\n❌ MISSING FILES ({len(missing_files)}):")
        for file in missing_files:
            print(f"  - {file}")
    
    if up_to_date_files:
        print(f"\n✅ UP-TO-DATE FILES ({len(up_to_date_files)}):")
        for file in up_to_date_files:
            print(f"  - {file}")
    
    if protected_files:
        print(f"\n🔒 PROTECTED FILES ({len(protected_files)}):")
        for file in protected_files:
            print(f"  - {file}")
    
    if error_files:
        print(f"\n⚠️  ERROR CHECKING ({len(error_files)}):")
        for file in error_files:
            print(f"  - {file}")
    
    print(f"\nTotal files checked: {len(files_to_check)}")
    print("="*50)
    
    return {
        'outdated': outdated_files,
        'locally_modified': locally_modified_files,
        'missing': missing_files,
        'up_to_date': up_to_date_files,
        'protected': protected_files,
        'differs': differs_files,
        'errors': error_files
    }

def main():
    # Check for command line arguments
    if len(sys.argv) > 1:
        if sys.argv[1] == "--check" or sys.argv[1] == "-c":
            # Only check for outdated files, don't update
            check_outdated_files()
            return
        elif sys.argv[1] == "--help" or sys.argv[1] == "-h":
            print("SolutionUpgrader.py - Update files from remote repository")
            print("\nUsage:")
            print("  python SolutionUpgrader.py --check                    # Only check for outdated files (SAFE)")
            print("  python SolutionUpgrader.py -c                         # Short version of --check")
            print("  python SolutionUpgrader.py --force-update             # Update all files (CREATES BACKUP)")
            print("  python SolutionUpgrader.py --update-file <filepath>   # Update specific file only")
            print("  python SolutionUpgrader.py --help                     # Show this help")
            print("\nExamples:")
            print("  python SolutionUpgrader.py --update-file README.md")
            print("  python SolutionUpgrader.py --update-file CMakeLists.txt")
            print("\nSecurity:")
            print("  - It is recommended to always run --check first")
            print("  - A backup will be created before updating")
            print("  - Files with <DOTNAME_NO_UPDATE> will not be overwritten")
            return
        elif sys.argv[1] == "--update-file":
            if len(sys.argv) < 3:
                print("❌ Error: Missing file path!")
                print("Usage: python SolutionUpgrader.py --update-file <filepath>")
                print("Example: python SolutionUpgrader.py --update-file README.md")
                return
            
            file_to_update = sys.argv[2]
            
            # Check if file can be updated
            if os.path.exists(file_to_update) and not can_update_file(file_to_update):
                print(f"🔒 File '{file_to_update}' is protected with <DOTNAME_NO_UPDATE> and cannot be updated.")
                return
            
            # Check if file exists in repository
            all_repo_files = get_all_files_from_repo()
            if file_to_update not in all_repo_files:
                print(f"❌ File '{file_to_update}' not found in repository.")
                print("Available files can be checked with: python SolutionUpgrader.py --check")
                return
            
            # Create backup directory
            backup_dir = create_backup_dir()
            
            print(f"🔄 Updating single file: {file_to_update}")
            
            if update_file(file_to_update, backup_dir):
                print(f"✅ Successfully updated: {file_to_update}")
                if backup_dir and os.path.exists(file_to_update):
                    print(f"📦 Backup created in: {backup_dir}")
                
                # Handle self-update - don't restart to avoid infinite loop
                if file_to_update == "SolutionUpgrader.py":
                    print("🔄 SolutionUpgrader.py has been updated!")
                    print("💡 Please run the script again to use the new version.")
                    return
            else:
                print(f"❌ Failed to update: {file_to_update}")
            return
        elif sys.argv[1] == "--force-update":
            # Pokračuj s aktualizací
            pass
        else:
            print("⚠️ SECURITY WARNING ⚠️")
            print("This script can overwrite files!")
            print("It is recommended to run: python SolutionUpgrader.py --check")
            print("To continue, use: python SolutionUpgrader.py --force-update")
            print("For help: python SolutionUpgrader.py --help")
            return
    else:
        print("⚠️  SECURITY WARNING ⚠️")
        print("This script can overwrite files!")
        print("It is recommended to run: python SolutionUpgrader.py --check")
        print("To continue, use: python SolutionUpgrader.py --force-update")
        print("For help: python SolutionUpgrader.py --help")
        return

    backup_dir = None

    # Get files to update (either predefined or auto-discovered)
    files_to_check = get_files_to_check()
    
    if not files_to_check:
        logging.error("No files to update found!")
        return

    for file_path in files_to_check:
        if os.path.exists(file_path):
            if can_update_file(file_path):
                logging.info(f"Updating: {file_path}")
                
                # Vytvoř backup_dir pouze když je potřeba zálohovat
                if file_path != "SolutionUpgrader.py" and backup_dir is None:
                    backup_dir = create_backup_dir()
                
                # Update souboru
                if update_file(file_path, backup_dir):
                    if file_path == "SolutionUpgrader.py":
                        print("🔄 SolutionUpgrader.py has been updated!")
                        print("💡 Please run the script again to use the new version.")
                        return
            else:
                logging.info(f"Skipped (protected): {file_path}")
        else:
            logging.info(f"Creating new file: {file_path}")
            if backup_dir is None:
                backup_dir = create_backup_dir()
            update_file(file_path, backup_dir)

if __name__ == "__main__":
    main()
    
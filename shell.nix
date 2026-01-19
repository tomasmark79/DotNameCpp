let
  pkgs = import <nixpkgs> {};
in pkgs.mkShell {
  packages = [
    pkgs.python3
  ];

  shellHook = ''
    # Create and activate virtual environment
    if [ ! -d .venv ]; then
      python -m venv .venv
    fi
    source .venv/bin/activate
    # Install required packages
    pip install --quiet conan cmake-format 2>/dev/null || true
  '';
}

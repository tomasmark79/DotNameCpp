{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            python3
            emscripten
          ];
          shellHook = ''
            if [ ! -d .venv ]; then
              python -m venv .venv
            fi
            source .venv/bin/activate
            pip install --quiet conan cmake-format 2>/dev/null || true

            # Set up Emscripten environment
            export EMSDK=${pkgs.emscripten}

            # Set up CPM cache directory
            export CPM_SOURCE_CACHE="$HOME/.cache/CPM"
            mkdir -p "$CPM_SOURCE_CACHE"
          '';
        };
      }
    );
}

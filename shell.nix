# Fallback for nix-shell (users without flakes)
(import (
  fetchTarball "https://github.com/edolstra/flake-compat/archive/master.tar.gz"
) { src = ./.; }).shellNix

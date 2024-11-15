# nix flake prefetch github:Bgabri/Themulator
{
  inputs = {
    flake-utils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };
  outputs =
    { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
        dontFixup = true;
        dontPatchShebangs = true;
        Themulator = pkgs.stdenv.mkDerivation {
          name = "thml";

          src = pkgs.fetchFromGitHub {
            owner = "Bgabri";
            repo = "Themulator";
            rev = "v0.2.1";
            sha256 = "sha256-KQOReco0uYIumt7pg939fVZ8jys1LlmoZp4VGVXTG0c=";
          };

          buildInputs = with pkgs; [
            gcc
            cmake
            curl
          ];

        };
      in
      with pkgs;
      {
        devShells.default = mkShell {
          nativeBuildInputs = [
            Themulator
            cmake
            curl
          ];
          shellHook = ''
            exec fish
          '';
        };
      }
    );
}

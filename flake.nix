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
            rev = "v0.2.0";
            sha256 = "sha256-sJge7Geybnvnwh5lizgXMtL0ESMq14bvWBVXC08zb9E=";
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

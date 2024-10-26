# nix flake prefetch github:Bgabri/Themulator
{
  inputs = {
    flake-utils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };
  outputs = { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        dontFixup = true;
        dontPatchShebangs = true;
        Themulator = pkgs.stdenv.mkDerivation {
          name = "thml";

          src = pkgs.fetchFromGitHub {
            owner = "Bgabri";
            repo = "Themulator";
            rev = "main";
            sha256 = "sha256-Iomn01miZE2s93ocDgAvw0jfvSlIaqMueJWqPpJ009E=";
          };
          
          buildInputs = with pkgs;[gcc cmake curl];

        };
      in
      with pkgs;
      {
        devShells.default = mkShell {
            nativeBuildInputs = [ Themulator cmake curl ];
          };
      });
}

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
          pname = "thml";
          version = "0.1.12";

          src = pkgs.fetchFromGitHub {
            owner = "Bgabri";
            repo = "Themulator";
            rev = "main";
            sha256 = "sha256-GInRtFu1gnj3mlOgyGd81f2sQYX7qSmJHJatA3nKFbI=";
          };
          
          buildInputs = with pkgs;[gcc cmake curl];

        };
      in
      with pkgs;
      {
        devShells.default = mkShell {
            nativeBuildInputs = [ Themulator ];
          };
      });
}

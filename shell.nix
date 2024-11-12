{
  pkgs ? import <nixpkgs> { },
}:
pkgs.mkShell {
  shellHook = ''
    exec fish
  '';
  buildInputs = [
    pkgs.curl
  ];
}

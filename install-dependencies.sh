#!/usr/bin/env bash
set -euo pipefail

have_cmd() {
  command -v "$1" >/dev/null 2>&1
}

if have_cmd apt-get; then
  PKG_MANAGER=apt
elif have_cmd pacman; then
  PKG_MANAGER=pacman
elif have_cmd dnf; then
  PKG_MANAGER=dnf
elif have_cmd zypper; then
  PKG_MANAGER=zypper
elif have_cmd brew; then
  PKG_MANAGER=brew
else
  echo "Unsupported system: need apt, pacman, dnf, zypper or brew." >&2
  exit 1
fi

echo "Detected package manager: $PKG_MANAGER"

case "$PKG_MANAGER" in
  apt)
    sudo apt-get update
    sudo apt-get install -y \
      build-essential \
      cmake \
      pkg-config \
      libglfw3-dev \
      libgl1-mesa-dev \
      libxrandr-dev \
      libxinerama-dev \
      libxcursor-dev \
      libxi-dev \
      libxxf86vm-dev \
      libgtk-3-dev
    ;;

  pacman)
    sudo pacman -Sy --needed \
      base-devel \
      cmake \
      pkgconf \
      glfw-wayland \
      mesa \
      libxrandr \
      libxinerama \
      libxcursor \
      libxi \
      libxxf86vm \
      gtk3
    ;;

  dnf)
    sudo dnf groupinstall -y "Development Tools"
    sudo dnf install -y \
      cmake \
      pkgconfig \
      glfw-devel \
      mesa-libGL-devel \
      libXrandr-devel \
      libXinerama-devel \
      libXcursor-devel \
      libXi-devel \
      libXxf86vm-devel \
      gtk3-devel
    ;;

  zypper)
    sudo zypper install -y -t pattern devel_basis
    sudo zypper install -y \
      cmake \
      pkg-config \
      glfw3-devel \
      Mesa-libGL-devel \
      libXrandr-devel \
      libXinerama-devel \
      libXcursor-devel \
      libXi-devel \
      libXxf86vm-devel \
      gtk3-devel
    ;;

  brew)
    brew update
    brew install \
      cmake \
      pkg-config \
      glfw \
      mesa \
      gtk+3
    ;;

  *)
    echo "Internal error: unhandled package manager '$PKG_MANAGER'" >&2
    exit 1
    ;;
esac

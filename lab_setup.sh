#!/usr/bin/env bash
set -e
sudo pacman -Syu --noconfirm
sudo pacman -S --needed --noconfirm git vim nano bash-completion base-devel python gcc openssh virtualbox clang
sudo usermod -aG vboxusers $USER || true


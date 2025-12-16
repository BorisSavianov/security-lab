#!/usr/bin/env bash
set -e
sudo pacman -Syu --noconfirm
sudo pacman -S --needed --noconfirm git vim nano bash-completion base-devel python gcc openssh virtualbox
sudo usermod -aG vboxusers $USER || true
mkdir -p ~/security-lab/week1 ~/security-lab/vm-snapshots
echo "Run 'source ~/.bashrc' after editing it. Reboot recommended to apply vboxusers group."



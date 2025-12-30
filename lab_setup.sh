#!/usr/bin/env bash
set -e
sudo pacman -Syu --noconfirm
sudo pacman -S --needed --noconfirm git vim nano bash-completion base-devel python gcc openssh virtualbox clang
sudo usermod -aG vboxusers $USER || true

# Install essential dev tools
sudo pacman -S --needed base-devel gcc make git gdb strace ltrace binutils

# Install python (for pwndbg setup)
sudo pacman -S --needed python python-pip

# Clone and install pwndbg (gdb plugin)
cd $HOME
git clone https://github.com/pwndbg/pwndbg.git
cd pwndbg
./setup.sh
# Follow on-screen prompts; it will install Python deps into your user pip.
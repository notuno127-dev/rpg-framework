# Project Instructions

## Project overview

This repository is a portfolio project named `rpg-framework`.

The project is intended to demonstrate:

- C++ programming ability
- SDL3-based application and UI development
- game framework architecture
- clear responsibility separation
- readable and explainable code
- CMake and Git-based development workflow

## Development environment

- OS: Windows 11
- Language: C++20
- Graphics and input library: SDL3
- Build system: CMake
- Build tool: Ninja
- Compiler: MSVC x64
- Package manager: vcpkg
- vcpkg root: `D:/Dev/vcpkg`
- vcpkg triplet: `x64-windows`
- Project root: `D:/Dev/Projects/rpg-framework`
- Build directory: `build`

## Build commands

Configure:

```powershell
cmake -S . -B build -G Ninja `
  -DCMAKE_TOOLCHAIN_FILE=D:/Dev/vcpkg/scripts/buildsystems/vcpkg.cmake `
  -DVCPKG_TARGET_TRIPLET=x64-windows

# AGENTS.md

## Project overview

`rpg-framework` is a personal C++ game framework project built with SDL3.

The project is intended to provide reusable systems for developing 2D RPG-style games, including rendering, input, scenes, UI, resources, and game-state management.

When modifying this repository:

* Prefer simple and maintainable implementations.
* Avoid unnecessary abstractions.
* Preserve existing behavior unless the task explicitly requests a change.
* Keep the project buildable after every change.
* Make changes small, focused, and easy to review.

---

## Development environment

The primary development environment is:

* OS: Windows
* Editor: Visual Studio Code
* Language: C++
* Build system: CMake
* Dependency manager: vcpkg
* Graphics and platform library: SDL3
* vcpkg triplet: `x64-windows`

The expected vcpkg environment variable is:

```powershell
$env:VCPKG_ROOT = "D:/Dev/vcpkg"
```

Do not assume that Visual Studio project files are edited manually.
CMake files are the source of truth for build configuration.

---

## Repository structure

Before making changes, inspect the repository structure and the following files when present:

* `CMakeLists.txt`
* `CMakePresets.json`
* `vcpkg.json`
* `README.md`
* `src/`
* `include/`
* `tests/`
* `.vscode/`

Expected responsibilities:

* `src/`: implementation files
* `include/`: public or shared header files
* `tests/`: automated tests
* `assets/`: runtime game assets
* `.vscode/`: VS Code build, launch, and task configuration

Do not reorganize directories unless the task explicitly requires it.

---

## Before editing

Before changing files:

1. Read the root `CMakeLists.txt`.
2. Check whether `CMakePresets.json` exists.
3. Check whether dependencies are declared in `vcpkg.json`.
4. Inspect related headers and implementations.
5. Search for existing utilities before adding new ones.
6. Check `git status` to avoid overwriting unrelated user changes.

Do not modify files unrelated to the requested task.

Do not discard or revert existing user changes.

---

## C++ standard

Use the C++ standard specified by `CMakeLists.txt`.

If no standard is explicitly specified, use C++20:

```cmake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
```

Do not introduce a newer language standard without explaining why it is required.

---

## Coding conventions

Follow the conventions already present in nearby files.

When no existing convention is clear, use the following defaults:

### Naming

* Classes and structs: `PascalCase`
* Public member functions: `camelCase`
* Local variables: `camelCase`
* Function parameters: `camelCase`
* Private data members: `m_memberName`
* Constants: `kPascalCase`
* Namespaces: lowercase
* Source files: match the main class name

Examples:

```cpp
class SceneManager
{
public:
    void changeScene();

private:
    int m_currentSceneId = 0;
};
```

### General rules

* Use `nullptr`, not `NULL` or `0`.
* Prefer scoped enums with `enum class`.
* Prefer RAII for resource management.
* Prefer stack allocation when ownership does not require dynamic allocation.
* Use smart pointers instead of raw owning pointers.
* Raw pointers may be used only as non-owning references when appropriate.
* Mark methods `const` when they do not modify object state.
* Use `override` for overridden virtual methods.
* Use `explicit` for single-argument constructors when implicit conversion is undesirable.
* Avoid global mutable state.
* Avoid macros unless required by a library or platform API.
* Avoid unnecessary comments that merely repeat the code.
* Add comments for design intent, constraints, and non-obvious behavior.

---

## Header files

Use `#pragma once` unless the existing project consistently uses include guards.

Header files should contain only the includes they require.

Prefer forward declarations when they reduce unnecessary dependencies and do not make the code less clear.

Do not put large non-template implementations in header files.

Keep public interfaces small.

---

## SDL3 rules

Use SDL3 APIs, not SDL2 APIs.

Do not copy SDL2 examples without confirming that the functions, constants, event structures, and return-value conventions are valid for the installed SDL3 version.

Include SDL3 using:

```cpp
#include <SDL3/SDL.h>
```

Prefer CMake package integration:

```cmake
find_package(SDL3 CONFIG REQUIRED)

target_link_libraries(rpg-framework
    PRIVATE
        SDL3::SDL3
)
```

Do not hard-code SDL library or include-directory paths when the dependency can be resolved through vcpkg and CMake.

Check return values from SDL initialization and resource-creation functions.

When an SDL operation fails, include `SDL_GetError()` in the error output where applicable.

SDL resources must be released using the matching SDL destruction function.

Examples include:

* `SDL_DestroyWindow`
* `SDL_DestroyRenderer`
* `SDL_DestroyTexture`
* `SDL_Quit`

Prefer RAII wrappers when ownership becomes complex.

---

## CMake rules

Use target-based CMake.

Prefer:

```cmake
target_sources(...)
target_include_directories(...)
target_link_libraries(...)
target_compile_features(...)
target_compile_options(...)
```

Avoid global commands such as:

```cmake
include_directories(...)
link_directories(...)
add_definitions(...)
```

Do not hard-code developer-specific absolute paths in `CMakeLists.txt`.

Use the `VCPKG_ROOT` environment variable or a CMake preset for the vcpkg toolchain.

Keep target names consistent. The primary executable target should remain:

```text
rpg-framework
```

When adding a source file, make sure it is included in the appropriate CMake target.

Do not use recursive `file(GLOB_RECURSE ...)` unless the project already intentionally uses it.

---

## vcpkg rules

Dependencies should preferably be declared in `vcpkg.json`.

Example:

```json
{
  "name": "rpg-framework",
  "version-string": "0.1.0",
  "dependencies": [
    "sdl3"
  ]
}
```

Do not add dependencies unless they provide clear value.

Before adding a library:

1. Check whether the functionality can be implemented reasonably with the standard library or existing dependencies.
2. Explain why the dependency is needed.
3. Add it to `vcpkg.json`.
4. Update CMake configuration.
5. Verify a clean configure and build.

Do not edit files under the vcpkg installation directory.

---

## Configure and build

Prefer CMake presets when `CMakePresets.json` exists.

Example:

```powershell
cmake --preset windows-debug
cmake --build --preset windows-debug
```

If no preset exists, configure the project from the repository root with:

```powershell
cmake -S . -B build `
  -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" `
  -DVCPKG_TARGET_TRIPLET=x64-windows
```

Build the Debug configuration with:

```powershell
cmake --build build --config Debug
```

Build the Release configuration with:

```powershell
cmake --build build --config Release
```

Do not report that a change is complete unless the relevant build command has been run, unless the environment prevents it.

---

## Clean configuration

CMake caches package and toolchain information.

After changing any of the following, perform a clean configuration:

* vcpkg triplet
* generator
* compiler
* toolchain file
* dependency versions
* SDL package configuration
* CMake presets

PowerShell command:

```powershell
Remove-Item -Recurse -Force build
```

Then configure and build again.

Do not attempt to solve package-architecture or stale-cache errors only by repeatedly running the same build command.

---

## Running the application

After a successful Debug build, the executable is normally located at one of these paths, depending on the CMake generator:

```text
build/Debug/rpg-framework.exe
```

or:

```text
build/rpg-framework.exe
```

Run it from the repository root when the application uses paths relative to the project directory.

PowerShell examples:

```powershell
& ".\build\Debug\rpg-framework.exe"
```

or:

```powershell
& ".\build\rpg-framework.exe"
```

If the executable location differs, inspect the CMake build output instead of guessing.

When adding or changing runtime assets, verify that the executable can locate them from the documented working directory.

---

## VS Code integration

When modifying `.vscode/tasks.json` or `.vscode/launch.json`:

* Keep build and launch configurations portable.
* Prefer `${workspaceFolder}` over absolute paths.
* Keep build tasks separate from launch configurations.
* Make the launch configuration depend on a build task.
* Set `cwd` to `${workspaceFolder}` when assets use repository-relative paths.

Recommended task names:

* `CMake: Configure`
* `CMake: Build Debug`
* `CMake: Build Release`
* `Run rpg-framework`

Do not overwrite unrelated existing VS Code configurations.

---

## Error handling

Do not silently ignore errors.

For recoverable errors:

* Return an error result or status.
* Log sufficient context.
* Preserve the original failure information.

For fatal initialization errors:

* Print a clear message.
* Include relevant SDL or system error details.
* Release resources already acquired.
* Exit cleanly.

Avoid empty `catch` blocks.

Do not introduce exceptions into code that consistently uses result values unless the change is intentional and documented.

---

## Logging

Use the existing logging mechanism when one is available.

When no logging mechanism exists:

* Use `SDL_Log` for SDL-related runtime information.
* Use `std::cerr` for startup or build-independent errors.
* Avoid excessive per-frame logging.
* Do not leave temporary debug output in completed changes.

Never log passwords, tokens, credentials, or private environment values.

---

## Architecture guidelines

Keep platform-specific and SDL-specific code separated from game-domain logic where practical.

Prefer clear ownership boundaries for:

* windows
* renderers
* textures
* scenes
* resources
* input state
* audio resources

Suggested dependency direction:

```text
Application
    -> Scene/Game systems
        -> Rendering/Input abstractions
            -> SDL3
```

Higher-level game logic should not depend directly on SDL3 when a small project abstraction already exists.

Do not introduce an abstraction solely for hypothetical future use.

---

## UI implementation

For UI-related work:

* Separate layout, state, input handling, and rendering where practical.
* Avoid hard-coding the same coordinates in multiple locations.
* Use named constants or layout structures for repeated dimensions.
* Handle window resizing when the existing design supports it.
* Keep keyboard, mouse, and controller input paths consistent.
* Ensure focused and selected elements have visible state changes.
* Avoid embedding game-state changes directly in low-level rendering code.

---

## Resource management

Each resource must have one clear owner.

Document ownership when it is not obvious.

Do not allow SDL resources to outlive the SDL subsystem they depend on.

When loading resources:

* Check whether loading succeeded.
* Report the resource path on failure.
* Avoid loading the same resource repeatedly each frame.
* Cache resources only when the ownership and invalidation rules are clear.

---

## Testing and validation

After modifying C++ or CMake files:

1. Configure the project.
2. Build the Debug configuration.
3. Run automated tests when present.
4. Start the application when practical.
5. Check the relevant changed behavior.
6. Review compiler warnings.
7. Check `git diff`.
8. Check `git status`.

If CTest is configured, run:

```powershell
ctest --test-dir build -C Debug --output-on-failure
```

For Release validation, use:

```powershell
ctest --test-dir build -C Release --output-on-failure
```

Do not claim that tests passed unless they were actually executed.

When a test or build cannot be executed, clearly state:

* which command was not run
* why it could not be run
* what remains unverified

---

## Compiler warnings

Do not introduce new compiler warnings.

When adding warning options, apply them per target.

For MSVC, preferred warnings are:

```cmake
target_compile_options(rpg-framework PRIVATE /W4 /permissive-)
```

Do not enable warnings-as-errors for the entire project without considering warnings originating from external libraries.

---

## Formatting

Use the existing `.clang-format` configuration when present.

When no configuration exists:

* Match formatting in nearby files.
* Use four spaces for indentation.
* Do not use tabs for indentation.
* Keep braces consistent with the surrounding code.
* Avoid unrelated formatting-only changes.

Do not reformat entire files when changing only a small section.

---

## Git rules

Before editing:

```powershell
git status
```

After editing:

```powershell
git diff
git status
```

Rules:

* Do not overwrite unrelated user changes.
* Do not run destructive Git commands without explicit permission.
* Do not use `git reset --hard`.
* Do not use `git clean -fd`.
* Do not force-push.
* Do not rewrite existing commits.
* Do not commit generated build files.
* Do not add the `build/` directory to Git.
* Do not commit IDE-local or machine-specific files unless intentionally shared.

Do not create a commit unless the user explicitly requests one.

---

## Scope control

Make the smallest change that fully solves the task.

Do not:

* refactor unrelated code
* rename unrelated files or symbols
* change formatting across unrelated areas
* add speculative features
* replace existing architecture without justification
* modify public APIs unnecessarily
* introduce dependencies for trivial functionality

If a broader refactor is necessary, explain the reason before or alongside the implementation.

---

## Documentation

Update documentation when a change affects:

* build commands
* required dependencies
* environment setup
* controls
* runtime behavior
* project structure
* public interfaces

Keep documentation commands executable and synchronized with the actual project.

Do not duplicate large amounts of documentation in `AGENTS.md`.

Place detailed architectural or design documentation in files such as:

```text
README.md
ARCHITECTURE.md
docs/
```

Use `AGENTS.md` primarily as an operational guide and a map to those documents.

---

## Security

Never add or expose:

* API keys
* access tokens
* passwords
* private certificates
* local credentials
* personal absolute paths unless explicitly required for local documentation

Use environment variables for secrets and machine-specific configuration.

Do not execute downloaded scripts or unknown binaries without explicit user approval.

---

## Completion report

After completing a task, provide a concise report containing:

1. Summary of the changes.
2. Important implementation decisions.
3. Files changed.
4. Build commands executed.
5. Tests executed.
6. Results of build and tests.
7. Remaining limitations or unverified items.

Do not say the task is fully complete when build or validation failures remain.

---

## Priority order

When instructions conflict, use this priority order:

1. Explicit instructions in the current user request
2. The closest applicable nested `AGENTS.md`
3. This root `AGENTS.md`
4. Existing project conventions
5. General conventions in this document

A more deeply nested `AGENTS.md` applies to its own directory tree and may override this file for that scope.


# How to add a new component

1. Firstly a new component (e.g. component1/) folder under the components/ folder must be added.
2. Inside that folder a docs/, include/, src/ and optionally an examples/ folder has to be inserted.
3. The docs/ folder should contain the documentation about this component, at least a README and a CHANGELOG.
4. The include/ folder must contain the header files and the src/ folder the source files.
5. A CMakeLists.txt with the following content must also be included to the examples/ folder:
   - `idf_component_register(SRCS "src/component1.cpp" INCLUDE_DIRS "include")`
    or if another component is required by this component:
   - `idf_component_register(SRCS "src/component1.cpp" INCLUDE_DIRS "include" REQUIRES componentX)`.
    If more than one source file is inside the src/ folder the following must be inserted instead:
   - `FILE(GLOB_RECURSE component_srcs src/*.*)` and
   - `idf_component_register(SRCS ${component_srcs} INCLUDE_DIRS "include")`
6. Lastly the component must be included in the CMakeLists.txt of the top-level src/ folder to use
   it in the main application: `idf_component_register(SRCS ${app_sources} REQUIRES component1 ...)`
7. Examples demonstrating the basic usage of the component can also be added to the examples/ folder.

## How to add executable example files for a component

The example(s) are intended as a starting point for experimenting with the component and
understanding its basic operations.

1. Inside the components/ folder a src/ folder with a `main.cpp` must be inserted.
2. In the top-level `platform.ini` file a new environment (e.g. example_component) has to be added
    which needs to have an extra cmake argument with the name of the environment (see section "New
    environment in platformio.ini").
3. In the top-level CMakeLists.txt an if statement needs to be added to check if the example
   environment is selected (see section "Additional if-statement in top-level CMakeLists.txt").
4. In the bottom status bar, the correct environment has to be selected.
5. Now any action (PlatformIO: Build, Upload, etc.) is executed on that environment.

## New environment in platformio.ini

```bash
[env:example_component]
board_build.cmake_extra_args =
    -DPRODUCT=example_component
```

## Additional if-statement in top-level CMakeLists.txt

```bash
if(PRODUCT STREQUAL "example_component") # or: elseif(PRODUCT STREQUAL "example_component") 
                                         # if its not the very first example
    # Example main
    set(MAIN_SRC ${CMAKE_SOURCE_DIR}/components/component1/examples/example_one/src/main.cpp)

    # If the example has headers next to its main, add that include dir
    set(EXTRA_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/components/component1/examples/example_one/include)
```

### Example directory structure

```bash
smart-light/
├── components/ ........................... # Project components
│  ├── common/ ............................ # Common component for shared types and utilities
│  │  ├── include/ ........................ # Common component header files
│  │  │  └── common_types.hpp ............. # Common types used across the project
│  │  └── CMakeLists.txt .................. # CMakeLists.txt for common component
│  ├── component1/ ........................ # Each component has its own folder
│  │  ├── docs/ ........................... # Documentation for this component
│  │  │  ├── images/ ...................... #
│  │  │  ├── README ....................... #
│  │  │  └── CHANGELOG .................... # Changelog
│  │  ├── include/ ........................ # Component header files
│  │  ├── src/ ............................ # Component source files
│  │  ├── examples/ ....................... # Basic usage examples
│  │  │  ├── example_one/ ................. # Optionally if more than one example
│  │  │  │  ├── src/ ...................... #
│  │  │  │  │  └──  main.cpp .............. #
│  │  └── CMakeLists.txt .................. # Component-level CMakeLists.txt
```

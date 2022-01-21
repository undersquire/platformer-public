**NOTE:** This project is not a good example of "professional" code. I wrote this in 2 weeks for a school project, so don't expect the cleanest or most efficient code.

**Project Information:**

Libraries used:

- SDL
- stb_image

Compiling this project (requires CMake 3.22+):

- Clone/download the project files
- Initialize CMake build directory - `cmake -S . -B build`
- Build the project via CMake - `cmake --build build`

**IMPORTANT**: The executable (now in the build directory) will **NOT** run unless it finds the `res` folder in the same directory. Either move the executable into the root project directory, or copy the `res` folder into the `build` directory.
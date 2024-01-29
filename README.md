**NOTE:** This project is not a good example of "professional" code. I wrote this in 2 weeks for a school project, so don't expect the cleanest or most efficient code.

**NOTE 2:** The art is not mine, I got it from [OpenGameArt](https://www.opengameart.org/). I just combined it all into a single PNG for easier asset management 🤓

**Project Information:**

Libraries used:

- SDL2
- stb_image

Compiling this project (requires CMake 3.25+):

- Clone the repository - `git clone https://www.github.com/undersquire/platformer-public --recursive`

**macOS, Linux** (requires Ninja):

- Initialize CMake build directory - `cmake -B build -G "Ninja Multi-Config" .`
- Build the project via CMake - `cmake --build build --config Release`

**Windows** (requires Visual C++ Build Tools):

- Initialize CMake build directory - `cmake -B build -G "Visual Studio 17 2022" -A x64 .`
- Build the project via VS Build - `cmake --build build --config Release`

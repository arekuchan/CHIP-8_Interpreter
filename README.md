Hi, I'm working on a Chip-8 emulator. This project isn't functional yet but thanks for stopping by. I'm just doing this to learn computer architecture and (modern) c++. As this is a personal project, I'm currently not accepting contributions, however, if you want to review my code, I'm all ears!

I'm using c++20, and are developing on Ubuntu 22.04.3 LTS on WSL 1.

Dependicies:
- Eigen 3
- Yaml-cpp
- SDL2

Having trouble with running the project on WSL? You probably need to install Xserver and configure enviornment variables for the emulator display window to work

Ramblings below:

What's the difference between my emulator and everyone elses'? Probably not much. Current plans are to get variable display resolution working so you can set the window size to your heart's desires. Also I'm trying to utilise modern C++ features appropriately, as well as write high-level C++ code that adheres to good SWE design principles. If you're looking for a reference implementation, hopefully this is a good one that is straightforward to understand.



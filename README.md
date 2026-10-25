## Requirements

- A C++17-compatible compiler
- CMake
- An internet connection is needed only for the first configure of the SFML graphical version, so CMake can download SFML 3.1.0


## Build and Run

Run these commands from the `Sliding-Number-Puzzle` folder:

```powershell
cmake -S . -B build
cmake --build build
.\build\SlidingNumberGame.exe
```

### Console-only version

This build does not require SFML or an internet connection. On Windows with MinGW installed:

```powershell
cmake -S . -B build-console -G "MinGW Makefiles" -DBUILD_SFML_GUI=OFF
cmake --build build-console --target SlidingNumberPuzzleConsole
.\build-console\SlidingNumberPuzzleConsole.exe
```



## Controls

- Arrow keys or WASD :- move a tile
- H :- show the A* solver’s suggested move
- Enter :- play the A* solution
- R :- restart the puzzle
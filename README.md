## Requirements

- A C++17-compatible compiler
- CMake
- An internet connection for the first configure, so CMake can download SFML 3.1.0


## Build and Run

Run these commands from the `Sliding-Number-Puzzle` folder:

```powershell
cmake -S . -B build
cmake --build build
.\build\SlidingNumberGame.exe
```


## Controls

- Arrow keys or WASD: move a tile
- H: show the A* solver’s suggested move
- Enter: play the A* solution
- R: restart the puzzle
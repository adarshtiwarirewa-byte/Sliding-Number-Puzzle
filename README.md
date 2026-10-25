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

### Console version

After the configure step above, build and run the console game:

```powershell
cmake --build build --target SlidingNumberPuzzleConsole
.\build\SlidingNumberPuzzleConsole.exe
```



## Controls

- Arrow keys or WASD: move a tile
- H: show the A* solver’s suggested move
- Enter: play the A* solution
- R: restart the puzzle
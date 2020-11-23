## minesweeper

This is a WINAPI implementation of [Minesweeper](https://en.wikipedia.org/wiki/Minesweeper_(video_game)).
You can choose Easy, Medium or Hardcore mode.

## Installer

TODO: add link
You can download the [installation file]().

## Requirements

* CMake
* MinGW
* GCC 

## How to build project

```
git clone https://github.com/iskislamov/minesweeper.git

cd ./minesweeper

cmake -G "MinGW Makefiles" -B build -S .

cmake --build ./build

./build/miner.exe
```
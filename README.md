## minesweeper

This is a WINAPI implementation of [Minesweeper](https://en.wikipedia.org/wiki/Minesweeper_(video_game)).

You can choose Easy, Medium or Hardcore mode.

## Installer

You can download the [installation file](https://drive.google.com/file/d/1xUM6SjUwFL_u62Ulnx1x3HaCtipcUR-I/view?usp=sharing).

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
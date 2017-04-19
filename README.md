# Project for database technology
  
## Installation
   * Edit the makefile and change `CXX` to your prefered C++ compiler (clang, gcc)
   * Run `make` to compile
   * `./dbms help` for quick info

## Usage

### `./dbms create filename #blocks`

This will create a binary file with that many blocks, named filename.

### `./dmbs print filename`

This will print the whole file named filename, record by record and block by block.

     * If you want to print a single block use `./dmbs print filename b #blockid`
     * If you want to print a single record use `./dmbs print filename r #recid`
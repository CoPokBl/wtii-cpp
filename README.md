# WTII C++
This is an implementation of the WTII interpreter in C++.
NOTE that this is just the interpreter not the parser, to run a script with this
you currently need to parse it using [the C# implementation](https://github.com/CoPokBl/wtii-lang).

## Usage
To run a parsed script (in the form of a JSON file) you can run the following command:
```sh
./WtiiInterpreter <path to the JSON file>
```
Omitting the path will result in the interpreter reading from `script_parsed.json`.

## Building
To build the interpreter you need to have [CMake](https://cmake.org/) installed.
Then you can run the following commands:
```sh
mkdir build
cd build
cmake ..
make
```
One liner:
```sh
mkdir build && cd build && cmake .. && make
```
CMake must be at least version 3.22.0. If you have an older version, you can try to change the `cmake_minimum_required` in `CMakeLists.txt` to your version. 
But I can't guarantee that it will work.

For a debug build do the following:
```sh
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```
One liner:
```sh
mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make
```

## Dependencies
The only dependency is [nlohmann/json](https://github.com/nlohmann/json) which is included in the repository
at `/nlohmann/` and is used to parse the JSON files. You don't need to install it separately.

## Contributing
If you want to contribute to this project, feel free to open a pull request.
Currently, the code style is all over the place, so try to stick to the existing style
but don't worry about it too much. I will try to clean it up at some point.

## TODO
The final goal is to have this as a full implementation of WTII to replace the
C# implementation. This means that the following needs to be done:
- [ ] Add a parser
- [ ] Write all the builtins that exist in the C# implementation
- [ ] Add support for wtii libraries, c# libraries and c++ libraries

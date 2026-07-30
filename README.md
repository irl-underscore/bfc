# Bfc
_Short for brainfuck compiler._ </br>
A brainfuck AOT compiler turning brainfuck code into GAS assembly.</br>
## Usage
```
bfc [flags] <source_file>
-h --help                   print help message
-v --version                print version info
-p --tape <size>            change tape size
-t --target <arcitecture>   change target arcitecture
-o --output <file_name>     declare output file (default to a.s)
```
### Supported arcitectures
- linux x86_64 (x86_64_linux)
- linux x86 (x86_linux)
> [!WARNING]
> linux x86 is not fully supported yet.
## Installation
Download the sourcecode via this command:
```
$ git clone https://github.com/irl-underscore/brainfuck.git
```
or get a compressed [copy](https://github.com/irl-underscore/brainfuck/archive/master.zip) of the code
## Building
Bfc requires make and a C compiler (e.g. clang or GCC)
If you are running clang, to build simply run:
```
$ make rel
```
else use this command instead:
```
$ make rel CC=<your_compiler>
```
and the compiled code will end up under `release/bfc`
## TODO
- Complete x86_linux support and add support for arm_linux and arm64_linux
- Add testing
- Add consistent and readable naming
## License 
This code is released under the Apache License v2.0, see [LICENSE](/LICENSE)
## Contributors
```
irl-underscore https://github.com/irl-underscore
```

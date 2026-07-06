# Brainfuck
A brainfuck AOT compiler turning brainfuck code into GAS assembly.</br>
## Usage
```
brainfuck [flags] <source_file>
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
## TODO
- Complete x86_linux support and add support for arm_linux and arm64_linux
- Add proper error system
- Add table mangement for assembler
## License 
This code is released under the Apache License v2.0, see [LICENSE](/LICENSE)
## Contributors
```
irl-underscore https://github.com/irl-underscore
```

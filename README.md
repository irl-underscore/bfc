# Brainfuck
A brainfuck AOT turning brainfuck code into GAS assembly.</br>
## Usage
```
  brainfuck [flags] <source_file>
  -h --help   print help message
  -v --version   print version info
  -p --tape <size>   change tape size
  -t --target <arcitecture>   change target arcitecture for the output
  -o --output <file_name>   declare the output file (default to a.s)
```
### Supported arcitectures
- linux x86_64 (x86_64_linux)
- linux x86 (x86_linux)
> [!WARNING]
> currently only x86_64_linux is fully supported. </br>
> Planning on adding full support for x86_linux, arm_linux and arm64_linux.
## License 
This code is released under the Apache License v2.0, see [LICENSE](/LICENSE)
## Contributors
```
  irl-underscore https://github.com/irl-underscore
```

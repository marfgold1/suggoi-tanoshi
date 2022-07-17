# Memory Editor
## Requirements
- GNU C++ Compiler (g++)
- Windows OS
## Builds
Before build, **make sure your current working directory is in this folder (the same as readme.md).**
### Target Program
You can build the target program by compile it with this command:
```sh
g++ -o targetprogram targetprogram.cpp
```
There will be a file named `targetprogram.exe` in your cwd for use.
### Memory Editor
You can build the memory editor by compile it with this command:
```sh
g++ -o memoryeditor memoryeditor.cpp -lpsapi
```
There will be a file named `memoryeditor.exe` in your cwd for use.
## Important Information
### Memory Type
There are 7 types of memory,   
|No|Type|
|--|----|
|0|string|
|1|long|
|2|unsigned long|
|3|long long|
|4|unsigned long long|
|5|float|
|6|double|

Enter the `No` of the corresponding type that you want to use when you are asked for the type of memory.
> If you enter `0` (string), it will ask for the size of the string. Enter the length of the string you want to access.
## Usage
### Target Program
Run your target program by double click `targetprogram.exe`. Leave it there and use memory program to manipulate target program's memory.
### Memory Editor
Run your memory editor by double click `memoryeditor.exe`. You can now edit any program memory. This example will guide you to hack the target program by modifying its memory:
1. Get the PID of the target program (mode 1).
2. Set the process to edit with the PID of the target program retrieved from step 1 (mode 2 > enter target program PID).
3. If you want to read the memory, enter mode 3:
    1.  Enter the memory address you want to access in hex, for example, 0x661610 or 0x0000000000661610 or 661610 (they all equivalent each other).
    2. Enter the type of memory to be accessed. For string, you also need to enter the length of the string. See [Memory Type](#memory-type) for reference.
    3. It will tell you the value of the memory in address you entered at step 3.1. as hexadecimal number, decimal (base 10) number, and ASCII string, plus the size of the memory you accessed.
    Example would be
        ```
        Memory at address 0x0000000000661610 in process PID 13532 is 'aaaaaaaaaaaaaa' (hex) 'aaaaaaaaaaaaaa' (dec) 'aaaaaaaaaaaaaa' (string) [15 bytes]
        ```
4. If you want to write to the memory, enter mode 4:
    1.  Enter the memory address you want to write in hex, for example, 0x661610 or 0x0000000000661610 or 661610 (they all equivalent each other).
    2. Enter the type of memory to be written. For string, you also need to enter the length of the string. See [Memory Type](#memory-type) for reference.
    3. If the type is integer (long, unsigned long, long long, or unsigned long long), enter the base of value you want to enter (hex/base 16 or decimal/base 10), then enter the value in the base you select. If not, you just have to enter the value you want to write (float, double, or string).
    3. It will tell you if the write is succeeded.
    Example would be
        ```
        Successfully writing 15 bytes memory at address 0x0000000000661610 in process PID 13532
        ```


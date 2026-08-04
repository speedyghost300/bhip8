# bhip8

This is a CHIP8 emulator I wrote in C. It is incomplete as of right now, but it can run the IBM logo file commonly used to test the most important opcodes.

## How to build

You will need CMake (at least v3.2.4) and a compiler compatible with C11. Use CMake to build the source code, then place any ROMs you would like to use in the same folder as bhip8.exe. When you run the .exe, it will ask you to enter the filename of the ROM you want to load. You must include the .ch8 extension.

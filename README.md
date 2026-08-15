# bhip8

This is a CHIP8 emulator I wrote in C. It is imperfect, but it should be able to run most ROM files. 

## How to build

You will need CMake (at least v3.2.4) and a compiler compatible with C11. Use CMake to build the source code, then place any ROMs you would like to use in the same folder as bhip8.exe. When you run the .exe, it will ask you to enter the filename of the ROM you want to load. You must include the .ch8 extension.

## Controls

The left side of the keyboard is used as the keypad. The table below is a visual reference that includes the corresponding hex codes for each key.

|											|   
|-------------------------------------------|   
| 1 (0x01) | 2 (0x02) | 3 (0x03) | 4 (0x0C) |   
| Q (0x04) | W (0x05) | E (0x06) | R (0x0D) |
| A (0x07) | S (0x08) | D (0x09) | F (0x0E) |
| Z (0x0A) | X (0x00) | C (0x0B) | V (0x0F) |

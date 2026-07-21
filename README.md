```
                             __
  ____________  ______ ___  / /_ 
 / ___/ ___/ / / / __ `__ \/ __ \
/ /__/ /  / /_/ / / / / / / /_/ /
\___/_/   \__,_/_/ /_/ /_/_.___/ 
                                 
```
# Crumb
> Crumb is a CHIP-8 emulator written in C from scratch.

![Version](https://img.shields.io/badge/version-1.0.0-blue)
![Language](https://img.shields.io/badge/language-C-informational)
![License](https://img.shields.io/badge/license-MIT-green)

## Summary

Crumb is a CHIP-8 interpreter built entirely in C, implementing the fetch-decode-execute cycle at the heart of every emulator. Crumb models the CHIP-8 virtual machine directly, including registers, memory, stack, timers, and a 64x32 monochrome display, and executes original CHIP-8 ROMs instruction by instruction with an SDL2 frontend for display and input.

This project was built to develop an understanding of how emulators work at a low level, examining opcode decoding, memory-mapped fonts, and the interpreter pattern that CHIP-8 (and by extension most virtual machines) is built on.

## Features

- Full CHIP-8 CPU core: 16 general-purpose registers, 4KB memory, 16-level stack, index register, and program counter
- Complete instruction set: all 35 CHIP-8 opcodes implemented (`0x0`-`0xF`), dispatched through a function-pointer opcode table
- ROM loading into memory starting at `0x200`
- Built-in fontset loaded into memory at `0x50` for the `FX29` sprite-character opcode
- Delay and sound timers that decrement each cycle
- `DXYN` sprite drawing with XOR pixel logic and collision (`VF`) detection into an internal framebuffer
- Keypad state array driving the `EX9E`/`EXA1`/`FX0A` key-input opcodes
- SDL2 window, renderer, and streaming texture that present the framebuffer each frame
- Main emulation loop tying input polling, `cpuCycle()`, and rendering together, with a configurable per-cycle delay
- Host keyboard mapped to the CHIP-8 hex keypad (`1234`/`QWER`/`ASDF`/`ZXCV`), plus `Esc` to quit

## Not Yet Implemented

- **Audio output**: `soundTimer` counts down, but no beep/tone is ever played

## Process

Once a ROM is loaded, `main.c` drives the emulation loop: poll input, run a CPU cycle, render the frame, repeat until the window is closed.

```
ROM → loadROM (cpu.c) → Fetch (cpuCycle) → Decode (op_table) → Execute (opXXXX)
                                                                        ↓
                                                          Mutate registers / memory / screen
                                                                        ↓
                                                        platformRender draws the framebuffer
```

Each cycle, `cpuCycle()` fetches the next 2-byte opcode from memory at `pc`, advances the program counter, dispatches to the matching handler via `op_table` (indexed by the opcode's top nibble), and ticks the delay/sound timers. `platformProcessInput()` polls SDL events into `cpu->keys` each frame, and `platformRender()` blits the `screen` framebuffer to the window via an SDL texture.

## Project Structure

```
crumb/
├── main.c          # Entry point; parses CLI args, inits CPU/platform, runs the main loop
├── cpu.c/h         # CPU state, opcode table, and all instruction handlers
├── platform.c/h    # SDL2 window/renderer setup, rendering, and input polling
└── CMakeLists.txt
```

## Build & Run

**Requirements:** CMake 3.20+, a C11-compatible compiler (GCC or Clang), SDL2

```bash
# Clone the repo
git clone https://github.com/carson-js/Crumb.git
cd Crumb

# Build
cmake -B cmake-build-debug
cmake --build cmake-build-debug

# Run
./cmake-build-debug/Crumb <scale> <delay> <rom-path>
```

`scale` sets the window size as a multiple of the 64x32 CHIP-8 display; `delay` is the number of milliseconds to wait between cycles. A few ROMs are included under [`roms/`](roms) to try out.

## Keypad

CHIP-8's hex keypad is mapped onto the host keyboard:

```
CHIP-8         Keyboard
1 2 3 C        1 2 3 4
4 5 6 D   →    Q W E R
7 8 9 E        A S D F
A 0 B F        Z X C V
```

`Esc` quits the emulator.

## Planned Features

- Audio beep while `soundTimer` is active
- Debug mode for stepping through opcodes and inspecting registers

## Author

Built by Carson Smith - [github.com/carson-js](https://github.com/carson-js) · [carsonjs.me](https://carsonjs.me)

## License

MIT License, see [LICENSE](LICENSE) for details.

```
                             __
  ____________  ______ ___  / /_ 
 / ___/ ___/ / / / __ `__ \/ __ \
/ /__/ /  / /_/ / / / / / / /_/ /
\___/_/   \__,_/_/ /_/ /_/_.___/ 
                                 
```
# Crumb
> Crumb is a CHIP-8 emulator written in C from scratch.

![Version](https://img.shields.io/badge/version-0.1.0-blue)
![Language](https://img.shields.io/badge/language-C-informational)
![License](https://img.shields.io/badge/license-MIT-green)

## Summary

Crumb is a CHIP-8 interpreter built entirely in C, implementing the fetch-decode-execute cycle at the heart of every emulator. Crumb models the CHIP-8 virtual machine directly, including registers, memory, stack, timers, and a 64x32 monochrome display, and executes original CHIP-8 ROMs instruction by instruction.

This project was built to develop an understanding of how emulators work at a low level, examining opcode decoding, memory-mapped fonts, and the interpreter pattern that CHIP-8 (and by extension most virtual machines) is built on.

## Features

- Full CHIP-8 CPU core: 16 general-purpose registers, 4KB memory, 16-level stack, index register, and program counter
- Complete instruction set: all 35 CHIP-8 opcodes implemented (`0x0`-`0xF`), dispatched through a function-pointer opcode table
- ROM loading into memory starting at `0x200`
- Built-in fontset loaded into memory at `0x50` for the `FX29` sprite-character opcode
- Delay and sound timers that decrement each cycle
- `DXYN` sprite drawing with XOR pixel logic and collision (`VF`) detection into an internal framebuffer
- Keypad state array for the `EX9E`/`EXA1`/`FX0A` key-input opcodes

## Not Yet Implemented

The CPU core is functionally complete, but Crumb currently has no runtime frontend: `main.c` initializes the CPU and loads a ROM, then exits without ever running the cycle loop. The following are stubbed or missing:

- **SDL2 display output**: SDL2 is linked but no window, renderer, or texture is created; the internal `screen` framebuffer is never drawn to the screen
- **Main emulation loop**: nothing calls `cpuCycle()` after startup, so no ROM actually runs
- **Keyboard input**: the `keys` array exists on the `CPU` struct, but no SDL event polling maps host keys to the CHIP-8 keypad
- **Audio output**: `soundTimer` counts down, but no beep/tone is ever played
- **Clock timing**: the `scale` and `delay` CLI arguments are parsed but unused; there's no frame timing or configurable cycle delay yet

## Process

When a ROM is loaded, Crumb's interpreter follows this pipeline:

```
ROM → loadROM (cpu.c) → Fetch (cpuCycle) → Decode (op_table) → Execute (opXXXX)
                                                                        ↓
                                                          Mutate registers / memory / screen
```

Each cycle, `cpuCycle()` fetches the next 2-byte opcode from memory at `pc`, advances the program counter, dispatches to the matching handler via `op_table` (indexed by the opcode's top nibble), and ticks the delay/sound timers.

## Project Structure

```
crumb/
├── main.c          # Entry point; parses CLI args, inits CPU, loads ROM
├── cpu.c/h         # CPU state, opcode table, and all instruction handlers
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

Note: since the main loop and display are not yet implemented, running Crumb currently loads a ROM into memory and exits without rendering or executing it.

## Planned Features

- SDL2 window and renderer for the 64x32 display
- Main emulation loop driving `cpuCycle()` at a configurable clock speed
- Keyboard-to-keypad input mapping via SDL events
- Audio beep while `soundTimer` is active
- Debug mode for stepping through opcodes and inspecting registers

## Author

Built by Carson Smith - [github.com/carson-js](https://github.com/carson-js) · [carsonjs.me](https://carsonjs.me)

## License

MIT License, see [LICENSE](LICENSE) for details.

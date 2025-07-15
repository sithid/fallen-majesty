# Fallen Majesty MUD

A classic Multi-User Dungeon (MUD) based on the Merc/Diku codebase, featuring extensive customizations and multiple character classes including vampires, werewolves, mages, and various other supernatural beings.

## Overview

Fallen Majesty is a text-based multiplayer online role-playing game that runs in a terminal environment. It's built on the foundation of the classic Merc 2.1 codebase with significant enhancements and customizations.

## Features

- **Multiple Character Classes**: Vampire, Werewolf, Mage, Monk, Paladin, Ninja, Shapeshifter, Drow, Highlander, Inquisitor, Undead Knight, Tanarri, Psionist, and more
- **Clan System**: Comprehensive clan management with territories and warfare
- **Advanced Combat System**: Enhanced fighting mechanics with special abilities
- **Online Building**: In-game world building capabilities (OLC - Online Creation)
- **Quest System**: Dynamic quest and mission system
- **Player vs Player**: PvP combat with kingdom system
- **Extensive Areas**: Large world with diverse environments and storylines

## Technical Details

### Dependencies

- **Operating System**: Linux (tested on Ubuntu in WSL)
- **Compiler**: GCC with C99 support
- **Libraries**: 
  - libcrypt (for password encryption)
  - Standard C libraries

### Build Requirements

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install gcc libc6-dev libcrypt-dev build-essential
```

### Compilation

1. Navigate to the source directory:
   ```bash
   cd src/
   ```

2. Compile the MUD:
   ```bash
   make
   ```

3. The compiled binary will be created as `dystopia`

4. Clean build files and logs (if needed):
   ```bash
   make clean
   ```

### Configuration

- **Port Configuration**: Edit startup scripts to change the default port (typically 1000)
- **Area Files**: World content is stored in `area/*.are` files
- **Help Files**: Player help system in `area/` directory
- **Startup Scripts**: Multiple startup configurations available in `src/`

## Directory Structure

```
fallen-majesty/
├── src/                    # Source code files
│   ├── *.c                # C source files
│   ├── *.h                # Header files
│   ├── Makefile           # Build configuration
│   ├── dystopia           # Compiled binary (generated)
│   └── startup*           # Startup scripts
├── area/                  # World files and game data
│   ├── *.are              # Area definition files
│   ├── help.are           # Help system
│   ├── socials.txt        # Social commands
│   └── *.txt              # Various game configuration files
├── player/                # Player files (runtime data)
├── log/                   # Log files and debugging output
├── doc/                   # Documentation
│   └── license.txt        # License information
├── db/                    # Database files
├── changes/               # Change logs
└── notes/                 # Development notes
```

## Running the MUD

### Basic Startup

```bash
cd src/
./startup
```

### Advanced Startup Options

- `startup2` - Alternative startup configuration
- `startup3` - Debug startup configuration
- `extstartup` - Extended startup with additional features

### Default Connection

- **Host**: localhost
- **Port**: 1000 (configurable)
- **Protocol**: Raw TCP sockets

## Development

### Key Source Files

- **Core Engine**: `comm.c`, `interp.c`, `handler.c`, `db.c`
- **Character Classes**: `vamp.c`, `ww.c`, `mage.c`, `monk.c`, etc.
- **Combat System**: `fight.c`, `kav_fight.c`
- **World Building**: `olc.c`, `olc_act.c`, `olc_save.c`
- **Player Actions**: `act_*.c` files
- **Clan System**: `clan.c`, `newclans.c`

### Memory Management

The codebase includes custom memory management and debugging:
- **Memory Watching**: `memwatch.c` for memory leak detection
- **Debug Builds**: Compile with `-DMEMWATCH` flag for detailed memory tracking
- **Valgrind Support**: Compatible with Valgrind for memory analysis

### Adding New Features

1. **New Commands**: Add to `interp.c` and implement in appropriate `act_*.c` file
2. **New Classes**: Create new class file following existing patterns
3. **New Areas**: Use in-game OLC system or edit `.are` files directly
4. **New Spells/Skills**: Add to `magic.c` and update skill tables

## Debugging

### Common Issues

1. **Compilation Errors**: Ensure all dependencies are installed
2. **Port Conflicts**: Check if port 1000 is available
3. **Memory Issues**: Use `memwatch` or `valgrind` for debugging
4. **Core Dumps**: Check `log/` directory for crash information

### Debug Tools

- **Memory Watch**: Built-in memory leak detection
- **Valgrind Integration**: Use `valgrind ./dystopia` for memory analysis
- **GDB Support**: Compiled with debug symbols (`-g3` flag)
- **Clean Build**: `make clean` removes compiled files and clears log files for fresh debugging sessions

## License

This MUD is based on the Merc 2.1 codebase, which is derived from DikuMUD. Usage is subject to the original Diku and Merc licenses:

- **Non-commercial use only**
- **Original author credits must be maintained**
- **No resale or profit operation**

See `doc/license.txt` for complete license terms.

## Contributing

When contributing to this codebase:

1. **Code Style**: Follow existing conventions and formatting
2. **Testing**: Test thoroughly on the target platform (WSL Ubuntu)
3. **Documentation**: Update relevant documentation for new features
4. **Memory Safety**: Use proper memory management practices
5. **Compatibility**: Ensure changes don't break existing functionality

## Historical Note

This is a classic MUD codebase that represents the golden age of text-based multiplayer gaming. The code reflects programming practices and gaming culture from the 1990s and early 2000s, preserved and updated to work in modern environments.

## Support

For issues with compilation or gameplay mechanics, review the source code and existing documentation. The codebase is well-commented and follows traditional MUD architectural patterns.

---

*"In the realm of Fallen Majesty, legends are born and destinies are forged."*

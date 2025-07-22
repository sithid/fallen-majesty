# Fallen Majesty MUD Project Plan

## Purpose and Goals
This document outlines the strategy for cleaning, documenting, and modernizing the Fallen Majesty MUD C codebase. The primary goals include:
- Removing unused, unfinished, or dead code.
- Thoroughly documenting the codebase for future maintainability.
- Refactoring for modern C coding standards and best practices.
- Creating a clean, well-documented, independent C codebase that can be maintained long-term.
- Ensuring the codebase compiles cleanly and runs reliably.

## Codebase Analysis

### Structure Overview
The codebase is organized into several directories including:
- `src/`: Contains key source files and a `Makefile` for building the `dystopia` binary.
   - Focus on files like `merc.h`, `interp.c`, `psionist.c`.
   - Approximately 150,000 lines of code.
- `area/`: Houses world content and game data in `.are` files and other text-based configurations.
- `doc/`: Documentation-related files, including licensing information.

### Key Features
- **Character Classes**: Vampire, Werewolf, Mage, and more.
- **Clan System**: Management of clans and warfare.
- **Combat System**: Advanced fighting mechanics.
- **OLC**: Online world building.
- **Quest System**: Dynamic quests and missions.

## Project Phases

### Phase 1: Cleanup and Removal (Current Phase)
Systematic cleanup of the codebase to remove clutter and unused elements:
- Remove unused structures, macros, and definitions
- Eliminate commented-out code blocks
- Remove dead code and unfinished features
- Clean up stale comments and outdated documentation
- Standardize formatting and naming conventions
- Focus on making the codebase lean and maintainable

### Phase 2: Documentation and Modernization (After Cleanup)
Comprehensive documentation and enhancement of the cleaned codebase:
- Document all major functions, structures, and systems
- Create architectural overview and system interaction diagrams
- Write developer guides and maintenance documentation
- Apply modern C coding standards where appropriate
- Enhance error handling and memory management
- Create build and deployment guides

## Refactoring Plan

### Unused Code Removal
- Systematically review each file to identify and remove dead code, unused macros like `memwatch`, and stale comments.

### File Cleanup
- Standardize file headers, update and simplify if needed.

### Improvements
- Refactor code for readability, maintainability, and performance improvements.
- Ensure consistent naming conventions and eliminate commented-out and redundant code sections.

### Dependency Management
- Assess current dependencies and update to newer versions where applicable.

## Documentation Strategy

### Code Comments
- Ensure comprehensive documentation of functions, classes, and file-level descriptions.

### README and Guides
- Update README to reflect current state and future roadmap.

### API Documentation
- Plan for detailed documentation of APIs and endpoints.

## Modernization Strategy

### Dependency Analysis
- Review all C libraries in use and update to modern versions where possible.
- Ensure compatibility with current Linux distributions and compilers.
- Remove obsolete or unnecessary dependencies.

### Code Standards
- Apply modern C coding standards and best practices.
- Improve memory management and error handling.
- Enhance portability and maintainability.

## Quality Assurance

### Testing
- Develop a testing suite and incorporate automated testing.

### Code Review
- Establish peer code review protocols for consistent quality.

## Future Enhancements
- Explore potential C codebase improvements such as performance optimizations.
- Consider additional debugging and development tools.
- Plan for potential feature additions within the C framework.
- Maintain flexibility for future architectural decisions.

## Action Log

### Progress Tracking
This section documents all completed tasks, changes made, and important decisions throughout the project.

**Format:** `[Date] - [Task/Change] - [Files Modified] - [Notes]`

### Completed Actions
- **2025-07-22** - Initial project plan created - `FallenMajesty_ProjectPlan.md` - Established project scope focusing on C codebase cleanup and documentation only
- **2025-07-22** - Added action log section - `FallenMajesty_ProjectPlan.md` - Created tracking mechanism for future progress updates
- **2025-07-22** - Removed accidental grep output files - `src/class`, `src/vnum` - Cleaned up unintended file artifacts from the codebase
- **2025-07-22** - Started cleanup of core header file - `src/merc.h` - Removed unused `king_data` struct at line 2701, replaced with cleanup note
- **2025-07-22** - Cleaned up unused autosave flags - `src/merc.h` - Removed `SV_UNUSED1`, `SV_UNUSED2`, and `SV_UNUSED3` definitions, replaced with cleanup comments
- **2025-07-22** - Cleaned up commented-out form definitions - `src/merc.h` - Removed 28 lines of unused commented-out form definitions (lines 901-924), replaced with concise cleanup comments. Reduced code clutter significantly
- **2025-07-22** - Resolved accidental area file modifications - `area/*.are` - Identified and reverted unintended numerical changes to area files. Area files contain game world data and should not be modified during source cleanup
- **2025-07-22** - Fixed build error from overzealous cleanup - `src/merc.h` - Restored accidentally removed `mob_index_data` fields that were still referenced in `db.c`. Learned that fields marked as "unused" may still be needed by loading/saving code. Build now compiles successfully again.
- **2025-07-22** - Communication module cleanup - `src/comm.c` - Removed obsolete networking declarations, cleaned up socket management code, and replaced legacy comments for improved clarity.
- **2025-07-22** - Combat system cleanup - `src/fight.c` - Removed specific unnecessary comments including legacy developer references (line ~990), outdated spell attack comments (line ~995), commented-out PK slowdown code (line ~2031), and non-functional death_cry code (line ~4661). Code functionality preserved while improving maintainability.
- **2025-07-22** - Header file cleanup completion - `src/merc.h` - **COMPLETED** final cleanup including removing commented-out PLR_BRIEF3 definition (line 1133), fixing excessive whitespace after newbie pack definitions, and improving unprofessional comment text. The core header file cleanup is now complete.
- **2025-07-22** - Communication actions cleanup - `src/act_comm.c` - **COMPLETED** comprehensive cleanup of commented-out legacy code including: removed age check comment (line 95), NPC check comments (lines 160-161), entire KTALK channel system remnants (lines 449-457, 547-550, 948-957), commented auction function (lines 682-688), debug code in flist function (lines 796-797), commented flame channel call (line 844), quit restrictions and socket close code (lines 2033-2039, 2087-2089), follower variables (lines 2234-2235), order restrictions (lines 2369-2380), group display comments (lines 2624, 2638, 2652), and gtell loop condition (line 2899). All functional code preserved while significantly improving maintainability.

### Next Actions
- [ ] Identify and clean up other core header files
- [ ] Begin cleanup of main source files based on priority - **READY TO START**
  - [ ] Review and prioritize source files for cleanup (act_*.c, db.c, interp.c, etc.)
  - [ ] Start with files that have high impact on maintainability
- [ ] **Phase 2**: Comprehensive documentation of cleaned codebase
  - [ ] Document all major functions and their purposes
  - [ ] Create architectural overview documentation
  - [ ] Document game systems and their interactions
  - [ ] Create developer guides for future maintainers

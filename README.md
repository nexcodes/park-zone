# Smart Parking Allocation & Management System

A Data Structures & Algorithms (DSA) academic project implementing an intelligent parking management system in C++ (currently console-based, Qt UI planned for Phase 4).

## Project Overview

This system manages parking allocation across multiple zones with features including:
- **Smart Allocation**: Same-zone preference with cross-zone fallback and penalty
- **State Management**: Enforced state machine for request lifecycle
- **Rollback Operations**: Stack-based undo of last k allocation operations
- **Analytics**: Zone utilization, parking duration, revenue tracking
- **Console Interface**: Comprehensive test suite demonstrating all features

## Key Constraints

- ✅ Custom data structures (no STL containers in core logic)
- ✅ Multi-file implementation (separate .h/.cpp files)
- ✅ In-memory storage (no database/file persistence)
- ✅ No global variables
- ✅ Manual memory management

## Architecture

```
ParkingSystem (Controller)
  ├── AllocationEngine (slot assignment logic)
  │   └── RollbackManager (stack-based undo)
  ├── AnalyticsEngine (queries trip history)
  ├── StateValidator (enforces state machine)
  └── zones: DynamicArray<Zone*>
       └── areas: DynamicArray<ParkingArea*>
            └── slots: DynamicArray<ParkingSlot*>
```

## Entity Hierarchy

```
City
 └── Zones (e.g., Downtown, Airport, Mall)
      └── Parking Areas (e.g., Level 1, Level 2)
           └── Parking Slots (individual spaces)
```

## Development Phases

| Phase | Status | Focus Area |
|-------|--------|------------|
| **Phase 1** | ✅ Complete | System Design & Planning |
| **Phase 2** | ✅ Complete | Core Logic Implementation (Console) |
| **Phase 3** | ✅ Complete | Analytics Module |
| **Phase 4** | 🔄 In Progress | Qt UI Integration |
| **Phase 5** | ⏳ Pending | Testing & Validation |

## Documentation

- [Roadmap](planning/roadmap.md) - Complete implementation guide with phase-by-phase details
- [Copilot Instructions](.github/copilot-instructions.md) - AI agent development guidelines

## Core Data Structures

All custom implementations (no STL containers):

- **DynamicArray<T>**: Auto-resizing array with `add()`, `get()`, `remove()`, `[]` operator
- **LinkedList<T>**: Singly-linked list with `append()`, `prepend()`, `find()`, iterator support
- **Stack**: Fixed-size stack (100 operations) storing `OperationLog` structs for rollback
- **Queue<T>**: Circular queue template (implementation complete, not yet used in workflows)

All templates implemented in headers (required for C++ templates).

## Request State Machine

Valid transitions enforced by [StateValidator](src/StateValidator.cpp):

```
REQUESTED → ALLOCATED → OCCUPIED → RELEASED
REQUESTED → CANCELLED
ALLOCATED → CANCELLED
```

**Invalid transitions blocked**: e.g., OCCUPIED → REQUESTED, RELEASED → ALLOCATED

## Build Instructions

### Prerequisites
- C++ Compiler with C++17 support (MSVC 2017+ / GCC 7+ / Clang 5+)
- Windows PowerShell (for build script) or manual g++ compilation

### Windows Build (Automated)
```powershell
# Run PowerShell build script
.\build.ps1

# Execute the program
.\build\parking_system.exe
```

### Manual Build (Cross-Platform)
```bash
# Compile all source files (order matters - see build.ps1 for correct sequence)
g++ -std=c++17 -Isrc/include \
  src/StateValidator.cpp src/Stack.cpp src/ParkingSlot.cpp src/Vehicle.cpp \
  src/ParkingArea.cpp src/Zone.cpp src/ParkingRequest.cpp \
  src/RollbackManager.cpp src/AllocationEngine.cpp src/AnalyticsEngine.cpp \
  src/ParkingSystem.cpp src/main.cpp \
  -o parking_system

./parking_system
```

**Important**: Compilation order matters due to dependencies. See [build.ps1](build.ps1) for the correct sequence.

## Testing

10 comprehensive test functions in [main.cpp](src/main.cpp) demonstrating:

1. Basic same-zone allocation
2. Cross-zone allocation with penalty (when zone full)
3. Cancellation before/after allocation
4. Invalid state transition rejection (StateValidator)
5. Rollback operations (undo last k allocations)
6. Vehicle arrival/exit workflow
7. Complete parking lifecycle
8. Multiple concurrent requests
9. Analytics queries (utilization, revenue, duration)
10. Edge cases (empty zones, invalid IDs)

Run all tests sequentially via `parking_system.exe`

## Key Features Implemented

### Allocation Logic
- **Same-zone first**: Searches requested zone for available slots
- **Cross-zone fallback**: Applies 1.5x penalty when same-zone unavailable
- **Automatic logging**: All allocations logged to rollback stack

### Memory Management
- **Manual allocation**: All entities use `new`/`delete` (no smart pointers)
- **Ownership model**: ParkingSystem owns zones/requests, manages cleanup
- **C-string handling**: Manual `new char[]`/`delete[]` for string data

### State Validation
- **Strict enforcement**: StateValidator blocks invalid transitions
- **Lifecycle tracking**: Each request progresses through defined states
- **Rollback consistency**: Analytics recalculate after rollback operations

## Academic Context

**Course**: Data Structures & Algorithms  
**Semester**: 3  
**Institution**: UMT  
**Focus**: Manual implementation of fundamental data structures without STL containers

## License

Academic project for educational purposes.

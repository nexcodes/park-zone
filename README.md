# Smart Parking Allocation & Management System

A Data Structures & Algorithms (DSA) academic project implementing an intelligent parking management system in C++ with Qt UI.

## Project Overview

This system manages parking allocation across multiple zones with features including:
- **Smart Allocation**: Same-zone preference with cross-zone fallback
- **State Management**: Enforced state machine for request lifecycle
- **Rollback Operations**: Undo last k allocation operations
- **Analytics**: Zone utilization, parking duration, peak usage tracking
- **Qt UI**: User-friendly graphical interface

## Key Constraints

- ✅ Custom data structures (no STL containers in core logic)
- ✅ Multi-file implementation (separate .h/.cpp files)
- ✅ In-memory storage (no database/file persistence)
- ✅ No global variables
- ✅ Manual memory management

## Architecture

```
Qt UI → ParkingSystem → AllocationEngine ↔ RollbackManager
                      → Zone → ParkingArea → ParkingSlot
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
| **Phase 2** | 🔄 Pending | Core Logic Implementation (Console) |
| **Phase 3** | 🔄 Pending | Analytics Module |
| **Phase 4** | 🔄 Pending | Qt UI Integration |
| **Phase 5** | 🔄 Pending | Testing & Validation |

## Documentation

- [Roadmap](planning/roadmap.md) - Complete implementation guide
- [System Design](planning/phase1-system-design.md) - Architecture & design principles
- [Data Structures](planning/phase1-data-structures.md) - Custom data structure specifications
- [State Machine](planning/phase1-state-machine.md) - Request lifecycle & transitions
- [Class Specifications](planning/phase1-class-specifications.md) - Detailed class documentation

## Core Data Structures

- **Dynamic Arrays**: Store zones, areas, and slots
- **Linked Lists**: Trip history and request logs
- **Stack**: Rollback operation history
- **Queue**: Incoming parking requests
- **Custom Adjacency**: Zone connections

## Request State Machine

```
REQUESTED → ALLOCATED → OCCUPIED → RELEASED
REQUESTED → CANCELLED
ALLOCATED → CANCELLED
```

## Build Instructions

### Prerequisites
- C++ Compiler (GCC 7+ / MSVC 2017+ / Clang 5+)
- Qt 6.x (for UI phase)
- CMake 3.16+ (recommended) or qmake

### Console Build (Phase 2)
```bash
g++ -std=c++17 main.cpp -o parking_system
./parking_system
```

### Qt Build (Phase 4)
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Testing

Minimum 10 test cases covering:
- Same-zone and cross-zone allocation
- Cancellation scenarios
- Rollback correctness
- Invalid state transition rejection
- Analytics accuracy

## Academic Context

**Course**: Data Structures & Algorithms  
**Semester**: 3  
**Focus**: Manual implementation of fundamental data structures

## License

Academic project for educational purposes.

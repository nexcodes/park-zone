# Smart Parking Management System - AI Agent Instructions

## Project Context

This is a **Data Structures & Algorithms (DSA) academic project** implementing a Smart Parking Allocation & Management System in C++ with Qt UI.

**Critical Constraints**: 
- Do NOT use STL containers (map, unordered_map, set, vector) for core logic
- Implement custom data structures using arrays, pointers, linked lists, stacks, and queues
- **The entire project must be in-memory** (no database, no file I/O for data persistence)
- **No global variables allowed**
- **Multi-file implementation is mandatory** (separate .h/.cpp files for each class)
- **Header files must contain declarations only** (no implementation in headers except inline functions)

## Architecture Overview

```
Qt UI → ParkingSystem (Controller) → AllocationEngine ↔ RollbackManager
                                   → Zone → ParkingArea → ParkingSlot
                                   → Vehicle
```

**Entity Hierarchy**: City → Zones → Parking Areas → Parking Slots

**Required File Structure**:
- `Zone.h` / `Zone.cpp`
- `ParkingArea.h` / `ParkingArea.cpp`
- `ParkingSlot.h` / `ParkingSlot.cpp`
- `Vehicle.h` / `Vehicle.cpp`
- `ParkingRequest.h` / `ParkingRequest.cpp`
- `AllocationEngine.h` / `AllocationEngine.cpp`
- `RollbackManager.h` / `RollbackManager.cpp`
- `ParkingSystem.h` / `ParkingSystem.cpp`
- `main.cpp`

This structure is modifiable according to requirements but multi-file separation is mandatory.

## Core Data Structures (Manual Implementation Required)

- **Arrays/Dynamic Arrays**: Store zones and parking areas (in-memory)
- **Linked Lists**: Parking request history and trip history
- **Stacks**: Rollback operations (stores slot ID, previous state, request state)
- **Queues**: Incoming parking requests
- **Custom Adjacency/Zone Relationship Structure**: Zone connections using array of zone IDs (not std::vector)
  - No routing graph required, only logical zone connections
  - Implement zone preference rules

## State Machine (Enforce Strictly)

Valid transitions:
- `REQUESTED → ALLOCATED → OCCUPIED → RELEASED`
- `REQUESTED → CANCELLED`
- `ALLOCATED → CANCELLED`

Block invalid transitions (e.g., OCCUPIED → REQUESTED).

## Development Workflow

1. **Console-first approach**: Complete all core logic in `main.cpp` with console I/O before touching Qt
2. **UI is a thin layer**: Qt widgets should only call methods from `ParkingSystem`, never contain business logic
3. **Build**: Use CMake or qmake for Qt integration (CMake preferred)

## Key Classes & Responsibilities

- **ParkingSlot**: 
  - Properties: Slot ID, Zone ID, availability status
  - Methods: `allocate()`, `release()`
  
- **ParkingArea**: 
  - Contains array of parking slots
  - Finds first available slot using first-available strategy
  
- **Zone**: 
  - Contains array of parking areas
  - Custom adjacency array for logically connected zones
  - Zone preference rules for allocation
  
- **Vehicle**: 
  - Properties: Vehicle ID, preferred zone
  
- **ParkingRequest**: 
  - Properties: Vehicle ID, requested zone, request time, current state (enum)
  - Lifecycle: REQUESTED → ALLOCATED → OCCUPIED → RELEASED (or CANCELLED)
  
- **AllocationEngine**: 
  - Same-zone preference with first-available slot strategy
  - Cross-zone allocation allowed if requested zone is full (incurs extra cost/penalty)
  
- **RollbackManager**: 
  - Stack-based undo of last k allocation operations
  - Must restore slot availability AND request state changes
  - Must recalculate analytics data after rollback
  
- **ParkingSystem**: 
  - Main controller connecting all components
  - Maintains trip history and analytics
  - Provides query support for analytics

## Critical Implementation Details

- **In-Memory Storage**: All data must be stored in memory using custom data structures (no database, no file I/O)

- **Allocation Logic**: 
  - Try requested zone first using first-available slot strategy
  - If requested zone is full, try adjacent/connected zones with extra cost/penalty
  - Same-zone preference must be enforced
  
- **Cancellation & Rolmust be implemented, covering:
- **Slot allocation correctness** (same-zone allocation)
- **Cross-zone allocation handling** (when requested zone is full)
- **Cancellation correctness** (before and after allocation)
- **Rollback correctness** (restore slot availability and request states)
- **Invalid state transition detection** (prevent illegal state changes)
- **Analytics correctness after rollback** (verify analytics recalculation)
- Zone utilization calculations
- Average parking duration tracking
- Peak zone identification
- Cancelled vs completed request ratios rollback
  
- **Trip History & Analytics**: 
  - Maintain complete history of all parking requests
  - Support queries for:
    - Average parking duration
    - Zone utilization rate
    - Cancelled vs completed requests ratio
    - Peak usage zones
  - Traverse history linked list but account for rolled-back operations
  
- **Memory Management**: Use raw pointers with proper cleanup in destructors (academic DSA requirement)

- **No External Dependencies**: No external map/routing APIs, no global variables

## Testing Requirements

Minimum 10 test cases covering:
- Same-zone & cross-zone allocation
- Cancellation before/after allocation
- Rollback correctness & analytics consistency
- Invalid state transition rejection

## File Organization

**Mandatory Multi-File Structure**:
- `main.cpp`: Entry point and console testing
- **Core classes must be in separate `.h`/`.cpp` files**:
  - Header files (`.h`): Declarations only
  - Implementation files (`.cpp`): All function implementations
- `planning/roadmap.md`: Full system specification and phase breakdown

**Compilation**: Use CMake or qmake for Qt integration (CMake preferred)

## References

See [planning/roadmap.md](planning/roadmap.md) for complete phase-by-phase implementation guide.

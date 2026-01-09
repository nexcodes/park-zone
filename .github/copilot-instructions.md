# Smart Parking Management System - AI Agent Instructions

## Project Context

This is a **Data Structures & Algorithms (DSA) academic project** implementing a Smart Parking Allocation & Management System in C++ with Qt UI.

**Critical Constraint**: Do NOT use STL containers (map, unordered_map, set, vector) for core logic. Implement custom data structures using arrays, pointers, linked lists, stacks, and queues.

## Architecture Overview

```
Qt UI → ParkingSystem (Controller) → AllocationEngine ↔ RollbackManager
                                   → Zone → ParkingArea → ParkingSlot
```

**Entity Hierarchy**: City → Zones → Parking Areas → Parking Slots

## Core Data Structures (Manual Implementation Required)

- **Arrays/Dynamic Arrays**: Store zones and parking areas
- **Linked Lists**: Parking request history
- **Stacks**: Rollback operations (stores slot ID, previous state, request state)
- **Queues**: Incoming parking requests
- **Custom Adjacency List**: Zone connections using array of zone IDs (not std::vector)

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

- **ParkingSlot**: Single slot with availability, vehicle ID; methods: `allocate()`, `release()`
- **ParkingArea**: Array of slots; finds first available
- **Zone**: Array of areas + custom adjacency array for neighboring zones
- **ParkingRequest**: Vehicle ID, zone, timestamp, current state (enum)
- **AllocationEngine**: Same-zone first, cross-zone with penalty
- **RollbackManager**: Stack-based undo of last k operations (must adjust analytics data)
- **ParkingSystem**: Main controller connecting all components

## Critical Implementation Details

- **Allocation logic**: Try requested zone first, then adjacent zones with penalty
- **Rollback**: Pop stack entries and restore slot availability + request states; must recalculate analytics after rollback
- **Analytics**: Track avg duration, utilization rate, cancellation rate, peak zone; traverse history linked list but account for rolled-back operations
- **Memory management**: Use raw pointers with proper cleanup in destructors (academic DSA requirement)

## Testing Requirements

Minimum 10 test cases covering:
- Same-zone & cross-zone allocation
- Cancellation before/after allocation
- Rollback correctness & analytics consistency
- Invalid state transition rejection

## File Organization

- `main.cpp`: Entry point and console testing
- Core classes: Implement in separate `.h`/`.cpp` files when project grows
- `planning/roadmap.md`: Full system specification and phase breakdown

## References

See [planning/roadmap.md](planning/roadmap.md) for complete phase-by-phase implementation guide.

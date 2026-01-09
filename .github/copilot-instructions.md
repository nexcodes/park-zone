# Smart Parking Management System - AI Agent Instructions

## Project Context

This is a **Data Structures & Algorithms (DSA) academic project** implementing a Smart Parking Allocation & Management System in C++ (currently console-based, Qt UI planned for Phase 4).

**Critical Constraints**: 
- ❌ NO STL containers (map, unordered_map, set, vector) in core logic
- ✅ Custom data structures only: DynamicArray, LinkedList, Stack, Queue templates
- ✅ In-memory storage (no database/file persistence)
- ✅ No global variables
- ✅ Multi-file implementation with separate .h/.cpp files

## Architecture & Data Flow

```
ParkingSystem (Controller)
  ├── AllocationEngine (handles slot assignment logic)
  │   └── RollbackManager (stack-based undo operations)
  ├── AnalyticsEngine (queries trip history)
  ├── StateValidator (enforces state machine rules)
  └── zones: DynamicArray<Zone*>
       └── areas: DynamicArray<ParkingArea*>
            └── slots: DynamicArray<ParkingSlot*>
```

**Entity Hierarchy**: Zones → Parking Areas → Parking Slots (e.g., Downtown → Level 1 → Slot 1001)

## Custom Data Structures (src/include/)

All templates implemented in headers (required for C++ templates):

- **DynamicArray<T>**: Auto-resizing array with `add()`, `get()`, `remove()`, `[]` operator
  - Example: `DynamicArray<Zone*> zones;`
  
- **LinkedList<T>**: Singly-linked list with `append()`, `prepend()`, `find()`, iterator support
  - Example: `LinkedList<TripHistory> tripHistory;`
  
- **Stack**: Fixed-size stack (100 operations) storing `OperationLog` structs
  - Used by RollbackManager to undo allocations
  
- **Queue<T>**: Circular queue for incoming requests (not yet used in current implementation)

- **Enums.h**: `RequestState` (REQUESTED, ALLOCATED, OCCUPIED, RELEASED, CANCELLED), `SlotStatus`

## State Machine (StateValidator enforces)

Valid transitions (see [StateValidator.cpp](src/StateValidator.cpp)):
```
REQUESTED → ALLOCATED → OCCUPIED → RELEASED
REQUESTED → CANCELLED
ALLOCATED → CANCELLED
```

**Invalid transitions blocked**: e.g., OCCUPIED → REQUESTED, RELEASED → ALLOCATED

## Key Workflows

### Building & Running
```powershell
# Windows build (see build.ps1)
.\build.ps1
.\build\parking_system.exe
```

Compilation order matters (see [build.ps1](build.ps1) line 13-24):
1. StateValidator, Stack (no dependencies)
2. ParkingSlot, Vehicle (basic entities)
3. ParkingArea, Zone (contain slots)
4. ParkingRequest (uses state enums)
5. RollbackManager, AllocationEngine, AnalyticsEngine (depend on entities)
6. ParkingSystem (controller)
7. main.cpp (last)

### Allocation Logic (AllocationEngine.cpp)

1. **Same-zone first**: `findSlotInZone(requestedZoneId)` - first available slot
2. **Cross-zone fallback**: `findSlotInAdjacentZones()` - applies 1.5x penalty
3. **Logging**: `rollbackManager->logAllocation(slotId, vehicleId, prevState)`

Example from [AllocationEngine.cpp](src/AllocationEngine.cpp#L11-L31):
```cpp
ParkingSlot* slot = findSlotInZone(requestedZoneId);
if (slot != nullptr) {
    slot->allocate(request->getVehicleId());
    request->allocateSlot(slot->getSlotId(), false); // false = same-zone
    rollbackManager->logAllocation(slot->getSlotId(), vehicleId, REQUESTED);
}
```

### Rollback Mechanism (RollbackManager.cpp)

- **Stack stores**: `{slotId, vehicleId, previousState}`
- **Rollback(k)**: Undo last k operations, restore slot availability + request state
- **Analytics**: Trip history remains but analytics recalculate to ignore rolled-back requests

## Memory Management Patterns

- **Manual allocation**: `new` for all entities (`Zone*`, `ParkingRequest*`)
- **Ownership**: ParkingSystem owns zones/requests, deletes in destructor
- **String handling**: C-strings (`char*`) with manual `new char[]` / `delete[]`
  - Example in [ParkingSystem.h](src/include/ParkingSystem.h#L16-L28): `TripHistory` struct with custom copy/assignment

## Testing Approach (main.cpp)

10 test functions demonstrating:
1. Basic same-zone allocation
2. Cross-zone allocation (when zone full)
3. Cancellation (before/after allocation)
4. Invalid state transitions
5. Rollback operations
6. Vehicle arrival/exit workflow
7. Complete parking lifecycle
8. Multiple concurrent requests
9. Analytics queries
10. Edge cases (empty zones, invalid IDs)

Run tests sequentially in `main()` with clear console output separators.

## Current Phase Status

- ✅ **Phase 2 Complete**: Core logic fully functional in console mode
- 🔄 **Phase 3 In Progress**: AnalyticsEngine implemented, needs refinement
- ⏳ **Phase 4 Pending**: Qt UI integration (console-first approach validated)

## Common Pitfalls

1. **Never modify header-only templates**: DynamicArray, LinkedList implementations must stay in .h files
2. **State validation**: Always use `StateValidator::canTransition()` before changing request state
3. **Memory leaks**: Ensure destructors delete all allocated memory (zones, requests, C-strings)
4. **Rollback consistency**: After rollback, analytics must recalculate (don't cache stale data)
5. **Cross-zone penalty**: Set via `AllocationEngine::setCrossZonePenalty()` (default 1.5x)

## Key Files Reference

- [planning/roadmap.md](planning/roadmap.md) - Full phase-by-phase implementation guide
- [README.md](README.md) - High-level overview and build instructions
- [src/main.cpp](src/main.cpp) - Test suite demonstrating all features
- [src/include/Enums.h](src/include/Enums.h) - State machine definitions

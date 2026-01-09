## SMART PARKING ALLOCATION & MANAGEMENT ROADMAP

* **Language:** C++
* **UI:** Qt

## Phase 1: Requirement Understanding & System Design

### 1.1 Understand the Problem Domain

Break the system into **real-world entities**:

* City → Zones → Parking Areas → Parking Slots
* Vehicles
* Parking Requests
* Allocation + Rollback
* Analytics

### 1.2 Decide Core Data Structures (DSA Focus)

Avoid STL maps/graphs for core logic.

Suggested structures:

* **Arrays / Dynamic Arrays** → Zones, Parking Areas
* **Linked Lists** → Parking requests history
* **Stacks** → Rollback mechanism
* **Queues** → Incoming parking requests
* **Enums + State Machine** → Request lifecycle

### 1.3 Define State Machine

```text
REQUESTED → ALLOCATED → OCCUPIED → RELEASED
REQUESTED → CANCELLED
ALLOCATED → CANCELLED
```

Invalid transitions must be blocked.

---

## Phase 2: Core Logic Implementation (Without UI)

> ⚠️ Important: **Finish all logic in console mode first**
> Qt UI should only *call* these functions.

---

### 2.1 Class-by-Class Implementation

#### ParkingSlot

Responsibilities:

* Slot ID
* Zone ID
* Availability
* Occupied Vehicle ID

Key methods:

* `allocate(vehicleId)`
* `release()`

---

#### ParkingArea

Responsibilities:

* Collection of parking slots
* Find first available slot

DSA:

* Array of `ParkingSlot`

---

#### Zone

Responsibilities:

* Zone ID
* Parking areas
* Logical adjacency (custom structure)

DSA:

* Array of ParkingArea
* Custom adjacency list (array of zone IDs)

---

#### Vehicle

Responsibilities:

* Vehicle ID
* Preferred zone

---

#### ParkingRequest

Responsibilities:

* Vehicle ID
* Requested zone
* Timestamp
* Current state

Includes:

* Enum `RequestState`

---

### 2.2 Allocation Engine

Responsibilities:

* Same-zone allocation first
* Cross-zone allocation with penalty
* Enforce rules

Methods:

* `allocateSlot(ParkingRequest&)`
* `cancelRequest(ParkingRequest&)`

---

### 2.3 Rollback Manager

Core DSA component.

Use:

* **Stack of operations**

Each stack entry stores:

* Slot ID
* Previous availability
* Request previous state

Supports:

* Rollback last **k** operations

---

### 2.4 Parking System (Controller)

Acts as the **brain**:

* Holds all zones
* Manages requests
* Connects allocation + rollback
* Stores history for analytics

---

## Phase 3: Analytics Module

### Metrics to Implement

* Average parking duration
* Zone utilization rate
* Cancelled vs completed requests
* Peak usage zone

DSA Used:

* Traversing linked lists
* Counters & accumulators

⚠️ Must handle **rollback-adjusted data**

---

## Phase 4: Qt UI Integration

### 4.1 Qt Project Setup

* Qt Widgets Application
* CMake or qmake (CMake preferred)

### 4.2 UI Screens

Minimum recommended screens:

1. **Main Dashboard**

   * Total slots
   * Occupied slots
   * Active requests

2. **Parking Request Screen**

   * Vehicle ID input
   * Zone selection
   * Request parking button

3. **Allocation Status Screen**

   * Slot allocated
   * Zone
   * Penalty if cross-zone

4. **Cancellation & Rollback Screen**

   * Cancel request
   * Rollback last k operations

5. **Analytics Screen**

   * Table + labels (Qt TableWidget)

---

### 4.3 Qt → C++ Core Communication

Qt UI should:

* Call methods from `ParkingSystem`
* Never store logic inside UI classes

Example:

```cpp
void MainWindow::onRequestParkingClicked() {
    parkingSystem.createRequest(vehicleId, zoneId);
}
```

---

## Phase 5: Testing & Validation (Week 6)

### Required Test Cases

At least **10**, including:

* Same-zone allocation
* Cross-zone allocation
* Cancellation before allocation
* Cancellation after allocation
* Rollback correctness
* Invalid state transition rejection
* Analytics after rollback

---

## Final Architecture Overview

```text
Qt UI (Widgets)
     ↓
ParkingSystem
     ↓
AllocationEngine ↔ RollbackManager
     ↓
Zones → Areas → Slots
```
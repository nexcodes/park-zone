#ifndef ENUMS_H
#define ENUMS_H

// Request lifecycle states
enum RequestState {
    REQUESTED,   // Initial state when request is created
    ALLOCATED,   // Slot has been allocated but vehicle not yet parked
    OCCUPIED,    // Vehicle is parked in the slot
    RELEASED,    // Vehicle has left, slot freed
    CANCELLED    // Request cancelled by user
};

// Slot availability status
enum SlotStatus {
    SLOT_AVAILABLE,
    SLOT_OCCUPIED,
    SLOT_RESERVED,     // Optional: for pre-booking
    SLOT_MAINTENANCE   // Optional: slot temporarily unavailable
};

#endif // ENUMS_H

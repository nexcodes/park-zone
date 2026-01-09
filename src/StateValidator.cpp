#include "include/StateValidator.h"

bool StateValidator::isValidTransition(RequestState current, RequestState next) {
    switch (current) {
        case REQUESTED:
            return (next == ALLOCATED || next == CANCELLED);
        
        case ALLOCATED:
            return (next == OCCUPIED || next == CANCELLED);
        
        case OCCUPIED:
            return (next == RELEASED);
        
        case RELEASED:
        case CANCELLED:
            return false;  // Terminal states - no further transitions
        
        default:
            return false;
    }
}

const char* StateValidator::getErrorMessage(RequestState current, RequestState next) {
    if (current == RELEASED) {
        return "Cannot modify a released request";
    }
    if (current == CANCELLED) {
        return "Cannot modify a cancelled request";
    }
    if (current == REQUESTED && next == OCCUPIED) {
        return "Cannot mark as occupied without allocation";
    }
    if (current == REQUESTED && next == RELEASED) {
        return "Cannot release without being occupied";
    }
    if (current == ALLOCATED && next == RELEASED) {
        return "Cannot release without being occupied";
    }
    if (current == OCCUPIED && next == REQUESTED) {
        return "Cannot revert to requested state";
    }
    if (current == OCCUPIED && next == ALLOCATED) {
        return "Cannot revert to allocated state";
    }
    if (current == OCCUPIED && next == CANCELLED) {
        return "Cannot cancel an occupied slot";
    }
    
    return "Invalid state transition";
}

const char* StateValidator::getStateString(RequestState state) {
    switch (state) {
        case REQUESTED: return "REQUESTED";
        case ALLOCATED: return "ALLOCATED";
        case OCCUPIED: return "OCCUPIED";
        case RELEASED: return "RELEASED";
        case CANCELLED: return "CANCELLED";
        default: return "UNKNOWN";
    }
}

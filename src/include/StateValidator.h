#ifndef STATE_VALIDATOR_H
#define STATE_VALIDATOR_H

#include "Enums.h"

class StateValidator {
public:
    // Check if transition from current to next state is valid
    static bool isValidTransition(RequestState current, RequestState next);
    
    // Get error message for invalid transition
    static const char* getErrorMessage(RequestState current, RequestState next);
    
    // Get string representation of state
    static const char* getStateString(RequestState state);
};

#endif // STATE_VALIDATOR_H

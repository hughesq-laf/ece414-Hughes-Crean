#include "inputnum.h"
#include <stdio.h>
static int32_t currentInput = 0;
void clear() {
    currentInput = 0;
}
int32_t getInput(Buttons button) {
    // === TODO: === Simple number building (NO overflow check)
    switch(button) {
    case B0: currentInput = currentInput * 10 + 0; break;
    case B1: currentInput = currentInput * 10 + 1; break;
    // === TODO: === Add cases for 2-9
    case B2: currentInput = currentInput * 10 + 2; break;
    case B3: currentInput = currentInput * 10 + 3; break;
    case B4: currentInput = currentInput * 10 + 4; break;
    case B5: currentInput = currentInput * 10 + 5; break;
    case B6: currentInput = currentInput * 10 + 6; break;
    case B7: currentInput = currentInput * 10 + 7; break;
    case B8: currentInput = currentInput * 10 + 8; break;
    case B9: currentInput = currentInput * 10 + 9; break;
    default: break;
    }
    return currentInput;
}
int32_t performOperation(int32_t num1, int32_t num2, OPERATOR op) {
    // === TODO: === Basic operations (NO overflow check)
    switch(op) {
    case PLUS: return num1 + num2;
    // === TODO: === Write remaining basic operations
    case MINUS: return num1 - num2;
    case MULT: return num1 * num2;
    case DIV: return num1 / num2; //CONDITION WAS MOVED TO main.c file
    break;
    // *** HINT: *** Use if condition to handle divide by zero case
    default: return 0;
    }
}
// Simple helper functions
bool pressC(Buttons button) { return (button == BCL); }
bool pressEq(Buttons button) { return (button == BEQ); }
bool pressOperator(Buttons button) {
    return (button == BPLUS || button == BMINUS ||
    button == BMULT || button == BDIV);
}
bool pressNumber(Buttons button) {
    return (button >= B0 && button <= B9);
}
OPERATOR readOperator(Buttons button) {
    switch(button) {
        case BPLUS: return PLUS;
        case BMINUS: return MINUS;
        case BMULT: return MULT;
        case BDIV: return DIV;
        default: return ERR;
    }
}
#include "Arduino.h"

#ifdef __cplusplus
extern "C"{
#endif

void setAdcsrb(uint8_t value) {
  ADCSRB = value;
}

uint8_t getAco() {
  delay(1);
  return (uint8_t)(ACSR & bit(ACO));
}

void setRisingEdge() {
  ACSR =  bit (ACI)     // (Clear) Analog Comparator Interrupt Flag
    | bit (ACIE)    // Analog Comparator Interrupt Enable
    | bit (ACIS1)
    | bit (ACIS0);  // ACIS1, ACIS0: Analog Comparator Interrupt Mode 
}

void setFallingEdge() {
  ACSR =  bit (ACI)     // (Clear) Analog Comparator Interrupt Flag
    | bit (ACIE)    // Analog Comparator Interrupt Enable
    | bit (ACIS1);  // ACIS1, ACIS0: Analog Comparator Interrupt Mode
}


#ifdef __cplusplus
} // extern "C"
#endif

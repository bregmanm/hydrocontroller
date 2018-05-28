#ifndef _ARDUINO_REGS_H_
#define _ARDUINO_REGS_H_

#ifdef __cplusplus
extern "C"{
#endif

void setAdcsrb(uint8_t); // set value to ADCSRB
uint8_t getAco(); // get ACO bit from ACSR
void setRisingEdge(); // enable analog comparator interrupt on rising edge
void setFallingEdge(); // enable analog comparator interrupt on rising edge

#ifdef __cplusplus
} // extern "C"
#endif

#endif

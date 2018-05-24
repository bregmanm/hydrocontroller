#ifndef _ARDUINO_EMU_
#define _ARDUINO_EMU_

#include <stdint.h>

#define HIGH 0x1
#define LOW  0x0
#define INPUT 0x0
#define OUTPUT 0x1

#ifdef __cplusplus
extern "C"{
#endif

void pinMode(uint8_t, uint8_t);
void digitalWrite(uint8_t, uint8_t);

#ifdef __cplusplus
} // extern "C"
#endif

#endif

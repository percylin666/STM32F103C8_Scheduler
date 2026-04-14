#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

void LED_Init(void);
void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);

#ifdef __cplusplus
}
#endif

#endif /* __LED_H */
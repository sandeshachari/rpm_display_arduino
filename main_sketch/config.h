#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

/* Common configuration */
#define TEN_MS                              (10U)

/* Configuration for RPM sensing */
#define N_RPM_PULSE_PER_REV                 (4U)              /* Number of pulses per rotation */
#define RPM_PULSE_INPUT_PIN                 (2U)              /* Input pin for RPM pulse input */
#define RPM_PULSE_INPUT_PIN_MODE            (INPUT_PULLUP)    /* RPM pulse pin input mode. Options: INPUT, INPUT_PULLUP, OUTPUT */
#define RPM_PULSE_TRIGGER                   (RISING)          /* Interrupt trigger. Options: LOW, CHANGE, RISING, FALLING */
#define RPM_PULSE_TIMEOUT_MS                (1000U)           /* RPM pulse timeout. After this timeout, 0 RPM will be displayed */
#define USE_FILTER_FOR_RPM_MEAS             (1U)              /* Use (or do not use) RPM filter */
#define IIR_COEF_RPM_FILTER                 (0.9F)           /* IIR filter coefficient. Max value is 1 which means filter is very weak. Min value is 0 which means filer is very strong  */
#define GENERATE_RPM                        (1U)              /* Generate PWM output for simulating RPM signal */
#define GENERATE_RPM_PIN                    (8U)              /* Pin on which pulse will be generated for simulating RPM signal */
#define RPM_SET_POINT                       (42.67F)          /* Rpm set point */

/* Configuration for TM1637 display */
#define TM1637_DISPLAY_BRIGHTNESS           (0x7)             /* Display brightness. 0x7: highest brightness, 0x0: lowest brightness */
#define DISPLAY_UPDATE_RATE_MS              (100U)
#define CLK                                 (21U)
#define DIO                                 (20U)

#endif /* CONFIG_H */
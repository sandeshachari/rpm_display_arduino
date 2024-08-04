#include <Arduino.h>

/* Extern functions */
extern void rpm_setup(void);
extern void rpm_loop(void);
extern void display_setup(void);
extern void display_loop(void);


void setup() {
  // put your setup code here, to run once:
  rpm_setup();
  display_setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  rpm_loop();
  display_loop();
}

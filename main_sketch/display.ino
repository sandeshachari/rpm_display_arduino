#include <Arduino.h>
#include <TM1637Display.h>
#include "config.h"


TM1637Display display(CLK, DIO);

void display_setup() {
    /* Set highest brightness. 0: lowest, 7: highest */
    display.setBrightness(TM1637_DISPLAY_BRIGHTNESS, true);
    display.clear();
}

void display_loop() {
  uint32_t u32NumToDisplay = 0U;
  uint8_t u8Data[4] = {0U};
  static unsigned long ulDisplayUpdTime_ms = 0U;
  static unsigned long ulDisplayUpdTimeOld_ms = 0U;

  /* Record the current time (since the board is powered-up) in milliseconds  */
  ulDisplayUpdTime_ms = millis();

  /* Check if 100 ms time interval is over. The enclosed logic inside if loop runs every 100 ms. */
  if((ulDisplayUpdTime_ms - ulDisplayUpdTimeOld_ms) >= DISPLAY_UPDATE_RATE_MS)
  {
    u32NumToDisplay = get_cur_rpm_scaled();
#if 0
    for (int i = 4U; i > 0U; i--)
    {
      u8Data[i-1] = display.encodeDigit((uint8_t)(u32NumToDisplay & 0xFFU));
      u32NumToDisplay /= 10U;
    }
    display.setSegments(u8Data);
#else
    /* dots = 0b01000000 i.e. display colon
       leading zero = false
       length = 4 digits
       pos = 0 i.e. MSB is left most digit */
    display.showNumberDecEx(u32NumToDisplay, 0b01000000, false, 4U, 0U);
#endif

    /* Update old time */
    ulDisplayUpdTimeOld_ms = ulDisplayUpdTime_ms;
  }
}

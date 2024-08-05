#include <Arduino.h>
#include "config.h"

/* Local variables */
static unsigned int ulRpmTimeoutCount = 0U;
static unsigned int uiRpmPulseCount = 0U;

/* Global variables */
float fRpm = 0.0F;


void rpm_setup() {
  if(0 == digitalPinToInterrupt(RPM_PULSE_INPUT_PIN))
  {
    /* Interrupt number successfully assigned for RPM pulse input pin */
    pinMode(RPM_PULSE_INPUT_PIN, RPM_PULSE_INPUT_PIN_MODE);
    /* Install ISR for rising edge of pulse */
    attachInterrupt(digitalPinToInterrupt(RPM_PULSE_INPUT_PIN), rpm_sense_isr, RPM_PULSE_TRIGGER);
  }
  else
  {
    /* Interrupt assignment failed. Invalid interrupt number for RPM pulse input pin.  */
  }

#if(GENERATE_RPM == 1)
  pinMode(GENERATE_RPM_PIN, OUTPUT);
#endif  /* (GENERATE_RPM == 1) */
}


void rpm_sense_isr()
{
  static unsigned long ulRpmPulseTime_ms = 0U;
  static unsigned long ulRpmPulseTimeOld_ms = 0U;
  float fRpmRaw = 0.0F;

  ulRpmPulseTime_ms = millis();

  uiRpmPulseCount++;
  /* Check if one rotation is complete */
  if(N_RPM_PULSE_PER_REV <= uiRpmPulseCount)
  {
    /* Reset pulse count */
    uiRpmPulseCount = 0U;
    /* Calculate RPM */
    fRpmRaw = 60000.0F/(float)(ulRpmPulseTime_ms - ulRpmPulseTimeOld_ms);

#if(USE_FILTER_FOR_RPM_MEAS == 1)
    fRpm = IIR_COEF_RPM_FILTER*fRpmRaw + (1.0F - IIR_COEF_RPM_FILTER)*fRpm;
#elif(USE_FILTER_FOR_RPM_MEAS == 0)
    fRpm = fRpmRaw;
#else
#error 'Invalid value specified for macro USE_FILTER_FOR_RPM_MEAS. Valid values: 0, 1'
#endif  /*(USE_FILTER_FOR_RPM_MEAS == 1)*/

    /* Update rotation time */
    ulRpmPulseTimeOld_ms = ulRpmPulseTime_ms;
  }

  /* Reset timeout count */
  ulRpmTimeoutCount = 0U;
}


void rpm_loop(void)
{
  /* Check for pulse timeout. If pulse trigger does not come in the timeout period then reset RPM value. */
  static unsigned long ulRpmPulseTimeout_ms = 0U;
  static unsigned long ulRpmPulseTimeoutOld_ms = 0U;

  /* Record the current time (since the board is powered-up) in milliseconds  */
  ulRpmPulseTimeout_ms = millis();

  /* Check if 10 ms time interval is over. The enclosed logic inside if loop runs every 10 ms. */
  if((ulRpmPulseTimeout_ms - ulRpmPulseTimeoutOld_ms) >= TEN_MS)
  {
    /* Update  old time */
    ulRpmPulseTimeoutOld_ms = ulRpmPulseTimeout_ms;
    
    ulRpmTimeoutCount++;
    if(ulRpmTimeoutCount >= (RPM_PULSE_TIMEOUT_MS/TEN_MS))
    {
      ulRpmTimeoutCount = 0U;
      /* Reset RPM to 0 */
      fRpm = 0.0F;
      /* Reset count of pulses (in one rotation) */
      uiRpmPulseCount = 0U;
    }
  }

#if(GENERATE_RPM == 1)
  rpm_simulate();
#endif  /* (GENERATE_RPM == 1) */
}


uint32_t get_cur_rpm_scaled(void)
{
  return (uint32_t)(fRpm*100.0F);
}


#if(GENERATE_RPM == 1)
void rpm_simulate(void)
{
  static unsigned long ulRpmGenTime_ms = 0U;
  static unsigned long ulRpmGenTimeOld_ms = 0U;
  static bool bState = true;

  ulRpmGenTime_ms = millis();

  if( (ulRpmGenTime_ms - ulRpmGenTimeOld_ms) >= (uint32_t)(60000.0F/RPM_SET_POINT/2.0F/N_RPM_PULSE_PER_REV) )
  {
    bState = !bState;
    if(bState)
    {
      digitalWrite(GENERATE_RPM_PIN, 1U);
    }
    else
    {
      digitalWrite(GENERATE_RPM_PIN, 0U);
    }
    ulRpmGenTimeOld_ms = ulRpmGenTime_ms;
  }
}
#endif  /* (GENERATE_RPM == 1) */

#include <stdio.h>

/**
 * Project tested with an Arduino Nano.
 * This should work on any Arduino as long as output pin is PWM capable.
 * Send value (fan speed pourcent) via Arduino serial to set speed.
 **/

#define PWM_FAN_PIN 3
#define SERIAL_BAUDS 9600
/* PWM configuration */
// generate 25kHz PWM pulse rate on Pin 3
#define __TCCR2A 0x23    // COM2B1, WGM21, WGM20
#define __TCCR2B 0x0A    // WGM21, Prescaler = /8
#define __OCR2A  79      // TOP DO NOT CHANGE, SETS PWM PULSE RATE
#define __OCR2B  0       // duty cycle for Pin 3 (0-79) generates 1 500nS pulse even when 0 :(
// Renaming for easier usage
#define MAX_VAL  __OCR2A
#define MIN_VAL  __OCR2B
/* end PWM configuration*/
#define MESSAGE "Setting speed to :"
#define DELAY 1000

void setup() {
  Serial.begin(SERIAL_BAUDS);
  pinMode(PWM_FAN_PIN, OUTPUT);

  TCCR2A = __TCCR2A;
  // Set prescaler  
  TCCR2B = __TCCR2B;
  // Set TOP and initialize duty cycle to zero(0)
  OCR2A = __OCR2A;
  OCR2B = __OCR2B;
}

/**
 * Read from serial and set the value when received
 */
void loop() {
 byte percent;
 byte current_value;
 char message[32];

 // New value available on serial ?
 if(Serial.available() > 0) {
   percent = get_percent_from_serial();
   if(percent <= 100 && percent >= 0) {
     current_value = PWM_value_by_percent(&percent);
     sprintf(message, "%s %3d%% (%2d)\n", MESSAGE, percent, current_value);
     Serial.print(message);
     OCR2B = current_value;
   }
 }
 
 delay(DELAY);
}

/**
 * Get PWM value to set from a given percent value.
 * @param p pointer to a byte corresponding to required percent value
 * @return a PWM value as byte
 */
byte PWM_value_by_percent(const byte* p) {
  if(*p > 100)
    return (byte)MAX_VAL;
  else
    return (byte)(((*p) * MAX_VAL) / 100);
}

/**
 * Get percentage as byte from Arduino's serial.
 * @return A byte corresponding to user's input. return 255 if nothing is available (byte is unsigned).
 */
byte get_percent_from_serial() {
  char str[4] = "000";
  byte ret = 255;
  byte num_avail;

  str[3] = '\0';
  
  while ( (num_avail=Serial.available()) > 0)
    str[(3-num_avail) % 3] = (char)(Serial.read());

  // Converting to int
  ret = (byte)atoi(str);
  if(ret > 100)
    ret = 100;
  
  return ret;
}


/* Hamshield
 * Example: SerialController
 * This application is used in conjunction with a computer to provide full serial control of HamShield.
*/

#include <HamShield.h>

#define PWM_PIN 3
#define RESET_PIN A3
#define SWITCH_PIN 2

HamShield radio;

uint8_t freq_buffer[32];

void setup() {
  // NOTE: if not using PWM out, it should be held low to avoid tx noise
  pinMode(PWM_PIN, OUTPUT);
  digitalWrite(PWM_PIN, LOW);
  
  // prep the switch
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  
  // set up the reset control pin
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);
  
  Serial.begin(9600);
  Serial.println("If the sketch freezes at radio status, there is something wrong with power or the shield");
  Serial.print("Radio status: ");
  int result = radio.testConnection();
  Serial.println(result,DEC);
  Serial.println("Setting radio to its defaults..");
  radio.initialize();
  radio.setRfPower(0);
  radio.frequency(146520);
}

void loop() {
  if(Serial.available()) { 
    uint8_t buf = Serial.read();
    Serial.write(buf);
    switch (buf) {
      case 'X':                               // absorb reset command because we are already reset
        break;
      case 'F':                               // frequency configuration command
        tune_freq();  break;
      case 'P':                               // TX/RX PL Tone configuration command
        pl_tone();  break;
      case 'T':                               // turn on transmitter command
        tx_on();  break;
      case 'O':                               // turn off transmitter command
        tx_off();  break;
      case 'A':                               // configure amplifier
        amplifier();  break;
      case 'D':                               // configure predeemph 
        predeemph();  break;
      default:
        break;
    }
  }
}

void tx_on() { 
  radio.setModeTransmit();
  Serial.println("Transmitting");
}

void tx_off() { 
  radio.setModeReceive();
  Serial.println("Transmit off");
}

void tune_freq() { 
  Serial.println("program frequency mode");
  memset(freq_buffer,0,32);
  uint8_t ptr = 0;
  while(1) {
    if(Serial.available()) { 
       uint8_t buf = Serial.read();
       Serial.write(buf);
       if(buf == 'X') { return; }
       if(buf == '!') { freq_buffer[ptr] = 0; program_frequency(); return; } 
       if(buf != '.') { freq_buffer[ptr] = buf; ptr++; } 
       if(ptr == 31) { return; } 
    }
  }
}

void program_frequency() { 
  Serial.print("programming frequency to ");
  for(int x = 0; x < 32; x++) {
  Serial.write(freq_buffer[x]);
  }
  long freq = atol(freq_buffer);
  Serial.print(" Which is LONG of ");
  Serial.println(freq,DEC);
  radio.frequency(freq);
}


void pl_tone() { } 

void amplifier() {
   while(1) {
    if(Serial.available()) { 
       uint8_t buf = Serial.read();
       Serial.write(buf);
       if(buf == 'X') { return; }
       if(buf != '!') { radio.setRfPower(buf); return; } 
       if(buf == '!') { return; }
    }
  } 
  
  
  } 

void predeemph() { }



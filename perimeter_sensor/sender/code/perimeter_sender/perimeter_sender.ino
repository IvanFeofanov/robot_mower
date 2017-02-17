#include "./TimerOne.h"

#define PIN_IN1 2
#define PIN_IN2 3
#define PIN_LED 13
//#define FREQUENCY_TEST  
#define FREQUENCY 9615

/* const signed char sigcode [] = { 0, 1 }; */
/* const signed char sigcode [] = { 1,0,-1, 0,1,-1,1,-1, 0,1,-1,1,0,-1, 0,1,-1, 0,1,-1, 0,1,0,-1 }; */
const signed char sigcode [] = { 1,1,-1,-1,1,-1,1,-1,-1,1,-1,1,1,-1,-1,1,-1,-1,1,-1,-1,1,1,-1 };
volatile uint16_t index;

#ifdef FREQUENCY_TEST
volatile uint32_t last_time = 0;
volatile uint32_t n = 0;
#endif

void timerHandler();

void setup()
{
  Serial.begin(9600);
  
  //pio
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  
  //timer
  uint16_t period = 1000000 / FREQUENCY;
  Timer1.initialize(period);
  Timer1.attachInterrupt(timerHandler);
  index = 0;
  
  Serial.println("Perimeter sender\n");
  
  //print settings
  Serial.println("Settings:");
  Serial.print("pin_in1 = ");
  Serial.println(PIN_IN1);
  Serial.print("pin_in2 = ");
  Serial.println(PIN_IN2);
  Serial.print("sigcode size = ");
  Serial.println(sizeof(sigcode));
  Serial.print("frequency = ");
  Serial.println(FREQUENCY);
  Serial.print("\n");
}

void loop()
{
  #ifdef FREQUENCY_TEST
  if(millis() - last_time >= 1000){
    Serial.print("frequency test = ");
    Serial.println(n);
    last_time = millis();
    n = 0;
  }
  #endif
}

void timerHandler()
{
  #ifdef FREQUENCY_TEST
  n++;
  #endif
  
  switch(sigcode[index])
  {
    case -1:
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
    break;
    
    case 0:
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, LOW);
    break;
    
    case 1:
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
    break;
  }
  
  if(index + 1 < sizeof(sigcode)){
    index++;
  }else{
    index = 0;
  }
}

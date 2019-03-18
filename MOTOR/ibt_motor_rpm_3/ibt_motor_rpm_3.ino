#include "Ticker.h"

#include "ibt_motor.h"

IBT_Motor Hip(5, 6, 14, Serial); // IBT_Motor(int Pin_RPWM, int Pin_LPWM, int SensorPin);

void blink();
int tickTime = 1;
//Ticker timer1(blink, tickTime, 0, MICROS);
Ticker timer1(blink, tickTime);

bool ledState;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  timer1.start();
}

void loop()
{
  /* Testing for TURN CW AND CCW*/
  //    timer1.update();
  //    Hip.TurnCW(550, 220); //angle (0-1023); speed
  //    while (Hip._IsRotate!=STOP){
  //      timer1.update();
  //    }
  //    Hip.TurnCCW(800, 60); //angle (0-1023); speed
  //    while (Hip._IsRotate!=STOP){
  //      timer1.update();
  //    }

  /* Testing for GOTOANGLE*/
  Hip.GoToAngle(450, 125);
  while (Hip._IsRotate != STOP)
  {
    timer1.update();
    Serial.print(analogRead(18));Serial.print(" - "); //cw, 19 high, 18 low
    Serial.println(analogRead(19));
  }

  Hip.GoToAngle(700, 40);
  while (Hip._IsRotate != STOP)
  {
    timer1.update();
    Serial.print(analogRead(18)); Serial.print(" - ");//ccw, 19 low, 18 high
    Serial.println(analogRead(19));
  }
  

  /*check blink() running time*/
  //  long int test = micros ();
  //  blink();
  //  Serial.println(micros() - test);
}

void blink() {
  // update the ADC that has been filtered
  Hip.UpdateADC();
//  Serial.print(Hip.GetADC()); Serial.print(" "); Serial.println(Hip.GetFilteredADC()); //Serial.print(" "); Serial.println(Hip.GetOmega());
  int delta = abs(Hip.GetTarget() - Hip.GetADC()); //*0.29325513196;
  if (delta > 2)
  {
    Hip.Driver(Hip.GetRotate(), Hip.GetSpeed());
  } else
  {
    Hip.Driver(STOP, Hip.GetSpeed());
    Hip._IsRotate = STOP;
  }

  //testing blink for timing
  digitalWrite(LED_BUILTIN, ledState);
  ledState = !ledState;
}

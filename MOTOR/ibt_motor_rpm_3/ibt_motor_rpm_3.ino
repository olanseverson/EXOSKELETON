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
  //  timer1.update();
  //  Hip.TurnCW(550, 220); //angle (0-1023); speed
  //  while (Hip._IsRotate!=STOP){
  //    timer1.update();
  //  }
  //  Hip.TurnCCW(800, 60); //angle (0-1023); speed
  //  while (Hip._IsRotate!=STOP){
  //    timer1.update();
  //  }

  /* Testing for GOTOANGLE*/
  //  Hip.GoToAngle(550, 200);
  //  while (Hip._IsRotate != STOP)
  //  {
  //    timer1.update();
  //  }
  //
  //  Hip.GoToAngle(800, 100);
  //  while (Hip._IsRotate != STOP)
  //  {
  //    timer1.update();
  //  }

  long int test = micros ();
  blink();
  Serial.println(micros() - test);
}

void blink() {
  digitalWrite(LED_BUILTIN, ledState);
  ledState = !ledState;
  Hip.UpdateADC();
  Serial.print(Hip.GetADC()); Serial.print(" "); Serial.println(Hip.GetFilteredADC());
}

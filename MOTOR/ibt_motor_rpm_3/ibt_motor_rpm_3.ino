#include "Ticker.h"

#include "ibt_motor.h"

IBT_Motor Hip(5, 6, 14, Serial); // IBT_Motor(int Pin_RPWM, int Pin_LPWM, int SensorPin);

void blink();
int tickTime = 1;
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
  Hip.GoToAngle(400, 110); // 100
  while (Hip._IsRotate != STOP)
  {
    timer1.update();
    //        Serial.print(analogRead(18));Serial.print(" - "); //cw, 19 high, 18 low
    //        Serial.println(analogRead(19));
  }

  Hip.GoToAngle(750, 60);
  while (Hip._IsRotate != STOP)
  {
    timer1.update();
    //        Serial.print(analogRead(18)); Serial.print(" - ");//ccw, 19 low, 18 high
    //        Serial.println(analogRead(19));
  }


  /*check blink() running time*/
  //    long int test = micros ();
  //    blink();
  //    Serial.println(micros() - test);
}

void blink() {
  // update the ADC that has been filtered
  Hip.UpdateADC();
  if (timer1.counter() % 25 == 0 && timer1.counter() > 250)
  {
    Hip.UpdateOmega();
    Hip.PIDController(0.6368, 0, 1.336);
  }
  //  Serial.print(Hip.GetRotate()); Serial.println(" ");
  //  Serial.print(Hip.GetADC()); Serial.print(" ");
  //  Serial.print(Hip.GetFilteredADC()); Serial.print(" ");

  int delta = abs(Hip.GetTarget() - Hip.GetADC());
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

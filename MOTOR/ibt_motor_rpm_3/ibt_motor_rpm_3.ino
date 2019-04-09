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

  /* Testing for GOTOANGLE*/
  int pwm = 150;
  Hip.GoToAngle(450, 150); // 100
  while (Hip._IsRotate != STOP)
  {
    timer1.update();
  }
  Hip.PID_i = 0;
  Hip.SetSpeed(150);

  Hip.GoToAngle(650, 60);
  while (Hip._IsRotate != STOP)
  {
    timer1.update();
  }


  /*check blink() running time*/
  //    long int test = micros ();
  //    blink();
  //    Serial.println(micros() - test);
}

void blink() {
  // update the ADC that has been filtered
  Hip.UpdateADC();
  if (timer1.counter() % 25 == 0)
  {
    Hip.UpdateOmega();
    if ( Hip.GetRotate () == 0)
    {
      Hip.PIDController(0.01508, 0, 1.207);

    }

    Serial.print(" - ");
  }
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
//  Serial.print(Hip.GetADC()); Serial.print(" ");
//  Serial.print(Hip.GetFilteredADC()); Serial.print(" ");
//  Serial.print(Hip.GetRotate()); Serial.println(" ");
}

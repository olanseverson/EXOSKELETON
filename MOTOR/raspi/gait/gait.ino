#include "Ticker.h"
#include "ibt_motor.h"
#include "Wire.h"

/*** MOTOR ***/

IBT_Motor Hip(5, 6, 14, Serial); // IBT_Motor(int Pin_RPWM, int Pin_LPWM, int SensorPin);

/*** TICKER TIMER ***/
void blink();
int tickTime = 1;
Ticker timer1(blink, tickTime);
bool ledState;

/*** TWI ***/
//#define SLAVE_ADDRESS 0x04
#define SLAVE_ADDRESS 0x05
//#define SLAVE_ADDRESS 0x06

int number[10] = {
  0, 150, 170, 0, 0, 0, 0, 0, 0, 0
};
int IsDone = 0; //CHECK IF THE MOTOR ALREADY IN IDLE MODE
static int cnt;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);// start serial for output

  /** TICKER TIMER INIT **/
  timer1.start(); //start the ticker timer


  /** TWI INIT **/
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  cnt = 0;

  Serial.println("Ready!");
  while (number[0] == 0) {
    Serial.println("waiting...");
  }
}

void loop()
{

//  Serial.println("oke");
//  Hip.GoToAngle(number[1] * 4, 50);
//  while (Hip._IsRotate != STOP)
//  {
//    timer1.update();
//
//  }
//  number[0] = 0;

  /* Testing for GOTOANGLE*/
    int pwm = 150;
    Hip.GoToAngle(number[1] * 4, 60); // 100 number[1] * 4
    while (Hip._IsRotate != STOP)
    {
      timer1.update();
    }
    //  Hip.PID_i = 0;
    //  Hip.SetSpeed(150);
  
    Hip.GoToAngle(number[2] * 4, 30);
    while (Hip._IsRotate != STOP)
    {
      timer1.update();
    }

  /*check blink() running time*/
  //     long int test = micros ();
  //     blink();
  //      Serial.println(micros() - test);
}

/**============= TICKER TIMER FUNCTION ==============**/
void blink() {
  Serial.print(Hip.GetFilteredADC()); Serial.print(" "); Serial.print(number[1] * 4); Serial.print(" "); Serial.println(number[2] * 4);

  // update the ADC that has been filtered
  Hip.UpdateADC();
  if (timer1.counter() % 25 == 0)
  {
    Hip.UpdateOmega();
    if ( Hip.GetRotate () == 0)
    {
      Hip.PIDController(0.01508, 0, 1.207);

    }

    //    Serial.print(number[1]);
  }
  int delta = abs(Hip.GetTarget() - Hip.GetADC());
  if (delta > 2)
  {
    Hip.Driver(Hip.GetRotate(), Hip.GetSpeed());
  }
  else
  {
    Hip.Driver(STOP, Hip.GetSpeed());
    Hip._IsRotate = STOP;
  }

  //testing blink for timing
  digitalWrite(LED_BUILTIN, ledState);
  ledState = !ledState;
  //  Serial.print(Hip.GetADC()); Serial.print(" ");
  //  Serial.print(Hip.GetFilteredADC()); Serial.println(" ");
  //  Serial.print(Hip.GetRotate()); Serial.println(" ");
}


/**============= RASPI-I2C FUNCTION ==============**/
// callback for received data
void receiveData(int byteCount) {
  while (Wire.available()) {
    //      Serial.print(i);
    //      Serial.print("=>");
    number[cnt] = Wire.read();
    //      Serial.print("data received: ");
    Serial.println(number[cnt]);
    cnt++;
    if (cnt == byteCount) {
      cnt = 0;
    }
  }
}

// callback for sending data
void sendData() {
  Wire.write(number[1]);
}

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
#define SLAVE_ADDRESS 0x20

int number[10] = {
  0, 400, 600, 0, 0, 0, 0, 0, 0, 0
};
int IsDone = 0; //CHECK IF THE MOTOR ALREADY IN IDLE MODE
static int cnt;
bool isReceived = false;


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
//  while (number[0] == 0) {
//    Serial.println("waiting...");
//  }
}

void loop()
{

//  if (isReceived) {
    
//    Serial.print(" ");
//    Serial.println(number[1]);
//    isReceived = false;
//  }
  /* Testing for GOTOANGLE*/
//  int pwm = 150;
  Hip.GoToAngle(number[1], 60); // 100 number[1] * 4
  Serial.println(Hip._IsRotate);
  while (Hip._IsRotate != STOP)
  {
    timer1.update();
  }
//  //  Hip.PID_i = 0;
//  //  Hip.SetSpeed(150);
//
//  Hip.GoToAngle(number[2], 30);
//  while (Hip._IsRotate != STOP)
//  {
//    timer1.update();
//  }

  /*check blink() running time*/
  //     long int test = micros ();
  //     blink();
  //      Serial.println(micros() - test);
}

/**============= TICKER TIMER FUNCTION ==============**/
void blink() {
    Serial.print(Hip.GetFilteredADC()); Serial.print(" "); Serial.print(number[1]); Serial.print(" "); Serial.println(Hip._IsRotate);

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
  int delta = Hip.GetTarget() - Hip.GetADC();
  if (abs(delta) > Hip.GetTolerance())
  {
    rotateState rotate = CW;
    if (delta>0) {rotate = CCW;}
    Hip.Driver(rotate, Hip.GetSpeed());
  }
  else
  {
    Hip.Driver(STOP, Hip.GetSpeed());
    Hip._IsRotate = STOP;
  }

  //testing blink for timing
  digitalWrite(LED_BUILTIN, ledState);
  ledState = !ledState;
}


/**============= RASPI-I2C FUNCTION ==============**/
// callback for received data
word receiveWord(int byteCount) {
  byte number[2] ;
  number[0] = Wire.read();
  number[1] = Wire.read();
  return (number[0]<<8)|number[1];
}

void receiveData(int byteCount) {
  Hip._IsRotate = STOP;
  while (Wire.available()) {
    number[1] = receiveWord(byteCount);
    Serial.println(number[1]);
    cnt++;
    if (cnt == byteCount) {
      cnt = 0;
    }
  }
  isReceived = true;
}

// callback for sending data
void sendData() {
  Wire.write(Hip._IsRotate != STOP);
}

/*
  ||
  || @file ibt_motor.h
  || @version 1.2
  || @author Yoland Nababan
  ||
  || @description
  || | Drive motor using BTS7960
  || | https://electronics.stackexchange.com/questions/398556/how-to-control-a-motor-driver-bts7960-without-pwm
  || #
  ||
*/
/*
  IBT-2 Motor Control Board driven by Arduino.

  Speed and direction controlled by a potentiometer attached to analog input 0.
  One side pin of the potentiometer (either one) to ground; the other side pin to +5V

  Connection to the IBT-2 board:
  IBT-2 pin 1 (RPWM) to Arduino pin 5(PWM)
  IBT-2 pin 2 (LPWM) to Arduino pin 6(PWM)
  IBT-2 pins 3 (R_EN), 4 (L_EN), 7 (VCC) to Arduino 5V pin
  IBT-2 pin 8 (GND) to Arduino GND
  IBT-2 pins 5 (R_IS) and 6 (L_IS) not connected
*/

/* ADC FOR POTENTIOMETER
   FULLY CCW = 1023
   FULLY CW  = 0
*/

#ifndef IBT_MOTOR_H
#define IBT_MOTOR_H


#include <Arduino.h>

#define MA_COEFF 100
//#define DEBUG_ON  //comment to disable debug()

enum rotateState {CW, CCW, STOP}; // 0 1 2

// LPWM high, L IS high 

class IBT_Motor
{
  private:
    volatile long int _ADC_Temp;
    volatile int _ADC;
    volatile int _filteredADC;
    volatile int _prevADC ;
    int BUFFER[MA_COEFF];
    int idxBuff;


  public:
    //methods
    IBT_Motor(int Pin_RPWM, int Pin_LPWM, int SensorPin, Stream &serial);
    void TurnCCW(int maxValue, int Speed);
    void TurnCW(int minValue, int Speed);
    void Driver(enum rotateState, int Speed);
    void GoToAngle(int toAngle, int Speed);
    void debug();
    void Brake(bool isHigh);

    /*GETTER*/
    int GetADC(){return this->_ADC;}
    int GetFilteredADC(){return _filteredADC;}
    float GetAngle(){return map(analogRead(_SensorPin), 0, 1023, 0, 300);}
    int GetTarget(){return _target;}
    rotateState GetRotate() {return _IsRotate;}
    int GetSpeed(){return _speed;}
    int GetOmega(){return _omega;}

    /*SETTER*/
    void SetRotate(rotateState Rotate){_IsRotate = Rotate; }
//    void SetOmega (){_omega = _filteredADC - prevADC;} 

    // variable
    Stream &serial;

    // dummy
    void UpdateADC();
    rotateState _IsRotate = STOP;
    volatile int _speed;
    int _target;
    int _RPWM;
    int _LPWM;
    int _SensorPin;

    int _omega;
    int _prevOmega;
};

#endif
/*
  || @changelog
  || | 1.2 2019-03-18 - Yoland Nababan : Add Getter and implement Driver
  || | 1.1 2019-03-16 - Yoland Nababan : FILTERING ADC using Moving Average
  || | 1.0 2019-03-13 - Yoland Nababan : Using ticker timer to sampling (https://github.com/sstaub/Ticker)
  || | 1.0 2019-03-13 - Yoland Nababan : Initial Release
  || #
*/

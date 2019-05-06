/*
  ||
 || @file ibt_motor.cpp
 || @version 1.2
 || @author Yoland Nababan
 ||
 || @description
 || | Drive motor using BTS7960
 || | https://electronics.stackexchange.com/questions/398556/how-to-control-a-motor-driver-bts7960-without-pwm
 || #
 ||
 ||
 */
#include "ibt_motor.h"

//Constructor
IBT_Motor::IBT_Motor(int Pin_RPWM, int Pin_LPWM, int SensorPin, Stream &serial):
_RPWM(Pin_RPWM),
_LPWM(Pin_LPWM),
_SensorPin(SensorPin),
serial(serial)
{
  _filteredADC = analogRead(_SensorPin);
  _prevADC = _filteredADC;
  for (int i = 0; i < MA_COEFF; i++)
  {
    BUFFER[i] = _filteredADC;
  }
  idxBuff = 0;
  _omegaMax = 0;
  PID_p = 0;
  PID_i = 0;
  PID_value = 0;
}

void IBT_Motor::debug()
{
  serial.print("Pwm : "); 
  serial.print(_ADC);
  serial.print(" Angle: "); 
  serial.println((_ADC / 1023.0) * 300);
}

void IBT_Motor::Brake(bool isHigh)
{
  if (isHigh)
  {
    analogWrite(_LPWM, 255);
    analogWrite(_RPWM, 255);
  } 
  else
  {
    analogWrite(_LPWM, 0);
    analogWrite(_RPWM, 0);
  }
}



void IBT_Motor::TurnCW(int minValue, int Speed)
{
  this->_target = minValue;
  this->_speed = Speed;
  this->_IsRotate = CW;
#ifdef DEBUG_ON
  serial.print("CW ");
  debug();
#endif
}

void IBT_Motor::TurnCCW(int maxValue, int Speed)
{
  this->_target = maxValue;
  this->_speed = Speed;
  this->_IsRotate = CCW;
#ifdef DEBUG_ON
  serial.print("CCW ");
  debug();
#endif
}

void IBT_Motor::Driver(rotateState IsRotate, int Speed = 255)
{
  switch (IsRotate)
  {
  case CCW :
    analogWrite(_LPWM, 0);
    analogWrite(_RPWM, Speed);
    break;
  case CW :
    analogWrite(_LPWM, Speed);
    analogWrite(_RPWM, 0);
    break;
  case STOP :
    analogWrite(_LPWM, Speed);
    analogWrite(_RPWM, Speed);
    break;
  }
}

void IBT_Motor::GoToAngle(int toAngle, int Speed)
// Speed   : 0 - 255
// toAngle : 0 - 1023 degree
{
  this->_ADC = analogRead(_SensorPin);
  int delta = toAngle - _ADC;
  _target = toAngle;
  _speed = Speed;
  if (delta > 2)
  {
    _IsRotate = CCW;
  } 
  else if (delta < -2)
  {
    _IsRotate = CW;
  } 
  else
  { // stop
    _IsRotate = STOP;
  }
}
void IBT_Motor::UpdateADC()
{
  this->_ADC = analogRead(_SensorPin);
  BUFFER[idxBuff] = _ADC;
  idxBuff++;
  if (idxBuff == MA_COEFF)
  {
    idxBuff = 0;
  }
  long int temp = 0;
  for (int i = 0; i < MA_COEFF; i++)
  {
    temp += BUFFER[i];
  }
  _filteredADC = temp / MA_COEFF;
}

void IBT_Motor::UpdateOmega()
{
  _omega = _filteredADC - _prevADC;
  _prevADC = _filteredADC;
}

void IBT_Motor::PIDController (double Kp, double Kd, double Ki)
{
  double setpoint = _speed / 255.0;
  double omega =  abs(_omega / 14.0);
  double error = setpoint - omega ;

  PID_i = PID_i + error;
  PID_d = error - prev_error;
  prev_error = error;
  PID_value = (int)(error * Kp + PID_i * Ki + PID_d * Kd);

  //    serial.print(" rpm: ");serial.print(_omega);
  //    serial.print(" setpoint: ");serial.print(setpoint);
  //    serial.print(" omega: "); serial.print(omega);
  //  serial.print(" error: "); serial.print(error);
  //  serial.print(" integral: "); serial.print(PID_i);
  //  serial.print(" value: "); serial.println(PID_value);


  PID_value = (PID_value > 100) ? 100 : PID_value;
  PID_value = (PID_value < 0) ? 0 : PID_value;
  PID_value = PID_value * 255 / 100;
}

/*
  || @changelog
 || | 1.2 2019-03-20 - Yoland Nababan : Add UpdateOmega
 || | 1.2 2019-03-18 - Yoland Nababan : Add Getter and implement Driver
 || | 1.1 2019-03-16 - Yoland Nababan : FILTERING ADC using Moving Average
 || | 1.0 2019-03-13 - Yoland Nababan : Using ticker timer to sampling (https://github.com/sstaub/Ticker)
 || | 1.0 2019-03-13 - Yoland Nababan : Initial Release
 || #
 */

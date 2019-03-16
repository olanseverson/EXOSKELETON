/*
  ||
  || @file ibt_motor.cpp
  || @version 1.1
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
  for (int i = 0; i < MA_COEFF; i++)
  {
    BUFFER[i] = _filteredADC;
  }
  idxBuff = 0;
}

void IBT_Motor::debug()
{
  serial.print("Pwm : "); serial.print(_ADC);
  serial.print(" Angle: "); serial.println((_ADC / 1023.0) * 300);
}

void IBT_Motor::Brake(bool isHigh)
{
  if (isHigh)
  {
    analogWrite(_LPWM, 255);
    analogWrite(_RPWM, 255);
  } else
  {
    analogWrite(_LPWM, 0);
    analogWrite(_RPWM, 0);
  }
}

int IBT_Motor::GetADC() {
  return this->_ADC;
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
  _filteredADC = temp/MA_COEFF;
}

float IBT_Motor::GetAngle()
{
  return map(analogRead(_SensorPin), 0, 1023, 0, 300);
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
  } else if (delta < -2)
  {
    _IsRotate = CW;
  } else
  { // stop
    _IsRotate = STOP;
  }

}

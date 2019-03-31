#ifdef REAL_M5STACK
#include <M5Stack.h>
#include "AnalogJoystick.h"

AnalogJoystick::AnalogJoystick()
{
  pos = 0;
  pinA = pinB = 0;
}

void AnalogJoystick::Setup(int a, int b)
{
  pinA = a;
  pinB = b;
}

void AnalogJoystick::Update()
{
  if (pinA == 0 || pinB == 0) {
    return;
  }
  pos %= AnalogJoystickMaxBuffer;
  bufferX[pos] = analogRead(pinA);
  bufferY[pos] = analogRead(pinB);
  pos ++;
}

int AnalogJoystick::GetRaw(const int buffer[], int count) const
{
  int sum = 0;
  for (int i = 0; i < count; i ++) {
    sum += buffer[i];
  }
  return sum / count;
}

int AnalogJoystick::GetX() const
{
  if (centerX - minX < threshold || maxX - centerX < threshold || maxX - minX < threshold) {
    return 0;
  }
  int v = GetRawX();
  if (v > centerX + threshold) {
    v = v > maxX ? maxX : v;
    v -= centerX;
    v *=  rangeMax / (maxX - centerX);
  } else if (v < centerX - threshold) {
    v = v < minX ? minX : v;
    v = centerX - v;
    v *=  rangeMin / (centerX - minX);
  } else {
    v = 0;
  }
  return v;
}

int AnalogJoystick::GetY() const
{
  if (centerY - minY < threshold || maxY - centerY < threshold || maxY - minY < threshold) {
    return 0;
  }
  int v = GetRawY();
  if (v > centerY + threshold) {
    v = v > maxY ? maxY : v;
    v -= centerY;
    v *=  rangeMax / (maxY - centerY);
  } else if (v < centerY - threshold) {
    v = v < minY ? minY : v;
    v = centerY - v;
    v *=  rangeMin / (centerY - minY);
  } else {
    v = 0;
  }
  return v;
}

void AnalogJoystick::CalibrateCenter(int msec)
{
  unsigned long end = millis() + msec;
  int sumX = 0, sumY = 0, count = 0;

  while (millis() < end) {
    sumX += analogRead(pinA);;
    sumY += analogRead(pinB);;
    count ++;
    delay(10);
  }
  centerX = sumX / count;
  centerY = sumY / count;
}

bool AnalogJoystick::CalibrateMinMax(int msec)
{
  unsigned long end = millis() + msec;

  for (int i = 0; i < AnalogJoystickMaxBuffer; i ++) {
    Update();
  }
  minX = minY = 9999;
  maxX = maxY = 0;
  while (millis() < end) {
    Update();
    int x = GetRawX();
    int y = GetRawY();
    minX = x < minX ? x : minX;
    maxX = x > maxX ? x : maxX;
    minY = y < minY ? y : minY;
    maxY = y > maxY ? y : maxY;
  }
  return minX != centerX && maxX != centerX && minY != centerY && maxY != centerY;
}

void AnalogJoystick::Print() const
{
  M5.Lcd.printf("x: %d, %d, %d\n", minX, centerX, maxX);
  M5.Lcd.printf("   %d - %d\n", rangeMin, rangeMax);
  M5.Lcd.printf("y: %d, %d, %d\n", minY, centerY, maxY);
  M5.Lcd.printf("   %d - %d\n", rangeMin, rangeMax);
}

#endif /* REAL_M5STACK */


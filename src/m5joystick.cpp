#ifdef REAL_M5STACK
#include <M5Stack.h>
#include "m5joystick.h"

/*
 * Joystick base class
 */

Joystick::Joystick(int _maxBuffer) : maxBuffer(_maxBuffer)
{
  bufferX = new int[maxBuffer];
  bufferY = new int[maxBuffer];
  pos = 0;
}

Joystick::~Joystick()
{
  if (bufferX) {
    delete[] bufferX;
    bufferX = NULL;
  }
  if (bufferY) {
    delete[] bufferY;
    bufferY = NULL;
  }
}

int Joystick::GetAverage(const int buffer[], int count) const
{
  int sum = 0;
  for (int i = 0; i < count; i ++) {
    sum += buffer[i];
  }
  return sum / count;
}

bool Joystick::Update()
{
  int x, y;

  if (!GetRaw(&x, &y)) {
    return false;
  }
  pos %= maxBuffer;
  bufferX[pos] = x;
  bufferY[pos] = y;
  pos ++;
  return true;
}

int Joystick::GetX() const
{
  if (centerX - minX < threshold || maxX - centerX < threshold || maxX - minX < threshold) {
    return 0;
  }
  int v = GetAverageX();
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

int Joystick::GetY() const
{
  if (centerY - minY < threshold || maxY - centerY < threshold || maxY - minY < threshold) {
    return 0;
  }
  int v = GetAverageY();
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

void Joystick::CalibrateCenter(int msec)
{
  unsigned long end = millis() + msec;
  int sumX = 0, sumY = 0, count = 0;

  for (int i = 0; i < maxBuffer; i ++) {
    Update();
    delay(10);
  }
  while (millis() < end) {
    if (Update()) {
      sumX += GetAverageX();
      sumY += GetAverageY();
      count ++;
    }
    delay(10);
  }
  centerX = sumX / count;
  centerY = sumY / count;
}

bool Joystick::CalibrateMinMax(int msec)
{
  unsigned long end = millis() + msec;

  minX = minY = 9999;
  maxX = maxY = 0;
  while (millis() < end) {
    if (Update()) {
      int x = GetAverageX();
      int y = GetAverageY();
      minX = x < minX ? x : minX;
      maxX = x > maxX ? x : maxX;
      minY = y < minY ? y : minY;
      maxY = y > maxY ? y : maxY;
    }
  }
  return minX != centerX && maxX != centerX && minY != centerY && maxY != centerY;
}

/*
 * Accelerometer Joystick
 */

AccJoystick::~AccJoystick()
{
  if (bufferX) {
    delete[] bufferX;
    bufferX = NULL;
  }
  if (bufferY) {
    delete[] bufferY;
    bufferY = NULL;
  }
  if (mpu) {
    delete mpu;
    mpu = NULL;
  }
}

void AccJoystick::Setup(TwoWire &w)
{
  if (!mpu) {
    mpu = new MPU6050(w);
    mpu->begin();
  }
  minX = minY = fmin * ratio;
  maxX = maxY = fmax * ratio;
}

bool AccJoystick::GetRaw(int *px, int *py) const
{
  mpu->update();
  float x = mpu->getAccX();
  x = x < fmin ? fmin : x;
  x = x > fmax ? fmax : x;
  *px = - x * ratio;
  float y = mpu->getAccY();
  y = y < fmin ? fmin : y;
  y = y > fmax ? fmax : y;
  *py = y * ratio;
  return true;
}

void AccJoystick::CalibrateCenter(int msec)
{
  mpu->calcGyroOffsets(false);
  Joystick::CalibrateCenter(msec);
}

void AccJoystick::Print() const
{
  M5.Lcd.printf("x: %d, %d, %d\n", minX, centerX, maxX);
  M5.Lcd.printf("   %d - %d\n", rangeMin, rangeMax);
  M5.Lcd.printf("y: %d, %d, %d\n", minY, centerY, maxY);
  M5.Lcd.printf("   %d - %d\n", rangeMin, rangeMax);
}

/*
 * I2C Joystick
 */

void I2CJoystick::Setup(TwoWire &w)
{
  wire = &w;
}

bool I2CJoystick::GetRaw(int *px, int *py) const
{
  wire->requestFrom(JOY_ADDR, 3);
  if (!wire->available()) {
    return false;
  }
  *px = wire->read();
  *py = - wire->read();
  wire->read();
  return true;
}

void I2CJoystick::Print() const
{
  M5.Lcd.printf("x: %d, %d, %d\n", minX, centerX, maxX);
  M5.Lcd.printf("   %d - %d\n", rangeMin, rangeMax);
  M5.Lcd.printf("y: %d, %d, %d\n", minY, centerY, maxY);
  M5.Lcd.printf("   %d - %d\n", rangeMin, rangeMax);
}

/*
 * Analog Joystick
 */

void AnalogJoystick::Setup(int a, int b)
{
  pinA = a;
  pinB = b;
}

bool AnalogJoystick::GetRaw(int *px, int *py) const
{
  if (pinA == 0 || pinB == 0) {
    return false;
  }
  *px = analogRead(pinA);
  *py = analogRead(pinB);
  return true;
}

void AnalogJoystick::Print() const
{
  M5.Lcd.printf("x: %d, %d, %d\n", minX, centerX, maxX);
  M5.Lcd.printf("   %d - %d\n", rangeMin, rangeMax);
  M5.Lcd.printf("y: %d, %d, %d\n", minY, centerY, maxY);
  M5.Lcd.printf("   %d - %d\n", rangeMin, rangeMax);
}

#endif /* REAL_M5STACK */

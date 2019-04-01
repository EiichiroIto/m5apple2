#ifdef REAL_M5STACK
#include <M5Stack.h>
#include "AccJoystick.h"

MPU6050 mpu6050(Wire);

AccJoystick::AccJoystick()
{
  mpu = NULL;
  pos = 0;
}

AccJoystick::~AccJoystick()
{
  if (mpu) {
    delete mpu;
  }
}

void AccJoystick::Setup(TwoWire &w)
{
  if (!mpu) {
    mpu = new MPU6050(w);
    mpu->begin();
  }
}

void AccJoystick::Update()
{
  mpu->update();
  pos %= AccJoystickMaxBuffer;
  bufferX[pos] = mpu->getAccX();
  bufferY[pos] = mpu->getAccY();
  pos ++;
}

float AccJoystick::GetRaw(const float buffer[], int count) const
{
  float sum = 0;
  for (int i = 0; i < count; i ++) {
    sum += buffer[i];
  }
  sum /= (float) count;
  sum = sum < min ? min : sum;
  sum = sum > max ? max : sum;
  return sum;
}

int AccJoystick::GetX() const
{
  if (centerX - min < threshold || max - centerX < threshold || max - min < threshold) {
    return 0;
  }
  float v = GetRawX();
  if (v > centerX + threshold) {
    v -= centerX;
    v *=  rangeMaxX / (max - centerX);
  } else if (v < centerX - threshold) {
    v = centerX - v;
    v *=  rangeMinX / (centerX - min);
  } else {
    v = 0;
  }
  return (int) - v;
}

int AccJoystick::GetY() const
{
  if (centerY - min < threshold || max - centerY < threshold || max - min < threshold) {
    return 0;
  }
  float v = GetRawY();
  if (v > centerY + threshold) {
    v -= centerY;
    v *=  rangeMaxY / (max - centerY);
  } else if (v < centerY - threshold) {
    v = centerY - v;
    v *=  rangeMinY / (centerY - min);
  } else {
    v = 0;
  }
  return (int) v;
}

void AccJoystick::CalibrateCenter(int msec)
{
  unsigned long end;
  float sumX = 0, sumY = 0;
  int count = 0;

  mpu->calcGyroOffsets(false);
  end = millis() + msec;
  while (millis() < end) {
    sumX += mpu->getAccX();
    sumY += mpu->getAccY();
    count ++;
    delay(10);
  }
  centerX = sumX / count;
  centerY = sumY / count;
}

void AccJoystick::Print() const
{
  M5.Lcd.printf("x: %f, %f, %f\n", min, centerX, max);
  M5.Lcd.printf("   %d - %d\n", rangeMinX, rangeMaxX);
  M5.Lcd.printf("y: %f, %f, %f\n", min, centerY, max);
  M5.Lcd.printf("   %d - %d\n", rangeMinY, rangeMaxY);
}

#endif /* REAL_M5STACK */


#pragma once
#include <MPU6050_tockn.h>
#include <Wire.h>

#define AccJoystickMaxBuffer 10

class AccJoystick {
  private:
    MPU6050 *mpu;
    const float threshold = 0.1;
    const float min = -0.5, max = 0.5;
    const int rangeMinX = -32768, rangeMaxX = 32767;
    const int rangeMinY = -32768, rangeMaxY = 32767;
    float centerX, centerY;
    int pos;
    float bufferX[AccJoystickMaxBuffer];
    float bufferY[AccJoystickMaxBuffer];
    float GetRaw(const float buffer[], int count) const;

  public:
    AccJoystick();
    ~AccJoystick();
    void Setup(TwoWire &w);
    void Update();
    float GetRawX() const { return GetRaw(bufferX, AccJoystickMaxBuffer); };
    float GetRawY() const { return GetRaw(bufferY, AccJoystickMaxBuffer); };
    int GetX() const;
    int GetY() const;
    void CalibrateCenter(int msec);
    void Print() const;
};


#pragma once

#define AnalogJoystickMaxBuffer 10

class AnalogJoystick {
  private:
    const int rangeMin = -32768, rangeMax = 32767;
    const int threshold = 30;
    int pinA, pinB;
    int minX, maxX, centerX;
    int minY, maxY, centerY;
    int pos;
    int bufferX[AnalogJoystickMaxBuffer];
    int bufferY[AnalogJoystickMaxBuffer];
    int GetRaw(const int buffer[], int count) const;

  public:
    AnalogJoystick();
    void Setup(int a, int b);
    void Update();
    int GetRawX() const { return GetRaw(bufferX, AnalogJoystickMaxBuffer); };
    int GetRawY() const { return GetRaw(bufferY, AnalogJoystickMaxBuffer); };
    int GetX() const;
    int GetY() const;
    void CalibrateCenter(int msec);
    bool CalibrateMinMax(int msec);
    void Print() const;
};


#pragma once

class Joystick {
  protected:
    const int rangeMin = -32768, rangeMax = 32767;
    const int threshold = 30;
    int maxBuffer;
    int minX, maxX, centerX;
    int minY, maxY, centerY;
    int pos;
    int *bufferX;
    int *bufferY;
    virtual bool GetRaw(int *px, int *py) const = 0;
    int GetAverage(const int buffer[], int count) const;
    int GetAverageX() const { return GetAverage(bufferX, maxBuffer); };
    int GetAverageY() const { return GetAverage(bufferY, maxBuffer); };

  public:
    Joystick(int _maxBuffer);
    ~Joystick();
    bool Update();
    int GetX() const;
    int GetY() const;
    virtual void CalibrateCenter(int msec);
    bool CalibrateMinMax(int msec);
    virtual void Print() const = 0;
};

#include <MPU6050_tockn.h>
#include <Wire.h>

class AccJoystick : public Joystick {
  private:
    MPU6050 *mpu;
    const float fmin = -0.5, fmax = 0.5;
    const float ratio = 1000.0;
    bool GetRaw(int *px, int *py) const;

  public:
    AccJoystick(int _maxBuffer) : Joystick(_maxBuffer), mpu(NULL) {}
    ~AccJoystick();
    void Setup(TwoWire &w);
    void CalibrateCenter(int msec);
    void Print() const;
};

#define JOY_ADDR 0x52

class I2CJoystick : public Joystick {
  private:
    TwoWire *wire;
    bool GetRaw(int *px, int *py) const;

  public:
    I2CJoystick(int _maxBuffer) : Joystick(_maxBuffer) {}
    void Setup(TwoWire &w);
    void Print() const;
};

class AnalogJoystick : public Joystick {
  private:
    int pinA, pinB;
    bool GetRaw(int *px, int *py) const;

  public:
    AnalogJoystick(int _maxBuffer) : Joystick(_maxBuffer), pinA(0), pinB(0) {}
    void Setup(int a, int b);
    void Print() const;
};



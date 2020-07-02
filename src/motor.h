#ifndef Motor_h
#define Motor_h

#include "Arduino.h"
#include <AFMotor.h>

#define MIN_ROTATION_M 150
#define MAX_ROTATION_M 255

class Motor {
  private:
    AF_DCMotor *motorLeft;
    AF_DCMotor *motorRight;
    int actualSpeedLeft = 0;
    int actualSpeedRight = 0;

  public:
    uint8_t directionLeft = RELEASE;
    uint8_t directionRight = RELEASE;

    Motor();
    void init(AF_DCMotor *motorLeft, AF_DCMotor *motorRight);
    void stop();
    void forward(int acceleration);
    void forwardTurn(int accelerationLeft, int accelerationRight);
    void backward(int acceleration);
    void backwardTurn(int accelerationLeft, int accelerationRight);
    void forwardBackwardTurn(int accelerationLeft, int accelerationRight);
    void backwardForwardTurn(int accelerationLeft, int accelerationRight);
    void rotate();

    int accelerationParse(int acceleration);
};

#endif

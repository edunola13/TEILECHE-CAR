#include "Arduino.h"
#include <AFMotor.h>

#include "motor.h"

Motor::Motor(){
}

void Motor::init(AF_DCMotor *motorLeft, AF_DCMotor *motorRight){
  this->motorLeft = motorLeft;
  this->motorRight = motorRight;
  this->motorLeft->run(RELEASE);
  this->motorRight->run(RELEASE);
}

void Motor::stop(){
  actualSpeedLeft = 0;
  actualSpeedRight = 0;
  directionLeft = RELEASE;
  directionRight = RELEASE;
  this->motorLeft->setSpeed(0);
  this->motorRight->setSpeed(0);
  this->motorLeft->run(directionLeft);
  this->motorRight->run(directionRight);
}

void Motor::forward(int acceleration){
  directionLeft = FORWARD;
  directionRight = FORWARD;
  int speed = this->accelerationParse(acceleration);
  actualSpeedLeft = speed;
  actualSpeedRight = speed;
  rotate();
}

void Motor::forwardTurn(int accelerationLeft, int accelerationRight){
  directionLeft = FORWARD;
  directionRight = FORWARD;
  actualSpeedLeft = this->accelerationParse(accelerationLeft);
  actualSpeedRight = this->accelerationParse(accelerationRight);
  rotate();
}

void Motor::backward(int acceleration){
  directionLeft = BACKWARD;
  directionRight = BACKWARD;
  int speed = this->accelerationParse(acceleration);
  actualSpeedLeft = speed;
  actualSpeedRight = speed;
  rotate();
};

void Motor::backwardTurn(int accelerationLeft, int accelerationRight){
  directionLeft = BACKWARD;
  directionRight = BACKWARD;
  actualSpeedLeft = this->accelerationParse(accelerationLeft);
  actualSpeedRight = this->accelerationParse(accelerationRight);
  rotate();
}

void Motor::forwardBackwardTurn(int accelerationLeft, int accelerationRight){
  directionLeft = FORWARD;
  directionRight = BACKWARD;
  actualSpeedLeft = this->accelerationParse(accelerationLeft);
  actualSpeedRight = this->accelerationParse(accelerationRight);
  rotate();
}

void Motor::backwardForwardTurn(int accelerationLeft, int accelerationRight){
  directionLeft = BACKWARD;
  directionRight = FORWARD;
  actualSpeedLeft = this->accelerationParse(accelerationLeft);
  actualSpeedRight = this->accelerationParse(accelerationRight);
  rotate();
}

void Motor::rotate(){
  this->motorLeft->setSpeed(actualSpeedLeft + MIN_ROTATION_M);
  this->motorRight->setSpeed(actualSpeedRight + MIN_ROTATION_M);
  this->motorLeft->run(directionLeft);
  this->motorRight->run(directionRight);
}

int Motor::accelerationParse(int acceleration) {
  return (acceleration * (MAX_ROTATION_M - MIN_ROTATION_M)) / 100;
}

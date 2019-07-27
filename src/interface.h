#ifndef RFInterface_h
#define RFInterface_h

#include "Arduino.h"
// #include <common_initial.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "motor.h"

struct MESSAGE {
  char action;
  int data[4];
};

class RFInterface {
  private:
    RF24 *radio;
    Motor *motor;
    MESSAGE message = {};

  public:
    bool iaEnabled = false;

    RFInterface();
    void init(RF24 *radio, Motor *motor, byte slaceAdd[]);
    bool isMessage();
    void executeAction();
};

#endif

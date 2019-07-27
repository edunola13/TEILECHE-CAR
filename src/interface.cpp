#include "interface.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "motor.h"

RFInterface::RFInterface(){
}

void RFInterface::init(RF24 *radio, Motor *motor, byte slaceAdd[]) {
  this->radio = radio;
  this->motor = motor;

  Serial.println("Radio Begin");
  radio->begin();
  radio->setDataRate(RF24_250KBPS);
  // radio.setPALevel(0);
  // byte slaceAdd[5] = {'R', 'x', 'A', 'A', 'A'};
  radio->openReadingPipe(1, slaceAdd);
  radio->enableAckPayload();
  radio->startListening();
  int ackData = {(int)iaEnabled};
  radio->writeAckPayload(1, &ackData, sizeof(ackData));
  Serial.println("Radio Conf");
}

bool RFInterface::isMessage() {
  return radio->available();
}

void RFInterface::executeAction() {
  // Leo la data
  byte data[32];
  radio->read(&data, sizeof(data));
  // Transformo solo el primer byte a un caracter
  char action;
  memcpy(&action, data, sizeof(action));
  // Segun la accion lo que hago
  if (action == 'a') {
    memcpy(&message, data, sizeof(message));
    int x = message.data[0];
    int y = message.data[1];
    int speedLeft = 0;
    int speedRight = 0;
    uint8_t direction = RELEASE;

    // Y-axis used for forward and backward control
    if (y < 470) {
      direction = BACKWARD;
      // Convert the declining Y-axis readings for going backward from 470 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
      speedRight = map(y, 470, 0, 0, 100);
      speedLeft = map(y, 470, 0, 0, 100);
    } else if (y > 550) {
      direction = FORWARD;
      // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
      speedRight = map(y, 550, 1023, 0, 100);
      speedLeft = map(y, 550, 1023, 0, 100);
    } else {
      // If joystick stays in middle the motors are not moving
      speedRight = 0;
      speedLeft = 0;
    }

    // X-axis used for left and right control
    if (x < 470) {
      // Convert the declining X-axis readings from 470 to 0 into increasing 0 to 255 value
      int xMapped = map(x, 470, 0, 0, 100);
      // Move to left - decrease left motor speed, increase right motor speed
      speedLeft = speedLeft - xMapped;
      speedRight = speedRight + xMapped;
      // Confine the range from 0 to 255
      if (speedLeft < 0) {
        speedLeft = 0;
      }
      if (speedRight > 100) {
        speedRight = 100;
      }
    }
    if (x > 550) {
      // Convert the increasing X-axis readings from 550 to 1023 into 0 to 255 value
      int xMapped = map(x, 550, 1023, 0, 255);
      // Move right - decrease right motor speed, increase left motor speed
      speedLeft = speedLeft + xMapped;
      speedRight = speedRight - xMapped;
      // Confine the range from 0 to 255
      if (speedLeft > 100) {
        speedLeft = 100;
      }
      if (speedRight < 0) {
        speedRight = 0;
      }
    }
    // Prevent buzzing at low speeds (Adjust according to your motors. My motors couldn't start moving if PWM value was below value of 70)
    if (speedLeft < 5) {
      speedLeft = 0;
    }
    if (speedRight < 5) {
      speedRight = 0;
    }

    if (direction == FORWARD) {
      motor->forwardTurn(speedLeft, speedRight);
    } else {
      motor->backwardTurn(speedLeft, speedRight);
    }
  }
  if (action == 'i') {
    memcpy(&message, data, sizeof(message));
    this->iaEnabled = (bool) message.data[0];
  }
}

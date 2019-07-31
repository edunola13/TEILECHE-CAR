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
  int ackData = {(int)iaEnabled};
  radio->writeAckPayload(1, &ackData, sizeof(ackData));
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
    uint8_t rotate = 0; // 0 = No rota, 1 = Derecha, 2 = Izquierda -> Este es para cuando solo quiere rotar

    // Y: Define hacia adelante o hacia atras. Se deja un intervalo de 8 en el que no definimos.
    if (y < 123) {
      direction = BACKWARD;
      // Convertimos los valores para que los entienda Motor
      speedRight = map(y, 123, 0, 0, 100);
      speedLeft = map(y, 123, 0, 0, 100);
    } else if (y > 131) {
      direction = FORWARD;
      // Convertimos los valores para que los entienda Motor
      speedRight = map(y, 131, 255, 0, 100);
      speedLeft = map(y, 131, 255, 0, 100);
    } else {
      // No se mueve
      speedRight = 0;
      speedLeft = 0;
    }

    if (direction != RELEASE) {
      // X: Define rotacion, va a depender de lo que paso en Y
      if (x < 123) {
        // Rigth
        // Convertimos los valores para que los entienda Motor
        // Reducimos a 50 el map para que no sea tran brusco el doblar
        int xMapped = map(x, 123, 0, 0, 50);
        // Move right - decrease right motor speed, increase left motor speed
        speedLeft = speedLeft + xMapped;
        speedRight = speedRight - xMapped;
      }
      if (x > 131) {
        // Left
        // Convertimos los valores para que los entienda Motor
        // Reducimos a 50 el map para que no sea tran brusco el doblar
        int xMapped = map(x, 131, 255, 0, 50);
        // Move to left - decrease left motor speed, increase right motor speed
        speedLeft = speedLeft - xMapped;
        speedRight = speedRight + xMapped;
      }
    } else {
      // No hay decision de movimiento, entonces rota
      if (x < 123) {
        // Rigth
        // Convertimos los valores para que los entienda Motor
        speedRight = map(y, 123, 0, 0, 100);
        speedLeft = map(y, 123, 0, 0, 100);
      }
      if (x > 131) {
        // Left
        // Convertimos los valores para que los entienda Motor
        speedRight = map(y, 131, 255, 0, 100);
        speedLeft = map(y, 131, 251, 0, 100);
      }
    }

    // Evitamos valores muy bajos -> Ruido de RF
    if (speedLeft < 3) {
      speedLeft = 0;
    }
    if (speedRight < 3) {
      speedRight = 0;
    }

    // Controlamos rangos
    if (speedLeft > 100) {
      speedLeft = 100;
    } else if (speedLeft < 0) {
      speedLeft = 0;
    }
    if (speedRight > 100) {
      speedRight = 100;
    } else if (speedRight < 0) {
      speedRight = 0;
    }

    if (direction == FORWARD) {
      motor->forwardTurn(speedLeft, speedRight);
    } else if (direction == BACKWARD) {
      motor->backwardTurn(speedLeft, speedRight);
    } else if (rotate == 1) { // Derecha
      motor->forwardBackwardTurn(speedLeft, speedRight);
    } else if (rotate == 2) { //Izquierda
      motor->backwardForwardTurn(speedLeft, speedRight);
    } else {
      motor->stop();
    }
  }
  if (action == 'i') {
    memcpy(&message, data, sizeof(message));
    this->iaEnabled = (bool) message.data[0];
  }
}

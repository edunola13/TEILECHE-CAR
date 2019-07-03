//0= Una unica placa, 1= Placas interconectadas
#define MODE_CORE 0

#include "Arduino.h"
#include <AFMotor.h>
// Uncomment to enable printing out nice debug messages.
#define DOMO_DEBUG
#define DOMO_SPEED 9600
//#define USE_WDT
//#define WDT_TIME WDTO_8S

#include <common_initial.h>
#include "messages.h"
#include <serial\StreamCom.h>
#include "motor.h"
#include "collision.h"

StreamCom streamCom;

AF_DCMotor motorLeft(3);
AF_DCMotor motorRight(4);
Motor motor;
CollisionDetect collision;
// Derecha a izquieda -> Vision de TEILECHE
int degrees[] = {25, 54, 84, 114, 143};

void setup() {
  initialGeneric();
  DEB_DO_PRINTLN(MSG_START);
  // Motor
  motor.init(&motorLeft, &motorRight);
  // Deteccion de colisiones
  collision.init(0, degrees);
}

void loop() {
  // Se ejecuta solo cada 400ms
  collision.calculateDistance();
  // Calcular velocidad??? solo serian con la distancia del centro
  if (collision.isCollisionIr()) {
    motor.stop();
  } else {
    int centro = collision.distancePosition(2);
    int derecha = collision.distancePosition(1);
    int izquierda = collision.distancePosition(3);
    if (centro >= 70 && derecha >= 25 && izquierda >= 25) {
      Serial.println("Rapido");
      motor.forward(100);
    } else if ((centro >= 40 && centro < 70)) { // (derecha >= 15 && derecha < 25) || (izquierda >= 15 && izquierda < 25)){
      Serial.println("Lento");
      motor.forward(50);
    } else if (centro >= 40 && izquierda < 25 && derecha > 40) {
      motor.forwardTurn(100, 0);
    } else if (centro >= 40 && derecha < 25 && izquierda > 40) {
      motor.forwardTurn(0, 100);
    } else {
      motor.stop();
    }
  }
  // Calculo distancias
  // // Bajo velocidad
  // // Stop Motor
  // // Doblar
  // // Ir marcha atras y girar
}

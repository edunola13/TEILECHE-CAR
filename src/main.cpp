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
// #include <serial\StreamCom.h>
#include "motor.h"
#include "collision.h"
#include "mapa.h"
#include "interface.h"

// StreamCom streamCom;

AF_DCMotor motorLeft(1);
AF_DCMotor motorRight(2);
Motor motor;
CollisionDetect collision;
Mapa mapa;
// Derecha a izquieda -> Vision de TEILECHE
int degrees[] = {25, 54, 84, 114, 143};

RF24 radio(48, 49);
RFInterface interface;

void setup() {
  initialGeneric();
  DEB_DO_PRINTLN(MSG_START);
  // Motor
  motor.init(&motorLeft, &motorRight);
  // Deteccion de colisiones
  collision.init(0, degrees);
  // Mapa
  mapa.init(&collision, &motor);
  // Interface RF24
  byte slaceAdd[5] = {'R', 'x', 'A', 'A', 'A'};
  interface.init(&radio, &motor, slaceAdd);
}

void loop() {
  if (interface.isMessage()) {
    interface.executeAction();
  } else if (interface.iaEnabled) {
    // Se ejecuta solo cada Xms
    collision.calculateDistance();
    // Actualizar Mapa cada Xms -> Mismo tiempo que collision
    mapa.updateMap();
    // Calcular nueva direccion
    mapa.updateDirection();
    // Moverse en base a todo lo calculado
    mapa.decideMove();
  }
  delay(10);
}

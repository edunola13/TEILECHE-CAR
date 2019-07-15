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

// StreamCom streamCom;

AF_DCMotor motorLeft(1);
AF_DCMotor motorRight(2);
Motor motor;
CollisionDetect collision;
Mapa mapa;
// Derecha a izquieda -> Vision de TEILECHE
int degrees[] = {25, 54, 84, 114, 143};

void setup() {
  initialGeneric();
  DEB_DO_PRINTLN(MSG_START);
  // Motor
  motor.init(&motorLeft, &motorRight);
  // Deteccion de colisiones
  collision.init(0, degrees);
  // Mapa
  mapa.init(&collision, &motor);
}



#define FRENTE 0
#define DERECHA 1
#define IZQUIERDA 2
#define REVERSA 3
uint8_t actualDirection = FRENTE;

#define MAX_SPEED 60
#define MEDIUM_SPEED 40
#define SLOW_SPEED 20

uint8_t centro = COLISION;
uint8_t derecha = COLISION;
uint8_t izquierda = COLISION;
uint8_t derecha_2 = COLISION;
uint8_t izquierda_2 = COLISION;


// void calcularDireccion() {
//     if (centro > CERCA && derecha > CERCA && izquierda > CERCA) {
//       return FRENTE;
//     }
//     if (centro <= CERCA && derecha <= CERCA && izquierda <= CERCA) {
//       return REVERSA;
//     }
//     if (centro <= CERCA && derecha <= CERCA && izquierda > CERCA) {
//       return IZQUIERDA;
//     }
//     if (centro <= CERCA && derecha > CERCA && izquierda <= CERCA) {
//       return DERECHA;
//     }
//     if (centro > CERCA) {
//       if (derecha <= CERCA && izquierda <= CERCA) {
//         return REVERSA;
//       }
//       if (derecha <= CERCA) {
//         return IZQUIERDA;
//       }
//       if (izquierda <= CERCA) {
//         return DERECJA;
//       }
//     }
// }
//
// void calcularGiro(uint8_t direction) {
//   if (direction == FRENTE) {
//     // Si voy de frente es 0
//     return 0;
//   }
//   if (direction == DERECHA) {
//     if (centro == COLISION && derecha_2 >= PROXIMO) {
//       return 90;
//     }
//     if (centro == CERCA && derecha_2 >= PROXIMO) {
//       return 45;
//     }
//     if (centro > CERCA && ) {
//       return
//     }
//   }
//   return -1; // ERROR -> CALCULAR DE NUEVO, EN CASO DE NUEVO ERROR REVERSA
// }

void loop() {
  // Se ejecuta solo cada Xms
  collision.calculateDistance();
  // Actualizar Mapa cada Xms -> Mismo tiempo que collision
  mapa.updateMap();
  // Calcular nueva direccion
  mapa.updateDirection();
  // Moverse en base a todo lo calculado
  mapa.decideMove();

  // Calcular velocidad??? solo serian con la distancia del centro
  // if (collision.isCollisionIr()) {
  //   motor.stop();
  // } else {
  //
  //   centro = distanciaCentro(collision.distancePosition(2));
  //   derecha = distanciaLateral(collision.distancePosition(1));
  //   izquierda = distanciaLateral(collision.distancePosition(3));
  //   derecha_2 = distanciaLateral2(collision.distancePosition(0));
  //   izquierda_2 = distanciaLateral2(collision.distancePosition(4));
  //
  //   calcularDireccion();
  //   calcularGiro();
  // }
}

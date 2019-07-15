#ifndef Mapa_h
#define Mapa_h

#include "Arduino.h"
#include "collision.h"
#include "motor.h"

#define COLISION 0
#define CERCA 1
#define PROXIMO 2
#define DISTANTE 3

class Mapa {
  private:
    CollisionDetect *collision;
    Motor *motor;
    int centralPosition = 2; // Es el centro
    int positions[5]; // El medio es el frente, 0 derecha, ultimo izquierda
    int positionsLevel[5] = {3, 2, 1, 2, 3};
    int historicDirections[15]; // Ultimas direcciones elegidas, por ahora solo uso la actual position "0"
    long lastUpdate;
    long lastMove;

  public:
    Mapa();
    void init(CollisionDetect *collision, Motor *motor);
    void updateMap();
    int updateDirection();
    bool isPositionAvailable(int pos, uint8_t len);
    int getActualDirection();
    void decideMove();

    uint8_t posLevel1(int dis);
    uint8_t posLevel2(int dis);
    uint8_t posLevel3(int dis);
};

#endif

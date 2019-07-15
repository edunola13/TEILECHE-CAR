#include "mapa.h"
#include "collision.h"
#include "motor.h"

Mapa::Mapa(){
}

void Mapa::init(CollisionDetect *collision, Motor *motor) {
  this->collision = collision;
  this->motor = motor;
}

void Mapa::updateMap() {
  if (this->lastUpdate <= this->collision->lastRead) {
    this->lastUpdate = millis();
    uint8_t len = sizeof(positions)/sizeof(positions[0]);
    Serial.print("Vaa: ");
    for (uint8_t i = 0; i < len; i++) {
      if (positionsLevel[i] == 1) {
        this->positions[i] = this->posLevel1(collision->distancePosition(i));
      } else if (positionsLevel[i] == 2) {
        this->positions[i] = this->posLevel2(collision->distancePosition(i));
      } else {
        this->positions[i] = this->posLevel3(collision->distancePosition(i));
      }
      Serial.print(" ");
      Serial.print(this->positions[i]);
      Serial.print(" ");
    }
    Serial.println("-");
  }
}

//
// START STRATEGY, se puede mover a una clase aparte
//

// Recorre la direccion y elige la primera en la que puede ir, no la mejor ruta
int Mapa::updateDirection() {
  if (this->lastMove >= this->lastUpdate) {
    return this->historicDirections[0];
  }
  int actPos = this->centralPosition;
  uint8_t len = sizeof(positions)/sizeof(positions[0]);
  if (this->isPositionAvailable(actPos, len)) {
    this->historicDirections[0] = actPos;
    return actPos;
  }
  for (uint8_t i = 1; i <= this->centralPosition; i++) {

    if (this->isPositionAvailable(actPos + i, len)) {
      actPos = actPos + i;
      this->historicDirections[0] = actPos;
      return actPos;

    }
    if (this->isPositionAvailable(actPos - i, len)) {
      actPos = actPos - i;
      this->historicDirections[0] = actPos;
      return actPos;
    }
  }
  this->historicDirections[0] = -1; // No definido -> Marcha Atras
  return this->historicDirections[0];
}

bool Mapa::isPositionAvailable(int pos, uint8_t len) {
  if (this->positions[pos] <= CERCA) {
    return false;
  }
  int r = pos - 1;
  if (r >= 0) {
    if (this->positions[r] <= CERCA) {
      return false;
    }
  }
  int l = pos + 1;
  if (l < len) {
    if (this->positions[l] <= CERCA) {
      return false;
    }
  }
  return true;
}

int Mapa::getActualDirection() {
  return this->historicDirections[0];
}

void Mapa::decideMove() {
  if (this->collision->isCollisionIr()) {
    this->motor->backward(50);
    delay(700);
    this->motor->stop();
    this->collision->initialReader();
  }
  if (this->lastMove < this->lastUpdate) {
    this->lastMove = millis();
    Serial.print("Direccion: ");
    Serial.println(this->getActualDirection());
    if (this->getActualDirection() == 2) {
      this->motor->forward(30);
    }
    if (this->getActualDirection() == 1) {
      // Ir hacia la derecha
      this->motor->forwardTurn(30, 0);
      delay(150);
      this->motor->forward(10);
    }
    if (this->getActualDirection() == 3) {
      // Ir hacia la izquierda
      this->motor->forwardTurn(0, 30);
      delay(150);
      this->motor->forward(10);
    }
    if (this->getActualDirection() == 0) {
      // Rotar hacia la derecha
      this->motor->forwardBackwardTurn(30, 30);
      delay(100);
      this->motor->stop();
    }
    if (this->getActualDirection() == 4) {
      // Ir hacia la izquierda
      this->motor->backwardForwardTurn(30, 30);
      delay(100);
      this->motor->stop();
    }
    if (this->getActualDirection() == -1) {
      if (this->positions[0] == this->positions[4]) {
        this->motor->backward(30);
        delay(300);
      } else if (this->positions[0] > this->positions[4]) {
        // Si de la derecha tengo los objetivos mas lejos
        this->motor->backwardTurn(30, 15);
        delay(300);
      } else {
        // Si no de la izquierda
        this->motor->backwardTurn(15, 30);
        delay(300);
      }
      this->motor->stop();
      this->collision->initialReader();
    }
  }
}

//
//  FIN STRATEGY
//

uint8_t Mapa::posLevel1(int dis) {
  if (dis <= 15) {
    return COLISION;
  } else if (dis > 15 && dis <= 40) {
    return CERCA;
  } else if (dis > 40 && dis <= 150) {
    return PROXIMO;
  } else if (dis > 150) {
    return DISTANTE;
  }
}

uint8_t Mapa::posLevel2(int dis) {
  if (dis <= 15) {
    return COLISION;
  } else if (dis > 15 && dis <= 30) {
    return CERCA;
  } else if (dis > 30 && dis <= 60) {
    return PROXIMO;
  } else if (dis > 60) {
    return DISTANTE;
  }
}

uint8_t Mapa::posLevel3(int dis) {
  if (dis <= 10) {
    return COLISION;
  } else if (dis > 10 && dis <= 20) {
    return CERCA;
  } else if (dis > 20 && dis <= 40) {
    return PROXIMO;
  } else if (dis > 40) {
    return DISTANTE;
  }
}

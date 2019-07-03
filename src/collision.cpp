#include <Servo.h>
#include <DistanceHCSR04.h>

#include "collision.h"

CollisionDetect::CollisionDetect(){
}

void CollisionDetect::init(int p, int *degrees) {
  this->degrees = degrees;
  this->actualDegree = degrees[p];
  this->lastRead = millis();
  pinMode(PIN_COLLISION_IR , INPUT);
  distance = DistanceHCSR04(PIN_DISTANCE_ECHO, PIN_DISTANCE_TRIG);
  servoDir.attach(10);  // El servo 1 se controla con el pin 10, deberia ser al 9
  servoDir.write(this->actualDegree);
  delay(1000);
  initialReader();
}

void CollisionDetect::initialReader() {
  for (uint8_t i = 0; i < 5; i++) {
    calculateDistance();
    delay(200);
  }
}

bool CollisionDetect::isCollisionIr() {
  detectIr = (digitalRead(PIN_COLLISION_IR) == LOW);
  return detectIr;
}

void CollisionDetect::setActualDegree(int p) {
  this->actualDegree = degrees[p];
  servoDir.write(this->actualDegree);
}

void CollisionDetect::calculateDistance() {
  if (lastRead + MAX_DISTANCE_TIME > millis()) {
    return;
  }
  distance.updateSensor();
  uint8_t p = actualDegreePosition();
  readers[p] = distance.getLevel();
  moveToNextDegree(p);
}

uint8_t CollisionDetect::distancePosition(uint8_t p) {
  return readers[p];
}

int CollisionDetect::actualDegreePosition() {
  uint8_t len = sizeof(readers)/sizeof(readers[0]);
  for (uint8_t i = 0; i < len; i++) {
    if (actualDegree == degrees[i]) {
      return i;
    }
  }
}

void CollisionDetect::moveToNextDegree(uint8_t p) {
  Serial.println(p);
  if (ascDegree) {
    if (p < 4) {
      p++;
    } else {
      p--;
      ascDegree = false;
    }
  } else if (! ascDegree) {
    if (p > 0) {
      p--;
    } else {
      p++;
      ascDegree = true;
    }
  }
  this->actualDegree = degrees[p];
  servoDir.write(this->actualDegree);
  // Calcular tiempo desde que se empezo a mover el servo
  this->lastRead = millis();
}

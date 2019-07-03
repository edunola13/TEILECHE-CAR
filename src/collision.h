#ifndef Collision_h
#define Collision_h

#include <Servo.h>
#include <DistanceHCSR04.h>

#define PIN_COLLISION_IR 24
#define PIN_DISTANCE_TRIG 22
#define PIN_DISTANCE_ECHO 23

#define MAX_DISTANCE_TIME 400

class CollisionDetect {
  private:
    Servo servoDir;
    DistanceHCSR04 distance;
    bool detectIr = false;

    int ascDegree = true;
    int actualDegree = 25;
    int *degrees;
    int readers[5];
    long lastRead;

  public:
    CollisionDetect();
    void init(int actualDegree, int *degrees);
    void initialReader();
    bool isCollisionIr();
    void setActualDegree(int actualDegree);
    void calculateDistance();
    uint8_t distancePosition(uint8_t p);

    int actualDegreePosition();
    void moveToNextDegree(uint8_t p);
};

#endif

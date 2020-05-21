#define EPSILON 1.0E-10

#include "DynamicPiece.h"

DynamicPiece::DynamicPiece(double x, double y, double z,
                           double vx, double vy, double vz,
                           double ax, double ay, double az,
                           double r1, double r2, double r3,
                           double vr1, double vr2, double vr3,
                           double ar1, double ar2, double ar3)
{
  this->pos[0] = x;
  this->pos[1] = y;
  this->pos[2] = z;

  this->vel[0] = vx;
  this->vel[1] = vy;
  this->vel[2] = vz;

  this->accel[0] = ax;
  this->accel[1] = ay;
  this->accel[2] = az;


  this->rot[0] = r1;
  this->rot[1] = r2;
  this->rot[2] = r3;

  this->rotVel[0] = vr1;
  this->rotVel[1] = vr2;
  this->rotVel[2] = vr3;

  this->rotAccel[0] = ar1;
  this->rotAccel[1] = ar2;
  this->rotAccel[2] = ar3;
}

DynamicPiece::~DynamicPiece() {}

void DynamicPiece::updatePhysics(double deltaTime) {
  this->lifetime += deltaTime;

  if (this->pos[1] > -0.49) {
    this->vel[0] += this->accel[0] * deltaTime;
    this->vel[1] += this->accel[1] * deltaTime;
    this->vel[2] += this->accel[2] * deltaTime;

    this->pos[0] += this->vel[0] * deltaTime;
    this->pos[1] += this->vel[1] * deltaTime;
    this->pos[2] += this->vel[2] * deltaTime;

    this->rotVel[0] += this->rotAccel[0] * deltaTime;
    this->rotVel[1] += this->rotAccel[1] * deltaTime;
    this->rotVel[2] += this->rotAccel[2] * deltaTime;

    this->rot[0] += this->rotVel[0] * deltaTime;
    this->rot[1] += this->rotVel[1] * deltaTime;
    this->rot[2] += this->rotVel[2] * deltaTime;
  } else {
    this->vel[0] = this->vel[0] / 3;
    this->vel[1] = -this->vel[1] / 7;
    this->vel[2] = this->vel[2] / 3;

    this->rotVel[0] = this->rotVel[0] * this->vel[0]*2;
    this->rotVel[1] = this->rotVel[1] * this->vel[1]*2;
    this->rotVel[2] = this->rotVel[2] * this->vel[2]*2;

    this->pos[1] = -0.49 + EPSILON;
  }
}

void DynamicPiece::generate() {
  glPushMatrix();

  glTranslatef(
      this->pos[0], this->pos[1], this->pos[2]
  );
  // mat4.rotate(
  //     pieceTransformations,
  //     pieceTransformations,
  //     this->rot[0],
  //     [1.0, 0.0, 0.0]
  // )
  // mat4.rotate(
  //     pieceTransformations,
  //     pieceTransformations,
  //     this->rot[1],
  //     [0.0, 1.0, 0.0]
  // )
  // mat4.rotate(
  //     pieceTransformations,
  //     pieceTransformations,
  //     this->rot[2],
  //     [0.0, 0.0, 1.0]
  // )
  Cube::draw();

  glPopMatrix();
}

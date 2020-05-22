#include "cube.h"

#ifndef DYNAMIC_PIECE_H
#define DYNAMIC_PIECE_H

class DynamicPiece
{
public:
  double m_lifetime;

  double m_pos[3];
  double m_vel[3];
  double m_accel[3];

  double m_rot[3];
  double m_rotVel[3];
  double m_rotAccel[3];
  DynamicPiece(double x, double y, double z,
               double vx, double vy, double vz,
               double ax, double ay, double az,
               double r1, double r2, double r3,
               double vr1, double vr2, double vr3,
               double ar1, double ar2, double ar3);
  ~DynamicPiece();
  void updatePhysics(double deltaTime);
  void generate();
};

#endif
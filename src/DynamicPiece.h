// #include "cube.h"

#ifndef DYNAMIC_PIECE_H
#define DYNAMIC_PIECE_H

#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <GL/glext.h> // Needed for GL_MULTISAMPLE
#include <iostream>

class DynamicPiece
{
public:
  GLint m_texture_name;
  double m_lifetime;

  double m_pos[3];
  double m_vel[3];
  double m_accel[3];

  double m_rot[3];
  double m_rotVel[3];
  double m_rotAccel[3];
  DynamicPiece();
  DynamicPiece(GLint texture_name,
               double x, double y, double z,
               double vx, double vy, double vz,
               double ax, double ay, double az,
               double r1, double r2, double r3,
               double vr1, double vr2, double vr3,
               double ar1, double ar2, double ar3);
  ~DynamicPiece();
  void updatePhysics(double deltaTime);
  void generate();

  friend bool operator<(const DynamicPiece &c1, const DynamicPiece &c2);
};

#endif
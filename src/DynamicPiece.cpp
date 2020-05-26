#define EPSILON 1.0E-10

// TODO get value from OpenGlTetris
#define CUBE_SIZE 0.005

#include "DynamicPiece.h"


double cubeCoords[] = {
  // front
  -CUBE_SIZE/2, -CUBE_SIZE/2, CUBE_SIZE/2,
  -CUBE_SIZE/2, CUBE_SIZE/2, CUBE_SIZE/2,
  CUBE_SIZE/2, CUBE_SIZE/2, CUBE_SIZE/2,
  CUBE_SIZE/2, -CUBE_SIZE/2, CUBE_SIZE/2,

  // back
  -CUBE_SIZE/2, -CUBE_SIZE/2, -CUBE_SIZE/2,
  -CUBE_SIZE/2, CUBE_SIZE/2, -CUBE_SIZE/2,
  CUBE_SIZE/2, CUBE_SIZE/2, -CUBE_SIZE/2,
  CUBE_SIZE/2, -CUBE_SIZE/2, -CUBE_SIZE/2,

  // left
  -CUBE_SIZE/2, -CUBE_SIZE/2, -CUBE_SIZE/2,
  -CUBE_SIZE/2, CUBE_SIZE/2, -CUBE_SIZE/2,
  -CUBE_SIZE/2, CUBE_SIZE/2, CUBE_SIZE/2,
  -CUBE_SIZE/2, -CUBE_SIZE/2, CUBE_SIZE/2,

  // right
  CUBE_SIZE/2, -CUBE_SIZE/2, -CUBE_SIZE/2,
  CUBE_SIZE/2, CUBE_SIZE/2, -CUBE_SIZE/2,
  CUBE_SIZE/2, CUBE_SIZE/2, CUBE_SIZE/2,
  CUBE_SIZE/2, -CUBE_SIZE/2, CUBE_SIZE/2,

  // top
  -CUBE_SIZE/2, CUBE_SIZE/2, CUBE_SIZE/2,
  -CUBE_SIZE/2, CUBE_SIZE/2, -CUBE_SIZE/2,
   CUBE_SIZE/2, CUBE_SIZE/2, -CUBE_SIZE/2,
   CUBE_SIZE/2, CUBE_SIZE/2, CUBE_SIZE/2,

  // bottom
  -CUBE_SIZE/2, -CUBE_SIZE/2, CUBE_SIZE/2,
  -CUBE_SIZE/2, -CUBE_SIZE/2, -CUBE_SIZE/2,
   CUBE_SIZE/2, -CUBE_SIZE/2, -CUBE_SIZE/2,
   CUBE_SIZE/2, -CUBE_SIZE/2, CUBE_SIZE/2
};

double cubeNormals[] = {
  // front
  0.0, 0.0, 1.0,
  0.0, 0.0, 1.0,
  0.0, 0.0, 1.0,
  0.0, 0.0, 1.0,

  // back
  0.0, 0.0, -1.0,
  0.0, 0.0, -1.0,
  0.0, 0.0, -1.0,
  0.0, 0.0, -1.0,

  // left
  -1.0, 0.0, 0.0,
  -1.0, 0.0, 0.0,
  -1.0, 0.0, 0.0,
  -1.0, 0.0, 0.0,

  // right
  1.0, 0.0, 0.0,
  1.0, 0.0, 0.0,
  1.0, 0.0, 0.0,
  1.0, 0.0, 0.0,

  // top
  0.0, 1.0, 0.0,
  0.0, 1.0, 0.0,
  0.0, 1.0, 0.0,
  0.0, 1.0, 0.0,

  // bottom
  0.0, -1.0, 0.0,
  0.0, -1.0, 0.0,
  0.0, -1.0, 0.0,
  0.0, -1.0, 0.0,
};

double cubeTextureCoords[12] = {
  0.0, 0.0,
  1.0, 0.0,
  1.0, 1.0,
  0.0, 1.0,
};

DynamicPiece::DynamicPiece() {}

DynamicPiece::DynamicPiece(GLint texture_name,
                           double x, double y, double z,
                           double vx, double vy, double vz,
                           double ax, double ay, double az,
                           double r1, double r2, double r3,
                           double vr1, double vr2, double vr3,
                           double ar1, double ar2, double ar3)
{
  m_texture_name = texture_name;
  m_lifetime = 0;

  m_pos[0] = x;
  m_pos[1] = y;
  m_pos[2] = z;

  m_vel[0] = vx;
  m_vel[1] = vy;
  m_vel[2] = vz;

  m_accel[0] = ax;
  m_accel[1] = ay;
  m_accel[2] = az;


  m_rot[0] = r1;
  m_rot[1] = r2;
  m_rot[2] = r3;

  m_rotVel[0] = vr1;
  m_rotVel[1] = vr2;
  m_rotVel[2] = vr3;

  m_rotAccel[0] = ar1;
  m_rotAccel[1] = ar2;
  m_rotAccel[2] = ar3;
}

DynamicPiece::~DynamicPiece() {}

void DynamicPiece::updatePhysics(double deltaTime) {
  deltaTime /= 4;
  m_lifetime += deltaTime;
  if (m_pos[1] > -0.49) {
    m_vel[0] += m_accel[0] * deltaTime;
    m_vel[1] += m_accel[1] * deltaTime;
    m_vel[2] += m_accel[2] * deltaTime;

    m_pos[0] += m_vel[0] * deltaTime;
    m_pos[1] += m_vel[1] * deltaTime;
    m_pos[2] += m_vel[2] * deltaTime;

    m_rotVel[0] += m_rotAccel[0] * deltaTime;
    m_rotVel[1] += m_rotAccel[1] * deltaTime;
    m_rotVel[2] += m_rotAccel[2] * deltaTime;

    m_rot[0] += m_rotVel[0] * deltaTime;
    m_rot[1] += m_rotVel[1] * deltaTime;
    m_rot[2] += m_rotVel[2] * deltaTime;
  }
  else {
    m_vel[0] = m_vel[0] / 2;
    m_vel[1] = -m_vel[1] / 2;
    m_vel[2] = m_vel[2] / 2;

    m_rotVel[0] = m_rotVel[0] * m_vel[0]*2;
    m_rotVel[1] = m_rotVel[1] * m_vel[1]*2;
    m_rotVel[2] = m_rotVel[2] * m_vel[2]*2;

    m_pos[1] = -0.49 + EPSILON;
  }
}

void DynamicPiece::generate() {
  glPushMatrix();


  glPushMatrix();
  glLoadIdentity();
  {
    glTranslatef(
      m_pos[0], m_pos[1], m_pos[2]
    );
    glRotatef(m_rot[0], 1.0, 0.0, 0.0);
    glRotatef(m_rot[1], 0.0, 1.0, 0.0);
    glRotatef(m_rot[2], 0.0, 0.0, 1.0);
  }
  GLdouble piece_tranformations[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, piece_tranformations);
  glPopMatrix();
  glMultMatrixd(piece_tranformations);



  glBindTexture(GL_TEXTURE_2D, m_texture_name);
  glEnable(GL_TEXTURE_2D);
  for(int i = 0; i < 72; i += 12)
  {
    int check = 0;
    glBegin(GL_QUADS);
      for(int j = i; j < i + 12; j += 3)
      {
          glTexCoord2f(cubeTextureCoords[check], cubeTextureCoords[check+1]);
          glNormal3f(cubeNormals[j], cubeNormals[j + 1], cubeNormals[j + 2]);
          glVertex3f(cubeCoords[j], cubeCoords[j + 1], cubeCoords[j + 2]);
          check += 2;
      }
    glEnd();
  }

  glPopMatrix();
}

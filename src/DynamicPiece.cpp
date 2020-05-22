#define EPSILON 1.0E-10

// TODO get value from OpenGlTetris
#define CUBE_SIZE 0.01

#include "DynamicPiece.h"


double cubeCoords[] = {
  // front
  0.0, 0.0, CUBE_SIZE,
  0.0, CUBE_SIZE, CUBE_SIZE,
  CUBE_SIZE, CUBE_SIZE, CUBE_SIZE,
  CUBE_SIZE, 0.0, CUBE_SIZE,

  // back
  0.0, 0.0, 0,
  0.0, CUBE_SIZE, 0,
  CUBE_SIZE, CUBE_SIZE, 0,
  CUBE_SIZE, 0.0, 0,

  // 1.0 left
  0.0, CUBE_SIZE, CUBE_SIZE,
  0.0, CUBE_SIZE, 0,
  0.0, 0.0, 0,
  0.0, 0.0, CUBE_SIZE,

  // 1.0 right
  CUBE_SIZE, CUBE_SIZE, CUBE_SIZE,
  CUBE_SIZE, CUBE_SIZE, 0,
  CUBE_SIZE, 0.0, 0,
  CUBE_SIZE, 0.0, CUBE_SIZE,

  // bottom
  0.0, 0.0, CUBE_SIZE,
  0.0, 0.0, 0,
  CUBE_SIZE, 0.0, 0,
  CUBE_SIZE, 0.0, CUBE_SIZE,

  // top
  0.0, CUBE_SIZE, CUBE_SIZE,
  0.0, CUBE_SIZE, 0,
  CUBE_SIZE, CUBE_SIZE, 0,
  CUBE_SIZE, CUBE_SIZE, CUBE_SIZE,
};

double cubeTextureCoords[12] = {
  0.0, 0.0,
  1.0, 0.0,
  1.0, 1.0,
  0.0, 1.0,
};


DynamicPiece::DynamicPiece(double x, double y, double z,
                           double vx, double vy, double vz,
                           double ax, double ay, double az,
                           double r1, double r2, double r3,
                           double vr1, double vr2, double vr3,
                           double ar1, double ar2, double ar3)
  // x{x},
  // y{y},
  // z{z}
{
  this->m_lifetime = 0;

  this->m_pos[0] = x;
  this->m_pos[1] = y;
  this->m_pos[2] = z;

  std::cout << "debug construct draw xyz " << this->m_pos[0] << " " << this->m_pos[1] << " " << this->m_pos[2] << '\n';


  // this->m_vel[0] = vx;
  // this->m_vel[1] = vy;
  // this->m_vel[2] = vz;

  // this->m_accel[0] = ax;
  // this->m_accel[1] = ay;
  // this->m_accel[2] = az;


  // this->m_rot[0] = r1;
  // this->m_rot[1] = r2;
  // this->m_rot[2] = r3;

  // this->m_rotVel[0] = vr1;
  // this->m_rotVel[1] = vr2;
  // this->m_rotVel[2] = vr3;

  // this->m_rotAccel[0] = ar1;
  // this->m_rotAccel[1] = ar2;
  // this->m_rotAccel[2] = ar3;
}

DynamicPiece::~DynamicPiece() {}

void DynamicPiece::updatePhysics(double deltaTime) {
  this->m_lifetime += deltaTime;

  // if (this->pos[1] > -0.49) {
    // this->vel[0] += this->accel[0] * deltaTime;
    // this->vel[1] += this->accel[1] * deltaTime;
    // this->vel[2] += this->accel[2] * deltaTime;

    // this->pos[0] += this->vel[0] * deltaTime;
    // this->pos[1] += this->vel[1] * deltaTime;
    // this->pos[2] += this->vel[2] * deltaTime;

    // this->rotVel[0] += this->rotAccel[0] * deltaTime;
    // this->rotVel[1] += this->rotAccel[1] * deltaTime;
    // this->rotVel[2] += this->rotAccel[2] * deltaTime;

    // this->rot[0] += this->rotVel[0] * deltaTime;
    // this->rot[1] += this->rotVel[1] * deltaTime;
    // this->rot[2] += this->rotVel[2] * deltaTime;
  // }
  // else {
  //   this->vel[0] = this->vel[0] / 3;
  //   this->vel[1] = -this->vel[1] / 7;
  //   this->vel[2] = this->vel[2] / 3;

  //   this->rotVel[0] = this->rotVel[0] * this->vel[0]*2;
  //   this->rotVel[1] = this->rotVel[1] * this->vel[1]*2;
  //   this->rotVel[2] = this->rotVel[2] * this->vel[2]*2;

  //   this->pos[1] = -0.49 + EPSILON;
  // }
}

void DynamicPiece::generate() {
  glPushMatrix();

  std::cout << "debug draw xyz " << m_pos[0] << " " << m_pos[1] << " " << m_pos[2] << '\n';

  glTranslatef(
    (float) m_pos[0], (float) m_pos[1], (float) m_pos[2]
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
  glBindTexture(GL_TEXTURE_2D, 0);
  glEnable(GL_TEXTURE_2D);
  for(int i = 0; i < 72; i += 12)
  {
    int check = 0;
    glBegin(GL_QUADS);
        for(int j = i; j < i + 12; j += 3)
        {
            glTexCoord2f(cubeTextureCoords[check], cubeTextureCoords[check+1]);
            glVertex3f(cubeCoords[j], cubeCoords[j + 1], cubeCoords[j + 2]);
            check += 2;
        }
    glEnd();
  }


  glPopMatrix();
}

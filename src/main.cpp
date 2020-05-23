#include <iostream>
#include <unistd.h>
#include <chrono>
#include <set>


#include "matrix.h"
#include "piece.h"
#include "tetris.h"
#include "testtetris.h"
#include "opengltetris.h"
#include "DynamicPiece.h"

#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <GL/glext.h> // Needed for GL_MULTISAMPLE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "SOIL.h"

#include <bits/stdc++.h>
using namespace std;
#define SIDE 0.005
#define SIZE 0.01
#define PI 3.14

#define CAMERA_FAR 100500 // TODO: ajust this
// GLOBALS

auto startTime(std::chrono::steady_clock::now());
int game_time_counter = 1;
double elapsedTime;


GLint QUADS  = 2;
GLint LIGHT = 1;
GLint ANTI_ALIASING = 1;
GLint LABEL = 1;

int STATE = 1;

map<string, GLuint> texture_map;
map<int, std::array<double, 3>> color_map;
map<int, GLuint> tetromino_texture_map;
OpenGLTetris oglt{SIDE, &tetromino_texture_map};
// textures
// GLuint red_block_texture;
// GLuint orange_block_texture




//---------------------
//  FALLING PIECES

double lastTime = glutGet(GLUT_ELAPSED_TIME)/1000.0;

extern std::vector<DynamicPiece> fallingPieces;

extern std::mt19937 mersenne;
extern std::uniform_real_distribution randomMagnitude; // TEMP

//------------------



double btn_arrow_point_map [] = {
  // front middle
  -0.005, -0.005, 0,
  0.005, -0.005, 0,
  0.005, 0.005, 0,
  -0.005, 0.005, 0,


  // up arrow
  0.005, 0.015, 0,
  -0.005, 0.015, 0,
  -0.005, 0.005, 0,
  0.005, 0.005, 0,


  // right arrow
  0.005, 0.005, 0,
  0.005, -0.005, 0,
  0.015, -0.005, 0,
  0.015, 0.005, 0,

  // down arrow

  -0.005, -0.005, 0,
  0.005, -0.005, 0,
  0.005, -0.015, 0,
  -0.005, -0.015, 0,

  // left arrow

  -0.005, -0.005, 0,
  -0.015, -0.005, 0,
  -0.015, 0.005, 0,
  -0.005, 0.005, 0,

  // top sides
  -0.005, 0.005, -0.005,
  -0.015, 0.005, -0.005,
  -0.015, 0.005, 0,
  -0.005, 0.005, 0,

  -0.005, 0.015, -0.005,
  0.005, 0.015, -0.005,
  0.005, 0.015, 0,
  -0.005, 0.015, 0,

  0.015, 0.005, -0.005,
  0.005, 0.005, -0.005,
  0.005, 0.005, 0,
  0.015, 0.005, 0,

  // bottom sides

  -0.005, -0.005, -0.005,
  -0.015, -0.005, -0.005,
  -0.015, -0.005, 0,
  -0.005, -0.005, 0,

  -0.005, -0.015, -0.005,
  0.005, -0.015, -0.005,
  0.005, -0.015, 0,
  -0.005, -0.015, 0,

  0.015, -0.005, -0.005,
  0.005, -0.005, -0.005,
  0.005, -0.005, 0,
  0.015, -0.005, 0,

  // left sides
  -0.005, -0.005, -0.005,
  -0.005, -0.015, -0.005,
  -0.005, -0.015, 0,
  -0.005, -0.005, 0,

  -0.015, -0.005, -0.005,
  -0.015, 0.005, -0.005,
  -0.015, 0.005, 0,
  -0.015, -0.005, 0,

  -0.005, 0.015, -0.005,
  -0.005, 0.005, -0.005,
  -0.005, 0.005, 0,
  -0.005, 0.015, 0,

  // right sides
  0.005, -0.005, -0.005,
  0.005, -0.015, -0.005,
  0.005, -0.015, 0,
  0.005, -0.005, 0,

  0.015, -0.005, -0.005,
  0.015, 0.005, -0.005,
  0.015, 0.005, 0,
  0.015, -0.005, 0,

  0.005, 0.015, -0.005,
  0.005, 0.005, -0.005,
  0.005, 0.005, 0,
  0.005, 0.015, 0,
};


double gameboy_texture_map [] = {
    // back
     (double)1/3, 0.0,
     (double)1/3, 1.0,
     (double)2/3, 1.0,
     (double)2/3, 0.0,

    //left
    (double)55/195, 0.0,
    (double) 55/195, 1,
    (double)1/3, 1,
    (double)1/3, 0.0,
    
    //right
    (double)0.24, 0.0,
    (double)0.24, 1,
    (double)55/195, 1,
    (double)55/195, 0.0,

    //top
    (double)0.24, 0.5,
    (double)0.24, 1.0,
    (double)0.2, 1.0,
    (double)0.2, 0.5,

    //bottom
    (double)0.24, 0.0,
    (double)0.24, (double)0.5,
    (double)0.2, (double)0.5,
    (double)0.2, 0.0,

    //front down
    (double)1.0, (double)7/13,
    (double)1.0, (double)1.0,
    (double)2/3, (double)1.0,
    (double)2/3, (double)7/13,

    // front left
    (double)0.718, 0.0,
    (double) 0.718, (double)7/13,
    (double)2/3, (double)7/13,
    (double)2/3, 0.0,

    //front right
    1, 0.0,
    1, (double)7/13,
    0.948, (double)7/13,
    0.948, 0.0,

    //front up
    (double)0.948, 0.0,
    (double)0.948, (double)10/130,
    (double)0.718, (double)10/130,
    (double)0.718, 0.0,

    //screen front
    0.99, 0.99,
    0.99, 1,
    1, 1,
    1, 0.99,

    //screen left
    0.99, 0.99,
    0.99, 1,
    1, 1,
    1, 0.99,

    //screen right
    0.99, 0.99,
    0.99, 1,
    1, 1,
    1, 0.99,

    //screen top
    0.99, 0.99,
    0.99, 1,
    1, 1,
    1, 0.99,

    //screen bottom
    0.99, 0.99,
    0.99, 1,
    1, 1,
    1, 0.99,
};

double gameboy_point_map [] = {
    // back
    0.065, 0.13, -0.009,
    0.065, -0.13, -0.009,
    -0.065, -0.13, -0.009,
    -0.065, 0.13, -0.009,

    // right
    -0.065, 0.13, 0.009,
    -0.065, -0.13, 0.009,
    -0.065, -0.13, -0.009,
    -0.065, 0.13, -0.009,

    // left
    0.065, 0.13, 0.009,
    0.065, -0.13, 0.009,
    0.065, -0.13, -0.009,
    0.065, 0.13, -0.009,

    // top
    0.065, 0.13, 0.009,
    -0.065, 0.13, 0.009,
    -0.065, 0.13, -0.009,
    0.065, 0.13, -0.009,

    // bottom
    0.065, -0.13, 0.009,
    -0.065, -0.13, 0.009,
    -0.065, -0.13, -0.009,
    0.065, -0.13, -0.009,

    // front down
    0.065, -0.01, 0.009,
    0.065, -0.13, 0.009,
    -0.065, -0.13, 0.009,
    -0.065, -0.01, 0.009,

    // front left
    -0.045, 0.13, 0.009,
    -0.045, -0.01, 0.009,
    -0.065, -0.01, 0.009,
    -0.065, 0.13, 0.009,

    // front right
    0.065, 0.13, 0.009,
    0.065, -0.01, 0.009,
    0.045, -0.01, 0.009,
    0.045, 0.13, 0.009,

    // front up
    0.045, 0.13, 0.009,
    0.045, 0.11, 0.009,
    -0.045, 0.11, 0.009,
    -0.045, 0.13, 0.009,

    // screen front
    0.045, 0.11, -0.0012,
    0.045, -0.01, -0.0012,
    -0.045, -0.01, -0.0012,
    -0.045, 0.11, -0.0012,

    // screen left
    -0.045, 0.11, -0.0012,
    -0.045, 0.11, 0.009,
    -0.045, -0.01, 0.009,
    -0.045, -0.01, -0.0012,

    // screen right
    0.045, 0.11, -0.0012,
    0.045, 0.11, 0.009,
    0.045, -0.01, 0.009,
    0.045, -0.01, -0.0012,

    // screen top
    0.045, 0.11, -0.0012,
    0.045, 0.11, 0.009,
    -0.045, 0.11, 0.009,
    -0.045, 0.11, -0.0012,

    // screen bottom
    0.045, -0.01, -0.0012,
    0.045, -0.01, 0.009,
    -0.045, -0.01, 0.009,
    -0.045, -0.01, -0.0012
};

double texture_position [] =
{
    0.0, 0.0,
    1.0, 0.0, 
    1.0, 1.0,
    0.0, 1.0,
};


float scalex = 3.0;
float scaley = 3.0;
float scalez = 3.0;

//-------------------------------------------
// Cameras
//-------------------------------------------

typedef struct {
  float eye[3];
  int up[3];
  float *center;

  float cameraViewNormal[3];
  float viewPos[3];
  float zoom;
  float maxZoom;
} Camera;


void changeCameraZoom(Camera *camera, float increment) {
  if (camera->zoom + increment > camera->maxZoom) {
    camera->zoom = camera->maxZoom;
  }
  else if (camera->zoom + increment < 0) {
    camera->zoom = 1;
  }
  else {
    camera->zoom += increment;
  }
  camera->viewPos[0] = camera->eye[0] + camera->zoom*camera->cameraViewNormal[0];
  camera->viewPos[1] = camera->eye[1] + camera->zoom*camera->cameraViewNormal[1];
  camera->viewPos[2] = camera->eye[2] + camera->zoom*camera->cameraViewNormal[2];
}



float originPoint[3] = {0,0,0};

Camera cameraFront = {
  {0.0, 0.0, 400.0},
  {0, 1, 0},
  originPoint,

  {0,0,-1},
  {0.0, 0.0, 400.0},
  0, 306
};

Camera cameraRight = {
  {400.0, 0.0, 0.0},
  {0, 1, 0},
  originPoint,

  {-1,0,0},
  {400.0, 0.0, 0.0},
  0, 306
};

Camera cameraBack = {
  {0.0, 0.0, -400.0},
  {0, 1, 0},
  originPoint,

  {0,0,1},
  {0.0, 0.0, -400.0},
  0, 306
};

Camera cameraLeft = {
  {-400.0, 0.0, 0.0},
  {0, 1, 0},
  originPoint,

  {1,0,0},
  {-400.0, 0.0, 0.0},
  0, 306
};

Camera cameraTop = {
  {0.0, 400.0, 0.0},
  {0, 0, -1},
  originPoint,

  {0,-1,0},
  {0.0, 400.0, 0.0},
  0, 306
};

Camera cameraBottom = {
  {0.0, -400.0, 0.0},
  {0, 0, 1},
  originPoint,

  {0,1,0},
  {0.0, -400.0, 0.0},
  0, 306
};

Camera cameraDiagonal = {
  {-200, 200, 200},
  {0, 1, 0},
  originPoint,

  {0.5773502691896257, -0.5773502691896257, -0.5773502691896257},
  {-200, 200, 200},
  0, 200
};

Camera *currentCamera = &cameraFront;

GLuint initTexture(char *imgFilename) {
    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    // load and generate the texture
    int width, height;
    unsigned char *data = SOIL_load_image(imgFilename, &width, &height, 0, SOIL_LOAD_RGBA);
    if (data)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    SOIL_free_image_data(data);
    return texture;
}


void activate_texture(GLint texture_name)
{
    if(QUADS == 2)
    {
        glBindTexture(GL_TEXTURE_2D, texture_name);
        glEnable(GL_TEXTURE_2D);
    }
    else
        glDisable(GL_TEXTURE_2D);
}

void draw_text(char *string, GLfloat x, GLfloat y, GLfloat z) {
    glPushMatrix();

    glRasterPos3f(x, y, z);
    while (*string)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *string++);

    glPopMatrix();
}


void draw_button_b(double z)
{
    int num_vertex = 100;
    const GLfloat delta_angle = 2.0*PI/num_vertex;
    float angle, radian, x, y, xcos, ysin, tx, ty;
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);

    for (angle=0.0; angle<360.0; angle+=2.0)
    {
        radian = angle * (PI/180.0f);
        xcos = (float)cos(radian);
        ysin = (float)sin(radian);

        tx = xcos * 0.5 + 0.5;
        ty = ysin * 0.5 + 0.5;

        x = (float)xcos * 5  + 35;
        y = (float)ysin * 5  + -25;

        glTexCoord2f(tx, ty);
        glVertex3f(x, y, z);
    }
    glEnd();
}

void draw_button_a_sides(int z)
{
    int num_vertex = 100;
    const GLfloat delta_angle = 2.0*PI/num_vertex;
    float angle, radian, x, y, xcos, ysin, tx, ty;
    glColor3f(0.0, 1.0, 0.0);
    for (angle=0.0; angle<360.0; angle+=0.5)
    {
        glBegin(GL_QUADS);
        radian = angle * (PI/180.0f);
        xcos = (float)cos(radian);
        ysin = (float)sin(radian);
        tx = xcos * 0.5 + 0.5;
        ty = ysin * 0.5 + 0.5;

        x = (float)xcos * 5  + 45;
        y = (float)ysin * 5  + -15;
        // glTexCoord2f(tx, ty);
        glVertex3f(x, y, z + 2);
        glVertex3f(x, y, z);
        angle += 0.5;
        radian = angle * (PI/180.0f);
        xcos = (float)cos(radian);
        ysin = (float)sin(radian);
        tx = xcos * 0.5 + 0.5;
        ty = ysin * 0.5 + 0.5;

        x = (float)xcos * 5  + 45;
        y = (float)ysin * 5  + -15;
        // glTexCoord2f(tx, ty);
        glVertex3f(x, y, z);
        glVertex3f(x, y, z + 2);

        glEnd();
        angle -= 0.5;
    }
}


void draw_button_b_sides(int z)
{
    int num_vertex = 100;
    const GLfloat delta_angle = 2.0*PI/num_vertex;
    float angle, radian, x, y, xcos, ysin, tx, ty;
    glColor3f(1.0, 0, 0.0);
    for (angle=0.0; angle<360.0; angle+=0.5)
    {
        glBegin(GL_QUADS);
        radian = angle * (PI/180.0f);
        xcos = (float)cos(radian);
        ysin = (float)sin(radian);
        tx = xcos * 0.5 + 0.5;
        ty = ysin * 0.5 + 0.5;

        x = (float)xcos * 5  + 35;
        y = (float)ysin * 5  + -25;
        // glTexCoord2f(tx, ty);
        glVertex3f(x, y, z + 2);
        glVertex3f(x, y, z);
        angle += 0.5;
        radian = angle * (PI/180.0f);
        xcos = (float)cos(radian);
        ysin = (float)sin(radian);
        tx = xcos * 0.5 + 0.5;
        ty = ysin * 0.5 + 0.5;

        x = (float)xcos * 5  + 35;
        y = (float)ysin * 5  + -25;
        // glTexCoord2f(tx, ty);
        glVertex3f(x, y, z);
        glVertex3f(x, y, z + 2);

        glEnd();
        angle -= 0.5;
    }
}


void vis_objeto(int op)
{
    switch(op)
    {
        case 1:
            QUADS = 1;
            break;
        case 2:
            QUADS = 2;
            break;
        default:
            QUADS = 0;
            break;
    }
    glutPostRedisplay();
}

void main_menu(int op)
{

}

void turn_up_the_light(int op)
{
    switch(op)
    {
        case 1:
            glEnable(GL_LIGHT0);
            break;
        default:
            glDisable(GL_LIGHT0);
            break;
    }
    glutPostRedisplay();
}

void turn_up_the_anti_aliasing(int op)
{
    switch(op)
    {
        case 1:
            ANTI_ALIASING = 1;
            break;
        default:
            ANTI_ALIASING = 0;
            break;
    }
    glutPostRedisplay();
}

void turn_shadow(int op)
{
    switch(op)
    {
        case 1:
            glShadeModel(GL_SMOOTH);
            break;
        default:
            glShadeModel(GL_FLAT);
            break;
    }
}

void Menu()
{
    int menu, submenu1, submenu2, submenu3, submenu4;
    submenu1 = glutCreateMenu(vis_objeto);
    glutAddMenuEntry("Arames", 0);
    glutAddMenuEntry("Solido", 1);
    glutAddMenuEntry("Textura", 2);

    submenu2 = glutCreateMenu(turn_up_the_light);
    glutAddMenuEntry("On", 1);
    glutAddMenuEntry("Off", 0);

    submenu3 = glutCreateMenu(turn_up_the_anti_aliasing);
    glutAddMenuEntry("On", 1);
    glutAddMenuEntry("Off", 0);

    submenu4 = glutCreateMenu(turn_shadow);
    glutAddMenuEntry("On", 1);
    glutAddMenuEntry("Off", 0);

    menu = glutCreateMenu(main_menu);
    glutAddSubMenu("Objeto", submenu1);
    glutAddSubMenu("Iluminacao", submenu2);
    glutAddSubMenu("Anti-Aliasing", submenu3);
    glutAddSubMenu("Shading", submenu4);

    glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void multisamplingOn(int msaa) {
    if (msaa) {
        // glutSetOption(GLUT_MULTISAMPLE, 4);
        glEnable(GL_MULTISAMPLE);
    }
    else {
        // glutSetOption(GLUT_MULTISAMPLE, 0);
        glDisable(GL_MULTISAMPLE);
    }
}

static int spinningX = 0;
static int spinningY = 0;
static int spinningLongPiece = 0;
static GLfloat spinX = 0.0;
static GLfloat spinY = 0.0;
static GLfloat spinLongPiece = 0.0;
static GLfloat speedX = 0.15;
static GLfloat speedY = 0.15;
static GLfloat speedLongPiece = 0.15;

void spinDisplay(GLfloat *spin, GLfloat speed)
{
    (*spin)=(*spin)+speed;
    if ((*spin) > 360.0)
        (*spin)=(*spin)-360.0;
    glutPostRedisplay();
}

void spinAxisX(void) {
    spinDisplay(&spinX, speedX*spinningX);
}

void spinAxisY(void) {
    spinDisplay(&spinY, speedY*spinningY);
}

void spinLongPieceAxisZ(void) {
    spinDisplay(&spinLongPiece, speedLongPiece*spinningLongPiece);
}

void spinAllAxes(void) {
    spinAxisX();
    spinAxisY();
    spinLongPieceAxisZ();
}

void resetSpin(GLfloat *spin) {
    (*spin) = 0.0;
}

void resetSpinX(void) {
    resetSpin(&spinX);
}

void resetSpinY(void) {
    resetSpin(&spinY);
}

void resetSpinLongPiece(void) {
    resetSpin(&spinLongPiece);
}

void resetAllSpins(void) {
    resetSpinX();
    resetSpinY();
    resetSpinLongPiece();
}

char *helpObjects[10] = {
    "Objeto:",
    "- Inclinar para baixo: Seta 'Para Cima'",
    "- Inclinar para cima: Seta 'Para Baixo'",
    "- Rodar para a direita: Seta 'Para a Direita'",
    "- Rodar para a esquerda: Seta 'Para a Esquerda'",
    "- Rodar tetramino 4x1 para a esquerda: Tecla 'J'",
    "- Rodar tetramino 4x1 para a direita: Tecla 'K'",
    "- Aumentar a velocidade: Tecla 'A'",
    "- Diminuir a velocidade: Tecla 'Z'",
    "- Redefinir rotacao: Tecla 'R'"
};

char *helpViews[10] = {
    "Vistas:",
    "- Zoom In: Tecla '+' (Numpad)",
    "- Zoom Out: Tecla '-' (Numpad)",
    "- Camera 1: Tecla 'F1'",
    "- Camera 2: Tecla 'F2'",
    "- Camera 3: Tecla 'F3'",
    "- Camera 4: Tecla 'F4'",
    "- Camera 5: Tecla 'F5'",
    "- Camera 6: Tecla 'F6'",
    "- Camera 7: Tecla 'F7'"
};

char *helpGeneral[4] = {
    "Geral:",
    "- Esconder/Mostrar Labels: Tecla 'L'",
    "- Ver 'menu': Botão direito do rato",
    "- Sair do Programa: Tecla 'Esc'"
};

class SkyBox
{
    double positions [72]=  {
    // Front face
    100.0, 100.0, 100.0,
    -100.0, 100.0, 100.0,
    -100.0, -100.0, 100.0,
    100.0, -100.0, 100.0,

    // Back face
    -100.0, 100.0, -100.0,
    100.0, 100.0, -100.0,
    100.0, -100.0, -100.0,
    -100.0, -100.0, -100.0,

    // Top face
    100.0, 100.0, -100.0,
    -100.0, 100.0, -100.0,
    -100.0, 100.0, 100.0,
    100.0, 100.0, 100.0,

    // Bottom face
    -100.0, -100.0, -100.0,
    100.0, -100.0, -100.0,
    100.0, -100.0, 100.0,
    -100.0, -100.0, 100.0,

    // Right face
    100.0, 100.0, -100.0,
    100.0, 100.0, 100.0,
    100.0, -100.0, 100.0,
    100.0, -100.0, -100.0,

    // Left face
    -100.0, 100.0, 100.0,
    -100.0, 100.0, -100.0,
    -100.0, -100.0, -100.0,
    -100.0, -100.0, 100.0,
    };

    double vertexNormals [72] = {
    // Front
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,

    // Back
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,

    // Top
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,

    // Bottom
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,

    // Right
    -1.0, 0.0, 0.0,
    -1.0, 0.0, 0.0,
    -1.0, 0.0, 0.0,
    -1.0, 0.0, 0.0,

    // Left
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0
    };

    double textureCoordinates [48] = {
    // Front
    (double)1/4, (double)1/3,
    (double)2/4, (double)1/3,
    (double)2/4, (double)2/3,
    (double)1/3, (double)2/3,

    // Back
    (double)3/4, (double)1/3,
    (double)1.0, (double)1/3,
    (double)1.0, (double)2/3,
    (double)3/4, (double)2/3,
    // Top
    (double)1/4, 0.0,
    (double)2/4, 0.0,
    (double)2/4, (double)1/3,
    (double)1/4, (double)1/3,
    // Bottom
    (double)1/4, (double)2/3,
    (double)2/4,(double) 2/3,
    (double)2/4, (double)1.0,
    (double)1/4, (double)1.0,
    // Right
    0.0, (double)1/3,
    (double)1/4, (double)1/3,
    (double)(double)1/4, (double)2/3,
    0.0, (double)2/3,

    // Left
    (double)2/4, (double)1/3,
    (double)3/4, (double)1/3,
    (double)3/4, (double)2/3,
    (double)2/4, (double)2/3,

    };
    public:
        GLint texture_name;
    SkyBox(){}

    SkyBox(GLint texture)
    {
        texture_name = texture;
    }
    void enable_texture()
    {
        if(QUADS == 2)
        {
            glBindTexture(GL_TEXTURE_2D, texture_name);
            glEnable(GL_TEXTURE_2D);
        }
        else
            glDisable(GL_TEXTURE_2D);
    }
    void generate()
    {
        enable_texture();
        int check = 0;
        // glColor3f(1.0, 0, 0);
        for(int i = 0; i < 72; i += 12)
        {
            glBegin(GL_QUADS);
                for(int j = i; j < i + 12; j += 3)
                {
                    // cout << positions[j] << " "<< positions[j + i] << "\n";

                    glTexCoord2f(textureCoordinates[check++], textureCoordinates[check++]); glNormal3f(vertexNormals[j], vertexNormals[j + 1], vertexNormals[j + 2]); glVertex3f(positions[j], positions[j + 1], positions[j + 2]);
                }
            glEnd();
        }
    }

};

class GameBoi
{

    private:
        const char * texture_btn_arr_order[5] = {
            "gb_btn_arrow_middle", "gb_btn_arrow_left",
            "gb_btn_arrow_right", "gb_btn_arrow_right",
            "gb_btn_arrow_up"
        };

    public:
        double _arrow_x, _arrow_y, _arrow_z, _a_x, _a_y, _b_x, _b_y, _a_b_z, _r, _h ;


    GameBoi(){}

    GameBoi(double arrow_x, double arrow_y, double arrow_z, double a_x, double a_y, double b_x, double b_y, double a_b_z, double radius, double height)
    {
        _arrow_x = arrow_x;
        _arrow_y = arrow_y;
        _arrow_z = arrow_z;
        _a_x = a_x;
        _a_y = a_y;
        _b_x = b_x;
        _b_y = b_y;
        _a_b_z = a_b_z;
        _r = radius;
        _h = height;
    }

    void generate()
    {
        glPushMatrix();
        draw_console();
        draw_btn_arrow();
        glColor3f(0.0, 1, 0);
        draw_btn_circle(texture_map["a_btn"], _a_x, _a_y, _a_b_z);
        glColor3f(1.0, 0, 0);
        draw_btn_circle(texture_map["b_btn"], _b_x, _b_y, _a_b_z);
        glPopMatrix();
    }

    void change_texture(GLint texture_name)
    {
        if(QUADS == 2)
        {
            glBindTexture(GL_TEXTURE_2D, texture_name);
            glEnable(GL_TEXTURE_2D);
        }
        else
        {
            glDisable(GL_TEXTURE_2D);
        }    
    }
    
    void game_end_screen()
    {
        glColor3f(0.5, 0.5, 0.5);
        change_texture(texture_map["game_over"]);
        glBegin(GL_QUADS);
            glTexCoord2d(0.0, 0.0);
            glVertex3d(-0.045, -0.01, 0.009);
            glTexCoord2d(1.0, 0.0);
            glVertex3d(0.045, -0.01, 0.009);
            glTexCoord2d(1.0, 1.0);
            glTexCoord2d(1.0, 1.0);
            glVertex3d(0.045, 0.11, 0.009);
            glTexCoord2d(0.0, 1.0);
            glVertex3d(-0.045, 0.11, 0.009);
        glEnd();  
    }


   void game_pause_screen()
    {
         glColor3f(0.5, 0.5, 0.5);
        change_texture(texture_map["pause_screen"]);
        glBegin(GL_QUADS);
            glTexCoord2d(0.0, 0.0);
            glVertex3d(-0.045, -0.01, 0.009);
            glTexCoord2d(1.0, 0.0);
            glVertex3d(0.045, -0.01, 0.009);
            glTexCoord2d(1.0, 1.0);
            glTexCoord2d(1.0, 1.0);
            glVertex3d(0.045, 0.11, 0.009);
            glTexCoord2d(0.0, 1.0);
            glVertex3d(-0.045, 0.11, 0.009);
        glEnd();  
    }

    private:
        void draw_console()
        {
            int n = sizeof(gameboy_point_map) / sizeof(gameboy_point_map)[0];
            int texture_pos = 0;
            
            // generate gameboi body
            int check = 0;
            change_texture(texture_map["gb_texture"]);
            glColor3f(0.5, 0, 0.5);
            for(int i = 0; i < n; i += 12)
            {
                glBegin(GL_QUADS);
                    for(int j = i; j < i + 12; j += 3)
                    {
                        glTexCoord2f(gameboy_texture_map[check++],gameboy_texture_map[check++]);glVertex3f(gameboy_point_map[j], gameboy_point_map[j + 1], gameboy_point_map[j + 2]);
                    }
                glEnd();
                texture_pos++;
            }
            glColor3f(0.5, 0.5, 0.5);
            glBegin(GL_QUADS);
                glVertex3d(-0.025, -0.01, -0.0010);
                glVertex3d(0.025, -0.01, -0.0010);
                glVertex3d(0.025, 0.095, -0.0010);
                glVertex3d(-0.025, 0.095, -0.0010);
            glEnd();  
        }

        void draw_btn_arrow()
        {
            glPushMatrix();
            glTranslatef(_arrow_x, _arrow_y, _arrow_z);
        
            glColor3f(0.5, 0.5, 0.5);

            int texture_pos = 0;
            // generate arrow
            int n = sizeof(btn_arrow_point_map) / sizeof(btn_arrow_point_map)[0];
            for(int i = 0; i < n; i += 12)
            {
                int check = 0;
                change_texture(texture_map[texture_btn_arr_order[texture_pos]]);
                glBegin(GL_QUADS);
                    for(int j = i; j < i + 12; j += 3)
                    {
                        glTexCoord2f(texture_position[check++],texture_position[check++]);
                        glVertex3f(btn_arrow_point_map[j], btn_arrow_point_map[j + 1], btn_arrow_point_map[j + 2]);
                    }
                glEnd();
                texture_pos++;
                texture_pos = texture_pos > 4 ? 1: texture_pos;

            }
            glPopMatrix();
        }

        void draw_btn_circle(GLint texture_name, double btn_x, double btn_y, double btn_z)
        {
            glPushMatrix();
            glTranslatef(btn_x, btn_y, btn_z);
            int num_vertex = 100;
            const GLfloat delta_angle = 2.0*PI/num_vertex;
            float angle, radian, x, y, xcos, ysin, tx, ty;
            change_texture(texture_name);
            glBegin(GL_POLYGON);
            for (angle=0.0; angle<360.0; angle+=2.0)
            {
                radian = angle * (PI/180.0f);
                xcos = (float)cos(radian);
                ysin = (float)sin(radian);

                tx = xcos * 0.5 + 0.5;
                ty = ysin * 0.5 + 0.5;

                x = (float)xcos * _r;
                y = (float)ysin * _r;

                glTexCoord2f(tx, ty);
                glVertex3f(x, y, _h);
            }
            glEnd();
            draw_btn_circle_side();
            glPopMatrix();
        }


        void draw_btn_circle_side()
        {
            int num_vertex = 100;
            const GLfloat delta_angle = 2.0*PI/num_vertex;
            float angle, radian, x, y, xcos, ysin, tx, ty;
            for (angle=0.0; angle<360.0; angle+=0.5)
            {
                glBegin(GL_QUADS);
                radian = angle * (PI/180.0f);
                xcos = (float)cos(radian);
                ysin = (float)sin(radian);
                tx = xcos * 0.5 + 0.5;
                ty = ysin * 0.5 + 0.5;

                x = (float)xcos * _r;
                y = (float)ysin * _r;
                // glTexCoord2f(tx, ty);
                glVertex3f(x, y, _h);
                glVertex3f(x, y, 0);
                angle += 0.5;
                radian = angle * (PI/180.0f);
                xcos = (float)cos(radian);
                ysin = (float)sin(radian);
                tx = xcos * 0.5 + 0.5;
                ty = ysin * 0.5 + 0.5;

                x = (float)xcos * _r;
                y = (float)ysin * _r;
                // glTexCoord2f(tx, ty);
                glVertex3f(x, y, 0);
                glVertex3f(x, y, _h);

                glEnd();
                angle -= 0.5;
            }
        }
};


class Square
{
    public:
        double _x, _y, _z;
    Square(){};
    Square(double x, double y, double z)
    {
        _x = x;
        _y = y;
        _z = z;
    }
    void generate()
    {
        glPushMatrix();
        glTranslatef(_x, _y, _z);
            Cube o1(tetromino_texture_map[2], 0.0, 0.0, 0.0, SIDE);
            Cube o2(tetromino_texture_map[2], 0.0 - SIDE, 0.0, 0.0, SIDE);
            Cube o3(tetromino_texture_map[2], 0.0 - SIDE, 0.0 - SIDE, 0.0, SIDE);
            Cube o4(tetromino_texture_map[2], 0.0, 0.0 - SIDE, 0.0, SIDE);
            o1.generate(); o2.generate(); o3.generate(); o4.generate();
        glPopMatrix();
    } 
};

class LongBoi
{
    public:
        double _x, _y, _z;
    LongBoi(){};
    LongBoi(double x, double y, double z)
    {
        _x = x;
        _y = y;
        _z = z;
    }
    void generate()
    {
        glPushMatrix();
        glTranslatef(_x, _y, _z);
            Cube o1(tetromino_texture_map[1], 0.0, 0.0, 0.0, SIDE);
            Cube o2(tetromino_texture_map[1], 0.0, 0.0 - SIDE, 0.0, SIDE);
            Cube o3(tetromino_texture_map[1], 0.0, 0.0 - SIDE * 2, 0.0, SIDE);
            Cube o4(tetromino_texture_map[1], 0.0, 0.0 - SIDE * 3, 0.0, SIDE);
            o1.generate(); o2.generate(); o3.generate(); o4.generate();
        glPopMatrix();
    } 
};
class L_Boi
{
    public:
        double _x, _y, _z;
    L_Boi(){};
    L_Boi(double x, double y, double z)
    {
        _x = x;
        _y = y;
        _z = z;
    }
    void generate()
    {
        glPushMatrix();
        glTranslatef(_x, _y, _z);
            Cube o1(tetromino_texture_map[7], 0.0, 0.0, 0.0, SIDE);
            Cube o2(tetromino_texture_map[7], 0.0, 0.0 - SIDE, 0.0, SIDE);
            Cube o3(tetromino_texture_map[7], 0.0, 0.0 - SIDE * 2, 0.0, SIDE);
            Cube o4(tetromino_texture_map[7], 0.0 + SIDE, 0.0 - SIDE * 2, 0.0, SIDE);
            o1.generate(); o2.generate(); o3.generate(); o4.generate();
        glPopMatrix();
    } 
};
class J_Boi
{
    public:
        double _x, _y, _z;
    J_Boi(){};
    J_Boi(double x, double y, double z)
    {
        _x = x;
        _y = y;
        _z = z;
    }
    void generate()
    {
        glPushMatrix();
        glTranslatef(_x, _y, _z);
            Cube o1(tetromino_texture_map[6], 0.0, 0.0, 0.0, SIDE);
            Cube o2(tetromino_texture_map[6], 0.0, 0.0 - SIDE, 0.0, SIDE);
            Cube o3(tetromino_texture_map[6], 0.0, 0.0 - SIDE * 2, 0.0, SIDE);
            Cube o4(tetromino_texture_map[6], 0.0 - SIDE, 0.0 - SIDE * 2, 0.0, SIDE);
            o1.generate(); o2.generate(); o3.generate(); o4.generate();
        glPopMatrix();
    } 
};
class Z_Boi
{
    public:
        double _x, _y, _z;
    Z_Boi(){};
    Z_Boi(double x, double y, double z)
    {
        _x = x;
        _y = y;
        _z = z;
    }
    void generate()
    {
        glPushMatrix();
        glTranslatef(_x, _y, _z);
            Cube o1(tetromino_texture_map[5], 0.0, 0.0, 0.0, SIDE);
            Cube o2(tetromino_texture_map[5], 0.0, 0.0 - SIDE, 0.0, SIDE);
            Cube o3(tetromino_texture_map[5], 0.0 - SIDE , 0.0 - SIDE, 0.0, SIDE);
            Cube o4(tetromino_texture_map[5], 0.0 - SIDE, 0.0 - SIDE * 2, 0.0, SIDE);
            o1.generate(); o2.generate(); o3.generate(); o4.generate();
        glPopMatrix();
    } 
};

class S_Boi
{
    public:
        double _x, _y, _z;
    S_Boi(){};
    S_Boi(double x, double y, double z)
    {
        _x = x;
        _y = y;
        _z = z;
    }
    void generate()
    {
        glPushMatrix();
        glTranslatef(_x, _y, _z);
            Cube o1(tetromino_texture_map[4], 0.0, 0.0, 0.0, SIDE);
            Cube o2(tetromino_texture_map[4], 0.0, 0.0 - SIDE, 0.0, SIDE);
            Cube o3(tetromino_texture_map[4], 0.0 + SIDE , 0.0 - SIDE, 0.0, SIDE);
            Cube o4(tetromino_texture_map[4], 0.0 + SIDE, 0.0 - SIDE * 2, 0.0, SIDE);
            o1.generate(); o2.generate(); o3.generate(); o4.generate();
        glPopMatrix();
    } 
};

class T_Boi
{
    public:
        double _x, _y, _z;
    T_Boi(){};
    T_Boi(double x, double y, double z)
    {
        _x = x;
        _y = y;
        _z = z;
    }
    void generate()
    {
        glPushMatrix();
        glTranslatef(_x, _y, _z);
            Cube o1(tetromino_texture_map[3], 0.0 - SIDE, 0.0 - SIDE, 0.0, SIDE);
            Cube o2(tetromino_texture_map[3], 0.0 + SIDE, 0.0 - SIDE, 0.0, SIDE);
            Cube o3(tetromino_texture_map[3], 0.0, 0.0 - SIDE, 0.0 - SIDE, SIDE);
            Cube o4(tetromino_texture_map[3], 0.0, 0.0, 0.0, SIDE);
            o1.generate(); o2.generate(); o3.generate(); o4.generate();
        glPopMatrix();
    } 
};


void get_next_piece()
{
    int next_p = static_cast<int>(oglt.viewNextPiece(0).getShapeName());

    switch (next_p)
    {
        case 1:
        {
            LongBoi lp(.035, 0.085, 0.0085);
            lp.generate();
            break; 
        }
        case 2:
        {
            Square sp(.035, 0.085, 0.0085);
            sp.generate();
            break;
        }
        case 3:
        {
            T_Boi tp(.035, 0.085, 0.0085);
            tp.generate();
            break; 
        }
        case 4:
        {
            S_Boi sp(.035, 0.085, 0.0085);
            sp.generate();
            break;
        }
        case 5:
        {
            Z_Boi zp(.035, 0.085, 0.0085);
            zp.generate();
            break;
        }
        case 6:
        {
            J_Boi jp(.035, 0.085, 0.0085);
            jp.generate();
            break;
        }
        case 7:
        {
            L_Boi lp(.035, 0.085, 0.0085);
            lp.generate();
            break;
        }
    }

}



void compile_game()
{
    SkyBox sb(texture_map["skybox"]);
    sb.generate();

    GameBoi gb(-0.028, -0.064, 0.012, 0.046, -0.04, 0.025, -0.06, 0.007, 0.01, 0.005);
    gb.generate();

    elapsedTime = std::chrono::duration<double>(std::chrono::steady_clock::now() - startTime).count();


    

    if(STATE)
    {
        bool game = oglt.nextState(elapsedTime, startTime);
        if (game) {
            get_next_piece();
            // game_time_counter++;
            cout << oglt << "\n\n";
        }
        else
        {
            gb.game_end_screen();
            
        }
    }
    else
    {
        gb.game_pause_screen();
    }
    

}

void display(void)
{
    if(QUADS == 2)
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glMatrixMode(GL_MODELVIEW);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (QUADS) {
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_FILL);
    }
    else {
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK, GL_LINE);
    }

    glLoadIdentity();

    if (LABEL) {
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
    
        glColor3f(0.96, 0.86, 0.58);
        for (int i = 0; i < 4; ++i)
            draw_text(helpGeneral[i], -1.85, 1.80+i*(-0.08), -3.0);
        for (int i = 4; i < 14; ++i)
            draw_text(helpObjects[i-4], -1.85, 1.75+i*(-0.08), -3.0);
        for (int i = 0; i < 10; ++i)
            draw_text(helpViews[i], -1.85, -1.0+i*(-0.08), -3.0);

        if (QUADS != 2)
            glEnable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
    }

    gluLookAt(
        currentCamera->viewPos[0],
        currentCamera->viewPos[1],
        currentCamera->viewPos[2],

        currentCamera->center[0],
        currentCamera->center[1],
        currentCamera->center[2],

        currentCamera->up[0],
        currentCamera->up[1],
        currentCamera->up[2]
    );

    multisamplingOn(ANTI_ALIASING);
/*    glFrontFace(GL_CCW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE); */

/*    glTranslatef(-10.0,-10.0,0.0);  */
    // glLoadIdentity();
    glRotatef(spinX, 1.0, 0.0, 0.0);
    glRotatef(spinY, 0.0, 1.0, 0.0);
    glScalef(500.0, 500.0, 500.0);
    glTranslatef(0.0, 0.0, 0.0);
    // double z = 2.5;

    // load class tetris or w/e


    // cout << sizeof(gameboy_point_map)/sizeof(gameboy_point_map[0]);
    // glColor3f(1.0, 0, 0);


    // falling pieces
    double currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    double deltaTime = currentTime - lastTime;
    lastTime = currentTime;


    compile_game();


    for (auto it = fallingPieces.begin(); it != fallingPieces.end();) {
        if ((*it).m_lifetime >= 5.0) {
            fallingPieces.erase(it);
            continue;
        }
        (*it).updatePhysics(deltaTime);

        if ((*it).m_lifetime >= 3.0 && ((int) round(100*(*it).m_lifetime)) % 10 >= 5) {
            ++it;
            continue;
        }

        (*it).generate();

        ++it;
    }


    glutSwapBuffers();
}


void reshape(int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glOrtho(-50.0, 50.0, -50.0, 50.0, -1000.0, 1000.0);

    gluPerspective(65, 1, 1, CAMERA_FAR);
}

void mouseHandler(int button, int state, int x, int y)
{
    switch (button) {
      case GLUT_LEFT_BUTTON:
        // if (state == GLUT_DOWN) {
        //     if (spinning) {
        //         spinning = 0;
        //         glutIdleFunc(NULL);
        //     }
        //     else {
        //         spinning = 1;
        //         glutIdleFunc(spinDisplay);
        //     }
        // }
        break;
      case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
            Menu();
        break;
    }
}


static GLfloat defaultSpeedUp = 1.2;

void spinSpeedUp(GLfloat *speed) {
    if (*speed < 1.2) {
        (*speed) = (*speed)*defaultSpeedUp;
    }
}

void allSpinsSpeedUp(void) {
    spinSpeedUp(&speedX);
    spinSpeedUp(&speedY);
    spinSpeedUp(&speedLongPiece);
}

void spinSpeedDown(GLfloat *speed) {
    if (*speed > 0.05) {
        (*speed) = (*speed)/defaultSpeedUp;
    }
}

void allSpinsSpeedDown(void) {
    spinSpeedDown(&speedX);
    spinSpeedDown(&speedY);
    spinSpeedDown(&speedLongPiece);
}

void keyboardHandler(unsigned char key, int x, int y) {
    switch (key) {
        case '+':
            changeCameraZoom(currentCamera, 4);
            display();
            break;

        case '-':
            changeCameraZoom(currentCamera, -4);
            display();
            break;

        case 'j':
            spinningLongPiece = 1;
            break;

        case 'k': 
            spinningLongPiece = -1;
            break;

        case 'a':
            // allSpinsSpeedUp();
            break;

        case 'z':
            oglt.m_currentMove = OpenGLTetris::Move::ROTATE_COUNTER_CLOCK;
            // allSpinsSpeedDown();
            break;

        case ' ':
            oglt.m_currentMove = OpenGLTetris::Move::HARD_DROP;
            break;

        case 'r':
            resetAllSpins();
            break;

        case 'l':
            if (LABEL)
                LABEL = 0;
            else
                LABEL = 1;
            break;

        case 27:             // ESCAPE key
            exit(0);
            break;

        case 'p':
            STATE = STATE == 1 ? 0 : 1;
            display();
            break;
        
        case 'n':
            OpenGLTetris new_game{SIDE, &tetromino_texture_map};
            oglt = new_game;
            display();
            break;
        
 
    }
}

void keyboardUpHandler(unsigned char key, int x, int y) {
    switch (key) {
        case 'j':
            spinningLongPiece = 0;
            break;

        case 'k':
            spinningLongPiece = 0;
            break;

        case 'z':
            oglt.m_currentMove = OpenGLTetris::Move::NO_MOVE;
            oglt.m_movementAllowed = true;
            // allSpinsSpeedDown();
            break;

        case ' ':
            oglt.m_currentMove = OpenGLTetris::Move::NO_MOVE;
            oglt.m_movementAllowed = true;
            break;
    }
}


void keyboardSpecialHandler(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1: // front camera
          currentCamera = &cameraFront;
          display();
          break;
        case GLUT_KEY_F2: // right camera
          currentCamera = &cameraRight;
          display();
          break;
        case GLUT_KEY_F3: // back camera
          currentCamera = &cameraBack;
          display();
          break;

        case GLUT_KEY_F4: // left camera
          currentCamera = &cameraLeft;
          display();
          break;
        case GLUT_KEY_F5: // top camera
          currentCamera = &cameraTop;
          display();
          break;
        case GLUT_KEY_F6: // bottom camera
          currentCamera = &cameraBottom;
          display();
          break;
        case GLUT_KEY_F7: // bottom camera
          currentCamera = &cameraDiagonal;
          display();
          break;

          // Movement Keys
        case GLUT_KEY_UP:
            // spinningX = -1;
            oglt.m_currentMove = OpenGLTetris::Move::ROTATE_CLOCK;
            break;
        case GLUT_KEY_DOWN:
            oglt.m_currentMove = OpenGLTetris::Move::DISABLE_SOFT_DROP;
            // spinningX = 1;
            break;
        case GLUT_KEY_LEFT:
            // spinningY = -1;
            oglt.m_currentMove = OpenGLTetris::Move::MOVE_LEFT;
            break;
        case GLUT_KEY_RIGHT:
            // spinningY = 1;
            oglt.m_currentMove = OpenGLTetris::Move::MOVE_RIGHT;
            break;
    }
}

void keyboardSpecialUpHandler(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            // spinningX = 0;
            oglt.m_currentMove = OpenGLTetris::Move::NO_MOVE;
            oglt.m_movementAllowed = true;
            break;
        case GLUT_KEY_DOWN:
            // spinningX = 0;
            // oglt.m_currentMove = OpenGLTetris::Move::DISABLE_SOFT_DROP;
            break;
        case GLUT_KEY_LEFT:
            // spinningY = 0;
            oglt.m_currentMove = OpenGLTetris::Move::NO_MOVE;
            oglt.m_movementAllowed = true;
            break;
        case GLUT_KEY_RIGHT:
            // spinningY = 0;
            oglt.m_currentMove = OpenGLTetris::Move::NO_MOVE;
            oglt.m_movementAllowed = true;
            break;
    }
}

void init(void)
{
    // Light settings
    GLfloat luzAmbiente[4]={0.25,0.25,0.25,1.0};  // color (0.2, 0.2, 0.2)
    GLfloat luzDifusa[4]={1.0,1.0,1.0,1.0};    // "cor"
    GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho"
    GLfloat posicaoLuz[4]={22.5, 30.0, -50.0, 1.0};

    // Capacidade de brilho do material
    GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
    GLint especMaterial = 60;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glShadeModel (GL_SMOOTH);

    glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade); // define refletância do material
    glMateriali(GL_FRONT,GL_SHININESS,especMaterial); // define concentração do objeto
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente); // ativa luz ambiente

    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_DEPTH_TEST);

    //Set blending
    glEnable(GL_BLEND);
    // glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    multisamplingOn(ANTI_ALIASING);

    // //Set antialiasing/multisampling
    // glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    // glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // glDisable(GL_LINE_SMOOTH);
    // glDisable(GL_POLYGON_SMOOTH);
    // glDisable(GL_MULTISAMPLE);
    tetromino_texture_map.insert(pair<int, GLuint>(1, initTexture("textures/cyan_block.png")));
    tetromino_texture_map.insert(pair<int, GLuint>(2, initTexture("textures/yellow_block.png")));
    tetromino_texture_map.insert(pair<int, GLuint>(3, initTexture("textures/pink_block.png")));
    tetromino_texture_map.insert(pair<int, GLuint>(4, initTexture("textures/green_block.png")));
    tetromino_texture_map.insert(pair<int, GLuint>(5, initTexture("textures/red_block.png")));
    tetromino_texture_map.insert(pair<int, GLuint>(6, initTexture("textures/blue_block.jpg")));
    tetromino_texture_map.insert(pair<int, GLuint>(7, initTexture("textures/orange_block.png")));
    texture_map.insert(pair<string, GLuint>("a_btn", initTexture("textures/a_btn.png")));
    texture_map.insert(pair<string, GLuint>("b_btn", initTexture("textures/b_btn.png")));
    texture_map.insert(pair<string, GLuint>("gb_btn_arrow", initTexture("textures/gb_btn_arrow.png")));
    texture_map.insert(pair<string, GLuint>("gb_btn_arrow_middle", initTexture("textures/gb_btn_middle.png")));
    texture_map.insert(pair<string, GLuint>("gb_btn_arrow_up", initTexture("textures/gb_btn_arrow_up.png")));
    texture_map.insert(pair<string, GLuint>("gb_btn_arrow_down", initTexture("textures/gb_btn_arrow_down.png")));
    texture_map.insert(pair<string, GLuint>("gb_btn_arrow_left", initTexture("textures/gb_btn_arrow_left.png")));
    texture_map.insert(pair<string, GLuint>("gb_btn_arrow_right", initTexture("textures/gb_btn_arrow_right.png")));
    texture_map.insert(pair<string, GLuint>("gb_texture", initTexture("textures/gameboie.png")));
    texture_map.insert(pair<string, GLuint>("skybox", initTexture("textures/skybox.png")));
    texture_map.insert(pair<string, GLuint>("game_over", initTexture("textures/game_over.png")));
    texture_map.insert(pair<string, GLuint>("pause_screen", initTexture("textures/pause_screen.png")));
    color_map.insert(pair<int, std::array<double, 3>>(1, {0.0, 1.0, 1.0}));
    color_map.insert(pair<int, std::array<double, 3>>(2, {1.0, 1.0, 0.0}));
    color_map.insert(pair<int, std::array<double, 3>>(3, {0.5, 0.0, 0.5}));
    color_map.insert(pair<int, std::array<double, 3>>(4, {0.0, 1.0, 0.0}));
    color_map.insert(pair<int, std::array<double, 3>>(5, {1.0, 0.0, 0.0}));
    color_map.insert(pair<int, std::array<double, 3>>(6, {0.0, 0.0, 1.0}));
    color_map.insert(pair<int, std::array<double, 3>>(7, {1.0, 0.65, 0.0}));
}


int main(int argc, char ** argv)
{
    // cout << oglt.currentPlayfield() << "\n";
    // while(elapsedTime <= game_time_counter)
    // {
    //     elapsedTime = std::chrono::duration<double>(std::chrono::steady_clock::now() - startTime).count();
    // }
    // oglt.nextState(elapsedTime, startTime);
    // game_time_counter++;
    // while(elapsedTime <= game_time_counter)
    // {
    //     elapsedTime = std::chrono::duration<double>(std::chrono::steady_clock::now() - startTime).count();
    // }
    // oglt.nextState(elapsedTime, startTime);
    // cout << oglt.currentPlayfield() << "\n";


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(900, 900);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(spinAllAxes);

    glutMouseFunc(mouseHandler);
    glutKeyboardFunc(keyboardHandler);
    glutKeyboardUpFunc(keyboardUpHandler);
    glutSpecialFunc (keyboardSpecialHandler);
    glutSpecialUpFunc(keyboardSpecialUpHandler);

    // oglt.nextState(elapsedTime, startTime);
       
    glutMainLoop();
}

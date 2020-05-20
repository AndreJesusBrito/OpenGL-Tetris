#ifndef OPENGLTETRIS_H
#define OPENGLTETRIS_H


#include <iostream>
#include <map>

#include <GL/glut.h>

#include "tetris.h"
#include "cube.h"


class OpenGLTetris: public tetris::Tetris {
public:
    enum class Move {
        NO_MOVE,

        ROTATE_CLOCK,
        ROTATE_COUNTER_CLOCK,
        MOVE_LEFT,
        MOVE_RIGHT,
        ENABLE_SOFT_DROP,
        DISABLE_SOFT_DROP,
        HARD_DROP,

        MAX_MOVES
    };

public:
    Move m_currentMove = Move::NO_MOVE;
    double m_side;
    std::map<int, GLuint> *m_tetrominoTextureMap;

    bool m_movementAllowed = true;

    OpenGLTetris();
    OpenGLTetris(double side, std::map<int, GLuint> *tetrominoTextureMap);

    virtual bool keystrokes() override final;
    virtual void nextStateExtra(double elapsedTime, bool keyHit) override final;
};

#endif
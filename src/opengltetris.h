#ifndef OPENGLTETRIS_H
#define OPENGLTETRIS_H


#include <iostream>
#include <map>

#include <GL/glut.h>

#include "tetris.h"
#include "cube.h"
#include "DynamicPiece.h"


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
    
private:
    bool m_movementAllowed = true;

public:
    Move m_currentMove = Move::NO_MOVE;
    double m_side;
    std::map<int, GLuint> *m_tetrominoTextureMap;


    OpenGLTetris();
    OpenGLTetris(double side, std::map<int, GLuint> *tetrominoTextureMap);
    void addFallingPieces(std::vector<tetris::Tetris::ClearedLine> clearLines);
    void allowMovement();

    virtual bool keystrokes() override final;
    virtual void nextStateExtra(double elapsedTime, bool keyHit, std::vector<ClearedLine> clearedLines) override final;
};

#endif
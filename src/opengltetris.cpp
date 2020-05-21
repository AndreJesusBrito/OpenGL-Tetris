#include "opengltetris.h"

OpenGLTetris::OpenGLTetris(): Tetris(), m_side{1.0}, m_tetrominoTextureMap{}
{

}

OpenGLTetris::OpenGLTetris(double side, std::map<int, GLuint> *tetrominoTextureMap)
    : Tetris(), m_side{side}
{
    m_tetrominoTextureMap = tetrominoTextureMap;
}

bool OpenGLTetris::keystrokes() {
    std::vector<ClearedLine> clearedLines;
    if (m_currentMove > Move::NO_MOVE && m_currentMove < Move::MAX_MOVES && m_movementAllowed) {
        if (m_currentMove == Move::ROTATE_CLOCK) {
            rotatePieceClockwise();
        }
        else if (m_currentMove == Move::ROTATE_COUNTER_CLOCK) {
            rotatePieceCounterClockwise();
        }
        else if (m_currentMove == Move::MOVE_LEFT) {
            movePieceLeft();
        }
        else if (m_currentMove == Move::MOVE_RIGHT) {
            movePieceRight();
        }
        // else if (m_currentMove == Move::ENABLE_SOFT_DROP) {
        //     enableSoftDropPiece();
        // }
        // else if (m_currentMove == Move::DISABLE_SOFT_DROP) {
        //     disableSoftDropPiece();
        // }
        else if (m_currentMove == Move::HARD_DROP) {
            clearedLines = hardDropPiece();
        }

        m_movementAllowed = false;

        return true;
    }
    return false;
}

void OpenGLTetris::nextStateExtra(double elapsedTime, bool keyHit, std::vector<ClearedLine> clearedLines) {
    double pos_x = -0.02;
    double pos_y = 0.085;
    double pos_z = 0.005;

    std::vector<Cube> gl;
    for(std::size_t i = 0; i < currentPlayfield().getHeight(); i++)
    {
        for(std::size_t j = 0; j < currentPlayfield().getWidth(); j++)
        {
            if(currentPlayfield().view(i, j))
            {
                Cube o1((*m_tetrominoTextureMap)[currentPlayfield().view(i, j)], pos_x, pos_y, pos_z, m_side);
                gl.push_back(o1);
            }
            pos_x += m_side;
        }
        pos_y -= m_side;
        pos_x = -0.02;
    }

    for(auto i = gl.begin(); i!= gl.end(); ++i)
    {
        (*i).generate();
    }
}

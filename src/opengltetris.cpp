#include "opengltetris.h"


// random generator
std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
std::uniform_real_distribution randomMagnitude{0.0, 1.0};

std::vector<DynamicPiece> fallingPieces{};

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
            m_movementAllowed = false;
        }
        else if (m_currentMove == Move::ROTATE_COUNTER_CLOCK) {
            rotatePieceCounterClockwise();
            m_movementAllowed = false;
        }
        else if (m_currentMove == Move::MOVE_LEFT) {
            movePieceLeft();
            m_movementAllowed = false;
        }
        else if (m_currentMove == Move::MOVE_RIGHT) {
            movePieceRight();
            m_movementAllowed = false;
        }
        else if (m_currentMove == Move::ENABLE_SOFT_DROP) {
            enableSoftDropPiece();
        }
        else if (m_currentMove == Move::DISABLE_SOFT_DROP) {
            disableSoftDropPiece();
        }
        else if (m_currentMove == Move::HARD_DROP) {
            clearedLines = hardDropPiece();

            addFallingPieces(clearedLines);
            m_movementAllowed = false;
        }

        // m_movementAllowed = false;

        return true;
    }
    return false;
}

void OpenGLTetris::nextStateExtra(double elapsedTime, bool keyHit, std::vector<ClearedLine> clearedLines) {
    double pos_x = -0.025;
    double pos_y = 0.085 - 0.3;
    double pos_z = 0.00085;

    
    WordDisplay cd("LINES " + std::to_string(m_linesCleared), 0.0012, {-0.02, -0.2, 0.0085}, {1, 1, 1});
    cd.generate();

    std::vector<Cube> gl;
    for(std::size_t i = 0; i < currentPlayfield().getHeight(); i++)
    {
        for(std::size_t j = 0; j < currentPlayfield().getWidth(); j++)
        {
            if(currentPlayfield().view(i, j))
            {
                Cube o1((*m_tetrominoTextureMap)[currentPlayfield().view(i, j)], 
                    pos_x + m_side*j, 
                    pos_y - m_side*i, 
                    pos_z, 
                    m_side);

                gl.push_back(o1);
            }
        }
    }

    Coords2D ghostPos = ghostPiecePosition();

    std::vector<Cube> ghost;

    for(std::size_t i = 0; i < m_currentPiece.currentFormation().getHeight(); ++i) {
        for(std::size_t j = 0; j < m_currentPiece.currentFormation().getWidth(); ++j) {
            if(m_currentPiece.currentFormation().view(i, j)) {
                Cube o2((*m_tetrominoTextureMap)[m_currentPiece.currentFormation().view(i, j)], 
                    pos_x + m_side*(j+ghostPos.second), 
                    pos_y - m_side*(i+ghostPos.first), 
                    pos_z, 
                    m_side);

                ghost.push_back(o2);
            }
        }
    }

    for(auto i = gl.begin(); i!= gl.end(); ++i)
    {
        (*i).generate();
    }

    for(auto i = ghost.begin(); i!= ghost.end(); ++i)
    {
        (*i).generate_ghost();
    }

    addFallingPieces(clearedLines);
}

void OpenGLTetris::addFallingPieces(std::vector<tetris::Tetris::ClearedLine> clearLines) {
    // double pos_x = -0.02;
    // double pos_y = 0.085;
    // double pos_z = 0.005;

    for (auto line = clearLines.begin(); line != clearLines.end(); ++line)
    {
        int linePos = line->first;

        for (int i = 0; i < 10; i++)
        {
            fallingPieces.push_back(
                DynamicPiece(
                    // texture
                    (*m_tetrominoTextureMap)[line->second.at(i)],

                    // pos
                    -0.02 + 0.01 * (i),
                    0.085 - 0.3 - 0.005 * linePos,
                    0.0125,

                    // velocity
                    0.05 * (randomMagnitude(mersenne) - .5),
                    0.05 * randomMagnitude(mersenne),
                    0.05 * randomMagnitude(mersenne),
                    // 0.0,
                    // 0.0,
                    // 0.0,

                    // acceleration
                    // 0.0, -0.0, 0.0,
                    0.0, -0.5, 0.0,

                    // rotation
                    0.0, 0.0, 0.0,

                    // rotation speed
                    (randomMagnitude(mersenne) - .5) * 573,
                    randomMagnitude(mersenne) * 573,
                    randomMagnitude(mersenne) * 573,
                    // 0.0,
                    // 0.0,
                    // 0.0,

                    // rotation acceleration
                    0.0, 0.0, 0.0));
        }
    }

}

void OpenGLTetris::allowMovement() {
    m_movementAllowed = true;
}

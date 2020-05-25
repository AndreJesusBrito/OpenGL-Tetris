#include "tetris.h"

namespace tetris {

    Tetris::Tetris(): m_playfield{}, m_pieceBuffer{} {
        m_currentPiece = Piece::getRandomPiece();
        for (int i = 0; i < 6; ++i)
            m_pieceBuffer.push_back(Piece::getRandomPiece());
    }

    bool Tetris::canPieceBePlaced(const Piece& piece, const Coords2D& position) const {
        std::size_t pieceFormHeight = piece.currentFormation().getHeight();
        std::size_t pieceFormWidth = piece.currentFormation().getWidth();

        for (std::size_t i = 0; i < pieceFormHeight; ++i) {
            for (std::size_t j = 0; j < pieceFormWidth; ++j) {
                if (piece.currentFormation().view(i, j)) {
                    if (i + position.first >= m_playfield.getHeight() 
                     || j + position.second >= m_playfield.getWidth()) {
                        return false;
                    }
                    else if (m_playfield.view(i + position.first, j + position.second)) {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    void Tetris::placePieceInField(const Piece& piece, const Coords2D& position) {
        std::size_t pieceFormHeight = piece.currentFormation().getHeight();
        std::size_t pieceFormWidth = piece.currentFormation().getWidth();

        for (std::size_t i = 0; i < pieceFormHeight; ++i) {
            for (std::size_t j = 0; j < pieceFormWidth; ++j) {
                if (piece.currentFormation().view(i, j)) {
                    m_playfield.at(i + position.first, j + position.second) 
                        = piece.currentFormation().view(i, j);
                }
            }
        }
    }

    void Tetris::nextPiece() {
        m_currentPiece = m_pieceBuffer.front();
        m_pieceBuffer.pop_front();
        m_pieceBuffer.push_back(Piece::getRandomPiece());
        m_piecePosition = {0, 3};
    }

    bool Tetris::movePiece(int vrtclDirection, int hrzntlDirection) {
        if (canPieceBePlaced(m_currentPiece, Coords2D{
                                                m_piecePosition.first+vrtclDirection,
                                                m_piecePosition.second+hrzntlDirection
                                            })) {
            m_piecePosition.first += vrtclDirection;
            m_piecePosition.second += hrzntlDirection;
            return true;
        }
        return false;
    }

    std::vector<Tetris::ClearedLine> Tetris::clearLines() {
        std::vector<ClearedLine> result;
        ClearedLine line;
        bool clear = true;
        for (std::size_t i = m_piecePosition.first; i < m_playfield.getHeight(); ++i) {
            for (std::size_t j = 0; j < m_playfield.getWidth(); ++j) {
                if (!m_playfield.view(i, j)) {
                    clear = false;
                    break;
                }
            }
            if (clear) {
                line = ClearedLine(i, {});
                for (std::size_t i2 = i; i2 > 0; --i2) {
                    for (std::size_t j2 = 0; j2 < m_playfield.getWidth(); ++j2) {
                        if (i2 == i)
                            line.second.at(j2) = m_playfield.at(i2, j2);
                        m_playfield.at(i2, j2) = m_playfield.at(i2-1, j2);
                    }
                }
                ++m_linesCleared;
                result.push_back(line);
            }
            clear = true;
        }

        return result;
    }

    const Piece& Tetris::viewNextPiece(int i) const {
        return m_pieceBuffer.at(i);
    }

    const Tetris::Coords2D Tetris::ghostPiecePosition() const {
        Coords2D result{m_piecePosition};
        while(canPieceBePlaced(m_currentPiece, result)) {
            ++result.first;
        }
        --result.first;
        return result;
    }

    bool Tetris::movePieceLeft() {
        return movePiece(0, -1);
    }

    bool Tetris::movePieceRight() {
        return movePiece(0, 1);
    }

    bool Tetris::movePieceDown() {
        return movePiece(1, 0);
    }

    std::vector<Tetris::ClearedLine> Tetris::placeWhenDownMovement() {
        std::vector<ClearedLine> result;
        if (!movePieceDown()) {
            placePieceInField(m_currentPiece, m_piecePosition);
            result = clearLines();
            nextPiece();
        }
        return result;
    }

    void Tetris::rotatePieceClockwise() {
        do {
            m_currentPiece.rotateClockwise();
        } while (!canPieceBePlaced(m_currentPiece, m_piecePosition));
    }

    void Tetris::rotatePieceCounterClockwise() {
        do {
            m_currentPiece.rotateCounterClockwise();
        } while (!canPieceBePlaced(m_currentPiece, m_piecePosition));
    }

    std::vector<Tetris::ClearedLine> Tetris::hardDropPiece() {
        bool prolong;
        do {
            prolong = movePieceDown();
        } while (prolong);
        placePieceInField(m_currentPiece, m_piecePosition);
        std::vector<ClearedLine> result = clearLines();
        nextPiece();
        return result;
    }

    void Tetris::enableSoftDropPiece() {
        m_stepTime = m_stepTimeRef/10.0;
    }

    void Tetris::disableSoftDropPiece() {
        m_stepTime = m_stepTimeRef;
    }

    Tetris::Playfield Tetris::currentPlayfield() const {
        Playfield result{};

        for (std::size_t i = 0; i < m_playfield.getHeight(); ++i) {
            int iInt = static_cast<int>(i);
            for (std::size_t j = 0; j < m_playfield.getWidth(); ++j) {
                int jInt = static_cast<int>(j);
                if (iInt >= m_piecePosition.first && iInt < m_piecePosition.first + static_cast<int>(m_currentPiece.currentFormation().getHeight())
                    && jInt >= m_piecePosition.second && jInt < m_piecePosition.second + static_cast<int>(m_currentPiece.currentFormation().getWidth())
                    && m_currentPiece.currentFormation().view(i - m_piecePosition.first, j - m_piecePosition.second)) {
                        result.at(i, j) = m_currentPiece.currentFormation().view(i - m_piecePosition.first, j - m_piecePosition.second);

                }
                else {
                    result.at(i, j) = m_playfield.view(i, j);
                }
            }
        }

        return result;
    }


    int Tetris::level() {
        return m_linesCleared / 10 + 1 > 20 ? 20 : m_linesCleared / 10 + 1;
    }

    void Tetris::updateStepTime() {
        m_stepTimeRef = pow(0.8 - ((level() - 1)*0.007), level() - 1);
    }

    // void Tetris::keystrokes() {}

    void Tetris::nextStateExtra(double elapsedTime, bool keyHit, std::vector<ClearedLine> clearedLines) {}

    bool Tetris::nextState(double elapsedTime, std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>& startTime) {
        bool keyHit = false;
        std::vector<ClearedLine> clearedLines;
        if (canPieceBePlaced(m_currentPiece, m_piecePosition)) {
            if (elapsedTime >= m_stepTime) {
                clearedLines = placeWhenDownMovement();
                startTime = std::chrono::steady_clock::now();
            }
            keyHit = keystrokes();

            updateStepTime();
        }
        else {
            return false;
        }
        nextStateExtra(elapsedTime, keyHit, clearedLines);
        return true;
    }

    void Tetris::gameloop() {
        bool game = true;
        auto startTime(std::chrono::steady_clock::now());
        // bool keyHit = false;
        while(game) {
            double elapsedTime = std::chrono::duration<double>(std::chrono::steady_clock::now() - startTime).count();
            // if (canPieceBePlaced(m_currentPiece, m_piecePosition)) {
            //     if (elapsedTime >= m_stepTime) {
            //         if (!movePieceDown()) {
            //             placePieceInField(m_currentPiece, m_piecePosition);
            //             clearLines();
            //             nextPiece();
            //         }
            //         startTime = std::chrono::steady_clock::now();
            //     }
            //     keyHit = keystrokes();
            // }
            // else {
            //     game = false;
            // }
            game = nextState(elapsedTime, startTime);
            // extraGameloop(elapsedTime, keyHit);
            // keyHit = false;
        }
    }

    std::ostream& operator<< (std::ostream &out, const Tetris &tetris) {
        out << tetris.currentPlayfield();

        return out;
    }
}

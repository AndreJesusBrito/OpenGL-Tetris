#ifndef TETRIS_H
#define TETRIS_H

#include <deque>
#include <utility>
#include <iostream>
#include <cmath>
#include <chrono>
#include <vector>
#include <array>

#include "matrix.h"
#include "piece.h"

namespace tetris {

    class Tetris {
    public:
        typedef mycontainers::Matrix<int, 10, 20> Playfield;
        typedef std::pair<int, int> Coords2D;
        typedef std::pair<int, std::array<int, 10>> ClearedLine;

    protected:
        Playfield m_playfield;
        int m_linesCleared = 0;
        std::deque<Piece> m_pieceBuffer;
        Piece m_currentPiece;

        // m_piecePosition is a pair of row and col indexes that point to where the
        // first element of the m_currentPiece formation should be translated to in the
        // m_playfield
        Coords2D m_piecePosition{0, 3};

        // time in seconds
        double m_stepTimeRef = 1.0;
        double m_stepTime = 1.0;

        bool canPieceBePlaced(const Piece& piece, const Coords2D& position) const;

        // Assumes piece can be placed
        virtual void placePieceInField(const Piece& piece, const Coords2D& position);

        void nextPiece();
        virtual bool movePiece(int vrtclDirection, int hrzntlDirection);
        virtual std::vector<ClearedLine> clearLines();

    public:
        Tetris();

        const Piece& viewNextPiece(int i) const;
        const Coords2D ghostPiecePosition() const;
        bool movePieceLeft();
        bool movePieceRight();
        bool movePieceDown();
        std::vector<ClearedLine> placeWhenDownMovement();
        void rotatePieceClockwise();
        void rotatePieceCounterClockwise();
        std::vector<ClearedLine> hardDropPiece();
        void enableSoftDropPiece();
        void disableSoftDropPiece();

        Playfield currentPlayfield() const;
        int level();
        int getNumLinesCleared();
        void updateStepTime();

        virtual bool keystrokes() = 0;
        virtual void nextStateExtra(double elapsedTime, bool keyHit, std::vector<ClearedLine> clearedLines);
        virtual bool nextState(double elapsedTime, std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>& startTime);
        void gameloop();

        friend std::ostream& operator<<(std::ostream &out, const Tetris &tetris);
    };
}

#endif

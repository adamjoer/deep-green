#pragma once

#include <QWidget>
#include <QLabel>

#include "square.h"
#include "../chess/team.h"

namespace Gui {

    class Board final : public QWidget {
    Q_OBJECT

    public:
        static const int SIZE = 8;

        explicit Board(QWidget *parent);

        void addTeamStartingPosition(const Chess::Team &team);

        void move(const Chess::Position &from, const Chess::Position &to);

        void reset();

        void clearHighlights();

        [[nodiscard]]
        const std::array<std::array<Chess::SquareState, Board::SIZE>, Board::SIZE>
        &getBoardRepresentation() const {
            return this->boardRepresentation;
        }

        void highlightPossibleMoves(const std::vector<Chess::Position> &possibleMoves);

        [[nodiscard]]
        int getSquarePixelSize() const { return this->squarePixelSize; }

        void setSquarePixelSize(int newSquarePixelSize);

        [[nodiscard]]
        const std::array<std::array<Square *, SIZE>, SIZE> &getSquares() const {
            return this->squares;
        }

    private:
        int squarePixelSize;

        std::array<std::array<Square *, SIZE>, SIZE> squares{nullptr};

        std::array<std::array<Chess::SquareState, SIZE>, SIZE> boardRepresentation;
    };
}

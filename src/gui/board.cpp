#include "board.h"

#include <cassert>

#include <QGridLayout>

#include "square.h"

namespace Gui {

    Board::Board(QWidget *parent)
        : QWidget(parent),
          squarePixelSize(Square::DEFAULT_PIXEL_SIZE) {

        auto *layout = new QGridLayout;
        layout->setSpacing(0);
        layout->setContentsMargins(0, 0, 0, 0);

        for (int column = 0; column < SIZE; ++column) {
            for (int row = 0; row < SIZE; ++row) {
                auto square = this->squares[column][row]
                                  = new Square(this, row, column);
                layout->addWidget(square, SIZE - 1 - row, column);
            }
        }
        setLayout(layout);

        setFixedSize(SIZE * this->squarePixelSize, SIZE * this->squarePixelSize);
    }

    void Board::addTeamStartingPosition(const Chess::Team &team) {
        static_assert((Chess::Team::SIZE / 2.0F) == SIZE);
        static_assert(Chess::Team::QUEEN_COLUMN > 0 && Chess::Team::QUEEN_COLUMN < SIZE);
        assert(team.isFull());

        const auto &queen = team.getQueen();
        const auto &officers = team.getOfficers();
        const auto &pawns = team.getPawns();

        int officerRow = (team.getColor() == Chess::Color::White) ? 0 : SIZE - 1;
        int pawnRow = (team.getColor() == Chess::Color::White) ? 1 : SIZE - 2;

        int i = 0;
        for (const auto &officer: officers) {
            if (i == Chess::Team::QUEEN_COLUMN)
                i++;
            this->squares[i++][officerRow]->setPiece(officer);
        }

        this->squares[Chess::Team::QUEEN_COLUMN][officerRow]->setPiece(queen);

        i = 0;
        for (const auto &pawn: pawns)
            this->squares[i++][pawnRow]->setPiece(pawn);
    }

    void Board::setSquarePixelSize(int newSquarePixelSize) {
        if (newSquarePixelSize <= 0 || newSquarePixelSize == this->squarePixelSize)
            return;

        this->squarePixelSize = newSquarePixelSize;

        setFixedSize(SIZE * this->squarePixelSize, SIZE * this->squarePixelSize);
    }
}

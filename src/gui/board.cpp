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

                this->boardRepresentation[column][row].color = Chess::Color::Empty;
            }
        }
        setLayout(layout);

        setFixedSize(SIZE * this->squarePixelSize, SIZE * this->squarePixelSize);
    }

    void Board::addTeamStartingPosition(const Chess::Team &team) {
        static_assert((Chess::Team::SIZE / 2.0F) == SIZE);
        static_assert(Chess::Team::QUEEN_COLUMN >= 0 && Chess::Team::QUEEN_COLUMN < SIZE);
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
            this->boardRepresentation[i][officerRow].color = team.getColor();
            this->squares[i++][officerRow]->setPiece(officer);
        }

        this->boardRepresentation[Chess::Team::QUEEN_COLUMN][officerRow].color = team.getColor();
        this->squares[Chess::Team::QUEEN_COLUMN][officerRow]->setPiece(queen);

        i = 0;
        for (const auto &pawn: pawns) {
            this->boardRepresentation[i][pawnRow].color = team.getColor();
            this->squares[i++][pawnRow]->setPiece(pawn);
        }
    }

    void Board::move(const Chess::Position &from, const Chess::Position &to) {
        auto origin = this->squares[from.first][from.second];
        auto destination = this->squares[to.first][to.second];

        this->boardRepresentation[from.first][from.second].color = Chess::Color::Empty;
        this->boardRepresentation[to.first][to.second].color = origin->getPiece()->color;

        destination->setPiece(origin->getPiece());
        origin->setPiece(nullptr);
    }

    void Board::reset() {
        for (int column = 0; column < SIZE; ++column) {
            for (int row = 0; row < SIZE; ++row) {
                auto square = this->squares[column][row];

                square->setState(Square::State::Default);
                square->setPiece(nullptr);

                this->boardRepresentation[column][row].color = Chess::Color::Empty;
            }
        }
    }

    void Board::clearHighlights() {
        for (auto &row: squares) {
            for (auto square: row) {
                square->setState(Square::State::Default);
            }
        }
    }

    void Board::highlightPossibleMoves(const std::vector<Chess::Position> &possibleMoves) {
        for (const auto &move: possibleMoves)
            this->squares[move.first][move.second]->setState(Square::State::PossibleMove);
    }

    void Board::setSquarePixelSize(int newSquarePixelSize) {
        if (newSquarePixelSize <= 0 || newSquarePixelSize == this->squarePixelSize)
            return;

        this->squarePixelSize = newSquarePixelSize;

        setFixedSize(SIZE * this->squarePixelSize, SIZE * this->squarePixelSize);
    }
}

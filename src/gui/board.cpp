#include "board.h"

#include <QGridLayout>

namespace Gui {

    Board::Board(QWidget *parent, const Chess::Board &chessBoard)
            : QWidget(parent) {
        createLayout();
        set(chessBoard);
    }

    Board::Board(QWidget *parent)
            : QWidget(parent) {
        createLayout();
    }

    void Board::createLayout() {
        auto *layout = new QGridLayout;
        layout->setSpacing(0);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setOriginCorner(Qt::Corner::BottomLeftCorner);

        for (int rank = 0; rank < 8; ++rank) {
            for (int file = 0; file < 8; ++file) {
                auto square = this->squares[rank * 8 + file]
                                      = new Square(this, rank, file);
                layout->addWidget(square, rank, file);
            }
        }
        setLayout(layout);

        setFixedSize(this->pixelSize, this->pixelSize);
    }

    void Board::performMove(Chess::Move move) {
        auto origin = this->squares[static_cast<int>(move.from)];
        auto destination = this->squares[static_cast<int>(move.to)];

        destination->setPiece(origin->getPiece());
        origin->setPiece(std::nullopt);

        if (move.enPassantCapture) {
            auto dropSquare = move.dropSquare.value();
            this->squares[static_cast<int>(dropSquare)]->setPiece(std::nullopt);
        }
    }

    void Board::set(const Chess::Board &chessBoard) {
        const auto whiteOccupiedSquares = chessBoard.teamOccupiedSquares(Chess::Color::White);
        const auto blackOccupiedSquares = chessBoard.teamOccupiedSquares(Chess::Color::Black);

        for (int i = 0; i < 64; ++i) {
            const auto square = Chess::Square(i);

            if (whiteOccupiedSquares.isOccupiedAt(square)) {
                auto type = chessBoard.pieceAt(square, Chess::Color::White);
                this->squares[i]->setPiece(std::make_optional(Piece(type, Chess::Color::White)));

            } else if (blackOccupiedSquares.isOccupiedAt(square)) {
                auto type = chessBoard.pieceAt(square, Chess::Color::Black);
                this->squares[i]->setPiece(std::make_optional(Piece(type, Chess::Color::Black)));

            } else {
                squares[i]->setPiece(std::nullopt);
            }
        }
    }

    void Board::clearHighlights() {
        for (auto square: squares) {
            square->setState(Square::State::Default);
        }
    }

    void Board::highlightPossibleMoves(const std::vector<Chess::Move> &possibleMoves) {
        for (const auto move: possibleMoves)
            this->squares[static_cast<int>(move.to)]->setState(Square::State::PossibleMove);
    }

    void Board::setPixelSize(int newPixelSize) {
        if (newPixelSize <= 0 || newPixelSize == this->pixelSize)
            return;

        this->pixelSize = newPixelSize;

        setFixedSize(this->pixelSize, this->pixelSize);
    }
}

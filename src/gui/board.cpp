#include "board.h"

#include <QGridLayout>

namespace Gui {

    Board::Board(QWidget *parent, const Chess::Board &chessBoard)
            : QWidget(parent) {
        for (int rank = 0; rank < 8; ++rank) {
            for (int file = 0; file < 8; ++file) {
                this->squares[rank * 8 + file] = new Square(this, rank, file);
            }
        }

        set(chessBoard);

        createLayout();
        setFixedSize(this->pixelSize, this->pixelSize);
    }

    Board::Board(QWidget *parent)
            : QWidget(parent) {
        for (int rank = 0; rank < 8; ++rank) {
            for (int file = 0; file < 8; ++file) {
                this->squares[rank * 8 + file] = new Square(this, rank, file);
            }
        }

        createLayout();
        setFixedSize(this->pixelSize, this->pixelSize);
    }

    Board::~Board() {
        for (auto square: this->squares)
            delete square;
    }

    void Board::createLayout() {
        auto *newLayout = new QGridLayout;
        newLayout->setSpacing(0);
        newLayout->setContentsMargins(0, 0, 0, 0);
        newLayout->setOriginCorner(this->originCorner);

        for (int rank = 0; rank < 8; ++rank) {
            for (int file = 0; file < 8; ++file) {
                newLayout->addWidget(this->squares[rank * 8 + file], rank, file);
            }
        }

        delete layout();
        setLayout(newLayout);
    }

    void Board::performMove(Chess::Move move) {
        auto origin = this->squares[static_cast<int>(move.from)];
        auto destination = this->squares[static_cast<int>(move.to)];

        clearRecentMoves();

        destination->setPiece(origin->getPiece());
        destination->setRecentMove(true);

        origin->setPiece(std::nullopt);
        origin->setRecentMove(true);

        if (move.enPassantCapture) {
            auto dropSquare = move.dropSquare.value();
            this->squares[static_cast<int>(dropSquare)]->setPiece(std::nullopt);
        }

        switch (move.castle) {
            case Chess::Castling::WhiteKing:
                this->squares[static_cast<int>(Chess::Square::H1)]->setPiece(std::nullopt);
                this->squares[static_cast<int>(Chess::Square::F1)]->setPiece(std::make_optional(
                        Piece(Chess::PieceType::Rook, Chess::Color::White)));
                break;
            case Chess::Castling::WhiteQueen:
                this->squares[static_cast<int>(Chess::Square::A1)]->setPiece(std::nullopt);
                this->squares[static_cast<int>(Chess::Square::D1)]->setPiece(std::make_optional(
                        Piece(Chess::PieceType::Rook, Chess::Color::White)));
                break;
            case Chess::Castling::BlackKing:
                this->squares[static_cast<int>(Chess::Square::H8)]->setPiece(std::nullopt);
                this->squares[static_cast<int>(Chess::Square::F8)]->setPiece(std::make_optional(
                        Piece(Chess::PieceType::Rook, Chess::Color::Black)));
                break;
            case Chess::Castling::BlackQueen:
                this->squares[static_cast<int>(Chess::Square::A8)]->setPiece(std::nullopt);
                this->squares[static_cast<int>(Chess::Square::D8)]->setPiece(std::make_optional(
                        Piece(Chess::PieceType::Rook, Chess::Color::Black)));
                break;
            case Chess::Castling::None:
                break;
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

        clearHighlights();
        clearRecentMoves();

        setDisabled(false);
    }

    void Board::clearHighlights() {
        for (auto square: squares)
            square->setState(Square::State::Default);
    }

    void Board::clearRecentMoves() {
        for (auto square : squares) {
            square->setRecentMove(false);
        }
    }

    void Board::highlightPossibleMoves(const std::vector<Chess::Move> &possibleMoves) {
        for (const auto move: possibleMoves)
            this->squares[static_cast<int>(move.to)]->setState(Square::State::PossibleMove);
    }

    void Board::flip() {
        this->originCorner = (this->originCorner == Qt::BottomLeftCorner) ? Qt::TopRightCorner
                                                                          : Qt::BottomLeftCorner;
        createLayout();
    }

    void Board::setPixelSize(int newPixelSize) {
        if (newPixelSize <= 0 || newPixelSize == this->pixelSize)
            return;

        this->pixelSize = newPixelSize;

        setFixedSize(this->pixelSize, this->pixelSize);
    }
}

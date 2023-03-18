#pragma once

#include "bitboard.h"

#include <optional>
#include <cassert>
#include <ostream>

namespace Chess {

    enum class Castling {
        None = 0,
        WhiteKing = 1,
        WhiteQueen = 2,
        BlackKing = 3,
        BlackQueen = 4,
    };

    struct Move {
        Move(Square from, Square to, bool promotion = false)
                : from(from),
                  to(to),
                  promotion(promotion) {
            assert(from != Square::None);
            assert(to != Square::None);
        }

        Move(Square from, Square to, std::optional<PieceType> dropPiece, bool promotion)
                : from(from),
                  to(to),
                  dropPiece(dropPiece),
                  promotion(promotion) {
            dropSquare = to;
            assert(from != Square::None);
            assert(to != Square::None);
        }

        Move(Square from, Square to, Square enPassant)
                : from(from),
                  to(to),
                  enPassant(enPassant) {
            assert(from != Square::None);
            assert(to != Square::None);
        }

        Move(Square from, Square to, Castling castle)
                : from(from),
                  to(to),
                  castle(castle) {
            assert(from != Square::None);
            assert(to != Square::None);
            assert(castle != Castling::None);
        }

        Move(Square from, Square to, bool enPassantCapture, Square dropSquare)
                : from(from),
                  to(to),
                  enPassantCapture(enPassantCapture),
                  dropSquare(dropSquare) {
            dropPiece = PieceType::Pawn;
            assert(from != Square::None);
            assert(to != Square::None);
        }

        Square from;
        Square to;
        std::optional<PieceType> dropPiece{std::nullopt};
        Square dropSquare{Square::None};
        Castling castle{Castling::None};
        Square enPassant{Square::None};
        bool enPassantCapture{false};
        bool promotion{false};

        bool operator==(const Move &rhs) const {
            return from == rhs.from &&
                   to == rhs.to &&
                   dropPiece == rhs.dropPiece;
        }

        bool operator!=(const Move &rhs) const {
            return !(rhs == *this);
        }

        friend std::ostream &operator<<(std::ostream &os, const Move &move) {
            os << move.from << "-->" << move.to;
            if (move.dropPiece)
                os << '(' << *move.dropPiece << ')';
            return os;
        }
    };
}

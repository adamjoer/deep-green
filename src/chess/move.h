#pragma once

#include "bitboard.h"

#include <optional>
#include <cassert>
#include <ostream>

namespace Chess {

    struct Move {
        Move(Square from, Square to)
                : from(from),
                  to(to) {
            assert(from != Square::None);
            assert(to != Square::None);
        }

        Move(Square from, Square to, std::optional<PieceType> dropPiece)
                : from(from),
                  to(to),
                  dropPiece(dropPiece) {
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

        Move(Square from, Square to, int castle)
                : from(from),
                  to(to),
                  castle(castle) {
            assert(from != Square::None);
            assert(to != Square::None);
        }

        Move(Square from, Square to, bool enPassantCapture, Square dropSquare)
                : from(from),
                  to(to),
                  enPassantCapture(enPassantCapture),
                  dropSquare(dropSquare) {
            assert(from != Square::None);
            assert(to != Square::None);
        }

        Square from;
        Square to;
        std::optional<PieceType> dropPiece{std::nullopt};
        std::optional<Square> dropSquare{std::nullopt};
        int castle{0};
        Square enPassant{Square::None};
        bool enPassantCapture{false};

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

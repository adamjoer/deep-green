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

        const Square from;
        const Square to;
        const std::optional<PieceType> dropPiece{std::nullopt};

        friend std::ostream &operator<<(std::ostream &os, const Move &move) {
            os << move.from << "-->" << move.to;
            if (move.dropPiece)
                os << '(' << *move.dropPiece << ')';
            return os;
        }
    };
}

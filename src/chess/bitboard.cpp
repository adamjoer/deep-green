#include "bitboard.h"

#include <iostream>

namespace Chess {

    Square Bitboard::squareToThe(Direction direction, Square square) {
        // TODO
        return Square::None;
    }

    std::ostream &operator<<(std::ostream &os, const Bitboard &bitboard) {
        os << "Bitboard: " << bitboard.bits << '\n';

        for (int rank = 7; rank >= 0; --rank) {
            for (int file = 0; file < 8; ++file) {

                if (file == 0)
                    os << ' ' << rank + 1 << '\t';

                auto square = Square(rank * 8 + file);
                os << (bitboard.isOccupiedAt(square) ? '*' : '.') << ' ';
            }

            os << '\n';
        }
        os << "\n\ta b c d e f g h";

        return os;
    }
}

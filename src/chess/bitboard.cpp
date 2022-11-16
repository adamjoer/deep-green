#include "bitboard.h"

#include <iostream>

namespace Chess {
    bool Bitboard::isOccupiedAt(Chess::Square square) const {
        return this->bits & (1ULL << static_cast<int>(square));
    }

    void Bitboard::setOccupancyAt(Chess::Square square) {
        this->bits |= (1ULL << static_cast<int>(square));
    }

    void Bitboard::print() const {

        std::cout << "Bitboard: " << this->bits << '\n';

        // loop over board ranks
        for (int rank = 7; rank >= 0; --rank) {

            // loop over board files
            for (int file = 0; file < 8; ++file) {

                // convert file & rank into square index
                int square = rank * 8 + file;

                if (!file)
                    std::cout << ' ' << rank + 1 << '\t';

                std::cout << (isOccupiedAt(static_cast<Square>(square)) ? 1 : 0) << ' ';

            }

            // print new line every rank
            std::cout << '\n';
        }

        // print board files
        std::cout << "\n\ta b c d e f g h\n";

    }
}

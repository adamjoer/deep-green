#include "bitboard.h"

#include <iostream>

namespace Chess {

    /*
     * Helper bitboards that can be used to detect pieces on the edge of the board.
     */
    static constexpr Bitboard aFile{Square::A1, Square::A2, Square::A3, Square::A4,
                                    Square::A5, Square::A6, Square::A7, Square::A8};
    static constexpr Bitboard hFile{Square::H1, Square::H2, Square::H3, Square::H4,
                                    Square::H5, Square::H6, Square::H7, Square::H8};
    static constexpr Bitboard oneRank{Square::A1, Square::B1, Square::C1, Square::D1,
                                      Square::E1, Square::F1, Square::G1, Square::H1};
    static constexpr Bitboard eightRank{Square::A8, Square::B8, Square::C8, Square::D8,
                                        Square::E8, Square::F8, Square::G8, Square::H8};

    Square Bitboard::squareToThe(Direction direction, Square square) {
        if (square == Square::None)
            return Square::None;

        const Bitboard squareBitboard(square);
        const auto squareIndex = static_cast<int>(square);

        Bitboard edgeBitboard;
        int offset;

        // Calculate edge bitboards and square index offset based on the direction.
        // The offset will be decided based on the following rules:
        // NW          N          NE
        //     +7     +8     +9
        //          \  |  /
        //  W  -1  <-  0  -> +1   E
        //          /  |  \
        //     -9     -8     -7
        // SW          S          SE
        // Source: https://www.chessprogramming.org/Classical_Approach#Ray_Attacks
        switch (direction) {
            case Direction::North:
                edgeBitboard = eightRank;
                offset = 8;
                break;

            case Direction::NorthEast:
                edgeBitboard = eightRank | hFile;
                offset = 9;
                break;

            case Direction::East:
                edgeBitboard = hFile;
                offset = 1;
                break;

            case Direction::SouthEast:
                edgeBitboard = oneRank | hFile;
                offset = -7;
                break;

            case Direction::South:
                edgeBitboard = oneRank;
                offset = -8;
                break;

            case Direction::SouthWest:
                edgeBitboard = oneRank | aFile;
                offset = -9;
                break;

            case Direction::West:
                edgeBitboard = aFile;
                offset = -1;
                break;

            case Direction::NorthWest:
                edgeBitboard = aFile | eightRank;
                offset = 7;
                break;

            default:
                assert(false);
                break;
        }

        if (squareBitboard.isOverlappingWith(edgeBitboard))
            return Square::None;
        return Square(squareIndex + offset);
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

    std::ostream &operator<<(std::ostream &os, Square square) {
        if (square == Square::None) {
            os << "None";
            return os;
        }

        static const char *const names[64]{
            "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
            "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
            "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
            "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
            "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
            "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
            "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
            "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
        };

        return os << names[static_cast<int>(square)];
    }
}
